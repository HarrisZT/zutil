/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: testunit_module_zthreads.c 
* Desc: zthreads module unit tests interface
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
#include "zutil_testunits.h"
#include "testunit_module_zthreads.h"



ZCondVar g_cond;  //condition variable
ZMutex*  g_mutex; //mutex variable 
Int32    g_count; //global count variable 
 
const TESTUNIT s_testunit[] = {
	{ "Test: ThreadLocalStorage",    Test_ThreadLocalStorage  },
	{ "Test: ThreadArgs",            Test_ThreadArgs          },
	{ "Test: MutexLocking",          Test_MutexLocking        },
	{ "Test: RecursiveMutex",        Test_RecursiveMutex      },
	{ "Test: ConditionVariables",    Test_ConditionVariables  },
	{ "Test: ThreadYield",           Test_ThreadYield         },
	{ "Test: ThreadSleep",           Test_Sleep               },
	{ "Test: ThreadTime",            Test_ThreadTime          },
	{ "Test: ThreadOnce",            Test_ThreadOnce          },
	{ "Test: ThreadSpecificStorage", Test_ThreadTSS           },
	{ "Test: TimedMutex",            Test_TimedMutex          },
	{ "Test: ExitThread",            Test_ExitThread          },
	{ NULL,                                                   }
};



Void 
TestUnit_Module_ZThreads(int _argc, char** _argv) {

	Int32           it, nTest;
	Dword           seed;
	struct timespec tv; 
	const TESTUNIT* test;
	
	g_mutex = ZMutex_Create(); 
	ZCondVar_Init(&g_cond); 

	timespec_get(&tv, TIME_UTC);
	srand(tv.tv_nsec);
	seed = rand();  
 
	for (nTest = 0; s_testunit[nTest].name != NULL; nTest++) {
		test = &(s_testunit[nTest]);
		{
			fputs("  ", stdout);
			fputs(test->name, stdout);
			for (it = strlen(test->name); it < 35; it++)
				fputc(' ', stdout);
			fflush(stdout);
			srand(seed);
			test->callback();
		}
		fprintf(stdout, "pass\n");
	}
	ZMutex_Release(g_mutex);
	ZCondVar_Release(&g_cond);  
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/  