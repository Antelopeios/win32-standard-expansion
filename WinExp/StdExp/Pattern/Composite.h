// Copyright 2012, ľͷ��
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
// Composite - ���
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-21
// Version:	1.0.0000.1532
//
// History:
//	- 1.0.0000.1532(2012-03-21)	@ ��ʼ����Compositeģʽ
//////////////////////////////////////////////////////////////////

#ifndef __Composite_h__
#define __Composite_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Container/List.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename TypeT>
interface ICompT
{
public:
	typedef TypeT type_t;
	typedef CListT<TypeT*> comp_t;
	typedef typename comp_t::iterator_t iter_t;

protected:
	TypeT*	m_Pare;		// ������ָ��
	comp_t	m_Cldr;		// ��������

public:
	ICompT(void)
		: m_Pare(NULL)
	{}

public:
	// ����ڲ�����
	TypeT* GetPare() const { return m_Pare; }
	comp_t& GetCldr() const { return m_Cldr; }

	// ����
	iter_t Find(TypeT* cmp) { return GetCldr().Find(cmp); }

	// ��Ͻӿ�
	BOOL Add(TypeT* cmp, iter_t ite = GetComp().Tail())
	{
		if (!cmp) return FALSE;
		// ��λ����
		iter_t it = Find(cmp);
		if (it != GetCldr().Tail()) return FALSE;
		// ����¶���
		if (cmp->m_Pare)
			cmp->m_Pare->Del(cmp);
		GetCldr().Add(cmp, ite);
		return TRUE;
	}
	BOOL Del(TypeT* cmp)
	{
		if (!cmp) return FALSE;
		// ��λ����
		iter_t it = Find(cmp);
		if (it == GetCldr().Tail()) return FALSE;
		// ɾ������
		GetCldr().Del(it);
		return TRUE;
	}
	TypeT* Pop(BOOL bLast = TRUE)
	{
		TypeT* cmp = NULL;
		if (GetCldr().Empty()) return cmp;
		if (bLast)
		{
			cmp = GetCldr().LastItem();
			GetCldr().PopLast();
		}
		else
		{
			cmp = GetCldr().HeadItem();
			GetCldr().PopHead();
		}
		return cmp;
	}
	void Clear()
	{ GetCldr().Clear(); }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Composite_h__*/
