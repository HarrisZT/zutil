/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zcompiler_msvc.h 
* Desc: compiler configuration for msvc
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
#ifndef __ZCOMPILER_MSVC_H__
#define __ZCOMPILER_MSVC_H__  


	  
/*
Compiler Description:
******************************************************************************/
#if defined(_MSC_VER)

#undef  Z_COMPILER_MSVC 
#undef  Z_COMPILER_NAME
#undef  Z_COMPILER_VER 
#undef  Z_COMPILER_DESC

#define Z_COMPILER_MSVC 1 
#define Z_COMPILER_NAME "msvc"
#define Z_COMPILER_VER  (_MSC_VER) 
#define Z_COMPILER_DESC (Z_COMPILER_NAME " " _MKSTR(Z_COMPILER_VER)) 

/*
Branch-prediction helpers
******************************************************************************/
#define IF(condition, hint)    if (condition)
#define IF_UNLIKELY(condition) if (condition)
#define IF_LIKELY(condition)   if (condition)
#define LIKELY(x)              (x)
#define UNLIKELY(x)            (x)  

/*
declaration specification helpers
******************************************************************************/
#define ALIGNAS(size) __declspec(align(size))
#define ALIGNOF(type) __alignof(type) 
#define THREADLOCAL   __declspec(thread) 
#define INLINE        __inline 
#define FORCEINLINE   __forceinline 
#define NOINLINE      __declspec(noinline)
#define NOINLINEWEAK  __declspec(noinline) inline
#define NORETURN      __declspec(noreturn)

/*
Disable/Enable specific compiler warnings. 
******************************************************************************/
#if !defined(_CRT_SECURE_NO_DEPRECATE)
#  define _CRT_SECURE_NO_DEPRECATE
#endif
#if !defined(_CRT_NONSTDC_NO_DEPRECATE)
#  define _CRT_NONSTDC_NO_DEPRECATE
#endif
#if !defined(_CRT_SECURE_NO_WARNINGS)
#  define _CRT_SECURE_NO_WARNINGS
#endif 
#if !defined(NOMINMAX)
#  define NOMINMAX
#endif 

/*
Compiler Features:
******************************************************************************/
#if (Z_COMPILER_VER < 1900) 
//patch alignof and noexcept (VS2012 + VS2013 only)
#  define _ALLOW_KEYWORD_MACROS
#  define alignof  __alignof
#  define noexcept throw()

#  if (Z_COMPILER_VER < 1800) 
#    define va_copy(d,s) ((d)=(s))
#  endif 
#endif 

#if (Z_PLATFORM_WINDOWS) && !defined(STDCALL)
#  define STDCALL __stdcall 
#endif 
#if (Z_COMPILE) && !defined(_CRT_SECURE_NO_WARNINGS)
#  define _CRT_SECURE_NO_WARNINGS 1
#endif 
#if !defined(_LINT)
#  define STATICASSERT static_assert
#endif 
#if defined(_CPPUNWIND)
#  define Z_COMPILER_EXCEPTIONS 1
#endif
#if defined(_CPPRTTI)
#  define Z_COMPILER_RTTI 1
#endif 
//__FUNC__ is like __func__, but it has the class name
#define __FUNC__ __FUNCTION__
#define Z_FUNC_HAS_SIGNATURE 0
/*
PREfast helpers*/
#define PREFAST_SUPPRESS_WARNING(W) __pragma(warning(suppress: W))
#if defined(_PREFAST_)
	#define PREFAST_ASSUME(cond) __analysis_assume(cond)
#else
	#define PREFAST_ASSUME(cond)
#endif 
/*
type-checking helper*/
#define PRINTF_PARAMS(...)
#define SCANF_PARAMS(...)
/*
Barrier to prevent R/W reordering by the compiler.
Note: 
  This does not emit any instruction, 
  nor does it prevent CPU reordering*/
#define MEMORY_RW_REORDERING_BARRIER _ReadWriteBarrier() 



#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/ 