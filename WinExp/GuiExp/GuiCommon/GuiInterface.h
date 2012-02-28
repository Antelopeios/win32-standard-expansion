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
// GuiInterface - ���湫�ýӿ�
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-02-02
// Version:	1.0.0019.1802
//
// History:
//	- 1.0.0001.1730(2011-05-05)	= GuiInterface�������������Ĺ����ӿ�
//	- 1.0.0002.1525(2011-05-11)	+ IGuiComp����й���Ϊ����Ӧ�ӿ�
//	- 1.0.0003.1428(2011-05-13)	= ����IGuiComp���й�ʱ����Ϊ
//								= ����IGuiEvent����Ľӿڼ���Ϊ
//								+ GUI �¼�ת����(IGuiSender)
//	- 1.0.0004.1527(2011-05-16)	+ ���IGuiComp::Init()��IGuiComp::Fina()�ӿ�
//	- 1.0.0005.1652(2011-05-18)	# ����IGuiComp::Init()����߼�����
//	- 1.0.0006.1620(2011-05-19)	+ ���IGuiBase�����������ඨ��
//								+ ���IGuiObject::Free()�ӿ�,����ExGui()���������ֶ��ͷ�ָ����Դ
//	- 1.0.0007.1000(2011-05-23)	+ ���IGuiEffectЧ�������������
//								= ����IGuiObject::Free()Ϊ�麯��
//	- 1.0.0008.1600(2011-05-25)	+ ���IGuiEffect::IsFinished();SetTimer();KillTimer()�ӿ�
//	- 1.0.0009.1411(2011-05-26)	+ ���IGuiBase::GetPtCtrl()��IGuiBase::GetRealRect()�ӿ�
//	- 1.0.0010.1447(2011-06-08)	^ ��IGuiBase�Ƴ�������ʵ��
//	- 1.0.0011.1530(2011-06-29)	# ����IGuiSender::Send()�ڲ�������Ϣ���ʱ���ܳ��ֵĴ���
//								^ IGuiSender����ӵ��¼�����ִ��,����ִ��������¼�����,�����ӿؼ�������Ϣ
//								+ �����Ͻӿ����Ins...()����,��������϶��е�ͷ������Ӷ���
//	- 1.0.0012.1523(2011-07-07)	# ����Comp��Event�ӿ���Find()������,��ֹ�ⲿ���ó�ͻ
//	- 1.0.0013.1537(2011-08-26)	# ����Comp��Event�ӿ���Trust��ؽӿڵ�����,��ֹ�ⲿ���ó�ͻ
//	- 1.0.0014.1646(2011-08-29)	+ ���IGuiEvent::Param()�ӿ�,�����ⲿ�Զ������
//	- 1.0.0015.1454(2011-09-02)	+ ���IGuiEvent���¼�״̬�ӿ�,����ǰ�¼��������,�ɸ��ݱ�����¼�״̬�ж��Ƿ����ִ����һ���¼�����
//	- 1.0.0016.1517(2011-09-28)	+ ΪIGuiComp::DelComp()�ӿ�����Ƿ��Զ��ͷ��Ƴ��Ĺ�������Ĳ���
//	- 1.0.0017.1720(2011-10-24)	# ������Event��MouseMoveʱ��Ctrlָ�������,���µ�һ��Comp�����Ƴ������Ctrl�Ӷ���ʱMouseMove�����Run-Time�쳣
//	- 1.0.0018.1712(2012-01-20)	+ IGuiEvent֧�ֶԵ�����ĳһ��ʱ����������й�
//								= IGuiSender��IGuiCompĬ���й������Ӷ���
//	- 1.0.0019.1802(2012-02-02)	+ ���IGuiObject::IsValid()
//								# ����ĳ�¼�����������������¼�֮��,�䷵��ֵ��Ȼ�п��ܱ�����δ��Ӧ�¼���Ĭ�Ϸ���ֵ���ǵ�����
//////////////////////////////////////////////////////////////////

#ifndef __GuiInterface_h__
#define __GuiInterface_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI ����ӿ�
EXP_INTERFACE IGuiObject : public IBaseObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiObject, IBaseObject)

protected:
	BOOL m_bFree;

public:
	IGuiObject() : m_bFree(FALSE) {}
	virtual ~IGuiObject() { m_bFree = TRUE; }

public:
	BOOL IsValid() { return (!m_bFree); }
	virtual void Free() { del(this); }
};

//////////////////////////////////////////////////////////////////

EXP_INTERFACE IGuiComp;
EXP_INTERFACE IGuiCtl;

// GUI �¼��ӿ�
EXP_INTERFACE IGuiEvent : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiEvent, IGuiObject)

	friend EXP_INTERFACE IGuiComp;

public:
	enum state_t
	{
		continue_next,	// �����¸��¼�
		break_next,		// �����¸��¼�
		return_next, 	// �¸��¼�ʱ�����¼�����
	};

protected:
	BOOL m_bTru;
	LRESULT m_Result;
	state_t m_State;

