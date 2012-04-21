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
// GuiCtrl - �ؼ�����
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-04-21
// Version:	1.0.0027.2235
//
// History:
//	- 1.0.0001.2236(2011-05-23)	+ IGuiCtrl���Ч��������ؽӿ�
//								+ ���IGuiCtrl::IsUpdated()�ӿ�
//	- 1.0.0002.1517(2011-05-25)	# ���ؼ���ЧʱIGuiCtrl::SetFocus()Ӧ��ֱ�ӷ���
//	- 1.0.0003.1746(2011-05-26)	# IGuiCtrl::SetFocus()Ӧ��ͬʱ�ݹ����ø��ؼ�����
//								# IGuiCtrl::GetFocus()Ӧ��ֱ�ӷ�����ײ�Ľ���ؼ�
//	- 1.0.0004.1709(2011-06-03)	= ����IGuiCtrl������ƽӿڵ�����
//								+ ���IGuiCtrl::GetClientRect()�ӿ�
//	- 1.0.0005.2304(2011-06-08)	+ ���IGuiCtrl::UpdateState()�ӿ�
//								+ ��IGuiCtrl::SetFocus()�ӿ�ʵ������Ӹ���״̬�߼�
//	- 1.0.0006.1010(2011-06-17)	= ��IGuiCtrlBase�ӿ��ƶ���GuiCtrl.h��,ʹ�ⲿ����ʹ�ô˽ӿ�
//	- 1.0.0007.1732(2011-06-24)	+ ��Ӿ�̬IGuiCtrl::SetFocus()�ӿ�
//	- 1.0.0008.2000(2011-07-16)	+ ���IGuiCtrl::GetClipRect()�ӿ�,���ڻ�ͼʱ��̬��ȡ��ǰ�������µĻ�ͼ����
//	- 1.0.0009.2111(2011-07-31)	= IGuiCtrl::Init()���жϿؼ��Ƿ�ɼ�,������WM_SHOWWINDOW��Ϣ
//	- 1.0.0010.1720(2011-08-03)	+ IGuiCtrl���Scrollϵ�нӿ�,֧���¼���Ӧ��ȡ����ƫ�Ʋ������ͼ
//	- 1.0.0011.1714(2011-08-08)	+ IGuiCtrl���Scroll�ؼ��󶨽ӿ�
//	- 1.0.0012.1715(2011-08-09)	# ��IGuiCtrlû�и�����ʱ������Ϣ����
//								^ �Ż�Scroll�ؼ��󶨽ӿڵ���Ϣ����
//	- 1.0.0013.1730(2011-08-11)	= IGuiCtrl��SetFocusʱ�ȷ��ͽ���ʧȥ��Ϣ,�ٷ��ͽ�������Ϣ
//	- 1.0.0014.1816(2011-08-24)	+ ��IGuiCtrl���һ��Scroll����ʱ,�Զ���������main����
//	- 1.0.0015.1643(2011-08-25)	^ �����IGuiCtrl::GetState()�ӿڵĵ��÷�ʽ��Ч��
//	- 1.0.0016.1318(2011-08-31)	^ ��IGuiCtrl::SetScroll()����Ĳ����뵱ǰm_Scroll���,��������m_Scroll��ˢ�²���
//	- 1.0.0017.2154(2011-09-22)	^ ��IGuiCtrl::SetScrollSize()����Ĳ����뵱ǰm_szScroll���,�������к�������Ϣ����
//	- 1.0.0018.1802(2012-02-02)	+ ���IGuiCtrl��Ч��������йܽӿ�
//	- 1.0.0019.1200(2012-02-22)	= ��IGuiCtrl����ΪIGuiCtl
//	- 1.0.0020.1723(2012-02-29)	% �޸Ĳ����ƹ��������ƻ���,��֧��ˮƽ�봹ֱ����ģʽ�Ĺ�����
//	- 1.0.0021.2237(2012-03-03)	+ ���itree_t���ͼ���,Ϊ��Ҫ���ͼ��ϵĿؼ���׼��
//								+ ���set_ins_t,���ڷ�����Ƽ��ϵĲ������,��Ϊ�����Ϳؼ����insert,delete��clear����
//	- 1.0.0022.0034(2012-03-04)	# ������IGuiCtl::SetWindowRect()��ʱ��Ӧ�ÿ��ǹ���ƫ��,����ᵼ�¿ؼ�λ�õ���ʾ�쳣
//	- 1.0.0023.1603(2012-03-12)	+ ���IGuiCtl::IsNeedScroll()�ӿ�,�����ⲿ�жϵ�ǰ�Ƿ���Ҫ��ʾ������
//	- 1.0.0024.2356(2012-03-16)	^ �Ƴ�IGuiCtl::set_ins_t,��IGuiCtl::GetState()��IGuiCtl::SetState()�ӿ�����Ӷ����param,��֧�ָ�Ϊͨ�õ������趨
//								- �Ƴ�IGuiCtl::GetClipRect(),���ڼ������Ŀ���ȫ��������ͼ�߼��㸺��
//	- 1.0.0025.1515(2012-03-24)	+ ��ӿؼ����ö���������ؽӿ�,֧��ͨ���ؼ����ö���Կؼ�����������̬����
//	- 1.0.0026.1449(2012-04-17)	^ �򻯿ؼ�����Ĳ�νṹ,�Ƴ�IGuiCtrlBase�ӿ�,�������й��ܲ���IGuiCtl
//	- 1.0.0027.2235(2012-04-21)	# ���������ڲ��ɼ�ʱ,�ؼ��Ľ���������Ȼ��Ч,���½�����Ӧ���쳣����
//////////////////////////////////////////////////////////////////

