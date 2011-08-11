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
// GuiListView - �б���ͼ�ؼ�
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-11
// Version:	1.0.0004.1730
//
// History:
//	- 1.0.0000.1543(2011-06-30)	@ ��ʼ����GuiListView
//	- 1.0.0001.1532(2011-07-21)	= ��CGuiListView�ڲ�items_t�ṹ��ָ���Ϊ����,������ø��Ӷ�
//	- 1.0.0002.1744(2011-08-05)	+ ����GuiListView����ʱĬ���б���ı���ͼ
//	- 1.0.0003.1508(2011-08-10)	= ��Ĭ���б���ı���ͼ��ͼƬ��ΪPic�ؼ�
//	- 1.0.0004.1730(2011-08-11)	= �б����ͼ��ƫ�Ʋ��õ���������(ico_off)����
//////////////////////////////////////////////////////////////////

#ifndef __GuiListView_hpp__
#define __GuiListView_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiLVItem : public CGuiButton /*CGuiListView�ڲ�ʹ�õ��б���*/
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiLVItem, CGuiButton)

protected:
	CImage m_Icon;
	bool m_bGlow;
	LONG m_IcoOff;		// ͼ��λ��ƫ��(m_Locate == center ʱ��Ч)

public:
	CGuiLVItem()
		: m_bGlow(false)
		, m_IcoOff(5)
	{
		// �����¼�����
		InsEvent((IGuiEvent*)ExGui(_T("CGuiLVItemEvent"), GetGC())); /*���û����ͼ*/
	}

public:
	// ��ÿؼ�״̬
	state_t* GetState(const CString& sType, CGC* pGC = NULL)
	{
		state_t* state = IGuiCtrlBase::GetState(sType, pGC);
		if (state)
		{
			if (state->sta_typ == _T("icon"))
				state->sta_arr.Add(&m_Icon);
			else
			if (state->sta_typ == _T("ico_off"))
				state->sta_arr.Add((void*)m_IcoOff);
			else
			if (state->sta_typ == _T("glow"))
				state->sta_arr.Add((void*)m_bGlow);
			else
				state = EXP_BASE::GetState(sType, pGC);
		}
		return state;
	}
	bool SetState(const CString& sType, void* pState)
	{
		if (sType == _T("icon"))
		{
			m_Icon = *((CImage*)pState);
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("ico_off"))
		{
			LONG old_sta = m_IcoOff;
			m_IcoOff = (LONG)(LONG_PTR)pState;
			if (old_sta != m_IcoOff)
				return EXP_BASE::SetState(sType, pState);
			else
				return true;
		}
		else
		if (sType == _T("glow"))
		{
			m_bGlow = (bool)(LONG_PTR)pState;
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
			return EXP_BASE::SetState(sType, pState);
	}
};

//////////////////////////////////////////////////////////////////

class CGuiListView : public CGuiPicture
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiListView, CGuiPicture)

public:
	typedef CListT<IGuiCtrl*> items_t;

protected:
	items_t m_ItemList;
	LONG m_Space;	// ����
	LONG m_AllLine, m_FraLine;
	CGuiPicture m_FocPic;

public:
	CGuiListView()
		: m_ItemList(NULL)
		, m_Space(0)
		, m_AllLine(0)
		, m_FraLine(0)
	{
		// �����¼�����
		InsEvent((IGuiEvent*)ExGui(_T("CGuiLVEvent"), GetGC())); /*���û����ͼ*/
		SetState(_T("color"), (void*)ExRGBA(EXP_CM, EXP_CM, EXP_CM, EXP_CM));
		AddComp(&m_FocPic);
	}

public:
	// ��ÿؼ�״̬
	state_t* GetState(const CString& sType, CGC* pGC = NULL)
	{
		CString type(sType);
		if (type.Left(4) == _T("foc_"))
		{
			type.TrimLeft(_T("foc_"));
			return m_FocPic.GetState(type, pGC);
		}
		state_t* state = IGuiCtrlBase::GetState(sType, pGC);
		if (state)
		{
			if (state->sta_typ == _T("foc"))
				state->sta_arr.Add(&m_FocPic);
			else
			if (state->sta_typ == _T("items"))
				state->sta_arr.Add(&m_ItemList);
			else
			if (state->sta_typ == _T("space"))
				state->sta_arr.Add((void*)m_Space);
			else
			if (state->sta_typ == _T("all_line"))
				state->sta_arr.Add((void*)m_AllLine);
			else
			if (state->sta_typ == _T("fra_line"))
				state->sta_arr.Add((void*)m_FraLine);
			else
				state = EXP_BASE::GetState(sType, pGC);
		}
		return state;
	}
	bool SetState(const CString& sType, void* pState)
	{
		CString type(sType);
		if (type.Left(4) == _T("foc_"))
		{
			type.TrimLeft(_T("foc_"));
			return m_FocPic.SetState(type, pState);
		}
		else
		if (sType == _T("items"))
		{
			items_t* new_sta = (items_t*)pState;
			if (new_sta == NULL) return false;
			for(items_t::iterator_t ite = m_ItemList.Head(); ite != m_ItemList.Tail(); ++ite)
			{
				IGuiCtrl* item = *ite;
				if (!item) continue;
				items_t::iterator_t it = items_t::finder_t::Find(*new_sta, item);
				if (it == new_sta->Tail()) DelComp(item);
			}
			m_ItemList = *(items_t*)pState;
			for(items_t::iterator_t ite = m_ItemList.Head(); ite != m_ItemList.Tail(); ++ite)
			{
				IGuiCtrl* item = *ite;
				if (!item) continue;
				AddComp(item);
			}
			return IGuiCtrlBase::SetState(sType, pState);
		}
		else
		if (sType == _T("space"))
		{
			LONG old_sta = m_Space;
			m_Space = (LONG)(LONG_PTR)pState;
			if (old_sta != m_Space)
				return IGuiCtrlBase::SetState(sType, pState);
			else
				return true;
		}
		else
		if (sType == _T("all_line"))
		{
			LONG old_sta = m_AllLine;
			m_AllLine = (LONG)(LONG_PTR)pState;
			if (old_sta != m_AllLine && GetScroll())
				GetScroll()->SetState(_T("sli_all"), (void*)m_AllLine);
			return true;
		}
		else
		if (sType == _T("fra_line"))
		{
			LONG old_sta = m_FraLine;
			m_FraLine = (LONG)(LONG_PTR)pState;
			if (old_sta != m_FraLine && GetScroll())
				GetScroll()->SetState(_T("sli_fra"), (void*)m_FraLine);
			return true;
		}
		else
			return EXP_BASE::SetState(sType, pState);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiLVItem, CGuiButton)
EXP_IMPLEMENT_DYNCREATE_MULT(CGuiListView, CGuiPicture)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiListView_hpp__*/