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
// GuiCtrl - 控件对象
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-04-21
// Version:	1.0.0027.2235
//
// History:
//	- 1.0.0001.2236(2011-05-23)	+ IGuiCtrl添加效果对象相关接口
//								+ 添加IGuiCtrl::IsUpdated()接口
//	- 1.0.0002.1517(2011-05-25)	# 当控件无效时IGuiCtrl::SetFocus()应该直接返回
//	- 1.0.0003.1746(2011-05-26)	# IGuiCtrl::SetFocus()应该同时递归设置父控件焦点
//								# IGuiCtrl::GetFocus()应该直接返回最底层的焦点控件
//	- 1.0.0004.1709(2011-06-03)	= 调整IGuiCtrl区域控制接口的命名
//								+ 添加IGuiCtrl::GetClientRect()接口
//	- 1.0.0005.2304(2011-06-08)	+ 添加IGuiCtrl::UpdateState()接口
//								+ 在IGuiCtrl::SetFocus()接口实现中添加更新状态逻辑
//	- 1.0.0006.1010(2011-06-17)	= 将IGuiCtrlBase接口移动到GuiCtrl.h中,使外部可以使用此接口
//	- 1.0.0007.1732(2011-06-24)	+ 添加静态IGuiCtrl::SetFocus()接口
//	- 1.0.0008.2000(2011-07-16)	+ 添加IGuiCtrl::GetClipRect()接口,用于绘图时动态获取当前剪切区下的绘图区域
//	- 1.0.0009.2111(2011-07-31)	= IGuiCtrl::Init()将判断控件是否可见,并发送WM_SHOWWINDOW消息
//	- 1.0.0010.1720(2011-08-03)	+ IGuiCtrl添加Scroll系列接口,支持事件响应获取滚动偏移并处理绘图
//	- 1.0.0011.1714(2011-08-08)	+ IGuiCtrl添加Scroll控件绑定接口
//	- 1.0.0012.1715(2011-08-09)	# 当IGuiCtrl没有父对象时忽略消息传递
//								^ 优化Scroll控件绑定接口的消息传递
//	- 1.0.0013.1730(2011-08-11)	= IGuiCtrl在SetFocus时先发送焦点失去消息,再发送焦点获得消息
//	- 1.0.0014.1816(2011-08-24)	+ 当IGuiCtrl添加一个Scroll对象时,自动对它设置main属性
//	- 1.0.0015.1643(2011-08-25)	^ 大幅简化IGuiCtrl::GetState()接口的调用方式及效率
//	- 1.0.0016.1318(2011-08-31)	^ 当IGuiCtrl::SetScroll()传入的参数与当前m_Scroll相等,将不进行m_Scroll的刷新操作
//	- 1.0.0017.2154(2011-09-22)	^ 当IGuiCtrl::SetScrollSize()传入的参数与当前m_szScroll相等,将不进行后续的消息发送
//	- 1.0.0018.1802(2012-02-02)	+ 添加IGuiCtrl对效果对象的托管接口
//	- 1.0.0019.1200(2012-02-22)	= 将IGuiCtrl改名为IGuiCtl
//	- 1.0.0020.1723(2012-02-29)	% 修改并完善滚动条控制机制,并支持水平与垂直两种模式的滚动条
//	- 1.0.0021.2237(2012-03-03)	+ 添加itree_t树型集合,为需要树型集合的控件做准备
//								+ 添加set_ins_t,用于方便控制集合的插入操作,并为集合型控件添加insert,delete与clear属性
//	- 1.0.0022.0034(2012-03-04)	# 当调用IGuiCtl::SetWindowRect()的时候应该考虑滚动偏移,否则会导致控件位置的显示异常
//	- 1.0.0023.1603(2012-03-12)	+ 添加IGuiCtl::IsNeedScroll()接口,方便外部判断当前是否需要显示滚动条
//	- 1.0.0024.2356(2012-03-16)	^ 移除IGuiCtl::set_ins_t,在IGuiCtl::GetState()与IGuiCtl::SetState()接口上添加额外的param,可支持更为通用的属性设定
//								- 移除IGuiCtl::GetClipRect(),关于剪切区的控制全部交给绘图逻辑层负责
//	- 1.0.0025.1515(2012-03-24)	+ 添加控件设置对象管理的相关接口,支持通过控件设置对象对控件的属性做动态管理
//	- 1.0.0026.1449(2012-04-17)	^ 简化控件对象的层次结构,移除IGuiCtrlBase接口,将其所有功能并入IGuiCtl
//	- 1.0.0027.2235(2012-04-21)	# 修正当窗口不可见时,控件的焦点设置仍然有效,导致界面响应的异常问题
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

// GUI 控件对象接口
EXP_INTERFACE IGuiCtl : public IGuiBase, public IGuiSetMgr
{
	EXP_DECLARE_DYNAMIC_MULT(IGuiCtl, IGuiBase)

public:
	typedef CListT<IGuiCtl*> items_t;
	typedef CTreeT<IGuiCtl*> itree_t;

protected:
	BOOL m_bEnable;		// 是否可用
	BOOL m_bVisible;	// 是否可见

	CRect m_Rect;		// 控件区域

	BOOL m_Updated;

	// 窗口区域
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

	// 控件设置对象管理
	BOOL AddSet(void* p);
	BOOL AddSet(LPCTSTR key);
	BOOL InsSet(void* p);
	BOOL InsSet(LPCTSTR key);

	// 获得控件状态
	BOOL Execute(const CString& key, const CString& val);
	void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent);
	virtual void* GetState(const CString& sType, void* pParam = NULL);
	virtual BOOL SetState(const CString& sType, void* pState, void* pParam = NULL);
	virtual void UpdateState(BOOL bRefreshSelf = TRUE);
	virtual BOOL IsUpdated();

	// 设置效果对象
	void SetEffectTrust(BOOL bTru = TRUE);
	void SetEffect(void* p);
	IGuiEffect* GetEffect();

	// 获得绘图板
	IGuiBase* GetParent() const;
	IGuiWnd* GetWnd() const;
	wnd_t GethWnd() const;

	// 区域控制
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

	// 判断是否可视(如控件在父控件窗口外)
	BOOL IsDisplayed() const;
	BOOL GetDisplaySize(CSize& sz) const;

	// 刷新绘图
	void Refresh(BOOL bSelf = TRUE);

	// 设置可用性
	BOOL SetEnable(BOOL bEnable = TRUE);
	BOOL IsEnabled() const;

	// 设置可见性
	BOOL SetVisible(BOOL bVisible = TRUE);
	BOOL IsVisible() const;

	// 判断有效性
	static BOOL IsEffect(const IGuiCtl* pCtrl);

	// 焦点控制
	static IGuiCtl* SetFocus(IGuiCtl* pFoc);
	virtual IGuiCtl* SetFocus();
	static IGuiCtl* GetFocus();
	virtual BOOL IsFocus() const;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiCtrl_h__*/