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
// MemHeap - 堆内存分配器
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-11
// Version:	1.0.0006.1202
//
// History:
//	- 1.0.0004.2110(2011-03-04)	^ CMemHeapAlloc的接口添加EXP_INLINE提高效率
//								+ 添加ExCheckMem()内存正确性检查接口
//	- 1.0.0005.1316(2011-11-27)	+ 定义EXP_MEMHEAP_ALLOC宏,支持外部替换默认的堆分配器
//	- 1.0.0006.1202(2012-01-11)	+ 添加CMemHeapAlloc::ReAlloc()
//////////////////////////////////////////////////////////////////

#ifndef __HeapAlloc_h__
#define __HeapAlloc_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/RegistAlloc.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CMemHeapAlloc
{
public:
	typedef CMemHeapAlloc alloc_t;

public:
	EXP_INLINE static BOOL Valid(void* pPtr)
	{ return HeapValidate(GetProcessHeap(), 0, pPtr); }
	EXP_INLINE static DWORD Size(void* pPtr)
	{ return HeapSize(GetProcessHeap(), 0, pPtr); }
	EXP_INLINE static void* Alloc(DWORD nSize)
	{ return HeapAlloc(GetProcessHeap(), 0, nSize); }
	EXP_INLINE static void* ReAlloc(void* pPtr, DWORD nSize)
	{
		if (nSize == 0 && !pPtr) return NULL;
		if (nSize == 0)
		{
			Free(pPtr);
			return NULL;
		}
		return pPtr ? HeapReAlloc(GetProcessHeap(), 0, pPtr, nSize) : Alloc(nSize);
	}
	EXP_INLINE static void Free(void* pPtr)
	{ HeapFree(GetProcessHeap(), 0, pPtr); }

	EXP_INLINE static void Check()
	{
		static const int HEAP_NUMBER = 1024;
		HANDLE heaps_hdl[HEAP_NUMBER];
		int heaps_siz = GetProcessHeaps(HEAP_NUMBER, heaps_hdl);
		if (heaps_siz > 0)
			for (int i = 0; i < heaps_siz; ++i)
				ExAssert(HeapValidate(heaps_hdl[i], 0, NULL));
	}
};

#ifndef EXP_MEMHEAP_ALLOC
#define EXP_MEMHEAP_ALLOC CMemHeapAlloc
#endif/*EXP_MEMHEAP_ALLOC*/

#ifndef ExCheckMem
#ifdef _DEBUG
#define ExCheckMem	EXP_MEMHEAP_ALLOC::Check
#else
#define ExCheckMem	__noop
#endif
#endif

typedef CRegistAllocT<EXP_MEMHEAP_ALLOC> _MemHeap;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__HeapAlloc_h__*/