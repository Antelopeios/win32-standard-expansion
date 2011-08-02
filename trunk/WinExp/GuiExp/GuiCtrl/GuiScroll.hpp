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
// GuiScroll - 滚动条控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-02
// Version:	1.0.0000.0912
//
// History:
//	- 1.0.0000.0912(2011-08-02)	@ 准备构建GuiScroll
//////////////////////////////////////////////////////////////////

#ifndef __GuiScroll_hpp__
#define __GuiScroll_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiSlider : public CGuiPicture /*滚动条滑槽*/
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiSlider, CGuiPicture)

protected:
	CGuiButton m_Slider;
	LONG m_All/*全部内容长度*/, m_Fra/*片段长度*/, m_Pos/*位置*/;
	bool m_Ori; // 方向

public:
	CGuiSlider()
		: m_All(0)
		, m_Fra(0)
		, m_Pos(0)
		, m_Ori(false)
	{
		// 添加事件对象
		InsEvent((IGuiEvent*)ExGui(_T("CGuiSliderEvent"), GetGC())); /*先让基类绘图*/
		m_Slider.AddEvent((IGuiEvent*)ExGui(_T("CGuiSliBlkEvent"), GetGC()));
		// 添加控件对象
		InsComp(&m_Slider);
	}

public:
	void Format()
	{
		if (m_Pos > m_All - m_Fra) m_Pos = m_All - m_Fra;
		if (m_Pos < 0) m_Pos = 0;
	}

	state_t* GetState(const CString& sType, CGC* pGC = NULL)
	{
		if (sType == _T("slider"))
		{
			state_t* state = IGuiCtrlBase::GetState(sType, pGC);
			if (state) state->sta_arr.Add(&m_Slider);
			return state;
		}
		if (sType == _T("all"))
		{
			state_t* state = IGuiCtrlBase::GetState(sType, pGC);
			if (state) state->sta_arr.Add((void*)m_All);
			return state;
		}
		else
		if (sType == _T("fra"))
		{
			state_t* state = IGuiCtrlBase::GetState(sType, pGC);
			if (state) state->sta_arr.Add((void*)m_Fra);
			return state;
		}
		else
		if (sType == _T("pos"))
		{
			state_t* state = IGuiCtrlBase::GetState(sType, pGC);
			if (state) state->sta_arr.Add((void*)m_Pos);
			return state;
		}
		else
		if (sType == _T("ori"))
		{
			state_t* state = IGuiCtrlBase::GetState(sType, pGC);
			if (state) state->sta_arr.Add((void*)m_Ori);
			return state;
		}
		else
		if (sType == _T("sli_image"))
			return m_Slider.GetState(_T("image"), pGC);
		else
		if (sType == _T("sli_color"))
			return m_Slider.GetState(_T("color"), pGC);
		else
		{
			state_t* state = CGuiPicture::GetState(sType, pGC);
			if (!state || state->sta_arr.Empty())
			{
				if (!pGC) ExMem::Free(state);
				state = m_Slider.GetState(sType, pGC);
			}
			return state;
		}
	}
	bool SetState(const CString& sType, void* pState)
	{
		if (sType == _T("all"))
		{
			m_All = (LONG)(LONG_PTR)pState;
			Format();
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("fra"))
		{
			m_Fra = (LONG)(LONG_PTR)pState;
			Format();
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("pos"))
		{
			m_Pos = (LONG)(LONG_PTR)pState;
			Format();
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("ori"))
		{
			m_Ori = (bool)(LONG_PTR)pState;
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("sli_image"))
			return m_Slider.SetState(_T("image"), pState);
		else
		if (sType == _T("sli_color"))
			return m_Slider.SetState(_T("color"), pState);
		else
		{
			if (!CGuiPicture::SetState(sType, pState))
				return m_Slider.SetState(sType, pState);
			else
				return true;
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiSlider, CGuiPicture)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiScroll_hpp__*/