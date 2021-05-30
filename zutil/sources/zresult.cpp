/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zresult.cpp 
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
#include <streambuf> 
#include <sstream>
#include "zutil/zcore.h"
#include "zutil/zresult.h"





/*
** Section I: 
** C++ helper stream
******************************************************************************/
static std::string lasterr;
  
class ErrStreamOut : public std::streambuf {
public:
	ErrStreamOut() {
		Char* buffer = new Char[0x40];
		setp(buffer, buffer + 0x40);
	}
	~ErrStreamOut() {
		sync();
		delete[] pbase();
	}
private:
	virtual Int32 overflow(_In_ Int32 _iChar) {
		Int32 iResult;
		if ((_iChar != EOF) && (pptr() != epptr()))
			iResult = sputc((Char)(_iChar));
		else if (_iChar != EOF) {
			sync();
			iResult = overflow(_iChar);
		}
		else iResult = sync();
		return iResult;
	}

	virtual Int32 sync() {
		if (pbase() != pptr()) {
			SizeT size = (Int32)(pptr() - pbase());
			fwrite(pbase(), 1, size, stderr);
			setp(pbase(), epptr());
		}
		return 0;
	}
};





/*
** Section II: 
** Interface functions
******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif  


	
void 
ZResult_PostError(
	_In_ const char* _cErr) {

	static ErrStreamOut buffer;
	static std::ostream stream(&buffer);
	lasterr = std::string(_cErr);
	stream << lasterr;
}
 

const char*
ZResult_GetLastError(void) {
	return lasterr.c_str();
}


ZRESULT 
ZResult_FromErrno(
	_In_ int _iErrnoCode) {

	switch (_iErrnoCode) {
	case 0           : return Z_OK;
	case EACCES      : return Z_EACCESS;
	case EAGAIN      : return Z_ETIMEDOUT;
	case EDEADLK     : return Z_EDEADLOCK;
	case EEXIST      : return Z_EOBJEXISTS;
	case EFAULT      : return Z_EFAULT;
	case EINTR       : return Z_EINTERRUPT;
	case EINVAL      : return Z_EINVALIDARG;
	case ENAMETOOLONG: return Z_ENAMETOOLONG;
	case ENOENT      : return Z_EHANDLE;
	case ENOMEM      : return Z_EOUTOFMEMORY;
	case ENOSPC      : return Z_EUNSUPPORTED; 
	case EPERM       : return Z_EPERMISSION;  
	case ETIMEDOUT   : return Z_ETIMEDOUT;
	}
	return Z_EFAIL;
}


ZRESULT 
ZResult_SetErrno(
	_In_ int _iResult) {

	ZRESULT hResult;
	if (_iResult != Z_OK)
		errno = _iResult;
	hResult = ZResult_FromErrno(_iResult);
	return hResult;
}



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
//EOF
/*****************************************************************************/