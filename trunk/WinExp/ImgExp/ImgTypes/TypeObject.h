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
// TypeObject - 类型基类
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-30
// Version:	1.0.0003.1557
//
// History:
//	- 1.0.0001.1730(2011-05-05)	+ ITypeObjectT::IsNull()添加const类型接口
//	- 1.0.0002.2319(2011-05-24)	+ ITypeObjectT::Get()添加const类型接口
//	- 1.0.0003.1557(2011-05-30)	- ITypeObjectT移除INonCopyable接口继承
//////////////////////////////////////////////////////////////////

#ifndef __TypeObject_h__
#define __TypeObject_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

struct _TypeAlloc
{
	EXP_INLINE static void Free(void* pPtr)
	{
		if (!pPtr) return;
		::DeleteObject((HGDIOBJ)pPtr);
	}
};

//////////////////////////////////////////////////////////////////

template <typename TypeT, typename TypeAllocT = _TypeAlloc>
interface ITypeObjectT
{
public:
	typedef TypeT type_t;
	typedef TypeAllocT type_alloc_t;

protected:
	CSmartPtrT<type_t, type_alloc_t> m_Type;

public:
	ITypeObjectT()
	{}
	ITypeObjectT(type_t tType)
	{ Set(tType); }
	ITypeObjectT(const ITypeObjectT& tType)
	{ Set(tType.m_Type); }
	virtual ~ITypeObjectT()
	{}

public:
	// 是否做托管
	void SetTrust(bool bTru = true) { m_Type.SetTrust(bTru); }
	bool IsTrust() { return m_Type.IsTrust(); }

	virtual void Set(type_t tType)
	{ m_Type = tType; }
	virtual type_t Get() const
	{ return m_Type; }

	type_t operator=(type_t tType)
	{
		Set(tType);
		return m_Type;
	}
	ITypeObjectT& operator=(const ITypeObjectT& tType)
	{
		Set(tType.m_Type);
		return (*this);
	}

	operator type_t() const
	{ return Get(); }

	virtual bool IsNull() const
	{ return (m_Type == NULL); }

	virtual bool Delete()
	{
		Set(NULL);
		return true;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__TypeObject_h__*/