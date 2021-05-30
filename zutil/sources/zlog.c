/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zlog.c 
* Desc: debug logging routines
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
#include "zutil/zthread.h" 
#include "zutil/zlog.h" 



 

/* Section 1:
** internal data*/
//***************************************************************************//
#define ZLOG_OUT_C_FORMAT "%c %s %ld %s:%d: "
#define ZLOG_MAX_FILENAME 255 

#define ZLog_ModeEnabled(flags, flag) \
    (flags & flag) == flag


static volatile Int32     s_modeflag      = ZLOGMODE_UNKNOWN;
static volatile ZLOGLEVEL s_loglevel      = ZLOGLEVEL_INFO;  
static volatile Bool      s_enabled       = Z_TRUE;
static volatile Bool      s_initialized   = Z_FALSE;  
static volatile Long      s_flushInterval = 1; //msec, 0 is auto flush off  
static Long               s_flushcounter[2];   //[0]-console [1]file
static FILE*              s_output[2];         //[0]-console [1]file
static Byte               s_maxBackupFiles;    //max number of backup files
static Long               s_filesize;          //currently set file size
static Long               s_maxfilesize;       //max file size limit
static ZMutex*            s_mutex;             //lock for logger  
static Char               s_filename[ZLOG_MAX_FILENAME + 1]; 


static ZRESULT
ZLog_InitMutex(Void) {
	if (!s_mutex || !s_initialized) {
		s_mutex = ZMutex_Create();
		if (s_mutex)
			s_initialized = Z_TRUE;
		else return Z_EFAIL;
	}
	return Z_OK;
} 





/* Section 2:
** log file management
******************************************************************************/
static Void 
ZLog_GetBackupFileName(
	_In_    Lpcstr _basename, 
	_Inout_ Char*  _backupname, 
	_In_    Byte   _index, 
	_In_    SizeT  _size) {

	Char indexname[5]; 
	assert(_size >= strlen(_basename) + sizeof(indexname)); 
	strncpy(_backupname, _basename, _size);

	if (_index > 0) {
		sprintf(indexname, ".%d", _index);
		strncat(_backupname, indexname, strlen(indexname));
	}
}


static Long 
ZLog_GetFileSize(
	_In_ Lpcstr _filename) {

	FILE* file;
	Long  size;

	file = fopen(_filename, "rb");
	if (file == NULL) {
		return 0;
	}
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fclose(file);
	return size;
}


static Bool 
ZLog_FileExists(
	_In_ Lpcstr _filename) {

	FILE* file;
	file = fopen(_filename, "r");
	if (file == NULL)
		return Z_FALSE;
	fclose(file);
	return Z_TRUE;
}


static Int32 
ZLog_SwapFile(Void) {  
	Int32 it;                         //file id iterator 
	Char  src[ZLOG_MAX_FILENAME + 5]; //backup filename 
	Char  dst[ZLOG_MAX_FILENAME + 5]; //name, with null character 	

	if (s_filesize < s_maxfilesize) {
		return s_output[1] != NULL;
	}
	fclose(s_output[1]);

	it = (Int32)s_maxBackupFiles;

	while (it > 0) {
		ZLog_GetBackupFileName(s_filename, src, it - 1, sizeof(src));
		ZLog_GetBackupFileName(s_filename, dst, it, sizeof(dst));

		if (ZLog_FileExists(dst)) {
			if (remove(dst) != 0) {
				fprintf(
					stderr, 
					"zlogger failed to remove file: `%s`\n",
					dst);
			}
		}
		if (ZLog_FileExists(src)) {
			if (rename(src, dst) != 0) {
				fprintf(
					stderr, 
					"zlogger failed to rename file: `%s` -> `%s`\n",
					src, dst);
			}
		}
		--it;
	}
	s_output[1] = fopen(s_filename, "a");
	if (s_output[1] == NULL) {
		fprintf(
			stderr, 
			"zlogger failed to open file: `%s`\n",
			s_filename);
		return 0;
	}
	s_filesize = ZLog_GetFileSize(s_filename);
	return 1;
}





