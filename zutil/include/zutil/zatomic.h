/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
*
* File: zatomic.h 
* Desc: Atomic operations and signal fencing utilities 
*  
*       Atomic operations functions allow one to atomically load, store and 
*       perform basic operations to 32 and 64 bit data locations. A signal 
*       fence guarantees memory order between threads on the same core or
*       between an interrupt and signal. Thread fences guarantee memory order
*       between multiple threads on a multicore system
* 
*       http://mintomic.github.io/lock-free/memory-model/ 
*       http://en.cppreference.com/w/cpp/atomic/memory_order 
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
#ifndef __ZATOMIC_H__
#define __ZATOMIC_H__ 

#include "zcore.h"  

#if (Z_COMPILER_MSVC || defined(__STDC_NO_ATOMICS__))  
#  define Z_HAVE_STDATOMIC 0
#else 
#  include <stdatomic.h>
#  define Z_HAVE_STDATOMIC 1
#endif 
#if defined(__cplusplus)
extern "C" {
#endif


#if (Z_COMPILER_MSVC || defined(__STDC_NO_ATOMICS__)) 
typedef enum memory_order {
	memory_order_relaxed,
	memory_order_consume,
	memory_order_acquire,
	memory_order_release,
	memory_order_acq_rel,
	memory_order_seq_cst
} memory_order; 
#endif 

	 
/* 
** Atomic types
******************************************************************************/
#if (Z_HAVE_STDATOMIC) 
  typedef volatile _Atomic(Int32)  ZATOMIC32;
  typedef volatile _Atomic(Int64)  ZATOMIC64;
  typedef volatile _Atomic(Handle) ZATOMICHANDLE; 
#else
  ALIGNAS(4) struct _ZAtomic32_t {
  	  volatile Int32 nonatomic;
  }; 
  ALIGNAS(8) struct _ZAtomic64_t {
  	  volatile Int64 nonatomic;
  };  
  ALIGNAS(Z_SIZEOF_VOIDP) struct _ZAtomicHandle_t {
  	  volatile Handle nonatomic;
  }; 
  typedef struct _ZAtomic32_t     ZATOMIC32;
  typedef struct _ZAtomic64_t     ZATOMIC64;
  typedef struct _ZAtomicHandle_t ZATOMICHANDLE; 
#endif




   
/* Section I:
** 32-Bit operations
******************************************************************************/
extern Int32 ZAPI 
ZAtomic32_Load(
  _In_ const ZATOMIC32* _lpAtomic,
  _In_ memory_order     _eMemOrder);

extern Void ZAPI
ZAtomic32_Store(
	_Inout_ ZATOMIC32*   _lpAtomic,
	_In_    Int32        _iValue,
	_In_    memory_order _eMemOrder);

extern Int32 ZAPI
ZAtomic32_FetchAdd(
	_Inout_ ZATOMIC32*   _lpAtomic, 
	_In_    Int32        _iValue, 
	_In_    memory_order _eMemOrder);

extern Int32 ZAPI
ZAtomic32_ExchangeAdd(
	_Inout_ ZATOMIC32*   _lpAtomic,
	_In_    Int32        _iValue,
	_In_    memory_order _eMemOrder);

extern Bool ZAPI
ZAtomic32_CompareAndSwap(
	_Inout_ ZATOMIC32*   _lpAtomic,
	_In_    Int32        _iValue,
	_In_    Int32        _iReference,
	_In_    memory_order _eSuccess,
	_In_    memory_order _eFailure);


#define ZAtomic32_Increment(a, m)\
  ZAtomic32_FetchAdd(a, +1, m)


#define ZAtomic32_Decrement(a, m)\
  ZAtomic32_FetchAdd(a, -1, m)





/* Section II:
** 64-Bit operations
******************************************************************************/
extern Int64 ZAPI
ZAtomic64_Load(
	_In_ const ZATOMIC64* _lpAtomic,
	_In_ memory_order     _eOrder);

extern Void ZAPI
ZAtomic64_Store(
	_Inout_ ZATOMIC64*   _lpAtomic,
	_In_    Int64        _iValue,
	_In_    memory_order _eMemOrder);

extern Int32 ZAPI 
ZAtomic64_FetchAdd(
	_Inout_ ZATOMIC64*   _lpAtomic, 
	_In_    Int64        _iValue, 
	_In_    memory_order _eMemOrder); 

extern Int32 ZAPI
ZAtomic64_ExchangeAdd(
	_Inout_ ZATOMIC64*   _lpAtomic,
	_In_    Int64        _iValue,
	_In_    memory_order _eMemOrder);

extern Bool ZAPI
ZAtomic64_CompareAndSwap(
	_Inout_ ZATOMIC64*   _lpAtomic,
	_In_    Int64        _iValue,
	_In_    Int64        _iReference,
	_In_    memory_order _eSuccess,
	_In_    memory_order _eFailure);

#define ZAtomic64_Increment(a, m)\
  ZAtomic64_FetchAdd(a, +1, m)

#define ZAtomic64_Decrement(a, m)\
  ZAtomic64_FetchAdd(a, -1, m)





/* Section III:
** Handle data operations
******************************************************************************/
extern Handle ZAPI
ZAtomicHandle_Load(
	_In_ const ZATOMICHANDLE* _lpAtomic,
	_In_ memory_order         _eOrder);

extern Void ZAPI
ZAtomicHandle_Store(
	_Inout_ ZATOMICHANDLE* _lpAtomic,
	_In_    Handle         _hValue,
	_In_    memory_order   _eMemOrder); 

extern Bool ZAPI
ZAtomicHandle_CompareAndSwap(
	_Inout_ ZATOMICHANDLE* _lpAtomic, 
	_In_    Handle         _hValue, 
	_In_    Handle         _hReference, 
	_In_    memory_order   _eSuccess,
	_In_    memory_order   _eFailure);





/* Section IV:
** Atomic Fence operations
******************************************************************************/
extern Void ZAPI
ZAtomicFence_AcquireSignal(Void);

extern Void ZAPI
ZAtomicFence_ReleaseSignal(Void);

extern Void ZAPI
ZAtomicFence_SignalSequentiallyConsistent(Void);

extern Void ZAPI
ZAtomicFence_AcquireThread(Void);

extern Void ZAPI
ZAtomicFence_ReleaseThread(Void);

extern Void ZAPI
ZAtomicFence_ThreadSequentiallyConsistent(Void); 



#if defined(__cplusplus)
}
#endif
/*****************************************************************************/  
#endif //EOF
/*****************************************************************************/  