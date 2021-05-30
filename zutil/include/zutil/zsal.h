/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zsal.h 
* Desc: a cross-platform adaptation of Microsoft's sal.h
*       SAL = source-code annotation language
https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2015/code-quality/using-sal-annotations-to-reduce-c-cpp-code-defects?view=vs-2015
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
#ifndef __ZSAL_H__
#define __ZSAL_H__  
 
#include "zconfig.h"



#define __ZUTIL_HAS_SAL_H 0
/*
First try to include the real sal.h if we're using a
Microsoft C/C++ compiler. If we can't find it, continue on*/
#if (ZUTIL_COMPILER_MSVC)
#  include <sal.h> 
#  undef  __ZUTIL_HAS_SAL_H 
#  define __ZUTIL_HAS_SAL_H 1 
#elif defined(__has_include)
#  if __has_include(<sal.h>)
#    include <sal.h> 
#    undef  __ZUTIL_HAS_SAL_H 
#    define __ZUTIL_HAS_SAL_H 1
#  endif
#endif 

/*the real sal.h not found. 
continue on to define empty sal macros for other platforms*/
#if (!__ZUTIL_HAS_SAL_H)

#if defined(_In_) && defined(__zsal_defined_In)
#  undef _In_
#endif
#if !defined(_In_)
#  define _In_
#  define __zsal_defined_In
#endif

#if defined(_Out_) && defined(__zsal_defined_Out)
#  undef _Out_
#endif
#if !defined(_Out_)
#  define _Out_
#  define __zsal_defined_Out
#endif

#if defined(_Inout_) && defined(__zsal_defined_Inout)
#  undef _Inout_
#endif
#if !defined(_Inout_)
#  define _Inout_
#  define __zsal_defined_Inout
#endif

#if defined(_In_z_) && defined(__zsal_defined_In_z)
#  undef _In_z_
#endif
#if !defined(_In_z_)
#  define _In_z_
#  define __zsal_defined_In_z
#endif

#if defined(_Inout_z_) && defined(__zsal_defined_Inout_z)
#  undef _Inout_z_
#endif
#if !defined(_Inout_z_)
#  define _Inout_z_
#  define __zsal_defined_Inout_z
#endif

#if defined(_In_reads_) && defined(__zsal_defined_In_reads)
#  undef _In_reads_
#endif
#if !defined(_In_reads_)
#  define _In_reads_(s)
#  define __zsal_defined_In_reads
#endif

#if defined(_In_reads_bytes_) && defined(__zsal_defined_In_reads_bytes)
#  undef _In_reads_bytes_
#endif
#if !defined(_In_reads_bytes_)
#  define _In_reads_bytes_(s)
#  define __zsal_defined_In_reads_bytes
#endif

#if defined(_In_reads_z_) && defined(__zsal_defined_In_reads_z)
#  undef _In_reads_z_
#endif
#if !defined(_In_reads_z_)
#  define _In_reads_z_(s)
#  define __zsal_defined_In_reads_z
#endif

#if defined(_In_reads_or_z_) && defined(__zsal_defined_In_reads_or_z)
#  undef _In_reads_or_z_
#endif
#if !defined(_In_reads_or_z_)
#  define _In_reads_or_z_(s)
#  define __zsal_defined_In_reads_or_z
#endif

#if defined(_Out_writes_) && defined(__zsal_defined_Out_writes)
#  undef _Out_writes_
#endif
#if !defined(_Out_writes_)
#  define _Out_writes_(s)
#  define __zsal_defined_Out_writes
#endif

#if defined(_Out_writes_bytes_) && defined(__zsal_defined_Out_writes_bytes)
#  undef _Out_writes_bytes_
#endif
#if !defined(_Out_writes_bytes_)
#  define _Out_writes_bytes_(s)
#  define __zsal_defined_Out_writes_bytes
#endif

#if defined(_Out_writes_z_) && defined(__zsal_defined_Out_writes_z)
#  undef _Out_writes_z_
#endif
#if !defined(_Out_writes_z_)
#  define _Out_writes_z_(s)
#  define __zsal_defined_Out_writes_z
#endif

#if defined(_Inout_updates_) && defined(__zsal_defined_Inout_updates)
#  undef _Inout_updates_
#endif
#if !defined(_Inout_updates_)
#  define _Inout_updates_(s)
#  define __zsal_defined_Inout_updates
#endif

#if defined(_Inout_updates_bytes_) && defined(__zsal_defined_Inout_updates_bytes)
#  undef _Inout_updates_bytes_
#endif
#if !defined(_Inout_updates_bytes_)
#  define _Inout_updates_bytes_(s)
#  define __zsal_defined_Inout_updates_bytes
#endif

#if defined(_Inout_updates_z_) && defined(__zsal_defined_Inout_updates_z)
#  undef _Inout_updates_z_
#endif
#if !defined(_Inout_updates_z_)
#  define _Inout_updates_z_(s)
#  define __zsal_defined_Inout_updates_z
#endif

#if defined(_Out_writes_to_) && defined(__zsal_defined_Out_writes_to)
#  undef _Out_writes_to_
#endif
#if !defined(_Out_writes_to_)
#  define _Out_writes_to_(s,c)
#  define __zsal_defined_Out_writes_to
#endif

#if defined(_Out_writes_bytes_to_) && defined(__zsal_defined_Out_writes_bytes_to)
#  undef _Out_writes_bytes_to_
#endif
#if !defined(_Out_writes_bytes_to_)
#  define _Out_writes_bytes_to_(s,c)
#  define __zsal_defined_Out_writes_bytes_to
#endif

#if defined(_Out_writes_all_) && defined(__zsal_defined_Out_writes_all)
#  undef _Out_writes_all_
#endif
#if !defined(_Out_writes_all_)
#  define _Out_writes_all_(s)
#  define __zsal_defined_Out_writes_all
#endif

#if defined(_Out_writes_bytes_all_) && defined(__zsal_defined_Out_writes_bytes_all)
#  undef _Out_writes_bytes_all_
#endif
#if !defined(_Out_writes_bytes_all_)
#  define _Out_writes_bytes_all_(s)
#  define __zsal_defined_Out_writes_bytes_all
#endif

#if defined(_In_updates_to_) && defined(__zsal_defined_In_updates_to)
#  undef _In_updates_to_
#endif
#if !defined(_In_updates_to_)
#  define _In_updates_to_(s,c)
#  define __zsal_defined_In_updates_to
#endif

#if defined(_In_updates_bytes_to_) && defined(__zsal_defined_In_updates_bytes_to)
#  undef _In_updates_bytes_to_
#endif
#if !defined(_In_updates_bytes_to_)
#  define _In_updates_bytes_to_(s,c)
#  define __zsal_defined_In_updates_bytes_to
#endif

#if defined(_Inout_updates_z_) && defined(__zsal_defined_Inout_updates_z)
#  undef _Inout_updates_z_
#endif
#if !defined(_Inout_updates_z_)
#  define _Inout_updates_z_(s)
#  define __zsal_defined_Inout_updates_z
#endif

