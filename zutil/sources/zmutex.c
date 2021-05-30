/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zmutex.c
* Desc: blocks concurrent access to shared resources from multiple threads
*******************************************************************************


This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
******************************************************************************/ 
#include "zutil/zlog.h" 

#if (Z_PLATFORM_WINDOWS)
#  include "zutil/zplatform_win32.h"
#else
#  include "zutil/zplatform_posix.h"
#endif 
#include "zutil/zmutex.h"





/*
A structure that contains platform-specific thread data
******************************************************************************/
struct _ZMutex {
	Int32 type;
#if (Z_PLATFORM_WINDOWS)
	Bool isLocked;    /*true if the mutex is locked*/
	Bool isRecursive; /*true if the mutex is recursive*/
	Bool isTimed;     /*true if the mutex is timed*/
	union {
		CRITICAL_SECTION cs;   /*(critical section - non-timed mutex)*/
		HANDLE           data; /*(handle to data   - for timed mutex) */
	} handle;                  /* data union */
#else
	pthread_mutex_t handle;
#endif 
};





/*
** Section I:
** Win32-specific mutex implementation:
******************************************************************************/ 
#if (Z_PLATFORM_WINDOWS) 


static Void
ZMutexWin32_SimulateDeadlock(
	_Inout_ ZMutex* _lpMutex) {

	while (_lpMutex->isLocked)
		Sleep(1); /* Simulate deadlock... */
	_lpMutex->isLocked = Z_TRUE;
}


static ZRESULT 
ZMutexWin32_TimedLock(
	_Inout_ ZMutex*                _lpMutex,
	_In_    const struct timespec* _lpTimeSpec) {

	struct timespec tsCurrent;
	Dword           dwTimeoutMs;

	if (!_lpMutex->isTimed) {
		//the mutex is not a timed one, so ignore it and exit
		return Z_OK;
	}
	timespec_get(&tsCurrent, TIME_UTC);

	if ((tsCurrent.tv_sec > _lpTimeSpec->tv_sec) ||
		((tsCurrent.tv_sec == _lpTimeSpec->tv_sec) &&
		(tsCurrent.tv_nsec >= _lpTimeSpec->tv_nsec))) {
		dwTimeoutMs = 0;
	}
	else {
		dwTimeoutMs = (Dword)(_lpTimeSpec->tv_sec - tsCurrent.tv_sec) * 1000;
		dwTimeoutMs += (_lpTimeSpec->tv_nsec - tsCurrent.tv_nsec) / 1000000;
		dwTimeoutMs += 1;
	}
	/*TODO:
	WaitForSingleObject timeout does not include time while asleep.*/
	switch (WaitForSingleObject(_lpMutex->handle.data, dwTimeoutMs)) {
	case WAIT_OBJECT_0:
		break;
	case WAIT_TIMEOUT:
		return Z_ETIMEDOUT;
	case WAIT_ABANDONED:
	default:
		return Z_EFAIL;
	}
	if (!_lpMutex->isRecursive)
		ZMutexWin32_SimulateDeadlock(_lpMutex);
	return Z_OK;
}


static ZRESULT 
ZMutexWin32_TryLock(
	_Inout_ ZMutex* _lpMutex) {

	ZRESULT hResult;
	if (!_lpMutex->isTimed) {
		hResult =
			TryEnterCriticalSection(&(_lpMutex->handle.cs)) ?
			Z_OK : Z_EBUSY;
	}
	else {
		hResult = (
			WaitForSingleObject(_lpMutex->handle.data, 0) == WAIT_OBJECT_0) ?
			Z_OK : Z_EBUSY;
	}
	if ((!_lpMutex->isRecursive) && (hResult == Z_OK)) {
		if (_lpMutex->isLocked) {
			LeaveCriticalSection(&(_lpMutex->handle.cs));
			hResult = Z_EBUSY;
		}
		else _lpMutex->isLocked = Z_TRUE;
	}
	return hResult;
}


