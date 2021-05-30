/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zsemaphore.c 
* Desc: semaphore routines, an adaptation of POSIX semaphore.h
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
#include "zutil/zplatform.h"

#if (Z_PLATFORM_WINDOWS)
#  include <winsock.h>  
#  include <fcntl.h>  
#else
#  include <semaphore.h>
#endif
#include "zutil/zsemaphore.h" 



/*
** Section I:
** Preprocessor defs:
******************************************************************************/
#if !defined(_POSIX_SEMAPHORES)
#  define _POSIX_SEMAPHORES 200809L //POSIX.1b
#endif
#if !defined(PTHREAD_PROCESS_SHARED)
#  define PTHREAD_PROCESS_PRIVATE 0
#  define PTHREAD_PROCESS_SHARED  1
#endif 
#if !defined(SEM_VALUE_MAX)
#  define SEM_VALUE_MAX Z_INT32_MAX
#endif 
#if !defined(SEM_FAILED)
#  define SEM_FAILED NULL
#endif 
#if !defined(ETIMEDOUT)
#  define ETIMEDOUT 138 /*VC 2010*/
#endif





/*
** Section II:
** Win32-specific details:
******************************************************************************/
#if (Z_PLATFORM_WINDOWS)
   /*
   Time conversion function*/ 
#  define INT64_C(x)\
     ((x) + (Z_INT64_MAX - Z_INT64_MAX))
   /* 
   Number of 100ns-seconds between the beginning of the 
   Windows epoch (Jan. 1, 1601) and the Unix epoch (Jan. 1, 1970)*/
#  define DELTA_EPOCH_IN_100NS\
     INT64_C(0x19DB1DED53E8000) 


    typedef struct {
    	Handle handle;
    } arch_sem_t;
 

   static Uint32 
   arch_rel_time_in_ms(
       _In_ const struct timespec* _ts) {

	   FILETIME filetime;
	   Int64    t0, t1, t2, iUnixTimeIn100ns;

	   t1 = _ts->tv_sec * INT64_C(0x3E8) +
		    _ts->tv_nsec / INT64_C(0xF4240);
	    
	   GetSystemTimeAsFileTime(&filetime);

	   iUnixTimeIn100ns =
		   ((((Int64)filetime.dwHighDateTime) << 0x20 |
			   filetime.dwLowDateTime) - DELTA_EPOCH_IN_100NS);

	   t2 = iUnixTimeIn100ns / INT64_C(0x2710);
	   t0 = t1 - t2;

	   if (t0 < 0 || t0 >= INT64_C(0xFFFFFFFF))
		   return 0;
	   return (Uint32)t0;
   }
#endif





/*
** Section III:
** Interface Functions:
******************************************************************************/ 

ZRESULT 
ZSemaphore_Init(
	_Inout_ ZSemaphore* _lpSem,
	_In_    Int32         _shared,
	_In_    Uint32        _uValue) {

	ZRESULT hResult;

#if (Z_PLATFORM_WINDOWS)
	Char        cBuffer[24] = { '\0' };
	arch_sem_t* archsem;

	if (_lpSem == NULL || _uValue > (Uint32)SEM_VALUE_MAX) {
		hResult = ZResult_SetErrno(EINVAL);
		return hResult;
	}
	archsem = (arch_sem_t*)calloc(1, sizeof(arch_sem_t));
	if (archsem == NULL) {
		hResult = ZResult_SetErrno(ENOMEM);
		return hResult;
	} 
	if (_shared != PTHREAD_PROCESS_PRIVATE) {
		sprintf(cBuffer, "Global\\%p", archsem);
	}
	archsem->handle =
		CreateSemaphore(NULL, _uValue, SEM_VALUE_MAX, cBuffer);

	if (archsem->handle == NULL) {
		free(archsem);
		hResult = ZResult_SetErrno(ENOSPC);
		return hResult;
	}
	*_lpSem = archsem;
	hResult = Z_OK;
#else
	hResult = sem_init(_lpSem, _shared, _uValue);
	hResult = ZResult_FromErrno(hResult);
#endif
	return hResult;
}


