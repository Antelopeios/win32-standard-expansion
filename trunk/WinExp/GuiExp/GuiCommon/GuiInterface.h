// Copyright 2011, 木头云
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
// Date:	2011-09-28
// Version:	1.0.0016.1517
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

public:
	IGuiObject() {}
	virtual ~IGuiObject() {}

public:
	virtual void Free() { ExMem::Free(this); }
};

//////////////////////////////////////////////////////////////////

// GUI 组合接口
EXP_INTERFACE IGuiComp : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiComp, IGuiObject)

public:
	typedef CListT<IGuiComp*> list_t;

protected:
	bool		m_bTru;		// 子容器链托管标记
	IGuiComp*	m_Pare;		// 父对象指针
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
	// 是否对子容器做托管
	void SetTrust(bool bTruCldr = true) { m_bTru = bTruCldr; }
	bool IsTrust() { return m_bTru; }
	// 获得内部对象
	IGuiComp* GetParent() { return m_Pare; }
	list_t& GetChildren() { return *m_Cldr; }

	// 查找
	list_t::iterator_t& FindComp(IGuiComp* pComp) { return GetChildren().Find(pComp); }

	// 组合接口
	virtual void AddComp(IGuiComp* pComp)
	{
		if (!pComp) return ;
		// 定位对象
		list_t::iterator_t ite = FindComp(pComp);
		if (ite != GetChildren().Tail()) return;
		// 添加新对象
		if( pComp->m_Pare )
			pComp->m_Pare->DelComp(pComp);
		pComp->Init(this);
		GetChildren().Add(pComp);
	}
	virtual void InsComp(IGuiComp* pComp)
	{
		if (!pComp) return ;
		// 定位对象
		list_t::iterator_t ite = FindComp(pComp);
		if (ite != GetChildren().Tail()) return;
		// 添加新对象
		if( pComp->m_Pare )
			pComp->m_Pare->DelComp(pComp);
		pComp->Init(this);
		GetChildren().Add(pComp, GetChildren().Head());
	}
	virtual void DelComp(IGuiComp* pComp, bool bAutoTru = true)
	{
		if (!pComp) return ;
		// 定位对象
		list_t::iterator_t ite = FindComp(pComp);
		if (ite == GetChildren().Tail()) return;
		// 删除对象
		GetChildren().Del(ite);
		pComp->Fina();
		if (bAutoTru && m_bTru) pComp->Free();
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

// GUI 事件接口
EXP_INTERFACE IGuiEvent : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiEvent, IGuiObject)

public:
	enum state_t
	{
		continue_next,	// 继续下个事件
		break_next,		// 跳过下个事件事件
		return_next, 	// 下个事件时跳出事件队列
	};

protected:
	LRESULT m_Result;
	state_t m_State;

public:
	IGuiEvent()
		: m_Result(0)
		, m_State(continue_next)
	{}
	virtual ~IGuiEvent()
	{}

public:
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
	bool m_bTru;		// 子容器链托管标记
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
	// 是否对子容器做托管
	void SetTrustEvent(bool bTruCldr = true) { m_bTru = bTruCldr; }
	bool IsTrustEvent() { return m_bTru; }
	// 获得内部对象
	evt_list_t& GetEvent() { return *m_CldrEvt; }

	// 查找
	evt_list_t::iterator_t& FindEvent(IGuiEvent* pEvent) { return GetEvent().Find(pEvent); }

	// 组合接口
	virtual void AddEvent(IGuiEvent* pEvent)
	{
		if (!pEvent) return ;
		// 定位对象
		evt_list_t::iterator_t ite = FindEvent(pEvent);
		if (ite != GetEvent().Tail()) return;
		// 添加新对象
		GetEvent().Add(pEvent);
	}
	virtual void InsEvent(IGuiEvent* pEvent)
	{
		if (!pEvent) return ;
		// 定位对象
		evt_list_t::iterator_t ite = FindEvent(pEvent);
		if (ite != GetEvent().Tail()) return;
		// 添加新对象
		GetEvent().Add(pEvent, GetEvent().Head());
	}
	virtual void DelEvent(IGuiEvent* pEvent)
	{
		if (!pEvent) return ;
		// 定位对象
		evt_list_t::iterator_t ite = FindEvent(pEvent);
		if (ite == GetEvent().Tail()) return;
		// 删除对象
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

	// 事件结果接口
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
	virtual void Send(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
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
				return;
			if (!(*ite)) continue;
			(*ite)->SetResult(ret); // 发送消息时,让事件对象收到上一个事件的处理结果
			(*ite)->OnMessage(pGui, nMessage, wParam, lParam);
			sta = (*ite)->GetState();
			LRESULT r = (*ite)->GetResult();
			if (r != 0 && ret != r) ret = r;
		}
		if (sta != IGuiEvent::continue_next) return;
		if (*ite)
		{
			(*ite)->SetResult(ret); // 发送消息时,让事件对象收到上一个事件的处理结果
			(*ite)->OnMessage(pGui, nMessage, wParam, lParam);
		}
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
	virtual bool IsInit() = 0;
	// 效果显示接口
	virtual void Show(IGuiObject* pGui, CImage& tImg) = 0;
	// 定时器
	virtual void SetTimer(wnd_t hWnd) = 0;
	virtual void KillTimer(wnd_t hWnd) = 0;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiInterface_h__*/