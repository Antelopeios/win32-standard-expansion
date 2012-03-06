// Copyright 2011-2012, 木头云
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////////////
// RegistAlloc - 登记并自动构造/析构的内存分配器
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-05
// Version:	1.0.0021.2317
//
// History:
//	- 1.0.0015.2359(2011-03-01)	# CRegistAllocT::Free()一个空指针时会引发内存异常
//	- 1.0.0017.2130(2011-03-03)	# CTraitsT::Construct()与CTraitsT::Destruct()添加分配异常断言
//								= CTraitsT<TypeT>改为CTraits,将模板放入成员函数中处理
//	- 1.0.0018.2110(2011-03-04)	= CTraits的接口调整为根据类型数量而不是内存块大小对,内存块进行构造/析构处理
//								= CRegistAllocT在分配内存时自动记录内存分配的初始类型块数量,并根据记录的值对内存块构造/析构
//	- 1.0.0019.2102(2012-01-14)	+ 添加CRegistAllocT::ReAlloc()
//								= 将_Regist的定义放在CRegistAllocT外部实现,方便外部调用_Regist内的各种方法
//								= 将萃取器部分从RegistAlloc中独立出来
//	- 1.0.0020.0451(2012-01-29)	= CRegistAllocT使用静态内存分配器作为策略,默认使用EXP_MEMPOOL_ALLOC
//								= CRegistAllocT自身改写为静态类
//								= 定义CRegistAlloc为EXP_MEMORY_ALLOC,并将ExMem定义为CRegistAlloc的别称
//	- 1.0.0021.2317(2012-03-05)	= 按照标准要求,当Alloc的大小为0时分配最小内存块
//////////////////////////////////////////////////////////////////

#ifndef __RegistAlloc_h__
#define __RegistAlloc_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/MemPool.h"
#include "Memory/Traits.h"

EXP_BEG

//////////////////////////////////////////////////////////////////
// 用于内存块登记的结构定义

struct _Regist
{
	typedef _Traits::traitor_t traitor_t;
	typedef void* (*maker_t)(void*, DWORD);

	traitor_t	destruct;
	DWORD		count;

	EXP_INLINE static void* RealPtr(void* pPtr)
	{
		return (((_Regist*)pPtr) - 1);
	}
	EXP_INLINE static void* PtrReal(void* pReal)
	{
		return (((_Regist*)pReal) + 1);
	}

	template <typename TypeT>
	EXP_INLINE static void* Make(void* pReal, DWORD nCount, _false_type)
	{
		_Regist* reg = (_Regist*)pReal;
		reg->count = nCount;
		reg->destruct = (traitor_t)(_Traits::Destruct<TypeT>);
		return pReal;
	}
	template <typename TypeT>
	EXP_INLINE static void* Make(void* pReal, DWORD nCount, _true_type)
	{
		_Regist* reg = (_Regist*)pReal;
		reg->count = nCount;
		reg->destruct = NULL;
		return pReal;
	}
	template <typename TypeT>
	EXP_INLINE static void* Make(void* pReal, DWORD nCount)
	{
		return Make<TypeT>(pReal, nCount, _TraitsT<TypeT>::is_POD_type());
	}
};

//////////////////////////////////////////////////////////////////
// 登记并自动构造/析构的内存分配器

template <typename AllocT = EXP_MEMPOOL_ALLOC>
class CRegistAllocT
{
public:
	typedef _Regist::traitor_t traitor_t;
	typedef typename AllocT::alloc_t alloc_t;

public:
	template <typename TypeT>
	EXP_INLINE static TypeT* Construct(_Regist* pReal)
	{
		return _Traits::Construct<TypeT>(_Regist::PtrReal(pReal), pReal->count);
	}
	EXP_INLINE static void* Destruct(_Regist* pReal)
	{
		if (pReal->destruct)
		{
			pReal->destruct(_Regist::PtrReal(pReal), pReal->count);
			pReal->destruct = NULL;	// 仅允许析构一次,防止高层调用时意外的重复析构
		}							// 若需要重复析构,需手动调用CTraits::Destruct<TypeT>
		return pReal;
	}

public:
	EXP_INLINE static BOOL Valid(void* pPtr)
	{
		return alloc_t::Valid(_Regist::RealPtr(pPtr));
	}
	EXP_INLINE static DWORD Size(void* pPtr)
	{
		return alloc_t::Size(_Regist::RealPtr(pPtr)) - sizeof(_Regist);
	}

	EXP_INLINE static void* Alloc(DWORD nSize, LPCSTR sFile = NULL, int nLine = 0)
	{
		if (nSize == 0) nSize = 1;
		_Regist* real = (_Regist*)alloc_t::Alloc(sizeof(_Regist) + nSize);
		return _Regist::PtrReal(_Regist::Make<BYTE>(alloc_t::SetAlloc(real, sFile, nLine), nSize));
	}
	template <typename TypeT>
	EXP_INLINE static TypeT* Alloc(DWORD nCount = 1, LPCSTR sFile = NULL, int nLine = 0)
	{
		if (nCount == 0) return NULL;
		_Regist* real = (_Regist*)alloc_t::Alloc(sizeof(_Regist) + (sizeof(TypeT) * nCount));
		return Construct<TypeT>((_Regist*)_Regist::Make<TypeT>(alloc_t::SetAlloc(real, sFile, nLine), nCount));
	}

