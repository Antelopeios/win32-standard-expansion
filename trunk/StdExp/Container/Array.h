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
// Array - 数组
//
// Author:	木头云
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-02-24
// Version:	1.0.0015.1600
//
// History:
//	- 1.0.0015.1600(2011-02-24)	# 修正迭代器获取接口内部实现的一处低级错误(static iterator_t iter(node_t(this));)
//								# 修正CArrayT::Clear()可能出现的元素重复析构问题
//////////////////////////////////////////////////////////////////

#ifndef __Array_h__
#define __Array_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/MemAlloc.h"
#include "Container/ContainerObject.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename AllocT = EXP_MEMORY_ALLOC>
struct _ArrayPolicyT;

template <typename TypeT, typename PolicyT = _ArrayPolicyT<> >
class CArrayT : public IContainerObjectT<TypeT, PolicyT, CArrayT<TypeT, PolicyT> >
{
protected:
	type_t*	m_Array;
	DWORD	m_nSize;
	DWORD	m_nCont;

protected:
	EXP_INLINE void InitElements(type_t* pDst, DWORD nCount)
	{
		if (!pDst || nCount == 0) return;
		ZeroMemory(pDst, sizeof(type_t) * nCount);
		CTraits::Construct<type_t>(pDst, nCount);
	}
	EXP_INLINE void CopyElements(type_t* pDst, const type_t* pSrc, DWORD nCount)
	{
		if (!pDst || !pSrc || nCount == 0) return;
		while (nCount--) *pDst++ = *pSrc++;
	}

public:
	CArrayT(DWORD nSize = PolicyT::s_nDefSize)
		: m_Array(NULL)
		, m_nSize(0)
		, m_nCont(0)
	{ SetSize(nSize); }

	CArrayT(const CArrayT& Array)
		: m_Array(NULL)
		, m_nSize(0)
		, m_nCont(0)
	{ (*this) = Array; }
	template <DWORD SizeT>
	CArrayT(const TypeT (&aArray)[SizeT])
		: m_Array(NULL)
		, m_nSize(0)
		, m_nCont(0)
	{ (*this) = aArray; }
	CArrayT(const type_t* pArray, DWORD nSize)
		: m_Array(NULL)
		, m_nSize(0)
		, m_nCont(0)
	{ SetArray(pArray, nSize); }

	~CArrayT()
	{}

public:
	DWORD GetSize() const
	{ return m_nSize; }
	void SetSize(DWORD nSize = PolicyT::s_nDefSize)
	{
		if( GetSize() >= nSize ) return;
		type_t* pArray = alloc_t::Alloc<type_t>(nSize);
		if (m_Array)
		{
			memcpy(pArray, m_Array, sizeof(type_t) * GetCount());
			InitElements(m_Array, GetSize());
			alloc_t::Free(m_Array);
		}
		m_Array = pArray;
		m_nSize = nSize;
	}
	void SetSizeExpan(DWORD nSize = PolicyT::s_nDefSize)
	{ SetSize(PolicyT::Expan(nSize)); }
	DWORD GetCount() const
	{ return m_nCont; }

	bool IsNull() const
	{ return (m_Array == NULL); }
	bool Empty() const
	{ return (m_nCont == 0); }
	void Clear()
	{
		if (m_Array)
		{
			InitElements(m_Array + GetCount(), GetSize() - GetCount());
			alloc_t::Free(m_Array);
			m_Array = NULL;
		}
		m_nCont = m_nSize = 0;
	}
	void Release()
	{
		if (GetCount() == GetSize()) return;
		if (m_Array)
		{
			type_t* pArray = alloc_t::Alloc<type_t>(GetCount());
			if (pArray)
				memcpy(pArray, m_Array, sizeof(type_t) * GetCount());
			InitElements(m_Array, GetSize());
			alloc_t::Free(m_Array);
			m_Array = pArray;
			m_nSize = m_nCont;
		}
		else
			m_nCont = m_nSize = 0;
	}

	type_t& operator[](DWORD nIndex)
	{ return GetAt(nIndex); }
	const type_t& operator[](DWORD nIndex) const
	{ return GetAt(nIndex); }

	type_t& GetAt(DWORD nIndex)
	{
		ExAssert(nIndex < m_nCont);
		return m_Array[nIndex];
	}
	const type_t& GetAt(DWORD nIndex) const
	{
		ExAssert(nIndex < m_nCont);
		return m_Array[nIndex];
	}

	type_t* operator+(DWORD nIndex) const
	{ return (m_Array + nIndex); }
	type_t* operator-(DWORD nIndex) const
	{ return (m_Array - nIndex); }

	CArrayT& SetArray(const type_t* pArray, DWORD nSize)
	{
		Clear();
		AddArray(pArray, nSize);
		return (*this);
	}
	template <DWORD SizeT>
	CArrayT& SetArray(const TypeT (&aArray)[SizeT])
	{ return SetArray(aArray, SizeT); }
	CArrayT& SetArray(const CArrayT& Array)
	{ return SetArray(Array, Array.GetCount()); }

	template <DWORD SizeT>
	CArrayT& operator=(const TypeT (&aArray)[SizeT])
	{ return SetArray<SizeT>(aArray); }
	CArrayT& operator=(const CArrayT& Array)
	{ return SetArray(Array); }