static ZRESULT 
ZMutexWin32_Create(
	_Inout_ ZMutex* _lpMutex,
	_In_    Int32   _eType) {

	_lpMutex->isLocked    = Z_FALSE;
	_lpMutex->type        = _eType;
	_lpMutex->isRecursive = _eType & Z_MUTEX_RECURSIVE;
	_lpMutex->isTimed     = _eType & Z_MUTEX_TIMED;

	if (!_lpMutex->isTimed)
		InitializeCriticalSection(&(_lpMutex->handle.cs));
	else {
		_lpMutex->handle.data = CreateMutex(NULL, Z_FALSE, NULL);
		if (_lpMutex->handle.data == NULL) {
			ZLogError("failed to create a timed mutex");
			Z_EFAIL;
		}
	}
	return Z_OK;
}


ZRESULT 
ZMutexWin32_Lock(
	_Inout_  ZMutex*                _lpMutex,
	_In_opt_ const struct timespec* _lpTimeSpec,
	_In_     Bool                   _bIsTryLock) {

	if (_lpTimeSpec != NULL) {
		return ZMutexWin32_TimedLock(_lpMutex, _lpTimeSpec);
	}
	if (_bIsTryLock) {
		return ZMutexWin32_TryLock(_lpMutex);
	}
	if (!_lpMutex->isTimed)
		EnterCriticalSection(&(_lpMutex->handle.cs));
	else {
		switch (WaitForSingleObject(_lpMutex->handle.data, INFINITE)) {
		case WAIT_OBJECT_0:
			break;
		case WAIT_ABANDONED:
		default:
			return Z_EFAIL;
		}
	}
	if (!_lpMutex->isRecursive)
		ZMutexWin32_SimulateDeadlock(_lpMutex);
	return Z_OK;
}


ZRESULT 
ZMutexWin32_Unlock(
	_Inout_ ZMutex* _lpMutex) {

	_lpMutex->isLocked = Z_FALSE;
	if (!_lpMutex->isTimed)
		LeaveCriticalSection(&(_lpMutex->handle.cs));
	else if (!ReleaseMutex(_lpMutex->handle.data))
		return Z_EFAIL;
	return Z_OK;
}
#endif





/*
** Section II:
** POSIX-specific mutex implementation:
******************************************************************************/ 
#if (Z_PLATFORM_POSIX)  


static ZRESULT 
ZMutexPosix_TimedLock(
	_Inout_ ZMutex*          _lpMutex,
	_In_    const ZTimeSpec* _lpTimeSpec) {

	ZRESULT hResult;

#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >= 0x30DB0L) &&\
    defined(_POSIX_THREADS)  && (_POSIX_THREADS >= 0x30DB0L)

	hResult = pthread_mutex_timedlock(_lpMutex->handle, _lpTimeSpec);
	hResult = Z_GetResultFromErrno(hResult);
#else 
	struct timespec cur, dur;
	/*
	Try to acquire the lock and sleep for 5ms on failure. */
	while ((hResult = pthread_mutex_trylock(_lpMutex->handle)) == EBUSY) {
		timespec_get(&cur, TIME_UTC);

		if ((cur.tv_sec > ts->tv_sec) ||
			((cur.tv_sec == ts->tv_sec) &&
			(cur.tv_nsec >= ts->tv_nsec))) {
			break;
		}
		dur.tv_sec = ts->tv_sec - cur.tv_sec;
		dur.tv_nsec = ts->tv_nsec - cur.tv_nsec;
		if (dur.tv_nsec < 0) {
			dur.tv_sec--;
			dur.tv_nsec += 0x3B9ACA00;
		}
		if ((dur.tv_sec != 0) || (dur.tv_nsec > 0x4C4B40)) {
			dur.tv_sec = 0;
			dur.tv_nsec = 0x4C4B40;
		}
		nanosleep(&dur, NULL);
	}
	hResult = Z_GetResultFromErrno(hResult);
	return hResult;
#endif
}


static ZRESULT 
ZMutexPosix_Create(
	_Inout_ ZMutex* _lpMutex,
	_In_    Int32   _eType) {

	ZRESULT hResult;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);

	if (_eType & Z_MUTEX_RECURSIVE) {
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	}
	_lpMutex->type = _eType;

	hResult = pthread_mutex_init(_lpMutex->handle, &attr);
	hResult = Z_GetResultFromErrno(hResult);
	pthread_mutexattr_destroy(&attr);
	return hResult;
}