#if defined(_Out_writes_to_) && defined(__zsal_defined_Out_writes_to)
#  undef _Out_writes_to_
#endif
#if !defined(_Out_writes_to_)
#  define _Out_writes_to_(s,c)
#  define __zsal_defined_Out_writes_to
#endif

#if defined(_Out_writes_bytes_to_) && defined(__zsal_defined_Out_writes_bytes_to)
#  undef _Out_writes_bytes_to_
#endif
#if !defined(_Out_writes_bytes_to_)
#  define _Out_writes_bytes_to_(s,c)
#  define __zsal_defined_Out_writes_bytes_to
#endif

#if defined(_Out_writes_all_) && defined(__zsal_defined_Out_writes_all)
#  undef _Out_writes_all_
#endif
#if !defined(_Out_writes_all_)
#  define _Out_writes_all_(s)
#  define __zsal_defined_Out_writes_all
#endif

#if defined(_Out_writes_bytes_all_) && defined(__zsal_defined_Out_writes_bytes_all)
#  undef _Out_writes_bytes_all_
#endif
#if !defined(_Out_writes_bytes_all_)
#  define _Out_writes_bytes_all_(s)
#  define __zsal_defined_Out_writes_bytes_all
#endif

#if defined(_Inout_updates_to_) && defined(__zsal_defined_Inout_updates_to)
#  undef _Inout_updates_to_
#endif
#if !defined(_Inout_updates_to_)
#  define _Inout_updates_to_(s,c)
#  define __zsal_defined_Inout_updates_to
#endif

#if defined(_Inout_updates_bytes_to_) && defined(__zsal_defined_Inout_updates_bytes_to)
#  undef _Inout_updates_bytes_to_
#endif
#if !defined(_Inout_updates_bytes_to_)
#  define _Inout_updates_bytes_to_(s,c)
#  define __zsal_defined_Inout_updates_bytes_to
#endif

#if defined(_Inout_updates_all_) && defined(__zsal_defined_Inout_updates_all)
#  undef _Inout_updates_all_
#endif
#if !defined(_Inout_updates_all_)
#  define _Inout_updates_all_(s)
#  define __zsal_defined_Inout_updates_all
#endif

#if defined(_Inout_updates_bytes_all_) && defined(__zsal_defined_Inout_updates_bytes_all)
#  undef _Inout_updates_bytes_all_
#endif
#if !defined(_Inout_updates_bytes_all_)
#  define _Inout_updates_bytes_all_(s)
#  define __zsal_defined_Inout_updates_bytes_all
#endif

#if defined(_In_reads_to_ptr_) && defined(__zsal_defined_In_reads_to_ptr)
#  undef _In_reads_to_ptr_
#endif
#if !defined(_In_reads_to_ptr_)
#  define _In_reads_to_ptr_(p)
#  define __zsal_defined_In_reads_to_ptr
#endif

#if defined(_In_reads_to_ptr_z_) && defined(__zsal_defined_In_reads_to_ptr_z)
#  undef _In_reads_to_ptr_z_
#endif
#if !defined(_In_reads_to_ptr_z_)
#  define _In_reads_to_ptr_z_(p)
#  define __zsal_defined_In_reads_to_ptr_z
#endif

#if defined(_Out_writes_to_ptr_) && defined(__zsal_defined_Out_writes_to_ptr)
#  undef _Out_writes_to_ptr_
#endif
#if !defined(_Out_writes_to_ptr_)
#  define _Out_writes_to_ptr_(p)
#  define __zsal_defined_Out_writes_to_ptr
#endif

#if defined(_Out_writes_to_ptr_z_) && defined(__zsal_defined_Out_writes_to_ptr_z)
#  undef _Out_writes_to_ptr_z_
#endif
#if !defined(_Out_writes_to_ptr_z_)
#  define _Out_writes_to_ptr_z_(p)
#  define __zsal_defined_Out_writes_to_ptr_z
#endif 

#if defined(_In_opt_) && defined(__zsal_defined_In_opt)
#  undef _In_opt_
#endif
#if !defined(_In_opt_)
#  define _In_opt_
#  define __zsal_defined_In_opt
#endif

#if defined(_Out_opt_) && defined(__zsal_defined_Out_opt)
#  undef _Out_opt_
#endif
#if !defined(_Out_opt_)
#  define _Out_opt_
#  define __zsal_defined_Out_opt
#endif

#if defined(_Inout_opt_) && defined(__zsal_defined_Inout_opt)
#  undef _Inout_opt_
#endif
#if !defined(_Inout_opt_)
#  define _Inout_opt_
#  define __zsal_defined_Inout_opt
#endif

#if defined(_In_opt_z_) && defined(__zsal_defined_In_opt_z)
#  undef _In_opt_z_
#endif
#if !defined(_In_opt_z_)
#  define _In_opt_z_
#  define __zsal_defined_In_opt_z
#endif

#if defined(_Inout_opt_z_) && defined(__zsal_defined_Inout_opt_z)
#  undef _Inout_opt_z_
#endif
#if !defined(_Inout_opt_z_)
#  define _Inout_opt_z_
#  define __zsal_defined_Inout_opt_z
#endif

#if defined(_In_reads_opt_) && defined(__zsal_defined_In_reads_opt)
#  undef _In_reads_opt_
#endif
#if !defined(_In_reads_opt_)
#  define _In_reads_opt_(s)
#  define __zsal_defined_In_reads_opt
#endif

#if defined(_In_reads_bytes_opt_) && defined(__zsal_defined_In_reads_bytes_opt)
#  undef _In_reads_bytes_opt_
#endif
#if !defined(_In_reads_bytes_opt_)
#  define _In_reads_bytes_opt_(s)
#  define __zsal_defined_In_reads_bytes_opt
#endif

#if defined(_In_reads_opt_z_) && defined(__zsal_defined_In_reads_opt_z)
#  undef _In_reads_opt_z_
#endif
#if !defined(_In_reads_opt_z_)
#  define _In_reads_opt_z_(s)
#  define __zsal_defined_In_reads_opt_z
#endif

#if defined(_Out_writes_opt_) && defined(__zsal_defined_Out_writes_opt)
#  undef _Out_writes_opt_
#endif
#if !defined(_Out_writes_opt_)
#  define _Out_writes_opt_(s)
#  define __zsal_defined_Out_writes_opt
#endif

#if defined(_Out_writes_bytes_) && defined(__zsal_defined_Out_writes_bytes)
#  undef _Out_writes_bytes_
#endif
#if !defined(_Out_writes_bytes_)
#  define _Out_writes_bytes_(s)
#  define __zsal_defined_Out_writes_bytes
#endif

#if defined(_Out_writes_opt_z_) && defined(__zsal_defined_Out_writes_opt_z)
#  undef _Out_writes_opt_z_
#endif
#if !defined(_Out_writes_opt_z_)
#  define _Out_writes_opt_z_(s)
#  define __zsal_defined_Out_writes_opt_z
#endif

#if defined(_Inout_updates_opt_) && defined(__zsal_defined_Inout_updates_opt)
#  undef _Inout_updates_opt_
#endif
#if !defined(_Inout_updates_opt_)
#  define _Inout_updates_opt_(s)
#  define __zsal_defined_Inout_updates_opt
#endif

