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
// Date:	2012-03-27
// Version:	1.0.0025.1216
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
//	- 1.0.0020.1110(2012-03-01)	+ 添加IGuiSender::PopEvent()与IGuiComp::PopComp(),方便删除事件与组合对象
//	- 1.0.0021.1830(2012-03-03)	= 调整IGuiComp::GetChildren()为IGuiComp::GetComp()
//	- 1.0.0022.1820(2012-03-21)	+ 添加IGuiItem与IGuiItemMgr,合并一些通用的元素聚合操作
//	- 1.0.0023.1505(2012-03-24)	+ IGuiItemMgr支持直接通过字符串创建或删除指定类型的IGuiItem对象
//	- 1.0.0024.1515(2012-03-26)	+ IGuiSetMgr支持直接通过其内部IGuiSet的Key值直接定位IGuiSet指针
//								= IGuiSetMgr将会自动覆盖掉Key值相同的IGuiSet对象(Key为空的不会被覆盖)
//	- 1.0.0025.1216(2012-03-27)	+ IGuiEvent支持通过GetPrev()和GetNext()枚举上/下一个事件接口对象
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
	BOOL IsValid() const { return (!m_bFree); }
	virtual void Free() { del(this); }
};

//////////////////////////////////////////////////////////////////

EXP_INTERFACE IGuiItemMgr;

// GUI 元素对象接口
EXP_INTERFACE IGuiItem : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiItem, IGuiObject)

	friend interface IGuiItemMgr;

protected:
	IGuiItemMgr* m_Pare;		// 父对象指针
	BOOL m_bTru;

public:
	IGuiItem()
		: m_bTru(TRUE)
	{}
	virtual ~IGuiItem()
	{}

public:
	IGuiItemMgr* GetPare() const { return m_Pare; }
	virtual void SetTrust(BOOL bTru = TRUE) { m_bTru = bTru; }
	virtual BOOL IsTrust() const { return m_bTru; }
};

//////////////////////////////////////////////////////////////////

// GUI 元素对象管理器
EXP_INTERFACE IGuiItemMgr : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiItemMgr, IGuiObject)

public:
	typedef CListT<IGuiItem*> itm_list_t;
	typedef itm_list_t::iterator_t itm_iter_t;
	typedef CMapT<CString, itm_iter_t> itm_map_t;

protected:
	BOOL m_bTru;		// 子容器链托管标记
	itm_list_t* m_CldrItm;
	itm_map_t* m_CldrMap;

public:
	IGuiItemMgr(void)
		: m_bTru(TRUE)
		, m_CldrItm(dbnew(itm_list_t))
		, m_CldrMap(dbnew(itm_map_t))
	{}
	virtual ~IGuiItemMgr(void)
	{
		Clear();
		del(m_CldrMap);
		m_CldrMap = NULL;
		del(m_CldrItm);
		m_CldrItm = NULL;
	}

