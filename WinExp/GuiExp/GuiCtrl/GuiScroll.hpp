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
// Date:	2011-09-28
// Version:	1.0.0002.1517
//
// History:
//	- 1.0.0000.0912(2011-08-02)	@ 准备构建GuiScroll
//	- 1.0.0001.1816(2011-08-24)	+ 添加GuiScroll的main属性,方便在GuiScroll内部获取其关联的控件指针
//	- 1.0.0002.1517(2011-09-28)	# 修正当外部销毁控件对象时,GuiScroll因内部对象析构顺序问题导致的内存访问异常
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
		m_Slider.AddEvent((IGuiEvent*)ExGui(_T("CGuiSliBlkEvent"), m_Slider.GetGC()));
		// 添加控件对象
		InsComp(&m_Slider);
	}
	~CGuiSlider()
	{
		DelComp(&m_Slider, false);
	}

public:
	void Format()
	{
		if (m_Pos > m_All - m_Fra) m_Pos = m_All - m_Fra;
		if (m_Pos < 0) m_Pos = 0;
		IGuiCtrl* pare = ExDynCast<IGuiCtrl>(GetParent());
		if (pare)
			pare->Send(ExDynCast<IGuiObject>(pare), WM_COMMAND, SB_THUMBPOSITION);
	}

	void* GetState(const CString& sType)
	{
		if (sType.Left(4) == _T("blk_"))
		{
			CString type(sType);
			type.TrimLeft(_T("blk_"));
			return m_Slider.GetState(type);
		}
		else
		if (sType == _T("slider"))
			return (void*)(&m_Slider);
		if (sType == _T("all"))
			return (void*)m_All;
		else
		if (sType == _T("fra"))
			return (void*)m_Fra;
		else
		if (sType == _T("pos"))
			return (void*)m_Pos;
		else
		if (sType == _T("ori"))
			return (void*)m_Ori;
		else
			return CGuiPicture::GetState(sType);
	}
	bool SetState(const CString& sType, void* pState)
	{
		if (sType == _T("all"))
		{
			if (m_All != (LONG)(LONG_PTR)pState)
			{
				m_All = (LONG)(LONG_PTR)pState;
				Format();
			}
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("fra"))
		{
			if (m_Fra != (LONG)(LONG_PTR)pState)
			{
				m_Fra = (LONG)(LONG_PTR)pState;
				Format();
			}
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("pos"))
		{
			if (m_Pos != (LONG)(LONG_PTR)pState)
			{
				m_Pos = (LONG)(LONG_PTR)pState;
				Format();
			}
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("ori"))
		{
			m_Ori = (bool)(LONG_PTR)pState;
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType.Left(4) == _T("blk_"))
		{
			CString type(sType);
			type.TrimLeft(_T("blk_"));
			return m_Slider.SetState(type, pState);
		}
		else
			return CGuiPicture::SetState(sType, pState);
	}
};

//////////////////////////////////////////////////////////////////

class CGuiScroll : public IGuiCtrlBase
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiScroll, IGuiCtrlBase)

protected:
	CGuiSlider m_Slider;
	CGuiButton m_Up, m_Down;
	IGuiCtrl* m_Main;

public:
	CGuiScroll()
		: m_Main(NULL)
	{
		// 添加事件对象
		InsEvent((IGuiEvent*)ExGui(_T("CGuiScrollEvent"), GetGC())); /*先让基类绘图*/
		m_Up.InsEvent((IGuiEvent*)ExGui(_T("CGuiScrUpEvent"), m_Up.GetGC()));
		m_Down.InsEvent((IGuiEvent*)ExGui(_T("CGuiScrDnEvent"), m_Down.GetGC()));
		// 添加控件对象
		InsComp(&m_Slider);
		m_Up.SetWindowRect(CRect(0, 0, 20, 20));
		InsComp(&m_Up);
		m_Down.SetWindowRect(CRect(0, 0, 20, 20));
		InsComp(&m_Down);
	}
	~CGuiScroll()
	{
		DelComp(&m_Slider, false);
		DelComp(&m_Up, false);
		DelComp(&m_Down, false);
	}

public:
	void* GetState(const CString& sType)
	{
		CString type(sType);
		if (sType.Left(4) == _T("sli_"))
		{
			type.TrimLeft(_T("sli_"));
			return m_Slider.GetState(type);
		}
		else
		if (sType.Left(3) == _T("up_"))
		{
			type.TrimLeft(_T("up_"));
			return m_Up.GetState(type);
		}
		else
		if (sType.Left(3) == _T("dn_"))
		{
			type.TrimLeft(_T("dn_"));
			return m_Down.GetState(type);
		}
		else
		if (sType == _T("main"))
			return (void*)m_Main;
		else
		if (sType == _T("slider"))
			return (void*)(&m_Slider);
		else
		if (sType == _T("up"))
			return (void*)(&m_Up);
		else
		if (sType == _T("down"))
			return (void*)(&m_Down);
		else
			return EXP_BASE::GetState(sType);
	}
	bool SetState(const CString& sType, void* pState)
	{
		CString type(sType);
		if (sType == _T("main"))
		{
			IGuiCtrl* old_sta = m_Main;
			m_Main = (IGuiCtrl*)pState;
			if (old_sta != m_Main)
				return IGuiCtrlBase::SetState(sType, pState);
			else
				return true;
		}
		else
		if (type.Left(4) == _T("sli_"))
		{
			type.TrimLeft(_T("sli_"));
			return m_Slider.SetState(type, pState);
		}
		else
		if (type.Left(3) == _T("up_"))
		{
			type.TrimLeft(_T("up_"));
			return m_Up.SetState(type, pState);
		}
		else
		if (type.Left(3) == _T("dn_"))
		{
			type.TrimLeft(_T("dn_"));
			return m_Down.SetState(type, pState);
		}
		return false;
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiSlider, CGuiPicture)
EXP_IMPLEMENT_DYNCREATE_MULT(CGuiScroll, IGuiCtrlBase)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiScroll_hpp__*/