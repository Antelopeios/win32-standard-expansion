// Copyright 2011-2012, ľͷ��
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
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-27
// Version:	1.0.0025.1216
//
// History:
//	- 1.0.0001.1730(2011-05-05)	= GuiInterface�������������Ĺ����ӿ�
//	- 1.0.0002.1525(2011-05-11)	+ IGuiComp����й���Ϊ����Ӧ�ӿ�
//	- 1.0.0003.1428(2011-05-13)	= ����IGuiComp���й�ʱ����Ϊ
//								= ����IGuiEvent����Ľӿڼ���Ϊ
//								+ GUI �¼�ת����(IGuiSender)
//	- 1.0.0004.1527(2011-05-16)	+ ���IGuiComp::Init()��IGuiComp::Fina()�ӿ�
//	- 1.0.0005.1652(2011-05-18)	# ����IGuiComp::Init()����߼�����
//	- 1.0.0006.1620(2011-05-19)	+ ���IGuiBase�����������ඨ��
//								+ ���IGuiObject::Free()�ӿ�,����ExGui()���������ֶ��ͷ�ָ����Դ
//	- 1.0.0007.1000(2011-05-23)	+ ���IGuiEffectЧ�������������
//								= ����IGuiObject::Free()Ϊ�麯��
//	- 1.0.0008.1600(2011-05-25)	+ ���IGuiEffect::IsFinished();SetTimer();KillTimer()�ӿ�
//	- 1.0.0009.1411(2011-05-26)	+ ���IGuiBase::GetPtCtrl()��IGuiBase::GetRealRect()�ӿ�
//	- 1.0.0010.1447(2011-06-08)	^ ��IGuiBase�Ƴ�������ʵ��
//	- 1.0.0011.1530(2011-06-29)	# ����IGuiSender::Send()�ڲ�������Ϣ���ʱ���ܳ��ֵĴ���
//								^ IGuiSender����ӵ��¼�����ִ��,����ִ��������¼�����,�����ӿؼ�������Ϣ
//								+ �����Ͻӿ����Ins...()����,��������϶��е�ͷ������Ӷ���
//	- 1.0.0012.1523(2011-07-07)	# ����Comp��Event�ӿ���Find()������,��ֹ�ⲿ���ó�ͻ
//	- 1.0.0013.1537(2011-08-26)	# ����Comp��Event�ӿ���Trust��ؽӿڵ�����,��ֹ�ⲿ���ó�ͻ
//	- 1.0.0014.1646(2011-08-29)	+ ���IGuiEvent::Param()�ӿ�,�����ⲿ�Զ������
//	- 1.0.0015.1454(2011-09-02)	+ ���IGuiEvent���¼�״̬�ӿ�,����ǰ�¼��������,�ɸ��ݱ�����¼�״̬�ж��Ƿ����ִ����һ���¼�����
//	- 1.0.0016.1517(2011-09-28)	+ ΪIGuiComp::DelComp()�ӿ�����Ƿ��Զ��ͷ��Ƴ��Ĺ�������Ĳ���
//	- 1.0.0017.1720(2011-10-24)	# ������Event��MouseMoveʱ��Ctrlָ�������,���µ�һ��Comp�����Ƴ������Ctrl�Ӷ���ʱMouseMove�����Run-Time�쳣
//	- 1.0.0018.1712(2012-01-20)	+ IGuiEvent֧�ֶԵ�����ĳһ��ʱ����������й�
//								= IGuiSender��IGuiCompĬ���й������Ӷ���
//	- 1.0.0019.1802(2012-02-02)	+ ���IGuiObject::IsValid()
//								# ����ĳ�¼�����������������¼�֮��,�䷵��ֵ��Ȼ�п��ܱ�����δ��Ӧ�¼���Ĭ�Ϸ���ֵ���ǵ�����
//	- 1.0.0020.1110(2012-03-01)	+ ���IGuiSender::PopEvent()��IGuiComp::PopComp(),����ɾ���¼�����϶���
//	- 1.0.0021.1830(2012-03-03)	= ����IGuiComp::GetChildren()ΪIGuiComp::GetComp()
//	- 1.0.0022.1820(2012-03-21)	+ ���IGuiItem��IGuiItemMgr,�ϲ�һЩͨ�õ�Ԫ�ؾۺϲ���
//	- 1.0.0023.1505(2012-03-24)	+ IGuiItemMgr֧��ֱ��ͨ���ַ���������ɾ��ָ�����͵�IGuiItem����
//	- 1.0.0024.1515(2012-03-26)	+ IGuiSetMgr֧��ֱ��ͨ�����ڲ�IGuiSet��Keyֱֵ�Ӷ�λIGuiSetָ��
//								= IGuiSetMgr�����Զ����ǵ�Keyֵ��ͬ��IGuiSet����(KeyΪ�յĲ��ᱻ����)
//	- 1.0.0025.1216(2012-03-27)	+ IGuiEvent֧��ͨ��GetPrev()��GetNext()ö����/��һ���¼��ӿڶ���
//////////////////////////////////////////////////////////////////

