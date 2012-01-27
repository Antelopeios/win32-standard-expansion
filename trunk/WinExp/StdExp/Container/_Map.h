// Copyright 2011-2012, 木头云
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
// Date:	2012-01-27
// Version:	1.0.0008.1733
//
// History:
//	- 1.0.0005.2230(2011-02-24)	# 修正迭代器获取接口内部实现的一处低级错误(static iterator_t iter(node_t(this));)
//								+ 添加CMapT::Del(iterator_t&)接口,支持由迭代器定位并删除结点
//								+ 添加CMapT::Null()接口,支持彻底清空CMapT
//	- 1.0.0006.1540(2011-07-20)	= 将CMapT::_Assoc与IPoolTypeT解耦,避免DLL中置换单例导致IPoolTypeT中的单例模板无法导出
//	- 1.0.0007.1712(2012-01-20)	# 修正CMapT析构时没有自动释放内部申请的结点内存而导致的内存泄漏
//	- 1.0.0008.1733(2012-01-27)	^ 采用POD数组及单链表优化CMapT的综合性能
//								+ _MapPolicyT支持直接通过模板参数调整默认大小
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

template <DWORD SizeT = 17, typename HashT = CHash, typename AllocT = EXP_MEMORY_ALLOC>
struct _MapPolicyT;

template <typename KeyT, typename TypeT, typename PolicyT = _MapPolicyT<> >
class CMapT : public IContainerObjectT<TypeT, PolicyT, CMapT<KeyT, TypeT, PolicyT> >
{
	friend typename node_t;

public:
	typedef KeyT key_t;
	typedef typename policy_t::hash_t hash_t;

public:
	typedef struct _Pair
	{
		const key_t	Key;
		type_t Val;

	protected:
		_Pair(const key_t& key) : Key(key)	{}
	} pair_t;

	typedef struct _Assoc : public _Pair
	{
		DWORD nHash;
		_Assoc* pNext;

		_Assoc(const key_t& key)
			: _Pair(key)
			, nHash(0)
			, pNext(NULL)
		{}

		EXP_INLINE static _Assoc* Alloc(IObjPool* pool, const key_t& key)
		{
			_Assoc* pa = (_Assoc*)alloc_t::Alloc(sizeof(_Assoc));
			pa->_Assoc::_Assoc(key);
			return pa;
		}
		EXP_INLINE void Free(IObjPool* pool)
		{
			this->~_Assoc();
			alloc_t::Free(this);
		}
	} assoc_t;

	typedef CBlockPoolT<_ObjPoolPolicyT<sizeof(assoc_t), alloc_t> > pool_t;

protected:
	assoc_t** m_Table;
	DWORD m_nSize, m_nCount;
	pool_t* m_Pool;

	// 定位
	assoc_t* Locate(_IN_ const key_t& Key, _OT_ assoc_t*& pLast, 
					_OT_ DWORD* pIndx = NULL, _OT_ DWORD* pHash = NULL)
	{
		pLast = NULL;
		if (IsNull() || Empty())
		{
			if (pHash || pIndx)
			{
				DWORD hash = hash_t::HashKey<key_t>(Key);
				if (pHash) (*pHash) = hash;
				if (pIndx) (*pIndx) = hash % m_nSize;
			}
			return NULL;
		}
		DWORD hash = hash_t::HashKey<key_t>(Key);
		DWORD indx = hash % m_nSize;
		if (pHash) (*pHash) = hash;
		if (pIndx) (*pIndx) = indx;
		assoc_t* assoc = m_Table[indx];
		for(; assoc; pLast = assoc, assoc = assoc->pNext)
			if (assoc->nHash == hash && assoc->Key == Key) break;
		return assoc;
	}
	assoc_t* Locate(_IN_ const key_t& Key, _OT_ DWORD* pIndx, _OT_ DWORD* pHash = NULL)
	{
		assoc_t* lst;
		return Locate(Key, lst, pIndx, pHash);
	}

public:
	CMapT(DWORD nSize = PolicyT::DEF_SIZE)
		: m_Table(NULL)
		, m_nSize(0), m_nCount(0)
		, m_Pool(NULL/*alloc_t::Alloc<pool_t>()*/)
	{ SetSize(nSize); }
	~CMapT()
	{
		Null();
		//alloc_t::Free(m_Pool);
	}

public:
	DWORD GetSize() const
	{ return m_nSize; }
	void SetSize(DWORD nSize = PolicyT::DEF_SIZE)
	{
		if (m_nSize == nSize) return;
		if (0 == nSize)
			Null();
		else
		{
			Clear();
			m_Table = alloc_t::ReAlloc<assoc_t*>(m_Table, nSize);
			ZeroMemory(m_Table, sizeof(assoc_t*) * nSize);
			m_nSize = nSize;
		}
	}
	DWORD GetCount() const
	{ return m_nCount; }

