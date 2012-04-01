// Copyright 2011-2012, ľͷ��
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
// GuiButton - ��ť�ؼ�
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-11
// Version:	1.0.0011.1931
//
// History:
//	- 1.0.0001.2305(2011-05-25)	+ CGuiButton���״̬����
//								= CGuiButton::m_Status��ʼ��Ϊnor
//								= CGuiButton�����й��ڲ���ͼƬ��Դ
//	- 1.0.0002.1310(2011-07-01)	+ CGuiButton���locate����,֧���Զ���������ʾλ��
//	- 1.0.0003.1648(2011-07-06)	+ CGuiButton���thr_sta����,֧����̬��ť
//	- 1.0.0004.1752(2011-07-11)	+ ���CGuiPushBtn�ؼ������Ӧ���¼���Ӧ
//	- 1.0.0005.1030(2011-08-12)	+ CGuiButton֧�ֵ�̬��ť
//	- 1.0.0006.1640(2011-08-15)	+ ��CGuiButton��ʵ��icon�������
//								# ����icon������Եĳ�ʼ������
//	- 1.0.0007.1513(2011-09-22)	= CGuiButton��ť״̬��Ϊ8̬
//	- 1.0.0008.1216(2011-09-23)	+ ���shake_ico����,֧�ֵ��ʱ�ζ�ͼ�꼰����
//	- 1.0.0009.2025(2012-03-01) + GuiButton֧��ͨ���ַ���ƥ���Ӧ��locate����
//	- 1.0.0010.1443(2012-03-08) ^ ��CGuiPushBtn�̳���GuiButton,���е����⴦��һ������GuiButton��
//	- 1.0.0011.1931(2012-03-11) ^ �Ż�GuiButton���ڲ��趨,Execute��ĳЩ������֧��ֱ�������Ķ�����������
//								+ GuiButton֧�ֹ�ѡ��ť
//////////////////////////////////////////////////////////////////

#ifndef __GuiButton_hpp__
#define __GuiButton_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////
// ��ť���Զ���
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
		return TRUE;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_font, IGuiSet)

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
	CImage* m_Image[9];	// �Ź���ָ�,ÿ��С�鱣������״̬

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
		����; ����; ����; push
	*/
	enum status_t {nor, ovr, prs, psh};
protected:
	status_t m_Status;	// ��ť״̬

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
		return (void*)m_Status;
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

		if (m_Status == prs) // �����º�̧��,��Ϊһ��Click
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
		����λ��: ��; ��; ��; ��; ��
	*/
	enum locate_t {center, top, bottom, left, right};
protected:
	locate_t m_Locate;	// ����λ��

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

// ����λ��ƫ��
class _btn_loc_off : public ICtrlSetT<LONG>
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_loc_off, IGuiSet)

public:
	CString GetKey() const { return _T("loc_off"); }
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_loc_off, IGuiSet)

//////////////////////////////////////////////////////////////////

// ͼ�����Ƿ�ҡ��
class _btn_shake_ico : public ICtrlSetT<BOOL>
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_shake_ico, IGuiSet)

public:
	CString GetKey() const { return _T("shake_ico"); }
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_shake_ico, IGuiSet)
	
//////////////////////////////////////////////////////////////////

// ��ť״̬����ѡ��
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
		��ť����
	*/
	enum styles_t
	{
		nor_btn,	// ��ͨ��ť
		psh_btn,	// ��ѹ��ť
		check_box	// ��ѡ��ť
	};
protected:
	styles_t m_BtnSty;	// ��ť״̬

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

// ��ѡ״̬
class _btn_chk_sta : public ICtrlSetT<int>
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_chk_sta, IGuiSet)

public:
	CString GetKey() const { return _T("chk_sta"); }
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_chk_sta, IGuiSet)

//////////////////////////////////////////////////////////////////

// ��ťͼ��
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

// ͼ��λ��ƫ��
class _btn_ico_off : public ICtrlSetT<LONG>
{
	EXP_DECLARE_DYNCREATE_CLS(_btn_ico_off, IGuiSet)

public:
	CString GetKey() const { return _T("ico_off"); }
};

EXP_IMPLEMENT_DYNCREATE_CLS(_btn_ico_off, IGuiSet)

//////////////////////////////////////////////////////////////////
// ��ť����
//////////////////////////////////////////////////////////////////

class CGuiButton : public IGuiCtrlBase
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiButton, IGuiCtrlBase)

public:
	CGuiButton()
	{
		// ����߼�����
		AddSet(_T("_btn_style"));
		AddSet(_T("_btn_color"));
		AddSet(_T("_btn_image"));
		AddSet(_T("_btn_font"));
		AddSet(_T("_pic_text"));
		AddSet(_T("_btn_status"));
		AddSet(_T("_btn_locate"));
		AddSet(_T("_btn_loc_off"));
		AddSet(_T("_btn_shake_ico"));
		AddSet(_T("_btn_thr_sta"));
		AddSet(_T("_btn_btn_sty"));
		AddSet(_T("_btn_chk_sta"));
		AddSet(_T("_btn_icon"));
		AddSet(_T("_btn_ico_off"));
		// ����¼�����
		AddEvent(_T("CGuiButtonEvent"));
	}
};

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiButton, IGuiCtrlBase)

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