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
// String - 字符串
//
// Author:	木头云
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-01-28
// Version:	1.0.0012.0150
//////////////////////////////////////////////////////////////////

#ifndef __String_h__
#define __String_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Container/Array.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename AllocT = DefMemAlloc>
struct _StringPolicyT;

template <typename TypeT = TCHAR, typename PolicyT = _StringPolicyT<> >
class CStringT : public CContainerObjectT<TypeT, PolicyT, CStringT<TypeT, PolicyT> >
{
public:
	typedef typename PolicyT::array_t<type_t> array_t;

protected:
	type_t*	m_Array;
	DWORD	m_nSize;

public:
	CStringT()
		: m_Array(NULL)
		, m_nSize(0)
	{}

	CStringT(const CStringT& String)
		: m_Array(NULL)
		, m_nSize(0)
	{ (*this) = String; }
	CStringT(const array_t& aString)
		: m_Array(NULL)
		, m_nSize(0)
	{ (*this) = aString; }
	CStringT(const type_t* pString)
		: m_Array(NULL)
		, m_nSize(0)
	{ (*this) = pString; }
	template <DWORD SizeT>
	CStringT(const type_t (&aString)[SizeT])
		: m_Array(NULL)
		, m_nSize(0)
	{ (*this) = (type_t*)aString; }

	~CStringT()
	{}

public:
	DWORD GetSize()
	{ return m_nSize; }
	void SetSize(DWORD nSize = PolicyT::s_nDefSize)
	{
		if( GetSize() >= nSize ) return;
		type_t* pArray = (type_t*)ZeroMemory(alloc_t::Alloc<type_t>(nSize), sizeof(type_t) * nSize);
		if (m_Array)
		{
			StringCchCopy(pArray, nSize, m_Array);
			alloc_t::Free(m_Array);
		}
		m_Array = pArray;
		m_nSize = nSize;
	}
	void SetSizeExpan(DWORD nSize = PolicyT::s_nDefSize)
	{ SetSize(PolicyT::Expan(nSize)); }

	DWORD GetLength() const
	{ return m_Array ? _tcslen(m_Array) : 0; }
	DWORD GetCount() const
	{ return GetLength() + 1; }