	EXP_INLINE static void* SetAlloc(void* p, LPCSTR sFile, int nLine)
	{
		alloc_t::SetAlloc(_Regist::RealPtr(p), sFile, nLine);
		return p;
	}

	EXP_INLINE static void* ReAlloc(void* pPtr, DWORD nSize, BOOL bFree = TRUE, LPCSTR sFile = NULL, int nLine = 0)
	{
		// 检查指针是否为空
		if (pPtr == NULL)
			return Alloc(nSize, sFile, nLine);
		// 检查大小是否为零
		if (nSize == 0)
		{
			if (bFree) Free(pPtr);
			return NULL;
		}
		// 检查指针现有大小
		DWORD ptr_siz = Size(pPtr);
		// 若现有大小可以满足需求, 则直接返回
		if (ptr_siz >= nSize)
			return pPtr;
		// 若现有大小无法满足需求, 则分配新的空间并拷贝原有数据
		void* ptr_new = Alloc(nSize, sFile, nLine);
		memcpy(ptr_new, pPtr, ptr_siz);
		// 释放旧空间, 返回新空间
		if (bFree) FreeNoDest(pPtr);
		return ptr_new;
	}
	template <typename TypeT>
	EXP_INLINE static TypeT* ReAlloc(void* pPtr, DWORD nCount, _true_type, 
									 BOOL bFree = TRUE, LPCSTR sFile = NULL, int nLine = 0)
	{
		return (TypeT*)ReAlloc(pPtr, sizeof(TypeT) * nCount, bFree, sFile, nLine);
	}
	template <typename TypeT>
	EXP_INLINE static TypeT* ReAlloc(void* pPtr, DWORD nCount, _false_type, 
									 BOOL bFree = TRUE, LPCSTR sFile = NULL, int nLine = 0)
	{
		// 检查指针是否为空
		if (pPtr == NULL)
			return Alloc<TypeT>(nCount, sFile, nLine);
		// 检查大小是否为零
		if (nCount == 0)
		{
			if (bFree) Free(pPtr);
			return NULL;
		}
		// 检查指针现有对象数量
		_Regist* real = (_Regist*)_Regist::RealPtr(pPtr);
		// 若现有数量等于需求数量, 则直接返回
		if (real->count == nCount)
			return (TypeT*)pPtr;
		// 若现有数量大于需求数量, 则销毁多余对象, 并直接返回
		if (real->count > nCount)
		{
			if (real->destruct)
				real->destruct(((TypeT*)pPtr) + nCount, real->count - nCount);
			real->count = nCount;
			return (TypeT*)pPtr;
		}
		// 若现有数量小于需求数量, 则检查指针现有大小
		DWORD ptr_siz = Size(pPtr);
		DWORD size = sizeof(TypeT) * nCount;
		// 若现有大小可以满足需求, 则构造新对象, 并直接返回
		if (ptr_siz >= size)
		{
			_Traits::Construct<TypeT>(((TypeT*)pPtr) + real->count, nCount - real->count);
			real->count = nCount;
			return (TypeT*)pPtr;
		}
		// 若现有大小无法满足需求, 则分配新的空间, 拷贝原有对象数据, 并构造新对象
		_Regist* real_new = (_Regist*)alloc_t::Alloc(sizeof(_Regist) + size);
		void* ptr_new = _Regist::PtrReal(_Regist::Make<TypeT>(alloc_t::SetAlloc(real_new, sFile, nLine), nCount));
		memcpy(ptr_new, pPtr, sizeof(TypeT) * real->count);
		_Traits::Construct<TypeT>(((TypeT*)ptr_new) + real->count, nCount - real->count);
		// 释放旧空间, 返回新空间
		if (bFree) FreeNoDest(pPtr);
		return (TypeT*)ptr_new;
	}
	template <typename TypeT>
	EXP_INLINE static TypeT* ReAlloc(void* pPtr, DWORD nCount, BOOL bFree = TRUE, LPCSTR sFile = NULL, int nLine = 0)
	{
		return ReAlloc<TypeT>(pPtr, nCount, _TraitsT<TypeT>::is_POD_type(), bFree, sFile, nLine);
	}

	EXP_INLINE static void Free(void* pPtr)
	{
		if (!pPtr) return;
		alloc_t::Free(Destruct((_Regist*)_Regist::RealPtr(pPtr)));
	}
	EXP_INLINE static void FreeNoDest(void* pPtr)
	{
		if (!pPtr) return;
		alloc_t::Free(_Regist::RealPtr(pPtr));
	}
};

typedef CRegistAllocT<> CRegistAlloc;
typedef CRegistAlloc ExMem;

#ifndef EXP_MEMORY_ALLOC
#define EXP_MEMORY_ALLOC CRegistAlloc
#endif/*EXP_MEMORY_ALLOC*/

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__RegistAlloc_h__*/