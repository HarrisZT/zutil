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
 


	static int Bsd_GetExePath(
		char* _cPathOut,
		int   _iCapacity,
		int*  _iDirnameLength) {

		char  cBuffer1[PATH_MAX];
		char  cBuffer2[PATH_MAX];
		char* cPath     = cBuffer1;
		char* cResolved = NULL;
		int   iLength   = -1;

		for (;;) {
		#if defined(__NetBSD__)
			int mib[4] = { CTL_KERN, KERN_PROC_ARGS, -1, KERN_PROC_PATHNAME };
		#else
			int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
		#endif
			size_t size = sizeof(cBuffer1);

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
			iLength = (int)strlen(cResolved);
			if (iLength <= iCapacity) {
				memcpy(_cPathOut, cResolved, iLength);

				if (_iDirnameLength) {
					int i;
					for (i = iLength - 1; i >= 0; --i) {
						if (_cPathOut[i] == '/') {
							*_iDirnameLength = i;
							break;
						}
					}
				}
			}
			break;
		}
		if (cPath != cBuffer1)
			free(cPath);
		return iLength;
	}

 
	static int Bsd_GetModulePath(
		char* _cPathOut,
		int   _iCapacity,
		int*  _iDirnameLength) {

		char  cBuffer[PATH_MAX];
		char* cResolved = NULL;
		int   iLength   = -1;

		for (;;) {
			Dl_info info;
			if (dladdr(F2D_ReturnAddress(), &info)) {
				cResolved = realpath(info.dli_fname, cBuffer);
				if (!cResolved) {
					break;
				}
				iLength = (int)strlen(cResolved);
				if (iLength <= iCapacity) {
					memcpy(_cPathOut, cResolved, iLength);

					if (_iDirnameLength) {
						int i;
						for (i = iLength - 1; i >= 0; --i) {
							if (_cPathOut[i] == '/') {
								*_iDirnameLength = i;
								break;
							}
						}
					}
				}
			}
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