/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zrect.h
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
#ifndef __ZRECT_H__
#define __ZRECT_H__
 
#include "zvec2.h"
#if defined(__cplusplus)
extern "C" {
#endif
 


/*
A structure defining a rectangle, with 4 values
(x-y coordinates and width-height dimensions)*/
typedef struct _ZRectangle {
	union {
		struct {
			Float x; //coordinate left 
			Float y; //coordinate top
			Float w; //width of rect
			Float h; //height of rect
		}; 
		//rect fields as an array:
		//[0] - x (coordinate left) 
		//[1] - y (coordinate top)
		//[2] - w (width of rect)
		//[3] - h (height of rect) 
		Float v[4];  
	};
} ZRECT; 
    



/*
Returns true if the two given rectangles are equal, else returns false*/
extern Bool 
ZRect_IsEqual(
	_In_ const ZRECT* _r0, 
	_In_ const ZRECT* _r1); 

/*
Returns the x-y position of the upper-left corner of the rectangle*/
extern ZVEC2 ZAPI
ZRect_GetPosition(
	_In_ const ZRECT* _rect);

/*
Returns the size of the rectangle*/
extern ZVEC2 ZAPI
ZRect_GetSize(
	_In_ const ZRECT* _rect);
 
/*
Returns the x-y position of the rectangle center*/
extern ZVEC2 ZAPI
ZRect_GetCenter(
	_In_ const ZRECT* _rect);
 
/*
Returns the calculated perimeter of the rectangle*/
extern Float ZAPI
ZRect_GetPerimeter(
	_In_ const ZRECT* _rect);

/*
Returns the calculated area of the rectangle*/
extern Float ZAPI
ZRect_GetArea(
	_In_ const ZRECT* _rect);

/*
Returns the x position of the right side*/
extern Float ZAPI
ZRect_GetRight(
	_In_ const ZRECT* _rect);

/*
Returns the y position of the bottom side*/
extern Float ZAPI
ZRect_GetBottom(
	_In_ const ZRECT* _rect);

/*
Determines whether the coordinates are contained within a rect
@_rect : the rect to test for coordinates
@_x    : x coordinate
@_y    : y coordinate
@return: True if the coordinates are inside the rect else false*/
extern Bool ZAPI
ZRect_ContainsPoint(
	_In_ const ZRECT* _rect,
	_In_ Float        _x,
	_In_ Float        _y);
			          
/*
Determines whether a rect is contained within another rect
@_rect0: a rect to compare with _rect1
@_rect1: a rect to compare with _rect0
@return: True if the rect1 is inside of rect0, else false*/
extern Bool ZAPI
ZRect_ContainsRect(
	_In_ const ZRECT* _rect0,
	_In_ const ZRECT* _rect1);

/*
Determines whether a rect intersects with another rect
@_rect0  : a rect to compare with _rect1
@_rect1  : a rect to compare with _rect0
@_rectOut: a rect to contain the resulting intersection
@return  : True if the rects intersect, else false*/
extern Bool ZAPI
ZRect_GetIntersection(
	_In_        const ZRECT* _rect0,
	_In_        const ZRECT* _rect1,
	_Inout_opt_ ZRECT*       _rectOut);
 
/* 
Merges 2 given rects and returns the resulting rect
@_rect0:  the first rect
@_rect1:  the second rect
@_return: the resulting merged rect*/
extern ZRECT ZAPI
ZRect_Merge(
	_In_ const ZRECT* _rect0,
	_In_ const ZRECT* _rect1);

/* 
Returns a normalized copy of the given rect
@_rect : the rect to normalize 
@return: the resulting normalized rect*/
extern Void ZAPI
ZRect_Normalize(
	_In_ ZRECT* _rect);



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  