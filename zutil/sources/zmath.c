/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zmath.c 
* Desc: math utilities 
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
#include "zutil/zvec2.h"
#include "zutil/zmath.h"



Uint64 
ZMath_AlignU64(
	_In_ Uint64 _value,
	_In_ Uint64 _align) {

	Uint64 alignment;
	alignment = (_value + _align - 1) & ~(_align - 1);
	return alignment;
}


Float
ZMath_AngleDelta(
	_In_ Float _angle1,
	_In_ Float _angle2) {

	Float delta;
	delta = ZMath_AngleNormalize180(_angle1 - _angle2);
	return delta;
}


Float 
ZMath_AngleGetRotation(
	_In_ Float _x,
	_In_ Float _y) {

	Float nx, ny, rot, result;
	ZVEC2 vec;

	vec.x = _x;
	vec.y = _y;
	ZVec2_Normal(&vec);

	nx  = acosf(vec.x);
	ny  = asinf(vec.y);
	rot = nx;
	if (ny < 0)
		rot = ZMath_DegToRad(360.f) - nx;
	result = ZMath_RadToDeg(rot);

	return result;
}


Float 
ZMath_AngleLerp(
	_In_ Float _from,
	_In_ Float _to,
	_In_ Float _frac) {

	Float lerp;
	if (_to - _from > 180.f)
		_to -= 360.f;
	if (_to - _from < -180.f)
		_to += 360.f;
	lerp = _from + _frac * (_to - _from);
	return lerp;
}


Float 
ZMath_AngleMod(
	_In_ Float _angle) {

	_angle = (Float)(
		(360.0 / 65536) * ((Int32)(_angle * (65536 / 360.0)) & 65535));
	return _angle;
}


Float 
ZMath_AngleNormalize180(
	_In_ Float _angle) {

	_angle = ZMath_AngleNormalize360(_angle);
	if (_angle > 180.0)
		_angle -= 360.0;
	return _angle;
}


Float 
ZMath_AngleNormalize360(
	_In_ Float _angle) {

	Float norm;
	norm = (Float)(
		(360.0 / 65536) * ((Int32)(_angle * (65536 / 360.0)) & 65535));
	return norm;
}


Float 
ZMath_AngleShortestRotation(
	_In_ Float _start,
	_In_ Float _end) {

	Float srot = _end - _start;
	if (srot > +180.f) { return srot - 360.f; }
	if (srot < -180.f) { return srot + 360.f; }
	return srot;
}


Float
ZMath_AngleSubtract(
	_In_ Float _a1,
	_In_ Float _a2) {

	Float a = _a1 - _a2;
	while (a > 180.f) { a -= 360.f; }
	while (a < -180.f) { a += 360.f; }
	return a;
}


Int32 
ZMath_BitCount(
	_In_ Int32 x) {

	x -= ((x >> 0x01) & 0x55555555);
	x = (((x >> 0x02) & 0x33333333) + (x & 0x33333333));
	x = (((x >> 0x04) + x) & 0x0F0F0F0F);
	x += (x >> 0x08);
	return ((x + (x >> 0x10)) & 0x0000003F);
}


Int32 
ZMath_BitReverse(
	_In_ Int32 x) {

	x = (((x >> 0x01) & 0x55555555) | ((x & 0x55555555) << 0x01));
	x = (((x >> 0x02) & 0x33333333) | ((x & 0x33333333) << 0x02));
	x = (((x >> 0x04) & 0x0F0F0F0F) | ((x & 0x0F0F0F0F) << 0x04));
	x = (((x >> 0x08) & 0x00FF00FF) | ((x & 0x00FF00FF) << 0x08));
	return ((x >> 0x10) | (x << 0x10));
}


Int32 
ZMath_BitsForFloat(
	_In_ Float x) {
	return ZMath_Log2f(x) + 1;
} 


Int32 
ZMath_BitsForInteger(
	_In_ Int32 x) {
	return ZMath_Log2f((Float)x) + 1;
}


Int32 
ZMath_CeilPowerOf2(
	_In_ Int32 x) {

	x--;
	x |= x >> 0x01;
	x |= x >> 0x02;
	x |= x >> 0x04;
	x |= x >> 0x08;
	x |= x >> 0x10;
	x++;
	return x;
} 


Float
ZMath_Fabs(
	_In_ Float x) {

	Int32 tmp;
	tmp = *(Int32*)&x;
	tmp &= 0x7FFFFFFF;
	return *(Float*)&tmp;
}


Int32 
ZMath_FloorPowerOf2(
	_In_ Int32 x) {

	x |= x >> 0x01;
	x |= x >> 0x02;
	x |= x >> 0x04;
	x |= x >> 0x08;
	x |= x >> 0x10;
	x++;
	return x >> 0x01;
} 


Float 
ZMath_Fraction(
	_In_ Float x) {

	Float frac;
	frac = x - floorf(x);
	return frac;
}


Bool 
ZMath_IsPowerOf2(
	_In_ Int32 x) {
	return (x & (x - 1)) == 0 && x > 0;
}


