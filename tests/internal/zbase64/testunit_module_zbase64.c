/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: testunit_module_zbase64.c
* Desc: unit test for ZLogger
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
#include "zutil_testunits.h"
#include <stdio.h>
#include <stdbool.h>

 

Void TestUnit_Module_ZBase64(int argc, char** argv) { 
	Lpcstr teststring = 
		"ABC123Test Testing this' data \"input\"";

	Char*  encodedstring; 
	Char*  decodedstring; 
	Uint32 encodedlength;
	Uint32 decodedlength;

	printf("intput:    '%s'\n", teststring);	 
	/*
	Get the required allocation size for a base64 encoded string, 
	then allocate the encoding string with the length:*/
	encodedlength = ZBase64_GetEncodingSize(teststring); 
	encodedstring = (Char*)malloc(encodedlength);
	/*
	The teststring is the string to be encoded, so that is the input variable.
	The encodedstring will contain the encoded version of teststring,
	so that is the output variable:*/ 
	ZBase64_Encode(teststring, strlen(teststring), encodedstring);
	printf("encoded:   '%s'\n", encodedstring); 
	/*
	Get the required allocation size for the decoded string, 
	then allocate the decoding string with the length:*/
	decodedlength = ZBase64_GetDecodingSize(encodedstring); 
	decodedstring = (Char*)malloc(decodedlength);
	/*
	The encodedstring is the string to be decoded, so that is the input var.
	The decodedstring will contain the decoded version of encodedstring,
	so that is the output variable:*/ 
	ZBase64_Decode(encodedstring, strlen(encodedstring), decodedstring);
	printf("encoded:   '%s'\n", decodedstring); 
	/*
	cleanup:*/
	free(encodedstring); 
	encodedstring = NULL;
	free(decodedstring); 
	decodedstring = NULL;
} 
/*****************************************************************************/  
//EOF
/*****************************************************************************/  