/* Section 3:
** data output formatting
******************************************************************************/
typedef struct {
	Char  levelchar;
	Dword dwThreadId;
	Long  milliseconds;
	Char  timestamp[32];
} ZLogOutData;



static Void
ZLog_InitOutputData(
	_Inout_ ZLogOutData* _outdata,
	_In_    ZLOGLEVEL    _level) {

	struct timeval tNow;
	ZChrono_GetTimeOfDay(&tNow, NULL);
	_outdata->milliseconds = 
		tNow.tv_sec * 1000 + 
		tNow.tv_usec / 1000;
	 
	switch (_level) {
	case ZLOGLEVEL_TRACE: _outdata->levelchar = 'T'; break;
	case ZLOGLEVEL_DEBUG: _outdata->levelchar = 'D'; break;
	case ZLOGLEVEL_INFO:  _outdata->levelchar = 'I'; break;
	case ZLOGLEVEL_WARN:  _outdata->levelchar = 'W'; break;
	case ZLOGLEVEL_ERROR: _outdata->levelchar = 'E'; break;
	case ZLOGLEVEL_FATAL: _outdata->levelchar = 'F'; break;
	default:              _outdata->levelchar = ' '; break;
	} 
	ZChrono_GetTimeStamp(
		&tNow, 
		_outdata->timestamp, 
		sizeof(_outdata->timestamp));

	_outdata->dwThreadId = ZThread_CurrentId();
} 
 

static Long 
ZLog_PrintData( 
	_In_    Lpcstr       _filename,
	_In_    Int32        _linenumber,
	_In_    Lpcstr       _format,
	_In_    ZLogOutData* _outdata, 
	_In_    va_list      _vaList,
	_Inout_ FILE*        _lpFile,
	_Inout_ Long*        _flushcounter) { 
	 				     
	Int32 iResult;
	Long  sizeout; 

	sizeout = 0; 
	iResult = fprintf(
		_lpFile, 
		ZLOG_OUT_C_FORMAT,
		_outdata->levelchar, 
		_outdata->timestamp, 
		_outdata->dwThreadId, 
		_filename, 
		_linenumber);

	if (iResult > 0) {
		sizeout += iResult;
	}
	iResult = vfprintf(_lpFile, _format, _vaList);
	if (iResult > 0) {
		sizeout += iResult;
	}
	iResult = fprintf(_lpFile, "\n");
	if (iResult > 0) {
		sizeout += iResult;
	}
	if (s_flushInterval > 0) {
		if (_outdata->milliseconds - (*_flushcounter) > s_flushInterval) {
			fflush(_lpFile);
			*_flushcounter = _outdata->milliseconds;
		}
	}
	return sizeout;
}


#define ZLOG_PRINT(file, line, fmat, n) \
	va_start(args[n], fmat);            \
	ZLog_PrintData( 			        \
		file, 					        \
		line, 					        \
		fmat,				            \
		&outdata,				        \
		args[n],				        \
		s_output[n],			        \
		&s_flushcounter[n]); 	        \
	va_end(args[n]) 