#if defined(_Inout_updates_bytes_opt_) && defined(__zsal_defined_Inout_updates_bytes_opt)
#  undef _Inout_updates_bytes_opt_
#endif
#if !defined(_Inout_updates_bytes_opt_)
#  define _Inout_updates_bytes_opt_(s)
#  define __zsal_defined_Inout_updates_bytes_opt
#endif

#if defined(_Inout_updates_opt_z_) && defined(__zsal_defined_Inout_updates_opt_z)
#  undef _Inout_updates_opt_z_
#endif
#if !defined(_Inout_updates_opt_z_)
#  define _Inout_updates_opt_z_(s)
#  define __zsal_defined_Inout_updates_opt_z
#endif

#if defined(_Out_writes_to_opt_) && defined(__zsal_defined_Out_writes_to_opt)
#  undef _Out_writes_to_opt_
#endif
#if !defined(_Out_writes_to_opt_)
#  define _Out_writes_to_opt_(s,c)
#  define __zsal_defined_Out_writes_to_opt
#endif

#if defined(_Out_writes_bytes_to_opt_) && defined(__zsal_defined_Out_writes_bytes_to_opt)
#  undef _Out_writes_bytes_to_opt_
#endif
#if !defined(_Out_writes_bytes_to_opt_)
#  define _Out_writes_bytes_to_opt_(s,c)
#  define __zsal_defined_Out_writes_bytes_to_opt
#endif

#if defined(_Out_writes_all_opt_) && defined(__zsal_defined_Out_writes_all_opt)
#  undef _Out_writes_all_opt_
#endif
#if !defined(_Out_writes_all_opt_)
#  define _Out_writes_all_opt_(s)
#  define __zsal_defined_Out_writes_all_opt
#endif

#if defined(_Out_writes_bytes_all_opt_) && defined(__zsal_defined_Out_writes_bytes_all_opt)
#  undef _Out_writes_bytes_all_opt_
#endif
#if !defined(_Out_writes_bytes_all_opt_)
#  define _Out_writes_bytes_all_opt_(s)
#  define __zsal_defined_Out_writes_bytes_all_opt
#endif

#if defined(_In_updates_to_opt_) && defined(__zsal_defined_In_updates_to_opt)
#  undef _In_updates_to_opt_
#endif
#if !defined(_In_updates_to_opt_)
#  define _In_updates_to_opt_(s,c)
#  define __zsal_defined_In_updates_to_opt
#endif

#if defined(_In_updates_bytes_to_opt_) && defined(__zsal_defined_In_updates_bytes_to_opt)
#  undef _In_updates_bytes_to_opt_
#endif
#if !defined(_In_updates_bytes_to_opt_)
#  define _In_updates_bytes_to_opt_(s,c)
#  define __zsal_defined_In_updates_bytes_to_opt
#endif

#if defined(_Inout_updates_all_opt_) && defined(__zsal_defined_Inout_updates_all_opt)
#  undef _Inout_updates_all_opt_
#endif
#if !defined(_Inout_updates_all_opt_)
#  define _Inout_updates_all_opt_(s)
#  define __zsal_defined_Inout_updates_all_opt
#endif

#if defined(_Inout_updates_bytes_all_opt_) && defined(__zsal_defined_Inout_updates_bytes_all_opt)
#  undef _Inout_updates_bytes_all_opt_
#endif
#if !defined(_Inout_updates_bytes_all_opt_)
#  define _Inout_updates_bytes_all_opt_(s)
#  define __zsal_defined_Inout_updates_bytes_all_opt
#endif

#if defined(_In_reads_to_ptr_opt_) && defined(__zsal_defined_In_reads_to_ptr_opt)
#  undef _In_reads_to_ptr_opt_
#endif
#if !defined(_In_reads_to_ptr_opt_)
#  define _In_reads_to_ptr_opt_(p)
#  define __zsal_defined_In_reads_to_ptr_opt
#endif

#if defined(_In_reads_to_ptr_opt_z_) && defined(__zsal_defined_In_reads_to_ptr_opt_z)
#  undef _In_reads_to_ptr_opt_z_
#endif
#if !defined(_In_reads_to_ptr_opt_z_)
#  define _In_reads_to_ptr_opt_z_(p)
#  define __zsal_defined_In_reads_to_ptr_opt_z
#endif

#if defined(_Out_writes_to_ptr_opt_) && defined(__zsal_defined_Out_writes_to_ptr_opt)
#  undef _Out_writes_to_ptr_opt_
#endif
#if !defined(_Out_writes_to_ptr_opt_)
#  define _Out_writes_to_ptr_opt_(p)
#  define __zsal_defined_Out_writes_to_ptr_opt
#endif

#if defined(_Out_writes_to_ptr_opt_z_) && defined(__zsal_defined_Out_writes_to_ptr_opt_z)
#  undef _Out_writes_to_ptr_opt_z_
#endif
#if !defined(_Out_writes_to_ptr_opt_z_)
#  define _Out_writes_to_ptr_opt_z_(p)
#  define __zsal_defined_Out_writes_to_ptr_opt_z
#endif 

#if defined(_Outptr_) && defined(__zsal_defined_Outptr)
#  undef _Outptr_
#endif
#if !defined(_Outptr_)
#  define _Outptr_
#  define __zsal_defined_Outptr
#endif

#if defined(_Outptr_opt_) && defined(__zsal_defined_Outptr_opt)
#  undef _Outptr_opt_
#endif
#if !defined(_Outptr_opt_)
#  define _Outptr_opt_
#  define __zsal_defined_Outptr_opt
#endif

#if defined(_Outptr_result_maybenull_) && defined(__zsal_defined_Outptr_result_maybenull)
#  undef _Outptr_result_maybenull_
#endif
#if !defined(_Outptr_result_maybenull_)
#  define _Outptr_result_maybenull_
#  define __zsal_defined_Outptr_result_maybenull
#endif

#if defined(_Outptr_opt_result_maybenull_) && defined(__zsal_defined_Outptr_opt_result_maybenull)
#  undef _Outptr_opt_result_maybenull_
#endif
#if !defined(_Outptr_opt_result_maybenull_)
#  define _Outptr_opt_result_maybenull_
#  define __zsal_defined_Outptr_opt_result_maybenull
#endif

#if defined(_Outptr_result_z_) && defined(__zsal_defined_Outptr_result_z)
#  undef _Outptr_result_z_
#endif
#if !defined(_Outptr_result_z_)
#  define _Outptr_result_z_
#  define __zsal_defined_Outptr_result_z
#endif

#if defined(_Outptr_opt_result_z_) && defined(__zsal_defined_Outptr_opt_result_z)
#  undef _Outptr_opt_result_z_
#endif
#if !defined(_Outptr_opt_result_z_)
#  define _Outptr_opt_result_z_
#  define __zsal_defined_Outptr_opt_result_z
#endif

