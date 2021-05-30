/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zplatform.h 
* Desc: determines platform and architecture configuration 
*
* information references:
* https://caiorss.github.io/C-Cpp-Notes/Preprocessor_and_Macros.html
* https://tinyurl.com/24yr5fh3
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
#ifndef __ZPLATFORM_H__
#define __ZPLATFORM_H__  



/*
Hardware family identification*/
#define Z_HARDWARETYPE_CONSOLE   0
#define Z_HARDWARETYPE_DESKTOP   0
#define Z_HARDWARETYPE_MOBILE    0 
/*
Architecture identification*/
#define Z_ARCHITECTURE_ARM       0
#define Z_ARCHITECTURE_ARM5      0
#define Z_ARCHITECTURE_ARM6      0
#define Z_ARCHITECTURE_ARM7      0
#define Z_ARCHITECTURE_ARM8      0
#define Z_ARCHITECTURE_ARM_64    0
#define Z_ARCHITECTURE_ARM8_64   0
#define Z_ARCHITECTURE_ENDIANBIG 0
#define Z_ARCHITECTURE_ENDIANLIL 0
#define Z_ARCHITECTURE_GENERIC   0 
#define Z_ARCHITECTURE_IA64      0
#define Z_ARCHITECTURE_MIPS      0
#define Z_ARCHITECTURE_MIPS_64   0
#define Z_ARCHITECTURE_NEON      0
#define Z_ARCHITECTURE_PPC       0
#define Z_ARCHITECTURE_PPC_64    0
#define Z_ARCHITECTURE_SSE2      0
#define Z_ARCHITECTURE_SSE3      0
#define Z_ARCHITECTURE_SSE4      0
#define Z_ARCHITECTURE_SSE4_FMA3 0
#define Z_ARCHITECTURE_THUMB     0 
#define Z_ARCHITECTURE_X86       0
#define Z_ARCHITECTURE_X86_64    0 
/*
Operating system identification*/
#define Z_PLATFORM_ANDROID       0
#define Z_PLATFORM_APPLE         0
#define Z_PLATFORM_BSD           0
#define Z_PLATFORM_IOS           0 
#define Z_PLATFORM_LINUX         0 
#define Z_PLATFORM_MACOS         0
#define Z_PLATFORM_POSIX         0
#define Z_PLATFORM_RASPBERRYPI   0  
#define Z_PLATFORM_WINDOWS       0 
#define Z_PLATFORM_WINRT         0  
/*
Compiler Identification*/
#define Z_COMPILER_CLANG 0
#define Z_COMPILER_GCC   0
#define Z_COMPILER_MSVC  0 
/*
Compiler Config*/
#define Z_COMPILER_NAME "undefined"
#define Z_COMPILER_VER  "undefined"
#define Z_COMPILER_DESC "undefined"
/*
Build Config*/
#define Z_BUILD_DEBUG 0
#define Z_BUILD_64BIT 0
#define Z_BUILD_32BIT 0 
#define Z_ENDIAN_BIG  0
#define Z_ENDIAN_LIL  0
/*
User configurable Options*/
#define Z_DISABLE_COMPILER_WARNINGS 0 





