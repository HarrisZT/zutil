/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zinteger.h 
* Desc: provides definitions of integer types 
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
#ifndef __ZINTEGER_H__
#define __ZINTEGER_H__ 

#include "zconfig.h"

 

/*
** 8-bit integer types*/
//---------------------------------------------------------------------------//
typedef signed char        z_int8_t;        //default signed 8-bit int 
typedef signed char        z_int_least8_t;  //signed int at least 8-bits
typedef signed char        z_int_fast8_t;   //fastest signed 8-bit int 
typedef unsigned char      z_uint8_t;		//default unsigned 8-bit int 
typedef unsigned char      z_uint_least8_t; //unsigned int at least 8-bits
typedef unsigned char      z_uint_fast8_t;  //fastest unsigned 8-bit int 
/*
** 16-bit integer types*/
//---------------------------------------------------------------------------//
typedef signed short       z_int16_t;		 //default signed 16-bit int 
typedef signed short       z_int_least16_t;  //signed int at least 16-bits
typedef signed int         z_int_fast16_t;   //fastest signed 16-bit int  
typedef unsigned short     z_uint16_t;	     //default unsigned 16-bit int 
typedef unsigned short     z_uint_least16_t; //unsigned int at least 16-bits
typedef unsigned int       z_uint_fast16_t;  //fastest unsigned 16-bit int 
/*
** 32-bit integer types*/
//---------------------------------------------------------------------------//
typedef signed int         z_int32_t;		 //default signed 32-bit inte 
typedef signed int         z_int_least32_t;  //signed int at least 32-bits
typedef signed int         z_int_fast32_t;   //fastest signed 32-bit int 
typedef unsigned int       z_uint32_t;	     //default unsigned 32-bit int 
typedef unsigned int       z_uint_least32_t; //unsigned int at least 32-bits
typedef unsigned int       z_uint_fast32_t;  //fastest signed 32-bit int 
/*
** 64-bit integer types*/
//---------------------------------------------------------------------------//
typedef long long          z_int64_t;		 //default signed 64-bit int  
typedef long long          z_int_least64_t;  //signed int at least 64-bits
typedef long long          z_int_fast64_t;   //fastest signed 64-bit int 
typedef unsigned long long z_uint64_t; 	     //default unsigned 64-bit int 
typedef unsigned long long z_uint_least64_t; //unsigned int at least 64-bits
typedef unsigned long long z_uint_fast64_t;  //fastest signed 64-bit int 
/*
** Max integer types*/
//---------------------------------------------------------------------------//
typedef long long          z_intmax_t;       //max size signed integer 
typedef unsigned long long z_uintmax_t;      //max size unsigned integer 

#if (Z_BUILD_64BIT)
#  define Z_SIZEOF_VOIDP 0x8U		  //64-bit (8-byte) pointer size
   typedef z_uint64_t    z_uintptr_t; //64-bit (8-byte) pointer size
   typedef z_int64_t     z_intptr_t;  //64-bit (8-byte) pointer size
#else					   
#  define Z_SIZEOF_VOIDP 0x4U		  //32-bit (4-byte) pointer size
   typedef z_uint32_t    z_uintptr_t; //32-bit (4-byte) pointer size
   typedef z_int32_t     z_intptr_t;  //32-bit (4-byte) pointer size
#endif 



/* Section II:
** define integer type prefixes
******************************************************************************/
#if (Z_COMPILER_MSVC)
#  define PRId32  "Id"
#  define PRIi32  "Ii"
#  define PRIo32  "Io"
#  define PRIu32  "Iu"
#  define PRIx32  "Ix"
#  define PRIX32  "IX"
#  define PRId64  "I64d"
#  define PRIi64  "I64i"
#  define PRIo64  "I64o"
#  define PRIu64  "I64u"
#  define PRIx64  "I64x"
#  define PRIX64  "I64X"
#  define PRIdPTR "Id"
#  define PRIiPTR "Ii"
#  define PRIoPTR "Io"
#  define PRIuPTR "Iu"
#  define PRIxPTR "Ix"
#  define PRIXPTR "IX"
#  define PRIsize "Iu"
#else
#  include <inttypes.h>
#  define PRIsize "zu"
#endif 

#define PRItick PRIi64
#define PRIhash PRIx64

#if (Z_BUILD_64BIT)
#  define PRIreal "lf"
#else			  
#  define PRIreal "f"
#endif