Int32 
ZMath_Log2f(
	_In_ Float x) {

	return (Int32)(
		(((*(Int32*)(&x)) >> Z_FLT_MANTISSA_BITS) &
	    ((1 << Z_FLT_EXPONENT_BITS) - 1)) - Z_FLT_EXPONENT_BIAS);
}


Int32 
ZMath_Log2i(
	_In_ Int32 x) { 
	return ZMath_Log2f((Float)x);
} 


Int32 
ZMath_MaskForFloatSign(
	_In_ Float x) { 
	return ((*(Int32*)(&x)) >> Z_FLT_SIGN_BIT);
} 


Int32 
ZMath_MaskForIntegerSign(
	_In_ Int32 x) {
	return (x >> INT32_SIGN_BIT);
}


Float
ZMath_Rint(
	_In_ Float x) {
	return floorf(x + 0.5f);
}


Float 
ZMath_Rsqrt(
	_In_ Float x) {

	Float x2, y;
	Long  i;
	x2 = x * 0.5f; 
	y  = x;
	i  = *(Long*)&y;
	i = 0x5F3759DF - (i >> 1);
	y = *(Float*)&i;
	y = y * (1.5f - (x2 * y * y));    
	return y;
} 





/*
Easing functions
******************************************************************************/
Float 
ZMath_Easing(
	_In_ ZMATH_EASING _eEasing, 
	_In_ Float        _f) {

	Float f2;
	switch (_eEasing) {

	case ZMATH_EASING_BOUNCEIN:
		return
			powf(2.f, 6.f * (_f - 1.f)) *
			(Float)fabs(sinf(_f * ZMATH_PI * 3.5f));

	case ZMATH_EASING_BOUNCEOUT:
		return
			1.f - powf(2.f, -6.f * _f) *
			(Float)fabs(cosf(_f * ZMATH_PI * 3.5f));

	case ZMATH_EASING_BOUNCEINOUT:
		if (_f < 0.5f)
			return
			8.f * powf(2.f, 8.f * (_f - 1.f)) *
			(Float)fabs(sinf(_f * ZMATH_PI * 7.f));
		return
			1.f - 8.f * powf(2.f, -8.f * _f) *
			(Float)fabs(sinf(_f * ZMATH_PI * 7.f));

	case ZMATH_EASING_ELASTICIN:
		f2 = _f * _f;
		return f2 * f2 * sinf(_f * ZMATH_PI * 4.5f);

	case ZMATH_EASING_ELASTICOUT:
		f2 = (_f - 1.f) * (_f - 1.f);
		return 1.f - f2 * f2 * cosf(_f * ZMATH_PI * 4.5f);

	case ZMATH_EASING_ELASTICINOUT:
		if (_f < 0.45f) {
			f2 = _f * _f;
			return 8.f * f2 * f2 * sinf(_f * ZMATH_PI * 9.f);
		}
		else if (_f < 0.55f)
			return 0.5f + 0.75f * sinf(_f * ZMATH_PI * 4.f);
		f2 = (_f - 1.f) * (_f - 1.f);
		return 1.f - 8.f * f2 * f2 * sinf(_f * ZMATH_PI * 9.f);

	case ZMATH_EASING_BACKIN:
		return _f * _f * (2.70158f * _f - 1.70158f);

	case ZMATH_EASING_BACKOUT:
		return 1.f + (--_f) * _f * (2.70158f * _f + 1.70158f);

	case ZMATH_EASING_BACKINOUT:
		if (_f < 0.5f)
			return _f * _f * (7.f * _f - 2.5f) * 2.f;
		return 1.f + (--_f) * _f * 2.f * (7.f * _f + 2.5f);

	case ZMATH_EASING_CIRCIN:
		return 1.f - sqrtf(1.f - _f);

	case ZMATH_EASING_CIRCOUT:
		return sqrtf(_f);

	case ZMATH_EASING_CIRCINOUT:
		if (_f < 0.5f)
			return (1.f - sqrtf(1.f - 2.f * _f)) * 0.5f;
		return (1.f + sqrtf(2.f * _f - 1.f)) * 0.5f;

	case ZMATH_EASING_EXPOIN:
		return (powf(2.f, 8.f * _f) - 1) / 255.f;

	case ZMATH_EASING_EXPOOUT:
		return 1.f - powf(2.f, -8.f * _f);

	case ZMATH_EASING_EXPOINOUT:
		if (_f < 0.5f)
			return (powf(2.f, 16.f * _f) - 1.f) / 510.f;
		return 1.f - 0.5f * powf(2.f, -16.f * (_f - 0.5f));

	case ZMATH_EASING_QUINTIN:
		f2 = _f * _f;
		return _f * f2 * f2;

	case ZMATH_EASING_QUINTOUT:
		f2 = (--_f) * _f;
		return 1.f + _f * f2 * f2;

	case ZMATH_EASING_QUINTINOUT:
		if (_f < 0.5f) {
			f2 = _f * _f;
			return 16.f * _f * f2 * f2;
		}
		f2 = (--_f) * _f;
		return 1.f + 16.f * _f * f2 * f2;

	case ZMATH_EASING_SINEIN:
		return sinf(1.5707963f * _f);

	case ZMATH_EASING_SINEOUT:
		return 1 + sinf(1.5707963f * (--_f));

	case ZMATH_EASING_SINEINOUT:
		return 0.5f * (1 + sinf(ZMATH_PI * (_f - 0.5f)));

	case ZMATH_EASING_QUADIN:
		return _f * _f;

	case ZMATH_EASING_QUADOUT:
		return _f * (2.f - _f);

	case ZMATH_EASING_QUADINOUT:
		if (_f < 0.5f)
			return 2.f * _f * _f;
		return _f * (4.f - 2.f * _f) - 1;

	case ZMATH_EASING_CUBICIN:
		return _f * _f * _f;

	case ZMATH_EASING_CUBICOUT:
		return 1.f + (--_f) * _f * _f;

	case ZMATH_EASING_CUBICINOUT:
		if (_f < 0.5f)
			return 4.f * _f * _f * _f;
		return 1.f + (--_f) * (2.f * (--_f)) * (2.f * _f);

	case ZMATH_EASING_QUARTIN:
		_f *= _f;
		return _f * _f;

	case ZMATH_EASING_QUARTOUT:
		_f = (--_f) * _f;
		return 1.f - _f * _f;

	case ZMATH_EASING_QUARTINOUT:
		if (_f < 0.5f) {
			_f *= _f;
			return 8.f * _f * _f;
		}
		_f = (--_f) * _f;
		return 1.f - 8.f * _f * _f;
	} 
	return _f; 
}





