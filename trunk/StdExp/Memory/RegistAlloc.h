// Copyright 2011, 木头云
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
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-03-03
// Version:	1.0.0016.1400
//
// History:
//	- 1.0.0015.2359(2011-03-01)	# CRegistAllocT::Free()一个空指针时会引发内存异常
//	- 1.0.0016.1400(2011-03-03)	# CTraitsT::Construct()与CTraitsT::Destruct()添加分配异常断言
//////////////////////////////////////////////////////////////////

#ifndef __RegistAlloc_h__
#define __RegistAlloc_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Pattern/Singleton.h"

#include <new>

EXP_BEG

//////////////////////////////////////////////////////////////////
// 构造/析构函数萃取器

template <typename TypeT>
class CTraitsT
{
public:
	typedef void (*traitor_t)(void*, DWORD);

public:
	EXP_INLINE static TypeT* Construct(void* pPtr, DWORD nSize = sizeof(TypeT))
	{
		DWORD count = (nSize / sizeof(TypeT));
		ExAssert(count);
		if (count == 0) return NULL;
	#pragma push_macro("new")
	#undef new
		for (DWORD i = 0; i < count; ++i)
			::new(((TypeT*)pPtr) + i) TypeT;
	#pragma pop_macro("new")
		return (TypeT*)pPtr;
	}
	EXP_INLINE static TypeT* Destruct(void* pPtr, DWORD nSize = sizeof(TypeT))
	{
		DWORD count = (nSize / sizeof(TypeT));
		ExAssert(count);
		if (count == 0) return NULL;
		for (DWORD i = 0; i < count; ++i)
			((TypeT*)pPtr)[i].TypeT::~TypeT();
		return (TypeT*)pPtr;
	}
};

//////////////////////////////////////////////////////////////////
// 登记并自动构造/析构的内存分配器

template <typename AllocT>
class CRegistAllocT
{
public:
	typedef CTraitsT<BYTE>::traitor_t traitor_t;
	typedef typename AllocT::alloc_t alloc_t;

protected:
	struct _Regist { traitor_t destruct; };
public:
	enum { HeadSize = sizeof(_Regist) };

protected:
	alloc_t m_Alloc;

protected:
	EXP_INLINE void* RealPtr(void* pPtr)
	{
		_Regist* real = (((_Regist*)pPtr) - 1);
		return real;
	}

public:
	EXP_INLINE alloc_t& GetAlloc() { return m_Alloc; }

	template <typename TypeT>
	EXP_INLINE TypeT* Construct(void* pPtr)
	{
		ExAssert(pPtr);
		_Regist* real = (_Regist*)RealPtr(pPtr);
		real->destruct = (traitor_t)(CTraitsT<TypeT>::Destruct);
		return CTraitsT<TypeT>::Construct(pPtr, (m_Alloc.Size(real) - HeadSize));
	}
	EXP_INLINE void* Destruct(void* pPtr)
	{
		ExAssert(pPtr);
		_Regist* real = (_Regist*)RealPtr(pPtr);
		if (real->destruct)
		{
			real->destruct(pPtr, (m_Alloc.Size(real) - HeadSize));
			real->destruct = NULL;	// 仅允许析构一次,防止高层调用时意外的重复析构
		}							// 若需要重复析构,需手动调用CTraitsT<TypeT>::Destruct
		return real;
	}

public:
	EXP_INLINE bool Valid(void* pPtr)
	{
		return m_Alloc.Valid(RealPtr(pPtr));
	}
	EXP_INLINE DWORD Size(void* pPtr)
	{
		void* real = RealPtr(pPtr);
		return (m_Alloc.Size(real) - HeadSize);
	}
	template <typename TypeT>
	EXP_INLINE TypeT* Alloc(DWORD nCount = 1)
	{
		if (nCount == 0) return NULL;
		_Regist* real = (_Regist*)m_Alloc.Alloc(HeadSize + (sizeof(TypeT) * nCount));
		return Construct<TypeT>(real + 1);
	}
	EXP_INLINE void* Alloc(DWORD nSize)
	{
		if (nSize == 0) return NULL;
		ExAssert(nSize <= ((DWORD)~0 - HeadSize));
		return (void*)Alloc<BYTE>(nSize);
	}
	EXP_INLINE void Free(void* pPtr)
	{
		if (!pPtr) return;
		m_Alloc.Free(Destruct(pPtr));
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__RegistAlloc_h__*/