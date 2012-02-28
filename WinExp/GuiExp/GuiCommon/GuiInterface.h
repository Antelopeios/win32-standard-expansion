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
// GuiInterface - 界面公用接口
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-02-02
// Version:	1.0.0019.1802
//
// History:
//	- 1.0.0001.1730(2011-05-05)	= GuiInterface里仅保留最基本的公共接口
//	- 1.0.0002.1525(2011-05-11)	+ IGuiComp添加托管行为及对应接口
//	- 1.0.0003.1428(2011-05-13)	= 调整IGuiComp在托管时的行为
//								= 调整IGuiEvent对外的接口及行为
//								+ GUI 事件转发器(IGuiSender)
//	- 1.0.0004.1527(2011-05-16)	+ 添加IGuiComp::Init()与IGuiComp::Fina()接口
//	- 1.0.0005.1652(2011-05-18)	# 修正IGuiComp::Init()里的逻辑错误
//	- 1.0.0006.1620(2011-05-19)	+ 添加IGuiBase界面对象基础类定义
//								+ 添加IGuiObject::Free()接口,方便ExGui()构造对象后手动释放指针资源
//	- 1.0.0007.1000(2011-05-23)	+ 添加IGuiEffect效果对象基础定义
//								= 调整IGuiObject::Free()为虚函数
//	- 1.0.0008.1600(2011-05-25)	+ 添加IGuiEffect::IsFinished();SetTimer();KillTimer()接口
//	- 1.0.0009.1411(2011-05-26)	+ 添加IGuiBase::GetPtCtrl()与IGuiBase::GetRealRect()接口
//	- 1.0.0010.1447(2011-06-08)	^ 将IGuiBase移出并单独实现
//	- 1.0.0011.1530(2011-06-29)	# 修正IGuiSender::Send()内部传递消息结果时可能出现的错误
//								^ IGuiSender后添加的事件优先执行,优先执行自身的事件对象,再向子控件传递消息
//								+ 相关组合接口添加Ins...()操作,用于在组合队列的头部添加子对象
//	- 1.0.0012.1523(2011-07-07)	# 调整Comp与Event接口内Find()的命名,防止外部调用冲突
//	- 1.0.0013.1537(2011-08-26)	# 调整Comp与Event接口内Trust相关接口的命名,防止外部调用冲突
//	- 1.0.0014.1646(2011-08-29)	+ 添加IGuiEvent::Param()接口,方便外部自定义参数
//	- 1.0.0015.1454(2011-09-02)	+ 添加IGuiEvent的事件状态接口,当当前事件处理完后,可根据保存的事件状态判断是否继续执行下一个事件对象
//	- 1.0.0016.1517(2011-09-28)	+ 为IGuiComp::DelComp()接口添加是否自动释放移除的关联对象的参数
//	- 1.0.0017.1720(2011-10-24)	# 修正因Event在MouseMove时对Ctrl指针的依赖,导致当一个Comp对象移除自身的Ctrl子对象时MouseMove引起的Run-Time异常
//	- 1.0.0018.1712(2012-01-20)	+ IGuiEvent支持对单独的某一个时间对象设置托管
//								= IGuiSender与IGuiComp默认托管所有子对象
//	- 1.0.0019.1802(2012-02-02)	+ 添加IGuiObject::IsValid()
//								# 修正某事件处理在跳过后面的事件之后,其返回值仍然有可能被后面未响应事件的默认返回值覆盖的问题
//////////////////////////////////////////////////////////////////

#ifndef __GuiInterface_h__
#define __GuiInterface_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI 对象接口
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

// GUI 事件接口
EXP_INTERFACE IGuiEvent : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiEvent, IGuiObject)

	friend EXP_INTERFACE IGuiComp;

public:
	enum state_t
	{
		continue_next,	// 继续下个事件
		break_next,		// 跳过下个事件
		return_next, 	// 下个事件时跳出事件队列
	};

protected:
	BOOL m_bTru;
	LRESULT m_Result;
	state_t m_State;

protected:
	// 鼠标离开检测的全局静态控件指针
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
	// 额外的存储指针
	virtual void* Param() { return NULL; }
	// 事件结果接口
	void SetResult(LRESULT lrRes = 0) { m_Result = lrRes; }
	LRESULT GetResult() const { return m_Result; }
	// 事件状态接口
	void SetState(state_t eSta) { m_State = eSta; }
	state_t GetState()
	{
		state_t sta = m_State;
		m_State = continue_next;
		return sta;
	}
	// 事件传递接口
	virtual void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0) = 0;
};

//////////////////////////////////////////////////////////////////

// GUI 事件转发器
EXP_INTERFACE IGuiSender : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiSender, IGuiObject)

public:
	typedef CListT<IGuiEvent*> evt_list_t;

