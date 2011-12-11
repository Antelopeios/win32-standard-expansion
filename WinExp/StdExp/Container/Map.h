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
// Map - 映射
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-07-20
// Version:	1.0.0006.1540
//
// History:
//	- 1.0.0005.2230(2011-02-24)	# 修正迭代器获取接口内部实现的一处低级错误(static iterator_t iter(node_t(this));)
//								+ 添加CMapT::Del(iterator_t&)接口,支持由迭代器定位并删除结点
//								+ 添加CMapT::Null()接口,支持彻底清空CMapT
//	- 1.0.0006.1540(2011-07-20)	= 将CMapT::_Assoc与IPoolTypeT解耦,避免DLL中置换单例导致IPoolTypeT中的单例模板无法导出
//////////////////////////////////////////////////////////////////

#ifndef __Map_h__
#define __Map_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Container/Array.h"
#include "Container/List.h"
#include "Algorithm/Hash.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename HashT = CHash, typename AllocT = EXP_MEMORY_ALLOC>
struct _MapPolicyT;

template <typename KeyT, typename TypeT, typename PolicyT = _MapPolicyT<> >
class CMapT : public IContainerObjectT<TypeT, PolicyT, CMapT<KeyT, TypeT, PolicyT> >
{
public:
	typedef KeyT key_t;
	typedef typename policy_t::hash_t hash_t;

public:
	typedef struct
	{
		key_t	Key;
		type_t	Val;
	} pair_t;

	typedef struct _Assoc : public pair_t
	{
		DWORD	nHash;
		BOOL	bSet;

		_Assoc(BOOL s = TRUE)
			: nHash(0)
			, bSet(s)
		{}

		static _Assoc* Alloc()
		{ return alloc_t::Alloc<_Assoc>(); }
		void Free()
		{ alloc_t::Free(this); }
	} assoc_t;

	typedef CListT<assoc_t*, _ListPolicyT<alloc_t> > alist_t;
	typedef typename alist_t::iterator_t ite_t;
	typedef CArrayT<ite_t, _ArrayPolicyT<alloc_t> > table_t;

protected:
	alist_t	m_Assoc;
	table_t m_Table;

	// 定位
	BOOL Locate(_IN_ const key_t& Key, _OT_ ite_t& Iter, _OT_ DWORD* pHash = NULL)
	{
		DWORD hash = hash_t::HashKey<key_t>(Key);
		DWORD indx = hash % GetSize();
		BOOL ret = FALSE;
		ite_t ite = (m_Table[indx] + 1);
		ite_t end = (indx == GetSize() - 1) ? m_Assoc.Tail() : m_Table[indx + 1];
		for(; ite != end; ++ite)
			if (ite->Val() && 
				ite->Val()->nHash == hash && 
				ite->Val()->Key == Key)
			{
				ret = TRUE;
				break;
			}
		Iter = ite;
		if (pHash) (*pHash) = hash;
		return ret;
	}

public:
	CMapT(DWORD nSize = PolicyT::DEF_SIZE)
	{ SetSize(nSize); }
	~CMapT()
	{}

public:
	DWORD GetSize() const
	{ return m_Table.GetCount(); }
	void SetSize(DWORD nSize = PolicyT::DEF_SIZE)
	{
		if (nSize == 0 || GetSize() == nSize) return;
		Null();
		m_Table.SetSize(nSize);
		// 初始化链表
		for(DWORD i = 0; i < nSize; ++i)
			m_Assoc.Add(NULL);
		// 初始化Table
		for(ite_t ite = m_Assoc.Head(); ite != m_Assoc.Tail(); ++ite)
			m_Table.Add(ite);
	}
	DWORD GetCount() const
	{ return m_Assoc.GetCount() - GetSize(); }

	BOOL Empty() const
	{ return m_Assoc.Empty(); }
	void Clear()
	{
		DWORD size = GetSize();
		m_Table.Clear();
		SetSize(size);
	}
	void Null()
	{
		// 清空 Table 与 Assoc
		for(ite_t ite = m_Assoc.Head(); ite != m_Assoc.Tail(); ++ite)
			if (ite->Val()) ite->Val()->Free();
		m_Assoc.Clear();
		m_Table.Clear();
	}

	type_t& operator[](const key_t& Key)
	{ return GetAt(Key); }

