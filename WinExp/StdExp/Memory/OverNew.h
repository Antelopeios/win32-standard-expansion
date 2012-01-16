// Copyright 2012, 木头云
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
// OverNew - 全局new操作符重载
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-15
// Version:	1.0.0000.1800
//
// History:
//	- 1.0.0000.1800(2012-01-15)	@ 开始构建全局new操作符重载
//////////////////////////////////////////////////////////////////

#ifndef __OverNew_h__
#define __OverNew_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/GC.h"

#ifdef	EXP_USING_NEW

//////////////////////////////////////////////////////////////////
// malloc

EXP_INLINE void* __cdecl operator new(size_t size)
{
	return EXP::ExMem::Alloc(size);
}

EXP_INLINE void __cdecl operator delete(void* ptr)
{
#ifdef	EXP_MANAGED_ALLPTR
	ZeroMemory(EXP::_Regist::RealPtr(ptr), sizeof(EXP::_Regist));
#endif/*EXP_MANAGED_ALLPTR*/
	EXP::ExMem::Free(ptr);
}

EXP_INLINE void* __cdecl operator new(size_t size, EXP::CGC& gc, EXP::_Regist::maker_t maker)
{
	return maker(EXP::ExMem::Alloc(&gc, size), size);
}

EXP_INLINE void __cdecl operator delete(void* ptr, EXP::CGC&, EXP::_Regist::maker_t)
{
	operator delete(ptr);
}

//////////////////////////////////////////////////////////////////
// realloc

EXP_INLINE void* __cdecl operator new(size_t size, void* ptr, void*)
{
	return EXP::ExMem::ReAlloc(ptr, size);
}

EXP_INLINE void __cdecl operator delete(void* ptr, void*, void*)
{
	operator delete(ptr);
}

EXP_INLINE void* __cdecl operator new(size_t size, EXP::CGC& gc, EXP::_Regist::maker_t maker, void* ptr)
{
	return maker(EXP::ExMem::ReAlloc(&gc, ptr, size), size);
}

EXP_INLINE void __cdecl operator delete(void* ptr, EXP::CGC&, EXP::_Regist::maker_t, void*)
{
	operator delete(ptr);
}

//////////////////////////////////////////////////////////////////

#undef	gcnew
#define gcnew(gc, type) new(gc, (&EXP::_Regist::Maker<type>)) type

#undef	renew
#define renew(ptr, type) new(ptr, NULL) type

#undef	gcrenew
#define gcrenew(gc, ptr, type) new(gc, (&EXP::_Regist::Maker<type>), ptr) type

//////////////////////////////////////////////////////////////////

#endif/*EXP_USING_NEW*/

#endif/*__OverNew_h__*/