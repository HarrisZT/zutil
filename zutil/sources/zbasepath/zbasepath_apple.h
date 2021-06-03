/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved. 
* Zlib license.
*
* File: zbasepath_apple.h 
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
#ifndef __ZBASEPATH_APPLE_H__
#define __ZBASEPATH_APPLE_H__

#include "zbasepath_common.h" 
 
#if defined(__APPLE__) 
#define _DARWIN_BETTER_REALPATH
#include <mach-o/dyld.h> 
#include <dlfcn.h>

#ifdef __cplusplus
extern "C" {
#endif
	

	static Int32
	AppleGetBasePath(
		_Inout_opt_ Char* _out,
		_In_        Int32 _len) {

		Char  cBuffer1[PATH_MAX];
		Char  cBuffer2[PATH_MAX];
		Char* cPath     = cBuffer1;
		Char* cResolved = NULL;
		Int32 iLength   = -1;

		for (;;) {
			Uint32 size = (Uint32)sizeof(cBuffer1);
			if (_NSGetExecutablePath(cPath, &size) == -1) {
				cPath = (Char*)malloc(size);
				if (!_NSGetExecutablePath(cPath, &size))
					break;
			} 
			cResolved = realpath(cPath, cBuffer2);
			if (!resolved) {
				break;
			}
			iLength = (Int32)strlen(resolved);
			if (iLength <= _len) 
				memcpy(_out, cResolved, iLength); 
			break;
		}
		if (cPath != cBuffer1)
			free(cPath);
		return iLength;
	} 

#ifdef __cplusplus
}
#endif 
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/ 
