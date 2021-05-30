/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zcondvar.h 
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
#ifndef __ZCONDVAR_H__
#define __ZCONDVAR_H__ 

#include "zmutex.h"   
#if defined(__cplusplus)
extern "C" {
#endif



/* Section I:
** Platform Implementation for threads
******************************************************************************/
#if (Z_PLATFORM_WINDOWS)
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#    define __UNDEF_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#  ifdef __UNDEF_LEAN_AND_MEAN
#    undef WIN32_LEAN_AND_MEAN
#    undef __UNDEF_LEAN_AND_MEAN
#  endif
#  define TSS_DTOR_ITERATIONS (4)

   /* 
   Condition variable for threads*/
   typedef struct {
       Handle           hEvents[2];  //signal and broadcast events
       Uint32           pending;     //number of pending waiters 
       CRITICAL_SECTION critSection; //serialized access to pending 
   } ZCondVar;

#else
	/* 
* POSIX
* For reference of the following POSIX defs, see:
* https://man7.org/linux/man-pages/man7/feature_test_macros.7.html
******************************************************************************/
#  undef _FEATURES_H
#  if !defined(_GNU_SOURCE)
#    define _GNU_SOURCE
#  endif
#  if !defined(_POSIX_C_SOURCE) || ((_POSIX_C_SOURCE - 0) < 199309L)
#    undef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 199309L
#  endif
#  if !defined(_XOPEN_SOURCE) || ((_XOPEN_SOURCE - 0) < 500)
#    undef _XOPEN_SOURCE
#    define _XOPEN_SOURCE 500
#  endif
#  define _XPG6
#  include <pthread.h>
#  define TSS_DTOR_ITERATIONS PTHREAD_DESTRUCTOR_ITERATIONS
   /*
   Condition variable for threads*/
   typedef pthread_cond_t ZCondVar;
#endif


   	 
/* Section II:
** Routines for Threading Condition Variables*/
//****************************************************************************/
/*
Initialize a condition variable.
@_lpcond: a pointer to condition variable 
@return : Z_OK on success, non-zero on failure*/
extern ZRESULT ZAPI
ZCondVar_Init(
	_Inout_ ZCondVar* _lpcond); 

/*
Deallocate and release any resources used by the condition variable.
@_lpcond: a pointer to condition variable*/
extern Void ZAPI
ZCondVar_Release(
	_Inout_ ZCondVar* _lpcond); 

/*
Signal a condition variable.
Unblocks one of the threads that are blocked on the given condition variable
at the time of the call. If non are blocked, the function does nothing.
@_lpcond: a pointer to condition variable 
@return : Z_OK on success, non-zero on failure*/
extern ZRESULT ZAPI
ZCondVar_PostSignal(
	_Inout_ ZCondVar* _lpcond); 

/*
Broadcast a condition variable.
Unblocks all of the threads that are blocked on the given condition variable
at the time of the call. If non are blocked, the function does nothing.
@_lpcond: a pointer to condition variable 
@return : Z_OK on success, non-zero on failure*/
extern ZRESULT ZAPI
ZCondVar_Broadcast(
	_Inout_ ZCondVar* _lpcond); 

/*
Wait for a condition variable to become signaled.
This function atomically unlocks the given mutex and blocks execution until
the condition variable is signaled by a call to ZCondVar_PostSignal or to
ZCondVar_Broadcast, or until after the specified time. When the calling
thread becomes unblocked it locks the mutex before it returns.
@_lpcond    : a pointer to condition variable 
@_lpmutex   : a pointer to mutex object.
@_cptimespec: an optional pointer to timespec (when to timeout). 
              This variable can be NULL. 
@return     : Z_OK on success, Z_ETIMEDOUT if the time specified in 
              the call was reached without acquiring the requested resource,
			  or non-zero on failure.*/
extern ZRESULT ZAPI
ZCondVar_Wait(
	_Inout_  ZCondVar*              _lpcond, 
	_Inout_  ZMutex*                _lpmutex,
	_In_opt_ const struct timespec* _cptimespec);



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  