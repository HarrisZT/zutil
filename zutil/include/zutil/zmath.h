/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zmath.h
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
#ifndef __ZMATH_H__
#define __ZMATH_H__  

#include <math.h>
#include "zcore.h" 
#if defined(__cplusplus)
extern "C" {
#endif



#define ZMATH_E          2.71828182845904523536f 
#define ZMATH_FULLCIRCLE 360.f
#define ZMATH_HALFCIRCLE 180.f
#define ZMATH_LN2	     0.69314718055994530942f
#define ZMATH_LN10	     2.30258509299404568402f
#define ZMATH_LOG2E	     1.44269504088896340736f
#define ZMATH_LOG10E	 0.43429448190325182765f
#define	ZMATH_NANMASK    (0xFF << 0x17)
#define ZMATH_PI         3.14159265358979323846f 
#define ZMATH_PI2        1.57079632679489661923f 
#define ZMATH_PI4        0.78539816339744830962f 
#define ZMATH_SQRT1_2	 0.70710678118654752440f 
#define ZMATH_SQRT2	     1.41421356237309504880f
#define ZMATH_TAU        6.28318530717958647693f
#define ZMATH_QUADSIZE   0x6

/*
identication of easing function*/
typedef enum { 
	ZMATH_EASING_QUARTIN,
	ZMATH_EASING_QUARTOUT,
	ZMATH_EASING_QUARTINOUT,
	ZMATH_EASING_CUBICIN,
	ZMATH_EASING_CUBICOUT,
	ZMATH_EASING_CUBICINOUT,
	ZMATH_EASING_QUADIN,
	ZMATH_EASING_QUADOUT,
	ZMATH_EASING_QUADINOUT,
	ZMATH_EASING_SINEIN,
	ZMATH_EASING_SINEOUT,
	ZMATH_EASING_SINEINOUT,
	ZMATH_EASING_QUINTIN,
	ZMATH_EASING_QUINTOUT,
	ZMATH_EASING_QUINTINOUT,
	ZMATH_EASING_EXPOIN,
	ZMATH_EASING_EXPOOUT,
	ZMATH_EASING_EXPOINOUT,
	ZMATH_EASING_CIRCIN,
	ZMATH_EASING_CIRCOUT,
	ZMATH_EASING_CIRCINOUT,
	ZMATH_EASING_BACKIN,
	ZMATH_EASING_BACKOUT,
	ZMATH_EASING_BACKINOUT,
	ZMATH_EASING_BOUNCEIN,
	ZMATH_EASING_BOUNCEOUT,
	ZMATH_EASING_BOUNCEINOUT, 
	ZMATH_EASING_ELASTICIN,
	ZMATH_EASING_ELASTICOUT,
	ZMATH_EASING_ELASTICINOUT,
} ZMATH_EASING;

 

/*
Returns the absolute value of the given (x)*/
#define ZMath_Abs(x)\
  ((x) < 0 ? -(x) : (x)) 

/*
Determines if the given values are in ascending order*/
#define ZMath_Ascending(i, j)\
  ((Int32)i < (Int32)j) 

/*
Clamps a given value (x) within the range of rmin and rmax*/
#define ZMath_Clamp(x, rmin, rmax) \
  (rmin < rmax ? \
  (x < rmin ? rmin : x > rmax ? rmax : x):\
  (x < rmax ? rmax : x > rmin ? rmin : x))

/*
Converts a given degree to equivalent radian*/
#define ZMath_DegToRad(x)\
  (((x) * ZMATH_PI) / 180.0f)

/*
Calculates the dot product of the given values*/
#define ZMath_DotProduct(x1, y1, x2, y2)\
  (x1 * x2 + y1 * y2) 

/*
Returns the given length as squared*/
#define ZMath_GetLengthSquared(x, y)\
  (x * x + y * y) 

/*
Equates to True if given number is even*/
#define ZMath_IsEven(x)\
  (((Int32)x & 1) == 0) 

/*
Determines whether a given value is nan*/
#define	ZMath_IsNan(x)\
  (((*(Int32*)&x)&ZMATH_NANMASK) == ZMATH_NANMASK)

/*
Converts a given radian to equivalent degree*/
#define ZMath_RadToDeg(x)\
  (((x) * 180.0f) / ZMATH_PI)





extern Uint64 ZAPI
ZMath_AlignU64(
	_In_ Uint64 _value, 
	_In_ Uint64 _align);


extern Float ZAPI
ZMath_AngleDelta(
	_In_ Float _angle1, 
	_In_ Float _angle2);

/*
Returns the angle of a given vector from 0 to 360°
depending on its direction on the unit circle*/
extern Float ZAPI
ZMath_AngleGetRotation(
	_In_ Float _x, 
	_In_ Float _y);


extern Float ZAPI
ZMath_AngleLerp(
	_In_ Float _from, 
	_In_ Float _to,
	_In_ Float _frac);


extern Float ZAPI	
ZMath_AngleMod(
	_In_ Float _a);


extern Float ZAPI
ZMath_AngleNormalize180(
	_In_ Float _angle);


extern Float ZAPI
ZMath_AngleNormalize360(
	_In_ Float _angle);

/*
Finds the shortest rotation (degrees) 
between the given start and end angles*/
extern Float ZAPI
ZMath_AngleShortestRotation(
	_In_ Float _start, 
	_In_ Float _end);


extern Float ZAPI
ZMath_AngleSubtract(
	_In_ Float _a1, 
	_In_ Float _a2);


extern Int32 ZAPI
ZMath_BitCount(
	_In_ Int32 x);


extern Int32 ZAPI
ZMath_BitReverse(
	_In_ Int32 x);


extern Int32 ZAPI
ZMath_BitsForFloat(
	_In_ Float x);


extern Int32 ZAPI 
ZMath_BitsForInteger(
	_In_ Int32 x);


extern Int32 ZAPI
ZMath_CeilPowerOf2(
	_In_ Int32 x);


extern Float ZAPI 
ZMath_Fabs(
	_In_ Float x);


extern Int32 ZAPI
ZMath_FloorPowerOf2(
	_In_ Int32 x);


extern Float ZAPI
ZMath_Fraction(
	_In_ Float x);

/* 
based on https://github.com/nicolausYes/easing-functions 
Float parameter is a given amount of time / progress in a normalised
0 - 1 range, and the function returns a value 0 - 1 based on the current 
position along a curve. 
Examples of each curve can be found at https://easings.net/en  
Easing curves are generally useful for creating interesting motion in 
animated items such as UI objects. They can also be used to create tables
of values where lookups are faster than repeatedly calling functions
that use sin/cos/sqrt.
@_enum : enum value specifying which easing function to use
@_norm : normalized time/progress range (0-1)
@return: the resulting easing value*/
extern Float ZAPI
ZMath_Easing(
	_In_ ZMATH_EASING _enum, 
	_In_ Float          _norm);

/*
Returns the value of seed being used for randomization
@return: the randomizer seed*/
extern Dword ZAPI
ZMath_GetRandSeed(Void);


extern Bool ZAPI
ZMath_IsPowerOf2(
	_In_ Int32 x);


extern Int32 ZAPI
ZMath_Log2f(
	_In_ Float x);


extern Int32 ZAPI
ZMath_Log2i(
	_In_ Int32 x);


extern Int32 ZAPI 
ZMath_MaskForFloatSign(
	_In_ Float x);


extern Int32 ZAPI
ZMath_MaskForIntegerSign(
	_In_ Int32 x);

/* 
Returns a random Byte from a lookup table*/
extern Byte ZAPI
ZMath_RandByte(Void); 

/* 
Returns a random Float within the range.
@_signed: determines whether the returned value will be signed or unsigned
          if true,  the value will be between (+0.0 and +1.0) 
	      if false, the value will be between (-1.0 and +1.0)
@return : a random float*/
extern Float ZAPI
ZMath_RandFloat(
    _In_ Bool _signed); 

/* 
Returns a random Int32 within the range of (0 and _iMax)
@_iMax : the max range value that can be returned
@return: a random integer*/
extern Int32 ZAPI
ZMath_RandInt( 
	_In_ Int32 _iMax);


extern Float ZAPI
ZMath_Rint(
	_In_ Float x);


extern Float ZAPI
ZMath_Rsqrt(
	_In_ Float x);

/*
Reset the randomizer seed.
@_dwSeed: the new seed*/
extern Void ZAPI
ZMath_SetRandSeed(
	_In_ Dword _dwSeed); 



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/ 