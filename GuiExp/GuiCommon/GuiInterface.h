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
// Date:	2011-06-08
// Version:	1.0.0010.0047
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
//	- 1.0.0010.0047(2011-06-08)	= IGuiSender优先向后添加的事件对象转发消息
//								^ 将IGuiBase移出并单独实现
//////////////////////////////////////////////////////////////////

#ifndef __GuiInterface_h__
#define __GuiInterface_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI 对象接口
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

// GUI 组合接口
interface EXP_API IGuiComp : public IGuiObject
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
	list_t::iterator_t& Find(IGuiComp* pComp) { return list_t::finder_t::Find(GetChildren(), pComp); }

	// 组合接口
	virtual void AddComp(IGuiComp* pComp)
	{
		if (!pComp) return ;
		// 定位对象
		list_t::iterator_t ite = Find(pComp);
		if (ite != GetChildren().Tail()) return;
		// 添加新对象
		if( pComp->m_Pare )
			pComp->m_Pare->DelComp(pComp);
		pComp->Init(this);
		GetChildren().Add(pComp);
	}
	virtual void DelComp(IGuiComp* pComp)
	{
		if (!pComp) return ;
		// 定位对象
		list_t::iterator_t ite = Find(pComp);
		if (ite == GetChildren().Tail()) return;
		// 删除对象
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

// GUI 事件接口
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
	// 事件结果接口
	void SetResult(LRESULT lrRes = 0) { m_Result = lrRes; }
	LRESULT GetResult() { return m_Result; }
	// 事件传递接口
	virtual void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0) = 0;
};

//////////////////////////////////////////////////////////////////

// GUI 事件转发器
interface EXP_API IGuiSender : public IGuiObject, protected CListT<IGuiEvent*>
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
	void SetTrust(bool bTruCldr = true) { m_bTru = bTruCldr; }
	bool IsTrust() { return m_bTru; }
	// 获得内部对象
	evt_list_t& GetEvent() { return *m_CldrEvt; }

	// 查找
	evt_list_t::iterator_t& Find(IGuiEvent* pEvent) { return evt_list_t::finder_t::Find(GetEvent(), pEvent); }

	// 组合接口
	virtual void AddEvent(IGuiEvent* pEvent)
	{
		if (!pEvent) return ;
		// 定位对象
		evt_list_t::iterator_t ite = Find(pEvent);
		if (ite != GetEvent().Tail()) return;
		// 添加新对象
		GetEvent().Add(pEvent);
	}
	virtual void DelEvent(IGuiEvent* pEvent)
	{
		if (!pEvent) return ;
		// 定位对象
		evt_list_t::iterator_t ite = Find(pEvent);
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
		for(evt_list_t::iterator_t ite = GetEvent().Head(); ite != GetEvent().Tail(); ++ite)
		{
			if (!(*ite)) continue;
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
		// 后添加的事件优先执行
		evt_list_t::iterator_t ite = GetEvent().Last();
		for(; ite != GetEvent().Head(); --ite)
		{
			if (!(*ite)) continue;
			(*ite)->SetResult(ret); // 发送消息时,让事件对象收到上一个事件的处理结果
			(*ite)->OnMessage(pGui, nMessage, wParam, lParam);
			ret = (*ite)->GetResult();
		}
		ite = GetEvent().Head();
		if (*ite)
		{
			(*ite)->SetResult(ret); // 发送消息时,让事件对象收到上一个事件的处理结果
			(*ite)->OnMessage(pGui, nMessage, wParam, lParam);
		}
	}
};

//////////////////////////////////////////////////////////////////

// GUI 效果对象基础
interface EXP_API IGuiEffect : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiEffect, IGuiObject)

public:
	// 初始化
	virtual void Init(CImage& tImg) = 0;
	virtual bool IsInit() = 0;
	// 完毕
	virtual bool IsFinished() = 0;
	// 效果显示接口
	virtual void Show(IGuiObject* pGui, CImage& tImg) = 0;
	// 定时器
	virtual void SetTimer(wnd_t hWnd) = 0;
	virtual void KillTimer(wnd_t hWnd) = 0;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiInterface_h__*/