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
// Tree - 树
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-06-15
// Version:	1.0.0005.1651
//
// History:
//	- 1.0.0000.1336(2011-06-01)	@ 开始构建Tree
//	- 1.0.0001.1955(2011-06-02)	@ Tree的基本功能构建完毕
//	- 1.0.0002.1830(2011-06-12)	# 修正CTreeT::item_t中导致无法正确编译通过的一些问题
//								= CTreeT::Add()将通过参数返回新结点的迭代器位置
//	- 1.0.0003.1120(2011-06-13)	= 调整CTreeT策略,迭代器支持获取父节点迭代器与子节点迭代器列表
//	- 1.0.0004.1650(2011-06-14)	# 修正CTreeT::_Item内部的alloc_t单例析构时内存异常的问题
//								# 修正CTreeT::Del()直接删除根节点时,不会清空根节点标记指针的问题
//	- 1.0.0005.1651(2011-06-15)	# 修正CTreeT::_Item内部调用finder_t::Find()时不规范导致的编译错误
//								# 修正当迭代器已移到Last的位置时,_TreePolicyT::node_t::Next()会将迭代器重新送回Head位置的问题(正常应该是Tail)
//////////////////////////////////////////////////////////////////

#ifndef __Tree_h__
#define __Tree_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Container/List.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename AllocT = EXP_MEMORY_ALLOC>
struct _TreePolicyT;

template <typename TypeT, typename PolicyT = _TreePolicyT<> >
class CTreeT : public IContainerObjectT<TypeT, PolicyT, CTreeT<TypeT, PolicyT> >
{
public:
	typedef struct _Item
	{
		typedef CListT<_Item*, _ListPolicyT<alloc_t> > list_t;
		typedef typename list_t::iterator_t ite_t;

		_Item* Pare;
		list_t Chdr;

		type_t Val;

		static _Item* Alloc()
		{ return ExMem::Alloc<_Item>(); }
		void Free()
		{ ExMem::Free(static_cast<_Item*>(this)); }

		_Item()
			: Pare(NULL)
		{}
		_Item(const _Item& item)
			: Pare(NULL)
		{ (*this) = item; }

		~_Item()
		{
			// 从父节点中移除自身
			if (Pare)
			{
				Pare->Chdr.Replace((_Item*)this);
				Pare = NULL;
			}
			// 清理子节点树
			ClrChdr();
		}

		_Item& operator=(const _Item& item)
		{
			ClrChdr();
			Val = item.Val;
			for(ite_t ite = item.Chdr.Head(); ite != item.Chdr.Tail(); ++ite)
			{
				_Item* itm = Alloc();
				itm->Pare = this;
				(*itm) = (*(*ite));	// 递归赋值
				Chdr.Add(itm);
			}
		}

		DWORD GetCount(DWORD nStart = 0)
		{
			++nStart;
			for(ite_t ite = Chdr.Head(); ite != Chdr.Tail(); ++ite)
				nStart = (*ite)->GetCount(nStart);
			return nStart;
		}

		_Item* GetPareNext()
		{
			if (!Pare) return NULL;
			_Item* item = this;
			ite_t ite = list_t::finder_t::Find(Pare->Chdr, item);
			if (ite == Pare->Chdr.Tail()) return NULL;
			if ((++ite) != Pare->Chdr.Tail()) return *ite;
			// Tail, 递归遍历
			return Pare->GetPareNext();
		}
		_Item* GetChdrLast()
		{
			if (Chdr.Empty()) return this;
			// 递归遍历
			return Chdr.LastItem()->GetChdrLast();
		}
		_Item* GetParePrev()
		{
			if (!Pare) return NULL;
			_Item* item = this;
			ite_t ite = list_t::finder_t::Find(Pare->Chdr, item);
			if (ite == Pare->Chdr.Tail()) return NULL;
			if (ite == Pare->Chdr.Head()) return Pare;
			return (*(--ite))->GetChdrLast();
		}

		void AddChdr(_Item* item, _Item* repl = NULL)
		{
			if (!item) return;
			ite_t ite = Chdr.Tail();
			if (repl)
				ite = list_t::finder_t::Find(Chdr, repl);
			Chdr.Add(item, ite);
			item->Pare = this;
		}
		void DelChdr(_Item* item)
		{
			if (!item) return;
			Chdr.Replace(item);
			item->Pare = NULL;
		}
		void ClrChdr()
		{
			// 清理子节点树
			if (Chdr.Empty()) return;
			for(ite_t ite = Chdr.Head(); ite != Chdr.Tail(); ++ite)
			{
				(*ite)->Pare = NULL;
				(*ite)->Free();
			}
			Chdr.Clear();
		}
	} item_t;