#ifndef __GuiInterface_h__
#define __GuiInterface_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

// GUI ����ӿ�
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

// GUI Ԫ�ض���ӿ�
EXP_INTERFACE IGuiItem : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiItem, IGuiObject)

	friend interface IGuiItemMgr;

protected:
	IGuiItemMgr* m_Pare;		// ������ָ��
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

// GUI Ԫ�ض��������
EXP_INTERFACE IGuiItemMgr : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiItemMgr, IGuiObject)

public:
	typedef CListT<IGuiItem*> itm_list_t;
	typedef itm_list_t::iterator_t itm_iter_t;
	typedef CMapT<CString, itm_iter_t> itm_map_t;

protected:
	BOOL m_bTru;		// ���������йܱ��
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
	// �Ƿ�����������й�
	void SetTrust(BOOL bTruCldr = TRUE) { m_bTru = bTruCldr; }
	BOOL IsTrust() const { return m_bTru; }
	// ����ڲ�����
	itm_list_t& GetItm() const { return *m_CldrItm; }

	// ����
	itm_iter_t Find(IGuiItem* p) const { return GetItm().Find(p); }
	itm_iter_t Find(LPCTSTR key) const
	{
		itm_map_t::iterator_t map_ite = m_CldrMap->Locate(key);
		if (map_ite == m_CldrMap->Tail()) return GetItm().Tail();
		return map_ite->Val();
	}

	// ��Ͻӿ�
	BOOL Add(void* p)
	{
		IGuiItem* itm = ExDynCast<IGuiItem>(p);
		if (!itm) return FALSE;
		// ��λ����
		itm_iter_t ite = Find(itm);
		if (ite != GetItm().Tail()) return TRUE;
		// ����¶���
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
		// ��λ����
		itm_iter_t ite = Find(itm);
		if (ite != GetItm().Tail()) return TRUE;
		// ����¶���
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
		// ��λ����
		itm_iter_t ite = Find(itm);
		if (ite == GetItm().Tail()) return TRUE;
		// ɾ������
		m_CldrMap->Replace(ite);
		if (!GetItm().Del(ite)) return FALSE;
		if (IsTrust() && itm->IsTrust()) itm->Free();
		return TRUE;
	}
	BOOL Del(LPCTSTR key)
	{
		// ��λ����
		itm_iter_t ite = Find(key);
		if (ite == GetItm().Tail()) return TRUE;
		// ɾ������
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

// GUI �¼��ӿ�
EXP_INTERFACE IGuiEvent : public IGuiItem
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiEvent, IGuiItem)

	friend EXP_INTERFACE IGuiComp;

public:
	enum state_t
	{
		continue_next,	// �����¸��¼�
		break_next,		// �����¸��¼�
		return_next, 	// �¸��¼�ʱ�����¼�����
	};

protected:
	LRESULT m_Result;
	state_t m_State;

protected:
	// ����뿪����ȫ�־�̬�ؼ�ָ��
	static IGuiCtl* s_MLMove;

public:
	IGuiEvent()
		: m_Result(0)
		, m_State(continue_next)
	{}

public:
	// ����Ĵ洢ָ��
	virtual void* Param() { return NULL; }

	// �¼�����ӿ�
	void SetResult(LRESULT lrRes = 0) { m_Result = lrRes; }
	LRESULT GetResult() const { return m_Result; }

	// �¼�״̬�ӿ�
	void SetState(state_t eSta) { m_State = eSta; }
	state_t GetState()
	{
		state_t sta = m_State;
		m_State = continue_next;
		return sta;
	}

	// �¼�����ö�ٽӿ�
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

	// �¼����ݽӿ�
	virtual void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0) = 0;
};

