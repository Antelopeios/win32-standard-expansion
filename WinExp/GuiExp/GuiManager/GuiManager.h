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
// GuiManager - 界面管理器
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-02-21
// Version:	1.0.0004.1646
//
// History:
//	- 1.0.0000.1622(2011-06-13)	@ 开始构建GuiManager接口
//	- 1.0.0001.0527(2011-08-01)	= 将GuiManager改为模板形式,提供泛型化的界面元素管理
//	- 1.0.0002.0128(2011-12-10)	+ GuiManager的Reg接口支持重复注册同一个key,第二次注册将会覆盖上次的注册
//	- 1.0.0003.1801(2012-01-31)	+ 添加CGuiManagerT::Clear()
//	- 1.0.0004.1646(2012-02-21)	= 调整并优化对外接口的调用方式
//////////////////////////////////////////////////////////////////

#ifndef __GuiManager_h__
#define __GuiManager_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCommon/GuiCommon.h"

//////////////////////////////////////////////////////////////////

#include "GuiManager/GuiXML.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename TypeT, const DWORD SizeT = 1009>
class CGuiManagerT
{
public:
	typedef CMapT<CString, TypeT*, _MapPolicyT<SizeT> > key_map_t;
	EXP_INLINE static key_map_t& Instance()
	{
		return ExSingleton<key_map_t>();
	}

public:
	EXP_INLINE static TypeT* Reg(LPCTSTR c_key, TypeT* inf)
	{
		if (!c_key) return NULL;
		return (Instance()[c_key] = inf);
	}
	EXP_INLINE static TypeT* Get(LPCTSTR c_key)
	{
		if (!c_key) return NULL;
		key_map_t& types = Instance();
		key_map_t::iterator_t ite = types.Locate(c_key);
		if (ite == types.Tail())
			return NULL;
		else
			return ite->Val();
	}
	EXP_INLINE static void Clear(BOOL bDel = TRUE)
	{
		for(key_map_t::iterator_t ite = Instance().Head(); ite != Instance().Tail(); ++ite)
			if (bDel) del(*ite);
	}
};

//////////////////////////////////////////////////////////////////

template <typename TypeT>
EXP_INLINE TypeT* ExReg(LPCTSTR c_key, void* inf)
{
	return EXP::CGuiManagerT<TypeT>::Reg(c_key, (TypeT*)(inf));
}

template <typename TypeT>
EXP_INLINE TypeT* ExGet(LPCTSTR c_key)
{
	return EXP::CGuiManagerT<TypeT>::Get(c_key);
}

template <typename TypeT>
EXP_INLINE void ExClr(BOOL bDel = TRUE)
{
	EXP::CGuiManagerT<TypeT>::Clear(bDel);
}

//////////////////////////////////////////////////////////////////

EXP_END

#include "GuiManager/Executors/Executor.h"
#include "GuiManager/GuiSkin.h"

//////////////////////////////////////////////////////////////////

#endif/*__GuiManager_h__*/