	bool Empty() const
	{ return (!m_Array) || (_tcscmp(m_Array, _T("")) == 0); }
	void Clear()
	{
		if (m_Array)
		{
			alloc_t::Free(m_Array);
			m_Array = NULL;
		}
		m_nSize = 0;
	}
	void Release()
	{
		if (GetLength() == GetSize()) return;
		if (m_Array)
		{
			DWORD count = GetCount();
			type_t* pArray = alloc_t::Alloc<type_t>(count);
			if (pArray)
				StringCchCopy(pArray, count, m_Array);
			alloc_t::Free(m_Array);
			m_Array = pArray;
			m_nSize = count;
		}
		else
			m_nSize = 0;
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
	type_t& GetAt(iterator_t& nIndex)
	{ return nIndex->Val(); }
	const type_t& GetAt(iterator_t& nIndex) const
	{ return nIndex->Val(); }

	void Format(LPCTSTR lpFormat, ...)
	{
		va_list args;
		va_start(args, lpFormat);

		size_t len = _vsctprintf(lpFormat, args) + 1;
		StringCchVPrintf(GetCStr(len), len, lpFormat, args);

		va_end(args);
	}

	CStringT& SetString(const type_t* pString)
	{
		GetCStr(_tcslen(pString)); /*由于函数参数逆序入栈,因此不能在参数中调用GetCStr*/
		StringCchCopy(m_Array, GetSize(), pString);
		return (*this);
	}
	CStringT& SetString(const array_t& aString)
	{ return SetString(aString); }
	CStringT& SetString(const CStringT& String)
	{ return SetString(String.m_Array); }

	CStringT& operator=(const CStringT& String)
	{ return SetString(String); }
	CStringT& operator=(const array_t& aString)
	{ return SetString(aString); }
	CStringT& operator=(const type_t* pString)
	{ return SetString(pString); }

	bool operator==(type_t* pString)
	{ return (_tcscmp(GetCStr(), pString) == 0); }
	bool operator==(const CStringT& String)
	{ return operator==(String.m_Array); }
	bool operator==(const array_t& aString)
	{ return operator==((const type_t*)aString); }
	bool operator!=(type_t* pString)
	{ return !(*this == pString); }
	bool operator!=(const CStringT& String)
	{ return !(*this == String); }
	bool operator!=(const array_t& aString)
	{ return !(*this == aString); }

	CStringT& operator+=(const type_t* pString)
	{
		if (!pString || _tcslen(pString) == 0) return (*this);
		GetCStr(GetLength() + _tcslen(pString));
		StringCchCat(m_Array, GetSize(), pString);
		return (*this);
	}
	CStringT& operator+=(const array_t& aString)
	{ return operator+=(aString); }
	CStringT& operator+=(const CStringT& String)
	{ return operator+=(String); }
	CStringT& operator+=(const type_t Char)
	{
		type_t tmp_chr[2] = {Char, _T('\0')};
		return operator+=(tmp_chr);
	}

	CStringT operator+(const type_t* pString)
	{
		CStringT tmp_str(*this);
		return tmp_str += pString;
	}
	CStringT operator+(const array_t& aString)
	{ return operator+(aString); }
	CStringT operator+(const CStringT& String)
	{ return operator+(String); }
	CStringT operator+(const type_t Char)
	{
		CStringT tmp_str(*this);
		return tmp_str += Char;
	}

	type_t* GetCStr(DWORD nLen = 0)
	{
		++nLen;	// 加上'\0'结尾
		if (GetSize() < nLen) SetSizeExpan(nLen);
		return (type_t*)m_Array;
	}

	type_t& operator*()
	{ return (*m_Array); }
	operator type_t*()
	{ return m_Array; }
	operator const type_t*() const
	{ return m_Array; }

	iterator_t& Head() const
	{
		static iterator_t iter(node_t(this));
		iter->nIndx = 0;
		return iter;
	}
	iterator_t& Tail() const
	{
		static iterator_t iter(node_t(this));
		iter->nIndx = GetLength() + 1;
		return iter;
	}
	iterator_t& Last()
	{
		static iterator_t iter(node_t(this));
		iter->nIndx = GetLength();
		return iter;
	}
	type_t& HeadItem() const
	{ return m_Array[0]; }
	type_t& LastItem() const
	{ return m_Array[GetLength()]; }

	bool AddString(const type_t* pString, DWORD nSize, iterator_t& Iter)
	{
		if (!pString || nSize == 0) return true;
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
		if (!ptr_array) ptr_array = m_Array;
		ExAssert(ptr_array);
		memcpy(ptr_array, pString, sizeof(type_t) * nSize);
		return true;
	}
	bool AddString(const type_t* pString, DWORD nSize)
	{ return AddString(pString, nSize, Tail()); }
	template <DWORD SizeT>
	bool AddString(const TypeT (&aString)[SizeT], iterator_t& Iter)
	{ return AddString(aString, SizeT, Iter); }
	template <DWORD SizeT>
	bool AddString(const TypeT (&aString)[SizeT])
	{ return AddString<SizeT>(aString, Tail()); }
	bool AddString(const CStringT& String, iterator_t& Iter)
	{ return AddString(String, String.GetCount(), Iter); }
	bool AddString(const CStringT& String)
	{ return AddString(String, Tail()); }

	bool Add(const type_t& Item, iterator_t& Iter)
	{ return AddString(&Item, 1, Iter); }
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
		// 移动元素
		if (inx_size)
		{
			type_t* ptr_array = m_Array + inx_node;
			ExAssert(ptr_array);
			memmove(ptr_array - nLen, ptr_array, sizeof(type_t) * inx_size);
		}
		return true;
	}
};

typedef CStringT<> CString;

//////////////////////////////////////////////////////////////////

template <typename AllocT/* = DefMemAlloc*/>
struct _StringPolicyT : public _ArrayPolicyT<AllocT>
{
	template <typename TypeT>
	class array_t : public CArrayT<TypeT, _StringPolicyT> {};
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__String_h__*/