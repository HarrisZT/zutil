/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved. 
* Zlib license.
*
* File: zbasepath_qnxnto.h 
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
#ifndef __ZBASEPATH_QNXNTO_H__
#define __ZBASEPATH_QNXNTO_H__

#include "zbasepath_common.h"  

#if defined(__QNXNTO__)  
#  include <dlfcn.h>  
#ifdef __cplusplus
extern "C" {
#endif	   


	static Int32
	QnxntoGetBasePath(
		_Inout_opt_ Char* _out,
		_In_        Int32 _len) {

		Char  cBuffer1[Z_MAXPATH];
		Char  cBuffer2[Z_MAXPATH];
		Char* cResolved = NULL;
		FILE* fSelfExe  = NULL;
		Int32 iLength   = -1;

		for (;;) {
			fSelfExe = fopen("/proc/self/exefile", "r");
			if (!fSelfExe) {
				break;
			}
			if (!fgets(cBuffer1, sizeof(cBuffer1), fSelfExe)) {
				break;
			}
			cResolved = realpath(cBuffer1, cBuffer2);
			if (!cResolved) {
				break;
			}
			iLength = (Int32)strlen(cResolved);
			if (iLength <= _len) 
				memcpy(_out, cResolved, iLength); 
			break;
		}
		fclose(fSelfExe);
		return iLength;
	} 

#ifdef __cplusplus
}
#endif 
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/