protected:
	// ����뿪����ȫ�־�̬�ؼ�ָ��
	static IGuiCtl* s_MLMove;

public:
	IGuiEvent()
		: m_bTru(TRUE)
		, m_Result(0)
		, m_State(continue_next)
	{}
	virtual ~IGuiEvent()
	{}

public:
	virtual void SetTrust(BOOL bTru = TRUE) { m_bTru = bTru; }
	virtual BOOL IsTrust() { return m_bTru; }
	// ����Ĵ洢ָ��
	virtual void* Param() { return NULL; }
	// �¼�����ӿ�
	void SetResult(LRESULT lrRes = 0) { m_Result = lrRes; }
	LRESULT GetResult() const { return m_Result; }
	// �¼�״̬�ӿ�
	void SetState(state_t eSta) { m_State = eSta; }
	state_t GetState()
	{
		state_t sta = m_State;
		m_State = continue_next;
		return sta;
	}
	// �¼����ݽӿ�
	virtual void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0) = 0;
};

//////////////////////////////////////////////////////////////////

// GUI �¼�ת����
EXP_INTERFACE IGuiSender : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiSender, IGuiObject)

public:
	typedef CListT<IGuiEvent*> evt_list_t;

protected:
	BOOL m_bTru;		// ���������йܱ��
private:
	evt_list_t* m_CldrEvt;

public:
	IGuiSender(void)
		: m_bTru(TRUE)
		, m_CldrEvt(dbnew(evt_list_t))
	{}
	virtual ~IGuiSender(void)
	{
		ClearEvent();
		del(m_CldrEvt);
		m_CldrEvt = NULL;
	}

public:
	// �Ƿ�����������й�
	void SetTrustEvent(BOOL bTruCldr = TRUE) { m_bTru = bTruCldr; }
	BOOL IsTrustEvent() { return m_bTru; }
	// ����ڲ�����
	evt_list_t& GetEvent() { return *m_CldrEvt; }

	// ����
	evt_list_t::iterator_t FindEvent(IGuiEvent* pEvent) { return GetEvent().Find(pEvent); }

	// ��Ͻӿ�
	virtual void AddEvent(void* p)
	{
		IGuiEvent* evt = ExDynCast<IGuiEvent>(p);
		if (!evt) return ;
		// ��λ����
		evt_list_t::iterator_t ite = FindEvent(evt);
		if (ite != GetEvent().Tail()) return;
		// ����¶���
		GetEvent().Add(evt);
	}
	virtual void InsEvent(void* p)
	{
		IGuiEvent* evt = ExDynCast<IGuiEvent>(p);
		if (!evt) return ;
		// ��λ����
		evt_list_t::iterator_t ite = FindEvent(evt);
		if (ite != GetEvent().Tail()) return;
		// ����¶���
		GetEvent().Add(evt, GetEvent().Head());
	}
	virtual void DelEvent(void* p = NULL)
	{
		IGuiEvent* evt = ExDynCast<IGuiEvent>(p);
		evt_list_t::iterator_t ite;
		if (evt)
		{
			// ��λ����
			ite = FindEvent(evt);
			if (ite == GetEvent().Tail()) return;
		}
		else
		{
			ite = GetEvent().Last();
			evt = GetEvent().LastItem();
		}
		// ɾ������
		GetEvent().Del(ite);
		if (m_bTru && evt->IsTrust()) evt->Free();
	}
	virtual void ClearEvent()
	{
		if (m_bTru)
			for(evt_list_t::iterator_t ite = GetEvent().Head(); ite != GetEvent().Tail(); ++ite)
			{
				if (!(*ite) || 
					!(*ite)->IsValid() || 
					!(*ite)->IsTrust()) continue;
				(*ite)->Free();
			}
		GetEvent().Clear();
	}

	// �¼�����ӿ�
	void SetResult(LRESULT lrRet = 0)
	{
		if(!IsValid()) return;
		for(evt_list_t::iterator_t ite = GetEvent().Head(); ite != GetEvent().Tail(); ++ite)
		{
			if (!(*ite)) continue;
			(*ite)->SetResult(lrRet);
		}
	}
	LRESULT GetResult(LRESULT lrDef = 0)
	{
		if(!IsValid()) return NULL;
		// ����ӵ��¼�����ִ��
		evt_list_t::iterator_t ite = GetEvent().Last();
		for(; ite != GetEvent().Head(); --ite)
		{
			if (!(*ite)) continue;
			LRESULT r = (*ite)->GetResult();
			if (r != 0 && lrDef != r) lrDef = r;
		}
		if (*ite)
		{
			LRESULT r = (*ite)->GetResult();
			if (r != 0 && lrDef != r) lrDef = r;
		}
		return lrDef;
	}
	// �¼����ͽӿ�
	virtual void Send(void* p, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiObject* gui = ExDynCast<IGuiObject>(p);
		if (!gui) return;
		if (GetEvent().Empty()) return;
		LRESULT ret = GetResult();
		IGuiEvent::state_t sta = IGuiEvent::continue_next;
		// ����ӵ��¼�����ִ��
		evt_list_t::iterator_t ite = GetEvent().Last();
		for(; ite != GetEvent().Head(); --ite)
		{
			if (sta == IGuiEvent::break_next)
				continue;
			else
			if (sta == IGuiEvent::return_next)
			{
				SetResult(ret);
				return;
			}
			if (!(*ite)) continue;
			(*ite)->SetResult(ret); // ������Ϣʱ,���¼������յ���һ���¼��Ĵ�����
			(*ite)->OnMessage(gui, nMessage, wParam, lParam);
			if (!(*ite)->IsValid()) return;
			sta = (*ite)->GetState();
			LRESULT r = (*ite)->GetResult();
			if (r != 0 && ret != r) ret = r;
		}
		if (sta != IGuiEvent::continue_next)
		{
			SetResult(ret);
			return;
		}
		if (*ite)
		{
			(*ite)->SetResult(ret); // ������Ϣʱ,���¼������յ���һ���¼��Ĵ�����
			(*ite)->OnMessage(gui, nMessage, wParam, lParam);
			LRESULT r = (*ite)->GetResult();
			if (r != 0 && ret != r) ret = r;
		}
		SetResult(ret);
	}
};

