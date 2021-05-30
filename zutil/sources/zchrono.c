/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zchrono.c 
* Desc: timing utilities 
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
#include "zutil/zmutex.h"
#include "zutil/zchrono.h"

#if (Z_PLATFORM_WINDOWS) 
#  define EPOCH_FILETIME 0x19DB1DED53E8000ULL
#else
#  if (Z_PLATFORM_MACOS) || (Z_PLATFORM_IOS)
#    include <mach/mach_time.h>
#  else
#    include <time.h>
#  endif
#endif

 



/* Section 1:
** Internal data*/
//***************************************************************************//
struct _ZTimer {
	ZTimeValue value;
}; 
static const ZTimeValue ZTIMEVALUE_ZERO = { 0 }; 
static volatile Bool    s_init        = Z_FALSE;
static ZMutex*          s_mutex       = NULL;

#if (Z_PLATFORM_WINDOWS)
    static Real64 s_inverseTime = 0;
    static Bool   s_isXpOrOlder = Z_FALSE;
#endif

 

 
 
static ZTimeValue
ZChrono_GetCurrentTime(Void) {  

	ZTimeValue ztimevalue; 
	ztimevalue = ZTIMEVALUE_ZERO;
	if (!s_init) {
		return ztimevalue;
	} 
#if (Z_PLATFORM_WINDOWS)
	LARGE_INTEGER lgInt;
	if (!s_isXpOrOlder)
		QueryPerformanceCounter(&lgInt);
	else {
		ZMutex_Lock(s_mutex);
		QueryPerformanceCounter(&lgInt);
		ZMutex_Unlock(s_mutex);
	}
	ZChrono_SetTimeAsMicroseconds(
		&ztimevalue,
		(Int64)(lgInt.QuadPart * s_inverseTime));

#elif ((Z_PLATFORM_MACOS) || (Z_PLATFORM_IOS)) 
    static mach_timebase_info_data_t frequency = { 0, 0 };
    if (frequency.denom == 0)
        mach_timebase_info(&frequency);
    Uint64 nanoseconds = 
		mach_absolute_time() * frequency.numer / frequency.denom;
    ztimevalue.microseconds = (nanoseconds / 1000);

#else
	struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    ztimevalue.microseconds = 
		((Uint64)(ts.tv_sec) * 1000000 + ts.tv_nsec / 1000);
#endif  
	return ztimevalue;
}





/* Section 2:
** ZTimer Interface*/
//***************************************************************************//
ZRESULT 
ZChrono_Init(Void) {
	ZRESULT zresult;
	zresult = Z_OK;

	if (!s_init && !s_mutex) {
		s_mutex = ZMutex_Create();
		if (s_mutex == NULL) {
			s_init = Z_FALSE;
			zresult = Z_EFAIL;
		}
	}
#if (Z_PLATFORM_WINDOWS)  
	ZWINVER       win32ver; 
	LARGE_INTEGER freq; 
    win32ver = ZWin32_GetVersion(); 
	QueryPerformanceFrequency(&freq); 
	s_inverseTime = 1000000.0 / freq.QuadPart;
	s_isXpOrOlder = win32ver <= ZWINVER_XP3;
#endif
	return zresult;
}


Void
ZChrono_Release(Void) {
	if (s_mutex != NULL) {
		ZMutex_Release(s_mutex);
		s_mutex = NULL;
	}
}


ZTimer*
ZChrono_CreateTimer(Void) {
	ZTimer* ztimer;
	ztimer = (ZTimer*)malloc(sizeof(struct _ZTimer));
	if (ztimer != NULL) 
		ztimer->value = ZChrono_GetCurrentTime();
	return ztimer;
}


Void
ZChrono_ReleaseTimer(
	_Inout_ ZTimer* _ztimer) {

	if (_ztimer != NULL) {
		free(_ztimer);
		_ztimer = NULL;
	}
}


ZTimeValue
ZChrono_GetTimerElapsed(
	_In_ const ZTimer* _ztimer) {

	ZTimeValue ztimevalue;
	ztimevalue.microseconds = 
		ZChrono_GetCurrentTime().microseconds -
		_ztimer->value.microseconds;
	return ztimevalue;
}


ZTimeValue
ZChrono_RestartTimer(
	_Inout_ ZTimer* _ztimer) {

	Int64      elapsed;
	ZTimeValue ztimevalue;

	ztimevalue = ZChrono_GetCurrentTime();
    elapsed = 
		ztimevalue.microseconds - 
		_ztimer->value.microseconds;
    _ztimer->value.microseconds = elapsed; 

	return ztimevalue;
} 





