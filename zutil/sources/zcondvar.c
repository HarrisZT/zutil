/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zcondvar.c 
* Desc: thread condition variable routines
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
#include "zutil/zcondvar.h" 
 
#if (Z_PLATFORM_WINDOWS)
#  include <process.h>
#  include <sys/timeb.h>
#else
#  include <signal.h>
#  include <sched.h>
#  include <unistd.h>
#  include <sys/time.h> 
#endif  
#if defined(__cplusplus)
extern "C" {
#endif

 



ZRESULT 
ZCondVar_Init(
	_Inout_ ZCondVar* _lpcond) {

#if (Z_PLATFORM_WINDOWS) 
	InitializeCriticalSection(&_lpcond->critSection);

	_lpcond->pending = 0;
	_lpcond->hEvents[0] =
		CreateEvent(NULL, FALSE, FALSE, NULL);

	if (_lpcond->hEvents[0] == NULL) {
		_lpcond->hEvents[1] = NULL;
		return Z_EFAIL;
	}
	_lpcond->hEvents[1] =
		CreateEvent(NULL, TRUE, FALSE, NULL);

	if (_lpcond->hEvents[1] == NULL) {
		CloseHandle(_lpcond->hEvents[0]);
		_lpcond->hEvents[0] = NULL;
		return Z_EFAIL;
	}
	return Z_OK;
#else
	return pthread_cond_init(_lpcond, NULL) == 0 ? Z_OK : Z_EFAIL;
#endif
}


Void
ZCondVar_Release(
	_Inout_ ZCondVar* _lpcond) {

#if (Z_PLATFORM_WINDOWS)
	if (_lpcond->hEvents[0] != NULL) {
		CloseHandle(_lpcond->hEvents[0]);
	}
	if (_lpcond->hEvents[1] != NULL) {
		CloseHandle(_lpcond->hEvents[1]);
	}
	DeleteCriticalSection(&_lpcond->critSection);
#else
	pthread_cond_destroy(_lpcond);
#endif
}


ZRESULT 
ZCondVar_PostSignal(
	_Inout_ ZCondVar* _lpcond) {

#if (Z_PLATFORM_WINDOWS)
	Bool pending;
	/*
	Are there any waiters? */
	EnterCriticalSection(&_lpcond->critSection);
	pending = (_lpcond->pending > 0);
	LeaveCriticalSection(&_lpcond->critSection);
	/*
	If we have any waiting threads, send them a signal */
	if (pending) {
		if (SetEvent(_lpcond->hEvents[0]) == 0)
			return Z_EFAIL;
	}
	return Z_OK;
#else
	return pthread_cond_signal(_lpcond) == 0 ? Z_OK : Z_EFAIL;
#endif
}


ZRESULT 
ZCondVar_Broadcast(
	_Inout_ ZCondVar* _lpcond) {

#if (Z_PLATFORM_WINDOWS)
	Bool pending;
	/*
	Are there any waiters? */
	EnterCriticalSection(&_lpcond->critSection);
	pending = (_lpcond->pending > 0);
	LeaveCriticalSection(&_lpcond->critSection);
	/*
	If we have any waiting threads, send them a signal */
	if (pending) {
		if (SetEvent(_lpcond->hEvents[1]) == 0)
			return Z_EFAIL;
	}
	return Z_OK;
#else
	return pthread_cond_broadcast(_lpcond) == 0 ? Z_OK : Z_EFAIL;
#endif
}



#if (Z_PLATFORM_WINDOWS)
static ZRESULT 
ZCondVar_TimedWaitWin32(
	_Inout_ ZCondVar* _lpcond,
	_Inout_ ZMutex*   _lpmutex,
	_In_    DWORD     _dwtimeout) {

	Dword dwResult;
	Int32 iLastWaiter;
	/*
	Increment number of waiters */
	EnterCriticalSection(&_lpcond->critSection);
	++_lpcond->pending;
	LeaveCriticalSection(&_lpcond->critSection);
	/*
	Release the mutex while waiting for the condition (will decrease
	the number of waiters when done)... */
	ZMutex_Unlock(_lpmutex);
	/*
	Wait for either event to become signaled due to cnd_signal() or
	cnd_broadcast() being called */
	dwResult =
		WaitForMultipleObjects(2, _lpcond->hEvents, FALSE, _dwtimeout);

	if (dwResult == WAIT_TIMEOUT) {
		ZMutex_Lock(_lpmutex);
		return Z_ETIMEDOUT;
	}
	else if (dwResult == WAIT_FAILED) {
		ZMutex_Lock(_lpmutex);
		return Z_EFAIL;
	}
	/*
	Check if we are the last waiter */
	EnterCriticalSection(&_lpcond->critSection);
	--_lpcond->pending;
	iLastWaiter =
		(dwResult == (WAIT_OBJECT_0 + 1)) &&
		(_lpcond->pending == 0);

	LeaveCriticalSection(&_lpcond->critSection);
	/*
	If we are the last waiter to be notified
	to stop waiting, reset the event */
	if (iLastWaiter) {
		if (ResetEvent(_lpcond->hEvents[1]) == 0) {
			/*
			The mutex is locked again before the function returns,
			even if an error occurred */
			ZMutex_Lock(_lpmutex);
			return Z_EFAIL;
		}
	}
	ZMutex_Lock(_lpmutex);
	return Z_OK;
}
#endif

	 

ZRESULT ZCondVar_Wait(
	_Inout_  ZCondVar*              _lpcond,
	_Inout_  ZMutex*                _lpmutex,
	_In_opt_ const struct timespec* _cptimespec)
{

	ZRESULT hResult = Z_EFAIL;

#if (Z_PLATFORM_WINDOWS)
	struct timespec now;
	Uint64          nowMs, tsMs;
	Dword           dwDelta;

	if (_cptimespec == NULL)
	{
		hResult = ZCondVar_TimedWaitWin32(_lpcond, _lpmutex, INFINITE);
	}
	else if (timespec_get(&now, TIME_UTC) == TIME_UTC) {
		nowMs = now.tv_sec * 1000 + now.tv_nsec / 0xF4240;
		tsMs = _cptimespec->tv_sec * 1000 + _cptimespec->tv_nsec / 0xF4240;
		dwDelta = (tsMs > nowMs) ? (Dword)(tsMs - nowMs) : 0;
		hResult = ZCondVar_TimedWaitWin32(_lpcond, _lpmutex, dwDelta);
	}
#else 
	if (_cptimespec == NULL)
		hResult = pthread_cond_wait(_lpcond, _lpmutex);
	else {
		hResult = pthread_cond_timedwait(_lpcond, _lpmutex, _cptimespec);
		if (hResult == ETIMEDOUT)
			hResult = Z_ETIMEDOUT;
	}
	if (hResult != Z_OK)
		hResult = Z_EFAIL;
#endif
	return hResult;
}



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
//EOF
/*****************************************************************************/  