/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zqueue.h
* Desc: A queue structure, contains a sequence of data
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
#ifndef __ZQUEUE_H__
#define __ZQUEUE_H__

#include "zcore.h"
#if defined(__cplusplus)
extern "C" {
#endif



/*
A queue structure, contains a sequence of data*/
typedef struct _ZQueue ZQueue;

/*
Allocate and return an empty queue structure
@_nElements: the size of the queue
@return    : an allocated structure, must be freed when finished*/
extern ZQueue* ZAPI
ZQueue_Create(
	_In_ SizeT _nElements);

/*
Insert data into the back of the queue.
@_lpQueue: the queue to insert data into
@_lpData : the data to be inserted
@return  : Z_OK on success, error code on failure*/
extern ZRESULT ZAPI
ZQueue_Push(
	_Inout_ ZQueue* _lpQueue,
	_In_    Handle  _lpData);

/*
Remove the front element from the queue. 
@_lpQueue: the queue to remove data from
@_lpData : (optional) a variable to contain the value of the data*/
extern Void ZAPI
ZQueue_Pop(
	_Inout_     ZQueue* _lpQueue,
	_Inout_opt_ Handle  _lpData);

/*
Return the value contained in the front element of the queue.
@_lpQueue: the queue to get data from
@return  : the value of the data*/
extern Handle ZAPI
ZQueue_Front(
	_In_ const ZQueue* _lpQueue); 
 
/*
Removes all data from the queue.
@_lpQueue: the queue to clear
@return  : Z_OK on success, error code on failure*/
extern ZRESULT ZAPI
ZQueue_Clear(
	_Inout_ ZQueue* _lpQueue);

/*
Deallocates the queue structure (and removes all data).
@_lpQueue: the queue to free*/
extern Void ZAPI
ZQueue_Release(
	_Inout_ ZQueue* _lpQueue);

/*
Returns size of the queue structure
@_lpQueue: the queue to get the size of
@return  : a count of the number of elements in the queue*/
extern SizeT ZAPI
ZQueue_GetSize(
	_In_ const ZQueue* _lpQueue);

/*
Reports whether the queue is empty.
@_lpQueue: the queue to check the status of
@return  : true if queue is empty else false*/
extern Bool ZAPI
ZQueue_IsEmpty(
	_In_ const ZQueue* _lpQueue);



#ifdef __cplusplus
} 
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  