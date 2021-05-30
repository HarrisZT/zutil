/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: test_condvars.c
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
 
#define CONDVAR_THREAD_COUNT 40



static Int32
ConditionNotify(Handle _hdata) {
	Z_Unused(_hdata);
	ZMutex_Lock(g_mutex);

	--g_count;
	ZCondVar_Broadcast(&g_cond);
	ZMutex_Unlock(g_mutex); 
	return Z_OK;
}

 
static Int32 
ConditionWait(Handle _hdata) {
	Z_Unused(_hdata);
	fflush(stdout);
	ZMutex_Lock(g_mutex);

	while (g_count > 0) {
		fflush(stdout);
		ZCondVar_Wait(&g_cond, g_mutex, NULL);
	}
	ZMutex_Unlock(g_mutex);
	return Z_OK;
}


Void Test_ConditionVariables(Void) { 
	ZThread t1, thread[CONDVAR_THREAD_COUNT];
	Int32   it; 
	 
	//Set global counter to the number of threads to run 
	g_count = CONDVAR_THREAD_COUNT;
	 
	/*Start the waiting thread
	(it will wait for g_count to reach zero). */
	ZThread_Init(&t1, ConditionWait, NULL); 
	
	/*Start child threads 
	(these will decrease g_count by 1 as they finish) */
	for (it = 0; it < CONDVAR_THREAD_COUNT; ++it) {
		ZThread_Init(&thread[it], ConditionNotify, NULL);
	} 
	//Wait for the waiting thread to finish 
	ZThread_Join(t1, NULL);
	  
	//Wait for remaining threads to finish 
	for (it = 0; it < CONDVAR_THREAD_COUNT; ++it) {
		ZThread_Join(thread[it], NULL);
	}
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/ 