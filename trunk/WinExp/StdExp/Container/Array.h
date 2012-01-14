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
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-12-12
// Version:	1.0.0020.1430
//
// History:
//	- 1.0.0015.1600(2011-02-24)	# 修正迭代器获取接口内部实现的一处低级错误(static iterator_t iter(node_t(this));)
//								# 修正CArrayT::Clear()可能出现的元素重复析构问题
//	- 1.0.0016.2202(2011-05-10)	# 修正bool operator==()与bool operator!=()在对指针做比较时无法通过编译的问题
//	- 1.0.0017.0035(2011-05-12)	= 调整bool operator==()与bool operator!=()的内部实现,支持对整个CArrayT做比较
//								- 移除CArrayT中直接与指针做比较的接口
//	- 1.0.0018.1520(2011-06-27)	# 修正当CArrayT的迭代器走到上/下限时可以继续移动的问题
//	- 1.0.0019.2025(2011-07-11)	# 修正CArrayT::operator[]()无法顺利通过编译的问题
//	- 1.0.0020.1430(2011-12-12)	^ 优化CArrayT::InitElements的效率,去掉元素构造前的置空操作
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
	EXP_INLINE static void InitElements(type_t* pDst, DWORD nCount)
	{
		if (!pDst || nCount == 0) return;
	//	ZeroMemory(pDst, sizeof(type_t) * nCount);
		_Traits::Construct<type_t>(pDst, nCount);
	}
	EXP_INLINE static void CopyElements(type_t* pDst, const type_t* pSrc, DWORD nCount, _true_type)
	{
		if (!pDst || !pSrc || nCount == 0) return;
		memcpy(pDst, pSrc, sizeof(type_t) * nCount);
	}
	EXP_INLINE static void CopyElements(type_t* pDst, const type_t* pSrc, DWORD nCount, _false_type)
	{
		if (!pDst || !pSrc || nCount == 0) return;
		while (nCount--) *pDst++ = *pSrc++;
	}
	EXP_INLINE static void CopyElements(type_t* pDst, const type_t* pSrc, DWORD nCount)
	{
		CopyElements(pDst, pSrc, nCount, _TraitsT<type_t>::is_POD_type());
	}

