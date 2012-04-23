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
// GuiButton - 按钮控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-04-11
// Version:	1.0.0012.1700
//
// History:
//	- 1.0.0001.2305(2011-05-25)	+ CGuiButton添加状态属性
//								= CGuiButton::m_Status初始化为nor
//								= CGuiButton不再托管内部的图片资源
//	- 1.0.0002.1310(2011-07-01)	+ CGuiButton添加locate属性,支持自定义文字显示位置
//	- 1.0.0003.1648(2011-07-06)	+ CGuiButton添加thr_sta属性,支持三态按钮
//	- 1.0.0004.1752(2011-07-11)	+ 添加CGuiPushBtn控件及其对应的事件响应
//	- 1.0.0005.1030(2011-08-12)	+ CGuiButton支持单态按钮
//	- 1.0.0006.1640(2011-08-15)	+ 在CGuiButton中实现icon相关属性
//								# 加上icon相关属性的初始化操作
//	- 1.0.0007.1513(2011-09-22)	= CGuiButton按钮状态改为8态
//	- 1.0.0008.1216(2011-09-23)	+ 添加shake_ico属性,支持点击时晃动图标及文字
//	- 1.0.0009.2025(2012-03-01) + GuiButton支持通过字符串匹配对应的locate属性
//	- 1.0.0010.1443(2012-03-08) ^ 将CGuiPushBtn继承于GuiButton,其中的特殊处理一并放入GuiButton中
//	- 1.0.0011.1931(2012-03-11) ^ 优化GuiButton的内部设定,Execute的某些设置项支持直接用中文定义整型属性
//								+ GuiButton支持勾选按钮
//	- 1.0.0012.1700(2012-04-11) + 添加comf属性,支持设置文字与字体时自动调整按钮大小以符合文字内容
//////////////////////////////////////////////////////////////////

#ifndef __GuiButton_hpp__
#define __GuiButton_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////
// 按钮属性对象
//////////////////////////////////////////////////////////////////

class _btn_style : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_style, IGuiSet)

public:
	CString GetKey() const { return _T("style"); }
	BOOL Exc(const CString& val)
	{
		style_t* sty = ExGet<style_t>(val);
		if(!sty) return TRUE;
		if(!sty->font.Empty())
			Ctl()->SetState(_T("font"), (void*)(CText**)sty->font, (void*)TRUE);
		if(!sty->color.Empty())
			Ctl()->SetState(_T("color"), (void*)(pixel_t*)sty->color);
		if(!sty->image.Empty())
			Ctl()->SetState(_T("image"), (void*)(CImage**)sty->image, (void*)TRUE);
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_style, IGuiSet)

//////////////////////////////////////////////////////////////////

class _btn_font : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_font, IGuiSet)

protected:
	CText* m_Text[10];

public:
	_btn_font()
	{ ZeroMemory(m_Text, sizeof(m_Text)); }

public:
	CString GetKey() const { return _T("font"); }
	BOOL Exc(const CString& val)
	{
		CArrayT<CString> sa;
		ExStringToArray(val, sa);
		for(int i = 0; i < (int)min(_countof(m_Text), sa.GetCount()); ++i)
			m_Text[i] = ExGet<CText>(sa[i]);
		Ctl()->UpdateState();
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		return (void*)m_Text;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		if (par)
		{
			for(int i = 0; i < _countof(m_Text); ++i)
				m_Text[i] = ((CText**)sta)[i];
		}
		else
		{
			for(int i = 0; i < _countof(m_Text); ++i)
				m_Text[i] = (CText*)sta + i;
		}
		BOOL comf = (BOOL)Ctl()->GetState(_T("comf"));
		if (comf)
		{
			DWORD status = (DWORD)Ctl()->GetState(_T("status"));
			CText* text = m_Text[status];
			CString* str = (CString*)Ctl()->GetState(_T("text"));
			if (!text || !str) return TRUE;
			CSize sz;
			text->GetSize(*str, sz);
			CRect rc;
			Ctl()->GetRect(rc);
			rc.Width(sz.cx + 4);
			rc.Height(sz.cy + 4);
			Ctl()->SetRect(rc);
		}
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_font, IGuiSet)
	
//////////////////////////////////////////////////////////////////

class _btn_text : public _pic_text
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_text, _pic_text)

public:
	BOOL Set(void* sta, void* par = NULL)
	{
		_pic_text::Set(sta, par);
		BOOL comf = (BOOL)Ctl()->GetState(_T("comf"));
		if (comf)
		{
			DWORD status = (DWORD)Ctl()->GetState(_T("status"));
			CText* text = ((CText**)Ctl()->GetState(_T("font")))[status];
			CString* str = &m_Val;
			if (!text || !str) return TRUE;
			CSize sz;
			text->GetSize(*str, sz);
			CRect rc;
			Ctl()->GetRect(rc);
			rc.Width(sz.cx + 4);
			rc.Height(sz.cy + 4);
			Ctl()->SetRect(rc);
		}
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_text, _pic_text)

