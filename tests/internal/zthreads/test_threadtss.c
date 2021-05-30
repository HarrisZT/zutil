/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: test_threadtss.c
* Desc: unit test for ZThreads 
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
#include "testunit_module_zthreads.h"
 


#define TSS_THREAD_COUNT 256 
 
static ZThreadTLS s_tss_key;
static ZMutex*    s_tss_mutex;
static Int32      s_tss_freed;
   




static Void 
ThreadTSS_Free(Handle hValue) {  
	ZMutex_Lock(s_tss_mutex);
	s_tss_freed++;
	ZMutex_Unlock(s_tss_mutex);
	free(hValue);
}


static Int32
ThreadTSS_Func(Handle _hData) { 

	Int32* value;
	Z_Unused(_hData); 

	value  = (Int32*)malloc(sizeof(Int32)); 
	*value = rand();

	assert(ZThreadTLS_GetValue(s_tss_key) == NULL);
	ZThreadTLS_Assign(s_tss_key, value);
	assert(ZThreadTLS_GetValue(s_tss_key) == value);

	ZThreadTLS_Assign(s_tss_key, NULL);
	assert(ZThreadTLS_GetValue(s_tss_key) == NULL);
	ZThreadTLS_Assign(s_tss_key, value);
	assert(ZThreadTLS_GetValue(s_tss_key) == value);
	
	return Z_OK;
}


Void Test_ThreadTSS(Void) { 
	ZThread threads[TSS_THREAD_COUNT];
	Int32*  value, i;
	 
	value = (Int32*)malloc(sizeof(Int32)); 
	*value = rand();

	ZThreadTLS_Init(&(s_tss_key), ThreadTSS_Free);
	s_tss_mutex = ZMutex_Create();
	s_tss_freed = 0;

	assert(ZThreadTLS_GetValue(s_tss_key) == NULL);
	ZThreadTLS_Assign(s_tss_key, value);
	assert(ZThreadTLS_GetValue(s_tss_key) == value);

	for (i = 0; i < TSS_THREAD_COUNT; ++i) {
		ZThread_Init(&(threads[i]), ThreadTSS_Func, NULL);
	} 
	for (i = 0; i < TSS_THREAD_COUNT; ++i) {
		ZThread_Join(threads[i], NULL);
	} 
	assert(s_tss_freed == TSS_THREAD_COUNT);
	assert(ZThreadTLS_GetValue(s_tss_key) == value);
	ZThreadTLS_Release(s_tss_key);
	assert(ZThreadTLS_GetValue(s_tss_key) == NULL);
	assert(s_tss_freed == TSS_THREAD_COUNT);

	free(value);
	ZMutex_Release(s_tss_mutex);
}





/*****************************************************************************/ 
/*****************************************************************************/  
/* 
NOTE: 
Mac OS X, early MinGW, and TCC do not support 
compile time thread-local storage */
#if defined(__APPLE__) || (defined(__MINGW32__) &&\
           (__GNUC__ < 4)) || defined(__TINYC__)

#  define TLS_SUPPORTED 0 
#else
#  define TLS_SUPPORTED 1
   THREADLOCAL Int32 g_localVar; //Compile time thread-local-storage variable 
#endif 
   	  


static Int32 
ThreadLocalStorage(Handle _hArg) {
#if (TLS_SUPPORTED) 
	(Void)_hArg;
	g_localVar = rand();
#endif
	return Z_OK;
}


Void Test_ThreadLocalStorage(Void) { 
#if (TLS_SUPPORTED) 
	ZThread t1; 
	
	/*Clear the TLS variable 
	(it should keep this value after all threads are finished).*/
	g_localVar = 1;
	 
	//Start a child thread that modifies g_localVar 
	ZThread_Init(&t1, ThreadLocalStorage, NULL);
	ZThread_Join(t1, NULL);

	//Check if the TLS variable has changed 
	assert(g_localVar == 1);
#endif
} 
/*****************************************************************************/  
//EOF
/*****************************************************************************/  