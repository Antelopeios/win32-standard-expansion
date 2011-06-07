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
// GuiInterface - ���湫�ýӿ�
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-06-08
// Version:	1.0.0010.0047
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
//	- 1.0.0010.0047(2011-06-08)	= IGuiSender���������ӵ��¼�����ת����Ϣ
//								^ ��IGuiBase�Ƴ�������ʵ��
//////////////////////////////////////////////////////////////////

#ifndef __GuiInterface_h__
#define __GuiInterface_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI ����ӿ�
interface EXP_API IGuiObject : public IBaseObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiObject, IBaseObject)

public:
	IGuiObject() {}
	virtual ~IGuiObject() {}

public:
	virtual void Free() { ExMem::Free(this); }
};

//////////////////////////////////////////////////////////////////

// GUI ��Ͻӿ�
interface EXP_API IGuiComp : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiComp, IGuiObject)

public:
	typedef CListT<IGuiComp*> list_t;

protected:
	bool		m_bTru;		// ���������йܱ��
	IGuiComp*	m_Pare;		// ������ָ��
private:
	list_t* m_Cldr;

public:
	IGuiComp(void)
		: m_bTru(false)
		, m_Pare(NULL)
		, m_Cldr(ExMem::Alloc<list_t>())
	{}
	virtual ~IGuiComp(void)
	{
		ClearComp();
		ExMem::Free(m_Cldr);
	}

protected:
	virtual void Init(IGuiComp* pComp) { m_Pare = pComp; }
	virtual void Fina() { m_Pare = NULL; }

public:
	// �Ƿ�����������й�
	void SetTrust(bool bTruCldr = true) { m_bTru = bTruCldr; }
	bool IsTrust() { return m_bTru; }
	// ����ڲ�����
	IGuiComp* GetParent() { return m_Pare; }
	list_t& GetChildren() { return *m_Cldr; }

	// ����
	list_t::iterator_t& Find(IGuiComp* pComp) { return list_t::finder_t::Find(GetChildren(), pComp); }

	// ��Ͻӿ�
	virtual void AddComp(IGuiComp* pComp)
	{
		if (!pComp) return ;
		// ��λ����
		list_t::iterator_t ite = Find(pComp);
		if (ite != GetChildren().Tail()) return;
		// ����¶���
		if( pComp->m_Pare )
			pComp->m_Pare->DelComp(pComp);
		pComp->Init(this);
		GetChildren().Add(pComp);
	}
	virtual void DelComp(IGuiComp* pComp)
	{
		if (!pComp) return ;
		// ��λ����
		list_t::iterator_t ite = Find(pComp);
		if (ite == GetChildren().Tail()) return;
		// ɾ������
		GetChildren().Del(ite);
		pComp->Fina();
		if (m_bTru) pComp->Free();
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

// GUI �¼��ӿ�
interface EXP_API IGuiEvent : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiEvent, IGuiObject)

protected:
	LRESULT m_Result;

public:
	IGuiEvent()
		: m_Result(0)
	{}
	virtual ~IGuiEvent()
	{}

public:
	// �¼�����ӿ�
	void SetResult(LRESULT lrRes = 0) { m_Result = lrRes; }
	LRESULT GetResult() { return m_Result; }
	// �¼����ݽӿ�
	virtual void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0) = 0;
};

//////////////////////////////////////////////////////////////////

// GUI �¼�ת����
interface EXP_API IGuiSender : public IGuiObject, protected CListT<IGuiEvent*>
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiSender, IGuiObject)

public:
	typedef CListT<IGuiEvent*> evt_list_t;

protected:
	bool m_bTru;		// ���������йܱ��
private:
	evt_list_t* m_CldrEvt;

public:
	IGuiSender(void)
		: m_bTru(false)
		, m_CldrEvt(ExMem::Alloc<evt_list_t>())
	{}
	virtual ~IGuiSender(void)
	{
		ClearEvent();
		ExMem::Free(m_CldrEvt);
	}

public:
	// �Ƿ�����������й�
	void SetTrust(bool bTruCldr = true) { m_bTru = bTruCldr; }
	bool IsTrust() { return m_bTru; }
	// ����ڲ�����
	evt_list_t& GetEvent() { return *m_CldrEvt; }

	// ����
	evt_list_t::iterator_t& Find(IGuiEvent* pEvent) { return evt_list_t::finder_t::Find(GetEvent(), pEvent); }

	// ��Ͻӿ�
	virtual void AddEvent(IGuiEvent* pEvent)
	{
		if (!pEvent) return ;
		// ��λ����
		evt_list_t::iterator_t ite = Find(pEvent);
		if (ite != GetEvent().Tail()) return;
		// ����¶���
		GetEvent().Add(pEvent);
	}
	virtual void DelEvent(IGuiEvent* pEvent)
	{
		if (!pEvent) return ;
		// ��λ����
		evt_list_t::iterator_t ite = Find(pEvent);
		if (ite == GetEvent().Tail()) return;
		// ɾ������
		GetEvent().Del(ite);
		if (m_bTru) pEvent->Free();
	}
	virtual void ClearEvent()
	{
		if (m_bTru)
			for(evt_list_t::iterator_t ite = GetEvent().Head(); ite != GetEvent().Tail(); ++ite)
			{
				if (!(*ite)) continue;
				(*ite)->Free();
			}
		GetEvent().Clear();
	}

	// �¼�����ӿ�
	void SetResult(LRESULT lrRet = 0)
	{
		for(evt_list_t::iterator_t ite = GetEvent().Head(); ite != GetEvent().Tail(); ++ite)
		{
			if (!(*ite)) continue;
			(*ite)->SetResult(lrRet);
		}
	}
	LRESULT GetResult(LRESULT lrDef = 0)
	{
		for(evt_list_t::iterator_t ite = GetEvent().Head(); ite != GetEvent().Tail(); ++ite)
		{
			if (!(*ite)) continue;
			LRESULT r = (*ite)->GetResult();
			if (r != 0 && lrDef != r) lrDef = r;
		}
		return lrDef;
	}
	// �¼����ͽӿ�
	virtual void Send(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		if (GetEvent().Empty()) return;
		LRESULT ret = GetResult();
		// ����ӵ��¼�����ִ��
		evt_list_t::iterator_t ite = GetEvent().Last();
		for(; ite != GetEvent().Head(); --ite)
		{
			if (!(*ite)) continue;
			(*ite)->SetResult(ret); // ������Ϣʱ,���¼������յ���һ���¼��Ĵ�����
			(*ite)->OnMessage(pGui, nMessage, wParam, lParam);
			ret = (*ite)->GetResult();
		}
		ite = GetEvent().Head();
		if (*ite)
		{
			(*ite)->SetResult(ret); // ������Ϣʱ,���¼������յ���һ���¼��Ĵ�����
			(*ite)->OnMessage(pGui, nMessage, wParam, lParam);
		}
	}
};

//////////////////////////////////////////////////////////////////

// GUI Ч���������
interface EXP_API IGuiEffect : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiEffect, IGuiObject)

public:
	// ��ʼ��
	virtual void Init(CImage& tImg) = 0;
	virtual bool IsInit() = 0;
	// ���
	virtual bool IsFinished() = 0;
	// Ч����ʾ�ӿ�
	virtual void Show(IGuiObject* pGui, CImage& tImg) = 0;
	// ��ʱ��
	virtual void SetTimer(wnd_t hWnd) = 0;
	virtual void KillTimer(wnd_t hWnd) = 0;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiInterface_h__*/