public:
	// 是否对子容器做托管
	void SetTrust(BOOL bTruCldr = TRUE) { m_bTru = bTruCldr; }
	BOOL IsTrust() const { return m_bTru; }
	// 获得内部对象
	itm_list_t& GetItm() const { return *m_CldrItm; }

	// 查找
	itm_iter_t Find(IGuiItem* p) const { return GetItm().Find(p); }
	itm_iter_t Find(LPCTSTR key) const
	{
		itm_map_t::iterator_t map_ite = m_CldrMap->Locate(key);
		if (map_ite == m_CldrMap->Tail()) return GetItm().Tail();
		return map_ite->Val();
	}

	// 组合接口
	BOOL Add(void* p)
	{
		IGuiItem* itm = ExDynCast<IGuiItem>(p);
		if (!itm) return FALSE;
		// 定位对象
		itm_iter_t ite = Find(itm);
		if (ite != GetItm().Tail()) return TRUE;
		// 添加新对象
		itm->m_Pare = this;
		return GetItm().Add(itm);
	}
	BOOL Add(LPCTSTR key)
	{
		void* p = ExGui(key);
		if(!Add(p))
		{
			del(p);
			return FALSE;
		}
		Del(key);
		(*m_CldrMap)[key] = GetItm().Last();
		return TRUE;
	}
	BOOL Ins(void* p, itm_iter_t i)
	{
		IGuiItem* itm = ExDynCast<IGuiItem>(p);
		if (!itm) return FALSE;
		// 定位对象
		itm_iter_t ite = Find(itm);
		if (ite != GetItm().Tail()) return TRUE;
		// 添加新对象
		itm->m_Pare = this;
		return GetItm().Add(itm, i);
	}
	BOOL Ins(void* p)
	{
		return Ins(p, GetItm().Head());
	}
	BOOL Ins(LPCTSTR key, itm_iter_t i)
	{
		void* p = ExGui(key);
		if(!Ins(p, i))
		{
			del(p);
			return FALSE;
		}
		Del(key);
		(*m_CldrMap)[key] = i;
		return TRUE;
	}
	BOOL Ins(LPCTSTR key)
	{
		return Ins(key, GetItm().Head());
	}
	BOOL Del(void* p)
	{
		IGuiItem* itm = ExDynCast<IGuiItem>(p);
		if (!itm) return FALSE;
		// 定位对象
		itm_iter_t ite = Find(itm);
		if (ite == GetItm().Tail()) return TRUE;
		// 删除对象
		m_CldrMap->Replace(ite);
		if (!GetItm().Del(ite)) return FALSE;
		if (IsTrust() && itm->IsTrust()) itm->Free();
		return TRUE;
	}
	BOOL Del(LPCTSTR key)
	{
		// 定位对象
		itm_iter_t ite = Find(key);
		if (ite == GetItm().Tail()) return TRUE;
		// 删除对象
		m_CldrMap->Del(key);
		if (!GetItm().Del(ite)) return FALSE;
		if (IsTrust() && (*ite)->IsTrust()) (*ite)->Free();
		return TRUE;
	}
	BOOL Pop(BOOL bLast = TRUE)
	{
		if (GetItm().Empty()) return TRUE;
		IGuiItem* itm = NULL;
		if (bLast)
		{
			itm = GetItm().LastItem();
			m_CldrMap->Replace(GetItm().Last());
			GetItm().PopLast();
		}
		else
		{
			itm = GetItm().HeadItem();
			m_CldrMap->Replace(GetItm().Head());
			GetItm().PopHead();
		}
		if (IsTrust() && itm && itm->IsTrust()) itm->Free();
		return TRUE;
	}
	void Clear()
	{
		if (IsTrust())
			for(itm_iter_t ite = GetItm().Head(); ite != GetItm().Tail(); ++ite)
			{
				if (!(*ite) || 
					!(*ite)->IsValid() || 
					!(*ite)->IsTrust()) continue;
				(*ite)->Free();
			}
		m_CldrMap->Clear();
		GetItm().Clear();
	}
};

//////////////////////////////////////////////////////////////////

EXP_INTERFACE IGuiComp;
EXP_INTERFACE IGuiCtl;

// GUI 事件接口
EXP_INTERFACE IGuiEvent : public IGuiItem
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiEvent, IGuiItem)

	friend EXP_INTERFACE IGuiComp;

public:
	enum state_t
	{
		continue_next,	// 继续下个事件
		break_next,		// 跳过下个事件
		return_next, 	// 下个事件时跳出事件队列
	};

protected:
	LRESULT m_Result;
	state_t m_State;

protected:
	// 鼠标离开检测的全局静态控件指针
	static IGuiCtl* s_MLMove;

public:
	IGuiEvent()
		: m_Result(0)
		, m_State(continue_next)
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

	// 事件对象枚举接口
	IGuiEvent* GetPrev() const
	{
		IGuiItemMgr* pare = GetPare();
		if (!pare) return NULL;
		IGuiItemMgr::itm_iter_t ite = pare->Find((IGuiItem*)this);
		if (ite == pare->GetItm().Tail()) return NULL;
		if (ite == pare->GetItm().Last()) return NULL;
		return (IGuiEvent*)(*(++ite));
	}
	IGuiEvent* GetNext() const
	{
		IGuiItemMgr* pare = GetPare();
		if (!pare) return NULL;
		IGuiItemMgr::itm_iter_t ite = pare->Find((IGuiItem*)this);
		if (ite == pare->GetItm().Tail()) return NULL;
		if (ite == pare->GetItm().Head()) return NULL;
		return (IGuiEvent*)(*(--ite));
	}

	// 事件传递接口
	virtual void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0) = 0;
};

