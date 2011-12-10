// Copyright 2011, ľͷ��
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
// List - ����
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-07-20
// Version:	1.0.0013.1540
//
// History:
//	- 1.0.0010.1600(2011-02-24)	# ������������ȡ�ӿ��ڲ�ʵ�ֵ�һ���ͼ�����(static iterator_t iter(node_t(this));)
//	- 1.0.0011.1742(2011-06-02)	# ����CListT::Del()������ĵ�����ΪTailʱ�ᵼ������Ľ��
//								# ����CListT::AddList()��ĵ�������ֵ����
//	- 1.0.0012.2025(2011-07-11)	# ����CListT::operator[]()�޷�˳��ͨ�����������
//	- 1.0.0013.1540(2011-07-20)	= ��CListT::_Block��IPoolTypeT����,����DLL���û���������IPoolTypeT�еĵ���ģ���޷�����
//////////////////////////////////////////////////////////////////

#ifndef __List_h__
#define __List_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/MemAlloc.h"
#include "Container/ContainerObject.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename AllocT = EXP_MEMORY_ALLOC>
struct _ListPolicyT;

template <typename TypeT, typename PolicyT = _ListPolicyT<> >
class CListT : public IContainerObjectT<TypeT, PolicyT, CListT<TypeT, PolicyT> >
{
public:
	typedef struct _Block
	{
		type_t	Buff;	// ���ݿ�
		_Block*	pPrev;	// ��һ�����
		_Block*	pNext;	// ��һ�����
		_Block()
			: pPrev(NULL)
			, pNext(NULL)
		{}

		static _Block* Alloc()
		{ return alloc_t::Alloc<_Block>(); }
		void Free()
		{ alloc_t::Free(this); }
	} block_t;

protected:
	block_t*	m_pHead;	// ����ͷָ��
	block_t*	m_pLast;	// ����βָ��
	DWORD		m_nCont;	// ������

public:
	CListT()
		: m_pHead(NULL)
		, m_pLast(NULL)
		, m_nCont(0)
	{}

	CListT(block_t* pList)
		: m_pHead(NULL)
		, m_pLast(NULL)
		, m_nCont(0)
	{ SetList(pList); }
	CListT(const CListT& List)
		: m_pHead(NULL)
		, m_pLast(NULL)
		, m_nCont(0)
	{ SetList(List); }

	~CListT()
	{}

public:
	DWORD GetCount() const
	{ return m_nCont; }

	bool IsNull() const
	{ return (m_pHead == NULL); }
	bool Empty() const
	{ return (m_nCont == 0); }
	void Clear()
	{
		while (m_pHead)
		{
			if (m_pHead->pNext)
			{
				m_pHead = m_pHead->pNext;
				m_pHead->pPrev->Free();
			} else
			{
				m_pHead->Free();
				break;
			}
		}
		m_pHead = m_pLast = NULL;
		m_nCont = 0;
	}

	type_t& operator[](INT_PTR nIndex)
	{ return GetAt(nIndex); }
	const type_t& operator[](INT_PTR nIndex) const
	{ return GetAt(nIndex); }

	type_t& GetAt(DWORD nIndex)
	{
		ExAssert(nIndex < m_nCont);
		block_t* pBlock = m_pHead;
		while (pBlock && nIndex--)
			pBlock = pBlock->pNext;
		return pBlock->Buff;
	}
	const type_t& GetAt(DWORD nIndex) const
	{
		ExAssert(nIndex < m_nCont);
		block_t* pBlock = m_pHead;
		while (pBlock && nIndex--)
			pBlock = pBlock->pNext;
		return pBlock->Buff;
	}

	CListT& SetList(block_t* pList)
	{
		Clear();
		AddList(pList);
		return (*this);
	}
	CListT& SetList(const CListT& List)
	{ return SetList(List.m_pHead); }

	CListT& operator=(block_t* pList)
	{ return SetList(pList); }
	CListT& operator=(const CListT& List)
	{ return SetList(List); }

	iterator_t Head() const
	{
		iterator_t ite(node_t(this));
		ite->nIndx = m_pHead;
		return ite;
	}
	iterator_t Tail() const
	{
		iterator_t ite(node_t(this));
		ite->nIndx = NULL;
		return ite;
	}
	iterator_t Last() const
	{
		iterator_t ite(node_t(this));
		ite->nIndx = m_pLast;
		return ite;
	}
	type_t& HeadItem() const
	{ return m_pHead->Buff; }
	type_t& LastItem() const
	{ return m_pLast->Buff; }

	bool AddList(block_t* pList, iterator_t Iter)
	{
		if (!pList) return true;
		if (!(Iter->InThis(this))) return false;
		// ������ʱ����
		block_t* pTempH = NULL;
		block_t* pTempL = NULL;
		block_t* pBlock = NULL;
		block_t* pItLst = pList;
		while (pItLst)
		{
			pBlock = block_t::Alloc();
			if (pTempL)
			{
				pTempL->pNext = pBlock;
				pBlock->pPrev = pTempL;
				pTempL = pBlock;
			} else
				pTempH = pTempL = pBlock;
			pBlock->Buff = pItLst->Buff;
			++m_nCont;
			pItLst = pItLst->pNext;
		}
		// ���Ԫ��
		block_t* pIndex = Iter->Index();
		if (pIndex)
		{
			pTempH->pPrev = pIndex->pPrev;
			if (pIndex->pPrev)
				pIndex->pPrev->pNext = pTempH;
			pTempL->pNext = pIndex;
			pIndex->pPrev = pTempL;
			if (pIndex == m_pHead) m_pHead = pTempH;
		} else
		{
			if (m_pLast)
			{
				m_pLast->pNext = pTempH;
				pTempL->pPrev = m_pLast;
				m_pLast = pTempL;
			} else
			{
				m_pHead = pTempH;
				m_pLast = pTempL;
			}
		}
		return true;
	}
	bool AddList(block_t* pList)
	{ return AddList(pList, Tail()); }
	bool AddList(const CListT& List, iterator_t Iter)
	{ return AddList(List.m_pHead, Iter); }
	bool AddList(const CListT& List)
	{ return AddList(List, Tail()); }

