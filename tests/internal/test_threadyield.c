/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: test_threadyield.c
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
 


static Int32
ThreadYield(Handle _hArg) {
	Z_Unused(_hArg);  
	ZThread_Yield();
	return 0;
}


Void Test_ThreadYield(Void) {
	ZThread t[40];
	Int32   i; 
	/* 
	Start a bunch of child threads */
	for (i = 0; i < 40; ++i) {
		ZThread_Init(&t[i], ThreadYield, NULL);
	} 
	/* Yield... */
	ZThread_Yield();

	/* Wait for the threads to finish */
	for (i = 0; i < 40; ++i) {
		ZThread_Join(t[i], NULL);
	}
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/  