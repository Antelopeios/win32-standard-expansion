// Copyright 2011, ľͷ��
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
// Date:	2011-09-23
// Version:	1.0.0008.1216
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
//////////////////////////////////////////////////////////////////

#ifndef __GuiButton_hpp__
#define __GuiButton_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiButton : public IGuiCtrlBase
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiButton, IGuiCtrlBase)

public:
	/*
		����; ����; ����
	*/
	enum status_t {nor, ovr, hit};
	/*
		����λ��: ��; ��; ��; ��; ��
	*/
	enum locate_t {center, top, bottom, left, right};

protected:
	status_t m_Status;	// ��ť״̬
	locate_t m_Locate;	// ����λ��
	LONG m_LocOff;		// ����λ��ƫ��(m_Locate == center ʱ��Ч)

	CImage m_Icon;
	BOOL m_bGlow;		// �Ƿ����ͼ���ⷢ��
	LONG m_IcoOff;		// ͼ��λ��ƫ��(m_Locate == center ʱ��Ч)
	int m_ShakeIco;		// ͼ����ҡ��

	int m_ThreeSta;		// �Ƿ�����̬��ť
	pixel_t m_Color[8];
	CImage m_Image[9];	// �Ź���ָ�,ÿ��С�鱣��8��״̬
	CText m_Text[8];

public:
	CGuiButton()
		: m_Status(nor)
		, m_Locate(center)
		, m_LocOff(5)
		, m_bGlow(FALSE)
		, m_IcoOff(5)
		, m_ShakeIco(0)
		, m_ThreeSta(0)
	{
		ZeroMemory(m_Color, sizeof(m_Color));
		// ����¼�����
		AddEvent((IGuiEvent*)ExGui(_T("CGuiButtonEvent"), GetGC()));
	}
	~CGuiButton()
	{}

public:
	void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent)
	{
		style_t* sty = CGuiManagerT<style_t>::Get(xml.GetAttr(_T("style"), ite));
		if (sty)
		{
			if(!sty->font.Empty())
			{
				for(int i = 0; i < (int)min(_countof(m_Text), sty->font.GetCount()); ++i)
					m_Text[i] = *(sty->font[i]);
			}
			if(!sty->color.Empty())
			{
				for(int i = 0; i < (int)min(_countof(m_Color), sty->color.GetCount()); ++i)
					m_Color[i] = sty->color[i];
			}
			if(!sty->image.Empty())
			{
				for(int i = 0; i < (int)min(_countof(m_Image), sty->image.GetCount()); ++i)
					m_Image[i] = *(sty->image[i]);
			}
		}
		m_Text[0].SetString(xml.GetAttr(_T("text"), ite));
		for(int i = 1; i < _countof(m_Text); ++i)
			m_Text[i].SetString(m_Text[0]);
		SetWindowRect(StringToRect(xml.GetAttr(_T("rect"), ite)));
		return this;
	}

	// ��ÿؼ�״̬
	void* GetState(const CString& sType)
	{
		if (sType == _T("status"))
			return (void*)m_Status;
		else
		if (sType == _T("locate"))
			return (void*)m_Locate;
		else
		if (sType == _T("loc_off"))
			return (void*)m_LocOff;
		else
		if (sType == _T("shake_ico"))
			return (void*)m_ShakeIco;
		else
		if (sType == _T("thr_sta"))
			return (void*)m_ThreeSta;
		else
		if (sType == _T("color"))
			return (void*)m_Color;
		else
		if (sType == _T("image"))
			return (void*)m_Image;
		else
		if (sType == _T("text"))
			return (void*)m_Text;
		else
		if (sType == _T("icon"))
			return (void*)(&m_Icon);
		else
		if (sType == _T("ico_off"))
			return (void*)m_IcoOff;
		else
		if (sType == _T("glow"))
			return (void*)m_bGlow;
		else
			return EXP_BASE::GetState(sType);
	}
	BOOL SetState(const CString& sType, void* pState)
	{
		if (sType == _T("status"))
		{
			status_t old_sta = m_Status;
			m_Status = (status_t)(LONG_PTR)pState;
			if (old_sta != m_Status)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("locate"))
		{
			locate_t old_sta = m_Locate;
			m_Locate = (locate_t)(LONG_PTR)pState;
			if (old_sta != m_Locate)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("loc_off"))
		{
			LONG old_sta = m_LocOff;
			m_LocOff = (LONG)(LONG_PTR)pState;
			if (old_sta != m_LocOff)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("shake_ico"))
		{
			int old_sta = m_ShakeIco;
			m_ShakeIco = (int)(LONG_PTR)pState;
			if (old_sta != m_ShakeIco)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("thr_sta"))
		{
			int old_sta = m_ThreeSta;
			m_ThreeSta = (int)(LONG_PTR)pState;
			if (old_sta != m_ThreeSta)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("color"))
		{
			for(int i = 0; i < _countof(m_Color); ++i)
				m_Color[i] = *((pixel_t*)pState + i);
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("image"))
		{
			for(int i = 0; i < _countof(m_Image); ++i)
				m_Image[i] = *((CImage*)pState + i);
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("text"))
		{
			for(int i = 0; i < _countof(m_Text); ++i)
				m_Text[i] = *((CText*)pState + i);
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("icon"))
		{
			m_Icon = *((CImage*)pState);
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("ico_off"))
		{
			LONG old_sta = m_IcoOff;
			m_IcoOff = (LONG)(LONG_PTR)pState;
			if (old_sta != m_IcoOff)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("glow"))
		{
			BOOL old_sta = m_bGlow;
			m_bGlow = (BOOL)(LONG_PTR)pState;
			if (old_sta != m_bGlow)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		return FALSE;
	}
};

