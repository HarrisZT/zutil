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

	
	static INT32 
	Win32GetBasePath( 
		_Inout_opt_ PCHAR _pathout,
		_In_        INT32 _capacity) {		
		
		WCHAR  wBuffer1[MAX_PATH];
		WCHAR  wBuffer2[MAX_PATH];
		DWORD  dwSize[2];
		DWORD  dwBufferSize;
		WCHAR* wPath1 = NULL; 
		WCHAR* wPath2 = NULL;
		INT32  iLength[3] = { -1, };
		
		for (;;) { 
			dwBufferSize = (DWORD)(sizeof(wBuffer1) / sizeof(wBuffer1[0]));

			dwSize[0] = GetModuleFileNameW(
				NULL, 
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
						GetModuleFileNameW(NULL, wPath1, dwSize[1]);

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
			iLength[0] = iLength[2]; 
			break;
		} 
		if (wPath1 != wBuffer1)
			free(wPath1);
		return iLength[0];
	} 


#ifdef __cplusplus
}
#endif 
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/