#if defined(_Outptr_result_maybenull_z_) && defined(__zsal_defined_Outptr_result_maybenull_z)
#  undef _Outptr_result_maybenull_z_
#endif
#if !defined(_Outptr_result_maybenull_z_)
#  define _Outptr_result_maybenull_z_
#  define __zsal_defined_Outptr_result_maybenull_z
#endif

#if defined(_Outptr_opt_result_maybenull_z_) && defined(__zsal_defined_Outptr_opt_result_maybenull_z)
#  undef _Outptr_opt_result_maybenull_z_
#endif
#if !defined(_Outptr_opt_result_maybenull_z_)
#  define _Outptr_opt_result_maybenull_z_
#  define __zsal_defined_Outptr_opt_result_maybenull_z
#endif

#if defined(_COM_Outptr_) && defined(__zsal_defined_COM_Outptr)
#  undef _COM_Outptr_
#endif
#if !defined(_COM_Outptr_)
#  define _COM_Outptr_
#  define __zsal_defined_COM_Outptr
#endif

#if defined(_COM_Outptr_opt_) && defined(__zsal_defined_COM_Outptr_opt)
#  undef _COM_Outptr_opt_
#endif
#if !defined(_COM_Outptr_opt_)
#  define _COM_Outptr_opt_
#  define __zsal_defined_COM_Outptr_opt
#endif

#if defined(_COM_Outptr_result_maybenull_) && defined(__zsal_defined_COM_Outptr_result_maybenull)
#  undef _COM_Outptr_result_maybenull_
#endif
#if !defined(_COM_Outptr_result_maybenull_)
#  define _COM_Outptr_result_maybenull_
#  define __zsal_defined_COM_Outptr_result_maybenull
#endif

#if defined(_Outptr_opt_result_maybenull_) && defined(__zsal_defined_Outptr_opt_result_maybenull)
#  undef _Outptr_opt_result_maybenull_
#endif
#if !defined(_Outptr_opt_result_maybenull_)
#  define _Outptr_opt_result_maybenull_
#  define __zsal_defined_Outptr_opt_result_maybenull
#endif

#if defined(_Outptr_result_buffer_) && defined(__zsal_defined_Outptr_result_buffer)
#  undef _Outptr_result_buffer_
#endif
#if !defined(_Outptr_result_buffer_)
#  define _Outptr_result_buffer_(s)
#  define __zsal_defined_Outptr_result_buffer
#endif

#if defined(_Outptr_result_bytebuffer_) && defined(__zsal_defined_Outptr_result_bytebuffer)
#  undef _Outptr_result_bytebuffer_
#endif
#if !defined(_Outptr_result_bytebuffer_)
#  define _Outptr_result_bytebuffer_(s)
#  define __zsal_defined_Outptr_result_bytebuffer
#endif

#if defined(_Outptr_opt_result_buffer_) && defined(__zsal_defined_Outptr_opt_result_buffer)
#  undef _Outptr_opt_result_buffer_
#endif
#if !defined(_Outptr_opt_result_buffer_)
#  define _Outptr_opt_result_buffer_(s)
#  define __zsal_defined_Outptr_opt_result_buffer
#endif

#if defined(_Outptr_opt_result_bytebuffer_) && defined(__zsal_defined_Outptr_opt_result_bytebuffer)
#  undef _Outptr_opt_result_bytebuffer_
#endif
#if !defined(_Outptr_opt_result_bytebuffer_)
#  define _Outptr_opt_result_bytebuffer_(s)
#  define __zsal_defined_Outptr_opt_result_bytebuffer
#endif

#if defined(_Outptr_result_buffer_to_) && defined(__zsal_defined_Outptr_result_buffer_to)
#  undef _Outptr_result_buffer_to_
#endif
#if !defined(_Outptr_result_buffer_to_)
#  define _Outptr_result_buffer_to_(s,c)
#  define __zsal_defined_Outptr_result_buffer_to
#endif

#if defined(_Outptr_result_bytebuffer_to_) && defined(__zsal_defined_Outptr_result_bytebuffer_to)
#  undef _Outptr_result_bytebuffer_to_
#endif
#if !defined(_Outptr_result_bytebuffer_to_)
#  define _Outptr_result_bytebuffer_to_(s,c)
#  define __zsal_defined_Outptr_result_bytebuffer_to
#endif

#if defined(_Outptr_opt_result_buffer_to_) && defined(__zsal_defined_Outptr_opt_result_buffer_to)
#  undef _Outptr_opt_result_buffer_to_
#endif
#if !defined(_Outptr_opt_result_buffer_to_)
#  define _Outptr_opt_result_buffer_to_(s,c)
#  define __zsal_defined_Outptr_opt_result_buffer_to
#endif

#if defined(_Outptr_opt_result_bytebuffer_to_) && defined(__zsal_defined_Outptr_opt_result_bytebuffer_to)
#  undef _Outptr_opt_result_bytebuffer_to_
#endif
#if !defined(_Outptr_opt_result_bytebuffer_to_)
#  define _Outptr_opt_result_bytebuffer_to_(s,c)
#  define __zsal_defined_Outptr_opt_result_bytebuffer_to
#endif

#if defined(_Result_nullonfailure_) && defined(__zsal_defined_Result_nullonfailure)
#  undef _Result_nullonfailure_
#endif
#if !defined(_Result_nullonfailure_)
#  define _Result_nullonfailure_
#  define __zsal_defined_Result_nullonfailure
#endif

#if defined(_Result_zeroonfailure_) && defined(__zsal_defined_Result_zeroonfailure)
#  undef _Result_zeroonfailure_
#endif
#if !defined(_Result_zeroonfailure_)
#  define _Result_zeroonfailure_
#  define __zsal_defined_Result_zeroonfailure
#endif

#if defined(_Outptr_result_nullonfailure_) && defined(__zsal_defined_Outptr_result_nullonfailure)
#  undef _Outptr_result_nullonfailure_
#endif
#if !defined(_Outptr_result_nullonfailure_)
#  define _Outptr_result_nullonfailure_
#  define __zsal_defined_Outptr_result_nullonfailure
#endif

#if defined(_Outptr_opt_result_nullonfailure_) && defined(__zsal_defined_Outptr_opt_result_nullonfailure)
#  undef _Outptr_opt_result_nullonfailure_
#endif
#if !defined(_Outptr_opt_result_nullonfailure_)
#  define _Outptr_opt_result_nullonfailure_
#  define __zsal_defined_Outptr_opt_result_nullonfailure
#endif

#if defined(_Outref_result_nullonfailure_) && defined(__zsal_defined_Outref_result_nullonfailure)
#  undef _Outref_result_nullonfailure_
#endif
#if !defined(_Outref_result_nullonfailure_)
#  define _Outref_result_nullonfailure_
#  define __zsal_defined_Outref_result_nullonfailure
#endif 

#if defined(_Outref_) && defined(__zsal_defined_Outref)
#  undef _Outref_
#endif
#if !defined(_Outref_)
#  define _Outref_
#  define __zsal_defined_Outref
#endif

#if defined(_Outref_result_maybenull_) && defined(__zsal_defined_Outref_result_maybenull)
#  undef _Outref_result_maybenull_
#endif
#if !defined(_Outref_result_maybenull_)
#  define _Outref_result_maybenull_
#  define __zsal_defined_Outref_result_maybenull
#endif

