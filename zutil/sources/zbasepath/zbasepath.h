/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zbasepath.h 
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
#ifndef __ZBASEPATH_H__
#define __ZBASEPATH_H__

#include "zutil/zcore.h"
#if defined(__cplusplus)
extern "C" {
#endif 


 
/* 
Returns the full path of the program executable.
Note that the string will also contain the executable name.
@return: an allocated, remember to free it when finished!
         result may be NULL on function failure*/
extern Char* 
ZGetBasePath(Void); 
 


#if defined(__cplusplus)
}
#endif 
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/