//////////////////////////////////////////////////////////////////

class _btn_color : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_color, IGuiSet)

protected:
	pixel_t m_Color[10];

public:
	_btn_color()
	{ ZeroMemory(m_Color, sizeof(m_Color)); }

public:
	CString GetKey() const { return _T("color"); }
	BOOL Exc(const CString& val)
	{
		CArrayT<CString> sa;
		ExStringToArray(val, sa);
		for(int i = 0; i < (int)min(_countof(m_Color), sa.GetCount()); ++i)
			m_Color[i] = ExStringToColor(sa[i]);
		Ctl()->UpdateState();
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		return (void*)m_Color;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		for(int i = 0; i < _countof(m_Color); ++i)
			m_Color[i] = ((pixel_t*)sta)[i];
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_color, IGuiSet)

//////////////////////////////////////////////////////////////////

class _btn_image : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_image, IGuiSet)

protected:
	CImage* m_Image[9];	// 九宫格分割,每个小块保存所有状态

public:
	_btn_image()
	{ ZeroMemory(m_Image, sizeof(m_Image)); }

public:
	CString GetKey() const { return _T("image"); }
	BOOL Exc(const CString& val)
	{
		CArrayT<CString> sa;
		ExStringToArray(val, sa);
		for(int i = 0; i < (int)min(_countof(m_Image), sa.GetCount()); ++i)
			m_Image[i] = ExGet<CImage>(sa[i]);
		Ctl()->UpdateState();
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		return (void*)m_Image;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		if (par)
		{
			for(int i = 0; i < _countof(m_Image); ++i)
				m_Image[i] = ((CImage**)sta)[i];
		}
		else
		{
			for(int i = 0; i < _countof(m_Image); ++i)
				m_Image[i] = (CImage*)sta + i;
		}
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_image, IGuiSet)

//////////////////////////////////////////////////////////////////

class _btn_status : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_status, IGuiSet)

public:
	/*
		正常; 浮动; 按下; push
	*/
	enum status_t {nor, ovr, prs, psh};
protected:
	status_t m_Status;	// 按钮状态

public:
	_btn_status()
		: m_Status(nor)
	{}

public:
	CString GetKey() const { return _T("status"); }
	BOOL Exc(const CString& val)
	{
		if (val == _T("nor") || val == _T("normal"))
			Set((void*)nor);
		else
		if (val == _T("ovr") || val == _T("over"))
			Set((void*)ovr);
		else
		if (val == _T("prs") || val == _T("press"))
			Set((void*)prs);
		else
		if (val == _T("psh") || val == _T("push"))
			Set((void*)psh);
		else
			Set((void*)_ttol(val));
		Ctl()->UpdateState();
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		int btn_sty = (int)Ctl()->GetState(_T("btn_sty"));
		if (btn_sty < 0 || btn_sty > 2) return NULL;
		LONG sta_tim = (btn_sty == 1 ? 10 : 8), nfc_tim = sta_tim >> 1;
		UINT thr_sta = (UINT)Ctl()->GetState(_T("thr_sta"));
		if (thr_sta & 0x01)				// ove
			nfc_tim -= 1;
		if (thr_sta & 0x02)				// prs
			nfc_tim -= 1;
		if (thr_sta & 0x04)				// dis
			nfc_tim -= 1;
		if (thr_sta & 0x08)				// foc
			sta_tim = nfc_tim;
		else
			sta_tim = nfc_tim << 1;
		DWORD status = (DWORD)m_Status;
		if(!Ctl()->IsEnabled())			// dis
		{
			if (thr_sta & 0x04)
				status = 0;
			else
				status = nfc_tim;
		}
		else
		if (status == 1)				// ove
		{
			if (thr_sta & 0x01)
				status = 0;
		}
		else
		if (status == 2)				// prs
		{
			if (thr_sta & 0x02)
				status = 0;
			else
			if (thr_sta & 0x01)
				--status;
		}
		else
		if (status == 3)				// psh
		{
			if (thr_sta & 0x02)
				--status;
			if (thr_sta & 0x01)
				--status;
		}
		if (Ctl()->IsFocus())			// foc
		{
			if(!(thr_sta & 0x08))
				status += nfc_tim;
		}
		return (void*)status;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		m_Status = (status_t)(LONG)sta;
		return TRUE;
	}

	void BtnDwn()
	{
		if (m_Status == psh) return;
		m_Status = prs;
		Ctl()->UpdateState();
	}
	void BtnUp()
	{
		if (m_Status == psh) return;

		IGuiWnd* wnd = Ctl()->GetWnd();
		if (!wnd) return;
		POINT pt_tmp = {0};
		::GetCursorPos(&pt_tmp);
		CPoint pt(pt_tmp);
		wnd->ScreenToClient(pt);
		CRect rc;
		Ctl()->GetRealRect(rc);

		if (m_Status == prs) // 当按下后抬起,视为一次Click
		{
			Ctl()->SendMessage(BM_CLICK);
			if(!Ctl()->IsValid()) return;
		}

		if (m_Status == psh) return;

		if (rc.PtInRect(pt))
			m_Status = ovr;
		else
			m_Status = nor;

		Ctl()->UpdateState();
	}
	void MusMov()
	{
		if (m_Status == psh || m_Status == prs) return;
		m_Status = ovr;
		Ctl()->UpdateState();
	}
	void MusLev()
	{
		if (m_Status == psh) return;
		m_Status = nor;
		Ctl()->UpdateState();
	}

	void Msg(UINT nMessage, WPARAM wParam, LPARAM lParam)
	{
		switch( nMessage )
		{
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			MusMov();
			break;
		case WM_KEYDOWN:
			if (wParam != VK_RETURN) break; // Enter
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			BtnDwn();
			break;
		case WM_KEYUP:
			if (wParam != VK_RETURN) break; // Enter
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			BtnUp();
			break;
		case WM_MOUSELEAVE:
			MusLev();
			break;
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_status, IGuiSet)

//////////////////////////////////////////////////////////////////

class _btn_locate : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_locate, IGuiSet)

