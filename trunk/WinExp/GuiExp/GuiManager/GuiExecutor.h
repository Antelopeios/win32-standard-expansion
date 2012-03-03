// Copyright 2012, 木头云
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
// Executor - 界面脚本执行
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-02-14
// Version:	1.0.0001.1518
//
// History:
//	- 1.0.0000.1058(2012-01-30)	@ 开始构建Executor
//	- 1.0.0001.1518(2012-02-14)	# 修正ExStringToColor中的若干错误
//////////////////////////////////////////////////////////////////

#ifndef __GuiExecutor_h__
#define __GuiExecutor_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCommon/GuiCommon.h"
#include "GuiManager/GuiXML.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

EXP_API pixel_t ExStringToColor(CString sColor);
EXP_API int ExStringToArray(const CString s, CArrayT<CString> &sa, TCHAR cSpl = _T(','), BOOL bTrim = TRUE);
EXP_API CRect ExStringToRect(const CString& sRect);
EXP_API BOOL ExReleaseBinary(HGLOBAL hData);
EXP_API HGLOBAL ExGetBinary(UINT nID, LPCTSTR szType, BYTE*& btBuff, DWORD& dwSize, HMODULE hInstance = NULL);

//////////////////////////////////////////////////////////////////

// 摘要
EXP_STRUCT detail_t
{
	CString name;		// 名称
	CString author;		// 作者
	CString home;		// 主页
	CString email;		// 邮箱
	CString version;	// 版本
	CString date;		// 日期
};

// 样式
EXP_STRUCT style_t
{
	CArrayT<CText*> font;
	CArrayT<pixel_t> color;
	CArrayT<CImage*> image;
};

//////////////////////////////////////////////////////////////////

EXP_INTERFACE IExecutor : public IBaseObject
{
	EXP_DECLARE_DYNAMIC_CLS(IExecutor, IBaseObject)

public:
	void* m_pNedDel;

public:
	IExecutor() : m_pNedDel(NULL) {}
	virtual ~IExecutor(void) {}

public:
	virtual void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent) = 0;
};

//////////////////////////////////////////////////////////////////

EXP_CLASS CExecutor
{
protected:
	typedef CMapT<CString, IExecutor*> exc_map_t;

protected:
	EXP_INLINE static CGC& GetGC()
	{
		return ExSingleton<CGC>();
	}
	EXP_INLINE static exc_map_t& GetMap()
	{
		return ExSingleton<exc_map_t>();
	}
	EXP_INLINE static LPCTSTR Transform(LPCTSTR key)
	{
		static CString k;
		k.Format(_T("_%s"), key);
		return k.Lower();
	}

public:
	EXP_INLINE static IExecutor* Get(LPCTSTR key)
	{
		if (!key || _tcslen(key) == 0) return NULL;
		exc_map_t::iterator_t ite = GetMap().Locate(key);
		if (ite == GetMap().Tail())
		{
			IExecutor* exc = ExDynCast<IExecutor>(ExDynCreate(Transform(key), &GetGC()));
			if (!exc) return NULL;
			return GetMap()[key] = exc;
		}
		else
			return (*ite);
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiExecutor_h__*/
