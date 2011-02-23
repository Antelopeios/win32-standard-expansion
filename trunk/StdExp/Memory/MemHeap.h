// Copyright 2011, Ä¾Í·ÔÆ
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
// MemHeap - ¶ÑÄÚ´æ·ÖÅäÆ÷
//
// Author:	Ä¾Í·ÔÆ
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-02-21
// Version:	1.0.0003.0025
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
	static bool Valid(void* pPtr)
	{ return HeapValidate(GetProcessHeap(), 0, pPtr); }
	static DWORD Size(void* pPtr)
	{ return HeapSize(GetProcessHeap(), 0, pPtr); }
	static void* Alloc(DWORD nSize)
	{ return HeapAlloc(GetProcessHeap(), 0, nSize); }
	static void Free(void* pPtr)
	{ HeapFree(GetProcessHeap(), 0, pPtr); }
};

typedef CRegistAllocT<CMemHeapAlloc> _MemHeap;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__HeapAlloc_h__*/