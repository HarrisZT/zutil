/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zthread.c
* Desc: C thread interface 
        Reference info for detaching threads in win32:
https://stackoverflow.com/questions/12744324/how-to-detach-a-thread-on-windows-c#answer-12746081
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
#include "zutil/zthread.h" 
 
#if (Z_PLATFORM_WINDOWS)
//#  include <windows.h>
//#  include <process.h>
//#  include <sys/timeb.h>
#else
#  include <signal.h>
#  include <sched.h>
#  include <unistd.h>
#  include <sys/time.h> 
#endif  
#if defined(__cplusplus)
extern "C" {
#endif



/*
Information to pass to the new thread task*/
typedef struct {
	ZThreadTaskFn func; //Pointer to the function to be executed 
	Handle        hArg; //Function argument for the thread function 
} ZThreadTaskData;





/* Section I:
** Win32-specific implementation*/
//***************************************************************************// 
#if (Z_PLATFORM_WINDOWS)


typedef struct _ZThreadTssData {
	Handle                  value;
	ZThreadTLS              key;
	struct _ZThreadTssData* next;
} ZThreadTssData, *LPTSS;


static THREADLOCAL LPTSS m_tss_head        = NULL;
static THREADLOCAL LPTSS m_tss_tail        = NULL;
static ZThreadTLS_DTOR   m_tss_dtors[1088] = { NULL, }; 



static Void
ZThreadWin32_Cleanup(Void) {

	LPTSS  data;
	Int32  it;
	Bool   bAgain;
	Handle hValue;

	bAgain = Z_TRUE;

	for (it = 0; it < TSS_DTOR_ITERATIONS && bAgain; it++) {
		bAgain = Z_FALSE;
		for (data = m_tss_head; data != NULL; data = data->next) {
			if (data->value != NULL) {
				hValue = data->value;
				data->value = NULL;

				if (m_tss_dtors[data->key] != NULL) {
					m_tss_dtors[data->key](hValue);
					bAgain = Z_TRUE;
				}
			}
		}
	}
	while (m_tss_head != NULL) {
		data = m_tss_head->next;
		free(m_tss_head);
		m_tss_head = data;
	}
	m_tss_head = NULL;
	m_tss_tail = NULL;
}
 

static Void WINAPI 
ZThreadWin32_Callback(
	_In_ Lpvoid _h,
	_In_ Dword  _dwReason,
	_In_ Lpvoid _pv) {

	Z_Unused(_h);
	Z_Unused(_pv);
	if (m_tss_head != NULL &&
		(_dwReason == DLL_THREAD_DETACH ||
			_dwReason == DLL_PROCESS_DETACH)) {
		ZThreadWin32_Cleanup();
	}
}

//#if defined(_MSC_VER)
//#  if defined(_M_X64)
//#    pragma const_seg(".CRT$XLB")
//#  else
//#    pragma data_seg(".CRT$XLB")
//#  endif
//
//PIMAGE_TLS_CALLBACK p_thread_callback = ZThreadWin32_Callback;
//
//#  if defined(_M_X64)
//#    pragma data_seg()
//#  else
//#    pragma const_seg()
//#  endif
//#else
//	PIMAGE_TLS_CALLBACK p_thread_callback \
//		__attribute__((section(".CRT$XLB"))) = _tls_callback;
//#  endif 
#endif 





/* Section II:
** Threads implementation
******************************************************************************/	 
#if (Z_PLATFORM_WINDOWS)
static Dword WINAPI 
ZThread_WrapperFunction(
	_In_ Handle _hArg)
#else
static Handle 
ZThread_WrapperFunctionn(
	_In_ Handle _hArg)
#endif
{
	ZThreadTaskFn    fnTask;
	Handle           hArg;
	Int32            iResult;
	ZThreadTaskData* taskdata;

	taskdata = (ZThreadTaskData*)_hArg;
	fnTask   = taskdata->func;
	hArg     = taskdata->hArg;
	/*
	The thread is responsible for freeing the startup information */
	free((Handle)taskdata);
	/*
	Call the actual client thread function */
	iResult = fnTask(hArg);

#if (Z_PLATFORM_WINDOWS)
	if (m_tss_head != NULL) 
		ZThreadWin32_Cleanup(); 
	return (Dword)iResult;
#else
	return (Handle)(Intptr)iResult;
#endif
}


ZRESULT 
ZThread_Init(
	_Inout_ ZThread*      _lpthread,
	_In_    ZThreadTaskFn _taskfunc,
	_In_    Handle          _taskArg) {

	/*
	Fill out the thread startup information
	(passed to the thread wrapper, which will eventually free it) */
	ZThreadTaskData* taskdata;
	taskdata = (ZThreadTaskData*)malloc(sizeof(ZThreadTaskData));
	if (taskdata == NULL) 
		return Z_EOUTOFMEMORY;
	 
	taskdata->func = _taskfunc;
	taskdata->hArg = _taskArg;

#if (Z_PLATFORM_WINDOWS)
	*_lpthread = CreateThread(
		NULL, 0,
		ZThread_WrapperFunction,
		(Handle)taskdata,
		0, NULL);
#else
	if (pthread_create(
		_lpthread, NULL,
		ZThread_WrapperFunction,
		(Handle)taskdata) != 0)
	{
		*_lpthread = 0;
	}
#endif 
	/*failed to create thread? */
	if (!*_lpthread) {
		free(taskdata);
		return Z_EFAIL;
	}
	return Z_OK;
}


ZThread
ZThread_Current(Void) { 
#if (Z_PLATFORM_WINDOWS)
	return GetCurrentThread();
#else
	return pthread_self();
#endif
}


Dword 
ZThread_CurrentId(Void) {
#if (Z_PLATFORM_WINDOWS)
	return (Dword)GetCurrentThreadId();
#elif (Z_PLATFORM_LINUX)
	return (Dword)syscall(SYS_gettid);
#elif defined(__APPLE__) && defined(__MACH__)
	return (Dword)syscall(SYS_thread_selfid);
#else
	return (Dword)pthread_self();
#endif 
}


ZRESULT 
ZThread_Detach(
	_Inout_ ZThread _lpthread) {

#if (Z_PLATFORM_WINDOWS) 
	return CloseHandle(_lpthread) != 0 ? Z_OK : Z_EFAIL;
#else
	return pthread_detach(_lpthread) == 0 ? Z_OK : Z_EFAIL;
#endif
}


Bool
ZThread_IsEqual(
	_In_ ZThread _lpthread0,
	_In_ ZThread _lpthread1) {

#if (Z_PLATFORM_WINDOWS)
	return GetThreadId(_lpthread0) == GetThreadId(_lpthread1);
#else
	return pthread_equal(_lpthread0, _lpthread1);
#endif
}


Void
ZThread_Exit(
	_In_ Int32 _iResult) {

#if (Z_PLATFORM_WINDOWS)
	if (m_tss_head != NULL)
		ZThreadWin32_Cleanup();
	ExitThread((Dword)_iResult);
#else
	pthread_exit((Handle)(Intptr)_iResult);
#endif
}


ZRESULT 
ZThread_Join(
	_Inout_     ZThread _lpthread,
	_Inout_opt_ Int32*    _iResultOut) {

#if (Z_PLATFORM_WINDOWS)
	Dword dwData;
	if (WaitForSingleObject(_lpthread, INFINITE) == WAIT_FAILED) {
		return Z_EFAIL;
	}
	if (_iResultOut != NULL) {
		if (GetExitCodeThread(_lpthread, &dwData) != 0)
			*_iResultOut = (Int32)dwData;
		else return Z_EFAIL;
	}
	CloseHandle(_lpthread);
#else
	Handle hData;
	if (pthread_join(_lpthread, &hData) != 0) {
		return Z_EFAIL;
	}
	if (_iResultOut != NULL)
		*_iResultOut = (Int32)(Intptr)hData;
#endif
	return Z_OK;
}


ZRESULT 
ZThread_Sleep(
	_In_     const struct timespec* _tsDuration,
	_In_opt_ struct timespec*       _tsRemaining) {

	ZRESULT hResult;

#if (Z_PLATFORM_WINDOWS)
	struct timespec start;
	timespec_get(&start, TIME_UTC);
	hResult =
		SleepEx((Dword)(_tsDuration->tv_sec * 1000 +
			_tsDuration->tv_nsec / 1000000 +
			(((_tsDuration->tv_nsec % 1000000) == 0) ? 0 : 1)), Z_TRUE);

	if (hResult != Z_OK) {
		if (_tsRemaining != NULL) {
			timespec_get(_tsRemaining, TIME_UTC);
			_tsRemaining->tv_sec -= start.tv_sec;
			_tsRemaining->tv_nsec -= start.tv_nsec;
			if (_tsRemaining->tv_nsec < 0) {
				_tsRemaining->tv_nsec += 1000000000;
				_tsRemaining->tv_sec -= 1;
			}
		}
		return (hResult == WAIT_IO_COMPLETION) ? -1 : -2;
	}
#else 
	hResult = nanosleep(_tsDuration, _tsRemaining);
	hResult = Z_GetResultFromErrno(hResult);
#endif
	return hResult;
}


Void
ZThread_Yield(Void) {
#if (Z_PLATFORM_WINDOWS)
	Sleep(0);
#else
	sched_yield();
#endif
}





/* Section III:
** Thread TLS implementation*/
//***************************************************************************//
ZRESULT 
ZThreadTLS_Init(
	_Inout_ ZThreadTLS*     _tls,
	_In_    ZThreadTLS_DTOR _dtor) {

#if (Z_PLATFORM_WINDOWS)
	*_tls = TlsAlloc();
	if (*_tls == TLS_OUT_OF_INDEXES) {
		return Z_EFAIL;
	}
	m_tss_dtors[*_tls] = _dtor;
#else
	if (pthread_key_create(_tls, _dtor) != 0)
		return Z_EFAIL;
#endif
	return Z_OK;
}


Void 
ZThreadTLS_Release(
	_In_ ZThreadTLS _tls) {

#if (Z_PLATFORM_WINDOWS)
	LPTSS data;
	LPTSS previous;

	previous = NULL;

	data = (LPTSS)TlsGetValue(_tls);
	if (data != NULL) {
		if (data == m_tss_head)
			m_tss_head = data->next;
		else {
			previous = m_tss_head;
			if (previous != NULL) {
				while (previous->next != data)
					previous = previous->next;
			}
		}
		if (data == m_tss_tail)
			m_tss_tail = previous;
		free(data);
	}
	m_tss_dtors[_tls] = NULL;
	TlsFree(_tls);
#else
	pthread_key_delete(_tls);
#endif
}


Handle 
ZThreadTLS_GetValue(
	_In_ ZThreadTLS _tls) {

#if (Z_PLATFORM_WINDOWS)
	LPTSS data;
	data = (LPTSS)TlsGetValue(_tls);
	if (data == NULL)
		return NULL;
	return data->value;
#else
	return pthread_getspecific(_tlsKey);
#endif
}


ZRESULT 
ZThreadTLS_Assign(
	_In_ ZThreadTLS _tls,
	_In_ Handle       _hArg) {

#if (Z_PLATFORM_WINDOWS)
	LPTSS data;
	data = (LPTSS)TlsGetValue(_tls);
	if (data == NULL) {
		data =
			(struct _ZThreadTssData*)malloc(
				sizeof(struct _ZThreadTssData));

		if (data == NULL) {
			return Z_EFAIL;
		}
		data->value = NULL;
		data->key = _tls;
		data->next = NULL;

		if (m_tss_tail != NULL)
			m_tss_tail->next = data;
		else m_tss_tail = data;

		if (m_tss_head == NULL) {
			m_tss_head = data;
		}
		if (!TlsSetValue(_tls, data)) {
			free(data);
			return Z_EFAIL;
		}
	}
	data->value = _hArg;
#else
	if (pthread_setspecific(_tls, _hArg) != 0)
		return Z_EFAIL;
#endif
	return Z_OK;
}





/* Section IV:
** Thread Call Once implementation*/
//***************************************************************************//
Void
ZThread_CallOnce(
	_Inout_ ZThreadOnceFlag* _lpflag,
	_In_    Void(*_callback)(Void)) {

#if (Z_PLATFORM_WINDOWS)
	/* The idea here is that we use a spin lock (via the
	   InterlockedCompareExchange function) to restrict access to the
	   critical section until we have initialized it, then we use the
	   critical section to block until the callback has completed
	   execution. */
	while (_lpflag->status < 3) {
		switch (_lpflag->status) {
		case 2:
			EnterCriticalSection(&(_lpflag->csLock));
			LeaveCriticalSection(&(_lpflag->csLock));
			break;
		case 0:
			if (InterlockedCompareExchange(&(_lpflag->status), 1, 0) == 0) {
				InitializeCriticalSection(&(_lpflag->csLock));
				EnterCriticalSection(&(_lpflag->csLock));
				_lpflag->status = 2;
				_callback();
				_lpflag->status = 3;
				LeaveCriticalSection(&(_lpflag->csLock));
				return;
			}
		}
	}
#else
	pthread_once(_lpflag, _callback);
#endif
}
#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
//EOF
/*****************************************************************************/ 