/* Section 4:
** public interface functions
******************************************************************************/ 
ZRESULT
ZLog_Init(
	_In_        Int32  _modeflag,
	_Inout_opt_ FILE*  _output,
	_In_opt_    Lpcstr _filename,
	_In_        Long   _maxFileSize,
	_In_        Byte   _maxBackupFiles) { 

	/*
	Set up console mode if flag specifies it:*/
	if (ZLog_ModeEnabled(_modeflag, ZLOGMODE_CONSOLE)) {

		_output = (_output != NULL) ? _output : stdout;
		if (_output != stdout && _output != stderr) {
			assert(0 && "output must be stdout or stderr");
			return Z_EINVALIDARG;
		}
		ZRESULT zresult;
		zresult = ZLog_InitMutex();
		if (Z_FAILURE(zresult)) {
			return zresult;
		}
		ZMutex_Lock(s_mutex);
		s_output[0] = _output;
		s_modeflag |= ZLOGMODE_CONSOLE;
		ZMutex_Unlock(s_mutex); 
	} 
	/*
	Set up file mode:*/
	if (ZLog_ModeEnabled(_modeflag, ZLOGMODE_FILEOUT)) {

		if (_filename == NULL) {
			assert(0 && "filename cannot be null");
			return Z_EINVALIDARG;
		}
		if (strlen(_filename) > ZLOG_MAX_FILENAME) {
			assert(0 && 
				"filename exceeds the maximum number of characters");
			return Z_EINVALIDARG;
		}
		ZRESULT zresult;
		zresult = ZLog_InitMutex();
		if (Z_FAILURE(zresult)) {
			return zresult;
		}
		ZMutex_Lock(s_mutex);

		if (s_output[1] != NULL) { /* reinit */
			fclose(s_output[1]);
		}
		s_output[1] = fopen(_filename, "a");
		if (s_output[1] == NULL) {
			fprintf(
				stderr, 
				"zlogger failed to open file: `%s`\n", _filename);
		}
		else {
			s_filesize = ZLog_GetFileSize(_filename);
			strncpy(s_filename, _filename, sizeof(s_filename));
			s_maxBackupFiles = _maxBackupFiles;
			s_maxfilesize =
				(_maxFileSize > 0) ? _maxFileSize : ZLOG_MAX_FILESIZE;

			s_modeflag |= ZLOGMODE_FILEOUT; 
		}
		ZMutex_Unlock(s_mutex);
	}
	return Z_OK;
} 


Void
ZLog_Release(Void) {
	if (s_mutex) {
		ZMutex_Release(s_mutex);
		s_mutex = NULL; 
	}
	if (s_output[1])
		fclose(s_output[1]);

	s_initialized = Z_FALSE;
}


Void 
ZLog_SetLevel(
	_In_ ZLOGLEVEL _loglevel) {
	s_loglevel = _loglevel;
}


ZLOGLEVEL 
ZLog_GetLevel(Void) {
	return s_loglevel;
}


Void
ZLog_SetEnabled(
	_In_ Bool _enable) {
	s_enabled = _enable;
}


Bool 
ZLog_IsEnabled(Void) {
	return s_enabled;
}


Bool
ZLog_LevelIsEnabled(
	_In_ ZLOGLEVEL _loglevel) {
	return s_loglevel <= _loglevel;
}


Void 
ZLog_SetFlushInterval(
	_In_ Long _interval) {
	s_flushInterval = _interval > 0 ? _interval : 0;
} 


Void
ZLog_FlushFile(Void) {
	if (s_modeflag == ZLOGMODE_UNKNOWN || !s_initialized) {
		assert(ZLOGMODE_UNKNOWN && "logger is not initialized");
		return;
	}
	if (ZLog_ModeEnabled(s_modeflag, ZLOGMODE_CONSOLE)) {
		fflush(s_output[0]);
	}
	if (ZLog_ModeEnabled(s_modeflag, ZLOGMODE_FILEOUT)) {
		fflush(s_output[1]);
	}
} 


Void 
ZLog_Output(
	_In_ ZLOGLEVEL _level,
	_In_ Lpcstr    _file,
	_In_ Int32     _line,
	_In_ Lpcstr    _format, ...) { 
	
	ZLogOutData outdata;
	va_list     args[2]; //[0]console, [1]file	 
	 
	if (s_modeflag == ZLOGMODE_UNKNOWN || !s_initialized) {
		assert(ZLOGMODE_UNKNOWN && "logger is not initialized");
		return;
	} 
	if (!s_enabled || !ZLog_LevelIsEnabled(_level)) {
		return;
	} 
	ZMutex_Lock(s_mutex);
	ZLog_InitOutputData(&outdata, _level); 

	if (ZLog_ModeEnabled(s_modeflag, ZLOGMODE_CONSOLE)) {
		ZLOG_PRINT(_file, _line, _format, 0);
	}
	if (ZLog_ModeEnabled(s_modeflag, ZLOGMODE_FILEOUT)) {
		if (ZLog_SwapFile()) {
			ZLOG_PRINT(_file, _line, _format, 1);
		}
	}
	ZMutex_Unlock(s_mutex);
}
/*****************************************************************************/  
//EOF
/*****************************************************************************/ 