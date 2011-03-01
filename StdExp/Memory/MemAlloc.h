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
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-02-23
// Version:	1.0.0015.0930
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

	inline static alloc_t& GetPolicy()
	{ return Instance(); }
	inline static typename alloc_t::alloc_t& GetAlloc()
	{ return GetPolicy().GetAlloc(); }

	template <typename TypeT>
	inline TypeT* Construct(void* pPtr)
	{ return GetPolicy().Construct<TypeT>(pPtr); }
	inline void* Destruct(void* pPtr)
	{ return GetPolicy().Destruct(pPtr); }

public:
	static bool Valid(void* pPtr)
	{ return GetPolicy().Valid(pPtr); }
	static DWORD Size(void* pPtr)
	{ return GetPolicy().Size(pPtr); }
	template <typename TypeT>
	static TypeT* Alloc(DWORD nCount = 1)
	{ return GetPolicy().Alloc<TypeT>(nCount); }
	static void* Alloc(DWORD nSize)
	{ return GetPolicy().Alloc(nSize); }
	static void Free(void* pPtr)
	{ GetPolicy().Free(pPtr); }
};

#ifndef EXP_MEMORY_ALLOC
#define EXP_MEMORY_ALLOC CMemAdapterT<>
#endif/*EXP_MEMORY_ALLOC*/

typedef EXP_MEMORY_ALLOC ExMem;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__MemAlloc_h__*/