#if defined(_Outref_result_buffer_) && defined(__zsal_defined_Outref_result_buffer)
#  undef _Outref_result_buffer_
#endif
#if !defined(_Outref_result_buffer_)
#  define _Outref_result_buffer_(s)
#  define __zsal_defined_Outref_result_buffer
#endif

#if defined(_Outref_result_bytebuffer_) && defined(__zsal_defined_Outref_result_bytebuffer)
#  undef _Outref_result_bytebuffer_
#endif
#if !defined(_Outref_result_bytebuffer_)
#  define _Outref_result_bytebuffer_(s)
#  define __zsal_defined_Outref_result_bytebuffer
#endif

#if defined(_Outref_result_buffer_to_) && defined(__zsal_defined_Outref_result_buffer_to)
#  undef _Outref_result_buffer_to_
#endif
#if !defined(_Outref_result_buffer_to_)
#  define _Outref_result_buffer_to_(s,c)
#  define __zsal_defined_Outref_result_buffer_to
#endif

#if defined(_Outref_result_bytebuffer_to_) && defined(__zsal_defined_Outref_result_bytebuffer_to)
#  undef _Outref_result_bytebuffer_to_
#endif
#if !defined(_Outref_result_bytebuffer_to_)
#  define _Outref_result_bytebuffer_to_(s,c)
#  define __zsal_defined_Outref_result_bytebuffer_to
#endif

#if defined(_Outref_result_buffer_all_) && defined(__zsal_defined_Outref_result_buffer_all)
#  undef _Outref_result_buffer_all_
#endif
#if !defined(_Outref_result_buffer_all_)
#  define _Outref_result_buffer_all_(s)
#  define __zsal_defined_Outref_result_buffer_all
#endif

#if defined(_Outref_result_bytebuffer_all_) && defined(__zsal_defined_Outref_result_bytebuffer_all)
#  undef _Outref_result_bytebuffer_all_
#endif
#if !defined(_Outref_result_bytebuffer_all_)
#  define _Outref_result_bytebuffer_all_(s)
#  define __zsal_defined_Outref_result_bytebuffer_all
#endif

#if defined(_Outref_result_buffer_maybenull_) && defined(__zsal_defined_Outref_result_buffer_maybenull)
#  undef _Outref_result_buffer_maybenull_
#endif
#if !defined(_Outref_result_buffer_maybenull_)
#  define _Outref_result_buffer_maybenull_(s)
#  define __zsal_defined_Outref_result_buffer_maybenull
#endif

#if defined(_Outref_result_bytebuffer_maybenull_) && defined(__zsal_defined_Outref_result_bytebuffer_maybenull)
#  undef _Outref_result_bytebuffer_maybenull_
#endif
#if !defined(_Outref_result_bytebuffer_maybenull_)
#  define _Outref_result_bytebuffer_maybenull_(s)
#  define __zsal_defined_Outref_result_bytebuffer_maybenull
#endif

#if defined(_Outref_result_buffer_to_maybenull_) && defined(__zsal_defined_Outref_result_buffer_to_maybenull)
#  undef _Outref_result_buffer_to_maybenull_
#endif
#if !defined(_Outref_result_buffer_to_maybenull_)
#  define _Outref_result_buffer_to_maybenull_(s,c)
#  define __zsal_defined_Outref_result_buffer_to_maybenull
#endif

#if defined(_Outref_result_bytebuffer_to_maybenull_) && defined(__zsal_defined_Outref_result_bytebuffer_to_maybenull)
#  undef _Outref_result_bytebuffer_to_maybenull_
#endif
#if !defined(_Outref_result_bytebuffer_to_maybenull_)
#  define _Outref_result_bytebuffer_to_maybenull_(s,c)
#  define __zsal_defined_Outref_result_bytebuffer_to_maybenull
#endif

#if defined(_Outref_result_buffer_all_maybenull_) && defined(__zsal_defined_Outref_result_buffer_all_maybenull)
#  undef _Outref_result_buffer_all_maybenull_
#endif
#if !defined(_Outref_result_buffer_all_maybenull_)
#  define _Outref_result_buffer_all_maybenull_(s)
#  define __zsal_defined_Outref_result_buffer_all_maybenull
#endif

#if defined(_Outref_result_bytebuffer_all_maybenull_) && defined(__zsal_defined_Outref_result_bytebuffer_all_maybenull)
#  undef _Outref_result_bytebuffer_all_maybenull_
#endif
#if !defined(_Outref_result_bytebuffer_all_maybenull_)
#  define _Outref_result_bytebuffer_all_maybenull_(s)
#  define __zsal_defined_Outref_result_bytebuffer_all_maybenull
#endif 

#if defined(_Ret_z_) && defined(__zsal_defined_Ret_z)
#  undef _Ret_z_
#endif
#if !defined(_Ret_z_)
#  define _Ret_z_
#  define __zsal_defined_Ret_z
#endif

#if defined(_Ret_writes_) && defined(__zsal_defined_Ret_writes)
#  undef _Ret_writes_
#endif
#if !defined(_Ret_writes_)
#  define _Ret_writes_(s)
#  define __zsal_defined_Ret_writes
#endif

#if defined(_Ret_writes_bytes_) && defined(__zsal_defined_Ret_writes_bytes)
#  undef _Ret_writes_bytes_
#endif
#if !defined(_Ret_writes_bytes_)
#  define _Ret_writes_bytes_(s)
#  define __zsal_defined_Ret_writes_bytes
#endif

#if defined(_Ret_writes_z_) && defined(__zsal_defined_Ret_writes_z)
#  undef _Ret_writes_z_
#endif
#if !defined(_Ret_writes_z_)
#  define _Ret_writes_z_(s)
#  define __zsal_defined_Ret_writes_z
#endif

#if defined(_Ret_writes_to_) && defined(__zsal_defined_Ret_writes_to)
#  undef _Ret_writes_to_
#endif
#if !defined(_Ret_writes_to_)
#  define _Ret_writes_to_(s,c)
#  define __zsal_defined_Ret_writes_to
#endif

#if defined(_Ret_writes_maybenull_) && defined(__zsal_defined_Ret_writes_maybenull)
#  undef _Ret_writes_maybenull_
#endif
#if !defined(_Ret_writes_maybenull_)
#  define _Ret_writes_maybenull_(s)
#  define __zsal_defined_Ret_writes_maybenull
#endif

#if defined(_Ret_writes_to_maybenull_) && defined(__zsal_defined_Ret_writes_to_maybenull)
#  undef _Ret_writes_to_maybenull_
#endif
#if !defined(_Ret_writes_to_maybenull_)
#  define _Ret_writes_to_maybenull_(s)
#  define __zsal_defined_Ret_writes_to_maybenull
#endif

#if defined(_Ret_writes_maybenull_z_) && defined(__zsal_defined_Ret_writes_maybenull_z)
#  undef _Ret_writes_maybenull_z_
#endif
#if !defined(_Ret_writes_maybenull_z_)
#  define _Ret_writes_maybenull_z_(s)
#  define __zsal_defined_Ret_writes_maybenull_z
#endif

