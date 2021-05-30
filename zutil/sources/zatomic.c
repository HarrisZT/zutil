/******************************************************************************
* zutil - C Utility Library
* Copyright (C) 2017-2021 Zachary T Harris. All Rights Reserved.  
* Zlib license.
* 
* File: zatomic.c 
* Desc: Atomic operations and signal fencing utilities 
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
#include "zutil/zatomic.h"
#if (Z_COMPILER_MSVC)
#  include <windows.h>
#  include <intrin.h>
#else
#  include <stdatomic.h>
#endif 
 
 



Int32 
ZAtomic32_Load(
  _In_ const ZATOMIC32* _lpAtomic,
  _In_ memory_order       _eMemOrder) {

#if (Z_HAVE_ATOMIC)
  return atomic_load_explicit((ZATOMIC32*)_lpAtomic, _eMemOrder);
#else
  if (_eMemOrder != memory_order_relaxed)
	  _ReadWriteBarrier();
  return _lpAtomic->nonatomic;
#endif
}


Int64 
ZAtomic64_Load(
	_In_ const ZATOMIC64* _lpAtomic,
	_In_ memory_order       _eOrder) {

#if (Z_HAVE_ATOMIC)
	return atomic_load_explicit((ZATOMIC64*)_lpAtomic, _eMemOrder);
#else
	if (_eOrder != memory_order_relaxed)
		_ReadWriteBarrier();
#  if (Z_ARCHITECTURE_X86)
	Int64 iResult;
	__asm {
		mov esi, _lpAtomic;
		mov ebx, eax;
		mov ecx, edx;
		lock cmpxchg8b [esi];
		mov dword ptr iResult, eax;
		mov dword ptr iResult[4], edx;
	}
	return iResult;
#  else
	return _lpAtomic->nonatomic;
#  endif
#endif
}


Handle 
ZAtomicHandle_Load(
	_In_ const ZATOMICHANDLE* _lpAtomic,
	_In_ memory_order           _eOrder) {

#if (Z_HAVE_ATOMIC)
	return atomic_load_explicit((ZATOMICHANDLE*)_lpAtomic, _eMemOrder);
#else
	if (_eOrder != memory_order_relaxed)
		_ReadWriteBarrier();
	return (void*)_lpAtomic->nonatomic;
#endif
} 


Void
ZAtomic32_Store(
	_Inout_ ZATOMIC32* _lpAtomic,
	_In_    Int32        _iValue,
	_In_    memory_order _eMemOrder) {

#if (Z_HAVE_STDATOMIC)
	atomic_store_explicit(_lpAtomic, _iValue, _eMemOrder);
#else
	_lpAtomic->nonatomic = _iValue;
	if (_eMemOrder >= memory_order_release)
		_ReadWriteBarrier();
#endif
} 


Void
ZAtomic64_Store(
	_Inout_ ZATOMIC64* _lpAtomic,
	_In_    Int64        _iValue,
	_In_    memory_order _eMemOrder) {

#if (Z_HAVE_STDATOMIC)
	atomic_store_explicit(_lpAtomic, _iValue, _eMemOrder);
#else
#  if (Z_ARCHITECTURE_X86)
#    pragma warning(disable : 4731)
	__asm {
		push ebx;
		mov esi, _lpAtomic;
		mov ebx, dword ptr _iValue;
		mov ecx, dword ptr _iValue[4];
		retry:
		cmpxchg8b [esi];
		jne retry;
		pop ebx;
	}
#  else
	_lpAtomic->nonatomic = _iValue;
#  endif
	if (_eMemOrder >= memory_order_release)
		_ReadWriteBarrier();
#endif
}


Void
ZAtomicHandle_Store(
	_Inout_ ZATOMICHANDLE* _lpAtomic,
	_In_    Handle           _hValue,
	_In_    memory_order     _eMemOrder) {

#if (Z_HAVE_STDATOMIC)
	atomic_store_explicit(_lpAtomic, _hValue, _eMemOrder);
#else
	_lpAtomic->nonatomic = _hValue;
	if (_eMemOrder >= memory_order_release)
		_ReadWriteBarrier();
#endif
} 
 

Int32 
ZAtomic32_FetchAdd(
	_Inout_ ZATOMIC32* _lpAtomic, 
	_In_    Int32        _iValue, 
	_In_    memory_order _eMemOrder) {

#if (Z_HAVE_STDATOMIC)
	return atomic_fetch_add_explicit(
		_lpAtomic, _iValue, _eMemOrder) + _iValue;
#else
	Int32 old = (Int32)_InterlockedExchangeAdd(
		(volatile Long*)&_lpAtomic->nonatomic, _iValue);
	return (old + _iValue);
#endif
}


Int32 
ZAtomic64_FetchAdd(
	_Inout_ ZATOMIC64* _lpAtomic, 
	_In_    Int64        _iValue, 
	_In_    memory_order _eMemOrder) {

#if (Z_HAVE_STDATOMIC)
	return (Int32)(atomic_fetch_add_explicit(
		_lpAtomic, _iValue, _eMemOrder) + _iValue);
#else
#  if (Z_ARCHITECTURE_X86)
	return (Int32)(ZAtomic64_ExchangeAdd(
		_lpAtomic, _iValue, _eMemOrder) + _iValue);
#  else
	return (Int32)(_InterlockedExchangeAdd64(
		&val->nonatomic, add) + add);
#  endif
#endif
} 
 

Int32 
ZAtomic32_ExchangeAdd(
	_Inout_ ZATOMIC32* _lpAtomic,
	_In_    Int32        _iValue,
	_In_    memory_order _eMemOrder) {

#if (Z_HAVE_STDATOMIC)
	return atomic_fetch_add_explicit(_lpAtomic, _iValue, _eMemOrder);
#else
	return _InterlockedExchangeAdd(
		(volatile Long*)&_lpAtomic->nonatomic, _iValue);
#endif
}


Int32 
ZAtomic64_ExchangeAdd(
	_Inout_ ZATOMIC64*   _lpAtomic,
	_In_    Int64        _iValue,
	_In_    memory_order _eMemOrder) {

#if (Z_HAVE_STDATOMIC)
	return atomic_fetch_add_explicit(_lpAtomic, _iValue, _eMemOrder);
#else
#  if (Z_ARCHITECTURE_X86)
	Int64 ref;
	do {   
		ref = _lpAtomic->nonatomic;
	} while (_InterlockedCompareExchange64(
		(volatile Int64*)&_lpAtomic->nonatomic, ref + _iValue, ref) != ref);
	return (Int32)ref;
#  else //X86_64
	return (Int32)_InterlockedExchangeAdd64(
		&_lpAtomic->nonatomic, _iValue);
#  endif
#endif
}


Bool 
ZAtomic32_CompareAndSwap(
	_Inout_ ZATOMIC32*   _lpAtomic,
	_In_    Int32        _iValue,
	_In_    Int32        _iReference,
	_In_    memory_order _eSuccess,
	_In_    memory_order _eFailure) {

#if (Z_HAVE_STDATOMIC)
	return atomic_compare_exchange_weak_explicit(
		_lpAtomic,
		&_iReference, _iValue,
		_eSuccess, _eFailure);
#else
	return (_InterlockedCompareExchange(
		(volatile Long*)&_lpAtomic->nonatomic, _iValue,
		_iReference) == _iReference) ? Z_TRUE : Z_FALSE;
#endif
}


Bool
ZAtomic64_CompareAndSwap(
	_Inout_ ZATOMIC64*   _lpAtomic,
	_In_    Int64        _iValue,
	_In_    Int64        _iReference,
	_In_    memory_order _eSuccess,
	_In_    memory_order _eFailure) {

#if (Z_HAVE_STDATOMIC)
	return atomic_compare_exchange_weak_explicit(
		_lpAtomic,
		&_iReference, _iValue,
		_eSuccess, _eFailure);
#else
	return (_InterlockedCompareExchange64(
		(volatile Int64*)&_lpAtomic->nonatomic, _iValue,
		_iReference) == _iReference) ? Z_TRUE : Z_FALSE;
#endif
}


Bool 
ZAtomicHandle_CompareAndSwap(
	_Inout_ ZATOMICHANDLE* _lpAtomic, 
	_In_    Handle         _hValue, 
	_In_    Handle         _hReference, 
	_In_    memory_order   _eSuccess,
	_In_    memory_order   _eFailure) {

#if (Z_HAVE_STDATOMIC)
	return atomic_compare_exchange_weak_explicit(
		_lpAtomic, 
		&_hReference, _hValue, 
		_eSuccess, _eFailure);
#else
#  if (Z_SIZEOF_VOIDP == 8)
	return ZAtomic64_CompareAndSwap(
		(ZATOMIC64*)_lpAtomic, (Int64)(Uintptr)_hValue, 
		(Int64)(Uintptr)_hReference, _eSuccess, _eFailure);
#  else
	return ZAtomic32_CompareAndSwap(
		(ZATOMIC32*)_lpAtomic, (Int32)(Uintptr)_hValue, 
		(Int32)(Uintptr)_hReference, _eSuccess, _eFailure);
#  endif
#endif
} 


Void 
ZAtomicFence_AcquireSignal(Void) {
#if (Z_HAVE_STDATOMIC)
	atomic_signal_fence(memory_order_acquire);
#else
	_ReadWriteBarrier();
#endif
}


Void
ZAtomicFence_ReleaseSignal(Void) {
#if (Z_HAVE_STDATOMIC)
	atomic_signal_fence(memory_order_release);
#else
	_ReadWriteBarrier();
#endif
}


Void 
ZAtomicFence_SignalSequentiallyConsistent(Void) {
#if (Z_HAVE_STDATOMIC)
	atomic_signal_fence(memory_order_seq_cst);
#else
	_ReadWriteBarrier();
#endif
}


Void 
ZAtomicFence_AcquireThread(Void) {
#if (Z_HAVE_STDATOMIC)
	atomic_thread_fence(memory_order_acquire);
#else
	_ReadWriteBarrier();
#endif
}


Void
ZAtomicFence_ReleaseThread(Void) {
#if (Z_HAVE_STDATOMIC)
	atomic_thread_fence(memory_order_release);
#else
	_ReadWriteBarrier();
#endif
}


Void 
ZAtomicFence_ThreadSequentiallyConsistent(Void) {
#if (Z_HAVE_STDATOMIC)
	atomic_signal_fence(memory_order_seq_cst);
#else
	MemoryBarrier();
#endif
} 
/*****************************************************************************/  
//EOF
/*****************************************************************************/ 