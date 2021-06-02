/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zbase64.c 
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
#include "zutil/zbase64.h"



static const Char ENCODINGTABLE[65] = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const Char DECODINGTABLE[256] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
	,-1,62,-1,-1,-1,63,52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,
	-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,
	23,24,25,-1,-1,-1,-1,-1,-1,26,27,28,29,30,31,32,33,34,35,36,37,
	38,39,40,41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};



Uint32
ZBase64_GetEncodingSize(
	_In_ Lpcstr _string) {

	Uint32 inlen;
	size_t ret;
	inlen = strlen(_string);
	ret = inlen;
	if (inlen % 3 != 0)
		ret += 3 - (inlen % 3);
	ret /= 3;
	ret *= 4;
	ret++; //< +1 for null termination
	return ret;
}


Uint32
ZBase64_GetDecodingSize(
	_In_ Lpcstr _string) {
	//+1 for null termination
	return strlen(_string) + 1;
} 


Void
ZBase64_Encode(
	_In_    Byte*  _input,
	_In_    Uint32 _inputlength,
	_Inout_ Byte*  _output) {

	Char   buffer1[3];
	Char   buffer2[4];
	Byte   it0, it1;
	Uint32 inputindex;
	Uint32 outputindex;
	Uint32 tableindex;

	it0         = 0;
	it1         = 0;
	inputindex  = 0;
	outputindex = 0;

	while (inputindex < _inputlength) {
		buffer1[it0++] = _input[inputindex++];
		if (it0 == 3) {
			tableindex = 
				(buffer1[0] & 0xFC) >> 0x02;

			_output[outputindex++] = ENCODINGTABLE[tableindex];
			
			tableindex = 
				((buffer1[0] & 0x03) << 0x04) + 
				((buffer1[1] & 0xF0) >> 0x04);

			_output[outputindex++] = ENCODINGTABLE[tableindex];

			tableindex = 
				((buffer1[1] & 0x0F) << 0x02) +
				((buffer1[2] & 0xC0) >> 0x06);

			_output[outputindex++] = ENCODINGTABLE[tableindex];

			tableindex = buffer1[2] & 0x3F;

			_output[outputindex++] = ENCODINGTABLE[tableindex];

			it0 = 0;
		}
	}
	if (it0) {
		for (it1 = it0; it1 < 3; it1++) {
			buffer1[it1] = '\0';
		}
		buffer2[0] = 
			(buffer1[0] & 0xFC) >> 0x02;

		buffer2[1] = 
			((buffer1[0] & 0x03) << 0x04) +
			((buffer1[1] & 0xF0) >> 0x04);

		buffer2[2] = 
			((buffer1[1] & 0x0F) << 0x02) + 
			((buffer1[2] & 0xC0) >> 0x06);

		buffer2[3] = 
			buffer1[2] & 0x3F;

		for (it1 = 0; it1 < (it0 + 1); it1++) {
			_output[outputindex++] = 
				ENCODINGTABLE[buffer2[it1]];
		}
		while (it0++ < 3) {
			_output[outputindex++] = '=';
		}
	}
	_output[outputindex] = '\0';
}


Void
ZBase64_Decode(
	_In_    Byte*  _encodedstring,
	_In_    Uint32 _inputlength,
	_Inout_ Byte*  _output) {

	Char   buffer1[4];
	Char   buffer2[4];
	Byte   it0, it1;
	Uint32 inputindex;
	Uint32 outputindex;

	it0         = 0;
	it1         = 0;
	inputindex  = 0;
	outputindex = 0;

	while (inputindex < _inputlength) {
		buffer2[it0] = _encodedstring[inputindex++];
		if (buffer2[it0] == '=') {
			break;
		}
		if (++it0 == 4) {
			for (it0 = 0; it0 != 4; it0++) {
				buffer2[it0] = DECODINGTABLE[buffer2[it0]];
			}
			_output[outputindex++] =
				(Char)((buffer2[0] << 0x02) +
				((buffer2[1] & 0x30) >> 0x04));

			_output[outputindex++] =
				(Char)(((buffer2[1] & 0x0F) << 0x04) +
				((buffer2[2] & 0x3C) >> 0x02));

			_output[outputindex++] =
				(Char)(((buffer2[2] & 0x03) << 0x06) +
					buffer2[3]);

			it0 = 0;
		}
	}
	if (it0) {
		for (it1 = it0; it1 < 4; it1++) {
			buffer2[it1] = '\0';
		}
		for (it1 = 0; it1 < 4; it1++) {
			buffer2[it1] = DECODINGTABLE[buffer2[it1]];
		}
		buffer1[0] =
			(buffer2[0] << 0x02) +
			((buffer2[1] & 0x30) >> 0x04);

		buffer1[1] =
			((buffer2[1] & 0x0F) << 0x04) +
			((buffer2[2] & 0x3c) >> 0x02);

		buffer1[2] =
			((buffer2[2] & 0x03) << 0x06) +
			buffer2[3];

		for (it1 = 0; it1 < (it0 - 1); it1++) {
			_output[outputindex++] = (Char)buffer1[it1];
		}
	}
	_output[outputindex] = '\0';
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/  
