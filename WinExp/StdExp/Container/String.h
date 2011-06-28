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
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-06-28
// Version:	1.0.0021.1755
//
// History:
//	- 1.0.0013.1600(2011-02-24)	# 修正迭代器获取接口内部实现的一处低级错误(static iterator_t iter(node_t(this));)
//	- 1.0.0014.0106(2011-02-25)	+ 为String添加用于Hash的DWORD_PTR重载
//	- 1.0.0015.1714(2011-05-10)	# 修正bool operator==()与bool operator!=()在对字符串常量做比较时无法通过编译的问题
//	- 1.0.0016.1642(2011-05-18)	= 使用友元方式重载bool operator==()与bool operator!=()
//	- 1.0.0017.2220(2011-05-24)	# 修正CStringT::SetString()当参数为NULL时的崩溃问题
//	- 1.0.0018.1638(2011-06-15)	# 修正CStringT::Compare()当参数为NULL时的崩溃问题
//	- 1.0.0019.1555(2011-06-17)	# 修正CStringT在不跟随项目Unicode设置的情况下调整TypeT,引起CStringT::GetLength()等函数结果错误的问题
//								# 修正CStringT::operator+=()可能导致路径递归的问题
//								+ 支持CStringT::operator[]()支持由迭代器获取值
//	- 1.0.0020.1817(2011-06-24)	# 修正CStringT::AddString()无法向字符串末尾添加字符的问题
//	- 1.0.0021.1755(2011-06-28)	# 修正CStringT::AddString()在针对字符串添加时包含了待添加字符串的结尾'\0'符,导致结果被意外截断的问题
//////////////////////////////////////////////////////////////////

#ifndef __String_h__
#define __String_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Container/Array.h"
#include "Algorithm/Hash.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename AllocT = EXP_MEMORY_ALLOC>
struct _StringPolicyT;

template <typename TypeT = TCHAR, typename PolicyT = _StringPolicyT<> >
class CStringT : public IContainerObjectT<TypeT, PolicyT, CStringT<TypeT, PolicyT> >
{
public:
	typedef typename CArrayT<type_t> array_t;

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

	operator DWORD_PTR()
	{ return CHash::HashAry<type_t*>(m_Array); }

public:
	DWORD GetSize()
	{ return m_nSize; }
	void SetSize(DWORD nSize = PolicyT::DEF_SIZE)
	{
		if( GetSize() >= nSize ) return;
		type_t* pArray = (type_t*)ZeroMemory(alloc_t::Alloc<type_t>(nSize), sizeof(type_t) * nSize);
		if (m_Array)
		{
			if (sizeof(type_t) == 1)
				StringCchCopyA((char*)pArray, nSize, (char*)m_Array);
			else
				StringCchCopyW((wchar_t*)pArray, nSize, (wchar_t*)m_Array);
			alloc_t::Free(m_Array);
		}
		m_Array = pArray;
		m_nSize = nSize;
	}
	void SetSizeExpan(DWORD nSize = PolicyT::DEF_SIZE)
	{ SetSize(PolicyT::Expan(nSize)); }

	DWORD GetLength() const
	{ return m_Array ? (sizeof(type_t) == 1 ? strlen((char*)m_Array) : wcslen((wchar_t*)m_Array)) : 0; }
	DWORD GetCount() const
	{ return GetLength() + 1; }

