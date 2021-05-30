/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved. 
* Zlib license.
*
* File: zbasepath_win32.h 
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
#ifndef __ZBASEPATH_WIN32_H__
#define __ZBASEPATH_WIN32_H__

#include "zbasepath_common.h" 

#if (Z_PLATFORM_WINDOWS) 
#  if !defined(WIN32_LEAN_AND_MEAN)
#    define WIN32_LEAN_AND_MEAN
#  endif
#  if defined(_MSC_VER)
#    pragma warning(push, 3)
#  endif
#  include <windows.h>
#  include <intrin.h>
#  if defined(_MSC_VER)
#    pragma warning(pop)
#  endif

#ifdef __cplusplus
extern "C" {
#endif


	
	static INT32 _Win32BasePath(
		HMODULE _hmodule,
		PCHAR   _pathout,
		INT32   _capacity,
		PINT32  _dirnamelength) {		
		
		WCHAR  wBuffer1[MAX_PATH];
		WCHAR  wBuffer2[MAX_PATH];
		DWORD  dwSize[2];
		DWORD  dwBufferSize;
		WCHAR* wPath1 = NULL; 
		WCHAR* wPath2 = NULL;
		INT32  iLength[3] = { -1 };
		
		for (;;) { 
			dwBufferSize = (DWORD)(sizeof(wBuffer1) / sizeof(wBuffer1[0]));

			dwSize[0] = GetModuleFileNameW(
				_hmodule, 
				wBuffer1, 
				dwBufferSize);

			if (dwSize[0] == 0) {
				break;
			}
			else if (dwSize[0] == dwBufferSize) {
				dwSize[1] = dwSize[0];
				do { 
					wPath2 = (WCHAR*)realloc(
						wPath1, 
						sizeof(WCHAR) * dwSize[1] * 2);

					if (!wPath2) {
						break;
					}
					dwSize[1] *= 2;
					wPath1 = wPath2;
					dwSize[0] = 
						GetModuleFileNameW(_hmodule, wPath1, dwSize[1]);

				} while (dwSize[0] == dwSize[1]);

				if (dwSize[0] == dwSize[1])
					break;
			}
			else { wPath1 = wBuffer1; }

			if (!_wfullpath(wBuffer2, wPath1, MAX_PATH)) {
				break;
			}
			iLength[1] = (INT32)wcslen(wBuffer2);
			iLength[2] = WideCharToMultiByte(
				CP_UTF8, 
				0, 
				wBuffer2,
				iLength[1], 
				_pathout, 
				_capacity, 
				NULL, NULL);

			if (iLength[2] == 0) {
				iLength[2] = WideCharToMultiByte(
					CP_UTF8,
					0,
					wBuffer2,
					iLength[1],
					NULL,
					0,
					NULL, NULL);
			}
			if (iLength[2] == 0) {
				break;
			}
			if (iLength[2] <= _capacity && _dirnamelength) {
				INT32 i; 
				for (i = iLength[2] - 1; i >= 0; --i) {
					if (_pathout[i] == '\\') {
						*_dirnamelength = i;
						break;
					}
				}
			} 
			iLength[0] = iLength[2]; 
			break;
		} 
		if (wPath1 != wBuffer1)
			free(wPath1);
		return iLength[0];
	} 


	static INT32 Win32_GetModPath(
		PCHAR _cPathOut,
		INT32 _iCapacity,
		PINT  _iDirnameLength) {

		HMODULE hModule;
		INT32   iLength;
		DWORD   dwFlags;
		BOOL    bResult;

		bResult = FALSE;
		iLength = -1;
		dwFlags = 
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | \
			GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT;

	    #if defined(_MSC_VER)
	    #  pragma warning(push)
	    #  pragma warning(disable: 4054)
	    #endif
		bResult = GetModuleHandleEx(
			dwFlags, 
			(LPCTSTR)Z_ReturnAddress(),
			&hModule);
		#if defined(_MSC_VER)
		#pragma warning(pop)
		#endif

		if (bResult == TRUE) {
			iLength = _Win32BasePath(
				hModule, 
				_cPathOut, 
				_iCapacity, 
				_iDirnameLength);
		} 
		return iLength;
	}


#  define Win32_GetExePath(out, cap, len)\
   _Win32BasePath(NULL, out, cap, len)


#ifdef __cplusplus
}
#endif 
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/