#if defined(_Ret_maybenull_) && defined(__zsal_defined_Ret_maybenull)
#  undef _Ret_maybenull_
#endif
#if !defined(_Ret_maybenull_)
#  define _Ret_maybenull_
#  define __zsal_defined_Ret_maybenull
#endif

#if defined(_Ret_maybenull_z_) && defined(__zsal_defined_Ret_maybenull_z)
#  undef _Ret_maybenull_z_
#endif
#if !defined(_Ret_maybenull_z_)
#  define _Ret_maybenull_z_
#  define __zsal_defined_Ret_maybenull_z
#endif

#if defined(_Ret_null_) && defined(__zsal_defined_Ret_null)
#  undef _Ret_null_
#endif
#if !defined(_Ret_null_)
#  define _Ret_null_
#  define __zsal_defined_Ret_null
#endif

#if defined(_Ret_notnull_) && defined(__zsal_defined_Ret_notnull)
#  undef _Ret_notnull_
#endif
#if !defined(_Ret_notnull_)
#  define _Ret_notnull_
#  define __zsal_defined_Ret_notnull
#endif

#if defined(_Ret_writes_bytes_to_) && defined(__zsal_defined_Ret_writes_bytes_to)
#  undef _Ret_writes_bytes_to_
#endif
#if !defined(_Ret_writes_bytes_to_)
#  define _Ret_writes_bytes_to_
#  define __zsal_defined_Ret_writes_bytes_to
#endif

#if defined(_Ret_writes_bytes_to_) && defined(__zsal_defined_Ret_writes_bytes_to)
#  undef _Ret_writes_bytes_to_
#endif
#if !defined(_Ret_writes_bytes_to_)
#  define _Ret_writes_bytes_to_
#  define __zsal_defined_Ret_writes_bytes_to
#endif

#if defined(_Ret_writes_bytes_maybenull_) && defined(__zsal_defined_Ret_writes_bytes_maybenull)
#  undef _Ret_writes_bytes_maybenull_
#endif
#if !defined(_Ret_writes_bytes_maybenull_)
#  define _Ret_writes_bytes_maybenull_
#  define __zsal_defined_Ret_writes_bytes_maybenull
#endif

#if defined(_Ret_writes_bytes_to_maybenull_) && defined(__zsal_defined_Ret_writes_bytes_to_maybenull)
#  undef _Ret_writes_bytes_to_maybenull_
#endif
#if !defined(_Ret_writes_bytes_to_maybenull_)
#  define _Ret_writes_bytes_to_maybenull_
#  define __zsal_defined_Ret_writes_bytes_to_maybenull
#endif 

#if defined(_In_range_) && defined(__zsal_defined_In_range)
#  undef _In_range_
#endif
#if !defined(_In_range_)
#  define _In_range_(low, hi)
#  define __zsal_defined_In_range
#endif

#if defined(_Out_range_) && defined(__zsal_defined_Out_range)
#  undef _Out_range_
#endif
#if !defined(_Out_range_)
#  define _Out_range_(low, hi)
#  define __zsal_defined_Out_range
#endif

#if defined(_Ret_range_) && defined(__zsal_defined_Ret_range)
#  undef _Ret_range_
#endif
#if !defined(_Ret_range_)
#  define _Ret_range_(low, hi)
#  define __zsal_defined_Ret_range
#endif

#if defined(_Deref_in_range_) && defined(__zsal_defined_Deref_in_range)
#  undef _Deref_in_range_
#endif
#if !defined(_Deref_in_range_)
#  define _Deref_in_range_(low, hi)
#  define __zsal_defined_Deref_in_range
#endif

#if defined(_Deref_out_range_) && defined(__zsal_defined_Deref_out_range)
#  undef _Deref_out_range_
#endif
#if !defined(_Deref_out_range_)
#  define _Deref_out_range_(low, hi)
#  define __zsal_defined_Deref_out_range
#endif

#if defined(_Deref_inout_range_) && defined(__zsal_defined_Deref_inout_range)
#  undef _Deref_inout_range_
#endif
#if !defined(_Deref_inout_range_)
#  define _Deref_inout_range_(low, hi)
#  define __zsal_defined_Deref_inout_range
#endif

#if defined(_Field_range_) && defined(__zsal_defined_Field_range)
#  undef _Field_range_
#endif
#if !defined(_Field_range_)
#  define _Field_range_(low, hi)
#  define __zsal_defined_Field_range
#endif

#if defined(_Pre_equal_to_) && defined(__zsal_defined_Pre_equal_to)
#  undef _Pre_equal_to_
#endif
#if !defined(_Pre_equal_to_)
#  define _Pre_equal_to_(expr)
#  define __zsal_defined_Pre_equal_to
#endif

#if defined(_Post_equal_to_) && defined(__zsal_defined_Post_equal_to)
#  undef _Post_equal_to_
#endif
#if !defined(_Post_equal_to_)
#  define _Post_equal_to_(expr)
#  define __zsal_defined_Post_equal_to
#endif

#if defined(_Struct_size_bytes_) && defined(__zsal_defined_Struct_size_bytes)
#  undef _Struct_size_bytes_
#endif
#if !defined(_Struct_size_bytes_)
#  define _Struct_size_bytes_(size)
#  define __zsal_defined_Struct_size_bytes
#endif 

#if defined(_Called_from_function_class_) && defined(__zsal_defined_Called_from_function_class)
#  undef _Called_from_function_class_
#endif
#if !defined(_Called_from_function_class_)
#  define _Called_from_function_class_(name)
#  define __zsal_defined_Called_from_function_class
#endif

#if defined(_Check_return_) && defined(__zsal_defined_Check_return)
#  undef _Check_return_
#endif
#if !defined(_Check_return_)
#  define _Check_return_
#  define __zsal_defined_Check_return
#endif

#if defined(_Function_class_) && defined(__zsal_defined_Function_class)
#  undef _Function_class_
#endif
#if !defined(_Function_class_)
#  define _Function_class_(name)
#  define __zsal_defined_Function_class
#endif

#if defined(_Raises_SEH_exception_) && defined(__zsal_defined_Raises_SEH_exception)
#  undef _Raises_SEH_exception_
#endif
#if !defined(_Raises_SEH_exception_)
#  define _Raises_SEH_exception_
#  define __zsal_defined_Raises_SEH_exception
#endif

#if defined(_Must_inspect_result_) && defined(__zsal_defined_Must_inspect_result)
#  undef _Must_inspect_result_
#endif
#if !defined(_Must_inspect_result_)
#  define _Must_inspect_result_
#  define __zsal_defined_Must_inspect_result
#endif

#if defined(_Use_decl_annotations_) && defined(__zsal_defined_Use_decl_annotations)
#  undef _Use_decl_annotations_
#endif
#if !defined(_Use_decl_annotations_)
#  define _Use_decl_annotations_
#  define __zsal_defined_Use_decl_annotations
#endif

#if defined(_Always_) && defined(__zsal_defined_Always)
#  undef _Always_
#endif
#if !defined(_Always_)
#  define _Always_(anno_list)
#  define __zsal_defined_Always
#endif