/* 
** Android
******************************************************************************/ 
#if (defined(ANDROID) || defined(__ANDROID__))
#  undef  Z_PLATFORM_ANDROID
#  define Z_PLATFORM_ANDROID 1 
#  undef  Z_PLATFORM_POSIX
#  define Z_PLATFORM_POSIX 1 
#  if defined(__arm__)
#    undef  Z_ARCHITECTURE_ARM
#    define Z_ARCHITECTURE_ARM 1
#    if defined(__ARM_ARCH_7A__)
#      undef  Z_ARCHITECTURE_ARM7
#      define Z_ARCHITECTURE_ARM7 1 
#    elif defined(__ARM_ARCH_5TE__)
#      undef  Z_ARCHITECTURE_ARM5
#      define Z_ARCHITECTURE_ARM5 1 
#    else
#      error Unsupported ARM architecture
#    endif
#  elif defined(__aarch64__)
#    undef  Z_ARCHITECTURE_ARM
#    define Z_ARCHITECTURE_ARM 1
#    undef  Z_ARCHITECTURE_ARM_64
#    define Z_ARCHITECTURE_ARM_64 1 
#    if defined(__ARM_ARCH) && (__ARM_ARCH == 8)
#      undef  Z_ARCHITECTURE_ARM8_64
#      define Z_ARCHITECTURE_ARM8_64 1  
#    endif
#  elif defined(__i386__)
#    undef  Z_ARCHITECTURE_X86
#    define Z_ARCHITECTURE_X86 1 
#  elif defined( __x86_64__ )
#    undef  Z_ARCHITECTURE_X86_64
#    define Z_ARCHITECTURE_X86_64 1 
#  elif (defined(__mips__) && defined(__mips64))
#    undef  Z_ARCHITECTURE_MIPS
#    define Z_ARCHITECTURE_MIPS 1
#    undef  Z_ARCHITECTURE_MIPS_64
#    define Z_ARCHITECTURE_MIPS_64 1 
#    if !defined(_MIPS_ISA)
#      define _MIPS_ISA 7  
#    endif
#  elif defined(__mips__)
#    undef  Z_ARCHITECTURE_MIPS
#    define Z_ARCHITECTURE_MIPS 1 
#    if !defined(_MIPS_ISA)
#      define _MIPS_ISA 6 
#    endif
#  else
#    error Unknown architecture
#  endif 
#  if Z_ARCHITECTURE_MIPS
#    if defined(__MIPSEL__) || defined(__MIPSEL) || defined(_MIPSEL)
#      undef  Z_ARCHITECTURE_ENDIANLIL
#      define Z_ARCHITECTURE_ENDIANLIL 1
#    else
#      undef  Z_ARCHITECTURE_ENDIANBIG
#      define Z_ARCHITECTURE_ENDIANBIG 1
#    endif
#  elif defined(__AARCH64EB__) || defined(__ARMEB__)
#    undef  Z_ARCHITECTURE_ENDIANBIG
#    define Z_ARCHITECTURE_ENDIANBIG 1
#  else
#    undef  Z_ARCHITECTURE_ENDIANLIL
#    define Z_ARCHITECTURE_ENDIANLIL 1
#  endif 
#  undef  Z_HARDWARETYPE_MOBILE
#  define Z_HARDWARETYPE_MOBILE 1 
#  undef  Z_HARDWARETYPE_CONSOLE
#  define Z_HARDWARETYPE_CONSOLE 1 


/*
** Apple Mac and iOS
******************************************************************************/ 
#elif(defined(__APPLE__) && __APPLE__) 
#  undef  Z_PLATFORM_APPLE
#  define Z_PLATFORM_APPLE 1 
#  undef  Z_PLATFORM_POSIX
#  define Z_PLATFORM_POSIX 1

#  include <TargetConditionals.h> 
#  if defined(__IPHONE__) || \
   (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE)  
#    undef  Z_PLATFORM_IOS
#    define Z_PLATFORM_IOS 1 
#    if defined(__arm__)
#      undef  Z_ARCHITECTURE_ARM
#      define Z_ARCHITECTURE_ARM 1
#      if defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7S__)
#        undef  Z_ARCHITECTURE_ARM7
#        define Z_ARCHITECTURE_ARM7 1 
#        if !defined(__ARM_NEON__)
#          error Missing ARM NEON support
#        endif
#      elif defined(__ARM_ARCH_6K__) ||\
            defined(__ARM_ARCH_6J__) ||\
            defined(__ARM_ARCH_6__)
