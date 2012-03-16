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
// DC - 设备上下文
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-16
// Version:	1.0.0004.2315
//
// History:
//	- 1.0.0001.1334(2011-04-12)	^ 移除IGraphObject接口,通过ITypeObjectT接口模板统一通用的接口
//								= CExpGraph更名为CGraph
//	- 1.0.0002.2319(2011-05-24)	+ 为CGraph的属性获取接口添加const类型
//	- 1.0.0003.2230(2012-01-04)	# CGraph::SetObject()只能记录第一个传入的类型参数,导致部分资源无法自动释放
//	- 1.0.0004.2315(2012-03-16)	= 将CGraph改名为CDC
//								+ 添加CDC::GetClipBox()与CDC::SetClipBox()
//////////////////////////////////////////////////////////////////

#ifndef __DC_h__
#define __DC_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgTypes/TypeObject.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

struct _DCAlloc
{
	EXP_INLINE static void Free(void* pPtr)
	{
		if (!pPtr) return;
		::DeleteDC((HDC)pPtr);
	}
};

//////////////////////////////////////////////////////////////////

class CDC : public ITypeObjectT<dc_t, _DCAlloc>
{
public:
	typedef ITypeObjectT<dc_t, _DCAlloc> base_obj_t;

protected:
	typedef CListT<DWORD>	typlst_t;
	typedef CListT<HGDIOBJ>	objlst_t;

	typlst_t m_TypLst;
	objlst_t m_ObjLst;

public:
	CDC(dc_t tGraph = NULL)
		: base_obj_t()
	{ Set(tGraph); }
	virtual ~CDC()
	{}

public:
	void Set(dc_t tGraph)
	{
		if (Get() == tGraph) return;
		if (!IsNull())
		{
			for(objlst_t::iterator_t ite = m_ObjLst.Head(); ite != m_ObjLst.Tail(); ++ite)
				::SelectObject(Get(), (*ite));
		}
		m_ObjLst.Clear();
		m_TypLst.Clear();
		base_obj_t::Set(tGraph);
	}

	dc_t operator=(dc_t tType)
	{
		Set(tType);
		return Get();
	}

	dc_t Create(dc_t tGraph = NULL)
	{
		Set(::CreateCompatibleDC(tGraph));
		return Get();
	}

	HGDIOBJ SetObject(HGDIOBJ hObj)
	{
		HGDIOBJ tmp_obj(::SelectObject(Get(), hObj));
		DWORD type = ::GetObjectType(hObj);
		if (m_TypLst.Find(type) == m_TypLst.Tail())
		{
			m_TypLst.Add(type);
			if (tmp_obj && (m_ObjLst.Find(tmp_obj) == m_ObjLst.Tail()))
				m_ObjLst.Add(tmp_obj);
		}
		return tmp_obj;
	}
	HGDIOBJ GetObject(UINT uType) const
	{ return ::GetCurrentObject(Get(), uType); }

	int GetClipBox(CRect& rc)
	{
		return ::GetClipBox(Get(), (LPRECT)&rc);
	}
	int SetClipBox(const CRect& rc)
	{
		HRGN rgn = ::CreateRectRgnIndirect((LPRECT)&rc);
		int r = ::SelectClipRgn(Get(), rgn);
		::DeleteObject(rgn);
		return r;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__DC_h__*/