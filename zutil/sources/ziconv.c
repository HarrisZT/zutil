/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: ziconv.c
* Desc: unicode string conversions
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
#include "zutil/zstring.h"
#include "zutil/zendian.h"
#include "zutil/ziconv.h" 

#if (Z_PLATFORM_WINDOWS)
#  include "zutil/zplatform_win32.h"
#else
#  include "zutil/zplatform_posix.h"
#endif  





#define UNICODE_BOM     0xFEFF 
#define UNKNOWN_ASCII   '?'
#define UNKNOWN_UNICODE 0xFFFD
enum {
	ENCODING_UNKNOWN,
	ENCODING_ASCII,
	ENCODING_LATIN1,
	ENCODING_UTF8,
	ENCODING_UTF16,             /* Needs byte order marker */
	ENCODING_UTF16BE,
	ENCODING_UTF16LE,
	ENCODING_UTF32,             /* Needs byte order marker */
	ENCODING_UTF32BE,
	ENCODING_UTF32LE,
	ENCODING_UCS2BE,
	ENCODING_UCS2LE,
	ENCODING_UCS4BE,
	ENCODING_UCS4LE,
};
#if (Z_ENDIAN_LIL)
#  define ENCODING_UTF16NATIVE ENCODING_UTF16LE
#  define ENCODING_UTF32NATIVE ENCODING_UTF32LE
#  define ENCODING_UCS2NATIVE  ENCODING_UCS2LE
#  define ENCODING_UCS4NATIVE  ENCODING_UCS4LE
#else
#  define ENCODING_UTF16NATIVE ENCODING_UTF16BE
#  define ENCODING_UTF32NATIVE ENCODING_UTF32BE
#  define ENCODING_UCS2NATIVE  ENCODING_UCS2BE
#  define ENCODING_UCS4NATIVE  ENCODING_UCS4BE 
#endif 
 




struct _ZIconV {
	Int32 iFmtSrc; //source format
	Int32 iFmtDst; //destination format
};

static struct {
	Lpcstr cName;
	Int32  iFormat;
} encodings[] = {
	{ "ASCII",          ENCODING_ASCII      },
	{ "US-ASCII",       ENCODING_ASCII      },
	{ "8859-1",         ENCODING_LATIN1     },
	{ "ISO-8859-1",     ENCODING_LATIN1     },
	{ "UTF8",           ENCODING_UTF8       },
	{ "UTF-8",          ENCODING_UTF8       },
	{ "UTF16",          ENCODING_UTF16      },
	{ "UTF-16",         ENCODING_UTF16      },
	{ "UTF16BE",        ENCODING_UTF16BE    },
	{ "UTF-16BE",       ENCODING_UTF16BE    },
	{ "UTF16LE",        ENCODING_UTF16LE    },
	{ "UTF-16LE",       ENCODING_UTF16LE    },
	{ "UTF32",          ENCODING_UTF32      },
	{ "UTF-32",         ENCODING_UTF32      },
	{ "UTF32BE",        ENCODING_UTF32BE    },
	{ "UTF-32BE",       ENCODING_UTF32BE    },
	{ "UTF32LE",        ENCODING_UTF32LE    },
	{ "UTF-32LE",       ENCODING_UTF32LE    },
	{ "UCS2",           ENCODING_UCS2BE     },
	{ "UCS-2",          ENCODING_UCS2BE     },
	{ "UCS-2LE",        ENCODING_UCS2LE     },
	{ "UCS-2BE",        ENCODING_UCS2BE     },
	{ "UCS-2-INTERNAL", ENCODING_UCS2NATIVE },
	{ "UCS4",           ENCODING_UCS4BE     },
	{ "UCS-4",          ENCODING_UCS4BE     },
	{ "UCS-4LE",        ENCODING_UCS4LE     },
	{ "UCS-4BE",        ENCODING_UCS4BE     },
	{ "UCS-4-INTERNAL", ENCODING_UCS4NATIVE },
};





static Char* 
Z_GetEnv(
	_In_ Lpcstr _var) {

#if (Z_PLATFORM_WINDOWS)
	Char  env[Z_MAXPATH];
	Char* temp = getenv(_var);
	env[0] = '\0';
	if (temp != NULL)
		ExpandEnvironmentStrings(temp, env, sizeof(env));
	return (env[0] != '\0') ? strdup(env) : NULL;
#else
	Char* env = getenv(variable);
	return (env && env[0]) ? strdup(env) : NULL;
#endif 
}


