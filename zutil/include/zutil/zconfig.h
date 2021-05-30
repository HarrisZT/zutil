/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zconfig.h 
* Desc: configure compiler
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
#ifndef __ZCONFIG_H__
#define __ZCONFIG_H__  

#include "zplatform.h"



/*
Convert argument to string constant
******************************************************************************/
#if !defined(_STR) 
#  define _STR(x) #x
#  define _MKSTR(x) _STR(x)
#endif 

/*
Define the proper null for C/C++
******************************************************************************/
#if !defined(__MACH__)
#  if !defined(NULL)
#    if defined(__cplusplus)
#      define NULL 0
#    else
#      define NULL ((void*)0)
#    endif
#  endif 
#endif 

/*
POSIX gnu source macro may be required 
*****************************************************************************/
#if (Z_PLATFORM_POSIX)
#  if !defined(_GNU_SOURCE)
#    define _GNU_SOURCE
#  endif
#endif 

/*
Directory seperator*/
#if (Z_PLATFORM_WINDOWS)
#  define Z_DIR_SEP '\\'
#else
#  define Z_DIR_SEP '/'
#endif

/*
Get Platform and Compiler Config:
******************************************************************************/ 
#if defined(_MSC_VER)
#  include "zcompiler_msvc.h"
#elif defined(__GNUC__)
#  include "zcompiler_gcc.h"
#elif defined(__clang__)
#  include "zcompiler_clang.h" 
#else
#  error Unsupported compiler.
#endif  

/*
F2D uses C calling convention (__cdecl) for C/C++ code 
******************************************************************************/
#if !defined(ZAPI)
#  if (Z_PLATFORM_WINDOWS && !Z_COMPILER_GCC) 
#    define ZAPI __cdecl 
#  else
#    define ZAPI
#  endif
#endif
#if !defined(WINAPI)
#  define WINAPI __stdcall
#endif 

/*
Build-type specifications:
******************************************************************************/
#if defined(_DEBUG)
#  undef  Z_BUILD_DEBUG 
#  define Z_BUILD_DEBUG 1 //this is a debug mode build
#endif  
 
#if (Z_ARCHITECTURE_ENDIANBIG == 1)
#  undef  Z_ENDIAN_BIG
#  define Z_ENDIAN_BIG 1 //platform is big endian
#endif

#if (Z_ARCHITECTURE_ENDIANLIL == 1)
#  undef  Z_ENDIAN_LIL
#  define Z_ENDIAN_LIL 1 //platform is little endian
#endif  

/*
Is 32 or 64 bit?*/ 
#if (Z_ARCHITECTURE_ARM_64 ||\
     Z_ARCHITECTURE_X86_64 ||\
     Z_ARCHITECTURE_PPC_64 ||\
     Z_ARCHITECTURE_IA64   ||\
     Z_ARCHITECTURE_MIPS_64)

#  undef  Z_BUILD_64BIT 
#  define Z_BUILD_64BIT 1 //this is a 64-bit build
#else 
#  undef  Z_BUILD_32BIT 
#  define Z_BUILD_32BIT 1 //this is a 32-bit build
#endif 

/* 
** Define Real number (floats)
******************************************************************************/ 
#define FLOAT32_C(x) (x##f)
#define FLOAT64_C(x) (x) 
 
#if (Z_BUILD_64BIT)
   typedef double FloatPtr; //real number 64-Bit (floating)
#  define FLOATPTR_C(x) FLOAT64_C(x)
#else			     
   typedef float FloatPtr;  //real number 32-Bit (floating)
#  define FLOATPTR_C(x) FLOAT32_C(x)
#endif 

#define Z_FLT_MANTISSA_BITS 0x0017
#define Z_FLT_EXPONENT_BITS 0x0008
#define Z_FLT_EXPONENT_BIAS 0x007F
#define Z_FLT_SIGN_BIT	    0x001F 
#define Z_FLT_SIGN_MASK	    (1UL << Z_FLT_SIGN_BIT)
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/   