/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: test_threadargs.c
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



//number of thread arguments accepted
#define THREAD_ARGS_COUNT 4 



static Int32 
ThreadArgs(Handle _hdata) {
	return *(Int32*)_hdata;
}


Void Test_ThreadArgs(Void) { 
	ZThread threads[THREAD_ARGS_COUNT];
	Int32   ids[THREAD_ARGS_COUNT];
	Int32   result, i;

	for (i = 0; i < THREAD_ARGS_COUNT; i++) {
		ids[i] = rand();
		ZThread_Init(
			&(threads[i]), 
			ThreadArgs, 
			(Void*)&(ids[i]));
	}
	for (i = 0; i < THREAD_ARGS_COUNT; i++) {
		ZThread_Join(threads[i], &result);
		assert(result == ids[i]);
	}
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/  