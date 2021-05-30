/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: main.c
* Desc: runs unit tests for Zutil
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
#include <vld.h>
#include "zutil_testunits.h"
  
 

const TESTUNIT s_module[] = {   
	{ "TestUnit Module: ZLog",     TestUnit_Module_ZLog     }, 
	{ "TestUnit Module: ZThreads", TestUnit_Module_ZThreads }, 
    { NULL,                                                 }
}; 



int main(int argc, char** argv) {
	Int32           it;
	const TESTUNIT* test;

	for (it = 0; s_module[it].name != NULL; it++) {
		/*
		print header: */
		fputs("\n\n\n", stdout);
	    fputs(s_module[it].name, stdout);
	    fputs("\n***************************************\n\n", stdout); 
		/*
		run callback to test module:*/
		test = &(s_module[it]);
		test->callback();
		/*
		conclusion: */
		printf("\n  Press 'ENTER' Key To Continue");
	    getchar(); 
	} 
	return Z_OK;
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/  