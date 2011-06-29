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
// GuiEdit - 编辑框控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-06-29
// Version:	1.0.0002.2018
//
// History:
//	- 1.0.0000.1030(2011-06-21)	@ 开始构建GuiEdit
//	- 1.0.0001.1615(2011-06-27)	+ 添加txt_sel_color与bkg_sel_color属性
//	- 1.0.0002.2018(2011-06-29)	= 将CGuiEdit的基类由CGuiButton改为CGuiPicture
//								^ 简化CGuiEdit的属性定义
//								+ 添加创建输入法上下文的相关代码
//////////////////////////////////////////////////////////////////

#ifndef __GuiEdit_h__
#define __GuiEdit_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(lib, "Imm32.lib")

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiEdit : public CGuiPicture
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiEdit, CGuiPicture)

protected:
	CString m_Edit;
	pixel_t m_ColorSelTxt;
	pixel_t m_ColorSelBkg;
	HIMC	m_ImmContext;

protected:
	void Init(IGuiComp* pComp)
	{
		EXP_BASE::Init(pComp);
		// 为窗口创建输入法上下文
		IGuiBase* base = ExDynCast<IGuiBase>(pComp);
		ExAssert(base); if (!base) return;
		::ImmAssociateContext(base->GethWnd(), m_ImmContext = ::ImmCreateContext());
	}
	void Fina()
	{
		// 销毁输入法上下文
		::ImmDestroyContext(m_ImmContext);
		EXP_BASE::Fina();
	}

public:
	CGuiEdit()
		: m_ImmContext(NULL)
	{
		// 添加事件对象
		InsEvent((IGuiEvent*)ExGui(_T("CGuiEditEvent"), GetGC()));
		pixel_t pix = ExRGBA(EXP_CM, EXP_CM, EXP_CM, EXP_CM);
		SetState(_T("color"), &pix);
		pix = ExRGBA(EXP_CM, EXP_CM, EXP_CM, EXP_CM);
		SetState(_T("txt_sel_color"), &pix);
		pix = ExRGBA(51, 153, EXP_CM, EXP_CM);
		SetState(_T("bkg_sel_color"), &pix);
	}

public:
	// 获得控件状态
	state_t* GetState(const CString& sType, CGC* pGC = NULL)
	{
		state_t* state = IGuiCtrlBase::GetState(sType, pGC);
		if (state)
		{
			if (state->sta_typ == _T("edit"))
				state->sta_arr.Add(&m_Edit);
			else
			if (state->sta_typ == _T("txt_sel_color"))
				state->sta_arr.Add(&m_ColorSelTxt);
			else
			if (state->sta_typ == _T("bkg_sel_color"))
				state->sta_arr.Add(&m_ColorSelBkg);
			else
				state = EXP_BASE::GetState(sType, pGC);
		}
		return state;
	}
	void SetState(const CString& sType, void* pState)
	{
		if (!pState) return;
		if (sType == _T("edit"))
		{
			m_Edit = *((CString*)pState);
			IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("txt_sel_color"))
		{
			m_ColorSelTxt = *((pixel_t*)pState);
			IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("bkg_sel_color"))
		{
			m_ColorSelBkg = *((pixel_t*)pState);
			IGuiCtrlBase::SetState(sType, pState);
		}
		else
			EXP_BASE::SetState(sType, pState);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiEdit, CGuiPicture);

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiEdit_h__*/