static ZRESULT 
ZMutexPosix_Lock(
	_Inout_  ZMutex*          _lpMutex,
	_In_opt_ const ZTimeSpec* _lpTimeSpec,
	_In_     Bool             _isTryLock) {

	ZRESULT hResult;
	if (_lpTimeSpec != NULL)
		hResult = ZMutexPosix_TimedLock(_lpMutex, _lpTimeSpec);
	else {
		if (_isTryLock)
			hResult = pthread_mutex_trylock(_lpMutex->handle);
		else hResult = pthread_mutex_lock(_lpMutex->handle);
	}
	hResult = Z_GetResultFromErrno(hResult);
	return hResult;
}


static ZRESULT 
ZMutexPosix_Unlock(
	_Inout_ ZMutex* _lpMutex) {

	ZRESULT hResult;
	hResult = pthread_mutex_unlock(_lpMutex->handle);
	hResult = Z_GetResultFromErrno(hResult);
	return hResult;
}
#endif





/*
** Section III:
** Interface implementation:
******************************************************************************/ 
ZMutex* 
ZMutex_CreateEx(
	_In_        ZMUTEXTYPE _eType,
	_Inout_opt_ ZRESULT*   _lpResult) {

	ZRESULT hResult;
	ZMutex* mutex;

	mutex = (ZMutex*)malloc(sizeof(struct _ZMutex));
	if (!mutex)
		hResult = Z_EOUTOFMEMORY;
	else {
	#if (Z_PLATFORM_WINDOWS)
		hResult = ZMutexWin32_Create(mutex, _eType);
	#else 
		hResult = ZMutexPosix_Create(mutex, _eType);
	#endif
	}
	if (_lpResult)
		*_lpResult = hResult;
	return mutex;
}


Int32 
ZMutex_GetType(
	_In_ const ZMutex* _lpMutex) {

	Int32 eType;
	eType = _lpMutex ? _lpMutex->type : Z_UNDEFINED;
	return eType;
}


ZRESULT 
ZMutex_LockEx(
	_Inout_  ZMutex*                _lpMutex,
	_In_opt_ const struct timespec* _lpTimeSpec) {

	ZRESULT hResult;
#if (Z_PLATFORM_WINDOWS)
	hResult = ZMutexWin32_Lock(_lpMutex, _lpTimeSpec, Z_FALSE);
#else
	hResult = ZMutexPosix_Lock(_lpMutex, _lpTimeSpec, Z_FALSE);
#endif
	return hResult;
}


ZRESULT 
ZMutex_TryLock(
	_Inout_ ZMutex* _lpMutex) {

	ZRESULT hResult;
#if (Z_PLATFORM_WINDOWS)
	hResult = ZMutexWin32_Lock(_lpMutex, NULL, Z_TRUE);
#else
	hResult = ZMutexPosix_Lock(_lpMutex, NULL, Z_TRUE);
#endif
	return hResult;
}


ZRESULT 
ZMutex_Unlock(
	_Inout_ ZMutex* _lpMutex) {

	ZRESULT hResult;
#if (Z_PLATFORM_WINDOWS)
	hResult = ZMutexWin32_Unlock(_lpMutex);
#else
	hResult = ZMutexPosix_Unlock(_lpMutex);
#endif
	return hResult;
}


ZRESULT 
ZMutex_Release(
	_Inout_ ZMutex* _lpMutex) {

	ZRESULT hResult;

	if (!_lpMutex)
		hResult = Z_EPOINTER;
	else {
	#if (Z_PLATFORM_WINDOWS)
		if (!_lpMutex->isTimed)
			DeleteCriticalSection(&(_lpMutex->handle.cs));
		else CloseHandle(_lpMutex->handle.data);
	#else
		pthread_mutex_destroy(_lpMutex->handle);
	#endif
		free(_lpMutex);
		_lpMutex = NULL;
		hResult = Z_OK;
	}
	return hResult;
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/ 