/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zqueue.c
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
#include "zutil/zqueue.h"




typedef struct _ZQueueData {
	Handle              data;
	struct _ZQueueData* next;
} ZQueueData;

struct _ZQueue {
	SizeT       count;
	SizeT       datasize;
	ZQueueData* head;
	ZQueueData* tail;
};





ZQueue*
ZQueue_Create(
	_In_ SizeT _datasize) {

	ZQueue* queue;
	queue = (ZQueue*)malloc(sizeof(queue));
	if (queue != NULL) {
		queue->datasize = _datasize;
		queue->count    = 0;
		queue->head     = NULL;
		queue->tail     = NULL;
	}
	return queue;
}


ZRESULT 
ZQueue_Push(
	_Inout_ ZQueue* _lpQueue,
	_In_    Handle  _lpData) {

	ZQueueData* newData;

	if (_lpQueue != NULL) {
		newData = (ZQueueData*)malloc(sizeof(ZQueueData));
		if (newData == NULL) {
			return Z_EOUTOFMEMORY;
		}
		newData->data = malloc(_lpQueue->datasize);
		if (newData->data == NULL) {
			free(newData);
			return Z_EOUTOFMEMORY;
		}
		newData->next = NULL;
		memcpy(newData->data, _lpData, _lpQueue->datasize);

		if (_lpQueue->count != 0) {
			_lpQueue->tail->next = newData;
			_lpQueue->tail       = newData;
		}
		else {
			_lpQueue->head = newData;
			_lpQueue->tail = newData;
		}
		_lpQueue->count++;
	}
	return Z_OK;
}


Void
ZQueue_Pop(
	_Inout_     ZQueue* _lpQueue,
	_Inout_opt_ Handle  _lpData) {

	ZQueueData* oldData;

	if (!ZQueue_IsEmpty(_lpQueue)) {

		oldData = _lpQueue->head;
		if (_lpQueue->count == 1) {
			if (_lpData)
				memcpy(_lpData, oldData->data, _lpQueue->datasize);
			free(oldData->data);
			free(oldData);
			_lpQueue->head = NULL;
			_lpQueue->tail = NULL;
			_lpQueue->count--;
			return;
		}
		_lpQueue->head = _lpQueue->head->next;
		if (_lpData)
			memcpy(_lpData, oldData->data, _lpQueue->datasize);
		free(oldData->data);
		free(oldData);
		_lpQueue->count--;
	}
}


Handle 
ZQueue_Front(
	_In_ const ZQueue* _lpQueue) {

	Handle front; 
	front = NULL;
	if (!ZQueue_IsEmpty(_lpQueue))
		memcpy(front, _lpQueue->head->data, _lpQueue->datasize);
	return front;
}


ZRESULT
ZQueue_Clear(
	_Inout_ ZQueue* _lpQueue) {

	ZQueueData* tmp;
	if (_lpQueue != NULL) {
		while (!ZQueue_IsEmpty(_lpQueue)) {
			tmp = _lpQueue->head;
			_lpQueue->head = _lpQueue->head->next;
			free(tmp->data);
			free(tmp);
			_lpQueue->count--;
		}
		return Z_OK;
	}
	return Z_EPOINTER;
}


SizeT 
ZQueue_GetSize(
	_In_ const ZQueue* _lpQueue) { 
	return _lpQueue ? _lpQueue->count : 0; 
}


Bool 
ZQueue_IsEmpty(
	_In_ const ZQueue* _lpQueue) {

	if (_lpQueue == NULL || !_lpQueue->count)
		return Z_TRUE;
	return Z_FALSE;
}


Void 
ZQueue_Release(
	_Inout_ ZQueue* _lpQueue) {

	if (_lpQueue) {
		ZQueue_Clear(_lpQueue);
		free(_lpQueue);
		_lpQueue = NULL;
	}
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/ 