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
// Date:	2011-12-10
// Version:	1.0.0033.1542
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
//	- 1.0.0022.1650(2011-07-28)	# 修正CStringT::operator[]()无法顺利通过编译的问题
//								# 修正CStringT::LastItem()定位错误的问题
//	- 1.0.0023.1555(2011-07-29)	+ CStringT::Compare()支持替换默认的比较函数接口进行字符串比较
//								+ 为CStringT增加一些方便的功能接口
//	- 1.0.0024.1105(2011-08-19)	+ 为CStringT的一些接口添加返回值
//								+ 添加CStringT::Trim()
//	- 1.0.0025.1651(2011-08-23)	+ 字符串类增加自动转码功能,将根据自身的TypeT类型自动将传入的char或wchar_t转换为合适的类型
//	- 1.0.0026.1505(2011-08-26)	# 修正某些情况下字符串类的+号重载编译时出现error C2666的问题
//	- 1.0.0027.1524(2011-09-05)	# 删除掉多余的字符串赋值接口,防止函数内出现递归赋值的编译错误
//	- 1.0.0028.0935(2011-09-06)	# 修正构造时意外的字符串浅拷贝现象
//	- 1.0.0029.1157(2011-09-07)	# 修正字符串转码算法中的内存溢出及内存泄漏现象
//	- 1.0.0030.1557(2011-09-14)	+ 为字符串Format;=;+=接口添加自动编码转换的相关重载
//	- 1.0.0031.1557(2011-09-15)	# 修正Compare接口在某些情况下返回结果错误的问题
//	- 1.0.0032.1256(2011-10-10)	^ 将负责编码转换的部分单独提取出来,作为独立的静态接口,方便外部直接调用
//	- 1.0.0033.1542(2011-12-10)	^ 单独定义并优化部分CStringT的Find及RevFind接口
//////////////////////////////////////////////////////////////////

#ifndef __String_h__
#define __String_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Container/Array.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename AllocT = EXP_MEMORY_ALLOC>
struct _StringPolicyT;

