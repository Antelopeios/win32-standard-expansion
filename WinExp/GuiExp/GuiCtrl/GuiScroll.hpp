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
// Date:	2012-03-12
// Version:	1.0.0003.1447
//
// History:
//	- 1.0.0000.0912(2011-08-02)	@ 准备构建GuiScroll
//	- 1.0.0001.1816(2011-08-24)	+ 添加GuiScroll的main属性,方便在GuiScroll内部获取其关联的控件指针
//	- 1.0.0002.1517(2011-09-28)	# 修正当外部销毁控件对象时,GuiScroll因内部对象析构顺序问题导致的内存访问异常
//	- 1.0.0003.1447(2012-03-12)	= GuiScroll内部的按钮采用CGuiSimpBtn
//								# 修正滚动条无限循环绘图导致CPU居高不下的bug
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
	CGuiSimpBtn m_Slider;
	LONG m_All/*全部内容长度*/, m_Fra/*片段长度*/, m_Pos/*位置*/;
	BOOL m_Ori; // 方向

public:
	CGuiSlider()
		: m_All(0)
		, m_Fra(0)
		, m_Pos(0)
		, m_Ori(FALSE)
	{
		// 添加事件对象
		InsEvent(ExGui(_T("CGuiSliderEvent"), GetGC())); /*先让基类绘图*/
		m_Slider.AddEvent(ExGui(_T("CGuiSliBlkEvent"), m_Slider.GetGC()));
		// 添加控件对象
		InsComp(&m_Slider);
	}
	~CGuiSlider()
	{
		DelComp(&m_Slider, FALSE);
	}

public:
	void Format()
	{
		if (m_Pos > m_All - m_Fra) m_Pos = m_All - m_Fra;
		if (m_Pos < 0) m_Pos = 0;
		SendMessage(WM_SIZE, SIZE_RESTORED, 
			(LPARAM)ExMakeLong(m_Rect.Width(), m_Rect.Height()));
		IGuiCtl* pare = ExDynCast<IGuiCtl>(GetParent());
		if (pare)
			pare->SendMessage(WM_COMMAND, SB_THUMBPOSITION);
	}

	BOOL Execute(const CString& key, const CString& val)
	{
		if (key == _T("all"))
			SetState(_T("all"), (void*)_ttol(val));
		else
		if (key == _T("pos"))
			SetState(_T("pos"), (void*)_ttol(val));
		else
		if (key == _T("ori"))
		{
			CString temp(val);
			temp.Lower();
			if (temp == _T("false"))
				SetState(_T("ori"), (void*)FALSE);
			else
			if (temp == _T("true"))
				SetState(_T("ori"), (void*)TRUE);
		}
		else
		if (key.Left(4) == _T("blk_"))
		{
			CString type(key);
			type.TrimLeft(_T("blk_"));
			m_Slider.Execute(type, val);
		}
		else
			return EXP_BASE::Execute(key, val);
		return IGuiCtrlBase::Execute(key, val);
	}

	void* GetState(const CString& sType, void* pParam = NULL)
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
			return EXP_BASE::GetState(sType);
	}
	BOOL SetState(const CString& sType, void* pState, void* pParam = NULL)
	{
		if (sType == _T("all"))
			SET_STATE(LONG, m_All, Format())
		else
		if (sType == _T("fra"))
			SET_STATE(LONG, m_Fra, Format())
		else
		if (sType == _T("pos"))
			SET_STATE(LONG, m_Pos, Format())
		else
		if (sType == _T("ori"))
			SET_STATE(BOOL, m_Ori)
		else
		if (sType.Left(4) == _T("blk_"))
		{
			CString type(sType);
			type.TrimLeft(_T("blk_"));
			return m_Slider.SetState(type, pState);
		}
		else
			return EXP_BASE::SetState(sType, pState);
	}
};

//////////////////////////////////////////////////////////////////

class CGuiScroll : public IGuiCtrlBase
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiScroll, IGuiCtrlBase)

protected:
	CGuiSlider m_Slider;
	CGuiSimpBtn m_Up, m_Down;
	IGuiCtl* m_Main;

public:
	CGuiScroll()
		: m_Main(NULL)
	{
		// 添加事件对象
		InsEvent(ExGui(_T("CGuiScrollEvent"), GetGC())); /*先让基类绘图*/
		m_Up.InsEvent(ExGui(_T("CGuiScrUpEvent"), m_Up.GetGC()));
		m_Down.InsEvent(ExGui(_T("CGuiScrDnEvent"), m_Down.GetGC()));
		// 添加控件对象
		InsComp(&m_Slider);
		m_Up.SetWindowRect(CRect(0, 0, 20, 20));
		InsComp(&m_Up);
		m_Down.SetWindowRect(CRect(0, 0, 20, 20));
		InsComp(&m_Down);
		// 调整默认属性
		SetState(_T("sli_color"), (void*)ExRGBA(220, 220, 220, EXP_CM));
		pixel_t pix[8] = {0};
		CImgASM::PixSetP(pix, _countof(pix), ExRGBA(120, 120, 120, EXP_CM));
		SetState(_T("sli_blk_color"), pix);
		CImgASM::PixSetP(pix, _countof(pix), ExRGBA(180, 180, 180, EXP_CM));
		SetState(_T("up_color"), pix);
		SetState(_T("dn_color"), pix);
	}
	~CGuiScroll()
	{
		DelComp(&m_Slider, FALSE);
		DelComp(&m_Up, FALSE);
		DelComp(&m_Down, FALSE);
	}

public:
	BOOL Execute(const CString& key, const CString& val)
	{
		if (key == _T("scroll"))
		{
			IGuiCtl* main = ExGet<IGuiCtl>(val);
			if (main) main->SetScroll(this);
		}
		else
		if (key.Left(4) == _T("sli_"))
		{
			CString type(key);
			type.TrimLeft(_T("sli_"));
			m_Slider.Execute(type, val);
		}
		else
		if (key.Left(3) == _T("up_"))
		{
			CString type(key);
			type.TrimLeft(_T("up_"));
			m_Up.Execute(type, val);
		}
		else
		if (key.Left(3) == _T("dn_"))
		{
			CString type(key);
			type.TrimLeft(_T("dn_"));
			m_Down.Execute(type, val);
		}
		return TRUE;
	}

	void* GetState(const CString& sType, void* pParam = NULL)
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
	BOOL SetState(const CString& sType, void* pState, void* pParam = NULL)
	{
		CString type(sType);
		if (sType == _T("main"))
			SET_STATE(IGuiCtl*, m_Main)
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
		return FALSE;
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiSlider, CGuiPicture)
EXP_IMPLEMENT_DYNCREATE_MULT(CGuiScroll, IGuiCtrlBase)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiScroll_hpp__*/