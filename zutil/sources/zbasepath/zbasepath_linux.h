/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved. 
* Zlib license.
*
* File: zbasepath_linux.h 
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
#ifndef __ZBASEPATH_LINUX_H__
#define __ZBASEPATH_LINUX_H__

#include "zbasepath_common.h" 

#if defined(__linux__) || defined(__CYGWIN__) || defined(__sun)  
#  if defined(__linux__)
#    include <linux/limits.h> 
#  endif
#  ifndef __STDC_FORMAT_MACROS
#    define __STDC_FORMAT_MACROS
#  endif
#  include <inttypes.h> 

#  if defined(__ANDROID__) || defined(ANDROID)
#    include <fcntl.h>
#    include <sys/mman.h>
#    include <unistd.h>
#  endif 
#  if defined(__sun)
#    define _PROC_SELF_EXE  "/proc/self/path/a.out" 
#  else
#    define _PROC_SELF_EXE  "/proc/self/exe" 
#  endif  

#ifdef __cplusplus
extern "C" {
#endif

	   
	static Int32
	LinuxGetBasePath(
		_Inout_opt_ Char* _out,
		_In_        Int32 _len) {

		Char  cBuffer[Z_MAXPATH];
		Char* cResolved = NULL;
		Int32 iLength   = -1;

		for (;;) {
			cResolved = realpath(_PROC_SELF_EXE, cBuffer);
			if (!cResolved) {
				break;
			}
			iLength = (Int32)strlen(resolved);
			if (iLength <= _len) 
				memcpy(_out, cResolved, iLength); 
			break;
		}
		return iLength;
	} 

#ifdef __cplusplus
}
#endif 
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/