//////////////////////////////////////////////////////////////////

// GUI �¼�ת����
EXP_INTERFACE IGuiSender : public IGuiItemMgr
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiSender, IGuiItemMgr)

public:
	typedef itm_list_t evt_list_t;
	typedef evt_list_t::iterator_t evt_iter_t;

public:
	// �Ƿ�����������й�
	void SetTrustEvent(BOOL bTruCldr = TRUE) { SetTrust(bTruCldr); }
	BOOL IsTrustEvent() const { return IsTrust(); }

	// ����ڲ�����
	evt_list_t& GetEvent() const { return GetItm(); }

	// ����
	evt_iter_t FindEvent(IGuiEvent* p) const { return Find(p); }

	// ��Ͻӿ�
	virtual void AddEvent(void* p) { Add(p); }
	virtual void AddEvent(LPCTSTR key) { Add(key); }
	virtual void InsEvent(void* p) { Ins(p); }
	virtual void InsEvent(LPCTSTR key) { Ins(key); }
	virtual void DelEvent(void* p) { Del(p); }
	virtual void DelEvent(LPCTSTR key) { Del(key); }
	virtual void PopEvent(BOOL bLast = TRUE) { Pop(bLast); }
	virtual void ClearEvent() { Clear(); }

	// �¼�����ӿ�
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
		// ����ӵ��¼�����ִ��
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
	// �¼����ͽӿ�
	virtual void Send(void* p, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		IGuiObject* gui = ExDynCast<IGuiObject>(p);
		if (!gui) return;
		if (GetEvent().Empty()) return;
		LRESULT ret = GetResult();
		IGuiEvent::state_t sta = IGuiEvent::continue_next;
		// ����ӵ��¼�����ִ��
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
			evt->SetResult(ret); // ������Ϣʱ,���¼������յ���һ���¼��Ĵ�����
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
			evt->SetResult(ret); // ������Ϣʱ,���¼������յ���һ���¼��Ĵ�����
			evt->OnMessage(gui, nMessage, wParam, lParam);
			LRESULT r = evt->GetResult();
			if (r != 0 && ret != r) ret = r;
		}
		SetResult(ret);
	}
};

//////////////////////////////////////////////////////////////////

// GUI ��Ͻӿ�
EXP_INTERFACE IGuiComp : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiComp, IGuiObject)

public:
	typedef CListT<IGuiComp*> list_t;

protected:
	BOOL		m_bTru;		// ���������йܱ��
	IGuiComp*	m_Pare;		// ������ָ��
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
	// �Ƿ�����������й�
	void SetTrust(BOOL bTruCldr = TRUE) { m_bTru = bTruCldr; }
	BOOL IsTrust() const { return m_bTru; }
	// ����ڲ�����
	IGuiComp* GetParent() const { return m_Pare; }
	list_t& GetComp() const { return *m_Cldr; }

	// ����
	list_t::iterator_t FindComp(void* p) const { return GetComp().Find(ExDynCast<IGuiComp>(p)); }

	// ��Ͻӿ�
	virtual void AddComp(void* p)
	{
		IGuiComp* cmp = ExDynCast<IGuiComp>(p);
		if (!cmp) return ;
		// ��λ����
		list_t::iterator_t ite = FindComp(cmp);
		if (ite != GetComp().Tail()) return;
		// ����¶���
		if (cmp->m_Pare)
			cmp->m_Pare->DelComp(cmp);
		cmp->Init(this);
		GetComp().Add(cmp);
	}
	virtual void InsComp(void* p)
	{
		IGuiComp* cmp = ExDynCast<IGuiComp>(p);
		if (!cmp) return ;
		// ��λ����
		list_t::iterator_t ite = FindComp(cmp);
		if (ite != GetComp().Tail()) return;
		// ����¶���
		if (cmp->m_Pare)
			cmp->m_Pare->DelComp(cmp);
		cmp->Init(this);
		GetComp().Add(cmp, GetComp().Head());
	}
	virtual void DelComp(void* p, BOOL bAutoTru = TRUE)
	{
		IGuiComp* cmp = ExDynCast<IGuiComp>(p);
		if (!cmp) return ;
		// ��λ����
		list_t::iterator_t ite = FindComp(cmp);
		if (ite == GetComp().Tail()) return;
		// ɾ������
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

// GUI Ч���������
EXP_INTERFACE IGuiEffect : public IGuiObject
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiEffect, IGuiObject)