	typedef typename item_t::list_t list_t;
	typedef typename item_t::ite_t ite_t;

protected:
	item_t*	m_pTree;	// 树根指针
	DWORD	m_nCont;	// 结点个数

public:
	CTreeT()
		: m_pTree(NULL)
		, m_nCont(0)
	{}

	CTreeT(item_t* tTree)
		: m_pTree(NULL)
		, m_nCont(0)
	{ SetTree(tTree); }
	CTreeT(const CTreeT& tTree)
		: m_pTree(NULL)
		, m_nCont(0)
	{ SetTree(tTree); }

	~CTreeT()
	{}

public:
	DWORD GetCount() const
	{ return m_nCont; }

	bool IsNull() const
	{ return (m_pTree == NULL); }
	bool Empty() const
	{ return (m_nCont == 0); }

	void Clear()
	{
		if (m_pTree)
			m_pTree->Free();
		m_pTree = NULL;
		m_nCont = 0;
	}

	CTreeT& SetTree(item_t* tTree)
	{
		Clear();
		AddTree(tTree);
		return (*this);
	}
	CTreeT& SetTree(const CTreeT& tTree)
	{ return SetTree(tTree.m_pTree); }

	CTreeT& operator=(item_t* tTree)
	{ return SetTree(tTree); }
	CTreeT& operator=(const CTreeT& tTree)
	{ return SetTree(tTree); }

	iterator_t& Head()
	{
		static iterator_t iter;
		iter = node_t(this);
		iter->nIndx = m_pTree;
		return iter;
	}
	iterator_t& Tail()
	{
		static iterator_t iter;
		iter = node_t(this);
		iter->nIndx = NULL;
		return iter;
	}
	iterator_t& Last()
	{
		static iterator_t iter;
		iter = node_t(this);
		iter->nIndx = m_pTree->GetChdrLast();
		return iter;
	}
	type_t& HeadItem()
	{ return m_pTree->Val; }
	type_t& LastItem()
	{ return Last()->Val(); }

	/*
		树连接算法,默认将新树连接到Iter的子节点上
		若Iter为Tail,新树将挂接到老树的最后
	*/
	bool LinkTree(item_t* tTree, iterator_t& Iter)
	{	// 连接树
		if (!tTree) return true;
		if (!(Iter->InThis(this))) return false;
		m_nCont += tTree->GetCount();
		item_t* item = Iter->Index();
		if (item)
			item->AddChdr(tTree);
		else
		{
			if (m_pTree)
				m_pTree->GetChdrLast()->AddChdr(tTree);
			else
				m_pTree = tTree;
		}
		Iter->nIndx = tTree;
		return true;
	}

	bool AddTree(item_t* tTree, iterator_t Iter)
	{
		if (!tTree) return true;
		if (!(Iter->InThis(this))) return false;
		// 构造临时树
		item_t* temp = item_t::Alloc();
		(*temp) = (*tTree);
		// 添加树
		return LinkTree(temp, Iter);
	}
	bool AddTree(item_t* tTree)
	{ return AddTree(tTree, Tail()); }
	bool AddTree(const CTreeT& tTree, iterator_t Iter)
	{ return AddTree(tTree.m_pTree, Iter); }
	bool AddTree(const CTreeT& tTree)
	{ return AddTree(tTree, Tail()); }

	bool Add(const type_t& Item, iterator_t& Iter)
	{
		if (!(Iter->InThis(this))) return false;
		// 构造临时元素
		item_t* temp = item_t::Alloc();
		temp->Val = Item;
		// 添加元素
		return LinkTree(temp, Iter);
	}
	bool Add(const type_t& Item)
	{ return Add(Item, Tail()); }