#        undef  Z_ARCHITECTURE_ARM6
#        define Z_ARCHITECTURE_ARM6 1 
#      else
#        error Unrecognized ARM architecture
#      endif
#    elif defined(__arm64__)
#      undef  Z_ARCHITECTURE_ARM
#      define Z_ARCHITECTURE_ARM 1
#      undef  Z_ARCHITECTURE_ARM_64
#      define Z_ARCHITECTURE_ARM_64 1
#      if defined(__ARM64_ARCH_8__)
#        undef  Z_ARCHITECTURE_ARM8_64
#        define Z_ARCHITECTURE_ARM8_64 1 
#      else
#        error Unrecognized ARM architecture
#      endif 
#    else
#      error Unknown architecture
#    endif 
#    undef  Z_ARCHITECTURE_ENDIANLIL
#    define Z_ARCHITECTURE_ENDIANLIL 1 
#    undef  Z_HARDWARETYPE_MOBILE
#    define Z_HARDWARETYPE_MOBILE 1 
#    undef  Z_HARDWARETYPE_CONSOLE
#    define Z_HARDWARETYPE_CONSOLE 1

#  elif defined(__MACH__) 
#    undef  Z_PLATFORM_MACOS
#    define Z_PLATFORM_MACOS 1 
#    if defined(__x86_64__) ||  defined(__x86_64) || defined(__amd64)
#      undef  Z_ARCHITECTURE_X86_64
#      define Z_ARCHITECTURE_X86_64 1
#      undef  Z_ARCHITECTURE_ENDIANLIL
#      define Z_ARCHITECTURE_ENDIANLIL 1 
#    elif defined(__i386__) || defined(__intel__)
#      undef  Z_ARCHITECTURE_X86
#      define Z_ARCHITECTURE_X86 1
#      undef  Z_ARCHITECTURE_ENDIANLIL
#      define Z_ARCHITECTURE_ENDIANLIL 1 
#    elif defined(__powerpc64__) || defined(__POWERPC64__)
#      undef  Z_ARCHITECTURE_PPC_64
#      define Z_ARCHITECTURE_PPC_64 1
#      undef  Z_ARCHITECTURE_ENDIANBIG
#      define Z_ARCHITECTURE_ENDIANBIG 1 
#    elif defined(__powerpc__) || defined(__POWERPC__)
#      undef  Z_ARCHITECTURE_PPC
#      define Z_ARCHITECTURE_PPC 1
#      undef  Z_ARCHITECTURE_ENDIANBIG
#      define Z_ARCHITECTURE_ENDIANBIG 1 
#    else
#      error Unknown architecture
#    endif 
#    undef  Z_HARDWARETYPE_DESKTOP
#    define Z_HARDWARETYPE_DESKTOP 1 
#  else
#    error Unknown Apple Platform
#  endif 


/*
** Linux
******************************************************************************/ 
#elif (defined(__linux__) || defined(__linux))
#  undef  Z_PLATFORM_LINUX
#  define Z_PLATFORM_LINUX 1 
#  undef  Z_PLATFORM_POSIX
#  define Z_PLATFORM_POSIX 1 

#  if defined(__x86_64__) || defined(__x86_64) || defined(__amd64)
#    undef  Z_ARCHITECTURE_X86_64
#    define Z_ARCHITECTURE_X86_64 1
#    undef  Z_ARCHITECTURE_ENDIANLIL
#    define Z_ARCHITECTURE_ENDIANLIL 1 
#  elif defined(__i386__) || defined(__intel__) || defined(_M_IX86)
#    undef  Z_ARCHITECTURE_X86
#    define Z_ARCHITECTURE_X86 1
#    undef  Z_ARCHITECTURE_ENDIANLIL
#    define Z_ARCHITECTURE_ENDIANLIL 1  
#  elif defined(__powerpc64__) || defined(__POWERPC64__)
#    undef  Z_ARCHITECTURE_PPC_64
#    define Z_ARCHITECTURE_PPC_64 1
#    undef  Z_ARCHITECTURE_ENDIANBIG
#    define Z_ARCHITECTURE_ENDIANBIG 1 
#  elif defined(__powerpc__) || defined(__POWERPC__)
#    undef  Z_ARCHITECTURE_PPC
#    define Z_ARCHITECTURE_PPC 1
#    undef  Z_ARCHITECTURE_ENDIANBIG
#    define Z_ARCHITECTURE_ENDIANBIG 1 
#  elif defined(__arm__)
#    undef  Z_ARCHITECTURE_ARM
#    define Z_ARCHITECTURE_ARM 1
#    if defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7S__)
#      undef  Z_ARCHITECTURE_ARM7
#      define Z_ARCHITECTURE_ARM7 1 
#      if !defined(__ARM_NEON__)
#        error Missing ARM NEON support
#      endif
#    elif defined(__ARM_ARCH_6K__) ||\
          defined(__ARM_ARCH_6J__) ||\
          defined(__ARM_ARCH_6ZK__)
