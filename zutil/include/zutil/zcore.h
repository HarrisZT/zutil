/****************************************************************************** 
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zcore.h 
* Desc: common core types header
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
#ifndef __ZCORE_H__
#define __ZCORE_H__   

#include <stdio.h>  
#include <ctype.h>  
#include <assert.h>  
#include <stdarg.h> 
#include <stdlib.h> 
#include <string.h>  
#include <stddef.h>
#include <memory.h>

#include "zsal.h" 
#include "zinteger.h" 
#include "zresult.h"

#if (!Z_PLATFORM_WINDOWS)
#  include <wchar.h>
#endif
#if (Z_PLATFORM_POSIX && !Z_PLATFORM_APPLE)
#  include <sys/types.h>
#endif 
#if defined(__cplusplus)
extern "C" {
#endif



/*  
** Section 1:
** Define common utility macros*/ 
//***************************************************************************// 
#define Z_UNDEFINED   -0x0001 //specifies invalid or undefined enum value 
#define Z_FALSE        0x0000 //boolean false
#define Z_TRUE         0x0001 //boolean true
#define Z_MAXPATH      0x0104 //maximum valid length for a filesystem path  
#define Z_OPAQUE       0x00FF //full alpha value
#define Z_TRANSPARENT  0x0000 //null alpha value 

#define Z_INITZERO      { 0, }    //initialize a struct with 0's
#define Z_Unused(x)     (void)(x) //nullify an unused parameter
#define Z_ArraySize(x)  (sizeof(x) / sizeof(x[0])) 
#define Z_ZeroMemory(x) memset(&(x), 0, sizeof((x))) 
#define Z_Max(a, b)     (((a) > (b)) ? (a) : (b))
#define Z_Min(a, b)     (((a) < (b)) ? (a) : (b)) 

#define Z_SafeFree(p) do {        \
  if (p) { free(p); (p) = NULL; } \
} while (0) 

	 

/*
** Section 2: 
** Define common base types*/
//***************************************************************************//  
typedef int              Bool;    //boolean variable (value of 1 or 0)
typedef unsigned char    Byte;    //a byte (8 bits)
typedef char             Char;    //8-bit (ANSI) character 
typedef char             Char8;   //8-bit (ANSI) character 
typedef z_uint_least16_t Char16;  //16-bit (ANSI) character 
typedef z_uint_least32_t Char32;  //32-bit (ANSI) character 
typedef unsigned long    Dword;   //32-bit unsigned integer 
typedef float            Float;   //floating-point variable 
typedef void*            Handle;  //a handle to an object or data 
typedef z_int_fast8_t    Int8;	  //8-bit signed integer
typedef z_int_fast16_t   Int16;	  //16-bit signed integer
typedef z_int_fast32_t   Int32;	  //32-bit signed integer
typedef z_int_fast64_t   Int64;   //64-bit signed integer 
typedef z_intptr_t       Intptr;  //signed integer for pointer precision
typedef long             Long;    //32-bit signed integer
typedef const char*      Lpcstr;  //ptr to const null-terminated 8-bit chars 
typedef const void*      Lpcvoid; //pointer to a constant of any type
typedef void*            Lpvoid;  //a pointer to any type
typedef const wchar_t*   Lpwstr;  //ptr to const null-terminated 16-bit chars 
typedef float            Real32;  //32-bit floating-point (real number)
typedef double           Real64;  //64-bit floating-point (real number)
typedef z_uintptr_t      SizeT;   //max bytes to which a pointer can point
typedef unsigned char    Uchar;   //an unsigned char
typedef z_uint_fast8_t   Uint8;	  //8-bit unsigned integer
typedef z_uint_fast16_t  Uint16;  //16-bit unsigned integer
typedef z_uint_fast32_t  Uint32;  //32-bit unsigned integer
typedef z_uint_fast64_t  Uint64;  //64-bit unsigned integer
typedef z_uintptr_t      Uintptr; //unsigned integer for pointer precision
typedef unsigned long    Ulong;   //unsigned long int
typedef unsigned short   Ushort;  //unsigned short int
#define Void             void     //no parameters or data returned
typedef wchar_t          Wchar;   //16-bit unicode character
typedef unsigned short   Word;    //16-bit unsigned integer.



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  