	bool Empty() const
	{ return (!m_Array) || (GetLength() == 0); }
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
			{
				if (sizeof(type_t) == 1)
					StringCchCopyA((char*)pArray, count, (char*)m_Array);
				else
					StringCchCopyW((wchar_t*)pArray, count, (wchar_t*)m_Array);
			}
			alloc_t::Free(m_Array);
			m_Array = pArray;
			m_nSize = count;
		}
		else
			m_nSize = 0;
	}

	type_t& GetAt(DWORD nIndex)
	{ return m_Array[nIndex]; }
	const type_t& GetAt(DWORD nIndex) const
	{ return m_Array[nIndex]; }
	type_t& GetAt(iterator_t& nIndex)
	{ return nIndex->Val(); }
	const type_t& GetAt(iterator_t& nIndex) const
	{ return nIndex->Val(); }

	type_t& operator[](DWORD nIndex)
	{ return GetAt(nIndex); }
	const type_t& operator[](DWORD nIndex) const
	{ return GetAt(nIndex); }
	type_t& operator[](iterator_t& nIndex)
	{ return GetAt(nIndex); }
	const type_t& operator[](iterator_t& nIndex) const
	{ return GetAt(nIndex); }

	void Format(const type_t* lpFormat, ...)
	{
		va_list args;
		va_start(args, lpFormat);
		if (sizeof(type_t) == 1)
		{
			size_t len = _vscprintf((char*)lpFormat, args) + 1;
			StringCchVPrintfA((char*)GetCStr(len), len, (char*)lpFormat, args);
		}
		else
		{
			size_t len = _vscwprintf((wchar_t*)lpFormat, args) + 1;
			StringCchVPrintfW((wchar_t*)GetCStr(len), len, (wchar_t*)lpFormat, args);
		}
		va_end(args);
	}

	CStringT& SetString(const type_t* pString)
	{
		if (!pString) return (*this);
		if (sizeof(type_t) == 1)
		{
			GetCStr(strlen((char*)pString)); /*由于函数参数逆序入栈,因此不能在参数中调用GetCStr*/
			StringCchCopyA((char*)m_Array, GetSize(), (char*)pString);
		}
		else
		{
			GetCStr(wcslen((wchar_t*)pString)); /*由于函数参数逆序入栈,因此不能在参数中调用GetCStr*/
			StringCchCopyW((wchar_t*)m_Array, GetSize(), (wchar_t*)pString);
		}
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

	int Compare(const type_t* pString) const
	{
		if (m_Array == pString) return true;
		if (m_Array == NULL || 
			pString == NULL) return false;
		if (sizeof(type_t) == 1)
			return strcmp((char*)m_Array, (char*)pString);
		else
			return wcscmp((wchar_t*)m_Array, (wchar_t*)pString);
	}

	friend bool operator==(const CStringT& str1, const CStringT& str2)
	{ return (str1.Compare(str2) == 0); }
	friend bool operator==(const CStringT& str1, const type_t* str2)
	{ return (str1.Compare(str2) == 0); }
	friend bool operator==(const type_t* str1, const CStringT& str2)
	{ return (str2.Compare(str1) == 0); }
	friend bool operator!=(const CStringT& str1, const CStringT& str2)
	{ return (str1.Compare(str2) != 0); }
	friend bool operator!=(const CStringT& str1, const type_t* str2)
	{ return (str1.Compare(str2) != 0); }
	friend bool operator!=(const type_t* str1, const CStringT& str2)
	{ return (str2.Compare(str1) != 0); }

	CStringT& operator+=(const type_t* pString)
	{
		if (!pString || pString[0] == 0) return (*this);
		if (sizeof(type_t) == 1)
		{
			GetCStr(GetLength() + strlen((char*)pString));
			StringCchCatA((char*)m_Array, GetSize(), (char*)pString);
		}
		else
		{
			GetCStr(GetLength() + wcslen((wchar_t*)pString));
			StringCchCatW((wchar_t*)m_Array, GetSize(), (wchar_t*)pString);
		}
		return (*this);
	}
	CStringT& operator+=(const array_t& aString)
	{ return operator+=((const type_t*)aString); }
	CStringT& operator+=(const CStringT& String)
	{ return operator+=((const type_t*)String); }
	CStringT& operator+=(const type_t Char)
	{
		type_t tmp_chr[2] = {Char, 0};
		return operator+=(tmp_chr);
	}

	CStringT operator+(const type_t* pString)
	{
		CStringT tmp_str(*this);
		return tmp_str += pString;
	}
	CStringT operator+(const array_t& aString)
	{ return operator+((const type_t*)aString); }
	CStringT operator+(const CStringT& String)
	{ return operator+((const type_t*)String); }
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
		static iterator_t iter;
		iter = node_t((CStringT*)this);
		iter->nIndx = 0;
		return iter;
	}
	iterator_t& Tail() const
	{
		static iterator_t iter;
		iter = node_t((CStringT*)this);
		iter->nIndx = GetLength();
		return iter;
	}
	iterator_t& Last()
	{
		static iterator_t iter;
		iter = node_t((CStringT*)this);
		iter->nIndx = GetLength() - 1;
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
	{ return AddString(String, String.GetLength(), Iter); }
	bool AddString(const CStringT& String)
	{ return AddString(String, Tail()); }

	bool Add(const type_t& Item, iterator_t& Iter)
	{ return AddString(&Item, 1, Iter); }
	bool Add(const type_t& Item)
	{ return Add(Item, Tail()); }

	bool Del(iterator_t& Iter, DWORD nLen = 1)
	{
		if (Iter->Index() >= Tail()->Index()) return false;
		if (nLen == 0 || Empty()) return true;
		if(!(Iter->InThis(this))) return false;
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

template <typename AllocT/* = EXP_MEMORY_ALLOC*/>
struct _StringPolicyT : public _ArrayPolicyT<AllocT>
{};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__String_h__*/