public:
	/*
		文字位置: 中; 上; 下; 左; 右
	*/
	enum locate_t {center, top, bottom, left, right};
protected:
	locate_t m_Locate;	// 文字位置

public:
	_btn_locate()
		: m_Locate(center)
	{}

public:
	CString GetKey() const { return _T("locate"); }
	BOOL Exc(const CString& val)
	{
		if (val == _T("center"))
			Set((void*)center);
		else
		if (val == _T("top"))
			Set((void*)top);
		else
		if (val == _T("bottom"))
			Set((void*)bottom);
		else
		if (val == _T("left"))
			Set((void*)left);
		else
		if (val == _T("right"))
			Set((void*)right);
		else
			Set((void*)_ttol(val));
		Ctl()->UpdateState();
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		return (void*)m_Locate;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		m_Locate = (locate_t)(LONG)sta;
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_locate, IGuiSet)

//////////////////////////////////////////////////////////////////

// 文字位置偏移
class _btn_loc_off : public ICtrlSetT<LONG>
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_loc_off, IGuiSet)

public:
	CString GetKey() const { return _T("loc_off"); }
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_loc_off, IGuiSet)

//////////////////////////////////////////////////////////////////

// 图标点击是否摇晃
class _btn_shake_ico : public ICtrlSetT<BOOL>
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_shake_ico, IGuiSet)

public:
	CString GetKey() const { return _T("shake_ico"); }
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_shake_ico, IGuiSet)
	
//////////////////////////////////////////////////////////////////

// 按钮状态屏蔽选项
class _btn_thr_sta : public ICtrlSetT<UINT>
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_thr_sta, IGuiSet)

public:
	CString GetKey() const { return _T("thr_sta"); }
	BOOL Exc(const CString& val)
	{
		CArrayT<CString> sa;
		int sta_i = _ttol(val);
		if (sta_i == 0 && val != _T("0"))
		{
			ExStringToArray(val, sa);
			for(int i = 0; i < (int)sa.GetCount(); ++i)
			{
				if (sa[i] == _T("0x01") || sa[i] == _T("NO_OVER"))
					sta_i |= 0x01;
				else
				if (sa[i] == _T("0x02") || sa[i] == _T("NO_PRESS"))
					sta_i |= 0x02;
				else
				if (sa[i] == _T("0x04") || sa[i] == _T("NO_DISABLE"))
					sta_i |= 0x04;
				else
				if (sa[i] == _T("0x08") || sa[i] == _T("NO_FOCUS"))
					sta_i |= 0x08;
				else
				if (sa[i] == _T("0x10") || sa[i] == _T("NO_MID_CHECK"))
					sta_i |= 0x10;
			}
		}
		Set((void*)sta_i);
		Ctl()->UpdateState();
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_thr_sta, IGuiSet)


//////////////////////////////////////////////////////////////////

class _btn_btn_sty : public IGuiSet
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_btn_sty, IGuiSet)

public:
	/*
		按钮类型
	*/
	enum styles_t
	{
		nor_btn,	// 普通按钮
		psh_btn,	// 下压按钮
		check_box	// 勾选按钮
	};
protected:
	styles_t m_BtnSty;	// 按钮状态

public:
	_btn_btn_sty()
		: m_BtnSty(nor_btn)
	{}