#ifndef __GuiCtrl_h__
#define __GuiCtrl_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCommon/GuiCommon.h"
#include "GuiBoard/GuiBoard.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI �ؼ�����ӿ�
EXP_INTERFACE IGuiCtl : public IGuiBase, public IGuiSetMgr
{
	EXP_DECLARE_DYNAMIC_MULT(IGuiCtl, IGuiBase)

public:
	typedef CListT<IGuiCtl*> items_t;
	typedef CTreeT<IGuiCtl*> itree_t;

protected:
	BOOL m_bEnable;		// �Ƿ����
	BOOL m_bVisible;	// �Ƿ�ɼ�

	CRect m_Rect;		// �ؼ�����

	BOOL m_Updated;

	// ��������
	CSize m_AllRect, m_FraRect;

	static IGuiCtl* m_Focus;
	IGuiEffect* m_Effect;
	BOOL m_bTruEff;
	CSize m_szScroll;
	IGuiCtl* (m_Scroll[2]);

public:
	IGuiCtl();
	BOOL IsValid() const;

protected:
	void Init(IGuiComp* pComp);
	void Fina();

public:
	void SetTrust(BOOL bTruCldr = TRUE);
	BOOL IsTrust() const;

	virtual void SendSet(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);
	virtual void SendMsg(void* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);
	void Send(void* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);
	void SendMessage(UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0);

	// �ؼ����ö������
	BOOL AddSet(void* p);
	BOOL AddSet(LPCTSTR key);
	BOOL InsSet(void* p);
	BOOL InsSet(LPCTSTR key);

	// ��ÿؼ�״̬
	BOOL Execute(const CString& key, const CString& val);
	void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent);
	virtual void* GetState(const CString& sType, void* pParam = NULL);
	virtual BOOL SetState(const CString& sType, void* pState, void* pParam = NULL);
	virtual void UpdateState(BOOL bRefreshSelf = TRUE);
	virtual BOOL IsUpdated();

	// ����Ч������
	void SetEffectTrust(BOOL bTru = TRUE);
	void SetEffect(void* p);
	IGuiEffect* GetEffect();

	// ��û�ͼ��
	IGuiBase* GetParent() const;
	IGuiWnd* GetWnd() const;
	wnd_t GethWnd() const;

	// �������
	BOOL GetRect(CRect& rc) const;
	BOOL SetRect(const CRect& rc);

	BOOL P2C(CRect& rc) const;
	BOOL C2P(CRect& rc) const;
	BOOL B2C(CRect& rc) const;
	BOOL C2B(CRect& rc) const;

	BOOL GetWindowRect(CRect& rc) const;
	BOOL SetWindowRect(const CRect& rc);
	BOOL GetClientRect(CRect& rc) const;

	BOOL GetRealRect(CRect& rc) const;
	BOOL SetRealRect(const CRect& rc);

	BOOL SetAllRect(const CSize& sz);
	BOOL GetAllRect(CSize& sz) const;
	BOOL SetFraRect(const CSize& sz);
	BOOL GetFraRect(CSize& sz) const;

	BOOL GetScrollSize(CSize& sz) const;
	BOOL SetScrollSize(const CSize& sz, BOOL bWheel = FALSE);
	IGuiCtl* GetScroll(BOOL bLine = TRUE) const;
	void SetScroll(void* p, BOOL bLine = TRUE);
	BOOL IsNeedScroll(BOOL bLine = TRUE);

	// �ж��Ƿ����(��ؼ��ڸ��ؼ�������)
	BOOL IsDisplayed() const;
	BOOL GetDisplaySize(CSize& sz) const;

	// ˢ�»�ͼ
	void Refresh(BOOL bSelf = TRUE);

	// ���ÿ�����
	BOOL SetEnable(BOOL bEnable = TRUE);
	BOOL IsEnabled() const;

	// ���ÿɼ���
	BOOL SetVisible(BOOL bVisible = TRUE);
	BOOL IsVisible() const;

	// �ж���Ч��
	static BOOL IsEffect(const IGuiCtl* pCtrl);

	// �������
	static IGuiCtl* SetFocus(IGuiCtl* pFoc);
	virtual IGuiCtl* SetFocus();
	static IGuiCtl* GetFocus();
	virtual BOOL IsFocus() const;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiCtrl_h__*/