ZRESULT
ZSemaphore_Wait(
	_Inout_ ZSemaphore* _lpSem) {

	ZRESULT hResult;
	hResult = Z_OK;

#if (Z_PLATFORM_WINDOWS)
	arch_sem_t* archsem;
	archsem = (arch_sem_t*)_lpSem;

	if (_lpSem == NULL || archsem == NULL) {
		hResult = ZResult_SetErrno(EINVAL);
		return hResult;
	}
	if (WaitForSingleObject(archsem->handle, INFINITE) != WAIT_OBJECT_0) {
		hResult = ZResult_SetErrno(EINVAL);
		return hResult;
	}
	hResult = Z_OK;
#else 
	hResult = sem_wait(_lpSem);
	hResult = ZResult_FromErrno(hResult);
#endif
	return hResult;
}


ZRESULT 
ZSemaphore_TryWait(
	_Inout_ ZSemaphore* _lpSem) {

	ZRESULT hResult;
	hResult = Z_OK;

#if (Z_PLATFORM_WINDOWS) 
	arch_sem_t* archsem;
	archsem = (arch_sem_t *)_lpSem;

	if (_lpSem == NULL || archsem == NULL) {
		hResult = ZResult_SetErrno(EINVAL);
		return hResult;
	}
	switch (WaitForSingleObject(archsem->handle, 0)) {
	case WAIT_OBJECT_0:
		hResult = Z_OK;
		break;
	case WAIT_TIMEOUT:
		hResult = ZResult_SetErrno(EAGAIN);
		break;
	default:
		hResult = ZResult_SetErrno(EINVAL);
		break;
	}
#else
	hResult = sem_trywait(_lpSem);
	hResult = ZResult_FromErrno(hResult);
#endif
	return hResult;
}


ZRESULT 
ZSemaphore_TimedWait(
	_Inout_ ZSemaphore*            _lpSem,
	_In_    const struct timespec* _lpAbsTimeout) {

	ZRESULT hResult;
	hResult = Z_OK;

#if (Z_PLATFORM_WINDOWS)
	Uint32 msTime;
	arch_sem_t* pv;

	pv = (arch_sem_t*)_lpSem;

	if (_lpSem == NULL || pv == NULL) {
		return ZResult_SetErrno(EINVAL);
	}
	msTime = arch_rel_time_in_ms(_lpAbsTimeout);

	switch (WaitForSingleObject(pv->handle, msTime)) {
	case WAIT_OBJECT_0:
		hResult = Z_OK;
		break;
	case WAIT_TIMEOUT:
		hResult = ZResult_SetErrno(ETIMEDOUT);
		break;
	default:
		hResult = ZResult_SetErrno(EINVAL);
		break;
	}
#else
	hResult = sem_timedwait(_lpSem, _lpAbsTimeout);
	hResult = ZResult_FromErrno(hResult);
#endif
	return hResult;
}


ZRESULT 
ZSemaphore_Post(
	_Inout_ ZSemaphore* _lpSem) {

	ZRESULT hResult;
	hResult = Z_OK;

#if (Z_PLATFORM_WINDOWS)
	arch_sem_t* pv;
	pv = (arch_sem_t*)_lpSem;

	if (_lpSem == NULL || pv == NULL) {
		hResult = ZResult_SetErrno(EINVAL);
		return hResult;
	}
	if (!ReleaseSemaphore(pv->handle, 1, NULL)) {
		hResult = ZResult_SetErrno(EINVAL);
	}
	else hResult = Z_OK;
#else
	hResult = sem_post(_lpSem);
	hResult = ZResult_FromErrno(hResult);
#endif
	return hResult;
}


