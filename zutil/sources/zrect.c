/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zrect.c
* Desc: rect structure defining x/y coordinates and width/height
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
#include "zutil/zrect.h"



Bool
ZRect_IsEqual(
	_In_ const ZRECT* _r0,
	_In_ const ZRECT* _r1) {

	Bool isEqual;
	isEqual = !(
		(_r0->x != _r1->x) ||
		(_r0->y != _r1->y) ||
		(_r0->w != _r1->w) ||
		(_r0->h != _r1->h));
	return isEqual;
}


ZVEC2
ZRect_GetPosition(
	_In_ const ZRECT* _r0) {

	ZVEC2 pos;
	pos.x = _r0->x;
	pos.y = _r0->y;
	return pos;
}


ZVEC2
ZRect_GetSize(
	_In_ const ZRECT* _r0) {

	ZVEC2 size;
	size.x = _r0->w;
	size.y = _r0->h;
	return size;
}
 

ZVEC2
ZRect_GetCenter(
	_In_ const ZRECT* _r0) {

	ZVEC2 center;
	center.x = _r0->x + (_r0->w / 2);
	center.y = _r0->y + (_r0->h / 2);
	return center;
}
 

Float 
ZRect_GetPerimeter(
	_In_ const ZRECT* _r0) {

	Float perimeter;
	perimeter = (Float)((_r0->w + _r0->h) * 2);
	return perimeter;
}


Float
ZRect_GetArea(
	_In_ const ZRECT* _r0) {

	Float area;
	area = (Float)(_r0->w * _r0->h);
	return area;
}


Float
ZRect_GetRight(
	_In_ const ZRECT* _r0) {

	Float right;
	right = (Float)(_r0->x + _r0->w);
	return right;
}


Float
ZRect_GetBottom(
	_In_ const ZRECT* _r0) {

	Float bottom;
	bottom = (Float)(_r0->y + _r0->h);
	return bottom;
}


Bool 
ZRect_ContainsPoint(
	_In_ const ZRECT* _r0,
	_In_ Float        _x,
	_In_ Float        _y) {

	Bool result;
	Float xmin, xmax, ymin, ymax;
	xmin = Z_Min(_r0->x, (Float)(_r0->x + _r0->w));
	xmax = Z_Max(_r0->x, (Float)(_r0->x + _r0->w));
	ymin = Z_Min(_r0->y, (Float)(_r0->y + _r0->h));
	ymax = Z_Max(_r0->y, (Float)(_r0->y + _r0->h));
	result = !(
		(_x < xmin)  ||
		(_x >= xmax) ||
		(_y < ymin)  ||
		(_y >= ymax));
	return result;
}

 
Bool 
ZRect_ContainsRect(
	_In_ const ZRECT* _r0,
	_In_ const ZRECT* _r1) {

	Bool result;
	result = !(
		(_r1->x < _r0->x) ||
		(_r1->y < _r0->y) ||
		(_r1->x + _r1->w > _r0->x + _r0->w) ||
		(_r1->y + _r1->h > _r0->y + _r0->h));
	return result;
}


Bool 
ZRect_GetIntersection(
	_In_        const ZRECT* _r0,
	_In_        const ZRECT* _r1,
	_Inout_opt_ ZRECT*       _out) {

	Bool  isIntersection;
	Float xmin[2], xmax[2], ymin[2], ymax[2];
	Float resultX, resultY, resultR, resultB;

	xmin[0] = Z_Min(_r0->x, (Float)(_r0->x + _r0->w));
	xmax[0] = Z_Max(_r0->x, (Float)(_r0->x + _r0->w));
	ymin[0] = Z_Min(_r0->y, (Float)(_r0->y + _r0->h));
	ymax[0] = Z_Max(_r0->y, (Float)(_r0->y + _r0->h));
	xmin[1] = Z_Min(_r1->x, (Float)(_r1->x + _r1->w));
	xmax[1] = Z_Max(_r1->x, (Float)(_r1->x + _r1->w));
	ymin[1] = Z_Min(_r1->y, (Float)(_r1->y + _r1->h));
	ymax[1] = Z_Max(_r1->y, (Float)(_r1->y + _r1->h));
	resultX = Z_Max(xmin[0], xmin[1]);
	resultY = Z_Max(ymin[0], ymin[1]);
	resultR = Z_Min(xmax[0], xmax[1]);
	resultB = Z_Min(ymax[0], ymax[1]);

	if ((resultX < resultR) && (resultY < resultB)) {
		isIntersection = Z_TRUE;
		if (_out) {
			_out->x = resultX;
			_out->y = resultY;
			_out->w = resultR - resultX;
			_out->h = resultB - resultY;
		}
	}
	else {
		isIntersection = Z_FALSE;
		if (_out) {
			_out->x = 0;
			_out->y = 0;
			_out->w = 0;
			_out->h = 0;
		}
	}
	return isIntersection;
}

 
ZRECT
ZRect_Merge(
	_In_ const ZRECT* _r0,
	_In_ const ZRECT* _r1) {

	ZRECT result;
	result.x = Z_Min(_r0->x, _r1->x);
	result.y = Z_Min(_r0->y, _r1->y);
	result.w = Z_Max(_r0->x + _r0->w, _r1->x + _r1->w);
	result.h = Z_Max(_r0->y + _r0->h, _r1->y + _r1->h);
	result.w -= result.x;
	result.h -= result.y;
	return result;
}


Void
ZRect_Normalize(
	_In_ ZRECT* _r0) {

	_r0->x = Z_Min(_r0->x, (_r0->x + _r0->w));
	_r0->y = Z_Min(_r0->y, (_r0->y + _r0->h));
	_r0->w = Z_Max(_r0->x, (_r0->x + _r0->w));
	_r0->h = Z_Max(_r0->y, (_r0->y + _r0->h));
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/  