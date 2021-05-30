/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zsemaphore.h 
* Desc: semaphore routines, an adaptation of POSIX semaphore.h
* Info: for additional resource information see:
*    https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html
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
#ifndef __ZSEMAPHORE_H__
#define __ZSEMAPHORE_H__ 

#include "zutil/zmutex.h"
#if defined(__cplusplus)
extern "C" {
#endif
 


/*
A semaphore data object (Handle)*/
typedef Handle ZSemaphore;

/* 
Create a semaphore (unnamed).
@_lpSem : a pointer to the semaphore 
@_shared: argument indicating whether the semaphore
          is to be shared between the threads 
		  (0 or PTHREAD_PROCESS_PRIVATE) of a process, or 
		   between processes (PTHREAD_PROCESS_SHARED).
@_uValue: argument specifying the initial value for the semaphore.
@return : Z_OK on success, non-zero on failure.*/
extern ZRESULT ZAPI 
ZSemaphore_Init(
	_Inout_ ZSemaphore* _lpSem,
	_In_    Int32       _shared,
	_In_    Uint32      _uValue);

/* 
Acquire a semaphore.
@_lpSem: a pointer to the semaphore 
@return: Z_OK on success, non-zero on failure.*/
extern ZRESULT ZAPI
ZSemaphore_Wait(
	_Inout_ ZSemaphore* _lpSem);

/* 
Try to acquire a semaphore.
@_lpSem: a pointer to the semaphore 
@return: Z_OK on success, non-zero on failure.*/ 
extern ZRESULT ZAPI 
ZSemaphore_TryWait(
	_Inout_ ZSemaphore* _lpSem);

/* 
Try to cquire a semaphore, using a timeout.
@_lpSem       : a pointer to the semaphore 
@_lpAbsTimeout: a pointer to the structure specifying an absolute timeout 
                in seconds and nanoseconds since the Epoch,
                1970-01-01 00:00:00 +0000 (UTC).
@return       : Z_OK on success, non-zero on failure.*/ 
extern ZRESULT ZAPI 
ZSemaphore_TimedWait(
	_Inout_ ZSemaphore*            _lpSem,
	_In_    const struct timespec* _lpAbsTimeout);

/* 
Release a semaphore.
@_lpSem: a pointer to the semaphore 
@return: Z_OK on success, non-zero on failure.*/ 
extern ZRESULT ZAPI 
ZSemaphore_Post(
    _Inout_ ZSemaphore* _lpSem);

/* 
Get the value of a semaphore.
@_lpSem : a pointer to the semaphore 
@_iValue: a pointer to the current value of the semaphore.
@return : Z_OK on success, non-zero on failure.*/ 
extern ZRESULT ZAPI
ZSemaphore_GetValue(
	_Inout_ ZSemaphore* _lpSem,
	_Inout_ Int32*      _lpiValue);

/* 
Destroy a semaphore.
@_lpSem: a pointer to the semaphore 
@return: Z_OK on success, non-zero on failure.*/ 
extern ZRESULT ZAPI
ZSemaphore_Release(
	_Inout_ ZSemaphore* _lpSem);

/* 
Open a named semaphore.
@_name : a name for the semaphore object.
@_flag : If O_CREAT is specified, then the semaphore is created if it does not
         already exist. If both O_CREAT and O_EXCL are specified, then an error
		 is returned if a semaphore with the given name already exists.
@_mode : (ignored by windows implementation)
          specifies the permissions to be placed on the new semaphore.
@_value: specifies the initial value for the semaphore.
@return: a pointer to a new semaphore or NULL on error*/
extern ZSemaphore* ZAPI
ZSemaphore_Open(
	_In_ Lpcstr _name,
	_In_ Int32  _flag,
	_In_ Uint16 _mode,
	_In_ Uint32 _value);

/* 
Close a named semaphore (equivalent to ZSemaphore_Destroy)
@_lpSem: a pointer to the semaphore 
@return: Z_OK on success, non-zero on failure.*/ 
extern ZRESULT ZAPI 
ZSemaphore_Close(
	_Inout_ ZSemaphore* _lpSem);

/* 
Remove semaphore by name.
Note: the semaphore object is not actually destroyed  
      until its last handle has been closed. If you need
	  to destroy the semaphore, call ZSemaphore_Release().
@_name : The name of the semaphore to remove.
@return: Z_OK on success, non-zero on failure*/
extern ZRESULT ZAPI
ZSemaphore_Unlink(
	_In_ Lpcstr _name);



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  