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
#  define WAI_PROC_SELF_EXE "/proc/self/exefile"
#ifdef __cplusplus
extern "C" {
#endif
	   


	static int Qnxnto_GetExePath(
		char* _cPathOut,
		int   _iCapacity,
		int*  _iDirnameLength) {

		char  cBuffer1[PATH_MAX];
		char  cBuffer2[PATH_MAX];
		char* cResolved = NULL;
		FILE* fSelfExe  = NULL;
		int   iLength   = -1;

		for (;;) {
			fSelfExe = fopen(_PROC_SELF_EXE, "r");
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
			iLength = (int)strlen(cResolved);
			if (iLength <= _iCapacity) {
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
		fclose(fSelfExe);
		return iLength;
	}

 
	static int Qnxnto_GetModPath(
		char* _cPathOut,
		int   _iCapacity,
		int*  _iDirnameLength) {

		char  cBuffer[PATH_MAX];
		char* cResolved = NULL;
		int   iLength = -1;

		for (;;) {
			Dl_info info;
			if (dladdr(F2D_ReturnAddress(), &info)) {
				cResolved = realpath(info.dli_fname, cBuffer);
				if (!cResolved) {
					break;
				}
				iLength = (int)strlen(cResolved);
				if (iLength <= _iCapacity) {
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