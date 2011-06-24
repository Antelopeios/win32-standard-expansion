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
// Finder - 查找器
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-06-20
// Version:	1.0.0002.1612
//
// History:
//	- 1.0.0002.1612(2011-06-20)	^ 简化CFinderT的查找接口
//								+ CFinderT添加在容器中查找子容器的算法
//////////////////////////////////////////////////////////////////

#ifndef __Finder_h__
#define __Finder_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename ContainerT>
class CFinderT
{
public:
	typedef typename ContainerT::type_t type_t;
	typedef typename ContainerT::iterator_t iterator_t;

public:
	// 正向搜索
	static iterator_t& Find(iterator_t& Head, iterator_t& Tail, type_t& Item)
	{
		static iterator_t iter;
		for(iter = Head; iter != Tail; ++iter)
			if (iter->Val() == Item) break;
		return iter;
	}
	static iterator_t& Find(iterator_t& Head, iterator_t& Tail, ContainerT& cnt2)
	{
		static iterator_t iter;
		for(iter = Head; iter != Tail; ++iter)
		{
			iterator_t ite1 = iter, ite2 = cnt2.Head();
			for(; (ite1 != Tail) && (ite2 != cnt2.Tail()); ++ite1, ++ite2)
				if (ite1->Val() != ite2->Val()) break;
			if (ite2 == cnt2.Tail()) break;
		}
		return iter;
	}
	static iterator_t& Find(ContainerT& cnt1, type_t& Item)
	{
		return Find(cnt1.Head(), cnt1.Tail(), Item);
	}
	static iterator_t& Find(ContainerT& cnt1, ContainerT& cnt2)
	{
		return Find(cnt1.Head(), cnt1.Tail(), cnt2);
	}

	// 反向搜索
	static iterator_t& RevFind(iterator_t& Head, iterator_t& Tail, type_t& Item)
	{
		static iterator_t iter;
		iter = Tail;
		for(--iter; iter != Head; --iter)
			if (iter->Val() == Item) return iter;
		if (iter->Val() != Item)
			iter = Tail;
		return iter;
	}
	static iterator_t& RevFind(iterator_t& Head, iterator_t& Tail, ContainerT& cnt2)
	{
		static iterator_t iter;
		iter = Tail;
		for(--iter; iter != Head; --iter)
		{
			iterator_t ite1 = iter, ite2 = cnt2.Last();
			for(; (ite1 != Head) && (ite2 != cnt2.Head()); --ite1, --ite2)
				if (ite1->Val() != ite2->Val()) break;
			if (ite2 == cnt2.Head() && ite1->Val() == ite2->Val()) return (iter = ite1);
		}
		iterator_t ite1 = iter, ite2 = cnt2.Last();
		for(; (ite1 != Head) && (ite2 != cnt2.Head()); --ite1, --ite2)
			if (ite1->Val() != ite2->Val()) return Tail;
		if (ite2 == cnt2.Head() && ite1->Val() == ite2->Val()) return (iter = ite1);
		return Tail;
	}
	static iterator_t& RevFind(ContainerT& cnt1, type_t& Item)
	{
		return RevFind(cnt1.Head(), cnt1.Tail(), Item);
	}
	static iterator_t& RevFind(ContainerT& cnt1, ContainerT& cnt2)
	{
		return RevFind(cnt1.Head(), cnt1.Tail(), cnt2);
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Finder_h__*/