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
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2010-05-18
// Version:	1.0.0005.1652
//
// History:
//	- 1.0.0001.1730(2010-05-05)	= GuiInterface�������������Ĺ����ӿ�
//	- 1.0.0002.1525(2010-05-11)	+ IGuiComp����й���Ϊ����Ӧ�ӿ�
//	- 1.0.0003.1428(2010-05-13)	= ����IGuiComp���й�ʱ����Ϊ
//								= ����IGuiEvent����Ľӿڼ���Ϊ
//								+ GUI �¼�ת����(IGuiSender)
//	- 1.0.0004.1527(2010-05-16)	+ ���IGuiComp::Init()��IGuiComp::Fina()�ӿ�
//	- 1.0.0005.1652(2010-05-18)	# ����IGuiComp::Init()����߼�����
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
};

//////////////////////////////////////////////////////////////////

// GUI ��Ͻӿ�
interface EXP_API IGuiComp : public IGuiObject, protected CListT<IGuiComp*>
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiComp, IGuiObject)

public:
	typedef CListT<IGuiComp*> list_t;

protected:
	bool		m_bTru;		// ���������йܱ��
	IGuiComp*	m_Pare;		// ������ָ��

public:
	IGuiComp(void)
		: m_bTru(false)
		, m_Pare(NULL)
	{}
	virtual ~IGuiComp(void)
	{ Clear(); }

protected:
	virtual void Init(IGuiComp* pComp) { m_Pare = pComp; }
	virtual void Fina() { m_Pare = NULL; }

public:
	// �Ƿ�����������й�
	void SetTrust(bool bTruCldr = true) { m_bTru = bTruCldr; }
	bool IsTrust() { return m_bTru; }
	// ����ڲ�����
	IGuiComp* GetParent() { return m_Pare; }
	list_t& GetChildren() { return *((list_t*)this); }

	// ����
	iterator_t& Find(IGuiComp* pComp) { return list_t::finder_t::Find(GetChildren(), pComp); }

	// ��Ͻӿ�
	virtual void Add(IGuiComp* pComp)
	{
		if (!pComp) return ;
		// ��λ����
		list_t::iterator_t ite = Find(pComp);
		if (ite != list_t::Tail()) return;
		// ����¶���
		if( pComp->m_Pare )
			pComp->m_Pare->Del(pComp);
		pComp->Init(this);
		list_t::Add(pComp);
	}
	virtual void Del(IGuiComp* pComp)
	{
		if (!pComp) return ;
		// ��λ����
		list_t::iterator_t ite = Find(pComp);
		if (ite == list_t::Tail()) return;
		// ɾ������
		list_t::Del(ite);
		pComp->Fina();
		if (m_bTru) ExMem::Free(pComp);
	}
	virtual void Clear()
	{
		for(list_t::iterator_t ite = list_t::Head(); ite != list_t::Tail(); ++ite)
		{
			if (!(*ite)) continue;
			(*ite)->Fina();
			if (m_bTru) ExMem::Free(*ite);
		}
		list_t::Clear();
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

public:
	IGuiSender(void)
		: m_bTru(false)
	{}
	virtual ~IGuiSender(void)
	{ ClearEvent(); }

public:
	// �Ƿ�����������й�
	void SetTrust(bool bTruCldr = true) { m_bTru = bTruCldr; }
	bool IsTrust() { return m_bTru; }
	// ����ڲ�����
	evt_list_t& GetEvent() { return *((evt_list_t*)this); }

	// ����
	iterator_t& Find(IGuiEvent* pEvent) { return evt_list_t::finder_t::Find(GetEvent(), pEvent); }

	// ��Ͻӿ�
	virtual void AddEvent(IGuiEvent* pEvent)
	{
		if (!pEvent) return ;
		// ��λ����
		evt_list_t::iterator_t ite = Find(pEvent);
		if (ite != evt_list_t::Tail()) return;
		// ����¶���
		evt_list_t::Add(pEvent);
	}
	virtual void DelEvent(IGuiEvent* pEvent)
	{
		if (!pEvent) return ;
		// ��λ����
		evt_list_t::iterator_t ite = Find(pEvent);
		if (ite == evt_list_t::Tail()) return;
		// ɾ������
		evt_list_t::Del(ite);
		if (m_bTru) ExMem::Free(pEvent);
	}
	virtual void ClearEvent()
	{
		if (m_bTru)
			for(evt_list_t::iterator_t ite = evt_list_t::Head(); ite != evt_list_t::Tail(); ++ite)
			{
				if (!(*ite)) continue;
				ExMem::Free(*ite);
			}
		evt_list_t::Clear();
	}

	// �¼����ͽӿ�
	virtual void Send(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		for(evt_list_t::iterator_t ite = evt_list_t::Head(); ite != evt_list_t::Tail(); ++ite)
		{
			if (!(*ite)) continue;
			(*ite)->OnMessage(pGui, nMessage, wParam, lParam);
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiInterface_h__*/