#if (Z_COMPILER_MSVC)
#  if (Z_BUILD_64BIT)
#    define PRIfixPTR "016I64X"
#  else				  
#    define PRIfixPTR "08IX"
#  endif
#else
#  if (Z_BUILD_64BIT)
#    define PRIfixPTR "016" PRIXPTR
#  else				  
#    define PRIfixPTR "08" PRIXPTR
#  endif
#endif


   	 
/* Section III:
** integer range macros*/
//***************************************************************************//
#define Z_INT8_MIN       (-0x000000000000007F - 1)
#define Z_INT16_MIN      (-0x0000000000007FFF - 1)
#define Z_INT32_MIN      (-0x000000007FFFFFFF - 1)
#define Z_INT64_MIN      (-0x7FFFFFFFFFFFFFFF - 1)
#define Z_INT8_MAX       0x000000000000007F
#define Z_INT16_MAX      0x0000000000007FFF 
#define Z_INT32_MAX      0x000000007FFFFFFF
#define Z_INT64_MAX      0x7FFFFFFFFFFFFFFF 
#define Z_UINT8_MAX      0x00000000000000FFU
#define Z_UINT16_MAX     0x000000000000FFFFU
#define Z_UINT32_MAX     0x00000000FFFFFFFFU
#define Z_UINT64_MAX     0xFFFFFFFFFFFFFFFFU
#define INT8_SIGN_BIT	 0x0007
#define INT16_SIGN_BIT	 0x000F
#define INT32_SIGN_BIT	 0x001F
#define INT64_SIGN_BIT	 0x003F 
#define INT8_SIGN_MASK	 (1 << INT8_SIGN_BIT)
#define INT16_SIGN_MASK	 (1 << INT16_SIGN_BIT)
#define INT32_SIGN_MASK	 (1UL << INT32_SIGN_BIT)
#define INT64_SIGN_MASK	 (1ULL << INT64_SIGN_BIT)    

#define Z_INT_LEAST8_MIN   Z_INT8_MIN
#define Z_INT_LEAST16_MIN  Z_INT16_MIN
#define Z_INT_LEAST32_MIN  Z_INT32_MIN
#define Z_INT_LEAST64_MIN  Z_INT64_MIN
#define Z_INT_LEAST8_MAX   Z_INT8_MAX
#define Z_INT_LEAST16_MAX  Z_INT16_MAX
#define Z_INT_LEAST32_MAX  Z_INT32_MAX
#define Z_INT_LEAST64_MAX  Z_INT64_MAX
#define Z_UINT_LEAST8_MAX  Z_UINT8_MAX
#define Z_UINT_LEAST16_MAX Z_UINT16_MAX
#define Z_UINT_LEAST32_MAX Z_UINT32_MAX
#define Z_UINT_LEAST64_MAX Z_UINT64_MAX 
#define Z_INT_FAST8_MIN    Z_INT8_MIN
#define Z_INT_FAST16_MIN   Z_INT32_MIN
#define Z_INT_FAST32_MIN   Z_INT32_MIN
#define Z_INT_FAST64_MIN   Z_INT64_MIN
#define Z_INT_FAST8_MAX    Z_INT8_MAX
#define Z_INT_FAST16_MAX   Z_INT32_MAX
#define Z_INT_FAST32_MAX   Z_INT32_MAX
#define Z_INT_FAST64_MAX   Z_INT64_MAX
#define Z_UINT_FAST8_MAX   Z_UINT8_MAX
#define Z_UINT_FAST16_MAX  Z_UINT32_MAX
#define Z_UINT_FAST32_MAX  Z_UINT32_MAX
#define Z_UINT_FAST64_MAX  Z_UINT64_MAX

#if (Z_BUILD_64BIT)
#  define Z_INTPTR_MIN   Z_INT64_MIN
#  define Z_INTPTR_MAX   Z_INT64_MAX
#  define Z_UINTPTR_MAX  Z_UINT64_MAX
#else
#  define Z_INTPTR_MIN   Z_INT32_MIN
#  define Z_INTPTR_MAX   Z_INT32_MAX
#  define Z_UINTPTR_MAX  Z_UINT32_MAX
#endif 
#define Z_INTMAX_MIN  Z_INT64_MIN
#define Z_INTMAX_MAX  Z_INT64_MAX
#define Z_UINTMAX_MAX Z_UINT64_MAX 
#define Z_PTRDIFF_MIN Z_INTPTR_MIN
#define Z_PTRDIFF_MAX Z_INTPTR_MAX

#if !defined(Z_SIZE_MAX)
#  define Z_SIZE_MAX Z_UINTPTR_MAX
#endif 
#define Z_SIG_ATOMIC_MIN Z_INT32_MIN
#define Z_SIG_ATOMIC_MAX Z_INT32_MAX 

#define Z_INT8_C(x)    (x)
#define Z_INT16_C(x)   (x)
#define Z_INT32_C(x)   (x)
#define Z_INT64_C(x)   (x ## LL) 
#define Z_UINT8_C(x)   (x)
#define Z_UINT16_C(x)  (x)
#define Z_UINT32_C(x)  (x ## U)
#define Z_UINT64_C(x)  (x ## ULL) 
#define Z_INTMAX_C(x)  Z_INT64_C(x)
#define Z_UINTMAX_C(x) Z_UINT64_C(x) 
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/