#      undef  Z_ARCHITECTURE_ARM6
#      define Z_ARCHITECTURE_ARM6 1 
#    else
#      error Unrecognized ARM architecture
#    endif 
#    if defined(__ARMEB__)
#      undef  Z_ARCHITECTURE_ENDIANBIG
#      define Z_ARCHITECTURE_ENDIANBIG 1
#    else
#      undef  Z_ARCHITECTURE_ENDIANLIL
#      define Z_ARCHITECTURE_ENDIANLIL 1
#    endif 
#  elif defined(__arm64__) || defined(__aarch64__)
#    undef  Z_ARCHITECTURE_ARM
#    define Z_ARCHITECTURE_ARM 1
#    undef  Z_ARCHITECTURE_ARM_64
#    define Z_ARCHITECTURE_ARM_64 1
#    undef  Z_ARCHITECTURE_ENDIANLIL
#    define Z_ARCHITECTURE_ENDIANLIL 1
#    if defined(__ARM64_ARCH_8__)
#      undef  Z_ARCHITECTURE_ARM8_64
#      define Z_ARCHITECTURE_ARM8_64 1 
#    else
#      error Unrecognized ARM architecture
#    endif 
#    if defined(__AARCH64EB__)
#      undef  Z_ARCHITECTURE_ENDIANBIG
#      define Z_ARCHITECTURE_ENDIANBIG 1
#    else
#      undef  Z_ARCHITECTURE_ENDIANLIL
#      define Z_ARCHITECTURE_ENDIANLIL 1
#    endif 
#  else
#    error Unknown architecture
#  endif 

#  if defined(__raspberrypi__)
#    undef  Z_PLATFORM_RASPBERRYPI
#    define Z_PLATFORM_RASPBERRYPI 1
#  endif 
#  undef  Z_HARDWARETYPE_DESKTOP
#  define Z_HARDWARETYPE_DESKTOP 1 


/*
** BSD Unix family
******************************************************************************/ 
#elif (defined(__BSD__)            || defined(__FreeBSD__) || \
       defined(__NetBSD__)         || defined(__OpenBSD__) || \
       defined(__FreeBSD_kernel__) || defined(__DragonFly__))  

#  undef  Z_PLATFORM_BSD
#  define Z_PLATFORM_BSD 1 
#  undef  Z_PLATFORM_POSIX
#  define Z_PLATFORM_POSIX 1 

