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
// Hash - 哈希算法
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-26
// Version:	1.0.0007.2140
//
// History:
//	- 1.0.0005.0106(2011-02-25)	= 以模板的形式为一般类型做更为通用的泛型算法
//	- 1.0.0006.1525(2011-08-26)	# 使用模板特化解决目标类无法重载DWORD_PTR操作符时将无法正常使用HashKey函数计算Hash的问题
//	- 1.0.0007.2140(2012-01-26)	^ 参考 MFC v10.0 的部分哈希算法优化CHash的计算过程
//////////////////////////////////////////////////////////////////

#ifndef __Hash_h__
#define __Hash_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Container/String.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CHash
{
public:
	template<typename KeyT>
	EXP_INLINE static DWORD HashKey(KeyT Key)
	{
		// (algorithm copied from STL hash in xfunctional)
		ldiv_t hash_val = ldiv((long)Key, 127773);
		hash_val.rem = 16807 * hash_val.rem - 2836 * hash_val.quot;
		if (hash_val.rem < 0) hash_val.rem += 2147483647;
		return ((UINT)hash_val.rem);
	}
	template<>
	EXP_INLINE static DWORD HashKey<LPCSTR>(LPCSTR Key)
	{
		// hash key to UINT value by pseudorandomizing transform
		// (algorithm copied from STL string hash in xfunctional)
		DWORD hash_val = 2166136261U;
		DWORD fst = 0;
		DWORD lst = (DWORD)strlen(Key);
		DWORD stride = 1 + lst / 10;
		for(; fst < lst; fst += stride)
			hash_val = 16777619U * hash_val ^ (DWORD)Key[fst];
		return (hash_val);
	}
	template<>
	EXP_INLINE static DWORD HashKey<LPCWSTR>(LPCWSTR Key)
	{
		// hash key to UINT value by pseudorandomizing transform
		// (algorithm copied from STL string hash in xfunctional)
		DWORD hash_val = 2166136261U;
		DWORD fst = 0;
		DWORD lst = (DWORD)wcslen(Key);
		DWORD stride = 1 + lst / 10;
		for(; fst < lst; fst += stride)
			hash_val = 16777619U * hash_val ^ (DWORD)Key[fst];
		return (hash_val);
	}
	template<>
	EXP_INLINE static DWORD HashKey<CStringT<char> >(CStringT<char> Key)
	{
		return HashKey<LPCSTR>((LPCSTR)Key);
	}
	template<>
	EXP_INLINE static DWORD HashKey<CStringT<wchar_t> >(CStringT<wchar_t> Key)
	{
		return HashKey<LPCWSTR>((LPCWSTR)Key);
	}
	template<>
	EXP_INLINE static DWORD HashKey<__int64>(__int64 key)
	{
		// (algorithm copied from STL hash in xfunctional)
		return (HashKey<DWORD>((DWORD)(key & 0xffffffffUL)) ^ HashKey<DWORD>((DWORD)(key >> 32)));
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Hash_h__*/