/*
Random Numbers
******************************************************************************/
#define	Z_ONE    0x3F800000UL 
#define	Z_MASK   0x007FFFFFUL 
#define ZMAXRAND 0x7FFF

static Dword m_seed;


Void
ZMath_SetRandSeed(
	_In_ Dword _dwSeed) { 
	m_seed = _dwSeed;
}


Dword
ZMath_GetRandSeed(Void) {
	return m_seed;
}	 
 

Int32
ZMath_RandInt(
	_In_ Int32 _iMaxValue) {

	Int32 iResult;
	if (_iMaxValue == 0) {
		return 0; //(prevent divide by zero)
	}
	m_seed =
		1664525L * m_seed + 1013904223L;
	iResult = ((Int32)m_seed & ZMAXRAND);
	iResult = (iResult >>
		      (0x10 - ZMath_BitsForInteger(_iMaxValue))) % _iMaxValue;
	return iResult;
}  
	 

Float 
ZMath_RandFloat(
	_In_ Bool _signed) {

	Float fResult;
	Dword dwRand;
	m_seed = 1664525L * m_seed + 1013904223L;
	dwRand = Z_ONE | (m_seed & Z_MASK);
	if (_signed)
		fResult = (2.0f * (*(Float*)&dwRand) - 3.0f);
	else fResult = ((*(Float*)&dwRand) - 1.0f);
	return fResult;
} 


Byte 
ZMath_RandByte(Void) {
	static Byte randbytes[] = {
	    0,   8, 109, 220, 222, 241, 149, 107,  75, 248, 254, 140,  16,  66,
	    74,  21, 211,  47,  80, 242, 154,  27, 205, 128, 161,  89,  77,  36,
	    95, 110,  85,  48, 212, 140, 211, 249,  22,  79, 200,  50,  28, 188,
	    52, 140, 202, 120,  68, 145,  62,  70, 184, 190,  91, 197, 152, 224,
	    149, 104,  25, 178, 252, 182, 202, 182, 141, 197,   4,  81, 181, 242,
	    145,  42,  39, 227, 156, 198, 225, 193, 219,  93, 122, 175, 249,   0,
	    175, 143,  70, 239,  46, 246, 163,  53, 163, 109, 168, 135,   2, 235,
	    25,  92,  20, 145, 138,  77,  69, 166,  78, 176, 173, 212, 166, 113,
	    94, 161,  41,  50, 239,  49, 111, 164,  70,  60,   2,  37, 171,  75,
	    136, 156,  11,  56,  42, 146, 138, 229,  73, 146,  77,  61,  98, 196,
	    135, 106,  63, 197, 195,  86,  96, 203, 113, 101, 170, 247, 181, 113,
	    80, 250, 108,   7, 255, 237, 129, 226,  79, 107, 112, 166, 103, 241,
	    24, 223, 239, 120, 198,  58,  60,  82, 128,   3, 184,  66, 143, 224,
	    145, 224,  81, 206, 163,  45,  63,  90, 168, 114,  59,  33, 159,  95,
	    28, 139, 123,  98, 125, 196,  15,  70, 194, 253,  54,  14, 109, 226,
	    71,  17, 161,  93, 186,  87, 244, 138,  20,  52, 123, 251,  26,  36,
	    17,  46,  52, 231, 232,  76,  31, 221,  84,  37, 216, 165, 212, 106,
	    197, 242,  98,  43,  39, 175, 254, 145, 190,  84, 118, 222, 187, 136,
	    120, 163, 236, 249
	};
	static Int32 idx;
	idx = (idx + 1) & 0xFF;
	return randbytes[idx];
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/  