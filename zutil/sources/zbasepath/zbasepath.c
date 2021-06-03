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
#  define _ZBasePath Win32GetBasePath 

#elif defined(__linux__) || defined(__CYGWIN__) || defined(__sun) 
#  include "zbasepath_linux.h"
#  define _ZBasePath LinuxGetBasePath  

#elif defined(__APPLE__)
#  include "zbasepath_apple.h"
#  define _ZBasePath AppleGetBasePath  

#elif defined(__QNXNTO__) 
#  include "zbasepath_qnxnto.h"
#  define _ZBasePath QnxntoGetBasePath  

#elif defined(__DragonFly__) || defined(__FreeBSD__) || \
      defined(__FreeBSD_kernel__) || defined(__NetBSD__)
#  include "zbasepath_bsd.h"
#  define _ZBasePath BsdGetBasePath  
#else 
#  error unsupported platform 
#endif

	   	
Char* 
ZGetBasePath(Void) { 
	Char* out;
	Int32 len;

	len = _ZBasePath(NULL, 0);
	out = (Char*)malloc(len + 1);
	if (!out)
		return NULL; //error

	_ZBasePath(out, len);
	out[len] = '\0';
	return out;
}


#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
//EOF
/*****************************************************************************/ 
