/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zbase64.h 
* Desc: base64 encoded/decode routines 
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
#ifndef __ZBASE64_H__
#define __ZBASE64_H__ 

#include "zcore.h"  
#if defined(__cplusplus)
extern "C" {
#endif



/*
Returns the required size for allocating a string to be encoded.
(The result accounts for null termination)
@_string: an input string to be encoded
@return : allocation size*/
extern Uint32 ZAPI
ZBase64_GetEncodingSize(
	_In_ Lpcstr _string);

/*
Returns the required size for allocating a string to be decoded.
(The result accounts for null termination)
@_string: an encoded string to be encoded
@return : allocation size*/
extern Uint32 ZAPI
ZBase64_GetDecodingSize(
	_In_ Lpcstr _string);

/*
Encodes a given string with base64. 
@_inputstring: an input string to be encoded
@_inputlength: length size of the input string
@_output     : an allocated string to contain the result*/
extern Void ZAPI
ZBase64_Encode(
	_In_    Byte*  _inputstring, 
	_In_    Uint32 _inputlength, 
	_Inout_ Byte*  _output);

/*
Decodes a given base64 encoded string.
@_encodedstring: a base64 encoded string to be decoded
@_inLength     : length size of the encoded string
@_output       : an allocated string to contain the result*/
extern Void ZAPI
ZBase64_Decode(
	_In_    Byte*  _encodedstring, 
	_In_    Uint32 _inLength, 
	_Inout_ Byte*  _output);



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  
