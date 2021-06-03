/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved. 
* Zlib license.
*
* File: zbasepath_bsd.h 
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
#ifndef __ZBASEPATH_BSD_H__
#define __ZBASEPATH_BSD_H__

#include "zbasepath_common.h" 

#if defined(__DragonFly__) || defined(__FreeBSD__) || \
    defined(__FreeBSD_kernel__) || defined(__NetBSD__) 
 
#include <sys/types.h>
#include <sys/sysctl.h>
#include <dlfcn.h>

#ifdef __cplusplus
extern "C" {
#endif 


	static Int32
	BsdGetBasePath(
		_Inout_opt_ Char* _out,
		_In_        Int32 _len) {

		Char  cBuffer1[PATH_MAX];
		Char  cBuffer2[PATH_MAX];
		Char* cPath     = cBuffer1;
		Char* cResolved = NULL;
		Int32 iLength   = -1;

		for (;;) {
		#if defined(__NetBSD__)
			Int32 mib[4] = { 
				CTL_KERN, KERN_PROC_ARGS, -1, KERN_PROC_PATHNAME
			};
		#else
			Int32 mib[4] = {
				CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 
			};
		#endif
			SizeT size = sizeof(cBuffer1);

			if (sysctl(
				mib,
				(u_int)(sizeof(mib) / sizeof(mib[0])),
				cPath,
				&size,
				NULL, 0) != 0) {
				break;
			}
			resolved = realpath(cPath, cBuffer2);
			if (!cResolved) {
				break;
			}
			iLength = (Int32)strlen(cResolved);
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