static Lpcstr 
ZIconV_GetLocale(
	_Inout_ Char* _cBuffer,
	_In_    SizeT _buffersize) {

	Lpcstr cLanguage;
	Char*  cPtr;

	cLanguage = Z_GetEnv("LC_ALL");
	if (!cLanguage)
		cLanguage = Z_GetEnv("LC_CTYPE");
	if (!cLanguage)
		cLanguage = Z_GetEnv("LC_MESSAGES");
	if (!cLanguage)
		cLanguage = Z_GetEnv("LANG");
	if (!cLanguage || !*cLanguage || strcmp(cLanguage, "C") == 0)
		cLanguage = "ASCII";

	/*
	Trim strings such as "en_US.UTF-8@Z" to "UTF-8" */
	cPtr = (Char*)strchr(cLanguage, '.');
	if (cPtr != NULL)
		cLanguage = cPtr + 1;

	ZString_Strlcpy(_cBuffer, cLanguage, _buffersize);
	cPtr = strchr(_cBuffer, '@');
	if (cPtr != NULL)
		*cPtr = '\0';

	return _cBuffer;
}


ZIconV
ZIconV_Open(
	_In_ Lpcstr _tocode,
	_In_ Lpcstr _fromcode) {

	Int32  iFmtSrc, iFmtDst, it;
	Char   fromcode_buffer[64];
	Char   tocode_buffer[64];
	ZIconV iconv;

	iFmtSrc = ENCODING_UNKNOWN;
	iFmtDst = ENCODING_UNKNOWN;

	if (!_fromcode || !*_fromcode) {
		_fromcode = ZIconV_GetLocale(
			fromcode_buffer, sizeof(fromcode_buffer));
	}
	if (!_tocode || !*_tocode) {
		_tocode = ZIconV_GetLocale(
			tocode_buffer, sizeof(tocode_buffer));
	}
	for (it = 0; it < Z_ArraySize(encodings); ++it) {
		if (ZString_StrCaseCmp(_fromcode, encodings[it].cName) == 0) {
			iFmtSrc = encodings[it].iFormat;
			if (iFmtDst != ENCODING_UNKNOWN)
				break;
		}
		if (ZString_StrCaseCmp(_tocode, encodings[it].cName) == 0) {
			iFmtDst = encodings[it].iFormat;
			if (iFmtSrc != ENCODING_UNKNOWN)
				break;
		}
	}
	if (iFmtSrc != ENCODING_UNKNOWN && iFmtDst != ENCODING_UNKNOWN) {
		iconv = (ZIconV)malloc(sizeof(*iconv));
		if (iconv) {
			iconv->iFmtSrc = iFmtSrc;
			iconv->iFmtDst = iFmtDst;
			return iconv;
		}
	}
	return (ZIconV)-1;
}


