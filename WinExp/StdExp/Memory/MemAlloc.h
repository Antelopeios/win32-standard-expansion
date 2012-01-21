// Copyright 2011-2012, ľͷ��
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
// MemAlloc - ��̬�ڴ������
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-14
// Version:	1.0.0018.1650
//
// History:
//	- 1.0.0016.2133(2011-04-05)	- �Ƴ�ExMem��ͨ���ڴ����ӿڶ���,�˶�����GC.h��ʵ��
//	- 1.0.0017.1540(2011-07-20)	= ��CMemAdapterT�ĵ����������ⲿ,����EXP_SINGLETON_MEMALLOC,�����ⲿ����Ҫ�����滻
//	- 1.0.0018.1650(2012-01-14)	+ ���CMemAdapterT::ReAlloc()
//////////////////////////////////////////////////////////////////

#ifndef __MemAlloc_h__
#define __MemAlloc_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/MemPool.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

#ifndef EXP_SINGLETON_MEMALLOC
#define EXP_SINGLETON_MEMALLOC EXP_SINGLETON<AllocT>
#endif/*EXP_SINGLETON_MEMALLOC*/

// ����ͨ�ڴ����ת��Ϊ��̬�ڴ�����������
template <typename AllocT = _MemPool>
class CMemAdapterT : public EXP_SINGLETON_MEMALLOC
{
public:
	typedef AllocT alloc_t;
	enum { HeadSize = alloc_t::HeadSize };

	EXP_INLINE static alloc_t& GetPolicy()
	{ return Instance(); }
	EXP_INLINE static typename alloc_t::alloc_t& GetAlloc()
	{ return GetPolicy().GetAlloc(); }

	template <typename TypeT>
	EXP_INLINE TypeT* Construct(void* pPtr)
	{ return GetPolicy().Construct<TypeT>(pPtr); }
	EXP_INLINE void* Destruct(void* pPtr)
	{ return GetPolicy().Destruct(pPtr); }

public:
	EXP_INLINE static BOOL Valid(void* pPtr)
	{ return GetPolicy().Valid(pPtr); }
	EXP_INLINE static DWORD Size(void* pPtr)
	{ return GetPolicy().Size(pPtr); }

	template <typename TypeT>
	EXP_INLINE static TypeT* Alloc(DWORD nCount = 1)
	{ return GetPolicy().Alloc<TypeT>(nCount); }
	EXP_INLINE static void* Alloc(DWORD nSize)
	{ return GetPolicy().Alloc(nSize); }

	template <typename TypeT>
	EXP_INLINE static TypeT* ReAlloc(void* pPtr, DWORD nCount, BOOL bFree = TRUE)
	{ return GetPolicy().ReAlloc<TypeT>(pPtr, nCount, bFree); }
	EXP_INLINE static void* ReAlloc(void* pPtr, DWORD nSize, BOOL bFree = TRUE)
	{ return GetPolicy().ReAlloc(pPtr, nSize, bFree); }

	EXP_INLINE static void Free(void* pPtr)
	{ GetPolicy().Free(pPtr); }
};

#ifndef EXP_MEMORY_ALLOC
#define EXP_MEMORY_ALLOC CMemAdapterT<>
#endif/*EXP_MEMORY_ALLOC*/

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__MemAlloc_h__*/