public:
	CArrayT(DWORD nSize = PolicyT::DEF_SIZE)
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
	void SetSize(DWORD nSize = PolicyT::DEF_SIZE)
	{
		if( GetSize() >= nSize ) return;
		m_Array = alloc_t::ReAlloc<type_t>(m_Array, nSize);
		m_nSize = nSize;
	}
	void SetSizeExpan(DWORD nSize = PolicyT::DEF_SIZE)
	{ SetSize(PolicyT::Expan(nSize)); }
	DWORD GetCount() const
	{ return m_nCont; }

	BOOL IsNull() const
	{ return (m_Array == NULL); }
	BOOL Empty() const
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
			m_nSize = GetCount();
			m_Array = alloc_t::ReAlloc<type_t>(m_Array, m_nSize);
		}
		else
			m_nCont = m_nSize = 0;
	}

	type_t& operator[](INT_PTR nIndex)
	{ return GetAt(nIndex); }
	const type_t& operator[](INT_PTR nIndex) const
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

	template <DWORD SizeT>
	BOOL operator==(const TypeT (&aArray)[SizeT]) const
	{ return (*this == CArrayT(aArray)); }
	BOOL operator==(const CArrayT& Array) const
	{
		if (m_Array == Array.m_Array)
			return TRUE;
		if (m_Array == NULL || Array.m_Array == NULL)
			return FALSE;
		if (m_nCont != Array.m_nCont)
			return FALSE;
		return (memcmp(m_Array, Array.m_Array, m_nCont * sizeof(type_t)) == 0);
	}
	template <DWORD SizeT>
	BOOL operator!=(const TypeT (&aArray)[SizeT]) const
	{ return !(*this == aArray); }
	BOOL operator!=(const CArrayT& Array) const
	{ return !(*this == Array); }

	type_t* operator->()
	{ return m_Array; }
	type_t& operator*()
	{ return *m_Array; }
	operator type_t*()
	{ return m_Array; }
	operator const type_t*() const
	{ return m_Array; }

	iterator_t Head() const
	{
		iterator_t ite(node_t(this));
		ite->nIndx = 0;
		return ite;
	}
	iterator_t Tail() const
	{
		iterator_t ite(node_t(this));
		ite->nIndx = m_nCont;
		return ite;
	}
	iterator_t Last() const
	{
		iterator_t ite(node_t(this));
		ite->nIndx = m_nCont - 1;
		return ite;
	}
	type_t& HeadItem() const
	{ return GetAt(0); }
	type_t& LastItem() const
	{ return GetAt(m_nCont - 1); }

	BOOL AddArray(const type_t* pArray, DWORD nSize, iterator_t& Iter)
	{
		if (!pArray || nSize == 0) return TRUE;
		if (Iter->Index() > Tail()->Index()) return FALSE;
		if (!(Iter->InThis(this))) return FALSE;
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
		return TRUE;
	}
	BOOL AddArray(const type_t* pArray, DWORD nSize)
	{ return AddArray(pArray, nSize, Tail()); }
	template <DWORD SizeT>
	BOOL AddArray(const TypeT (&aArray)[SizeT], iterator_t& Iter)
	{ return AddArray(aArray, SizeT, Iter); }
	template <DWORD SizeT>
	BOOL AddArray(const TypeT (&aArray)[SizeT])
	{ return AddArray<SizeT>(aArray, Tail()); }
	BOOL AddArray(const CArrayT& Array, iterator_t& Iter)
	{ return AddArray(Array, Array.GetCount(), Iter); }
	BOOL AddArray(const CArrayT& Array)
	{ return AddArray(Array, Tail()); }

	BOOL Add(const type_t& Item, iterator_t& Iter)
	{ return AddArray(&Item, 1, Iter); }
	BOOL Add(const type_t& Item)
	{ return Add(Item, Tail()); }

	BOOL Del(iterator_t& Iter, DWORD nLen = 1)
	{
		if (Iter->Index() >= Tail()->Index()) return TRUE;
		if (nLen == 0) return TRUE;
		if (Empty()) return TRUE;
		if (!(Iter->InThis(this))) return FALSE;
		// 定位区域
		if (Iter->Index() + nLen > GetCount())
			nLen = GetCount() - Iter->Index();
		DWORD inx_node = Iter->Index() + nLen;
		DWORD inx_size = (GetCount() > inx_node) ? (GetCount() - inx_node) : 0;
		type_t* ptr_array = m_Array + inx_node;
		ExAssert(ptr_array);
		// 删除元素
		_Traits::Destruct<type_t>(ptr_array - nLen, sizeof(type_t) * nLen);
		m_nCont -= nLen;
		// 移动元素
		if (inx_size)
			memmove(ptr_array - nLen, ptr_array, sizeof(type_t) * inx_size);
		return TRUE;
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
		node_t(const container_t* p)
			: pCont((container_t*)p)
			, nIndx(0)
		{}

		type_t& Val() { return pCont->GetAt(nIndx); }

		BOOL InThis(container_t* cnt) { return pCont == cnt; }
		DWORD Index() { return nIndx; }

		BOOL operator==(const node_t& node)
		{ return (memcmp(this, &node, sizeof(node_t)) == 0); }
		BOOL operator!=(const node_t& node)
		{ return (memcmp(this, &node, sizeof(node_t)) != 0); }

		void Next(long nOff = 1) { if (nIndx < pCont->Tail()->Index()) nIndx += nOff; }
		void Prev(long nOff = 1) { if (nIndx > pCont->Head()->Index()) nIndx -= nOff; }
	};

	static const DWORD DEF_SIZE = 0;
	static DWORD Expan(DWORD nSize)
	{ return nSize ? (nSize << 1) : 1; }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Array_h__*/