template <typename TypeT = TCHAR, typename PolicyT = _StringPolicyT<> >
class CStringT : public IContainerObjectT<TypeT, PolicyT, CStringT<TypeT, PolicyT> >
{
public:
	static int MultiByteToWideChar(UINT CodePage, LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, void* pGC = NULL)
	{
		cchWideChar = ::MultiByteToWideChar(CodePage, NULL, lpMultiByteStr, cbMultiByte, NULL, 0);
		lpWideCharStr = ExMem::Alloc<wchar_t>((CGC*)pGC, cchWideChar);
		return ::MultiByteToWideChar(CodePage, NULL, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
	}

	static int WideCharToMultiByte(UINT CodePage, LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, void* pGC = NULL)
	{
		cbMultiByte = ::WideCharToMultiByte(CodePage, NULL, lpWideCharStr, cchWideChar, NULL, 0, NULL, FALSE);
		lpMultiByteStr = ExMem::Alloc<char>((CGC*)pGC, cbMultiByte);
		return ::WideCharToMultiByte(CodePage, NULL, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, NULL, FALSE);
	}

public:
	typedef typename CArrayT<type_t> array_t;

protected:
	iterator_t m_Head;
	iterator_t m_Tail;
	iterator_t m_Last;

	type_t*	m_Array;
	DWORD	m_nSize;

public:
	CStringT()
		: m_Array(NULL)
		, m_nSize(0)
	{}

	CStringT(const CStringT& str)
		: m_Array(NULL)
		, m_nSize(0)
	{ (*this) = str; }
	CStringT(const array_t& str)
		: m_Array(NULL)
		, m_nSize(0)
	{ (*this) = str; }
	CStringT(const char* str)
		: m_Array(NULL)
		, m_nSize(0)
	{ (*this) = str; }
	CStringT(const wchar_t* str)
		: m_Array(NULL)
		, m_nSize(0)
	{ (*this) = str; }

	~CStringT()
	{}

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
				strcpy_s((char*)pArray, nSize, (char*)m_Array);
			else
				wcscpy_s((wchar_t*)pArray, nSize, (wchar_t*)m_Array);
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

	BOOL Empty() const
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
					strcpy_s((char*)pArray, count, (char*)m_Array);
				else
					wcscpy_s((wchar_t*)pArray, count, (wchar_t*)m_Array);
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

	type_t& operator[](INT_PTR nIndex)
	{ return GetAt(nIndex); }
	const type_t& operator[](INT_PTR nIndex) const
	{ return GetAt(nIndex); }
	type_t& operator[](iterator_t& nIndex)
	{ return GetAt(nIndex); }
	const type_t& operator[](iterator_t& nIndex) const
	{ return GetAt(nIndex); }

	void Format(const char* lpFormat, ...)
	{
		va_list args;
		va_start(args, lpFormat);
		if (sizeof(type_t) == 1)
		{
			size_t len = _vscprintf((char*)lpFormat, args) + 1;
			vsprintf_s((char*)GetCStr(len), len, (char*)lpFormat, args);
		}
		else
		{
			CStringT fmt(lpFormat);
			size_t len = _vscwprintf((wchar_t*)fmt.GetCStr(), args) + 1;
			vswprintf_s((wchar_t*)GetCStr(len), len, (wchar_t*)fmt.GetCStr(), args);
		}
		va_end(args);
	}
	void Format(const wchar_t* lpFormat, ...)
	{
		va_list args;
		va_start(args, lpFormat);
		if (sizeof(type_t) == 1)
		{
			CStringT fmt(lpFormat);
			size_t len = _vscprintf((char*)fmt.GetCStr(), args) + 1;
			vsprintf_s((char*)GetCStr(len), len, (char*)fmt.GetCStr(), args);
		}
		else
		{
			size_t len = _vscwprintf((wchar_t*)lpFormat, args) + 1;
			vswprintf_s((wchar_t*)GetCStr(len), len, (wchar_t*)lpFormat, args);
		}
		va_end(args);
	}

	CStringT& SetString(const char* pString)
	{
		if (!pString) return (*this);
		if (sizeof(type_t) == 1)
		{
			GetCStr(strlen((char*)pString)); /*由于函数参数逆序入栈,因此不能在参数中调用GetCStr*/
			strcpy_s((char*)m_Array, GetSize(), (char*)pString);
		}
		else
		{
			wchar_t* tmp_str = NULL; int len = 0;
			MultiByteToWideChar(CP_ACP, pString, -1, tmp_str, len);
			GetCStr(len); /*由于函数参数逆序入栈,因此不能在参数中调用GetCStr*/
			wcscpy_s((wchar_t*)m_Array, GetSize(), tmp_str);
			ExMem::Free(tmp_str);
		}
		return (*this);
	}
	CStringT& SetString(const wchar_t* pString)
	{
		if (!pString) return (*this);
		if (sizeof(type_t) == 1)
		{
			char* tmp_str = NULL; int len = 0;
			WideCharToMultiByte(CP_OEMCP, pString, -1, tmp_str, len);
			GetCStr(len); /*由于函数参数逆序入栈,因此不能在参数中调用GetCStr*/
			strcpy_s((char*)m_Array, GetSize(), tmp_str);
			ExMem::Free(tmp_str);
		}
		else
		{
			GetCStr(wcslen((wchar_t*)pString)); /*由于函数参数逆序入栈,因此不能在参数中调用GetCStr*/
			wcscpy_s((wchar_t*)m_Array, GetSize(), (wchar_t*)pString);
		}
		return (*this);
	}

	CStringT& operator=(const CStringT& str)
	{ return SetString(str.m_Array); }
	CStringT& operator=(const char* str)
	{ return SetString(str); }
	CStringT& operator=(const wchar_t* str)
	{ return SetString(str); }
	CStringT& operator=(const char c)
	{
		char tmp_chr[2] = {c, 0};
		return SetString(tmp_chr);
	}
	CStringT& operator=(const wchar_t c)
	{
		wchar_t tmp_chr[2] = {c, 0};
		return SetString(tmp_chr);
	}

	CStringT Mid(int nStart, int nCount = -1) const
	{
		if (nStart < 0) nStart = 0;
		if (nStart >= (int)GetLength()) nStart = (int)GetLength() - 1;
		int max_cnt = (int)GetLength() - nStart;
		if (nCount < 0 || nCount > max_cnt)
			nCount = max_cnt;
		CStringT str;
		if (nCount > 0)
		{
			memcpy(str.GetCStr(nCount), (m_Array + nStart), (nCount * sizeof(type_t)));
			str[nCount] = 0;
		}
		return str;
	}
	CStringT Left(int nCount) const
	{
		return Mid(0, nCount);
	}
	CStringT Right(int nCount) const
	{
		return Mid((int)GetLength() - nCount, -1);
	}

	CStringT& Upper()
	{
		if (Empty()) return (*this);
		type_t* s = m_Array, *str = m_Array;
		if (sizeof(type_t) == 1)
			while(*s) *str++ = (type_t)toupper(*s++);
		else
			while(*s) *str++ = (type_t)towupper(*s++);
		return (*this);
	}
	CStringT& Lower()
	{
		if (Empty()) return (*this);
		type_t* s = m_Array, *str = m_Array;
		if (sizeof(type_t) == 1)
			while(*s) *str++ = (type_t)tolower(*s++);
		else
			while(*s) *str++ = (type_t)towlower(*s++);
		return (*this);
	}

	CStringT& TrimLeft(const type_t cTar)
	{
		if (Empty()) return (*this);
		int cnt = 0;
		type_t* str = m_Array;
		while(*str)
		{
			if (*str++ != cTar) break;
			++cnt;
		}
		Del(Head(), cnt);
		return (*this);
	}
	CStringT& TrimLeft(const type_t* sTar)
	{
		if (!sTar) return (*this);
		type_t* str = (type_t*)sTar;
		while(*str) TrimLeft(*str++);
		return (*this);
	}
	CStringT& TrimRight(const type_t cTar)
	{
		if (Empty()) return (*this);
		int cnt = 0, len = GetLength();
		type_t* str = m_Array + len - 1;
		while(cnt < len)
		{
			if (*str-- != cTar) break;
			++cnt;
		}
		Del(Tail() - cnt, cnt);
		return (*this);
	}
	CStringT& TrimRight(const type_t* sTar)
	{
		if (!sTar) return (*this);
		int cnt = 0, len = (sizeof(type_t) == 1 ? strlen((char*)sTar) : wcslen((wchar_t*)sTar));
		type_t* str = (type_t*)sTar + len - 1;
		while(cnt < len)
		{
			TrimRight(*str--);
			++cnt;
		}
		return (*this);
	}
	CStringT& Trim(const type_t cTar)
	{
		TrimLeft(cTar);
		TrimRight(cTar);
		return (*this);
	}
	CStringT& Trim(const type_t* sTar)
	{
		TrimLeft(sTar);
		TrimRight(sTar);
		return (*this);
	}

	typedef int (__cdecl *a_comp_t)(const char*, const char*);
	typedef int (__cdecl *w_comp_t)(const wchar_t*, const wchar_t*);
	int Compare(const type_t* pString, a_comp_t a_comp = NULL, w_comp_t w_comp = NULL) const
	{
		if (m_Array == pString) return 0;
		if (m_Array == NULL)
		{
			if (pString[0] == _T('\0'))
				return 0;
			else
				return -1;
		}
		if (pString == NULL)
		{
			if (m_Array[0] == _T('\0'))
				return 0;
			else
				return 1;
		}
		if (a_comp == NULL) a_comp = strcmp;
		if (w_comp == NULL) w_comp = wcscmp;
		if (sizeof(type_t) == 1)
			return a_comp((char*)m_Array, (char*)pString);
		else
			return w_comp((wchar_t*)m_Array, (wchar_t*)pString);
	}

	friend BOOL operator==(const CStringT& str1, const CStringT& str2)
	{ return (str1.Compare(str2) == 0); }
	friend BOOL operator==(const CStringT& str1, const type_t* str2)
	{ return (str1.Compare(str2) == 0); }
	friend BOOL operator==(const type_t* str1, const CStringT& str2)
	{ return (str2.Compare(str1) == 0); }
	friend BOOL operator!=(const CStringT& str1, const CStringT& str2)
	{ return (str1.Compare(str2) != 0); }
	friend BOOL operator!=(const CStringT& str1, const type_t* str2)
	{ return (str1.Compare(str2) != 0); }
	friend BOOL operator!=(const type_t* str1, const CStringT& str2)
	{ return (str2.Compare(str1) != 0); }

	CStringT& operator+=(const char* str)
	{
		if (!str || str[0] == 0) return (*this);
		if (sizeof(type_t) == 1)
		{
			GetCStr(GetLength() + strlen((char*)str));
			strcat_s((char*)m_Array, GetSize(), (char*)str);
		}
		else
		{
			CStringT fmt(str);
			GetCStr(GetLength() + fmt.GetLength());
			wcscat_s((wchar_t*)m_Array, GetSize(), (wchar_t*)fmt.GetCStr());
		}
		return (*this);
	}
	CStringT& operator+=(const wchar_t* str)
	{
		if (!str || str[0] == 0) return (*this);
		if (sizeof(type_t) == 1)
		{
			CStringT fmt(str);
			GetCStr(GetLength() + fmt.GetLength());
			strcat_s((char*)m_Array, GetSize(), (char*)fmt.GetCStr());
		}
		else
		{
			GetCStr(GetLength() + wcslen((wchar_t*)str));
			wcscat_s((wchar_t*)m_Array, GetSize(), (wchar_t*)str);
		}
		return (*this);
	}
	CStringT& operator+=(const char c)
	{
		char tmp_chr[2] = {c, 0};
		return operator+=(tmp_chr);
	}
	CStringT& operator+=(const wchar_t c)
	{
		wchar_t tmp_chr[2] = {c, 0};
		return operator+=(tmp_chr);
	}

	CStringT operator+(const char* str)
	{ return (CStringT(*this) += str); }
	CStringT operator+(const wchar_t* str)
	{ return (CStringT(*this) += str); }
	CStringT operator+(const char c)
	{ return (CStringT(*this) += c); }
	CStringT operator+(const wchar_t c)
	{ return (CStringT(*this) += c); }

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

	iterator_t Head() const
	{
		iterator_t ite(node_t((CStringT*)this));
		ite->nIndx = 0;
		return ite;
	}
	iterator_t Tail() const
	{
		iterator_t ite(node_t((CStringT*)this));
		ite->nIndx = GetLength();
		return ite;
	}
	iterator_t Last()
	{
		iterator_t ite(node_t((CStringT*)this));
		ite->nIndx = GetLength() - 1;
		return ite;
	}
	type_t& HeadItem() const
	{ return m_Array[0]; }
	type_t& LastItem() const
	{ return m_Array[GetLength() - 1]; }

	BOOL AddString(const type_t* pString, DWORD nSize, iterator_t& Iter)
	{
		if (!pString || nSize == 0) return TRUE;
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
		if (!ptr_array) ptr_array = m_Array;
		ExAssert(ptr_array);
		memcpy(ptr_array, pString, sizeof(type_t) * nSize);
		return TRUE;
	}
	BOOL AddString(const type_t* pString, DWORD nSize)
	{ return AddString(pString, nSize, Tail()); }
	template <DWORD SizeT>
	BOOL AddString(const TypeT (&aString)[SizeT], iterator_t& Iter)
	{ return AddString(aString, SizeT, Iter); }
	template <DWORD SizeT>
	BOOL AddString(const TypeT (&aString)[SizeT])
	{ return AddString<SizeT>(aString, Tail()); }
	BOOL AddString(const CStringT& String, iterator_t& Iter)
	{ return AddString(String, String.GetLength(), Iter); }
	BOOL AddString(const CStringT& String)
	{ return AddString(String, Tail()); }

	BOOL Add(const type_t& Item, iterator_t& Iter)
	{ return AddString(&Item, 1, Iter); }
	BOOL Add(const type_t& Item)
	{ return Add(Item, Tail()); }

	BOOL Del(iterator_t& Iter, DWORD nLen = 1)
	{
		if (Iter->Index() >= Tail()->Index()) return FALSE;
		if (nLen == 0 || Empty()) return TRUE;
		if(!(Iter->InThis(this))) return FALSE;
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
		return TRUE;
	}

	iterator_t Find(const type_t& tItem) const
	{
		if (Empty()) return Tail();
		if (sizeof(type_t) == 1)
		{
			char* c = strchr((char*)m_Array, (char)tItem);
			if (c)
			{
				iterator_t ite(node_t((CStringT*)this));
				ite->nIndx = (c - (char*)m_Array);
				return ite;
			}
			else
				return Tail();
		}
		else
		{
			wchar_t* c = wcschr((wchar_t*)m_Array, (wchar_t)tItem);
			if (c)
			{
				iterator_t ite(node_t((CStringT*)this));
				ite->nIndx = (c - (wchar_t*)m_Array);
				return ite;
			}
			else
				return Tail();
		}
	}
	iterator_t RevFind(const type_t& tItem) const
	{
		if (Empty()) return Tail();
		if (sizeof(type_t) == 1)
		{
			char* c = strrchr((char*)m_Array, (char)tItem);
			if (c)
			{
				iterator_t ite(node_t((CStringT*)this));
				ite->nIndx = (c - (char*)m_Array);
				return ite;
			}
			else
				return Tail();
		}
		else
		{
			wchar_t* c = wcsrchr((wchar_t*)m_Array, (wchar_t)tItem);
			if (c)
			{
				iterator_t ite(node_t((CStringT*)this));
				ite->nIndx = (c - (wchar_t*)m_Array);
				return ite;
			}
			else
				return Tail();
		}
	}
	iterator_t Find(const container_t& tCnt2) const
	{
		if (Empty()) return Tail();
		if (sizeof(type_t) == 1)
		{
			char* c = strstr((char*)m_Array, (char*)tCnt2.m_Array);
			if (c)
			{
				iterator_t ite(node_t((CStringT*)this));
				ite->nIndx = (c - (char*)m_Array);
				return ite;
			}
			else
				return Tail();
		}
		else
		{
			wchar_t* c = wcsstr((wchar_t*)m_Array, (wchar_t*)tCnt2.m_Array);
			if (c)
			{
				iterator_t ite(node_t((CStringT*)this));
				ite->nIndx = (c - (wchar_t*)m_Array);
				return ite;
			}
			else
				return Tail();
		}
	}

	iterator_t Find(iterator_t& iHead, iterator_t& iTail, const type_t& tItem) const
	{ return Empty() ? iTail : finder_t::Find(iHead, iTail, tItem); }
	iterator_t Find(iterator_t& iHead, iterator_t& iTail, const container_t& tCnt2) const
	{ return Empty() ? iTail : finder_t::Find(iHead, iTail, tCnt2); }

	iterator_t RevFind(iterator_t& iHead, iterator_t& iTail, const type_t& tItem) const
	{ return Empty() ? iTail : finder_t::RevFind(iHead, iTail, tItem); }
	iterator_t RevFind(iterator_t& iHead, iterator_t& iTail, const container_t& tCnt2) const
	{ return Empty() ? iTail : finder_t::RevFind(iHead, iTail, tCnt2); }
	iterator_t RevFind(const container_t& tCnt2) const
	{ return Empty() ? Tail() : finder_t::RevFind(*(This()), tCnt2); }
};

typedef CStringT<> CString;

//////////////////////////////////////////////////////////////////

template <typename AllocT/* = EXP_MEMORY_ALLOC*/>
struct _StringPolicyT : public _ArrayPolicyT<AllocT>
{};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__String_h__*/