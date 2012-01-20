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
// Date:	2012-01-20
// Version:	1.0.0007.1712
//
// History:
//	- 1.0.0002.0047(2011-06-08)	@ 完善IGuiBase,添加全局通用消息预处理并统一GC
//	- 1.0.0003.1544(2011-06-28)	+ 添加事件捕获接口,支持针对一个IGuiBase设置捕获所有的事件
//	- 1.0.0004.1020(2011-07-01)	+ 重写IGuiBase::InsEvent()接口,当事件链表中有数据时,向第二个结点处插入新结点(为了保证第一个插入的结点始终最后执行)
//	- 1.0.0005.1550(2011-07-07)	# IGuiBase::GetPtCtrl()会返回不可见控件,导致可见控件被遮住而无法响应消息
//	- 1.0.0006.2000(2011-07-16)	+ 在IGuiBase中添加绘图剪切区的相关接口
//	- 1.0.0007.1712(2012-01-20)	= IGuiBase::GetGC()默认返回空GC
//////////////////////////////////////////////////////////////////

#ifndef __GuiBase_h__
#define __GuiBase_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI 界面对象基础
EXP_INTERFACE IGuiBase : public IGuiComp, public IGuiSender
{
	EXP_DECLARE_DYNAMIC_MULT2(IGuiBase, IGuiComp, IGuiSender)

protected:
	CGC* m_GC;

	static IGuiBase* s_pCapture;

	CRect m_rcClip;

public:
	IGuiBase()
		: m_GC(NULL/*ExMem::Alloc<CGC>()*/)
	{
		// 添加事件对象
		InsEvent((IGuiEvent*)ExGui(_T("CGuiWndEvent"), GetGC()));
	}
	virtual ~IGuiBase(void)
	{
	//	ExMem::Free(m_GC);
	}

public:
	void InsEvent(IGuiEvent* pEvent)
	{
		if (!pEvent) return ;
		// 定位对象
		evt_list_t::iterator_t ite = FindEvent(pEvent);
		if (ite != GetEvent().Tail()) return;
		// 添加新对象
		if (GetEvent().Empty())
			GetEvent().Add(pEvent, GetEvent().Head());
		else
			GetEvent().Add(pEvent, GetEvent().Head() + 1);
	}

	virtual CGC* GetGC() { return m_GC; }

	virtual BOOL GetRealRect(CRect& rc) = 0;

	void SetClipBox(const CRect& rc)
	{ m_rcClip = rc; }
	void GetClipBox(CRect& rc)
	{ rc = m_rcClip; }

	virtual BOOL IsVisible() const = 0;

	IGuiBase* GetPtCtrl(const CPoint& pt)
	{
		if (!IsVisible()) return NULL;
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
