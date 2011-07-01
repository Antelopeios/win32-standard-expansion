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
// IGuiBase - ����������
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-07-01
// Version:	1.0.0004.1020
//
// History:
//	- 1.0.0002.0047(2011-06-08)	@ ����IGuiBase,���ȫ��ͨ����ϢԤ����ͳһGC
//	- 1.0.0003.1544(2011-06-28)	+ ����¼�����ӿ�,֧�����һ��IGuiBase���ò������е��¼�
//	- 1.0.0004.1020(2011-07-01)	+ ��дIGuiBase::InsEvent()�ӿ�,���¼�������������ʱ,��ڶ�����㴦�����½��(Ϊ�˱�֤��һ������Ľ��ʼ�����ִ��)
//////////////////////////////////////////////////////////////////

#ifndef __GuiBase_h__
#define __GuiBase_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI ����������
interface EXP_API IGuiBase : public IGuiComp, public IGuiSender
{
	EXP_DECLARE_DYNAMIC_MULT2(IGuiBase, IGuiComp, IGuiSender)

protected:
	CGC* m_GC;

	static IGuiBase* s_pCapture;

public:
	IGuiBase()
		: m_GC(ExMem::Alloc<CGC>())
	{
		// ����¼�����
		InsEvent((IGuiEvent*)ExGui(_T("CGuiWndEvent"), GetGC()));
	}
	virtual ~IGuiBase(void)
	{
		ExMem::Free(m_GC);
	}

public:
	void InsEvent(IGuiEvent* pEvent)
	{
		if (!pEvent) return ;
		// ��λ����
		evt_list_t::iterator_t ite = IGuiSender::Find(pEvent);
		if (ite != GetEvent().Tail()) return;
		// ����¶���
		if (GetEvent().Empty())
			GetEvent().Add(pEvent, GetEvent().Head());
		else
			GetEvent().Add(pEvent, GetEvent().Head() + 1);
	}

	virtual CGC* GetGC() { return m_GC; }

	virtual bool GetRealRect(CRect& rc) = 0;

	IGuiBase* GetPtCtrl(const CPoint& pt)
	{
		CRect rc;
		GetRealRect(rc);
		if (rc.PtInRect(pt))
		{
			if (GetChildren().Empty()) return this;
			for(list_t::iterator_t ite = GetChildren().Last(); ite != GetChildren().Head(); --ite)
			{
				IGuiBase* base = ExDynCast<IGuiBase>(*ite);
				if (!base) continue;
				base = base->GetPtCtrl(pt);
				if (base) return base;
			}
			IGuiBase* base = ExDynCast<IGuiBase>(GetChildren().HeadItem());
			if (base)
			{
				base = base->GetPtCtrl(pt);
				if (base) return base;
			}
			return this;
		}
		else
			return NULL;
	}

	void Free() { EXP_MULT::Free(); }

	virtual wnd_t GethWnd() = 0;

	// �����¼�����
	void SetCapture()
	{
		::SetCapture(GethWnd());
		s_pCapture = this;
	}
	static IGuiBase* GetCapture()
	{
		return s_pCapture;
	}
	void ReleaseCapture()
	{
		s_pCapture = NULL;
		::ReleaseCapture();
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiBase_h__*/
