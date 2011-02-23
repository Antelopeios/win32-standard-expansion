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
// Swap - 交换算法
//
// Author:	木头云
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-02-15
// Version:	1.0.0001.1340
//////////////////////////////////////////////////////////////////

#ifndef __Swap_h__
#define __Swap_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

template <typename TypeT>
class CSwapT
{
public:
	typedef TypeT type_t;

public:
	static type_t Swap(type_t& Type1, type_t& Type2)
	{
		type_t typ_tmp(Type1);
		Type1 = Type2;
		Type2 = typ_tmp;
		return typ_tmp;
	}
};

template <typename TypeT>
TypeT ExSwap(TypeT& Type1, TypeT& Type2)
{ return CSwapT<TypeT>::Swap(Type1, Type2); }

//////////////////////////////////////////////////////////////////

#endif/*__Swap_h__*/