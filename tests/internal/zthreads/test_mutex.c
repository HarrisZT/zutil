/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: test_timedmutex.c
* Desc: unit test for ZThreads 
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
#include "testunit_module_zthreads.h" 


#define NANOSECS_PER_SECOND         1000000000 
#define MUTEX_LOCKING_THREADCOUNT   128
#define MUTEX_RECURSIVE_THREADCOUNT 128
#define TEST_RECURSIVE_ITERATIONS   10000


typedef struct {
	ZMutex*         mutex;
	struct timespec start;
	struct timespec timeout;
	struct timespec end;
	struct timespec upper;
} TimedMutexData;


typedef struct {
	ZMutex*        mutex;
	volatile Int32 iterator;
	volatile Int32 completed;
} MutexTestData;





static Int32 
TimedMutexFunc(
	_Inout_ Handle _hArg) {

	Int32           ret;
	struct timespec ts; 
	TimedMutexData* data;

	data = (TimedMutexData*)_hArg; 
	ret  = ZMutex_LockEx(data->mutex, &(data->timeout));
	assert(ret == Z_ETIMEDOUT);

	timespec_get(&ts, TIME_UTC);
	ret = ZChrono_CompareTimespec(&ts, &(data->start));
	assert(ret >= 0);
	ret = ZChrono_CompareTimespec(&ts, &(data->timeout));
	assert(ret >= 0);
	ret = ZChrono_CompareTimespec(&ts, &(data->end));
	assert(ret < 0);

	ret = ZMutex_Lock(data->mutex);
	assert(ret == Z_OK);

	timespec_get(&ts, TIME_UTC);
	ret = ZChrono_CompareTimespec(&ts, &(data->end));
	assert(ret >= 0);

	ret = ZChrono_CompareTimespec(&ts, &(data->upper));
	assert(ret < 0);

	ZMutex_Unlock(data->mutex);
	return Z_OK;
}


static Int32 
ThreadLock(
	_Inout_ Handle _hArg) { 

	Int32   it;
	ZMutex* mutex;

	Z_Unused(_hArg);

	for (it = 0; it < TEST_RECURSIVE_ITERATIONS; ++it) {
		ZMutex_Lock(g_mutex);
		assert(ZMutex_TryLock(g_mutex) == Z_EBUSY);
		++g_count;
		ZMutex_Unlock(g_mutex);
	}
	mutex = ZMutex_Create();

	ZMutex_Lock(g_mutex);

	for (it = 0; it < TEST_RECURSIVE_ITERATIONS; ++it) {
		assert(ZMutex_TryLock(mutex) == Z_OK);
		assert(ZMutex_TryLock(mutex) == Z_EBUSY);
		++g_count;
		ZMutex_Unlock(mutex);
	}
	ZMutex_Unlock(g_mutex); 
	ZMutex_Release(mutex);

	return Z_OK;
} 


static Int32 
RecursiveMutexCallback(
	_Inout_ Handle _hdata) {  

	Int32 it;
	MutexTestData* mutexdata;
	mutexdata = (MutexTestData*)_hdata;

	assert(ZMutex_Lock(mutexdata->mutex) == Z_OK);

	for (it = 0; it < TEST_RECURSIVE_ITERATIONS; it++) {
		ZMutex_Lock(mutexdata->mutex);
		assert(mutexdata->iterator++ == it);
	}
	for (it = TEST_RECURSIVE_ITERATIONS - 1; it >= 0; it--) {
		ZMutex_Unlock(mutexdata->mutex);
		assert(--(mutexdata->iterator) == it);
	}
	assert(mutexdata->iterator == 0);

	mutexdata->completed++;
	ZMutex_Unlock(mutexdata->mutex);

	return Z_OK;
}





/* Section II: Unit Tests 
******************************************************************************/ 
Void Test_TimedMutex(Void) { 

	TimedMutexData  data;
	ZThread         thread;
	struct timespec start;
	struct timespec end;
	struct timespec interval = Z_INITZERO;	

	interval.tv_sec  = 0;
	interval.tv_nsec = (NANOSECS_PER_SECOND / 10) * 2;

	data.mutex = ZMutex_CreateEx(Z_MUTEX_TIMED, NULL);
	ZMutex_Lock(data.mutex);

	timespec_get(&(data.start), TIME_UTC);

	data.timeout = data.start;
	ZChrono_AddNanosecsToTimespec(
		&(data.timeout), 
		NANOSECS_PER_SECOND / 10, 
		NANOSECS_PER_SECOND);

	data.end = data.timeout;
	ZChrono_AddNanosecsToTimespec(
		&(data.end), 
		NANOSECS_PER_SECOND / 10, 
		NANOSECS_PER_SECOND);

	data.upper = data.end;
	ZChrono_AddNanosecsToTimespec(
		&(data.upper), 
		NANOSECS_PER_SECOND / 10, 
		NANOSECS_PER_SECOND);

	ZThread_Init(&thread, TimedMutexFunc, &data);

	timespec_get(&start, TIME_UTC);
	assert(ZThread_Sleep(&interval, &interval) == 0);
	timespec_get(&end, TIME_UTC);
	ZMutex_Unlock(data.mutex);
	ZThread_Join(thread, NULL);
	ZMutex_Release(data.mutex);
}


Void Test_Sleep(Void) {

	struct timespec ts;
	struct timespec interval;
	struct timespec end_ts;

	interval.tv_sec  = 0;
	interval.tv_nsec = NANOSECS_PER_SECOND / 10;
	 
	//Calculate current time + 100ms 
	timespec_get(&ts, TIME_UTC);
	ZChrono_AddNanosecsToTimespec(
		&ts, 
		NANOSECS_PER_SECOND / 10, 
		NANOSECS_PER_SECOND);
	 
	//Sleep... 
	ZThread_Sleep(&interval, NULL); 
	timespec_get(&end_ts, TIME_UTC); 

	assert(ZChrono_CompareTimespec(&ts, &end_ts) <= 0);
}


Void Test_ThreadTime(Void) {
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
} 


Void Test_MutexLocking(Void) {

	ZThread thread[MUTEX_LOCKING_THREADCOUNT];
	Int32   it;

	g_count = 0;

	for (it = 0; it < MUTEX_LOCKING_THREADCOUNT; ++it) {
		ZThread_Init(&(thread[it]), ThreadLock, NULL);
	} 
	for (it = 0; it < MUTEX_LOCKING_THREADCOUNT; ++it) {
		ZThread_Join(thread[it], NULL);
	} 
	assert(g_count == 
		(MUTEX_LOCKING_THREADCOUNT * 
		 TEST_RECURSIVE_ITERATIONS * 2));
} 


Void Test_RecursiveMutex(Void) { 

	ZThread       thread[MUTEX_RECURSIVE_THREADCOUNT];
	Int32         it;
	MutexTestData testdata;

	testdata.mutex     = ZMutex_CreateEx(Z_MUTEX_RECURSIVE, NULL);
	testdata.iterator  = 0;
	testdata.completed = 0;

	for (it = 0; it < MUTEX_RECURSIVE_THREADCOUNT; it++) {
		ZThread_Init(
			&(thread[it]), 
			RecursiveMutexCallback, 
			&testdata);
	}
	for (it = 0; it < MUTEX_RECURSIVE_THREADCOUNT; it++) {
		ZThread_Join(thread[it], NULL);
	}
	ZMutex_Release(testdata.mutex);

	assert(testdata.completed == MUTEX_RECURSIVE_THREADCOUNT); 
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/  