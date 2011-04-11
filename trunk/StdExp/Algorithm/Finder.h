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
// Finder - ²éÕÒÆ÷
//
// Author:	Ä¾Í·ÔÆ
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-01-06
// Version:	1.0.0001.1040
//////////////////////////////////////////////////////////////////

#ifndef __Finder_h__
#define __Finder_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

template <typename ContainerT>
class CFinderT
{
public:
	typedef typename ContainerT::type_t type_t;
	typedef typename ContainerT::iterator_t iterator_t;

public:
	static iterator_t& Find(ContainerT& Container, type_t& Item, iterator_t& Head, iterator_t& Tail)
	{
		static iterator_t iter;
		for(iter = Head; iter != Tail; ++iter)
			if (iter->Val() == Item) break;
		return iter;
	}

	static iterator_t& Find(ContainerT& Container, type_t& Item)
	{
		return Find(Container, Item, Container.Head(), Container.Tail());
	}

	static iterator_t& RevFind(ContainerT& Container, type_t& Item, iterator_t& Head, iterator_t& Tail)
	{
		static iterator_t iter;
		iter = Tail;
		if (iter == Container.Tail())
			iter = Container.Last();
		else
			--iter;
		for(; iter != Head; --iter)
			if (iter->Val() == Item) return iter;
		if (iter->Val() != Item)
			iter = Tail;
		return iter;
	}

	static iterator_t& RevFind(ContainerT& Container, type_t& Item)
	{
		return RevFind(Container, Item, Container.Head(), Container.Tail());
	}
};

//////////////////////////////////////////////////////////////////

#endif/*__Finder_h__*/