/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zbasepath.c 
* Desc: utility function for finding the base program path
*       (executable or module)
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
#include "zbasepath.h"
#if defined(__cplusplus)
extern "C" {
#endif 

#if (Z_PLATFORM_WINDOWS)  
#  include "zbasepath_win32.h"
#  define _ZExePath Win32_GetExePath
#  define _ZModPath Win32_GetModPath

#elif defined(__linux__) || defined(__CYGWIN__) || defined(__sun) 
#  include "zbasepath_linux.h"
#  define _ZExePath Linux_GetExePath
#  define _ZModPath Linux_GetModPath

#elif defined(__APPLE__)
#  include "zbasepath_apple.h"
#  define _ZExePath Apple_GetExePath
#  define _ZModPath Apple_GetModPath

#elif defined(__QNXNTO__) 
#  include "zbasepath_qnxnto.h"
#  define _ZExePath Qnxnto_GetExePath
#  define _ZModPath Qnxnto_GetModPath

#elif defined(__DragonFly__) || defined(__FreeBSD__) || \
      defined(__FreeBSD_kernel__) || defined(__NetBSD__)
#  include "zbasepath_bsd.h"
#  define _ZExePath Bsd_GetExePath
#  define _ZModPath Bsd_GetModPath
#else 
#  error unsupported platform 
#endif




	static Int32 
	_zbasepath(
		_In_        Int32  _iExeOrModFlag,
		_Inout_opt_ Char*  _cPathOut,
		_In_        Int32  _iCapacity,
		_Inout_opt_ Int32* _iDirnameLength) {

		Int32 iResult = -1; //invalid

		if (_iExeOrModFlag == ZBASEPATH_GETEXE) {
			iResult = _ZExePath(_cPathOut, _iCapacity, _iDirnameLength);
		} 
		else if (_iExeOrModFlag == ZBASEPATH_GETMOD) {
			iResult = _ZModPath(_cPathOut, _iCapacity, _iDirnameLength);
		} 
		return iResult; 
	} 

	
	Char* 
	ZBasePath_Get(
		_In_        Int32  _iExeOrModFlag,
		_Inout_opt_ Int32* _iDirnameLength) { 

		Char* cPathOut;
		Int32 iLength;

		iLength  = _zbasepath(_iExeOrModFlag, NULL, 0, _iDirnameLength);
		cPathOut = (Char*)malloc(iLength + 1);
		if (!cPathOut)
			return NULL; //error
		_zbasepath(_iExeOrModFlag, cPathOut, iLength, _iDirnameLength);
		cPathOut[iLength] = '\0';
		return cPathOut;
	}


#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
//EOF
/*****************************************************************************/ 