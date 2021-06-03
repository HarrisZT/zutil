/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zsystem.cpp
* Desc: interface for retreiving system info
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
#include <string>
#include "zbasepath/zbasepath.h" 
#include "zutil/zsystem.h" 

#if (Z_PLATFORM_WINDOWS)
#  pragma comment(lib, "winmm.lib")
#  include <Windows.h>
#else
#  include <sys/types.h>
#  include <sys/ptrace.h> 
#endif 

#define Z_MAJOR_VERSION 0
#define Z_MINOR_VERSION 1
#define Z_PATCH_VERSION 5 

 
 

static Lpcstr ZAPI
ZSystemInfo_GetPlatformId(Void) {
#if (Z_PLATFORM_WINDOWS)
	return "WINDOWS";
#elif (Z_PLATFORM_ANDROID)
	return "ANDROID";
#elif (Z_PLATFORM_APPLE)
	return "APPLE";
#elif (Z_PLATFORM_BSD)
	return "BSD";
#elif (Z_PLATFORM_IOS)
	return "IOS";
#elif (Z_PLATFORM_LINUX) 
	return "LINUX";
#elif (Z_PLATFORM_MACOS) 
	return "MACOS";
#elif (Z_PLATFORM_RASPBERRYPI)
	return "RASPBERRYPI";
#else     
	return "UNSUPPORTED";
#endif 
}


static Lpcstr ZAPI
ZSystemInfo_GetArchId(Void) {
#if (Z_ARCHITECTURE_X86)
	return "X86";
#elif (Z_ARCHITECTURE_IA64)
	return "IA-64";
#elif (Z_ARCHITECTURE_X86_64)
	return "X86_64";
#elif (Z_ARCHITECTURE_ARM8_64)
	return "ARM64V8";
#elif (Z_ARCHITECTURE_ARM6) 
	return "ARMV6";
#elif (Z_ARCHITECTURE_ARM7) 
	return "ARMV7";
#elif (Z_ARCHITECTURE_PPC) 
	return "PPC";
#elif (Z_ARCHITECTURE_PPC_64)
	return "PPC64";
#elif (Z_ARCHITECTURE_ARM5)
	return "ARMV5";
#elif (Z_ARCHITECTURE_MIPS)
	return "MIPS";
#elif (Z_ARCHITECTURE_MIPS_64)
	return "MIPS64";
#else
	return "UNSUPPORTED";
#endif
} 


static std::string
ZSystemInfo_GetExePath() {
	Char*       path;
	std::string str;
	Uint32      last_slash_idx;

	path = ZGetBasePath();
	str  = std::string(path);

	last_slash_idx = str.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
		str.erase(last_slash_idx + 1, str.length());

	free(path);
	path = NULL;
	return str;
}


static std::string
ZSystemInfo_GetNameOfExe(Void) {
	Char*       path;
	std::string str;
	Uint32      last_slash_idx;
	Uint32      period_idx;

	path = ZGetBasePath();
	str  = std::string(path);

	last_slash_idx = str.find_last_of("\\/");
	if (std::string::npos != last_slash_idx) {
		str.erase(0, last_slash_idx + 1);
	}
	period_idx = str.rfind('.');
	if (std::string::npos != period_idx)  
		str.erase(period_idx); 

	free(path);
	path = NULL;
	return str;
}

 
Void
ZSystem_GetInfo(
    _Inout_ ZSystemInfo* _lpInfo) {

	if (_lpInfo) { 
		_lpInfo->platformId = ZSystemInfo_GetPlatformId();  
		_lpInfo->archId     = ZSystemInfo_GetArchId();
		_lpInfo->compilerId = Z_COMPILER_NAME;   

		strcpy(_lpInfo->basepath, ZSystemInfo_GetExePath().c_str());
		strcpy(_lpInfo->exeTitle, ZSystemInfo_GetNameOfExe().c_str());

	#if (Z_PLATFORM_WINDOWS)
		_lpInfo->dirsep  = '\\'; 
		_lpInfo->isDebug = IsDebuggerPresent();
	#else 
	    _lpInfo->dirsep  = '/';
		_lpInfo->isDebug = Z_FALSE;
		if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0)
			_lpInfo->isDebug = Z_TRUE;
		else ptrace(PTRACE_DETACH, 0, 1, 0);
	#endif
		_lpInfo->version.major = Z_MAJOR_VERSION;
		_lpInfo->version.minor = Z_MINOR_VERSION;
		_lpInfo->version.patch = Z_PATCH_VERSION;

	#if (Z_ENDIAN_BIG)
		_lpInfo->endian = "big";
	#else 
		_lpInfo->endian = "little";
	#endif 
	}  
} 


Void 
ZSystem_Sleep(
	_In_ Dword _dwMillisecs) {

#if (Z_PLATFORM_WINDOWS)
	TIMECAPS tcaps;
	timeGetDevCaps(&tcaps, sizeof(TIMECAPS));
	timeBeginPeriod(tcaps.wPeriodMin);
	Sleep(_dwMillisecs);
	timeEndPeriod(tcaps.wPeriodMin);
#else
	Uint64    usecs;
	ZTimeSpec ti;
	usecs      = _dwMillisecs * 1000;
	ti.tv_nsec = (usecs % 1000000) * 1000;
	ti.tv_sec  = usecs / 1000000;
	while ((nanosleep(&ti, &ti) == -1) && (errno == EINTR)) {
	}
#endif
} 
/*****************************************************************************/  
//EOF
/*****************************************************************************/  
