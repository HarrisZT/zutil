/****************************************************************************** 
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zbase64.h 
* Desc: base64 encode/decode routines 
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
Determines binary data size of a given base64-encoded data string.
@_str  : a string containing base64-encoded data
@return: the binary size of the string*/
extern SizeT ZAPI
ZBase64_DecodedSize(
	_In_ Lpcstr _str);

/*
Determines the number of base64 characters needed
for creating a base64-encoded string.
@_iLen : the length of a string that will be encoded
@return: the required length for a base64 data array*/
extern SizeT ZAPI
ZBase64_EncodedSize(
	_In_ SizeT _iLen);

/*
Reports whether the given char is valid for base64 binary.
@_char : the char to validate
@return: 1 (true) if the char is base64 compatible, else (0) false*/
extern Bool ZAPI
ZBase64_IsValidChar( 
	_In_ Char _char);

/*
Decodes a string and converts the result to base64 binary data. 
(Performs the opposite operation of f2dbase64_encode). 
@_str    : the string to be decoded into base64 binary
@_dataOut: an array to contain the decoded results (preallocated)
@_iLen   : length of the string (_str)
@return  : true if operation succeeded, else false*/
extern Bool ZAPI
ZBase64_Decode(
	_In_    Lpcstr _str, 
	_Inout_ Byte*  _dataOut, 
	_In_    SizeT  _iLen);

/*
Encodes binary data and returns the result as a string 
(Performs the opposite operation of f2dbase64_decode).
!Warning! - This function returns a string allocated with malloc. 
            Caller is responsible for deallocating it with free.
@_lpData: binary data to be encoded
@_iLen  : length of the _lpData array
@return : an allocated string*/
extern Char* ZAPI
ZBase64_Encode(
	_In_ const Byte* _lpData, 
	_In_ SizeT       _iLen); 
 


#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  