/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zstring.h 
* Desc: C string utility routines
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
#ifndef __ZSTRING_H__
#define __ZSTRING_H__  

#include "zcore.h" 
#if defined(__cplusplus)
extern "C" {
#endif

	 

/*(C) 
Append a char to the end of a string.
@_str : the string to append to
@_char: the character to append onto _str*/
extern Void ZAPI
ZString_Append(_Inout_ Char* _str,
	           _In_    Char  _char); 

/*(C) 
Compare the case of the characters in the given strings.
@_str1 : a string to compare with _cstring2
@_str2 : a string to compare with _cstring1
@return: one of the following values-
	     (< 0)- _str1 less than _str2
	     (= 0)- _str1 equivalent to _str2
	     (> 0)- _str1 greater than _str2*/
extern Int32 ZAPI
ZString_CaseCmp(
	_In_ Lpcstr _str1,
	_In_ Lpcstr _str2); 

/*(C) 
Duplicate the given source string into a new string.
!Warning! - This function returns a string allocated with malloc. 
            Caller is responsible for deallocating it with free.
@_src  : the source string to duplicate
@return: an allocated string*/
extern Char* ZAPI 
ZString_Copy(
	_In_ Lpcstr _src);

/*(C) 
Returns the last index of a character if it is present
within the given string, otherwise returns -1
@_str  : the string to search
@_char : the char to search for
@return: index of char in the string*/
extern Int32 ZAPI
ZString_FindLastOf(
	_In_ Lpcstr _str,
	_In_ Char   _char); 

/*(C) 
Convert a float value to equivalent string. 
@_fValue   : the float value to convert
@_dst      : the destination string (preallocated)
@_decPlaces: number of decimal places to print*/
extern Void ZAPI
ZString_Ftoa(
	_In_    Float _fValue,
	_Inout_ Char* _dst,
	_In_    Int32 _decPlaces); 

/*(C) 
Remove all instances of a character from the given string.
@_str : the string to remove chars from
@_char: the char to remove from _str*/
extern Void ZAPI
ZString_RemoveChar(
	_Inout_ Char* _cout,
	_In_    Char  _char);

/*(C) 
Copies a string (similar to strcpy), but with buffer overflow prevention.
@_dst:    the destrination string 
@_src:    the source string
@_dstlen: length of the destination string
@return : length of the entire source string*/
extern Uint32 ZAPI
ZString_Strlcpy(
	_Inout_ Char*  _dst,
	_In_    Lpcstr _src,
	_In_    Uint32 _dstlen);

/*(C) 
Get a substring between the given start and end indexes.
!WARNING! - This function returns a string allocated with malloc. 
            Caller is responsible for deallocating it with free.
@_src  : source string
@_begin: first index
@_end  : last index
@return: the resulting string (must be freed)*/
extern Char* ZAPI
ZString_SubStr(
	_In_ Lpcstr _src,
	_In_ Uint32 _begin,
	_In_ Uint32 _end);

/*(C)
Converts the given string to a boolean value (integer).  
@_str  : the string to convert to boolean value
@return: possible return values:
         (+1)- string equates to boolean True
		 (+0)- string equates to boolean False
		 (-1)- string does not equate to a boolean value*/
extern Bool ZAPI
ZString_ToBoolean(
	_In_ Lpcstr _str);

/*(C)
Transforms a string to all lowercase.
@_str: the string to be transformed to lowercase (preallocated)*/
extern Void ZAPI
ZString_ToLower(
    _Inout_ Char* _str); 

/*(C)
Transforms a string to all uppercase.
@_str: the string to be transformed to lowercase (preallocated)*/
extern Void ZAPI
ZString_ToUpper(
    _Inout_ Char* _str); 
 
/*(C)
Compares while ignoring differences in case, 
the string pointed to by _str1 to the string pointed to by _str2.
@return: an integer greater than, equal to, or less than 0, 
         if the string pointed to by _str1 is, ignoring case, greater than, 
		 equal to, or less than the string pointed to by _str2*/
extern Int32 ZAPI
ZString_StrCaseCmp(
	_In_ Lpcstr _str1,
	_In_ Lpcstr _str2);

/*(C)
Compares two strings to determine if they are equal in case (lower/upper)
@_cStr1    : a string to compare with cStr2
@_cStr2    : a string to compare with cStr1
@_iEndPoint: check str1 up until this point
@return    : true if strings are equal, else false*/
extern Bool ZAPI
ZString_StrCaseCmpN(
	_In_ Lpcstr _cStr1, 
	_In_ Lpcstr _cStr2, 
	_In_ Int32  _iEndPoint); 



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  