SizeT 
ZIconV_Init(
	_Inout_ ZIconV _lpIconV,
	_In_    Lpcstr*  _inbuffer,
	_In_    SizeT*   _inbytesleft,
	_Inout_ Char**   _outbuffer,
	_In_    SizeT*   _outbytesleft) {

	/*
	convert everything to and from UCS-4 for simplicity*/
	Lpcstr src;
	Char*  dst;
	SizeT  total, srclen, dstlen;
	Uint32 ch;

	ch = 0;
	if (!_inbuffer || !*_inbuffer) {
		return 0;//Reset the context 
	}
	if (!_outbuffer || !*_outbuffer ||
		!_outbytesleft || !*_outbytesleft) {
		return Z_EICONVTOOBIG;
	}
	src = *_inbuffer;
	srclen = (_inbytesleft ? *_inbytesleft : 0);
	dst = *_outbuffer;
	dstlen = *_outbytesleft;

	switch (_lpIconV->iFmtSrc) {
	case ENCODING_UTF16:
	{/*
		Scan for a byte order marker */
		Uint8* p = (Uint8*)src;
		SizeT n = srclen / 2;
		while (n) {
			if (p[0] == 0xFF && p[1] == 0xFE) {
				_lpIconV->iFmtSrc = ENCODING_UTF16BE;
				break;
			}
			else if (p[0] == 0xFE && p[1] == 0xFF) {
				_lpIconV->iFmtSrc = ENCODING_UTF16LE;
				break;
			}
			p += 2;
			--n;
		}
		if (n == 0) {
			/*default to host order */
			_lpIconV->iFmtSrc = ENCODING_UTF16NATIVE;
		}
	}
	break;
	case ENCODING_UTF32:
	{/*
	 Scan for a byte order marker */
		Uint8* p = (Uint8*)src;
		SizeT  n = srclen / 4;
		while (n) {
			if (p[0] == 0xFF && p[1] == 0xFE &&
				p[2] == 0x00 && p[3] == 0x00) {
				_lpIconV->iFmtSrc = ENCODING_UTF32BE;
				break;
			}
			else if (p[0] == 0x00 && p[1] == 0x00 &&
				p[2] == 0xFE && p[3] == 0xFF) {
				_lpIconV->iFmtSrc = ENCODING_UTF32LE;
				break;
			}
			p += 4;
			--n;
		}
		if (n == 0) {
			/*default to host order */
			_lpIconV->iFmtSrc = ENCODING_UTF32NATIVE;
		}
	}
	break;
	}

	switch (_lpIconV->iFmtSrc) {
	case ENCODING_UTF16:
		/*
		Default to host order, need to add byte order marker */
		if (dstlen < 2) {
			return Z_EICONVTOOBIG;
		}
		*(Uint16*)dst = UNICODE_BOM;
		dst += 2;
		dstlen -= 2;
		_lpIconV->iFmtDst = ENCODING_UTF16NATIVE;
		break;

	case ENCODING_UTF32:
		/*
		Default to host order, need to add byte order marker */
		if (dstlen < 4) {
			return Z_EICONVTOOBIG;
		}
		*(Uint32*)dst = UNICODE_BOM;
		dst += 4;
		dstlen -= 4;
		_lpIconV->iFmtDst = ENCODING_UTF32NATIVE;
		break;
	}

	total = 0;
	while (srclen > 0) {
		/*
		Decode a character */
		switch (_lpIconV->iFmtSrc) {
		case ENCODING_ASCII:
		{
			Byte* p = (Byte*)src;
			ch = (Uint32)(p[0] & 0x7F);
			++src;
			--srclen;
		}
		break;
		case ENCODING_LATIN1:
		{
			Byte* p = (Byte*)src;
			ch = (Uint32)p[0];
			++src;
			--srclen;
		}
		break;
		case ENCODING_UTF8:
		{/* RFC 3629 */
			Byte* p = (Byte*)src;
			SizeT left = 0;
			Bool  overlong = Z_FALSE;
			if (p[0] >= 0xF0) {
				if ((p[0] & 0xF8) != 0xF0)
					ch = UNKNOWN_UNICODE;
				else {
					if (p[0] == 0xF0 && srclen > 1
						&& (p[1] & 0xF0) == 0x80) {
						overlong = Z_TRUE;
					}
					ch = (Uint32)(p[0] & 0x07);
					left = 3;
				}
			}
			else if (p[0] >= 0xE0) {
				if ((p[0] & 0xF0) != 0xE0)
					ch = UNKNOWN_UNICODE;
				else {
					if (p[0] == 0xE0 && srclen > 1
						&& (p[1] & 0xE0) == 0x80) {
						overlong = Z_TRUE;
					}
					ch = (Uint32)(p[0] & 0x0F);
					left = 2;
				}
			}
			else if (p[0] >= 0xC0) {
				if ((p[0] & 0xE0) != 0xC0)
					ch = UNKNOWN_UNICODE;
				else {
					if ((p[0] & 0xDE) == 0xC0) {
						overlong = Z_TRUE;
					}
					ch = (Uint32)(p[0] & 0x1F);
					left = 1;
				}
			}
			else {
				if ((p[0] & 0x80) != 0x00)
					ch = UNKNOWN_UNICODE;
				else ch = (Uint32)p[0];
			}
			++src;
			--srclen;
			if (srclen < left) {
				return Z_EICONVINVAL;
			}
			while (left--) {
				++p;
				if ((p[0] & 0xC0) != 0x80) {
					ch = UNKNOWN_UNICODE;
					break;
				}
				ch <<= 6;
				ch |= (p[0] & 0x3F);
				++src;
				--srclen;
			}
			if (overlong) {
				ch = UNKNOWN_UNICODE;
			}
			if ((ch >= 0xD800 && ch <= 0xDFFF) ||
				(ch == 0xFFFE || ch == 0xFFFF) || ch > 0x10FFFF) {
				ch = UNKNOWN_UNICODE;
			}
		}
		break;
		case ENCODING_UTF16BE:
		{/* RFC 2781 */
			Byte* p = (Byte*)src;
			Uint16 W1, W2;
			if (srclen < 2) {
				return Z_EICONVINVAL;
			}
			W1 = ((Uint16)p[0] << 8) | (Uint16)p[1];
			src += 2;
			srclen -= 2;
			if (W1 < 0xD800 || W1 > 0xDFFF) {
				ch = (Uint32)W1;
				break;
			}
			if (W1 > 0xDBFF) {
				ch = UNKNOWN_UNICODE;
				break;
			}
			if (srclen < 2) {
				return Z_EICONVINVAL;
			}
			p = (Byte*)src;
			W2 = ((Uint16)p[0] << 8) | (Uint16)p[1];
			src += 2;
			srclen -= 2;
			if (W2 < 0xDC00 || W2 > 0xDFFF) {
				ch = UNKNOWN_UNICODE;
				break;
			}
			ch = (((Uint32)(W1 & 0x3FF) << 10) |
				(Uint32)(W2 & 0x3FF)) + 0x10000;
		}
		break;
		case ENCODING_UTF16LE:
		{/* RFC 2781 */
			Byte*p = (Byte*)src;
			Uint16 W1, W2;
			if (srclen < 2) {
				return Z_EICONVINVAL;
			}
			W1 = ((Uint16)p[1] << 8) | (Uint16)p[0];
			src += 2;
			srclen -= 2;
			if (W1 < 0xD800 || W1 > 0xDFFF) {
				ch = (Uint32)W1;
				break;
			}
			if (W1 > 0xDBFF) {
				ch = UNKNOWN_UNICODE;
				break;
			}
			if (srclen < 2) {
				return Z_EICONVINVAL;
			}
			p = (Byte*)src;
			W2 = ((Uint16)p[1] << 8) | (Uint16)p[0];
			src += 2;
			srclen -= 2;
			if (W2 < 0xDC00 || W2 > 0xDFFF) {
				ch = UNKNOWN_UNICODE;
				break;
			}
			ch = (((Uint32)(W1 & 0x3FF) << 10) |
				(Uint32)(W2 & 0x3FF)) + 0x10000;
		}
		break;
		case ENCODING_UCS2LE:
		{
			Byte*p = (Byte*)src;
			if (srclen < 2) {
				return Z_EICONVINVAL;
			}
			ch = ((Uint32)p[1] << 8) | (Uint32)p[0];
			src += 2;
			srclen -= 2;
		}
		break;
		case ENCODING_UCS2BE:
		{
			Byte*p = (Byte*)src;
			if (srclen < 2) {
				return Z_EICONVINVAL;
			}
			ch = ((Uint32)p[0] << 8) | (Uint32)p[1];
			src += 2;
			srclen -= 2;
		}
		break;
		case ENCODING_UCS4BE:
		case ENCODING_UTF32BE:
		{
			Byte*p = (Byte*)src;
			if (srclen < 4) {
				return Z_EICONVINVAL;
			}
			ch = ((Uint32)p[0] << 24) |
				((Uint32)p[1] << 16) |
				((Uint32)p[2] << 8) | (Uint32)p[3];
			src += 4;
			srclen -= 4;
		}
		break;
		case ENCODING_UCS4LE:
		case ENCODING_UTF32LE:
		{
			Byte* p = (Byte*)src;
			if (srclen < 4) {
				return Z_EICONVINVAL;
			}
			ch = ((Uint32)p[3] << 24) |
				((Uint32)p[2] << 16) |
				((Uint32)p[1] << 8) | (Uint32)p[0];
			src += 4;
			srclen -= 4;
		}
		break;
		}
		/*
		Encode a character */
		switch (_lpIconV->iFmtDst) {
		case ENCODING_ASCII:
		{
			Byte* p = (Byte*)dst;
			if (dstlen < 1) {
				return Z_EICONVTOOBIG;
			}
			if (ch > 0x7F)
				*p = UNKNOWN_ASCII;
			else *p = (Byte)ch;
			++dst;
			--dstlen;
		}
		break;
		case ENCODING_LATIN1:
		{
			Byte* p = (Byte*)dst;
			if (dstlen < 1) {
				return Z_EICONVTOOBIG;
			}
			if (ch > 0xFF)
				*p = UNKNOWN_ASCII;
			else *p = (Byte)ch;

			++dst;
			--dstlen;
		}
		break;
		case ENCODING_UTF8:
		{   /* RFC 3629 */
			Byte* p = (Byte*)dst;
			if (ch > 0x10FFFF) {
				ch = UNKNOWN_UNICODE;
			}
			if (ch <= 0x7F) {
				if (dstlen < 1) {
					return Z_EICONVTOOBIG;
				}
				*p = (Byte)ch;
				++dst;
				--dstlen;
			}
			else if (ch <= 0x7FF) {
				if (dstlen < 2) {
					return Z_EICONVTOOBIG;
				}
				p[0] = 0xC0 | (Byte)((ch >> 6) & 0x1F);
				p[1] = 0x80 | (Byte)(ch & 0x3F);
				dst += 2;
				dstlen -= 2;
			}
			else if (ch <= 0xFFFF) {
				if (dstlen < 3) {
					return Z_EICONVTOOBIG;
				}
				p[0] = 0xE0 | (Byte)((ch >> 12) & 0x0F);
				p[1] = 0x80 | (Byte)((ch >> 6) & 0x3F);
				p[2] = 0x80 | (Byte)(ch & 0x3F);
				dst += 3;
				dstlen -= 3;
			}
			else {
				if (dstlen < 4) {
					return Z_EICONVTOOBIG;
				}
				p[0] = 0xF0 | (Byte)((ch >> 18) & 0x07);
				p[1] = 0x80 | (Byte)((ch >> 12) & 0x3F);
				p[2] = 0x80 | (Byte)((ch >> 6) & 0x3F);
				p[3] = 0x80 | (Byte)(ch & 0x3F);
				dst += 4;
				dstlen -= 4;
			}
		}
		break;
		case ENCODING_UTF16BE:
		{/* RFC 2781 */
			Byte*p = (Byte*)dst;
			if (ch > 0x10FFFF) {
				ch = UNKNOWN_UNICODE;
			}
			if (ch < 0x10000) {
				if (dstlen < 2) {
					return Z_EICONVTOOBIG;
				}
				p[0] = (Byte)(ch >> 8);
				p[1] = (Byte)ch;
				dst += 2;
				dstlen -= 2;
			}
			else {
				Uint16 W1, W2;
				if (dstlen < 4) {
					return Z_EICONVTOOBIG;
				}
				ch = ch - 0x10000;
				W1 = 0xD800 | (Uint16)((ch >> 10) & 0x3FF);
				W2 = 0xDC00 | (Uint16)(ch & 0x3FF);
				p[0] = (Uint8)(W1 >> 8);
				p[1] = (Uint8)W1;
				p[2] = (Uint8)(W2 >> 8);
				p[3] = (Uint8)W2;
				dst += 4;
				dstlen -= 4;
			}
		}
		break;
		case ENCODING_UTF16LE:
		{/* RFC 2781 */
			Byte*p = (Byte*)dst;
			if (ch > 0x10FFFF) {
				ch = UNKNOWN_UNICODE;
			}
			if (ch < 0x10000) {
				if (dstlen < 2) {
					return Z_EICONVTOOBIG;
				}
				p[1] = (Byte)(ch >> 8);
				p[0] = (Byte)ch;
				dst += 2;
				dstlen -= 2;
			}
			else {
				Uint16 W1, W2;
				if (dstlen < 4) {
					return Z_EICONVTOOBIG;
				}
				ch = ch - 0x10000;
				W1 = 0xD800 | (Uint16)((ch >> 10) & 0x3FF);
				W2 = 0xDC00 | (Uint16)(ch & 0x3FF);
				p[1] = (Uint8)(W1 >> 8);
				p[0] = (Uint8)W1;
				p[3] = (Uint8)(W2 >> 8);
				p[2] = (Uint8)W2;
				dst += 4;
				dstlen -= 4;
			}
		}
		break;
		case ENCODING_UCS2BE:
		{
			Byte *p = (Byte*)dst;
			if (ch > 0xFFFF) {
				ch = UNKNOWN_UNICODE;
			}
			if (dstlen < 2) {
				return Z_EICONVTOOBIG;
			}
			p[0] = (Byte)(ch >> 8);
			p[1] = (Byte)ch;
			dst += 2;
			dstlen -= 2;
		}
		break;
		case ENCODING_UCS2LE:
		{
			Byte* p = (Byte*)dst;
			if (ch > 0xFFFF) {
				ch = UNKNOWN_UNICODE;
			}
			if (dstlen < 2) {
				return Z_EICONVTOOBIG;
			}
			p[1] = (Byte)(ch >> 8);
			p[0] = (Byte)ch;
			dst += 2;
			dstlen -= 2;
		}
		break;
		case ENCODING_UTF32BE:
			if (ch > 0x10FFFF) {
				ch = UNKNOWN_UNICODE;
			}
			/* fallthrough */
		case ENCODING_UCS4BE:
			if (ch > 0x7FFFFFFF)
				ch = UNKNOWN_UNICODE;
			{
				Byte* p = (Byte*)dst;
				if (dstlen < 4) {
					return Z_EICONVTOOBIG;
				}
				p[0] = (Byte)(ch >> 24);
				p[1] = (Byte)(ch >> 16);
				p[2] = (Byte)(ch >> 8);
				p[3] = (Byte)ch;
				dst += 4;
				dstlen -= 4;
			}
			break;
		case ENCODING_UTF32LE:
			if (ch > 0x10FFFF) {
				ch = UNKNOWN_UNICODE;
			}
			/* fallthrough */
		case ENCODING_UCS4LE:
			if (ch > 0x7FFFFFFF)
				ch = UNKNOWN_UNICODE;
			{
				Byte* p = (Byte*)dst;
				if (dstlen < 4) {
					return Z_EICONVTOOBIG;
				}
				p[3] = (Byte)(ch >> 24);
				p[2] = (Byte)(ch >> 16);
				p[1] = (Byte)(ch >> 8);
				p[0] = (Byte)ch;
				dst += 4;
				dstlen -= 4;
			}
			break;
		}
		/*
		Update state */
		*_inbuffer = src;
		*_inbytesleft = srclen;
		*_outbuffer = dst;
		*_outbytesleft = dstlen;
		++total;
	}
	return total;
}


