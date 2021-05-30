/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zchrono.h 
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
#ifndef __ZCHRONO_H__
#define __ZCHRONO_H__
 
#include <time.h>
#include "zcore.h" 

#if (Z_PLATFORM_WINDOWS)
#  include "zplatform_win32.h" //for winsock2.h
#else
#  include "zplatform_posix.h" //for sys/time.h
#endif  
#if defined(__cplusplus)
extern "C" {
#endif



 
  
/*
utility object for measuring elapsed time*/
typedef struct _ZTimer ZTimer;

/*
a structure containing a time value which can be
represented as microseconds, milliseconds, or seconds*/
typedef struct {
	Int64 microseconds;
} ZTimeValue;





/*
Initialize the zchrono interface.
This function is required before using the ZTimer struct and
before the program terminates, a call to ZChrono_Release is required.*/
extern ZRESULT ZAPI
ZChrono_Init(Void);


/*
Close the zchrono interface and release resources.
This function is required before the program terminates.*/
extern Void ZAPI
ZChrono_Release(Void);


extern ZTimer* ZAPI
ZChrono_CreateTimer(Void);


extern Void
ZChrono_ReleaseTimer(
    _Inout_ ZTimer* _ztimer);


/*
Returns the time elapsed since calling ZChrono_RestartTimer()
@_lptimer: the timer object to measure
@return  : milliseconds elapsed*/
extern ZTimeValue ZAPI
ZChrono_GetTimerElapsed(
	_In_ const ZTimer* _ztimer);


/*
Restarts the timer object back to 0.
@_lptimer: the timer object to restart
@return  : time elapsed since the timer was started*/
extern ZTimeValue ZAPI
ZChrono_RestartTimer(
	_Inout_ ZTimer* _ztimer);


/*
Adds nanoseconds to a given timespec object.*/
extern Void ZAPI
ZChrono_AddNanosecsToTimespec(
	_Inout_ struct timespec* _lptimespec, 
	_In_    Long             _nanoseconds,
	_In_    Long             _nanopersec);
					       

/*
Compares the values of two timespec objects. 
@return: 0 if they are equal, else the difference in their values*/
extern Int32 ZAPI
ZChrono_CompareTimespec(
	_In_ const struct timespec* _lptimespec0,
	_In_ const struct timespec* _lptimespec1);


/*
Converts given time since epoch into calendar time. 
This function replaces the POSIX localtime_r function.

@_lptimer :	pointer to a Int64 to convert
@_lpbuffer:	pointer to a struct tm object to store the result
@return   : tm object on success, or null pointer otherwise*/
extern struct tm* ZAPI
Zchrono_GetLocalTime(
	_Inout_ Int64*     _lptimer, 
	_Inout_ struct tm* _lpbuffer);


/*
Gets the time as polled from the system.
@_lptimevalue: gives the number of seconds and microseconds since the Epoch
@_hdata      : timezone (obsolete, specify as NULL)
@return      : Z_OK on success, error code on failure*/
extern ZRESULT ZAPI
ZChrono_GetTimeOfDay(
	_Inout_ struct timeval* _lptimevalue,
	_In_    Handle          _hdata);


/*
Fills a string with the current time stamp. 
(Useful for logging as an example).
The string will be formatted as "%y-%m-%d %H:%M:%S"

@_lptimevalue : time object to get seconds from
@_timestampstr: the string to initialize 
@_sizeInBytes : the size of the string in bytes*/
extern Void ZAPI
ZChrono_GetTimeStamp(
	_In_    const struct timeval* _lptimevalue, 
	_Inout_ Char*                 _timestampstr,
	_In_    SizeT                 _sizeInBytes);




extern Float ZAPI 
ZChrono_GetTimeAsSeconds(
    _In_ const ZTimeValue* _ztimevalue);


extern Int32 ZAPI
ZChrono_GetTimeAsMilliseconds(
    _In_ const ZTimeValue* _ztimevalue);


extern Int64 ZAPI
ZChrono_GetTimeAsMicroseconds(
    _In_ const ZTimeValue* _ztimevalue);
 

extern Void ZAPI
ZChrono_SetTimeAsSeconds(
	_Inout_ ZTimeValue* _ztimevalue,
	_In_    Float       _seconds);


extern Void ZAPI
ZChrono_SetTimeAsMilliseconds(
	_Inout_ ZTimeValue* _ztimevalue,
	_In_    Int32       _milliseconds);

 
extern Void ZAPI
ZChrono_SetTimeAsMicroseconds(
	_Inout_ ZTimeValue* _ztimevalue,
	_In_    Int64       _microseconds);



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  