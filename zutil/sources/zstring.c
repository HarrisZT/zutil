/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zstring.c
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
#include <math.h>
#include "zutil/zstring.h" 




Void
ZString_Append(
	_Inout_ Char* _str,
	_In_    Char  _char) {

	SizeT len;
	len = strlen(_str);
	_str[len] = _char;
	_str[len + 1] = '\0';
}


Int32 
ZString_CaseCmp(
	_In_ Lpcstr _str1,
	_In_ Lpcstr _str2) {

	Char a, b;
	a = 0;
	b = 0;

	while (*_str1 && *_str2) {
		a = toupper((Byte)*_str1);
		b = toupper((Byte)*_str2);
		if (a != b)
			break;
		++_str1;
		++_str2;
	}
	a = toupper(*_str1);
	b = toupper(*_str2);
	return (Int32)((Byte)a - (Byte)b);
}


Char*
ZString_Copy(
	_In_ Lpcstr _src) {

	SizeT len;
	Char* out = NULL; 
	if (_src != NULL) {
		len = strlen(_src);
		out = (Char*)malloc((len + 1) * sizeof(Char));
		if (out != NULL) {
			strcpy(out, _src);
			out[len] = '\0';
		}
	}
	return out;
}


Int32 
ZString_FindLastOf(
	_In_ Lpcstr _str,
	_In_ Char   _char) {

	Int32 index;
	index = strlen(_str) - 1;
	for (; index >= 0; index--) {
		if (_str[index] == _char)
			return index;
	}
	return Z_UNDEFINED;
}


Void
ZString_Ftoa(
	_In_    Float _fValue,
	_Inout_ Char* _dststr,
	_In_    Int32 _decPlaces) {

	Char* tmpstr1;
	Int32 iPart, index;
	Int32 a, b, c, d, x, temp;
	Float fPart;

	iPart = (Int32)_fValue;
	fPart = _fValue - (Float)iPart;
	{
		tmpstr1 = _dststr;
		x = iPart;
		d = 0;
		a = 0;
		b = 0;
		while (x) {
			tmpstr1[a++] = (x % 0xA) + '0';
			x = x / 0xA;
		}
		while (a < d) { tmpstr1[a++] = '0'; }

		c = a - 1;
		while (b < c) {
			temp = tmpstr1[b];
			tmpstr1[b] = tmpstr1[c];
			tmpstr1[c] = temp;
			b++; 
			c--;
		}
		tmpstr1[a] = '\0';
		index = a;
	}
	if (_decPlaces != 0) {
		_dststr[index] = '.';
		fPart = (Float)(fPart * pow(10.f, _decPlaces));
		{
			tmpstr1 = _dststr + index + 1;
			x = (Int32)fPart;
			d = _decPlaces;
			a = 0;
			b = 0;
			while (x) {
				tmpstr1[a++] = (x % 0xA) + '0';
				x = x / 0xA;
			}
			while (a < d) { tmpstr1[a++] = '0'; }

			c = a - 1;
			while (b < c) {
				temp = tmpstr1[b];
				tmpstr1[b] = tmpstr1[c];
				tmpstr1[c] = temp;
				b++; c--;
			}
			tmpstr1[a] = '\0';
			index = a;
		}
	}
} 


Void
ZString_RemoveChar(
	_Inout_ Char* _str,
	_In_    Char  _char) {

	Int32 size, a, b = 0;
	Char  ch1, str1[10];

	size = strlen(_str);
	for (a = 0; a < size; a++) {
		if (_str[a] != _char) {
			ch1 = _str[a];
			str1[b] = ch1;
			b++;
		}
	}
	str1[b] = '\0';
}


Uint32 
ZString_Strlcpy(
	_Inout_ Char*  _dst,
	_In_    Lpcstr _src,
	_In_    Uint32 _dstlen) {

	Uint32 len, srclen;
	srclen = strlen(_src);

	if (_dstlen > 0) {
		len = Z_Min(srclen, _dstlen - 1);
		memcpy(_dst, _src, len);
		_dst[len] = '\0';
	}  
	return srclen;
}


Char* 
ZString_SubStr(
	_In_ Lpcstr _src,
	_In_ Uint32 _begin,
	_In_ Uint32 _end) {

	Char*  dst;
	Uint32 len;

	len = 0;
	dst = NULL;

	if (_end >= _begin) {
		len = _end - _begin;
		dst = (Char*)malloc(sizeof(Char)* (len + 1));
		if (dst != NULL)
			strncpy(dst, (_src + _begin), len);
	}
	return dst;
}


Bool 
ZString_ToBoolean(
	_In_ Lpcstr _str) {

	SizeT len;
	Char* dst;
	Bool  bResult;

	bResult = Z_UNDEFINED;

	len = strlen(_str);
	if (!len)
		return bResult;

	dst = (Char*)malloc(sizeof(Char)* (strlen(_str) + 1));
	if (!dst)
		return bResult;

	strcpy(dst, _str);
	ZString_ToLower(dst);

	if (dst == "1"      ||
		dst == "true"   ||
		dst == "enable" ||
		dst == "on") {
		bResult = Z_TRUE;
	}
	else if (
		dst == "0"       ||
		dst == "false"   ||
		dst == "disable" ||
		dst == "off") {
		bResult = Z_FALSE;
	}
	free(dst);
	dst = NULL;
	return bResult;
}


Void
ZString_ToLower(
	_Inout_ Char* _str) {

	Uint32 index, size;

	if (_str != NULL) {
		size = strlen(_str);
		for (index = 0; index < size; ++index)
			_str[index] = tolower(_str[index]);
	}
}


Void
ZString_ToUpper(
	_Inout_ Char* _str) {

	Uint32 index, size;

	if (_str != NULL) {
		size = strlen(_str);
		for (index = 0; index < size; ++index)
			_str[index] = toupper(_str[index]);
	}
}


Void
ZString_Sprintf(
	_Inout_ Char* _dst,
	_In_    Int32 _iSize,
	_In_    Char* _fmt, ...) {

	Int32   len;
	va_list argptr;
	Char	bigbuffer[0x10000];

	va_start(argptr, _fmt);
	len = vsprintf(bigbuffer, _fmt, argptr);
	va_end(argptr);
	if (len >= _iSize)
		printf("overflow of %i in %i\n", len, _iSize);
	strncpy(_dst, bigbuffer, _iSize - 1);
}


Int32 
ZString_StrCaseCmp(
	_In_ Lpcstr _str1,
	_In_ Lpcstr _str2) {

	Char a, b;
	while (*_str1 && *_str2) {
		a = toupper((Byte)*_str1);
		b = toupper((Byte)*_str2);
		if (a != b)
			break;
		++_str1;
		++_str2;
	}
	a = toupper(*_str1);
	b = toupper(*_str2);
	return (Int32)((Byte)a - (Byte)b);
}


Bool
ZString_StrCaseCmpN(
	_In_ Lpcstr _str1,
	_In_ Lpcstr _str2,
	_In_ Int32  _iEndPoint) {

	Int32 c1, c2;
	do {
		c1 = *_str1++;
		c2 = *_str2++;
		if (!_iEndPoint--)
			return Z_TRUE; //strings are equal until end point

		if (c1 != c2) {
			if (c1 >= 'a' && c1 <= 'z')
				c1 -= ('a' - 'A');
			if (c2 >= 'a' && c2 <= 'z')
				c2 -= ('a' - 'A');
			if (c1 != c2)
				return Z_FALSE; //strings not equal
		}
	} while (c1);
	return Z_TRUE; //strings are equal
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/ 