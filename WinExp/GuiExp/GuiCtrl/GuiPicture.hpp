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
// GuiPicture - 绘图板控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-25
// Version:	1.0.0003.2305
//
// History:
//	- 1.0.0001.2236(2011-05-23)	+ 添加CGuiPicture::IsUpdated()接口
//	- 1.0.0002.1500(2011-05-24)	+ CGuiPicture添加Color属性
//	- 1.0.0003.2305(2011-05-25)	+ CGuiPicture添加Text属性
//								= CGuiPicture调整控件状态接口
//								= CGuiPicture不再托管内部的图片资源
//////////////////////////////////////////////////////////////////

#ifndef __GuiPicture_hpp__
#define __GuiPicture_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiPicture : public IGuiCtrlBase
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiPicture, IGuiCtrlBase)

protected:
	pixel_t m_Color;
	CImage m_Image;
	CText m_Text;

public:
	CGuiPicture()
		: m_Color(0)
	{
		// 添加事件对象
		AddEvent((IGuiEvent*)ExGui(_T("CGuiPictureEvent"), GetGC()));
	}
	~CGuiPicture()
	{
	}

public:
	// 获得控件状态
	state_t* GetState(const CString& sType, CGC* pGC = NULL)
	{
		state_t* state = EXP_BASE::GetState(sType, pGC);
		if (state)
		{
			if (state->sta_typ == _T("color"))
				state->sta_arr.Add(&m_Color);
			else
			if (state->sta_typ == _T("image"))
				state->sta_arr.Add(&m_Image);
			else
			if (state->sta_typ == _T("text"))
				state->sta_arr.Add(&m_Text);
		}
		return state;
	}
	bool SetState(const CString& sType, void* pState)
	{
		if (sType == _T("color"))
		{
			m_Color = *(pixel_t*)pState;
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("image"))
		{
			m_Image = *(CImage*)pState;
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("text"))
		{
			m_Text = *(CText*)pState;
			return EXP_BASE::SetState(sType, pState);
		}
		return false;
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiPicture, IGuiCtrlBase)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiPicture_hpp__*/