#  if defined(__x86_64__) || defined(__x86_64) || defined(__amd64)
#    undef  Z_ARCHITECTURE_X86_64
#    define Z_ARCHITECTURE_X86_64 1
#    undef  Z_ARCHITECTURE_ENDIANLIL
#    define Z_ARCHITECTURE_ENDIANLIL 1 
#  elif defined(__i386__) || defined(__intel__) || defined(_M_IX86)
#    undef  Z_ARCHITECTURE_X86
#    define Z_ARCHITECTURE_X86 1
#    undef  Z_ARCHITECTURE_ENDIANLIL
#    define Z_ARCHITECTURE_ENDIANLIL 1  
#  elif defined(__powerpc64__) || defined(__POWERPC64__)
#    undef  Z_ARCHITECTURE_PPC_64
#    define Z_ARCHITECTURE_PPC_64 1
#    undef  Z_ARCHITECTURE_ENDIANBIG
#    define Z_ARCHITECTURE_ENDIANBIG 1 
#  elif defined(__powerpc__) || defined(__POWERPC__)
#    undef  Z_ARCHITECTURE_PPC
#    define Z_ARCHITECTURE_PPC 1
#    undef  Z_ARCHITECTURE_ENDIANBIG
#    define Z_ARCHITECTURE_ENDIANBIG 1 
#  else
#    error Unknown architecture
#  endif 
#  undef  Z_HARDWARETYPE_DESKTOP
#  define Z_HARDWARETYPE_DESKTOP 1 
 

/* 
** Microsoft Windows
******************************************************************************/ 
#elif defined(_WIN32)     || defined(_WINDOWS)   ||                  \
      defined(_WIN64)     || defined(__WIN32__)  || defined(WIN32) ||\
      defined(__CYGWIN__) || defined(__MINGW32__) 
 
#  if defined(_MSC_VER) && defined(__has_include)
#    if __has_include(<winapifamily.h>) 
#      define __HAS_WINAPIFAMILY_H_  
#    endif 
#  elif defined(_MSC_VER) && (_MSC_VER >= 1700 && !_USING_V110_SDK71_)  
#    define __HAS_WINAPIFAMILY_H_  
#  endif 
#  if defined(__HAS_WINAPIFAMILY_H_)
#    include <winapifamily.h> 
#    if (!WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) &&\
          WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)) 
#      undef  Z_PLATFORM_WINRT
#      define Z_PLATFORM_WINRT 1 
#    endif 
#  endif  
#  undef __HAS_WINAPIFAMILY_H_

#  if defined(__x86__) || defined(_M_IX86) || defined(_X86_) 
#    undef  Z_ARCHITECTURE_X86
#    define Z_ARCHITECTURE_X86 1 
#  elif defined(__x86_64__) || defined(_M_AMD64) || defined(_AMD64_) 
#    undef  Z_ARCHITECTURE_X86_64
#    define Z_ARCHITECTURE_X86_64 1
#  elif defined(__ia64__) || defined(_M_IA64) || defined(_IA64_) 
#    undef  Z_ARCHITECTURE_IA64
#    define Z_ARCHITECTURE_IA64 1
#  else
#    error Unsupported Architecture
#  endif 
#  undef  Z_PLATFORM_WINDOWS
#  define Z_PLATFORM_WINDOWS 1 
#  undef  Z_HARDWARETYPE_DESKTOP
#  define Z_HARDWARETYPE_DESKTOP 1
#  undef  Z_ARCHITECTURE_ENDIANLIL
#  define Z_ARCHITECTURE_ENDIANLIL 1
#else
#  error Unsupported Platform
#endif  
 

/*
** Additional architecture details:
******************************************************************************/
#if defined(__SSE2__) || (Z_ARCHITECTURE_X86_64)
#  undef  Z_ARCHITECTURE_SSE2
#  define Z_ARCHITECTURE_SSE2 1
#endif 
#if defined(__SSE3__)
#  undef  Z_ARCHITECTURE_SSE3
#  define Z_ARCHITECTURE_SSE3 1
#endif 
#if defined(__SSE4_1__)
#  undef  Z_ARCHITECTURE_SSE4
#  define Z_ARCHITECTURE_SSE4 1
#endif 
#if defined(__ARM_NEON__)
#  undef  Z_ARCHITECTURE_NEON
#  define Z_ARCHITECTURE_NEON 1
#endif 
#if defined(__thumb__)
#  undef  Z_ARCHITECTURE_THUMB
#  define Z_ARCHITECTURE_THUMB 1
#endif 
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/   