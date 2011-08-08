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
// Date:	2011-08-01
// Version:	1.0.0003.0455
//
// History:
//	- 1.0.0000.1030(2011-06-21)	@ 开始构建GuiEdit
//	- 1.0.0001.1615(2011-06-27)	+ 添加txt_sel_color与bkg_sel_color属性
//	- 1.0.0002.2018(2011-06-29)	= 将CGuiEdit的基类由CGuiButton改为CGuiPicture
//								^ 简化CGuiEdit的属性定义
//								+ 添加创建输入法上下文的相关代码
//	- 1.0.0003.0455(2011-08-01)	+ 将CGuiEdit的一些属性拓展为数组,以支持更多属性状态
//////////////////////////////////////////////////////////////////

#ifndef __GuiEdit_hpp__
#define __GuiEdit_hpp__

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
	pixel_t m_ColorSelTxt[2];
	pixel_t m_ColorSelBkg[2];
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
		InsEvent((IGuiEvent*)ExGui(_T("CGuiEditEvent"), GetGC())); /*先让基类绘图*/
		pixel_t pix[2] = 
		{
			ExRGBA(EXP_CM, EXP_CM, EXP_CM, EXP_CM), 
			ExRGBA(0, 0, 0, EXP_CM)
		};
		SetState(_T("color"), (void*)(pix[0]));
		SetState(_T("txt_sel_color"), pix);
		pix[0] = ExRGBA(51, 153, EXP_CM, EXP_CM);
		pix[1] = ExRGBA(191, 205, 219, EXP_CM);
		SetState(_T("bkg_sel_color"), pix);
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
				for(int i = 0; i < _countof(m_ColorSelTxt); ++i)
					state->sta_arr.Add(m_ColorSelTxt + i);
			else
			if (state->sta_typ == _T("bkg_sel_color"))
				for(int i = 0; i < _countof(m_ColorSelBkg); ++i)
					state->sta_arr.Add(m_ColorSelBkg + i);
			else
				state = EXP_BASE::GetState(sType, pGC);
		}
		return state;
	}
	bool SetState(const CString& sType, void* pState)
	{
		if (sType == _T("edit"))
		{
			m_Edit = *((CString*)pState);
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("txt_sel_color"))
		{
			for(int i = 0; i < _countof(m_ColorSelTxt); ++i)
				m_ColorSelTxt[i] = *((pixel_t*)pState + i);
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("bkg_sel_color"))
		{
			for(int i = 0; i < _countof(m_ColorSelBkg); ++i)
				m_ColorSelBkg[i] = *((pixel_t*)pState + i);
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
			return EXP_BASE::SetState(sType, pState);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiEdit, CGuiPicture)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiEdit_hpp__*/