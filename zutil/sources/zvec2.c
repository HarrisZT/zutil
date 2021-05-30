/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zvec2.c
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
#include "zutil/zmath.h"
#include "zutil/zvec2.h"

 

Float 
ZVec2_Angle(
	_In_ const ZVEC2* _v0) {

	Float angle;
	angle =
		(Float)((_v0->x == 0 && _v0->y == 0) ?
			0 : atan2(_v0->y, _v0->x));
	return angle;
}


Float 
ZVec2_Area(
	_In_ const ZVEC2* _v0) {

	Float area;
	area = (Float)(_v0->x * _v0->y);
	return area;
}


ZVEC2
ZVec2_BezierCurve3(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1,
	_In_ const ZVEC2* _v2,
	_In_ Float        _degrees) {

	ZVEC2 v[3];
	v[0] = ZVec2_Lerp(_v0, _v1, _degrees);
	v[1] = ZVec2_Lerp(_v1, _v2, _degrees);
	v[2] = ZVec2_Lerp(&v[0], &v[1], _degrees);
	return v[2];
}


ZVEC2
ZVec2_BezierCurve4(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1,
	_In_ const ZVEC2* _v2,
	_In_ const ZVEC2* _v3,
	_In_ Float        _degrees) {

	ZVEC2 v[6];
	v[0] = ZVec2_Lerp(_v0, _v1, _degrees);
	v[1] = ZVec2_Lerp(_v1, _v2, _degrees);
	v[2] = ZVec2_Lerp(_v2, _v3, _degrees);
	v[3] = ZVec2_Lerp(&v[0], &v[1], _degrees);
	v[4] = ZVec2_Lerp(&v[1], &v[2], _degrees);
	v[5] = ZVec2_Lerp(&v[3], &v[4], _degrees);
	return v[5];
}


ZVEC2 
ZVec2_Ceil(
	_In_ const ZVEC2* _v0) {

	ZVEC2 out;
	out.x = ceilf(_v0->x);
	out.y = ceilf(_v0->y);
	return out;
}


Float 
ZVec2_Distance(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1) {

	Float distance;
	distance = (Float)(sqrtf(
		(_v0->x - _v1->x) * (_v0->x - _v1->x) +
		(_v0->y - _v1->y) * (_v0->y - _v1->y)
	));
	return distance;
}


Float
ZVec2_DotProduct(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1) {

	Float dp;
	dp = (Float)(_v0->x * _v1->x + _v0->y * _v1->y);
	return dp;
}


ZVEC2 
ZVec2_Floor(
	_In_ const ZVEC2* _v0) {

	ZVEC2 out;
	out.x = floorf(_v0->x);
	out.y = floorf(_v0->y);
	return out;
}


Float
ZVec2_Length(
	_In_ const ZVEC2* _v0) {

	return (Float)(sqrtf(
		(Float)_v0->x * (Float)_v0->x +
		(Float)_v0->y * (Float)_v0->y));
}


ZVEC2
ZVec2_Lerp(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1,
	_In_ Float        _degrees) {

	ZVEC2 out;
	out.x = _v0->x + (_v1->x - _v0->x) * _degrees;
	out.y = _v0->y + (_v1->y - _v0->y) * _degrees;
	return out;
}
	 

ZVEC2
ZVec2_Max(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1) {

	ZVEC2 out;
	out.x = Z_Max(_v0->x, _v1->x);
	out.y = Z_Max(_v0->y, _v1->y);
	return out;
}


ZVEC2
ZVec2_Min(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1) {

	ZVEC2 out;
	out.x = Z_Min(_v0->x, _v1->x);
	out.y = Z_Min(_v0->y, _v1->y);
	return out;
}

 
ZVEC2
ZVec2_Normal(
	_In_ const ZVEC2* _v0) {

	ZVEC2 out;
	Float n;
	n = sqrtf(
		(Float)_v0->x * (Float)_v0->x +
		(Float)_v0->y * (Float)_v0->y);
	out.x = _v0->x / (Float)n;
	out.y = _v0->y / (Float)n;
	return out;
}
 

ZVEC2
ZVec2_Polar(
	_In_ const ZVEC2* _v0) {

	ZVEC2 out;
	if (_v0->x == 0 && _v0->y == 0) {
		out.x = 0;
		out.y = 0;
	}
	else {
		out.x = ZVec2_Normal(_v0).x;
		out.y = ZVec2_Angle(_v0);
	}
	return out;
}


ZVEC2 
ZVec2_Projection(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _v1) {

	Float d, s;
	ZVEC2 out;
	d = ZVec2_DotProduct(_v1, _v1);
	s = ZVec2_DotProduct(_v0, _v1) / d;
	out.x = _v1->x * s;
	out.y = _v1->y * s;
	return out;
}


ZVEC2
ZVec2_Rectangular(
	_In_ const ZVEC2* _v0) {

	ZVEC2 out;
	out.x = cosf(_v0->y);
	out.y = sinf(_v0->y) * _v0->x;
	return out;
}


ZVEC2
ZVec2_Reflection(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _normal) {

	Float dp;
	ZVEC2 out;
	dp = ZVec2_DotProduct(_v0, _normal) * 2;
	out.x = _normal->x * dp - _v0->x;
	out.y = _normal->y * dp - _v0->y;
	return out;
}


ZVEC2 
ZVec2_Rotation(
	_In_ const ZVEC2* _v0,
	_In_ Float        _degrees) {

	Float cs, sn;
	ZVEC2 vec;
	cs = cosf(_degrees);
	sn = sinf(_degrees);
	vec.x = _v0->x * cs - _v0->y * sn;
	vec.y = _v0->x * sn + _v0->y * cs;
	return vec;
}


ZVEC2
ZVec2_Slide(
	_In_ const ZVEC2* _v0,
	_In_ const ZVEC2* _normal) {

	Float dp;
	ZVEC2 out;
	dp = ZVec2_DotProduct(_v0, _normal);
	out.x = _v0->x - _normal->x * dp;
	out.y = _v0->y - _normal->y * dp;
	return out;
}


ZVEC2
ZVec2_Tangent(
	_In_ const ZVEC2* _v0) {

	ZVEC2 vec;
	vec.x = _v0->y;
	vec.y = -_v0->x;
	return vec;
}


ZVEC2 
ZVec2_Unit(
	_In_ const ZVEC2* _v0) {

	ZVEC2 vec, norm;
	norm = ZVec2_Normal(_v0);
	vec.x = _v0->x / norm.x;
	vec.y = _v0->y / norm.y;
	return vec;
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/  