//////////////////////////////////////////////////////////////////

// GUI 事件转发器
EXP_INTERFACE IGuiSender : public IGuiItemMgr
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiSender, IGuiItemMgr)

public:
	typedef itm_list_t evt_list_t;
	typedef evt_list_t::iterator_t evt_iter_t;

public:
	// 是否对子容器做托管
	void SetTrustEvent(BOOL bTruCldr = TRUE) { SetTrust(bTruCldr); }
	BOOL IsTrustEvent() const { return IsTrust(); }

	// 获得内部对象
	evt_list_t& GetEvent() const { return GetItm(); }

	// 查找
	evt_iter_t FindEvent(IGuiEvent* p) const { return Find(p); }

	// 组合接口
	virtual void AddEvent(void* p) { Add(p); }
	virtual void AddEvent(LPCTSTR key) { Add(key); }
	virtual void InsEvent(void* p) { Ins(p); }
	virtual void InsEvent(LPCTSTR key) { Ins(key); }
	virtual void DelEvent(void* p) { Del(p); }
	virtual void DelEvent(LPCTSTR key) { Del(key); }
	virtual void PopEvent(BOOL bLast = TRUE) { Pop(bLast); }
	virtual void ClearEvent() { Clear(); }

	// 事件结果接口
	void SetResult(LRESULT lrRet = 0)
	{
		if(!IsValid()) return;
		for(evt_iter_t ite = GetEvent().Head(); ite != GetEvent().Tail(); ++ite)
		{
			IGuiEvent* evt = (IGuiEvent*)(*ite);
			ExAssert(evt);
			evt->SetResult(lrRet);
		}
	}
	LRESULT GetResult(LRESULT lrDef = 0)
	{
		if(!IsValid()) return NULL;
		if (GetEvent().Empty()) return lrDef;
		// 后添加的事件优先执行
		evt_iter_t ite = GetEvent().Last();
		for(; ite != GetEvent().Head(); --ite)
		{
			IGuiEvent* evt = (IGuiEvent*)(*ite);
			if (!evt) continue;
			LRESULT r = evt->GetResult();
			if (r != 0 && lrDef != r) lrDef = r;
		}
		IGuiEvent* evt = (IGuiEvent*)(*ite);
		if (evt)
		{
			LRESULT r = evt->GetResult();
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
		evt_iter_t ite = GetEvent().Last();
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
			IGuiEvent* evt = (IGuiEvent*)(*ite);
			if (!evt) continue;
			evt->SetResult(ret); // 发送消息时,让事件对象收到上一个事件的处理结果
			evt->OnMessage(gui, nMessage, wParam, lParam);
			if (!evt->IsValid()) return;
			sta = evt->GetState();
			LRESULT r = evt->GetResult();
			if (r != 0 && ret != r) ret = r;
		}
		if (sta != IGuiEvent::continue_next)
		{
			SetResult(ret);
			return;
		}
		IGuiEvent* evt = (IGuiEvent*)(*ite);
		if (evt)
		{
			evt->SetResult(ret); // 发送消息时,让事件对象收到上一个事件的处理结果
			evt->OnMessage(gui, nMessage, wParam, lParam);
			LRESULT r = evt->GetResult();
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
	BOOL IsTrust() const { return m_bTru; }
	// 获得内部对象
	IGuiComp* GetParent() const { return m_Pare; }
	list_t& GetComp() const { return *m_Cldr; }

	// 查找
	list_t::iterator_t FindComp(void* p) const { return GetComp().Find(ExDynCast<IGuiComp>(p)); }

	// 组合接口
	virtual void AddComp(void* p)
	{
		IGuiComp* cmp = ExDynCast<IGuiComp>(p);
		if (!cmp) return ;
		// 定位对象
		list_t::iterator_t ite = FindComp(cmp);
		if (ite != GetComp().Tail()) return;
		// 添加新对象
		if (cmp->m_Pare)
			cmp->m_Pare->DelComp(cmp);
		cmp->Init(this);
		GetComp().Add(cmp);
	}
	virtual void InsComp(void* p)
	{
		IGuiComp* cmp = ExDynCast<IGuiComp>(p);
		if (!cmp) return ;
		// 定位对象
		list_t::iterator_t ite = FindComp(cmp);
		if (ite != GetComp().Tail()) return;
		// 添加新对象
		if (cmp->m_Pare)
			cmp->m_Pare->DelComp(cmp);
		cmp->Init(this);
		GetComp().Add(cmp, GetComp().Head());
	}
	virtual void DelComp(void* p, BOOL bAutoTru = TRUE)
	{
		IGuiComp* cmp = ExDynCast<IGuiComp>(p);
		if (!cmp) return ;
		// 定位对象
		list_t::iterator_t ite = FindComp(cmp);
		if (ite == GetComp().Tail()) return;
		// 删除对象
		GetComp().Del(ite);
		cmp->Fina();
		if (bAutoTru && IsTrust())
		{
			if (IGuiEvent::s_MLMove == (IGuiCtl*)cmp)
				IGuiEvent::s_MLMove = NULL;
			cmp->Free();
		}
	}
	virtual void PopComp(BOOL bLast = TRUE, BOOL bAutoTru = TRUE)
	{
		if (GetComp().Empty()) return;
		IGuiComp* cmp = NULL;
		if (bLast)
		{
			cmp = GetComp().LastItem();
			GetComp().PopLast();
		}
		else
		{
			cmp = GetComp().HeadItem();
			GetComp().PopHead();
		}
		if(!cmp) return;
		cmp->Fina();
		if (bAutoTru && IsTrust() && cmp)
		{
			if (IGuiEvent::s_MLMove == (IGuiCtl*)cmp)
				IGuiEvent::s_MLMove = NULL;
			cmp->Free();
		}
	}
	virtual void ClearComp()
	{
		for(list_t::iterator_t ite = GetComp().Head(); ite != GetComp().Tail(); ++ite)
		{
			if (!(*ite)) continue;
			(*ite)->Fina();
			if (IsTrust()) (*ite)->Free();
		}
		GetComp().Clear();
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

// GUI 控件属性及逻辑设置对象
EXP_INTERFACE IGuiSet : public IGuiItem
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiSet, IGuiItem)

protected:
	IGuiCtl* m_Ctl;

public:
	IGuiSet()
		: m_Ctl(NULL)
	{}

public:
	// 获得该属性对象所属的控件指针(支持修改)
	EXP_INLINE IGuiCtl*& Ctl() { return m_Ctl; }
	// 获得该属性对象的属性关键字
	virtual CString GetKey() const { return _T(""); }
	// 传入一个字符串比较是否是该属性对象的属性关键字
	virtual BOOL Key(const CString& key) { return (key == GetKey()); }
	// 脚本解析接口
	virtual BOOL Exc(const CString& val) { return FALSE; }
	// 属性获取接口
	virtual void* Get(void* par) { return NULL; }
	// 属性设置接口
	virtual BOOL Set(void* sta, void* par) { return TRUE; }
	// 属性消息响应接口
	virtual void Msg(UINT nMessage, WPARAM wParam, LPARAM lParam) {}
};

//////////////////////////////////////////////////////////////////

// GUI 设置对象控制器
EXP_INTERFACE IGuiSetMgr : public IGuiItemMgr
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiSetMgr, IGuiItemMgr)

