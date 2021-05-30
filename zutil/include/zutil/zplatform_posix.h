/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zplatform_posix.h 
* Desc: POSIX includes and functions
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
#ifndef __ZPLATFORM_POSIX_H__
#define __ZPLATFORM_POSIX_H__

#include "zconfig.h" 

#if (Z_PLATFORM_POSIX) 
#  if (Z_COMPILER_GCC)
#    pragma GCC diagnostic push
#    if (Z_GCC_VERSION > 40700)
#      pragma GCC diagnostic ignored "-Wpedantic"
#    endif
#  elif (Z_COMPILER_CLANG)
#    pragma clang diagnostic push
#    if __has_warning( "-Wreserved-id-macro" )
#      pragma clang diagnostic ignored "-Wreserved-id-macro"
#    endif
#    if __has_warning( "-Wpedantic" )
#      pragma clang diagnostic ignored "-Wpedantic"
#    endif
#    if __has_warning( "-Wundef" )
#      pragma clang diagnostic ignored "-Wundef"
#    endif
#    if __has_warning( "-Wpacked" )
#      pragma clang diagnostic ignored "-Wpacked"
#    endif
#  endif 

#  define radixsort __stdlib_radixsort
#  if !defined(__error_t_defined)
#    define __error_t_defined 1
#  endif 

#  if (Z_PLATFORM_APPLE)
#    define _UUID_T
#    define _UUID_UUID_H
#  endif 
#  if !defined(_GNU_SOURCE)
#    define _GNU_SOURCE 1
#  endif

#  include <unistd.h> 
#  include <sched.h>
#  include <pwd.h> 
#  include <pthread.h>
#  include <fcntl.h>
#  include <signal.h>
#  include <setjmp.h>

#  include <sys/ioctl.h>
#  include <sys/types.h>
#  include <sys/wait.h>
#  include <sys/errno.h>
#  include <sys/time.h>

#  if (Z_PLATFORM_LINUX || Z_PLATFORM_ANDROID)
#    include <sys/eventfd.h>
#    include <sys/epoll.h>
#  endif

#  undef radixsort

#  if (Z_PLATFORM_APPLE)
#    undef _UUID_T
#    undef _UUID_UUID_H
#  endif

#  if (Z_COMPILER_GCC)
#    pragma GCC diagnostic pop
#  elif (Z_COMPILER_CLANG)
#    pragma clang diagnostic pop
#  endif
   
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/   