	BOOL Empty() const
	{ return (m_nCount == 0); }
	void Clear()
	{
		if (IsNull() || Empty()) return;
		for(DWORD i = 0; i < m_nSize; ++i)
			for(assoc_t* asc = m_Table[i], *lst; lst = asc; asc = asc->pNext, lst->Free(m_Pool));
		ZeroMemory(m_Table, sizeof(assoc_t*) * m_nSize);
		m_nCount = 0;
	}

	BOOL IsNull() const
	{
		return (m_nSize == 0 && !m_Table);
	}
	void Null()
	{
		Clear();
		alloc_t::Free(m_Table);
		m_Table = NULL;
		m_nSize = 0;
	}

	type_t& operator[](const key_t& Key)
	{ return GetAt(Key); }

	type_t& GetAt(const key_t& Key)
	{
		ExAssert(!IsNull());
		DWORD inx = 0, hash = 0;
		assoc_t* asc = Locate(Key, &inx, &hash);
		if(!asc)
		{
			asc = assoc_t::Alloc(m_Pool, Key);
			asc->nHash = hash;
			asc->pNext = m_Table[inx];
			m_Table[inx] = asc;
			++m_nCount;
		}
		ExAssert(asc);
		return asc->Val;
	}

	iterator_t Locate(const key_t& Key)
	{
		iterator_t iter(node_t(this));
		iter->nIndx = Locate(Key, NULL);
		return iter;
	}

	iterator_t Head() const
	{
		if (IsNull() || Empty()) return Tail();
		DWORD i = 0;
		for(; i < m_nSize; ++i) if (m_Table[i]) break;
		ExAssert(i < m_nSize);
		return node_t(this, m_Table[i]);
	}
	iterator_t Tail() const
	{
		return node_t(this, NULL);
	}
	iterator_t Last() const
	{
		if (IsNull() || Empty()) return Head();
		long i = (long)(m_nSize - 1);
		for(; i >= 0; --i) if (m_Table[i]) break;
		ExAssert(i >= 0);
		assoc_t* asc = m_Table[i], *lst = asc;
		for(; asc; lst = asc, asc = asc->pNext);
		ExAssert(lst);
		return node_t(this, lst);
	}
	type_t HeadItem() const
	{ return Head()->Val(); }
	type_t LastItem() const
	{ return Last()->Val(); }

	BOOL Add(const key_t& Key, const type_t& Val)
	{
		GetAt(Key) = Val;
		return TRUE;
	}
	BOOL Add(const pair_t& Pair)
	{ return Add(Pair.Key, Pair.Val); }

	BOOL Del(const key_t& Key)
	{
		DWORD inx = 0;
		assoc_t* lst = NULL, *asc = Locate(Key, lst, &inx);
		if (asc)
		{
			if (lst)
				lst->pNext = asc->pNext;
			else
				m_Table[inx] = NULL;
			asc->Free(m_Pool);
			--m_nCount;
			return TRUE;
		}
		else
			return FALSE;
	}
	BOOL Del(iterator_t& Iter)
	{
		if (Empty()) return TRUE;
		if (!(Iter->InThis(this))) return FALSE;
		if (!(Iter->Index())) return FALSE;
		return Del(Iter->Key());
	}
};

//////////////////////////////////////////////////////////////////

template <DWORD SizeT/* = 17*/, typename HashT/* = CHash*/, typename AllocT/* = EXP_MEMORY_ALLOC*/>
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
		typedef typename container_t::assoc_t assoc_t;

		container_t* pCont;
		assoc_t* nIndx;

		node_t(container_t* p = NULL)
			: pCont(p)
			, nIndx(NULL)
		{}
		node_t(const container_t* p, assoc_t* b)
			: pCont((container_t*)p)
			, nIndx(b)
		{}

		const key_t& Key() { return nIndx->Key; }
		type_t& Val() { return nIndx->Val; }

		BOOL InThis(container_t* cnt) { return pCont == cnt; }
		pair_t* Index() { return (pair_t*)nIndx; }

		BOOL operator==(const node_t& node)
		{ return (pCont == node.pCont && (nIndx == node.nIndx)); }
		BOOL operator!=(const node_t& node)
		{ return (pCont != node.pCont || (nIndx != node.nIndx)); }

		void Next(long nOff = 1)
		{
			if (!pCont || nOff == 0) return;
			ExAssert(nOff > 0);
			assoc_t* lst = NULL;
			while(nIndx && 0 < nOff--)
			{
				lst = nIndx;
				nIndx = nIndx->pNext;
				if (!nIndx)
				{
					DWORD inx = (lst->nHash % pCont->m_nSize) + 1;
					do
					{
						if (inx >= pCont->m_nSize) return;
						nIndx = pCont->m_Table[inx++];
					} while(!nIndx);
				}
			}
		}
		void Prev(long nOff = 1)
		{ Next(-nOff); }
	};

	static const DWORD DEF_SIZE = SizeT;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Map_h__*/