	/*
		树删除算法
		头尾迭代器一致,则将迭代器指向的树结点及其子节点完全删除
		不一致,则头迭代器到尾迭代器之间的结点及其子节点将被删除
		尾迭代器将代替头迭代器被连接到头迭代器父节点下作为其子节点

		例如,删除根节点,新的根节点为其下第二个子节点:
		tree_t::iterator_t ite_head = tree.Head();
		tree_t::iterator_t ite_newh = ite_head->Children()[1];
		tree.Del(ite_head, ite_newh);
	*/
	bool Del(iterator_t& iteStt, iterator_t& iteEnd)
	{
		if (Empty()) return true;
		if (!(iteStt->InThis(this))) return false;
		if (!(iteEnd->InThis(this))) return false;
		if (iteStt == Tail()) return false;
		if (iteStt == iteEnd)
		{
			item_t* stt_inx = iteStt->Index();
			if (stt_inx == m_pTree)
				Clear();
			else
			{
				m_nCont -= stt_inx->GetCount();
				stt_inx->Free();
			}
		}
		else
		{
			item_t* stt_inx = iteStt->Index();
			item_t* end_inx = iteEnd->Index();
			if(!end_inx) end_inx = m_pTree->GetChdrLast();
			if (stt_inx->Pare)
			{
				stt_inx->Pare->AddChdr(end_inx, stt_inx);
				stt_inx->Pare->DelChdr(stt_inx);
			}
			else
			{
				ExAssert(end_inx->Pare);
				end_inx->Pare->DelChdr(end_inx);
				m_pTree = end_inx;
			}
			end_inx->GetChdrLast()->AddChdr(stt_inx);
		}
		return true;
	}
	bool Del(iterator_t& ite)
	{ return Del(ite, ite); }
};

//////////////////////////////////////////////////////////////////

template <typename AllocT/* = EXP_MEMORY_ALLOC*/>
struct _TreePolicyT
{
	typedef AllocT alloc_t;

	template <typename ContainerT>
	struct node_t
	{
		typedef ContainerT container_t;

		typedef typename container_t::type_t type_t;
		typedef typename container_t::item_t item_t;
		typedef typename container_t::list_t list_t;
		typedef typename container_t::ite_t ite_t;

		typedef typename container_t::iterator_t iter_t;
		typedef CListT<iter_t> iter_list_t;
		typedef typename iter_list_t::iterator_t iterator_t;

		container_t* pCont;
		item_t* nIndx;

		node_t(container_t* p = NULL)
			: pCont(p)
			, nIndx(NULL)
		{}

		type_t& Val() { return nIndx->Val; }

		bool InThis(container_t* cnt) { return pCont == cnt; }
		item_t* Index() { return nIndx; }

		const iter_t& Parent()
		{
			static iter_t iter;
			iter = node_t(pCont);
			iter->nIndx = nIndx ? nIndx->Pare : NULL;
			return iter;
		}
		const iter_list_t& Children()
		{
			static iter_list_t list;
			list.Clear();
			for(ite_t ite = nIndx->Chdr.Head(); ite != nIndx->Chdr.Tail(); ++nIndx->Chdr)
			{
				if (!(*ite)) continue;
				iter_t iter(node_t(pCont));
				iter->nIndx = (*ite);
				list.Add(iter);
			}
			return list;
		}

		bool operator==(const node_t& node)
		{ return (memcmp(this, &node, sizeof(node_t)) == 0); }
		bool operator!=(const node_t& node)
		{ return (memcmp(this, &node, sizeof(node_t)) != 0); }

		// 深度优先遍历
		void Next(long nOff = 1)
		{
			if (!pCont || nOff == 0) return;
			if (nOff > 0)
			{
				ExAssert(nIndx);
				if (nIndx->Chdr.Empty())
					nIndx = nIndx->GetPareNext();
				else
					nIndx = nIndx->Chdr.HeadItem();
				// 递归遍历
				if (nOff > 1) Next(nOff - 1);
			}
			else
			{
				if (nIndx)
					nIndx = nIndx->GetParePrev();
				else
					nIndx = pCont->Last()->Index();
				// 递归遍历
				if (nOff < -1) Next(nOff + 1);
			}
		}
		void Prev(long nOff = 1)
		{ Next(-nOff); }
	};
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Tree_h__*/