#if defined(_On_failure_) && defined(__zsal_defined_On_failure)
#  undef _On_failure_
#endif
#if !defined(_On_failure_)
#  define _On_failure_(anno_list)
#  define __zsal_defined_On_failure
#endif

#if defined(_Return_type_success_) && defined(__zsal_defined_Return_type_success)
#  undef _Return_type_success_
#endif
#if !defined(_Return_type_success_)
#  define _Return_type_success_(expr)
#  define __zsal_defined_Return_type_success
#endif

#if defined(_Success_) && defined(__zsal_defined_Success)
#  undef _Success_
#endif
#if !defined(_Success_)
#  define _Success_(expr)
#  define __zsal_defined_Success
#endif 

#if defined(_Field_range_) && defined(__zsal_defined_Field_range)
#  undef _Field_range_
#endif
#if !defined(_Field_range_)
#  define _Field_range_(low, high)
#  define __zsal_defined_Field_range
#endif

#if defined(_Field_size_) && defined(__zsal_defined_Field_size)
#  undef _Field_size_
#endif
#if !defined(_Field_size_)
#  define _Field_size_(size)
#  define __zsal_defined_Field_size
#endif

#if defined(_Field_size_part_) && defined(__zsal_defined_Field_size_part)
#  undef _Field_size_part_
#endif
#if !defined(_Field_size_part_)
#  define _Field_size_part_(size)
#  define __zsal_defined_Field_size_part
#endif

#if defined(_Field_size_opt_) && defined(__zsal_defined_Field_size_opt)
#  undef _Field_size_opt_
#endif
#if !defined(_Field_size_opt_)
#  define _Field_size_opt_(size)
#  define __zsal_defined_Field_size_opt
#endif

#if defined(_Field_size_bytes_) && defined(__zsal_defined_Field_size_bytes)
#  undef _Field_size_bytes_
#endif
#if !defined(_Field_size_bytes_)
#  define _Field_size_bytes_(size)
#  define __zsal_defined_Field_size_bytes
#endif

#if defined(_Field_size_bytes_opt_) && defined(__zsal_defined_Field_size_bytes_opt)
#  undef _Field_size_bytes_opt_
#endif
#if !defined(_Field_size_bytes_opt_)
#  define _Field_size_bytes_opt_(size)
#  define __zsal_defined_Field_size_bytes_opt
#endif

#if defined(_Field_size_part_) && defined(__zsal_defined_Field_size_part)
#  undef _Field_size_part_
#endif
#if !defined(_Field_size_part_)
#  define _Field_size_part_(size, count)
#  define __zsal_defined_Field_size_part
#endif

#if defined(_Field_size_part_opt_) && defined(__zsal_defined_Field_size_part_opt)
#  undef _Field_size_part_opt_
#endif
#if !defined(_Field_size_part_opt_)
#  define _Field_size_part_opt_(size, count)
#  define __zsal_defined_Field_size_part_opt
#endif

#if defined(_Field_size_bytes_part_) && defined(__zsal_defined_Field_size_bytes_part)
#  undef _Field_size_bytes_part_
#endif
#if !defined(_Field_size_bytes_part_)
#  define _Field_size_bytes_part_(size, count)
#  define __zsal_defined_Field_size_bytes_part
#endif

#if defined(_Field_size_bytes_part_opt_) && defined(__zsal_defined_Field_size_bytes_part_opt)
#  undef _Field_size_bytes_part_opt_
#endif
#if !defined(_Field_size_bytes_part_opt_)
#  define _Field_size_bytes_part_opt_(size, count)
#  define __zsal_defined_Field_size_bytes_part_opt
#endif

#if defined(_Field_size_full_) && defined(__zsal_defined_Field_size_full)
#  undef _Field_size_full_
#endif
#if !defined(_Field_size_full_)
#  define _Field_size_full_(size)
#  define __zsal_defined_Field_size_full
#endif

#if defined(_Field_size_full_opt_) && defined(__zsal_defined_Field_size_full_opt)
#  undef _Field_size_full_opt_
#endif
#if !defined(_Field_size_full_opt_)
#  define _Field_size_full_opt_(size)
#  define __zsal_defined_Field_size_full_opt
#endif

#if defined(_Field_size_bytes_full_) && defined(__zsal_defined_Field_size_bytes_full)
#  undef _Field_size_bytes_full_
#endif
#if !defined(_Field_size_bytes_full_)
#  define _Field_size_bytes_full_(size)
#  define __zsal_defined_Field_size_bytes_full
#endif

#if defined(_Field_size_bytes_full_opt_) && defined(__zsal_defined_Field_size_bytes_full_opt)
#  undef _Field_size_bytes_full_opt_
#endif
#if !defined(_Field_size_bytes_full_opt_)
#  define _Field_size_bytes_full_opt_(size)
#  define __zsal_defined_Field_size_bytes_full_opt
#endif

#if defined(_Struct_size_bytes_) && defined(__zsal_defined_Struct_size_bytes)
#  undef _Struct_size_bytes_
#endif
#if !defined(_Struct_size_bytes_)
#  define _Struct_size_bytes_(size)
#  define __zsal_defined_Struct_size_bytes
#endif 

#if defined(_Acquires_exclusive_lock_) && defined(__zsal_defined_Acquires_exclusive_lock)
#  undef _Acquires_exclusive_lock_
#endif
#if !defined(_Acquires_exclusive_lock_)
#  define _Acquires_exclusive_lock_(expr)
#  define __zsal_defined_Acquires_exclusive_lock
#endif

#if defined(_Acquires_lock_) && defined(__zsal_defined_Acquires_lock)
#  undef _Acquires_lock_
#endif
#if !defined(_Acquires_lock_)
#  define _Acquires_lock_(expr)
#  define __zsal_defined_Acquires_lock
#endif

#if defined(_Acquires_nonreentrant_lock_) && defined(__zsal_defined_Acquires_nonreentrant_lock)
#  undef _Acquires_nonreentrant_lock_
#endif
#if !defined(_Acquires_nonreentrant_lock_)
#  define _Acquires_nonreentrant_lock_(expr)
#  define __zsal_defined_Acquires_nonreentrant_lock
#endif

#if defined(_Acquires_shared_lock_) && defined(__zsal_defined_Acquires_shared_lock)
#  undef _Acquires_shared_lock_
#endif
#if !defined(_Acquires_shared_lock_)
#  define _Acquires_shared_lock_(expr)
#  define __zsal_defined_Acquires_shared_lock
#endif

#if defined(_Create_lock_level_) && defined(__zsal_defined_Create_lock_level)
#  undef _Create_lock_level_
#endif
#if !defined(_Create_lock_level_)
#  define _Create_lock_level_(name)
#  define __zsal_defined_Create_lock_level
#endif

#if defined(_Has_lock_kind_) && defined(__zsal_defined_Has_lock_kind)
#  undef _Has_lock_kind_
#endif
#if !defined(_Has_lock_kind_)
#  define _Has_lock_kind_(kind)
#  define __zsal_defined_Has_lock_kind
#endif

