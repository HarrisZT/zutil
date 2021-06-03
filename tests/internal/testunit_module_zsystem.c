/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: testunit_module_zsystem.c
* Desc: unit tests
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
#include "zutil/zsystem.h"
#include "zutil_testunits.h" 

 



Void TestUnit_Module_ZSystem(int argc, char** argv) { 
	ZSystemInfo info;
	ZSystem_GetInfo(&info);

	printf("\n ZSystem Report");
	printf("\n **************************************");
	printf("\n architecture  :   '%s'", info.archId); 
	printf("\n endianess     :   '%s'", info.endian);
	printf("\n platform      :   '%s'", info.platformId);
	printf("\n compiler      :   '%s'", info.compilerId);
	printf("\n dir seperator :   '%c'", info.dirsep);
	printf("\n base path     :   '%s'", info.basepath);
	printf("\n exe title     :   '%s'", info.exeTitle);
	printf("\n debug build   :   '%d'", info.isDebug);
	printf("\n zutil version :   '%d'", info.version);
	printf("\n **************************************\n");
} 
/*****************************************************************************/  
//EOF
/*****************************************************************************/  