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
// ContainerObject - 容器对象基类
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-01-27
// Version:	1.0.0008.1730
//////////////////////////////////////////////////////////////////

#ifndef __ContainerObject_h__
#define __ContainerObject_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Pattern/Iterator.h"
#include "Algorithm/Finder.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename TypeT, typename PolicyT, typename ContainerT>
interface IContainerObjectT
{
public:
	typedef TypeT		type_t;
	typedef PolicyT		policy_t;
	typedef ContainerT	container_t;

	typedef typename policy_t::alloc_t alloc_t;
	typedef typename policy_t::node_t<container_t> node_t;
	typedef CIteratorT<node_t> iterator_t;
	typedef CFinderT<container_t> finder_t;

private:
	EXP_INLINE container_t* This()
	{ return (container_t*)this; }
	EXP_INLINE const container_t* This() const
	{ return (const container_t*)this; }

public:
	IContainerObjectT()
	{}
	virtual ~IContainerObjectT()
	{ Clear(); }

public:
	DWORD GetCount() const
	{ return This()->container_t::GetCount(); }
	bool Empty() const
	{ return This()->container_t::Empty(); }
	void Clear()
	{ return This()->container_t::Clear(); }

	iterator_t& Head()
	{ return This()->container_t::Head(); }
	iterator_t& Tail()
	{ return This()->container_t::Tail(); }
	iterator_t& Last()
	{ return This()->container_t::Last(); }
	type_t& HeadItem()
	{ return This()->container_t::HeadItem(); }
	type_t& LastItem()
	{ return This()->container_t::LastItem(); }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ContainerObject_h__*/