#if defined(_Has_lock_level_) && defined(__zsal_defined_Has_lock_level)
#  undef _Has_lock_level_
#endif
#if !defined(_Has_lock_level_)
#  define _Has_lock_level_(name)
#  define __zsal_defined_Has_lock_level
#endif

#if defined(_Lock_level_order_) && defined(__zsal_defined_Lock_level_order)
#  undef _Lock_level_order_
#endif
#if !defined(_Lock_level_order_)
#  define _Lock_level_order_(name1, name2)
#  define __zsal_defined_Lock_level_order
#endif

#if defined(_Post_same_lock_) && defined(__zsal_defined_Post_same_lock)
#  undef _Post_same_lock_
#endif
#if !defined(_Post_same_lock_)
#  define _Post_same_lock_(expr1, expr2)
#  define __zsal_defined_Post_same_lock
#endif

#if defined(_Releases_exclusive_lock_) && defined(__zsal_defined_Releases_exclusive_lock)
#  undef _Releases_exclusive_lock_
#endif
#if !defined(_Releases_exclusive_lock_)
#  define _Releases_exclusive_lock_(expr)
#  define __zsal_defined_Releases_exclusive_lock
#endif

#if defined(_Releases_lock_) && defined(__zsal_defined_Releases_lock)
#  undef _Releases_lock_
#endif
#if !defined(_Releases_lock_)
#  define _Releases_lock_(expr)
#  define __zsal_defined_Releases_lock
#endif

#if defined(_Releases_nonreentrant_lock_) && defined(__zsal_defined_Releases_nonreentrant_lock)
#  undef _Releases_nonreentrant_lock_
#endif
#if !defined(_Releases_nonreentrant_lock_)
#  define _Releases_nonreentrant_lock_(expr)
#  define __zsal_defined_Releases_nonreentrant_lock
#endif

#if defined(_Releases_shared_lock_) && defined(__zsal_defined_Releases_shared_lock)
#  undef _Releases_shared_lock_
#endif
#if !defined(_Releases_shared_lock_)
#  define _Releases_shared_lock_(expr)
#  define __zsal_defined_Releases_shared_lock
#endif

#if defined(_Requires_lock_held_) && defined(__zsal_defined_Requires_lock_held)
#  undef _Requires_lock_held_
#endif
#if !defined(_Requires_lock_held_)
#  define _Requires_lock_held_(expr)
#  define __zsal_defined_Requires_lock_held
#endif

#if defined(_Requires_lock_not_held_) && defined(__zsal_defined_Requires_lock_not_held)
#  undef _Requires_lock_not_held_
#endif
#if !defined(_Requires_lock_not_held_)
#  define _Requires_lock_not_held_(expr)
#  define __zsal_defined_Requires_lock_not_held
#endif

#if defined(_Requires_no_locks_held_) && defined(__zsal_defined_Requires_no_locks_held)
#  undef _Requires_no_locks_held_
#endif
#if !defined(_Requires_no_locks_held_)
#  define _Requires_no_locks_held_
#  define __zsal_defined_Requires_no_locks_held
#endif

#if defined(_Requires_shared_lock_held_) && defined(__zsal_defined_Requires_shared_lock_held)
#  undef _Requires_shared_lock_held_
#endif
#if !defined(_Requires_shared_lock_held_)
#  define _Requires_shared_lock_held_(expr)
#  define __zsal_defined_Requires_shared_lock_held
#endif

#if defined(_Requires_exclusive_lock_held_) && defined(__zsal_defined_Requires_exclusive_lock_held)
#  undef _Requires_exclusive_lock_held_
#endif
#if !defined(_Requires_exclusive_lock_held_)
#  define _Requires_exclusive_lock_held_(expr)
#  define __zsal_defined_Requires_exclusive_lock_held
#endif 

#if defined(_Guarded_by_) && defined(__zsal_defined_Guarded_by)
#  undef _Guarded_by_
#endif
#if !defined(_Guarded_by_)
#  define _Guarded_by_(expr)
#  define __zsal_defined_Guarded_by
#endif

#if defined(_Interlocked_) && defined(__zsal_defined_Interlocked)
#  undef _Interlocked_
#endif
#if !defined(_Interlocked_)
#  define _Interlocked_
#  define __zsal_defined_Interlocked
#endif

#if defined(_Interlocked_operand_) && defined(__zsal_defined_Interlocked_operand)
#  undef _Interlocked_operand_
#endif
#if !defined(_Interlocked_operand_)
#  define _Interlocked_operand_
#  define __zsal_defined_Interlocked_operand
#endif

#if defined(_Write_guarded_by_) && defined(__zsal_defined_Write_guarded_by)
#  undef _Write_guarded_by_
#endif
#if !defined(_Write_guarded_by_)
#  define _Write_guarded_by_(expr)
#  define __zsal_defined_Write_guarded_by
#endif 

#if defined(_At_) && defined(__zsal_defined_At)
#  undef _At_
#endif
#if !defined(_At_)
#  define _At_(expr, anno_list)
#  define __zsal_defined_At
#endif

#if defined(_At_buffer_) && defined(__zsal_defined_At_buffer)
#  undef _At_buffer_
#endif
#if !defined(_At_buffer_)
#  define _At_buffer_(expr, iter, elem_count, anno_list)
#  define __zsal_defined_At_buffer
#endif

#if defined(_Group_) && defined(__zsal_defined_Group)
#  undef _Group_
#endif
#if !defined(_Group_)
#  define _Group_(anno_list)
#  define __zsal_defined_Group
#endif

#if defined(_When_) && defined(__zsal_defined_When)
#  undef _When_
#endif
#if !defined(_When_)
#  define _When_(expr, anno_list)
#  define __zsal_defined_When
#endif 

#if defined(_Curr_) && defined(__zsal_defined_Curr)
#  undef _Curr_
#endif
#if !defined(_Curr_)
#  define _Curr_
#  define __zsal_defined_Curr
#endif

#if defined(_Inexpressible_) && defined(__zsal_defined_Inexpressible)
#  undef _Inexpressible_
#endif
#if !defined(_Inexpressible_)
#  define _Inexpressible_(expr)
#  define __zsal_defined_Inexpressible
#endif

#if defined(_Nullterm_length_) && defined(__zsal_defined_Nullterm_length)
#  undef _Nullterm_length_
#endif
#if !defined(_Nullterm_length_)
#  define _Nullterm_length_(param)
#  define __zsal_defined_Nullterm_length
#endif

#if defined(_Old_) && defined(__zsal_defined_Old)
#  undef _Old_
#endif
#if !defined(_Old_)
#  define _Old_(expr)
#  define __zsal_defined_Old
#endif

#if defined(_Param_) && defined(__zsal_defined_Param)
#  undef _Param_
#endif
#if !defined(_Param_)
#  define _Param_(n)
#  define __zsal_defined_Param
#endif 

#if defined(_String_length_) && defined(__zsal_defined_String_length)
#  undef _String_length_
#endif
#if !defined(_String_length_)
#  define _String_length_(param)
#  define __zsal_defined_String_length
#endif 

#endif 
#endif 