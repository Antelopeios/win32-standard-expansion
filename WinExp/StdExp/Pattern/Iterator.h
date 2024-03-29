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
// Iterator - 迭代器
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-11
// Version:	1.0.0006.2325
//
// History:
//	- 1.0.0006.2325(2011-05-11)	= 微调获取NodeT的操作符重载
//								^ 调整CIteratorT::operator*(),支持通过此操作符直接获取NodeT存储的值
//////////////////////////////////////////////////////////////////

#ifndef __Iterator_h__
#define __Iterator_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename NodeT>
class CIteratorT
{
public:
	typedef NodeT node_t;
	typedef typename node_t::type_t type_t;

protected:
	node_t m_Node;

public:
	CIteratorT()
	{}
	CIteratorT(const CIteratorT& rIte)
	{ (*this) = rIte; }
	CIteratorT(const node_t& rNode)
	{ (*this) = rNode; }

public:
	EXP_INLINE node_t* operator->()
	{ return &m_Node; }
	EXP_INLINE node_t* Node()
	{ return &m_Node; }
	EXP_INLINE type_t& operator*()
	{ return m_Node.Val(); }

	EXP_INLINE CIteratorT& operator=(const CIteratorT& rIte)
	{ m_Node = rIte.m_Node; return (*this); }
	EXP_INLINE CIteratorT& operator=(const node_t& rNode)
	{ m_Node = rNode; return (*this); }

	EXP_INLINE BOOL operator==(const CIteratorT& rIte)
	{ return m_Node == rIte.m_Node; }
	EXP_INLINE BOOL operator==(const node_t& rNode)
	{ return m_Node == rNode; }
	EXP_INLINE BOOL operator!=(const CIteratorT& rIte)
	{ return m_Node != rIte.m_Node; }
	EXP_INLINE BOOL operator!=(const node_t& rNode)
	{ return m_Node != rNode; }

	EXP_INLINE CIteratorT& operator++()
	{
		m_Node.Next();
		return (*this);
	}
	EXP_INLINE CIteratorT operator++(int)
	{
		CIteratorT tmp(*this);
		m_Node.Next();
		return tmp;
	}
	EXP_INLINE CIteratorT& operator--()
	{
		m_Node.Prev();
		return (*this);
	}
	EXP_INLINE CIteratorT operator--(int)
	{
		CIteratorT tmp(*this);
		m_Node.Prev();
		return tmp;
	}
	EXP_INLINE CIteratorT& operator+=(long nOff)
	{
		if (nOff == 0) return (*this);
		m_Node.Next(nOff);
		return (*this);
	}
	EXP_INLINE CIteratorT& operator-=(long nOff)
	{
		if (nOff == 0) return (*this);
		m_Node.Prev(nOff);
		return (*this);
	}
	EXP_INLINE CIteratorT operator+(long nOff) const
	{
		if (nOff == 0) return (*this);
		CIteratorT tmp(*this);
		tmp->Next(nOff);
		return tmp;
	}
	EXP_INLINE CIteratorT operator-(long nOff) const
	{
		if (nOff == 0) return (*this);
		CIteratorT tmp(*this);
		tmp->Prev(nOff);
		return tmp;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Iterator_h__*/