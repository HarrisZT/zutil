/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zvec2.h 
* Desc: A structure that defines a point in 2D space with x-y coordinates
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
#ifndef __ZVEC2_H__
#define __ZVEC2_H__

#include "zcore.h"
#if defined(__cplusplus)
extern "C" {
#endif



/*  
A structure defining a vector 
(a point in 2D space with x-y cartesian coordinates)*/ 
typedef struct _ZVector2D {
	union {
		struct {
			Float x; //coordinate left
			Float y; //coordinate top
		}; 
		//coordinate fields as an array:
		//[0] - x (coordinate left) 
		//[1] - y (coordinate top) 
		Float v[2];  
	};
} ZVEC2; 



	 

/* 
** Vector Math Functions*/
//***************************************************************************//

extern Float ZAPI
ZVec2_Angle(
	_In_ const ZVEC2* _v0); 


extern Float ZAPI
ZVec2_Area(
	_In_ const ZVEC2* _v0); 


extern ZVEC2 ZAPI
ZVec2_BezierCurve3(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1,
	_In_ const ZVEC2* _v2,
	_In_ Float        _degrees); 


extern ZVEC2 ZAPI
ZVec2_BezierCurve4(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1,
	_In_ const ZVEC2* _v2,
	_In_ const ZVEC2* _v3,
	_In_ Float        _degrees); 


extern ZVEC2 ZAPI
ZVec2_Ceil(
	_In_ const ZVEC2* _v0); 


extern Float ZAPI
ZVec2_Distance(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1); 


extern Float ZAPI
ZVec2_DotProduct(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1); 


extern ZVEC2 ZAPI
ZVec2_Floor(
	_In_ const ZVEC2* _v0); 


extern Float ZAPI
ZVec2_Length(
	_In_ const ZVEC2* _v0); 


extern ZVEC2 ZAPI
ZVec2_Lerp(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1,
	_In_ Float        _degrees); 


extern ZVEC2 ZAPI
ZVec2_Max(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1); 


extern ZVEC2 ZAPI
ZVec2_Min(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1); 
 

extern ZVEC2 ZAPI
ZVec2_Normal(
	_In_ const ZVEC2* _v0); 


extern ZVEC2 ZAPI
ZVec2_Polar(
	_In_ const ZVEC2* _v0); 


extern ZVEC2 ZAPI
ZVec2_Projection(
    _In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1); 


extern ZVEC2 ZAPI
ZVec2_Rectangular(
	_In_ const ZVEC2* _v0);

/* 
Reflects a velocity vector from a given normal
@_velocity: the velocity vector
@_normal  : the normalized vector
@return   : the resulting reflection*/ 
extern ZVEC2 ZAPI
ZVec2_Reflection(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _normal);

/*
Rotates two points with a given angle (in degrees)*/  
extern ZVEC2 ZAPI
ZVec2_Rotation(
	_In_ const ZVEC2* _v0,
	_In_ Float        _degrees); 


extern ZVEC2 ZAPI
ZVec2_Slide(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _normal); 


extern ZVEC2 ZAPI
ZVec2_Tangent(
	_In_ const ZVEC2* _v0); 


extern ZVEC2 ZAPI
ZVec2_Unit(
	_In_ const ZVEC2* _v0);



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  