/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zthread.h 
* Desc: C thread interface 
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
#ifndef __ZTHREAD_H__
#define __ZTHREAD_H__ 

#include "zcondvar.h" 
#if defined(__cplusplus)
extern "C" {
#endif

	 

/* Section I:
** Threads Types
******************************************************************************/
#if (Z_PLATFORM_WINDOWS)
  typedef Handle ZThread;
#else
  typedef pthread_t ZThread;
#endif

/*
Thread task function.
Any thread that is started with the ZThread_Init() function must be
started through a function of this type.
@_hArg:  the thread argument 
         (of the corresponding ZThread_Init(...) call).
@return: the thread's return value (a ZRESULT perhaps),
         which can be obtained by another thread
         by using the ZThread_Join(...) function.*/
typedef Int32(*ZThreadTaskFn)(Handle _hArg);





/* Section II:
** Threads Api Functions*/
//****************************************************************************/
/*
Create a new thread.
@_lpthread: Identifier of the newly created thread.
@_func    : A function pointer to the function that will be executed in
            the new thread.
@_hArg    : An argument to the thread function.
@return   : Z_OK on success, non-zero on error
@note A thread’s identifier may be reused for a different thread once the
original thread has exited and either been detached or joined to another
thread.*/
extern ZRESULT ZAPI
ZThread_Init(
	_Inout_ ZThread*      _lpthread,
	_In_    ZThreadTaskFn _taskfunc, 
	_In_    Handle        _hArg);


/*
Retrieves a pseudo handle for the calling thread*/
extern ZThread ZAPI
ZThread_GetCurrent(Void);


/*
Retrieves the thread identifier of the calling thread*/
extern Dword ZAPI
ZThread_CurrentId(Void);


/*
Dispose of any resources allocated to the thread when that thread exits.
@return: Z_OK, or non-zero on error*/
extern ZRESULT ZAPI
ZThread_Detach(
	_Inout_ ZThread _lpthread);

/*
Compare two thread identifiers. The function determines if two thread 
identifiers refer to the same thread.
@return: Zero if the two thread identifiers refer to different threads. 
         Otherwise a nonzero value is returned. */
extern Bool ZAPI
ZThread_IsEqual(
	_In_ ZThread _lpthread0, 
	_In_ ZThread _lpthread1);

/*
Terminate execution of the calling thread.
@_iResult: result code of the calling thread.*/
extern NORETURN Void ZAPI
ZThread_Exit(
	_In_ Int32 _iResult);

/*
Wait for a thread to terminate, joining the given thread 
with the current thread by blocking until the other thread has terminated.
@_lpthread: the thread to join with.
@_iResult : if this pointer is not NULL, the function will store the result
            code of the given thread in the integer pointed to by @c res.
@return   : Z_OK on success, non-zero on error*/
extern ZRESULT ZAPI
ZThread_Join(
	_Inout_     ZThread _lpthread, 
	_Inout_opt_ Int32*  _iResult);

/*
Put the calling thread to sleep, suspending execution of the calling thread.
@_tsduration : interval to sleep for
@_tsremaining: if non-NULL, this parameter will hold the remaining
               time until time_point upon return. This will
               typically be zero, but if the thread was woken up
               by a signal that is not ignored before duration was
               reached @c remaining will hold a positive time.
@return      : Z_OK on success, non-zero on error*/
extern ZRESULT ZAPI
ZThread_Sleep(
	_In_     const struct timespec* _tsDuration, 
	_In_opt_ struct timespec*       _tsRemaining);

/*
Yield execution to another thread.
Permit other threads to run, even if the current thread 
would ordinarily continue to run*/
extern Void ZAPI 
ZThread_Yield(Void);





/* Section III:
** Thread-Local-Storage
******************************************************************************/
#if (Z_PLATFORM_WINDOWS)
  typedef Dword ZThreadTLS;//Thread local storage 
#else
  typedef pthread_key_t ZThreadTLS;//Thread local storage 
#endif
/*
Destructor function for a thread local storage 
@_hValue: The value of the destructed thread-specific storage*/
typedef Void(*ZThreadTLS_DTOR)(Handle _hValue);



/*
Create a thread-specific storage.
@_tls  : The unique key id that will be set if the function is successful.
@_dtor : destructor function. This can be NULL.
@return: Z_OK on success, non-zero on error

@note On Windows, the @c dtor will definitely be called when
appropriate for threads created with ZThread_Create.  It will be
called for other threads in most cases, the possible exception being
for DLLs loaded with LoadLibraryEx.  In order to be certain, you
should use ZThread_Create whenever possible*/
extern ZRESULT ZAPI 
ZThreadTLS_Init(
	_Inout_ ZThreadTLS*     _lpTls,
	_In_    ZThreadTLS_DTOR _dtor);

/*
Delete a thread-specific storage. 
@_tls: The key that shall be deleted*/
extern Void ZAPI
ZThreadTLS_Release(
	_In_ ZThreadTLS _tls);

/*
Get the value for a thread-specific storage.
@_tls  : The thread-specific storage identifier.
@return: The value held in the given thread-specific storage*/
extern Handle ZAPI
ZThreadTLS_GetValue(
	_In_ ZThreadTLS _tls);

/*
Assign a value to the thread-specific storage.
@_tlsKey: the thread-specific storage identifier.
@_hValue: the value to be held in the tls 
@return : Z_OK on success, non-zero on error*/
extern ZRESULT ZAPI
ZThreadTLS_Assign(
	_In_ ZThreadTLS _tlsKey, 
	_In_ Handle     _hValue);





/* Section IV:
** Call Once Flag
******************************************************************************/
#if (Z_PLATFORM_WINDOWS)
   typedef struct {
       LONG volatile    status;
       CRITICAL_SECTION csLock;
   } _zthread_once_t;

#  define ZThreadOnceFlag   _zthread_once_t
#  define ZTHREAD_ONCE_INIT Z_INITZERO 
#else
#  define ZThreadOnceFlag   pthread_once_t
#  define ZTHREAD_ONCE_INIT PTHREAD_ONCE_INIT 
#endif  
 
extern Void ZAPI
ZThread_CallOnce(
    _Inout_ ZThreadOnceFlag* _lpflag, 
    _In_    Void(*_callback)(Void));



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/ 