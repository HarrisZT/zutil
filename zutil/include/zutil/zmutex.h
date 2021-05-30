/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zmutex.h
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
#ifndef __ZMUTEX_H__
#define __ZMUTEX_H__   

#include "zchrono.h" 
#if defined(__cplusplus)
extern "C" {
#endif



/*
flags used to specify a type of mutex to create*/
typedef enum {
    Z_MUTEX_DEFAULT   = 0x00, //non-recursive, non-timed
    Z_MUTEX_TIMED     = 0x01, //supports timeout periods
    Z_MUTEX_RECURSIVE = 0x02, //supports recursion
} ZMUTEXTYPE; 

/*
Mutually Exclusive primitive.
Blocks concurrent access to shared resources from multiple threads*/
typedef struct _ZMutex ZMutex; 





/*
Create a mutex structure (is unlocked on initialization).
@_eType   : bit-mask value:
		    Z_MUTEX_DEFAULT
		    Z_MUTEX_TIMED
		    Z_MUTEX_DEFAULT | Z_MUTEX_RECURSIVE
		    Z_MUTEX_TIMED   | Z_MUTEX_RECURSIVE
@_lpResult: Z_OK on success, non-zero on failure (optional, can be NULL)
@return   : an allocated mutex structure, 
            !must call ZMutex_Release when finished using it!*/
extern ZMutex* ZAPI
ZMutex_CreateEx(
	_In_        ZMUTEXTYPE _eType,
	_Inout_opt_ ZRESULT*   _lpResult); 
 
/*
Create a default-type mutex structure
(is unlocked on initialization).
@return: an allocated mutex structure, 
         !must call ZMutex_Release when finished using it!*/
#define ZMutex_Create()\
  ZMutex_CreateEx(Z_MUTEX_DEFAULT, NULL)


/*
Destroy the given mutex structure and free its resources.
@_lpMutex: the mutex to destroy
@return  : Z_OK on success, non-zero on failure*/
extern ZRESULT ZAPI
ZMutex_Release(
	_Inout_ ZMutex* _lpMutex); 

/*
Returns the enum type id of the given mutex.
May return Z_UNDEFINED if mutex has not yet been initialized or error
@_lpMutex: the mutex to get an enum value from
@return  : Z_MUTEXTYPE enum value*/ 
extern Int32 ZAPI
ZMutex_GetType(
	_In_ const ZMutex* _lpMutex);

/*
Block execution until mutex can be locked or block until a specified 
point in time. If the mutex is non-recursive, and the calling thread 
already has a lock on the mutex, this call will block forever.
@_lpMutex   : the mutex instance to lock
@_lpTimeSpec: a UTC based calendar time (optional, can be NULL)
@return     : Z_OK on success, or Z_ETIMEDOUT if the time
			  specified was reached without acquiring the requested
			  resource, or Z_EFAIL on failure */
extern ZRESULT ZAPI
ZMutex_LockEx(
    _Inout_  ZMutex*                _lpMutex,
	_In_opt_ const struct timespec* _lpTimeSpec); 

/* 
Block execution until mutex can be locked or block. If the mutex is 
non-recursive, and the calling thread already has a lock on the mutex,
this call will block forever.
@_lpMutex: the mutex instance to lock 
@return  : Z_OK on success, or Z_ETIMEDOUT if the time
		   specified was reached without acquiring the requested
		   resource, or Z_EFAIL on failure */
#define ZMutex_Lock(lpMutex)\
  ZMutex_LockEx(lpMutex, NULL)

/*
Try to lock the mutex, which will support either test-and-return or
timeout. If it is already locked, the function returns without blocking.
@_lpMutex: the mutex instance to lock
@return  : Z_OK on success, or Z_EBUSY if the resource
		   requested is already in use, or Z_EFAIL on failure */
extern ZRESULT ZAPI 
ZMutex_TryLock(
    _Inout_ ZMutex* _lpMutex);

/*
Unlocks the mutex.
@_lpMutex: the mutex instance to unlock
@return  : Z_OK on success, non-zero on failure */
extern ZRESULT ZAPI 
ZMutex_Unlock(
    _Inout_ ZMutex* _lpMutex);  
 


#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  