	type_t& GetAt(const key_t& Key)
	{
		ite_t ite;
		DWORD hash = 0;
		if(!Locate(Key, ite, &hash))
		{
			assoc_t* assoc = assoc_t::Alloc();
			assoc->nHash = hash;
			assoc->Key	 = Key;
			m_Assoc.Add(assoc, ite);
			--ite;
		}
		ExAssert(ite->Val());
		return ite->Val()->Val;
	}

	iterator_t Locate(const key_t& Key)
	{
		iterator_t iter(node_t(this));
		ite_t ite;
		if (Locate(Key, ite))
		{
			iter->nIndx = ite;
			return iter;
		}
		else
			return Tail();
	}

	iterator_t Head() const
	{
		iterator_t ite(node_t(this));
		ite->nIndx = m_Assoc.Head();
		if (ite->nIndx != m_Assoc.Tail() && !ite->nIndx->Val())
			++ ite;
		return ite;
	}
	iterator_t Tail() const
	{
		iterator_t ite(node_t(this));
		ite->nIndx = m_Assoc.Tail();
		return ite;
	}
	iterator_t Last() const
	{
		iterator_t ite(node_t(this));
		ite->nIndx = m_Assoc.Last();
		if (ite->nIndx != m_Assoc.Tail() && !ite->nIndx->Val())
			-- ite;
		return ite;
	}
	type_t HeadItem() const
	{ return Head()->Val()->Val; }
	type_t LastItem() const
	{ return Last()->Val()->Val; }

	BOOL Add(const key_t& Key, const type_t& Val)
	{
		GetAt(Key) = Val;
		return TRUE;
	}
	BOOL Add(const pair_t& Pair)
	{ return Add(Pair.Key, Pair.Val); }

	BOOL Del(const key_t& Key)
	{
		ite_t ite;
		if (Locate(Key, ite))
		{
			ite->Val()->Free();
			return m_Assoc.Del(ite);
		}
		else
			return FALSE;
	}
	BOOL Del(iterator_t& Iter)
	{
		if (Empty()) return TRUE;
		if (!(Iter->InThis(this))) return FALSE;
		if (!(Iter->Index())) return FALSE;
		Iter->nIndx->Val()->Free();
		return m_Assoc.Del(Iter->nIndx);
	}
};

//////////////////////////////////////////////////////////////////

template <typename HashT/* = CHash*/, typename AllocT/* = EXP_MEMORY_ALLOC*/>
struct _MapPolicyT
{
	typedef AllocT alloc_t;
	typedef HashT hash_t;

	template <typename ContainerT>
	struct node_t
	{
		typedef ContainerT container_t;
		typedef typename container_t::key_t key_t;
		typedef typename container_t::type_t type_t;
		typedef typename container_t::pair_t pair_t;
		typedef typename container_t::ite_t ite_t;

		container_t* pCont;
		ite_t nIndx;

		node_t(container_t* p = NULL)
			: pCont(p)
		{}
		node_t(const container_t* p)
			: pCont((container_t*)p)
		{}

		key_t& Key() { return nIndx->Val()->Key; }
		type_t& Val() { return nIndx->Val()->Val; }

		BOOL InThis(container_t* cnt) { return pCont == cnt; }
		pair_t* Index() { return (pair_t*)(nIndx->Val()); }

		BOOL operator==(const node_t& node)
		{ return (pCont == node.pCont && (nIndx == node.nIndx)); }
		BOOL operator!=(const node_t& node)
		{ return (pCont != node.pCont || (nIndx != node.nIndx)); }

		void Next(long nOff = 1)
		{
			if (!pCont || nOff == 0) return;
			if (nOff > 0)
			{
				if (!nIndx->Val()) --nOff;
				while (nIndx != pCont->Tail()->nIndx)
				{
					if (0 < nOff)
					{
						if ((nIndx++)->Val())
							--nOff;
					}
					else
					{
						if (nIndx->Val())
							break;
						else
							++nIndx;
					}
				}
			}
			else
			{
				if (!nIndx->Val()) ++nOff;
				while (nIndx != pCont->Head()->nIndx)
				{
					if (0 > nOff)
					{
						if ((nIndx--)->Val())
							++nOff;
					}
					else
					{
						if (nIndx->Val())
							break;
						else
							--nIndx;
					}
				}
			}
		}
		void Prev(long nOff = 1)
		{ Next(-nOff); }
	};

	static const DWORD DEF_SIZE = 17;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Map_h__*/