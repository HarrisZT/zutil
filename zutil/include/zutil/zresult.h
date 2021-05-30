/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zresult.h 
* Desc: data type representing the result of an operation
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
#ifndef __ZRESULT_H__
#define __ZRESULT_H__  

#include <errno.h> 
#include "zconfig.h"
#if defined(__cplusplus)
extern "C" {
#endif



/*
data type representing the result of an operation.
Its value is Z_OK if successful, else an error code
*******************************************************************************/ 
typedef long ZRESULT;
enum {
    Z_OK           = +0x00, //operation successful 
    Z_EABORT       = -0x01, //operation aborted 
    Z_EACCESS      = -0x02, //general access denied error
    Z_EBUSY        = -0x03, //mutex or resource busy, lock failure
    Z_EDEADLOCK    = -0x04, //A deadlock condition was detected.
    Z_EFAIL        = -0x05, //unspecified operation failure	
	Z_EFAULT       = -0x06, //pointer to outside of accessible address space 
    Z_EFILEMISSING = -0x07, //specified file was not found
    Z_EFILEREAD    = -0x08, //a file failed to read properly
    Z_EHANDLE      = -0x09, //invalid data handle
    Z_EICONVERROR  = -0x0A, //iconv error: unspecified
    Z_EICONVTOOBIG = -0x0B, //iconv error: 
    Z_EICONVILSEQ  = -0x0C, //iconv error: 
    Z_EICONVINVAL  = -0x0D, //iconv error: 
    Z_EINITSYSTEM  = -0x0E, //subsystem has not been initialized
    Z_EINTERRUPT   = -0x0F, //a signal interrupted this function
    Z_EINTR	       = -0x10, //interrupted system call
    Z_EINVALIDARG  = -0x11, //one or more arguments are invalid 
	Z_ENAMETOOLONG = -0x12, //length of string name arg exceeds F2D_MAXPATH
    Z_ENOTDIR      = -0x13, //not a directory
    Z_ELASTERROR   = -0x14, //posting of last error message
    Z_ELOADASSET   = -0x15, //subsystem failed to load asset 
    Z_EOBJEXISTS   = -0x16, //cannot create object that already exists
    Z_EOUTOFMEMORY = -0x17, //failed to allocate necessary memory
    Z_EPERMISSION  = -0x18, //process lacks appropriate privileges 
    Z_EPOINTER     = -0x19, //encountered invalid pointer 
    Z_EREGISTERED  = -0x1A, //specified file is not registered
    Z_ETIMEDOUT    = -0x1B, //mutex or connection timed out
    Z_EUNSUPPORTED = -0x1C, //unsupported operation or feature 	 
};
#define Z_FAILURE(r) (((ZRESULT)(r)) < 0)  /**< Did operation fail?*/
#define Z_SUCCESS(r) (((ZRESULT)(r)) >= 0) /**< Did operation succeed?*/ 




/*
Convert a given standard errno code into ZRESULT code.
@_iErrnoCode: the errno code to convert
@return     : the resulting ZRESULT code*/
extern ZRESULT ZAPI
ZResult_FromErrno(
	_In_ int _iErrnoCode);

/*
Wrapper function for setting an error code with errno.
@_iResult: the errno code to set
@return  : the resulting ZRESULT code*/
extern ZRESULT ZAPI
ZResult_SetErrno(
	_In_ int _iResult); 

/*
Set an error description
@_cErr: a string describing the current error*/
extern void ZAPI 
ZResult_PostError(
	_In_ const char* _cErr);

/*
Get an error description
@return: a string describing the most recently posted error*/
extern const char* ZAPI
ZResult_GetLastError(void);



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/