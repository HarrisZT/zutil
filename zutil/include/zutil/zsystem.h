/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zsystem.h 
* Desc: interface for retreiving system info and system utilities
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
#ifndef __ZSYSTEM_H__
#define __ZSYSTEM_H__  

#include "zcore.h" 
#if defined(__cplusplus)
extern "C" {
#endif



/*
interface for retreiving system info*/
typedef struct {
	struct {
		Byte major;    //major version 
		Byte minor;    //minor version 
		Byte patch;    //update version 
	} version;         //identification of library version

	int    isDebug;       //binary is debug or release build? 
	Char   exeTitle[260]; //name of the program executable 
	Char   basepath[260]; //the base path (where exe was run from)
	Lpcstr dirsep;        //directory seperator (platform dependent)
	Lpcstr platformId;    //identification of platform 
	Lpcstr compilerId;    //identification of compiler
	Lpcstr archId;        //identification of architecture
	Lpcstr endian;        //system architecture is big or little endian? 
} ZSystemInfo;


/*
Initialize the given structure with system information.
@_lpInfo: structure to initialize, must be preallocated*/
extern Void ZAPI
ZSystem_GetInfo(
    _Inout_ ZSystemInfo* _lpInfo); 


/*
Pause program execution for a specified amount of time in milliseconds.
@_millisecs: the amount of time to pause for*/
extern Void ZAPI
ZSystem_Sleep(
    _In_ Dword _millisecs);



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  