Int32 
ZIconV_Close(
	_Inout_ ZIconV _lpIconV) {

	if (_lpIconV != (ZIconV)-1)
		free(_lpIconV);
	return 0;
}


Char* 
ZIconV_ConvertString(
	_In_ Lpcstr _tocode,
	_In_ Lpcstr _fromcode,
	_In_ Lpcstr _inbuffer,
	_In_ SizeT  _inbytesleft) {

	ZIconV iconv;
	Char*  string;
	Char*  outbuffer;
	SizeT  stringsize, outbytesleft, returncode = 0;

	iconv = ZIconV_Open(_tocode, _fromcode);

	if (iconv == (ZIconV)-1) {
		if (!_tocode || !*_tocode)
			_tocode = "UTF-8";
		if (!_fromcode || !*_fromcode)
			_fromcode = "UTF-8";
		iconv = ZIconV_Open(_tocode, _fromcode);
	}
	if (iconv == (ZIconV)-1) {
		return NULL;
	}
	stringsize = _inbytesleft > 4 ? _inbytesleft : 4;
	string = (Char*)malloc(stringsize);
	if (!string) {
		ZIconV_Close(iconv);
		return NULL;
	}
	outbuffer = string;
	outbytesleft = stringsize;
	memset(outbuffer, 0, 4);

	while (_inbytesleft > 0) {
		const SizeT oldinbytesleft = _inbytesleft;
		returncode = ZIconV_Init(
			iconv,
			&_inbuffer,
			&_inbytesleft,
			&outbuffer,
			&outbytesleft);

		switch (returncode) {
		case Z_EICONVTOOBIG:
		{
			Char* oldstring = string;
			stringsize *= 2;
			string = (Char*)realloc(string, stringsize);
			if (!string) {
				ZIconV_Close(iconv);
				return NULL;
			}
			outbuffer = string + (outbuffer - oldstring);
			outbytesleft = stringsize - (outbuffer - string);
			memset(outbuffer, 0, 4);
		} break;

		case Z_EICONVILSEQ:
			++_inbuffer;
			--_inbytesleft;
			break;

		case Z_EICONVINVAL:
		case Z_EICONVERROR:
			_inbytesleft = 0;
			break;
		}
		if (oldinbytesleft == _inbytesleft)
			break; //Avoid infinite loops when nothing gets converted
	}
	ZIconV_Close(iconv);
	return string;
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/ 