	bool operator==(type_t* pArray) const
	{ return (m_Array == pArray); }
	bool operator==(const CArrayT& Array) const
	{ return (m_Array == (type_t*)Array); }
	bool operator!=(type_t* pArray) const
	{ return (m_Array != pArray); }
	bool operator!=(const CArrayT& Array) const
	{ return (m_Array != (type_t*)Array); }

	type_t* operator->()
	{ return m_Array; }
	type_t& operator*()
	{ return *m_Array; }
	operator type_t*()
	{ return m_Array; }
	operator const type_t*() const
	{ return m_Array; }

	iterator_t& Head()
	{
		static iterator_t iter;
		iter = node_t(this);
		iter->nIndx = 0;
		return iter;
	}
	iterator_t& Tail()
	{
		static iterator_t iter;
		iter = node_t(this);
		iter->nIndx = m_nCont;
		return iter;
	}
	iterator_t& Last()
	{
		static iterator_t iter;
		iter = node_t(this);
		iter->nIndx = m_nCont - 1;
		return iter;
	}
	type_t& HeadItem()
	{ return GetAt(0); }
	type_t& LastItem()
	{ return GetAt(m_nCont - 1); }

	bool AddArray(const type_t* pArray, DWORD nSize, iterator_t& Iter)
	{
		if (!pArray || nSize == 0) return true;
		if (Iter->Index() > Tail()->Index()) return false;
		if (!(Iter->InThis(this))) return false;
		// 检查大小
		DWORD arr_size = (GetCount() + nSize);
		if (GetSize() < arr_size) SetSizeExpan(arr_size);
		// 定位区域
		DWORD inx_node = Iter->Index();
		DWORD inx_size = (GetCount() > inx_node) ? (GetCount() - inx_node) : 0;
		type_t* ptr_array = m_Array + inx_node;
		// 移动元素
		if (inx_size && ptr_array)
			memmove(ptr_array + nSize, ptr_array, sizeof(type_t) * inx_size);
		// 添加元素
		if (ptr_array)
			InitElements(ptr_array, nSize);
		else
			ptr_array = m_Array;
		ExAssert(ptr_array);
		CopyElements(ptr_array, pArray, nSize);
		m_nCont = arr_size;
		return true;
	}
	bool AddArray(const type_t* pArray, DWORD nSize)
	{ return AddArray(pArray, nSize, Tail()); }
	template <DWORD SizeT>
	bool AddArray(const TypeT (&aArray)[SizeT], iterator_t& Iter)
	{ return AddArray(aArray, SizeT, Iter); }
	template <DWORD SizeT>
	bool AddArray(const TypeT (&aArray)[SizeT])
	{ return AddArray<SizeT>(aArray, Tail()); }
	bool AddArray(const CArrayT& Array, iterator_t& Iter)
	{ return AddArray(Array, Array.GetCount(), Iter); }
	bool AddArray(const CArrayT& Array)
	{ return AddArray(Array, Tail()); }

	bool Add(const type_t& Item, iterator_t& Iter)
	{ return AddArray(&Item, 1, Iter); }
	bool Add(const type_t& Item)
	{ return Add(Item, Tail()); }

	bool Del(iterator_t& Iter, DWORD nLen = 1)
	{
		if (Iter->Index() >= Tail()->Index()) return true;
		if (nLen == 0) return true;
		if (Empty()) return true;
		if (!(Iter->InThis(this))) return false;
		// 定位区域
		if (Iter->Index() + nLen > GetCount())
			nLen = GetCount() - Iter->Index();
		DWORD inx_node = Iter->Index() + nLen;
		DWORD inx_size = (GetCount() > inx_node) ? (GetCount() - inx_node) : 0;
		type_t* ptr_array = m_Array + inx_node;
		ExAssert(ptr_array);
		// 删除元素
		CTraits::Destruct<type_t>(ptr_array - nLen, sizeof(type_t) * nLen);
		m_nCont -= nLen;
		// 移动元素
		if (inx_size)
			memmove(ptr_array - nLen, ptr_array, sizeof(type_t) * inx_size);
		return true;
	}
};

//////////////////////////////////////////////////////////////////

template <typename AllocT/* = EXP_MEMORY_ALLOC*/>
struct _ArrayPolicyT
{
	typedef AllocT alloc_t;

	template <typename ContainerT>
	struct node_t
	{
		typedef ContainerT container_t;
		typedef typename container_t::type_t type_t;

		container_t* pCont;
		DWORD	nIndx;

		node_t(container_t* p = NULL)
			: pCont(p)
			, nIndx(0)
		{}

		type_t& Val() { return pCont->GetAt(nIndx); }

		bool InThis(container_t* cnt) { return pCont == cnt; }
		DWORD Index() { return nIndx; }

		bool operator==(const node_t& node)
		{ return (memcmp(this, &node, sizeof(node_t)) == 0); }
		bool operator!=(const node_t& node)
		{ return (memcmp(this, &node, sizeof(node_t)) != 0); }

		void Next(long nOff = 1) { nIndx += nOff; }
		void Prev(long nOff = 1) { nIndx -= nOff; }
	};

	static const DWORD s_nDefSize = 0;
	static DWORD Expan(DWORD nSize)
	{ return nSize ? (nSize << 1) : 1; }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Array_h__*/