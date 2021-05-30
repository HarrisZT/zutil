/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zbase64.c
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
#include "zutil/zbase64.h"



static Int32 b64_decoder[] = {
	62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
	59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
	6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
	43, 44, 45, 46, 47, 48, 49, 50, 51 
};
static const Char b64_encoder[] = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";





SizeT
ZBase64_DecodedSize(
	_In_ Lpcstr _str) {

	SizeT len, i, decodedsiz;

	if (!_str)
		return 0;

	len = strlen(_str);
	decodedsiz = len / 4 * 3;

	for (i = len; i-- > 0; ) {
		if (_str[i] == '=')
			decodedsiz--;
		else break;
	}
	return decodedsiz;
}


SizeT
ZBase64_EncodedSize(
	_In_ SizeT _iLen) {

	SizeT encodedsiz;
	encodedsiz = _iLen;
	if (_iLen % 3 != 0)
		encodedsiz += 3 - (_iLen % 3);
	encodedsiz /= 3;
	encodedsiz *= 4;
	return encodedsiz;
}


Bool
ZBase64_IsValidChar(
	_In_ Char _char) {

	if (_char >= '0' && _char <= '9')
		return Z_TRUE;
	if (_char >= 'A' && _char <= 'Z')
		return Z_TRUE;
	if (_char >= 'a' && _char <= 'z')
		return Z_TRUE;
	if (_char == '+' || _char == '/' || _char == '=')
		return Z_TRUE;
	return Z_FALSE;
}


Bool
ZBase64_Decode(
	_In_    Lpcstr _str,
	_Inout_ Byte*  _dataOut,
	_In_    SizeT  _iLen) {

	Int32 v;
	SizeT len, i, j;

	if (_str == NULL || _dataOut == NULL)
		return Z_FALSE;

	len = strlen(_str);
	if (_iLen < ZBase64_DecodedSize(_str) || len % 4 != 0)
		return Z_FALSE;

	for (i = 0; i < len; i++) {
		if (!ZBase64_IsValidChar(_str[i]))
			return Z_FALSE;
	}
	for (i = 0, j = 0; i < len; i += 4, j += 3) {
		v = b64_decoder[_str[i] - 0x2B];
		v = (v << 6) | b64_decoder[_str[i + 1] - 0x2B];

		v = _str[i + 2] == '=' ? v << 0x06 :
			(v << 0x06) | b64_decoder[_str[i + 2] - 0x2B];

		v = _str[i + 3] == '=' ? v << 0x06 :
			(v << 0x06) | b64_decoder[_str[i + 3] - 0x2B];

		_dataOut[j] = (v >> 0x10) & 0xFF;
		if (_str[i + 2] != '=')
			_dataOut[j + 1] = (v >> 0x08) & 0xFF;
		if (_str[i + 3] != '=')
			_dataOut[j + 2] = v & 0xFF;
	}
	return Z_TRUE;
}


Char*
ZBase64_Encode(
	_In_ const Byte* _lpData,
	_In_ SizeT       _iLen) {

	Char* dst;
	SizeT eLen;
	SizeT i, j, v;

	if (!_lpData || !_iLen)
		return NULL;

	eLen = ZBase64_EncodedSize(_iLen);
	dst = (Char*)malloc(eLen + 1);
	if (!dst)
		return NULL;
	dst[eLen] = '\0';

	for (i = 0, j = 0; i < _iLen; i += 3, j += 4) {
		v = _lpData[i];
		v = i + 1 < _iLen ? v << 0x08 | _lpData[i + 1] : v << 0x08;
		v = i + 2 < _iLen ? v << 0x08 | _lpData[i + 2] : v << 0x08;

		dst[j] = b64_encoder[(v >> 0x12) & 0x3F];
		dst[j + 1] = b64_encoder[(v >> 0x0C) & 0x3F];

		if (i + 1 < _iLen)
			dst[j + 2] = b64_encoder[(v >> 0x06) & 0x3F];
		else dst[j + 2] = '=';

		if (i + 2 < _iLen)
			dst[j + 3] = b64_encoder[v & 0x3F];
		else dst[j + 3] = '=';
	}
	return dst;
}
/****************************************************************************** 
******************************************************************************/  





/*
example function demonstrating use of zbase64 routines:
******************************************************************************/
///static void ExampleUsageFunction() {
///
///    size_t      iLen;
///    char*       encoding;
///    char*       cResult; 
///    const char* b64data;
///
///     b64data = 
///    	    "ABC123 \"The quick brown fox jumped over the lazy dog\""; 
///    
///    printf("data:    '%s'\n", b64data);
///    
///    encoding = ZBase64_Encode((const Byte*)b64data, strlen(b64data));
///    printf("encoded: '%s'\n", encoding); 
///    printf(
///    	"decoded %s data size\n", 
///    	ZBase64_DecodedSize(encoding) == strlen(b64data) ? "==" : "!=");
///    
///    //(add +1 for NULL terminated str) 
///    iLen    = ZBase64_DecodedSize(encoding) + 1;
///    cResult = (Char*)malloc(iLen);
///    
///    if (!ZBase64_Decode(encoding, (Byte*)cResult, iLen)) {
///    	printf("Decoding Failed\n");
///    	return 1;
///    }
///    cResult[iLen] = '\0';
///    
///    printf("dec:     '%s'\n", cResult);
///    printf("data %s dec\n", strcmp(b64data, cResult) == 0 ? "==" : "!=");
///    free(cResult); 
///} 
/*****************************************************************************/  
//EOF
/*****************************************************************************/