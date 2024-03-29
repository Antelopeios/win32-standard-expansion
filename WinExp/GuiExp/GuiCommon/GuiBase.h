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
// IGuiBase - 界面对象基础
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-04-26
// Version:	1.0.0009.1716
//
// History:
//	- 1.0.0002.0047(2011-06-08)	@ 完善IGuiBase,添加全局通用消息预处理并统一GC
//	- 1.0.0003.1544(2011-06-28)	+ 添加事件捕获接口,支持针对一个IGuiBase设置捕获所有的事件
//	- 1.0.0004.1020(2011-07-01)	+ 重写IGuiBase::InsEvent()接口,当事件链表中有数据时,向第二个结点处插入新结点(为了保证第一个插入的结点始终最后执行)
//	- 1.0.0005.1550(2011-07-07)	# IGuiBase::GetPtCtrl()会返回不可见控件,导致可见控件被遮住而无法响应消息
//	- 1.0.0006.2000(2011-07-16)	+ 在IGuiBase中添加绘图剪切区的相关接口
//	- 1.0.0007.1712(2012-01-20)	= IGuiBase::GetGC()默认返回空GC
//	- 1.0.0008.1417(2012-03-13)	+ 添加IGuiBase::ExcState()接口,方便外部调用时的名称统一(与调用IGuiBase::Execute()同义)
//	- 1.0.0009.1346(2012-04-26)	+ IGuiBase的消息穿透接口支持重写
//								+ IGuiBase支持保存一个额外的void*指针
//////////////////////////////////////////////////////////////////

#ifndef __GuiBase_h__
#define __GuiBase_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiManager/GuiManager.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI 界面对象基础
EXP_INTERFACE IGuiBase : public IGuiComp, public IGuiSender, public IExecutor
{
	EXP_DECLARE_DYNAMIC_MULT3(IGuiBase, IGuiComp, IGuiSender, IExecutor)

protected:
	static IGuiBase* s_pCapture;

	void* m_Param;

	CRect m_rcClip;

	BOOL m_bThrough;

public:
	IGuiBase()
		: m_Param(NULL)
		, m_bThrough(FALSE)
	{
		// 添加事件对象
		InsEvent(_T("CGuiWndEvent"));
	}
	virtual ~IGuiBase(void)
	{}

public:
	BOOL IsValid() const { return EXP_MULT::IsValid(); }

	void*& Param() { return m_Param; }

	void SetTrust(BOOL bTruCldr = TRUE) { IGuiComp::SetTrust(bTruCldr); }
	BOOL IsTrust() const { return IGuiComp::IsTrust(); }

	virtual BOOL SetThrough(BOOL bThrough = TRUE)
	{
		if (m_bThrough == bThrough) return m_bThrough;
		BOOL old = m_bThrough;
		m_bThrough = bThrough;
		return old;
	}
	virtual BOOL IsThrough() const { return m_bThrough; }

	virtual BOOL Execute(const CString& key, const CString& val)
	{
		return FALSE;
	}
	void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent)
	{
		CGuiXML::map_t::iterator_t it = ite->Val()->att.Head();
		for(; it != ite->Val()->att.Tail(); ++it)
			if(!Execute(it->Key(), it->Val())) break;
		if (it == ite->Val()->att.Head())
		{
			del(this);
			return NULL;
		}
		else
			return this;
	}
	virtual BOOL ExcState(const CString& key, const CString& val)
	{
		return Execute(key, val);
	}

	void InsEvent(void* p)
	{
		if (GetEvent().Empty())
			IGuiSender::Ins(p);
		else
			IGuiSender::Ins(p, GetEvent().Head() + 1);
	}
	void InsEvent(LPCTSTR key)
	{
		if (GetEvent().Empty())
			IGuiSender::Ins(key);
		else
			IGuiSender::Ins(key, GetEvent().Head() + 1);
	}

	void PopEvent(BOOL bLast = TRUE)
	{
		if (GetEvent().GetCount() <= 1)
			return;
		else
		if (bLast)
			IGuiSender::PopEvent(bLast);
		else
		{
			evt_iter_t ite = GetEvent().Head() + 1;
			IGuiItem* evt = *ite;
			m_CldrMap->Replace(ite);
			GetEvent().Del(ite);
			if (IsTrustEvent() && evt && evt->IsTrust()) evt->Free();
		}
	}

	virtual BOOL GetWindowRect(CRect& rc) const = 0;
	virtual BOOL GetClientRect(CRect& rc) const = 0;
	virtual BOOL GetRealRect(CRect& rc) const = 0;

	void SetClipBox(const CRect& rc)
	{ m_rcClip = rc; }
	void GetClipBox(CRect& rc) const
	{ rc = m_rcClip; }

	virtual BOOL IsVisible() const = 0;

	IGuiBase* GetPtCtrl(const CPoint& pt)
	{
		if (!IsVisible()) return NULL;
		CRect rc;
		GetRealRect(rc);
		if (rc.PtInRect(pt))
		{
			if (GetComp().Empty()) return this;
			for(list_t::iterator_t ite = GetComp().Last(); ite != GetComp().Head(); --ite)
			{
				IGuiBase* base = ExDynCast<IGuiBase>(*ite);
				if (!base || base->IsThrough()) continue;
				base = base->GetPtCtrl(pt);
				if (base) return base;
			}
			IGuiBase* base = ExDynCast<IGuiBase>(GetComp().HeadItem());
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

	virtual wnd_t GethWnd() const = 0;

	// 设置事件捕获
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