/* Section 3:
** ZTimeValue interface*/
//***************************************************************************//
Float 
ZChrono_GetTimeAsSeconds(
    _In_ const ZTimeValue* _ztimevalue) {
	return (Float)(_ztimevalue->microseconds / 1000000);
}


Int32
ZChrono_GetTimeAsMilliseconds(
    _In_ const ZTimeValue* _ztimevalue) {
	return (Int32)(_ztimevalue->microseconds / 1000);
}


Int64
ZChrono_GetTimeAsMicroseconds(
    _In_ const ZTimeValue* _ztimevalue) {
	return _ztimevalue->microseconds;
}
 

Void
ZChrono_SetTimeAsSeconds(
	_Inout_ ZTimeValue* _ztimevalue,
	_In_    Float       _seconds) {  
	_ztimevalue->microseconds = (Int64)(_seconds * 1000000);
}


Void
ZChrono_SetTimeAsMilliseconds(
	_Inout_ ZTimeValue* _ztimevalue,
	_In_    Int32       _milliseconds) {  
	_ztimevalue->microseconds = (Int64)(_milliseconds * 1000);
}

 
Void
ZChrono_SetTimeAsMicroseconds(
	_Inout_ ZTimeValue* _ztimevalue,
	_In_    Int64       _microseconds) {  
	_ztimevalue->microseconds = _microseconds;
}





/* Section 4:
** ZChrono Interface*/
//***************************************************************************//
Void
ZChrono_AddNanosecsToTimespec(
	_Inout_ struct timespec* _lptimespec, 
	_In_    Long             _nanoseconds,
	_In_    Long             _nanopersec) {

	_lptimespec->tv_sec  += _nanoseconds / _nanopersec;
	_lptimespec->tv_nsec += _nanoseconds % _nanopersec;
	if (_lptimespec->tv_nsec >= _nanopersec) {		
		_lptimespec->tv_nsec -= _nanopersec;
		_lptimespec->tv_sec++;
	}
} 


Int32 
ZChrono_CompareTimespec(
	_In_ const struct timespec* _lptimespec0,
	_In_ const struct timespec* _lptimespec1) {

	if (_lptimespec0->tv_sec != _lptimespec1->tv_sec)
		return (Int32)(_lptimespec0->tv_sec - _lptimespec1->tv_sec);
	else if (_lptimespec0->tv_nsec != _lptimespec1->tv_nsec)
		return (Int32)(_lptimespec0->tv_nsec - _lptimespec1->tv_nsec);
	return 0;
}


struct tm* 
Zchrono_GetLocalTime(
	_Inout_ Int64*     _timep, 
	_Inout_ struct tm* _result) {

#if (Z_PLATFORM_WINDOWS)
    localtime_s(_result, _timep);
#else 
	localtime_r(_result, _timep);
#endif
    return _result;
} 


ZRESULT
ZChrono_GetTimeOfDay(
	_Inout_ struct timeval* _lptimevalue,
	_In_    Handle          _hdata) {

	ZRESULT zresult;
	zresult = Z_EFAIL;

	Z_Unused(_hdata);

#if (Z_PLATFORM_WINDOWS) 
	FILETIME       filetime;
	ULARGE_INTEGER lgInt;
	UINT64         t64;

	if (_lptimevalue != NULL) {
		GetSystemTimeAsFileTime(&filetime);
		lgInt.LowPart  = filetime.dwLowDateTime;
		lgInt.HighPart = filetime.dwHighDateTime;

		t64 = (lgInt.QuadPart - EPOCH_FILETIME) / 10;
		_lptimevalue->tv_sec = (Long)(t64 / 1000000);
		_lptimevalue->tv_usec = t64 % 1000000;
		zresult = Z_OK;
	}
#else
	zresult = gettimeofday(_lptimevalue, NULL);
	zresult = ZResult_FromErrno(zresult);
#endif
	return zresult;
}


Void 
ZChrono_GetTimeStamp(
	_In_    const struct timeval* _lptimevalue, 
	_Inout_ Char*                 _timestamp,
	_In_    SizeT                 _sizeInBytes) {

	Int64     seconds;
	struct tm calendar;

	seconds = _lptimevalue->tv_sec;  
	assert(_sizeInBytes >= 25);

	Zchrono_GetLocalTime(&seconds, &calendar);
	strftime(_timestamp, _sizeInBytes, "%y-%m-%d %H:%M:%S", &calendar);
	sprintf(&_timestamp[17], ".%06ld", (Long)_lptimevalue->tv_usec);
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/