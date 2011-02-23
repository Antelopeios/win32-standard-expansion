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
// Iterator - µü´úÆ÷
//
// Author:	Ä¾Í·ÔÆ
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-01-24
// Version:	1.0.0004.1700
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
protected:
	NodeT m_Node;

public:
	CIteratorT()
	{}
	CIteratorT(const CIteratorT& rIte)
	{ (*this) = rIte; }
	CIteratorT(const NodeT& rNode)
	{ (*this) = rNode; }

public:
	NodeT& Node()
	{ return m_Node; }
	NodeT* operator->()
	{ return &m_Node; }

	CIteratorT& operator=(const CIteratorT& rIte)
	{ m_Node = rIte.m_Node; return (*this); }
	CIteratorT& operator=(const NodeT& rNode)
	{ m_Node = rNode; return (*this); }

	bool operator==(const CIteratorT& rIte)
	{ return m_Node == rIte.m_Node; }
	bool operator==(const NodeT& rNode)
	{ return m_Node == rNode; }
	bool operator!=(const CIteratorT& rIte)
	{ return m_Node != rIte.m_Node; }
	bool operator!=(const NodeT& rNode)
	{ return m_Node != rNode; }

	CIteratorT& operator++()
	{
		m_Node.Next();
		return (*this);
	}
	CIteratorT operator++(int)
	{
		CIteratorT tmp(*this);
		m_Node.Next();
		return tmp;
	}
	CIteratorT& operator--()
	{
		m_Node.Prev();
		return (*this);
	}
	CIteratorT operator--(int)
	{
		CIteratorT tmp(*this);
		m_Node.Prev();
		return tmp;
	}
	CIteratorT& operator+=(long nOff)
	{
		if (nOff == 0) return (*this);
		m_Node.Next(nOff);
		return (*this);
	}
	CIteratorT& operator-=(long nOff)
	{
		if (nOff == 0) return (*this);
		m_Node.Prev(nOff);
		return (*this);
	}
	CIteratorT operator+(long nOff) const
	{
		if (nOff == 0) return (*this);
		CIteratorT tmp(*this);
		tmp->Next(nOff);
		return tmp;
	}
	CIteratorT operator-(long nOff) const
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