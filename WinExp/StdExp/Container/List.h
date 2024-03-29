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
// List - 链表
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-07-20
// Version:	1.0.0013.1540
//
// History:
//	- 1.0.0010.1600(2011-02-24)	# 修正迭代器获取接口内部实现的一处低级错误(static iterator_t iter(node_t(this));)
//	- 1.0.0011.1742(2011-06-02)	# 修正CListT::Del()当传入的迭代器为Tail时会导致意外的结果
//								# 修正CListT::AddList()里的迭代器赋值错误
//	- 1.0.0012.2025(2011-07-11)	# 修正CListT::operator[]()无法顺利通过编译的问题
//	- 1.0.0013.1540(2011-07-20)	= 将CListT::_Block与IPoolTypeT解耦,避免DLL中置换单例导致IPoolTypeT中的单例模板无法导出
//////////////////////////////////////////////////////////////////

#ifndef __List_h__
#define __List_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/OverNew.h"
#include "Container/ContainerObject.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

struct _ListPolicy
{
	template <typename ContainerT>
	struct node_t
	{
		typedef ContainerT container_t;
		typedef typename container_t::type_t type_t;
		typedef typename container_t::block_t block_t;

		container_t* pCont;
		block_t* nIndx;

		node_t(container_t* p = NULL, block_t* b = NULL)
			: pCont(p)
			, nIndx(b)
		{}
		node_t(const container_t* p, block_t* b = NULL)
			: pCont((container_t*)p)
			, nIndx(b)
		{}

		type_t& Val() { return nIndx->Buff; }

		BOOL InThis(container_t* cnt) { return pCont == cnt; }
		block_t* Index() { return nIndx; }

		BOOL operator==(const node_t& node)
		{ return (memcmp(this, &node, sizeof(node_t)) == 0); }
		BOOL operator!=(const node_t& node)
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

template <typename TypeT, typename PolicyT = _ListPolicy>
class CListT : public IContainerObjectT<TypeT, PolicyT, CListT<TypeT, PolicyT> >
{
public:
	typedef struct _Block
	{
		type_t	Buff;	// 数据块
		_Block*	pPrev;	// 上一个结点
		_Block*	pNext;	// 下一个结点
		_Block()
			: pPrev(NULL)
			, pNext(NULL)
		{}

		static _Block* Alloc()
		{ return dbnew(_Block); }
		void Free()
		{ del(this); }
	} block_t;

protected:
	block_t*	m_pHead;	// 链表头指针
	block_t*	m_pLast;	// 链表尾指针
	DWORD		m_nCont;	// 结点个数

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

	BOOL IsNull() const
	{ return (m_pHead == NULL); }
	BOOL Empty() const
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
		if (pList == m_pHead) return (*this);
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
		return node_t(this, m_pHead);
	}
	iterator_t Tail() const
	{
		return node_t(this, NULL);
	}
	iterator_t Last() const
	{
		return node_t(this, m_pLast);
	}
	type_t& HeadItem() const
	{ return m_pHead->Buff; }
	type_t& LastItem() const
	{ return m_pLast->Buff; }

	BOOL AddList(block_t* pList, iterator_t Iter)
	{
		if (!pList) return TRUE;
		if (!(Iter->InThis(this))) return FALSE;
		// 构造临时链表
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
		// 添加元素
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
		return TRUE;
	}
	BOOL AddList(block_t* pList)
	{ return AddList(pList, Tail()); }
	BOOL AddList(const CListT& List, iterator_t Iter)
	{ return AddList(List.m_pHead, Iter); }
	BOOL AddList(const CListT& List)
	{ return AddList(List, Tail()); }

	BOOL Add(const type_t& Item, iterator_t& Iter)
	{
		if (!(Iter->InThis(this))) return FALSE;
		// 添加元素
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
		return TRUE;
	}
	BOOL Add(const type_t& Item)
	{ return Add(Item, Tail()); }

	BOOL Del(iterator_t& iteStt, iterator_t& iteEnd)
	{
		if (Empty()) return TRUE;
		if (!(iteStt->InThis(this))) return FALSE;
		if (!(iteEnd->InThis(this))) return FALSE;
		if (iteStt == Tail()) return FALSE;
		if (iteStt == iteEnd)
		{
			// 定位待删除结点
			block_t* pIndex = iteStt->Index();
			if(!pIndex) pIndex = m_pLast;
			// 重新链接链表
			if (pIndex == m_pHead) m_pHead = m_pHead->pNext;
			if (pIndex == m_pLast) m_pLast = m_pLast->pPrev;
			// 移动并销毁数据
			ExAssert(pIndex);
			if (pIndex->pPrev) pIndex->pPrev->pNext = pIndex->pNext;
			if (pIndex->pNext) pIndex->pNext->pPrev = pIndex->pPrev;
			pIndex->Free();
			--m_nCont;
		} else
		{
			// 定位待删除结点
			block_t* pIdxStt = iteStt->Index();
			if(!pIdxStt) pIdxStt = m_pHead;
			block_t* pIdxEnd = iteEnd->Index();
			// 重新链接链表
			ExAssert(pIdxStt);
			ExAssert(pIdxEnd != m_pHead);
			block_t* pIndex = pIdxStt;				// 保存待删除链表头部
			if (pIndex == m_pHead) m_pHead = pIdxEnd;
			if (pIndex == m_pLast) m_pLast = m_pLast->pPrev;
			pIdxStt = pIdxStt->pPrev;				// 向前移一位
			if (pIdxStt) pIdxStt->pNext = pIdxEnd;	// 链接不需要删除的部分
			if (pIdxEnd) pIdxEnd->pPrev = pIdxStt;
			// 移动并销毁数据
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
		return TRUE;
	}
	BOOL Del(iterator_t& ite)
	{ return Del(ite, ite); }

	BOOL PushHead(const type_t& Item)
	{ return Add(Item, Head()); }
	BOOL PushLast(const type_t& Item)
	{ return Add(Item); }
	BOOL PopHead()
	{ return Del(Head()); }
	BOOL PopLast()
	{ return Del(Last()); }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__List_h__*/