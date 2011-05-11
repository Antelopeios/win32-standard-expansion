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
// Date:	2010-05-11
// Version:	1.0.0002.1525
//
// History:
//	- 1.0.0001.1730(2010-05-05)	= GuiInterface�������������Ĺ����ӿ�
//	- 1.0.0002.1525(2010-05-11)	+ IGuiComp����й���Ϊ����Ӧ�ӿ�
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
interface EXP_API IGuiComp : public IGuiObject, public CListT<IGuiComp*>
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiComp, IGuiObject)

public:
	typedef CListT<IGuiComp*> base_list_t;

protected:
	bool		m_bTruCldr;	// ���������йܱ��
	IGuiComp*	m_Pare;		// ������ָ��

public:
	IGuiComp(void)
		: m_bTruCldr(false)
		, m_Pare(NULL)
	{}
	virtual ~IGuiComp(void)
	{ if (IsTrustChildren()) Clr(); }

public:
	// �Ƿ�����������й�
	void SetTrustChildren(bool bTruCldr = true) { m_bTruCldr = bTruCldr; }
	bool IsTrustChildren() { return m_bTruCldr; }
	// ��ø�����
	IGuiComp* GetParent() { return m_Pare; }

	// ��Ͻӿ�
	virtual void Add(IGuiComp* pComp)
	{
		if (!pComp) return ;
		// ��λ����
		iterator_t ite = finder_t::Find(*this, pComp);
		if (ite != Tail()) return;
		// ����¶���
		base_list_t::Add(pComp);
		if( pComp->m_Pare )
			pComp->m_Pare->Del(pComp);
		pComp->m_Pare = this;
	}
	virtual void Del(IGuiComp* pComp)
	{
		if (!pComp) return ;
		// ��λ����
		iterator_t ite = finder_t::Find(*this, pComp);
		if (ite == Tail()) return;
		// ɾ������
		base_list_t::Del(ite);
		pComp->m_Pare = NULL;
	}
	virtual void Clr()
	{
		for(iterator_t ite = Head(); ite != Tail(); ++ite)
			ite->Val()->m_Pare = NULL;
		Clear();
	}
};

// GUI �¼��ӿ�
interface EXP_API IGuiEvent : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiEvent, IGuiObject)

protected:
	IGuiComp* m_pComp;

public:
	IGuiEvent()
		: m_pComp(NULL)
	{}
	virtual ~IGuiEvent()
	{}

public:
	void SetComp(IGuiComp* pComp) { m_pComp = pComp; }
	IGuiComp* GetComp() { return m_pComp; }

	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiInterface_h__*/