public:
	// ��ʼ��
	virtual void Init(CImage& tImg) = 0;
	virtual BOOL IsInit() = 0;
	// Ч����ʾ�ӿ�
	virtual void Show(IGuiObject* pGui, CImage& tImg) = 0;
	// ��ʱ��
	virtual void SetTimer(wnd_t hWnd) = 0;
	virtual void KillTimer(wnd_t hWnd) = 0;
};

//////////////////////////////////////////////////////////////////

// GUI �ؼ����Լ��߼����ö���
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
	// ��ø����Զ��������Ŀؼ�ָ��(֧���޸�)
	EXP_INLINE IGuiCtl*& Ctl() { return m_Ctl; }
	// ��ø����Զ�������Թؼ���
	virtual CString GetKey() const { return _T(""); }
	// ����һ���ַ����Ƚ��Ƿ��Ǹ����Զ�������Թؼ���
	virtual BOOL Key(const CString& key) { return (key == GetKey()); }
	// �ű������ӿ�
	virtual BOOL Exc(const CString& val) { return FALSE; }
	// ���Ի�ȡ�ӿ�
	virtual void* Get(void* par) { return NULL; }
	// �������ýӿ�
	virtual BOOL Set(void* sta, void* par) { return TRUE; }
	// ������Ϣ��Ӧ�ӿ�
	virtual void Msg(UINT nMessage, WPARAM wParam, LPARAM lParam) {}
};

//////////////////////////////////////////////////////////////////

// GUI ���ö��������
EXP_INTERFACE IGuiSetMgr : public IGuiItemMgr
{
	EXP_DECLARE_DYNAMIC_CLS(IGuiSetMgr, IGuiItemMgr)

public:
	typedef itm_list_t set_list_t;
	typedef itm_map_t set_map_t;

protected:
	set_map_t m_KeyMap;

public:
	// �Ƿ�����������й�
	void SetTrustSet(BOOL bTruCldr = TRUE) { SetTrust(bTruCldr); }
	BOOL IsTrustSet() const { return IsTrust(); }

	// ����ڲ�����
	set_list_t& GetSet() const { return GetItm(); }

	// ����
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

	// ��Ͻӿ�
	virtual BOOL AddSet(void* p)
	{
		IGuiSet* set = (IGuiSet*)(p);
		if (!set) return FALSE;
		CString str(set->GetKey());
		if (str.Empty()) return Add(p);
		// �滻����ǰ���е�����
		Del(FindKeySet(str, FALSE));
		if (!Add(p)) return FALSE;
		m_KeyMap[str] = GetSet().Last();
		return TRUE;
	}
	virtual BOOL AddSet(LPCTSTR key)
	{
		if (FindSet(key) != GetSet().Tail())
			Del(key)/*��ɾ��m_KeyMap��ӳ��*/;
		if (!Add(key)) return FALSE;
		set_list_t::iterator_t ite = GetSet().Last();
		IGuiSet* set = (IGuiSet*)(*ite);
		if (!set)
		{
			DelSet(key)/*ɾ��m_KeyMap��ӳ��*/;
			return FALSE;
		}
		CString str(set->GetKey());
		if (!str.Empty())
		{	// �滻����ǰ���е�����
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
		// �滻����ǰ���е�����
		Del(FindKeySet(str, FALSE));
		if (!Ins(p)) return FALSE;
		if (!str.Empty()) m_KeyMap[str] = GetSet().Head();
		return TRUE;
	}
	virtual BOOL InsSet(LPCTSTR key)
	{
		if (FindSet(key) != GetSet().Tail())
			Del(key)/*��ɾ��m_KeyMap��ӳ��*/; // �滻����ǰ���е�����
		if (!Ins(key)) return FALSE;
		set_list_t::iterator_t ite = GetSet().Head();
		IGuiSet* set = (IGuiSet*)(*ite);
		if (!set)
		{
			DelSet(key)/*ɾ��m_KeyMap��ӳ��*/;
			return FALSE;
		}
		CString str(set->GetKey());
		if (!str.Empty())
		{	// �滻����ǰ���е�����
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