public:
	CString GetKey() const { return _T("btn_sty"); }
	BOOL Exc(const CString& val)
	{
		if (val == _T("nor_btn"))
			Set((void*)nor_btn);
		else
		if (val == _T("psh_btn"))
			Set((void*)psh_btn);
		else
		if (val == _T("check_box"))
			Set((void*)check_box);
		else
			Set((void*)_ttol(val));
		Ctl()->UpdateState();
		return TRUE;
	}
	void* Get(void* par = NULL)
	{
		return (void*)m_BtnSty;
	}
	BOOL Set(void* sta, void* par = NULL)
	{
		m_BtnSty = (styles_t)(LONG)sta;
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_btn_sty, IGuiSet)

//////////////////////////////////////////////////////////////////

// 勾选状态
class _btn_chk_sta : public ICtrlSetT<int>
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_chk_sta, IGuiSet)

public:
	CString GetKey() const { return _T("chk_sta"); }
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_chk_sta, IGuiSet)

//////////////////////////////////////////////////////////////////

// 按钮图标
class _btn_icon : public ICtrlSetT<CImage*>
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_icon, IGuiSet)

public:
	CString GetKey() const { return _T("icon"); }
	BOOL Exc(const CString& val)
	{
		Set(ExGet<CImage>(val));
		Ctl()->UpdateState();
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_icon, IGuiSet)

//////////////////////////////////////////////////////////////////

// 图标位置偏移
class _btn_ico_off : public ICtrlSetT<LONG>
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_ico_off, IGuiSet)

public:
	CString GetKey() const { return _T("ico_off"); }
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_ico_off, IGuiSet)
	
//////////////////////////////////////////////////////////////////

// 让按钮控件的大小与文字内容一致
class _btn_comf : public ICtrlSetT<BOOL>
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_comf, IGuiSet)

public:
	CString GetKey() const { return _T("comf"); }
	BOOL Set(void* sta, void* par = NULL)
	{
		ICtrlSetT<BOOL>::Set(sta, par);
		if (m_Val)
		{
			DWORD status = (DWORD)Ctl()->GetState(_T("status"));
			CText* text = ((CText**)Ctl()->GetState(_T("font")))[status];
			CString* str = (CString*)Ctl()->GetState(_T("text"));
			if (!text || !str) return TRUE;
			CSize sz;
			text->GetSize(*str, sz);
			CRect rc;
			Ctl()->GetRect(rc);
			rc.Width(sz.cx + 4);
			rc.Height(sz.cy + 4);
			Ctl()->SetRect(rc);
		}
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_comf, IGuiSet)

//////////////////////////////////////////////////////////////////
// 按钮对象
//////////////////////////////////////////////////////////////////

class CGuiButton : public IGuiCtl
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiButton, IGuiCtl)

public:
	CGuiButton()
	{
		// 添加逻辑对象
		AddSet(_T("_btn_comf"));
		AddSet(_T("_btn_style"));
		AddSet(_T("_btn_color"));
		AddSet(_T("_btn_image"));
		AddSet(_T("_btn_font"));
		AddSet(_T("_btn_text"));
		AddSet(_T("_btn_status"));
		AddSet(_T("_btn_locate"));
		AddSet(_T("_btn_loc_off"));
		AddSet(_T("_btn_shake_ico"));
		AddSet(_T("_btn_thr_sta"));
		AddSet(_T("_btn_btn_sty"));
		AddSet(_T("_btn_chk_sta"));
		AddSet(_T("_btn_icon"));
		AddSet(_T("_btn_ico_off"));
		// 添加事件对象
		AddEvent(_T("CGuiButtonEvent"));
	}
};

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiButton, IGuiCtl)

//////////////////////////////////////////////////////////////////

class CGuiStatic : public CGuiButton
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiStatic, CGuiButton)

public:
	CGuiStatic()
	{
		SetState(_T("thr_sta"), (void*)(0x01 | 0x02 | 0x04 | 0x08));
	}
};

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiStatic, CGuiButton)

//////////////////////////////////////////////////////////////////

class CGuiSimpBtn : public CGuiButton
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiSimpBtn, CGuiButton)

public:
	CGuiSimpBtn()
	{
		SetState(_T("thr_sta"), (void*)(0x04 | 0x08));
	}
};

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiSimpBtn, CGuiButton)

//////////////////////////////////////////////////////////////////

class CGuiPushBtn : public CGuiSimpBtn
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiPushBtn, CGuiSimpBtn)

public:
	CGuiPushBtn()
	{
		SetState(_T("btn_sty"), (void*)1);
	}
};

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiPushBtn, CGuiSimpBtn)

//////////////////////////////////////////////////////////////////

class CGuiCheckBox : public CGuiButton
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiCheckBox, CGuiButton)

public:
	CGuiCheckBox()
	{
		SetState(_T("btn_sty"), (void*)2);
	}
};

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiCheckBox, CGuiButton)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiButton_hpp__*/