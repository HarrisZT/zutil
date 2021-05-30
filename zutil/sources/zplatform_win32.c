/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zplatform_win32.c
* Desc: win32 includes and functions 
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
#include "zutil/zcore.h"
#include "zutil/zplatform_win32.h"





static Bool 
IsWinVersionOrGreater(
	_In_ Word _wMajor, 
	_In_ Word _wMinor, 
	_In_ Word _wPatchMajor) { 

	Bool             bResult;
	OSVERSIONINFOEXW osvi;
	DWORDLONG        dwlConditionMask; 

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    osvi.dwMajorVersion      = _wMajor;
    osvi.dwMinorVersion      = _wMinor;
    osvi.wServicePackMajor   = _wPatchMajor;

	dwlConditionMask = 
		VerSetConditionMask(
        VerSetConditionMask(
        VerSetConditionMask(
            0, VER_MAJORVERSION,     VER_GREATER_EQUAL),
               VER_MINORVERSION,     VER_GREATER_EQUAL),
               VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);

	bResult = VerifyVersionInfoW(
		&osvi, 
		VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, 
		dwlConditionMask);
	 
	return bResult != FALSE;
}


ZWINVER 
ZWin32_GetVersion(Void) {

#if (Z_PLATFORM_WINRT)
	return ZWINVER_CURRENT;
#endif
	if (IsWinVersionOrGreater(
		HIBYTE(_WIN32_WINNT_WINTHRESHOLD),
		LOBYTE(_WIN32_WINNT_WINTHRESHOLD), 0)) {
		return ZWINVER_CURRENT;
	}
	if (IsWinVersionOrGreater(
		HIBYTE(_WIN32_WINNT_WINBLUE), LOBYTE(_WIN32_WINNT_WINBLUE), 1)) {
		return ZWINVER_81;
	}
	if (IsWinVersionOrGreater(
		HIBYTE(_WIN32_WINNT_WIN8), LOBYTE(_WIN32_WINNT_WIN8), 0)) {
		return ZWINVER_8;
	}
	if (IsWinVersionOrGreater(
		HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 1)) {
		return ZWINVER_71;
	} 
	if (IsWinVersionOrGreater(
		HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 0)) {
		return ZWINVER_7;
	}
	if (IsWinVersionOrGreater(
		HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 2)) {
		return ZWINVER_VISTA2;
	} 
	if (IsWinVersionOrGreater(
		HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 1)) {
		return ZWINVER_VISTA1;
	}
	if (IsWinVersionOrGreater(
		HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 0)) {
		return ZWINVER_VISTA;
	} 
	if (IsWinVersionOrGreater(
		HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 3)) {
		return ZWINVER_XP3;
	} 
	if (IsWinVersionOrGreater(
		HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 2)) {
		return ZWINVER_XP2;
	}
	if (IsWinVersionOrGreater(
		HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 1)) {
		return ZWINVER_XP1;
	}
	if (IsWinVersionOrGreater(
		HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 0)) {
		return ZWINVER_XP;
	}
	return ZWINVER_UNDEFINED; 
} 
/*****************************************************************************/  
//EOF
/*****************************************************************************/ 