protected:
	BOOL m_bTru;		// 子容器链托管标记
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
	// 是否对子容器做托管
	void SetTrustEvent(BOOL bTruCldr = TRUE) { m_bTru = bTruCldr; }
	BOOL IsTrustEvent() { return m_bTru; }
	// 获得内部对象
	evt_list_t& GetEvent() { return *m_CldrEvt; }

	// 查找
	evt_list_t::iterator_t FindEvent(IGuiEvent* pEvent) { return GetEvent().Find(pEvent); }

	// 组合接口
	virtual void AddEvent(void* p)
	{
		IGuiEvent* evt = ExDynCast<IGuiEvent>(p);
		if (!evt) return ;
		// 定位对象
		evt_list_t::iterator_t ite = FindEvent(evt);
		if (ite != GetEvent().Tail()) return;
		// 添加新对象
		GetEvent().Add(evt);
	}
	virtual void InsEvent(void* p)
	{
		IGuiEvent* evt = ExDynCast<IGuiEvent>(p);
		if (!evt) return ;
		// 定位对象
		evt_list_t::iterator_t ite = FindEvent(evt);
		if (ite != GetEvent().Tail()) return;
		// 添加新对象
		GetEvent().Add(evt, GetEvent().Head());
	}
	virtual void DelEvent(void* p = NULL)
	{
		IGuiEvent* evt = ExDynCast<IGuiEvent>(p);
		evt_list_t::iterator_t ite;
		if (evt)
		{
			// 定位对象
			ite = FindEvent(evt);
			if (ite == GetEvent().Tail()) return;
		}
		else
		{
			ite = GetEvent().Last();
			evt = GetEvent().LastItem();
		}
		// 删除对象
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

	// 事件结果接口
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
		// 后添加的事件优先执行
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
	// 事件发送接口
	virtual void Send(void* p, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiObject* gui = ExDynCast<IGuiObject>(p);
		if (!gui) return;
		if (GetEvent().Empty()) return;
		LRESULT ret = GetResult();
		IGuiEvent::state_t sta = IGuiEvent::continue_next;
		// 后添加的事件优先执行
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
			(*ite)->SetResult(ret); // 发送消息时,让事件对象收到上一个事件的处理结果
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
			(*ite)->SetResult(ret); // 发送消息时,让事件对象收到上一个事件的处理结果
			(*ite)->OnMessage(gui, nMessage, wParam, lParam);
			LRESULT r = (*ite)->GetResult();
			if (r != 0 && ret != r) ret = r;
		}
		SetResult(ret);
	}
};

//////////////////////////////////////////////////////////////////

// GUI 组合接口
EXP_INTERFACE IGuiComp : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiComp, IGuiObject)

public:
	typedef CListT<IGuiComp*> list_t;

protected:
	BOOL		m_bTru;		// 子容器链托管标记
	IGuiComp*	m_Pare;		// 父对象指针
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
	// 是否对子容器做托管
	void SetTrust(BOOL bTruCldr = TRUE) { m_bTru = bTruCldr; }
	BOOL IsTrust() { return m_bTru; }
	// 获得内部对象
	IGuiComp* GetParent() { return m_Pare; }
	list_t& GetChildren() { return *m_Cldr; }

	// 查找
	list_t::iterator_t FindComp(void* p) { return GetChildren().Find(ExDynCast<IGuiComp>(p)); }

	// 组合接口
	virtual void AddComp(void* p)
	{
		IGuiComp* cmp = ExDynCast<IGuiComp>(p);
		if (!cmp) return ;
		// 定位对象
		list_t::iterator_t ite = FindComp(cmp);
		if (ite != GetChildren().Tail()) return;
		// 添加新对象
		if (cmp->m_Pare)
			cmp->m_Pare->DelComp(cmp);
		cmp->Init(this);
		GetChildren().Add(cmp);
	}
	virtual void InsComp(void* p)
	{
		IGuiComp* cmp = ExDynCast<IGuiComp>(p);
		if (!cmp) return ;
		// 定位对象
		list_t::iterator_t ite = FindComp(cmp);
		if (ite != GetChildren().Tail()) return;
		// 添加新对象
		if (cmp->m_Pare)
			cmp->m_Pare->DelComp(cmp);
		cmp->Init(this);
		GetChildren().Add(cmp, GetChildren().Head());
	}
	virtual void DelComp(void* p, BOOL bAutoTru = TRUE)
	{
		IGuiComp* cmp = ExDynCast<IGuiComp>(p);
		if (!cmp) return ;
		// 定位对象
		list_t::iterator_t ite = FindComp(cmp);
		if (ite == GetChildren().Tail()) return;
		// 删除对象
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

// GUI 效果对象基础
EXP_INTERFACE IGuiEffect : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiEffect, IGuiObject)

public:
	// 初始化
	virtual void Init(CImage& tImg) = 0;
	virtual BOOL IsInit() = 0;
	// 效果显示接口
	virtual void Show(IGuiObject* pGui, CImage& tImg) = 0;
	// 定时器
	virtual void SetTimer(wnd_t hWnd) = 0;
	virtual void KillTimer(wnd_t hWnd) = 0;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiInterface_h__*/