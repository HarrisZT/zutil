/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved. 
* Zlib license.
*
* File: zbasepath_linux.h 
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
#ifndef __ZBASEPATH_LINUX_H__
#define __ZBASEPATH_LINUX_H__

#include "zbasepath_common.h" 

#if defined(__linux__) || defined(__CYGWIN__) || defined(__sun)  
#  if defined(__linux__)
#    include <linux/limits.h> 
#  endif
#  ifndef __STDC_FORMAT_MACROS
#    define __STDC_FORMAT_MACROS
#  endif
#  include <inttypes.h> 
#  if defined(__ANDROID__) || defined(ANDROID)
#    include <fcntl.h>
#    include <sys/mman.h>
#    include <unistd.h>
#  endif
 
#  if defined(__sun)
#    define _PROC_SELF_EXE  "/proc/self/path/a.out"
#    define _PROC_SELF_MAPS "/proc/self/map"
#  else
#    define _PROC_SELF_EXE  "/proc/self/exe"
#    define _PROC_SELF_MAPS "/proc/self/maps"
#  endif 
#  define _PROC_SELF_MAPS_RETRY 5  
 
#define _SCAN_FORMAT \
  "%" PRIx64 "-%" PRIx64 " %s %" PRIx64 " %x:%x %u %s\n"
 
// local file header signature found
#define _LOCAL_HDR_SIG 0x04034B50UL

#ifdef __cplusplus
extern "C" {
#endif


	   
	static int Linux_GetExePath(
		char* _cPathOut,
		int   _iCapacity, 
		int*  _iDirnameLength) {

		char  cBuffer[PATH_MAX];
		char* cResolved = NULL;
		int   iLength = -1;

		for (;;) {
			cResolved = realpath(_PROC_SELF_EXE, cBuffer);
			if (!cResolved) {
				break;
			}
			iLength = (int)strlen(resolved);
			if (iLength <= _iCapacity) {
				memcpy(_cPathOut, cResolved, iLength);

				if (_iDirnameLength) {
					int i; 
					for (i = iLength - 1; i >= 0; --i) {
						if (_cPathOut[i] == '/') {
							*_iDirnameLength = i;
							break;
						}
					}
				}
			}
			break;
		}
		return iLength;
	}

 
	static int Linux_GetModPath(
		char* _cPathOut, 
		int   _iCapacity, 
		int*  _iDirnameLength) {

		int   iLength = -1;
		FILE* maps    = NULL;

		for (int r = 0; r < _PROC_SELF_MAPS_RETRY; ++r) {
			maps = fopen(_PROC_SELF_MAPS, "r");
			if (!maps) { 
				break;
			}
			for (;;) {
				char     perms[5];
				char     path[PATH_MAX];
				char     buffer[PATH_MAX < 1024 ? 1024 : PATH_MAX];
				uint64_t low, high; 
				uint64_t offset;
				uint32_t major, minor; 
				uint32_t inode;

				if (!fgets(buffer, sizeof(buffer), maps))
					break;

				if (sscanf(
					buffer, 
					_SCAN_FORMAT, 
					&low, &high,
					perms, 
					&offset, 
					&major, &minor, 
					&inode, 
					path) == 8) {

					uint64_t addr = (uintptr_t)F2D_ReturnAddress();
					if (low <= addr && addr <= high) {
						char* resolved; 
						resolved = realpath(path, buffer);
						if (!resolved) {
							break;
						}
						iLength = (int)strlen(resolved);

					#if defined(__ANDROID__) || defined(ANDROID)
						if (iLength > 4
							&& buffer[iLength - 1] == 'k'
							&& buffer[iLength - 2] == 'p'
							&& buffer[iLength - 3] == 'a'
							&& buffer[iLength - 4] == '.') {

							int fd = open(path, O_RDONLY);
							if (fd == -1) {
								iLength = -1; // retry
								break;
							} 
							char* begin = (char*)mmap(
								0, offset, 
								PROT_READ, MAP_SHARED, 
								fd, 0);

							if (begin == MAP_FAILED) {
								close(fd);
								iLength = -1; // retry
								break;
							} 
							// minimum size of local file header:
							char* p = begin + offset - 30; 

							while (p >= begin) {// scan backwards 
								if (*((uint32_t*)p) == _LOCAL_HDR_SIG) {
									uint16_t length_ = *((uint16_t*)(p + 26));

									if (iLength + 2 + length_ < 
										(int)sizeof(buffer)) {

										memcpy(
											&buffer[iLength], "!/", 2);
										memcpy(
											&buffer[iLength + 2], 
											p + 30, length_);

										iLength += 2 + length_;
									} 
									break;
								} 
								--p;
							} 
							munmap(begin, offset);
							close(fd);
						}
					#endif

						if (iLength <= _iCapacity) {
							memcpy(_cPathOut, resolved, iLength);

							if (_iDirnameLength) {
								int i; 
								for (i = iLength - 1; i >= 0; --i) {
									if (_cPathOut[i] == '/') {
										*_iDirnameLength = i;
										break;
									}
								}
							}
						} 
						break;
					}
				}
			}
			fclose(maps);
			maps = NULL; 

			if (iLength != -1) {
				break;
			}
		}
		if (maps) {
			fclose(maps);
		}
		return iLength;
	}


#ifdef __cplusplus
}
#endif 
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/