//////////////////////////////////////////////////////////////////

// GUI ��Ͻӿ�
EXP_INTERFACE IGuiComp : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiComp, IGuiObject)

public:
	typedef CListT<IGuiComp*> list_t;

protected:
	BOOL		m_bTru;		// ���������йܱ��
	IGuiComp*	m_Pare;		// ������ָ��
private:
	list_t* m_Cldr;

public:
	IGuiComp(void)
		: m_bTru(TRUE)
		, m_Pare(NULL)
		, m_Cldr(dbnew(list_t))
	{}
	virtual ~IGuiComp(void)
	{
		ClearComp();
		del(m_Cldr);
	}

protected:
	virtual void Init(IGuiComp* pComp) { m_Pare = pComp; }
	virtual void Fina() { m_Pare = NULL; }

public:
	// �Ƿ�����������й�
	void SetTrust(BOOL bTruCldr = TRUE) { m_bTru = bTruCldr; }
	BOOL IsTrust() { return m_bTru; }
	// ����ڲ�����
	IGuiComp* GetParent() { return m_Pare; }
	list_t& GetChildren() { return *m_Cldr; }

	// ����
	list_t::iterator_t FindComp(void* p) { return GetChildren().Find(ExDynCast<IGuiComp>(p)); }

	// ��Ͻӿ�
	virtual void AddComp(void* p)
	{
		IGuiComp* cmp = ExDynCast<IGuiComp>(p);
		if (!cmp) return ;
		// ��λ����
		list_t::iterator_t ite = FindComp(cmp);
		if (ite != GetChildren().Tail()) return;
		// ����¶���
		if (cmp->m_Pare)
			cmp->m_Pare->DelComp(cmp);
		cmp->Init(this);
		GetChildren().Add(cmp);
	}
	virtual void InsComp(void* p)
	{
		IGuiComp* cmp = ExDynCast<IGuiComp>(p);
		if (!cmp) return ;
		// ��λ����
		list_t::iterator_t ite = FindComp(cmp);
		if (ite != GetChildren().Tail()) return;
		// ����¶���
		if (cmp->m_Pare)
			cmp->m_Pare->DelComp(cmp);
		cmp->Init(this);
		GetChildren().Add(cmp, GetChildren().Head());
	}
	virtual void DelComp(void* p, BOOL bAutoTru = TRUE)
	{
		IGuiComp* cmp = ExDynCast<IGuiComp>(p);
		if (!cmp) return ;
		// ��λ����
		list_t::iterator_t ite = FindComp(cmp);
		if (ite == GetChildren().Tail()) return;
		// ɾ������
		GetChildren().Del(ite);
		cmp->Fina();
		if (bAutoTru && m_bTru)
		{
			if (IGuiEvent::s_MLMove == (IGuiCtl*)cmp)
				IGuiEvent::s_MLMove = NULL;
			cmp->Free();
		}
	}
	virtual void ClearComp()
	{
		for(list_t::iterator_t ite = GetChildren().Head(); ite != GetChildren().Tail(); ++ite)
		{
			if (!(*ite)) continue;
			(*ite)->Fina();
			if (m_bTru) (*ite)->Free();
		}
		GetChildren().Clear();
	}
};

//////////////////////////////////////////////////////////////////

// GUI Ч���������
EXP_INTERFACE IGuiEffect : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiEffect, IGuiObject)

public:
	// ��ʼ��
	virtual void Init(CImage& tImg) = 0;
	virtual BOOL IsInit() = 0;
	// Ч����ʾ�ӿ�
	virtual void Show(IGuiObject* pGui, CImage& tImg) = 0;
	// ��ʱ��
	virtual void SetTimer(wnd_t hWnd) = 0;
	virtual void KillTimer(wnd_t hWnd) = 0;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiInterface_h__*/