public:
	typedef itm_list_t set_list_t;
	typedef itm_map_t set_map_t;

protected:
	set_map_t m_KeyMap;

public:
	// 是否对子容器做托管
	void SetTrustSet(BOOL bTruCldr = TRUE) { SetTrust(bTruCldr); }
	BOOL IsTrustSet() const { return IsTrust(); }

	// 获得内部对象
	set_list_t& GetSet() const { return GetItm(); }

	// 查找
	set_list_t::iterator_t FindSet(IGuiSet* p) const { return Find(p); }
	set_list_t::iterator_t FindSet(LPCTSTR key) const { return Find(key); }
	IGuiSet* FindKeySet(const CString& key, BOOL bSearch = TRUE)
	{
		if (key.Empty())
		{
			for(set_list_t::iterator_t ite = GetSet().Head(); ite != GetSet().Tail(); ++ite)
			{
				IGuiSet* set = (IGuiSet*)(*ite);
				if(!set || !set->Key(key)) continue;
				return set;
			}
			return NULL;
		}
		else
		{
			set_map_t::iterator_t it = m_KeyMap.Locate(key);
			if (it == m_KeyMap.Tail())
			{
				if (bSearch)
				for(set_list_t::iterator_t ite = GetSet().Head(); ite != GetSet().Tail(); ++ite)
				{
					IGuiSet* set = (IGuiSet*)(*ite);
					if(!set || !set->Key(key)) continue;
					return set;
				}
				return NULL;
			}
			IGuiSet* set = (IGuiSet*)(*(*it));
			set->Key(key);
			return set;
		}
	}

	// 组合接口
	virtual BOOL AddSet(void* p)
	{
		IGuiSet* set = (IGuiSet*)(p);
		if (!set) return FALSE;
		CString str(set->GetKey());
		if (str.Empty()) return Add(p);
		// 替换掉当前已有的属性
		Del(FindKeySet(str, FALSE));
		if (!Add(p)) return FALSE;
		m_KeyMap[str] = GetSet().Last();
		return TRUE;
	}
	virtual BOOL AddSet(LPCTSTR key)
	{
		if (FindSet(key) != GetSet().Tail())
			Del(key)/*不删除m_KeyMap的映射*/;
		if (!Add(key)) return FALSE;
		set_list_t::iterator_t ite = GetSet().Last();
		IGuiSet* set = (IGuiSet*)(*ite);
		if (!set)
		{
			DelSet(key)/*删除m_KeyMap的映射*/;
			return FALSE;
		}
		CString str(set->GetKey());
		if (!str.Empty())
		{	// 替换掉当前已有的属性
			Del(FindKeySet(str, FALSE));
			m_KeyMap[str] = ite;
		}
		return TRUE;
	}
	virtual BOOL InsSet(void* p)
	{
		IGuiSet* set = (IGuiSet*)(p);
		if (!set) return FALSE;
		CString str(set->GetKey());
		if (str.Empty()) return Ins(p);
		// 替换掉当前已有的属性
		Del(FindKeySet(str, FALSE));
		if (!Ins(p)) return FALSE;
		if (!str.Empty()) m_KeyMap[str] = GetSet().Head();
		return TRUE;
	}
	virtual BOOL InsSet(LPCTSTR key)
	{
		if (FindSet(key) != GetSet().Tail())
			Del(key)/*不删除m_KeyMap的映射*/; // 替换掉当前已有的属性
		if (!Ins(key)) return FALSE;
		set_list_t::iterator_t ite = GetSet().Head();
		IGuiSet* set = (IGuiSet*)(*ite);
		if (!set)
		{
			DelSet(key)/*删除m_KeyMap的映射*/;
			return FALSE;
		}
		CString str(set->GetKey());
		if (!str.Empty())
		{	// 替换掉当前已有的属性
			Del(FindKeySet(str, FALSE));
			m_KeyMap[str] = ite;
		}
		return TRUE;
	}
	virtual BOOL DelSet(void* p)
	{
		IGuiSet* set = (IGuiSet*)(p);
		if (!set) return FALSE;
		CString str(set->GetKey());
		if (!str.Empty()) m_KeyMap.Del(str);
		return Del(p);
	}
	virtual BOOL DelSet(LPCTSTR key)
	{
		set_list_t::iterator_t ite = FindSet(key);
		IGuiSet* set = (IGuiSet*)(*ite);
		if (!set) return Del(key);
		CString str(set->GetKey());
		if (!str.Empty()) m_KeyMap.Del(str);
		return Del(key);
	}
	virtual BOOL PopSet(BOOL bLast = TRUE)
	{
		if (GetSet().Empty()) return TRUE;
		set_list_t::iterator_t ite;
		if (bLast)
			ite = GetSet().Last();
		else
			ite = GetSet().Head();
		IGuiSet* set = (IGuiSet*)(*ite);
		if (!set) return Pop(bLast);
		CString str(set->GetKey());
		if (!str.Empty()) m_KeyMap.Del(str);
		return Pop(bLast);
	}
	virtual void ClearSet()
	{
		m_KeyMap.Clear();
		Clear();
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiInterface_h__*/