	bool Add(const type_t& Item, iterator_t& Iter)
	{
		if (!(Iter->InThis(this))) return false;
		// ���Ԫ��
		block_t* pBlock = block_t::Alloc();
		block_t* pIndex = Iter->Index();
		if (pIndex)
		{
			pBlock->pPrev = pIndex->pPrev;
			if (pIndex->pPrev)
				pIndex->pPrev->pNext = pBlock;
			pBlock->pNext = pIndex;
			pIndex->pPrev = pBlock;
			if (pIndex == m_pHead) m_pHead = pBlock;
		} else
		{
			if (m_pLast)
			{
				m_pLast->pNext = pBlock;
				pBlock->pPrev = m_pLast;
				m_pLast = pBlock;
			} else
				m_pHead = m_pLast = pBlock;
		}
		pBlock->Buff = Item;
		++m_nCont;
		return true;
	}
	bool Add(const type_t& Item)
	{ return Add(Item, Tail()); }

	bool Del(iterator_t& iteStt, iterator_t& iteEnd)
	{
		if (Empty()) return true;
		if (!(iteStt->InThis(this))) return false;
		if (!(iteEnd->InThis(this))) return false;
		if (iteStt == Tail()) return false;
		if (iteStt == iteEnd)
		{
			// ��λ��ɾ�����
			block_t* pIndex = iteStt->Index();
			if(!pIndex) pIndex = m_pLast;
			// ������������
			if (pIndex == m_pHead) m_pHead = m_pHead->pNext;
			if (pIndex == m_pLast) m_pLast = m_pLast->pPrev;
			// �ƶ�����������
			ExAssert(pIndex);
			if (pIndex->pPrev) pIndex->pPrev->pNext = pIndex->pNext;
			if (pIndex->pNext) pIndex->pNext->pPrev = pIndex->pPrev;
			pIndex->Free();
			--m_nCont;
		} else
		{
			// ��λ��ɾ�����
			block_t* pIdxStt = iteStt->Index();
			if(!pIdxStt) pIdxStt = m_pHead;
			block_t* pIdxEnd = iteEnd->Index();
			// ������������
			ExAssert(pIdxStt);
			ExAssert(pIdxEnd != m_pHead);
			block_t* pIndex = pIdxStt;				// �����ɾ������ͷ��
			if (pIndex == m_pHead) m_pHead = pIdxEnd;
			if (pIndex == m_pLast) m_pLast = m_pLast->pPrev;
			pIdxStt = pIdxStt->pPrev;				// ��ǰ��һλ
			if (pIdxStt) pIdxStt->pNext = pIdxEnd;	// ���Ӳ���Ҫɾ���Ĳ���
			if (pIdxEnd) pIdxEnd->pPrev = pIdxStt;
			// �ƶ�����������
			while (pIndex && pIndex != pIdxEnd && m_nCont--)
			{
				if (pIndex->pNext)
				{
					pIndex = pIndex->pNext;
					pIndex->pPrev->Free();
				} else
				{
					pIndex->Free();
					break;
				}
			}
		}
		return true;
	}
	bool Del(iterator_t& ite)
	{ return Del(ite, ite); }
};

//////////////////////////////////////////////////////////////////

template <typename AllocT/* = EXP_MEMORY_ALLOC*/>
struct _ListPolicyT
{
	typedef AllocT alloc_t;

	template <typename ContainerT>
	struct node_t
	{
		typedef ContainerT container_t;
		typedef typename container_t::type_t type_t;
		typedef typename container_t::block_t block_t;

		container_t* pCont;
		block_t* nIndx;

		node_t(container_t* p = NULL)
			: pCont(p)
			, nIndx(NULL)
		{}
		node_t(const container_t* p)
			: pCont((container_t*)p)
			, nIndx(NULL)
		{}

		type_t& Val() { return nIndx->Buff; }

		bool InThis(container_t* cnt) { return pCont == cnt; }
		block_t* Index() { return nIndx; }

		bool operator==(const node_t& node)
		{ return (memcmp(this, &node, sizeof(node_t)) == 0); }
		bool operator!=(const node_t& node)
		{ return (memcmp(this, &node, sizeof(node_t)) != 0); }

		void Next(long nOff = 1)
		{
			if (!pCont || nOff == 0) return;
			if (nOff > 0)
				while (nIndx && 0 < nOff--) nIndx = nIndx->pNext;
			else
			{
				if(!nIndx)
				{
					nIndx = pCont->Last()->Index();
					++nOff;
				}
				ExAssert(nIndx);
				while (nIndx != pCont->Head()->Index() && 0 > nOff++)
					nIndx = nIndx->pPrev;
			}
		}
		void Prev(long nOff = 1)
		{ Next(-nOff); }
	};
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__List_h__*/