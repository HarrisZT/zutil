/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zplatform_win32.h 
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
#ifndef __ZPLATFORM_WIN32_H__
#define __ZPLATFORM_WIN32_H__

#include "zconfig.h" 

#if (Z_PLATFORM_WINDOWS) 
#  if (Z_COMPILER_MSVC)
#    pragma warning(disable : 4091)
#  elif (Z_COMPILER_CLANG)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wnonportable-system-include-path"
#  endif 



//TODO: do we need to bother with lean_and_mean? 
//      it causes issues... 
//      including windows.h without lean_and_mean is simpler

//#  define WIN32_LEAN_AND_MEAN 
#  include <Windows.h> 
//#  include <WinSock2.h> 
//#  include <IPTypes.h>
//#  include <WS2tcpip.h>
//#  include <iphlpapi.h>
//#  include <share.h>
//#  include <io.h>
//#  include <shellapi.h>
//#  include <ShlObj.h>
//#  include <DbgHelp.h>
//#  include <crtdbg.h> 
#  include <timeapi.h>

#  if (Z_COMPILER_CLANG)
#    undef  WINAPI
#    define WINAPI STDCALL 
#  endif 



/*
win32 version identification*/
typedef enum _ZWINVER { 
	ZWINVER_XP,        //windows xp or greater
	ZWINVER_XP1,       //windows xp service pack 1 or greater
	ZWINVER_XP2,       //windows xp service pack 2 or greater
	ZWINVER_XP3,       //windows xp service pack 3 or greater
	ZWINVER_VISTA,     //windows vista or greater
	ZWINVER_VISTA1,    //windows vista service pack 1 or greater
	ZWINVER_VISTA2,    //windows vista service pack 2 or greater
	ZWINVER_7,         //windows 7 or greater
	ZWINVER_71,        //windows 7 service pack 1 or greater
	ZWINVER_8,         //windows 8 or greater
	ZWINVER_81,        //windows 8 service pack 1 or greater
	ZWINVER_CURRENT,   //windows current version (threshold) 
	ZWINVER_UNDEFINED, //Windows 2000 or less 
} ZWINVER;

/*
Returns an enum value representing the 
version of windows running on this computer*/
extern ZWINVER ZAPI
ZWin32_GetVersion(Void);



#endif 
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/   