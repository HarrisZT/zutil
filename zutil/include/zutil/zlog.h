/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zlog.h 
* Desc: logging routines
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
#ifndef __ZLOG_H__
#define __ZLOG_H__
  
#include "zcore.h"  
#if defined(__cplusplus)
extern "C" {
#endif



/*
default max filesize for logger (1MB)*/
#define ZLOG_MAX_FILESIZE 1048576L

/*
enum flag specifying operation mode*/
typedef enum _ZLOGMODE {
	ZLOGMODE_UNKNOWN = 0x00, //zlog has not been initialized
	ZLOGMODE_CONSOLE = 0x01, //output to console only
	ZLOGMODE_FILEOUT = 0x02, //output to file only
	ZLOGMODE_DEFAULT =       //outputs to console and file
	ZLOGMODE_CONSOLE | ZLOGMODE_FILEOUT,
} ZLOGMODE; 

/*
enum specifying a logging priority level*/
typedef enum _ZLOGLEVEL { 
	ZLOGLEVEL_TRACE, 
	ZLOGLEVEL_DEBUG,
	ZLOGLEVEL_INFO, 
	ZLOGLEVEL_WARN, 
	ZLOGLEVEL_ERROR, 
	ZLOGLEVEL_FATAL,
	ZLOGLEVEL_UNDEFINED,
} ZLOGLEVEL; 





/*
Initialize the logging module. 
@_modeflag   : a flag to determine the mode which logger will operate in.
               ZLOG_MODE_CONSOLE                     -console mode only
			   ZLOG_MODE_FILEOUT                     -file mode only
			   ZLOG_MODE_CONSOLE | ZLOG_MODE_FILEOUT -console and file mode 
@_output     : this file pointer is relavent to console mode. By default,
               will be set to stdout but it can be set to stderr
@_filename   : the name for a file to be output by the logger (file mode only)
@_maxFileSize: max limit (bytes) to write to the output file (file mode only)
@_maxBackups : max limit on the the number of backup log files (file mode only)
@return      : Z_OK on success, error code on faiulure*/
extern ZRESULT ZAPI
ZLog_Init(
	_In_        ZLOGMODE _modeflag,
	_Inout_opt_ FILE*    _output,
	_In_opt_    Lpcstr   _filename,
	_In_        Long     _maxFileSize,
	_In_        Byte     _maxBackupFiles);


/*
Initialize the logging module with default settings.
This will specify the following parameters:
1. flag both console and file output,
2. output to console using stdout, 
3. an output file named "zlog.txt",
4. 1MB max filesize,
5. a single backup file*/
#define ZLog_InitDefault()\
  ZLog_Init(              \
    ZLOGMODE_DEFAULT,     \
    NULL,                 \
    "zlog.txt",           \
    ZLOG_MAX_FILESIZE,    \
    1)


/*
Close the logger and release resources.
This function must be called before program termineates*/
extern Void ZAPI
ZLog_Release(Void);


/*
Specify the priority level in which logger will operate in
@_loglevel: the priority level*/
extern Void ZAPI
ZLog_SetLevel(
	_In_ ZLOGLEVEL _loglevel);


/*
Reports the currently set priority level in which logger is operating in*/
extern ZLOGLEVEL ZAPI
ZLog_GetLevel(Void);


/*
Specify whether logger is enabled or disabled.
@_enable: 1 to enable, 0 to disable*/
extern Void ZAPI
ZLog_SetEnabled(
	_In_ Bool _enable);


/*
Reports whether logger is currently enabled or disabled.
@return: 1 if enabled, 0 if disabled*/
extern Bool ZAPI
ZLog_IsEnabled(Void);


/*
Reports whether the given priority level is high enough to 
warrant the logger to post output. (low priority messages wont be logged 
if the ZLOGLEVEL is currently set higher).
@_loglevel: the level to validate
@return   : 1 if level is enabled, 0 if disabled*/
extern Bool ZAPI
ZLog_LevelIsEnabled(
	_In_ ZLOGLEVEL _loglevel);


/*
Specify how often (if ever) the logger should autoflush its contents.
@_interval: the value of which is in milliseconds. 
            To disable autoflushing, set value to 0*/
extern Void ZAPI
ZLog_SetFlushInterval(
	_In_ Long _interval);


/* 
Manually flush the contents of the logger.*/
extern Void ZAPI
ZLog_FlushFile(Void);


/* 
Post a message to the logger. 
@_loglevel: the priority level
@_srcfile : a file name string
@_linenum : the line number
@_format  : string format for msg
@_...     : additional arguments */
extern Void ZAPI
ZLog_Output(
	_In_ ZLOGLEVEL _loglevel,
	_In_ Lpcstr    _file,
	_In_ Int32     _line,
	_In_ Lpcstr    _format, ...);
 




/*
Macro interface*/
//***************************************************************************// 
#define __ZLOG_FILENAME__ \
    (strrchr(__FILE__, Z_DIR_SEP) ? \
     strrchr(__FILE__, Z_DIR_SEP) + 1 : __FILE__)

#if (Z_BUILD_DEBUG)
#  define ZLogInfo(...)\
	ZLog_Output(ZLOGLEVEL_INFO, __ZLOG_FILENAME__, __LINE__, __VA_ARGS__)
#  define ZLogWarning(...)\
    ZLog_Output(ZLOGLEVEL_WARN, __ZLOG_FILENAME__, __LINE__, __VA_ARGS__)
#  define ZLogError(...)\
	ZLog_Output(ZLOGLEVEL_ERROR, __ZLOG_FILENAME__, __LINE__, __VA_ARGS__) 
#  define ZLogTrace(...)\
	ZLog_Output(ZLOGLEVEL_TRACE, __ZLOG_FILENAME__, __LINE__, __VA_ARGS__)
#  define ZLogFatal(...)\
    ZLog_Output(ZLOGLEVEL_FATAL, __ZLOG_FILENAME__, __LINE__, __VA_ARGS__)
#  define ZLogDebug(...)\
	ZLog_Output(ZLOGLEVEL_DEBUG, __ZLOG_FILENAME__, __LINE__, __VA_ARGS__) 
#else 
#  define ZLogInfo(...) 
#  define ZLogWarning(...) 
#  define ZLogError(...)  
#  define ZLogTrace(...) 
#  define ZLogFatal(...) 
#  define ZLogDebug(...)  
#endif



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  