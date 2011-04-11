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
// MemAlloc - 静态内存分配器
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-05
// Version:	1.0.0016.2133
//
// History:
//	- 1.0.0016.2133(2011-04-05)	- 移除ExMem的通用内存分配接口定义,此定义在GC.h中实现
//////////////////////////////////////////////////////////////////

#ifndef __MemAlloc_h__
#define __MemAlloc_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/MemPool.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// 将普通内存分配转换为静态内存分配的适配器
template <typename AllocT = _MemPool>
class CMemAdapterT : public CSingletonT<AllocT>
{
public:
	typedef AllocT alloc_t;

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
	EXP_INLINE static bool Valid(void* pPtr)
	{ return GetPolicy().Valid(pPtr); }
	EXP_INLINE static DWORD Size(void* pPtr)
	{ return GetPolicy().Size(pPtr); }
	template <typename TypeT>
	EXP_INLINE static TypeT* Alloc(DWORD nCount = 1)
	{ return GetPolicy().Alloc<TypeT>(nCount); }
	EXP_INLINE static void* Alloc(DWORD nSize)
	{ return GetPolicy().Alloc(nSize); }
	EXP_INLINE static void Free(void* pPtr)
	{ GetPolicy().Free(pPtr); }
};

#ifndef EXP_MEMORY_ALLOC
#define EXP_MEMORY_ALLOC CMemAdapterT<>
#endif/*EXP_MEMORY_ALLOC*/

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__MemAlloc_h__*/