/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: testunit_module_zlog.c
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
#include "zutil/zlog.h"
#include "zutil/zstring.h"
#include "zutil_testunits.h"



Void TestUnit_Module_ZLog(int _argc, char** _argv) {
	ZRESULT zResult;  
	Uint32  it;  
	 
	zResult = ZLog_InitDefault();
	if (Z_FAILURE(zResult)) {
		printf("ZLog failed to initialize");
		goto EXIT;
	}  	 
	for (it = 0; it < ZLOGLEVEL_UNDEFINED; ++it) {
		Char str[20];
		Char strIt[20];
        itoa(it, strIt, 10); 
		strcpy(str, "Log Test: ");
		strcat(str, strIt);

		ZLog_Output(
			(ZLOGLEVEL)it, 
			__FILE__, 
			__LINE__, 
			str);
	} 
EXIT:
	ZLog_Release(); 
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/  