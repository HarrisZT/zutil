/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zbasepath_common.h 
* Desc: common utility used thoughout the zbasepath module
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
#ifndef __ZBASEPATH_COMMON_H__
#define __ZBASEPATH_COMMON_H__

#include <limits.h>  
#include "zutil/zcore.h" 

#if defined(_MSC_VER)
#  define Z_ReturnAddress() _ReturnAddress()
#elif defined(__GNUC__)
#  define Z_ReturnAddress() \
          __builtin_extract_return_addr(__builtin_return_address(0))
#else
#  error unsupported compiler
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/
