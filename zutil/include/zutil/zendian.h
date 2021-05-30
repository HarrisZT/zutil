/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zendian.h 
* Desc: endian utility functions
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
#ifndef __ZENDIAN_H__
#define __ZENDIAN_H__  

#include "zcore.h"  
#if defined(__cplusplus)
extern "C" {
#endif



/*
16-Bit operations:
******************************************************************************/
#if (Z_COMPILER_GCC)
#  if defined(__i386__) && !(__GNUC__ == 2 && __GNUC_MINOR__ == 95)
	 FORCEINLINE Uint16
	 ZEndian_SwapU16(_In_ Uint16 x)
	 {
		 __asm__("xchgb %b0,%h0": "=q"(x) : "0"(x));
		 return x;
	 }
#  elif defined(__x86_64__)
	 FORCEINLINE Uint16 
	 ZEndian_SwapU16(_In_ Uint16 x)
	 {
		 __asm__("xchgb %b0,%h0": "=Q"(x) : "0"(x));
		 return x;
	 }
#  elif (defined(__powerpc__) || defined(__ppc__))
	 FORCEINLINE Uint16
	 ZEndian_SwapU16(_In_ Uint16 x)
	 {
		 Int32 result;
		 __asm__(
			 "rlwimi %0,%2,8,16,23":
		 "=&r"(result) : "0"(x >> 0x08), "r"(x));
		 return (Uint16)result;
	 }
#  elif (defined(__M68000__) || defined(__M68020__)) && !defined(__mcoldfire__)
	 FORCEINLINE Uint16
	 ZEndian_SwapU16(_In_ Uint16 x)
	 {
		 __asm__("rorw #8,%0": "=d"(x) : "0"(x) : "cc");
		 return x;
	 }
#  endif
#else
	FORCEINLINE Uint16
	ZEndian_SwapU16(_In_ Uint16 x) {
		return (Uint16)((x << 0x08) | (x >> 0x08));
	}
#endif 

/*
32-Bit operations:
******************************************************************************/
#if (Z_COMPILER_GCC)
#  if defined(__i386__)
	 FORCEINLINE Uint32
	 ZEndian_SwapU32(_In_ Uint32 x)
	 {
		 __asm__("bswap %0": "=r"(x) : "0"(x));
		 return x;
	 }
#  elif defined(__x86_64__)
	 FORCEINLINE Uint32
	 ZEndian_SwapU32(_In_ Uint32 x)
	 {
		 __asm__("bswapl %0": "=r"(x) : "0"(x));
		 return x;
	 }
#  elif (defined(__powerpc__) || defined(__ppc__))
	 FORCEINLINE Uint32
	 ZEndian_SwapU32(_In_ Uint32 x)
	 {
		 Uint32 result;
		 __asm__(
			 "rlwimi %0,%2,24,16,23":
		 "=&r"(result) : "0"(x >> 0x18), "r"(x));
		 __asm__(
			 "rlwimi %0,%2,8,8,15":
		 "=&r"(result) : "0"(result), "r"(x));
		 __asm__(
			 "rlwimi %0,%2,24,0,7":
		 "=&r"(result) : "0"(result), "r"(x));
		 return result;
	 }
#  elif (defined(__M68000__) || defined(__M68020__)) && !defined(__mcoldfire__)
	 FORCEINLINE Uint32 
	 ZEndian_SwapU32(_In_ Uint32 x)
	 {
		 __asm__(
			 "rorw #8,%0\n\tswap %0\n\trorw #8,%0":
		 "=d"(x) : "0"(x) : "cc");
		 return x;
	 }
#  endif
#else
	FORCEINLINE Uint32
	ZEndian_SwapU32(_In_ Uint32 x) {
		return (Uint32)(
			(x << 0x18) | ((x << 0x08) & 0x00FF0000) |
			((x >> 0x08) & 0x0000FF00) | (x >> 0x18));
	}
#endif 

/*
64-Bit operations:
******************************************************************************/
#if (Z_COMPILER_GCC)
#  if defined(__i386__)
	 FORCEINLINE Uint64
	 ZEndian_SwapU64(_In_ Uint64 x)
	 {
		 union
		 {
			 struct
			 {
				 Uint32 a, b;
			 } s;
			 Uint64 u;
		 } v;
		 v.u = x;
		 __asm__(
			 "bswapl %0 ; bswapl %1 ; xchgl %0,%1":
		 "=r"(v.s.a), "=r"(v.s.b) : "0"(v.s.a), "1"(v.s.b));
		 return v.u;
	 }
#  elif defined(__x86_64__)
	 FORCEINLINE Uint64 
	 ZEndian_SwapU64(_In_ Uint64 x)
	 {
		 __asm__("bswapq %0": "=r"(x) : "0"(x));
		 return x;
	 }
#  endif
#else
	FORCEINLINE Uint64
	ZEndian_SwapU64(_In_ Uint64 x) {
		Uint32 hi, lo;
		lo = (Uint32)(x & 0xFFFFFFFF);
		x >>= 0x20;
		hi = (Uint32)(x & 0xFFFFFFFF);
		x = ZEndian_SwapU32(lo);
		x <<= 0x20;
		x |= ZEndian_SwapU32(hi);
		return (x);
	}
#endif

/*
32-Bit Float operations:
******************************************************************************/
FORCEINLINE Float 
ZEndian_SwapF32(_In_ Float x) {
	union {
		Float  f32;
		Uint32 u32;
	} swapVal;
	swapVal.f32 = x;
	swapVal.u32 = ZEndian_SwapU32(swapVal.u32);
	return swapVal.f32;
}

/* 
This function is similar to memcpy except that it reverses bytes.
@_dst   : data to be initialized
@_src   : data to copy into _dst
@_nbytes: size of data to be copied*/
FORCEINLINE Void
ZEndian_ReverseCopy(
	_Inout_ Handle  _dst,
	_In_    Lpcvoid _src,
	_In_    Int32   _nbytes) {

	Byte*       d;
	const Byte* s;
	d = (Byte*)_dst;
	s = (const Byte*)_src;
	d += _nbytes;
	while (_nbytes--)
		*--d = *s++;
}



#if (Z_ENDIAN_LIL)
#  define ZEndian_SwapLE16(_x)  (_x)
#  define ZEndian_SwapLE32(_x)  (_x)
#  define ZEndian_SwapLE64(_x)  (_x)
#  define ZEndian_SwapF32LE(_x) (_x)
#  define ZEndian_SwapBE16(_x)  ZEndian_SwapU16(_x)
#  define ZEndian_SwapBE32(_x)  ZEndian_SwapU32(_x)
#  define ZEndian_SwapBE64(_x)  ZEndian_SwapU64(_x)
#  define ZEndian_SwapF32BE(_x) ZEndian_SwapF32(_x)
#else
#  define ZEndian_SwapLE16(_x)  ZEndian_SwapU16(_x)
#  define ZEndian_SwapLE32(_x)  ZEndian_SwapU32(_x)
#  define ZEndian_SwapLE64(_x)  ZEndian_SwapU64(_x)
#  define ZEndian_SwapF32LE(_x) ZEndian_SwapF32(_x)
#  define ZEndian_SwapBE16(_x)  (_x)
#  define ZEndian_SwapBE32(_x)  (_x)
#  define ZEndian_SwapBE64(_x)  (_x)
#  define ZEndian_SwapF32BE(_x) (_x)
#endif 

#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  