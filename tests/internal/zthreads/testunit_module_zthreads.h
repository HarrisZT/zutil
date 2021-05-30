/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: testunit_module_zthreads.h 
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
#ifndef __TESTUNIT_MODULE_ZTHREADS_H__
#define __TESTUNIT_MODULE_ZTHREADS_H__ 

#include <assert.h> 
#include <limits.h> 

#include "zutil/zsystem.h"
#include "zutil/zstring.h"
#include "zutil/zthread.h"  
#include "zutil/zcondvar.h" 

#if !(Z_PLATFORM_WINDOWS)
#  include <unistd.h>
#  include <strings.h>
#endif
#if defined(__cplusplus)
extern "C" {
#endif



extern ZCondVar g_cond;  //condition variable
extern ZMutex*  g_mutex; //mutex variable 
extern Int32    g_count; //global count variable


extern Void Test_ThreadTSS(Void);
extern Void Test_ThreadArgs(Void);
extern Void Test_ThreadOnce(Void);
extern Void Test_TimedMutex(Void);
extern Void Test_Sleep(Void);
extern Void Test_ThreadTime(Void); 
extern Void Test_ThreadLocalStorage(Void); 
extern Void Test_ConditionVariables(Void); 
extern Void Test_ThreadYield(Void); 
extern Void Test_ExitThread(Void); 
extern Void Test_MutexLocking(Void); 
extern Void Test_RecursiveMutex(Void);



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  