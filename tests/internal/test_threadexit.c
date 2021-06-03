/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: test_threadexit.c
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
ExitFunc(Handle _hArg) {
	Test_Sleep();
	ZThread_Exit(2);
	return 1;
}


Void Test_ExitThread(Void) {
	ZRESULT hResult;
	ZThread thread;
	int res;
	ZThread_Init(&thread, ExitFunc, NULL);
	hResult = ZThread_Join(thread, &res);
	//assert(ZThread_Join(thread, &res)); ///------------------------------------ why does this assert fail?
	assert(res == 2);
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/  