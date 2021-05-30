/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: Ziconv.h 
* Desc: unicode string conversions
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
#ifndef __ZICONV_H__
#define __ZICONV_H__

#include "zcore.h"
#if defined(__cplusplus)
extern "C" {
#endif
 


 
typedef struct _ZIconV *ZIconV;


extern ZIconV ZAPI 
ZIconV_Open(
	_In_ Lpcstr _toCode,
    _In_ Lpcstr _fromCode);

extern Int32 ZAPI
ZIconV_Close(
	_Inout_ ZIconV _lpIconV);

extern SizeT ZAPI
ZIconV_Init(
	_Inout_ ZIconV  _lpIconV, 
	_In_    Lpcstr* _inbuffer,
    _In_    SizeT*  _inbytesleft, 
	_Inout_ Char**  _outbuffer,
    _In_    SizeT*  _outbytesleft);


/* 
Converts a string between encodings, 
!WARNING! - This function returns a string allocated with malloc. 
            Caller is responsible for deallocating it with free.*/
extern Char* ZAPI
ZIconV_ConvertString(
	_In_ Lpcstr _tocode,
    _In_ Lpcstr _fromcode,
    _In_ Lpcstr _inbuffer,
    _In_ SizeT  _inbytesleft);


#define ZIconV_UTF8_Locale(x)\
  ZIconV_ConvertString(\
  "", "UTF-8", x, strlen(x) + 1)

#define ZIconV_UTF8_UCS2(x)\
  (Uint16*)ZIconV_ConvertString(\
  "UCS-2-INTERNAL", "UTF-8", x, strlen(x) + 1)

#define ZIconV_UTF8_UCS4(x) \
  (Uint32*)ZIconV_ConvertString(\
  "UCS-4-INTERNAL", "UTF-8", x, strlen(x) + 1)
 


#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  