//////////////////////////////////////////////////////////////////

class CGuiPushBtn : public IGuiCtrlBase
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiPushBtn, IGuiCtrlBase)

public:
	/*
		����; ����; ����; Push
	*/
	enum status_t {nor, ovr, hit, psh};
	/*
		����λ��: ��; ��; ��; ��; ��
	*/
	enum locate_t {center, top, bottom, left, right};

protected:
	status_t m_Status;	// ��ť״̬
	locate_t m_Locate;	// ����λ��
	LONG m_LocOff;		// ����λ��ƫ��(m_Locate == center ʱ��Ч)

	pixel_t m_Color[4];
	CImage m_Image;		// ����4��״̬
	CText m_Text[4];

public:
	CGuiPushBtn()
		: m_Status(nor)
		, m_Locate(center)
		, m_LocOff(5)
	{
		ZeroMemory(m_Color, sizeof(m_Color));
		// ����¼�����
		AddEvent((IGuiEvent*)ExGui(_T("CGuiPushBtnEvent"), GetGC()));
	}
	~CGuiPushBtn()
	{}

public:
	// ��ÿؼ�״̬
	void* GetState(const CString& sType)
	{
		if (sType == _T("status"))
			return (void*)m_Status;
		else
		if (sType == _T("locate"))
			return (void*)m_Locate;
		else
		if (sType == _T("loc_off"))
			return (void*)m_LocOff;
		else
		if (sType == _T("color"))
			return (void*)m_Color;
		else
		if (sType == _T("image"))
			return (void*)(&m_Image);
		else
		if (sType == _T("text"))
			return (void*)m_Text;
		else
			return EXP_BASE::GetState(sType);
	}
	BOOL SetState(const CString& sType, void* pState)
	{
		if (sType == _T("status"))
		{
			status_t old_sta = m_Status;
			m_Status = (status_t)(LONG_PTR)pState;
			if (old_sta != m_Status)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("locate"))
		{
			locate_t old_sta = m_Locate;
			m_Locate = (locate_t)(LONG_PTR)pState;
			if (old_sta != m_Locate)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("loc_off"))
		{
			LONG old_sta = m_LocOff;
			m_LocOff = (LONG)(LONG_PTR)pState;
			if (old_sta != m_LocOff)
				return EXP_BASE::SetState(sType, pState);
			else
				return TRUE;
		}
		else
		if (sType == _T("color"))
		{
			for(int i = 0; i < _countof(m_Color); ++i)
				m_Color[i] = *((pixel_t*)pState + i);
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("image"))
		{
			m_Image = *((CImage*)pState);
			return EXP_BASE::SetState(sType, pState);
		}
		else
		if (sType == _T("text"))
		{
			for(int i = 0; i < _countof(m_Text); ++i)
				m_Text[i] = *((CText*)pState + i);
			return EXP_BASE::SetState(sType, pState);
		}
		return FALSE;
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiButton, IGuiCtrlBase)
EXP_IMPLEMENT_DYNCREATE_MULT(CGuiPushBtn, IGuiCtrlBase)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiButton_hpp__*/