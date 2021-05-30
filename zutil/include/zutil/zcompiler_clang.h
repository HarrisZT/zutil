/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zcompiler_clang.h 
* Desc: compiler configuration for clang
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
#ifndef __ZCOMPILER_CLANG_H__
#define __ZCOMPILER_CLANG_H__  
 


/*
Compiler Description:
******************************************************************************/
#if defined(__clang__)

#undef  Z_COMPILER_CLANG
#undef  Z_COMPILER_NAME
#undef  Z_COMPILER_VER 
#undef  Z_COMPILER_DESC 
 
#define Z_COMPILER_CLANG 1 
#define Z_COMPILER_NAME  "clang"
#define Z_COMPILER_VER   ((__clang_major__ * 100) + (__clang_minor__ * 10))
#define Z_COMPILER_DESC  (Z_COMPILER_NAME " " _MKSTR(Z_COMPILER_VER))  

/*
Branch-prediction helpers
******************************************************************************/
#define IF(condition, hint)    if (__builtin_expect(!!(condition), hint))
#define IF_UNLIKELY(condition) if (__builtin_expect(!!(condition), 0))
#define IF_LIKELY(condition)   if (__builtin_expect(!!(condition), 1))
#define LIKELY(x)              __builtin_expect(!!(x), 1)
#define UNLIKELY(x)            __builtin_expect(!!(x), 0) 

/*
declaration specification helpers
******************************************************************************/ 
#define ALIGNAS(size) __attribute__((__##aligned##__(size)))  
#define ALIGNOF(type) __alignof__(type) 
#define THREADLOCAL   _Thread_local
#define INLINE        inline
#define FORCEINLINE   __attribute__((always_inline)) inline 
#define NOINLINE      __attribute__ ((noinline))
#define NOINLINEWEAK  __attribute__ ((noinline)) __attribute__((weak))
#define NORETURN      _Noreturn
 
/*
Disable/Enable specific compiler warnings. 
******************************************************************************/
#if (Z_DISABLE_COMPILER_WARNINGS)
#  if !defined(Z_DISABLE_WARNING_UNUSED_VARIABLES) 
#    define Z_DISABLE_WARN_UNUSED_VARIABLES() \
       _Pragma("clang diagnostic push") \
       _Pragma("clang diagnostic ignored \"-Wunused-variable\"")
#    define Z_RESTORE_WARN_UNUSED_VARIABLES() \
       _Pragma("clang diagnostic pop") 
#  endif
#endif 

/*
Compiler Features:
******************************************************************************/
#if __has_feature(cxx_exceptions)
	#define Z_COMPILER_EXCEPTIONS 1
#endif
#if __has_feature(cxx_rtti)
	#define Z_COMPILER_RTTI 1
#endif
/*
__FUNC__ is like __func__, but it has the class name*/
#define __FUNC__ __PRETTY_FUNCTION__
#define Z_FUNC_HAS_SIGNATURE 1

/*
PREfast not supported*/
#define PREFAST_SUPPRESS_WARNING(W)
#define PREFAST_ASSUME(cond)
#define _Out_writes_z_(x)
#define _Inout_updates_z_(x)

#ifdef  _MSC_VER
//(For clang on MSBuild)
#  define stricmp   _stricmp
#  define strnicmp  _strnicmp
#  define wcsicmp   _wcsicmp
#  define wcsnicmp  _wcsnicmp
#  define alloca    _alloca
#  define itoa      _itoa
#endif 

/*
type-checking helper*/
#define PRINTF_PARAMS(...) __attribute__((format(printf, __VA_ARGS__)))
#define SCANF_PARAMS(...)  __attribute__((format(scanf, __VA_ARGS__)))

/*
Barrier to prevent R/W reordering by the compiler.
Note: 
  This does not emit any instruction, 
  nor does it prevent CPU reordering*/
#define MEMORY_RW_REORDERING_BARRIER asm volatile ("" ::: "memory") 


#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/ 