ZRESULT
ZSemaphore_GetValue(
	_Inout_ ZSemaphore* _lpSem,
	_Inout_ Int32*      _lpValue) {

	ZRESULT hResult;
	hResult = Z_OK;

#if (Z_PLATFORM_WINDOWS)
	Long previous;
	arch_sem_t* pv;

	pv = (arch_sem_t*)_lpSem;

	switch (WaitForSingleObject(pv->handle, 0)) {
	case WAIT_OBJECT_0:
		if (!ReleaseSemaphore(pv->handle, 1, &previous))
			hResult = ZResult_SetErrno(EINVAL);
		else {
			*_lpValue = previous + 1;
			hResult = Z_OK;
		}
		break;

	case WAIT_TIMEOUT:
		*_lpValue = 0;
		hResult = Z_OK;
		break;

	default:
		hResult = ZResult_SetErrno(EINVAL);
		break;
	}
#else
	hResult = sem_getvalue(_lpSem);
	hResult = ZResult_FromErrno(hResult);
#endif
	return hResult;
}


ZRESULT
ZSemaphore_Release(
	_Inout_ ZSemaphore* _lpSem) {

	ZRESULT hResult;
	hResult = Z_OK;

#if (Z_PLATFORM_WINDOWS)
	arch_sem_t* pv;
	pv = (arch_sem_t*)_lpSem;

	if (!pv || !CloseHandle(pv->handle)) {
		hResult = ZResult_SetErrno(EINVAL);
		return hResult;
	}
	free(pv);
	*_lpSem = NULL;
	hResult = Z_OK;
#else
	hResult = sem_destroy(_lpSem);
	hResult = ZResult_FromErrno(hResult);
#endif
	return hResult;
}


ZSemaphore* 
ZSemaphore_Open(
	_In_ Lpcstr _name,
	_In_ Int32  _flag,
	_In_ Uint16 _mode,
	_In_ Uint32 _value) {

	ZSemaphore* semout;

#if (Z_PLATFORM_WINDOWS)
	Int32       len;
	Char        buffer[512];
	arch_sem_t* pv;

	Z_Unused(_mode);
	len = strlen(_name);

	if (_value > (Uint32)SEM_VALUE_MAX ||
		len > (Int32)sizeof(buffer) - 8 || len < 1) {
		ZResult_SetErrno(EINVAL);
		return NULL;
	}
	pv = (arch_sem_t*)calloc(1, sizeof(arch_sem_t));
	if (pv == NULL) {
		ZResult_SetErrno(ENOMEM);
		return NULL;
	}
	memmove(buffer, "Global\\", 7);
	memmove(buffer + 7, _name, len);
	buffer[len + 7] = '\0';

	pv->handle =
		CreateSemaphore(NULL, _value, SEM_VALUE_MAX, buffer);

	if (pv->handle == NULL) {
		switch (GetLastError()) {
		case ERROR_ACCESS_DENIED:
			ZResult_SetErrno(EACCES);
			break;
		case ERROR_INVALID_HANDLE:
			ZResult_SetErrno(ENOENT);
			break;
		default:
			ZResult_SetErrno(ENOSPC);
			break;
		}
		free(pv);
		return NULL;
	}
	else {
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			if ((_flag & O_CREAT) && (_flag & O_EXCL)) {
				CloseHandle(pv->handle);
				free(pv);
				ZResult_SetErrno(EEXIST);
				return NULL;
			}
			return (ZSemaphore*)pv;
		}
		else if (!(_flag & O_CREAT)) {
			free(pv);
			ZResult_SetErrno(ENOENT);
			return NULL;
		}
	}
	semout = (ZSemaphore*)pv;
#else
	semout = (ZSemaphore*)sem_open(_name, _flag, _mode, _value);
#endif
	return semout;
}


ZRESULT
ZSemaphore_Close(
	_Inout_ ZSemaphore* _lpSem) {

	ZRESULT hResult;

#if (Z_PLATFORM_WINDOWS)
	hResult = ZSemaphore_Release(_lpSem);
#else
	hResult = sem_destroy(_lpSem);
	hResult = ZResult_FromErrno(hResult);
#endif
	return hResult;
}


ZRESULT 
ZSemaphore_Unlink(
	_In_ Lpcstr _name) {

	ZRESULT hResult;

#if (Z_PLATFORM_WINDOWS)
	Z_Unused(_name);
	hResult = Z_OK;
#else
	hResult = sem_unlink(_name);
	hResult = ZResult_FromErrno(hResult);
#endif
	return hResult;
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/ 