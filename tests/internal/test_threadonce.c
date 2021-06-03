/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: test_threadonce.c
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
 


#define THREAD_ONCE_COUNT 16
static ZThreadOnceFlag s_onceFlags[10000];  //thread-once flags


 
static Void 
ThreadOnce_Func(Void) {
	ZMutex_Lock(g_mutex);
	++g_count;
	ZMutex_Unlock(g_mutex);
}


static Int32 
ThreadOnce(Handle _hData) { 
	Int32 i;
	Z_Unused(_hData); 
	for (i = 0; i < 10000; i++)
		ZThread_CallOnce(&(s_onceFlags[i]), ThreadOnce_Func);
	return 0;
}


Void Test_ThreadOnce(Void) {
	const ZThreadOnceFlag
		once_flag_init = { 0, };

	ZThread threads[THREAD_ONCE_COUNT];
	Int32   i; 
	/* 
	Init 10000 once_flags */
	for (i = 0; i < 10000; i++) {
		s_onceFlags[i] = once_flag_init;
	}
	/*
	Clear the global counter. */
	ZMutex_Lock(g_mutex);
	g_count = 0;
	ZMutex_Unlock(g_mutex); 
	/*
	Create threads */
	for (i = 0; i < THREAD_ONCE_COUNT; i++) {
		ZThread_Init(&(threads[i]), ThreadOnce, NULL);
	}
	/*
	Wait for all threads to finish:*/
	for (i = 0; i < THREAD_ONCE_COUNT; i++) {
		ZThread_Join(threads[i], NULL);
	}	 
	assert(g_count == 10000);
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/  