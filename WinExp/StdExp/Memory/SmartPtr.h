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
// SmartPtr - ����ָ��
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-30
// Version:	1.3.0028.1804
//
// History:
//	- 1.0.0001.1148(2009-08-13)	@ ��ɻ�������ģ�幹��
//	- 1.0.0002.1758(2009-08-14)	+ ���TSmartPtr����ز���������(==)
//								+ ���CSmartPtrManager����ָ������༰ȫ�ֵ�����ָ���������
//	- 1.0.0003.1508(2009-08-17)	^ �Ż�CSmartPtrManagerʹ֧֮�����ü���
//								+ ���TSmartPtr����ز���������(*)
//	- 1.0.0004.1600(2009-08-18)	^ �Ż�CSmartPtrManagerָ����������listΪmap
//								- ɾ��CSmartPtrManager���Find()����
//								= ����TSmartPtr��ģ�����,��is_ary��ΪDelFunction����ָ��
//								= ����CSmartPtrManager��,���亯����Ϊ��̬����,�������Զ����ն���CAutoRecycle
//								+ ���CDelFunc��,����ͳһ����ָ��ɾ���㷨
//	- 1.0.0005.1720(2009-08-19)	+ ���TSmartPtr����ز���������([],+,-)
//								= ����TSmartPtr��=�������ķ���ֵΪTSmartPtr&
//								# ������ָ�����Ҹ���ʱ���µ�ָ�뱻���ٵ�bug
//	- 1.1.0006.2300(2009-08-20)	+ ���TSmartPtr����س�Ա�����Բ�ͬ�ͱ�ָ���֧��
//								- ɾ��CSmartPtrManager������,��TSmartPtr��ʹ��TPtrָ�������ָ���Զ�����ָ�����
//								= ����_Ptr��_DelFun�����ض��嵽CDelFunc���ڲ�
//								^ �Ż�TSmartPtr(TYPE*)���캯����ִ��Ч��
//								= �����༰���Ա������������ʽ
//	- 1.1.0007.1645(2009-08-21)	+ ���TPtr����ز���������(!=)
//								+ ���TSmartPtr����ز���������(!=)
//	- 1.1.0008.1255(2009-08-25)	# ����TSmartPtr��bool operator==()��bool operator!=()��NULLָ����жϴ���
//	- 1.1.0009.1353(2009-08-27)	= ����TPtr��TSmartPtr��ģ��TYPE������Ĭ��ֵΪCDelFunc::_Ptr
//	- 1.1.0010.1200(2009-08-31)	# ������ʹ����ָ��,����ָ������ʱ�������������������bug
//	- 1.2.0018.1500(2011-02-24)	# ��SmartPtr���Ͻ�StdExp֮��,SmartPtr::Release()û�е���alloc_t�ͷ�ptr��Դ
//								# �����򻯸�дCReferPtrTʱû�н��ڲ���protected��Ա��Ϊpublic��Ա����ı������
//								+ ���SmartPtr::Inc()��SmartPtr::Dec()�ӿ�,����ѭ�����õ��������ʱ�ֶ��������ü���
//	- 1.3.0019.2100(2011-02-24)	= �ı����ü����Ĺ���ʽ,��ԭΪ��ʼ��Manager��ͳһ����(������GCЭͬ����)
//	- 1.3.0020.0400(2011-02-25)	# ��ʹ�ö��߳�ģ��ʱ,PtrManager��DefMemAlloc�ľ�̬��ʼ��������ͻ
//	- 1.3.0021.1723(2011-02-28)	+ ���SmartPtr::operator!()����������
//	- 1.3.0022.2000(2011-03-01)	# ����SmartPtr::Inc()�ڲ���ģ����ô���
//	- 1.3.0023.2110(2011-03-04)	= CPtrManagerT::CReferPtrT������ڴ���CPtrManagerT��AllocT����
//								+ CPtrManagerT::Clear()֧�ִ��ζ����Ƿ���ȫ����CPtrManagerT::m_ReferPtrs�������ڴ�
//	- 1.3.0024.1130(2011-04-02)	# ����CPtrManagerT::CReferPtrT::Dec()���ܳ��ֶ��Free���������
//	- 1.3.0025.1858(2011-05-11)	^ �Ż�CPtrManagerT�ĵ���ʵ�ַ�ʽ
//	- 1.3.0026.1724(2011-05-12)	# ����CSmartPtrT����ָ��Ĺ��캯����ʵ���ϴ������޵ݹ������
//	- 1.3.0027.1619(2011-05-19)	= CPtrManagerT�ڵ��������,�ڲ�����ᱻ�ڴ���Զ�����,����������������κι���
//	- 1.3.0028.1804(2011-05-30)	= ��CPtrManagerT������Ϊһ��������ģ��ά��
//								= ����CSmartPtrTģ���ڲ�����,����TypeTֱ����Ϊ�ڲ�����������,������TypeT*
//								+ CSmartPtrT֧�ֲ��й��ڲ�����
//////////////////////////////////////////////////////////////////

#ifndef __SmartPtr_h__
#define __SmartPtr_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/MemAlloc.h"
#include "Memory/PtrManager.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// ����ָ����ģ��
template <typename TypeT, typename AllocT = EXP_MEMORY_ALLOC, typename ModelT = EXP_THREAD_MODEL>
class CSmartPtrT
{
public:
	typedef TypeT type_t;
	typedef AllocT alloc_t;
	typedef ModelT model_t;

	// ��Ա����
protected:
	type_t m_Ptr;
	bool m_bTru;

	// ����/����
public:
	CSmartPtrT(void)
		: m_Ptr(NULL)
		, m_bTru(true)
	{}
	CSmartPtrT(type_t ptr)
		: m_Ptr(NULL)
		, m_bTru(true)
	{ (*this) = ptr; }
	CSmartPtrT(const CSmartPtrT& ptr)
		: m_Ptr(NULL)
		, m_bTru(true)
	{ (*this) = ptr; }

	//////////////////////////////////

	template <typename Type2T>
	CSmartPtrT(Type2T ptr)
		: m_Ptr(NULL)
		, m_bTru(true)
	{ (*this) = ptr; }
	template <typename Type2T>
	CSmartPtrT(const CSmartPtrT<Type2T>& ptr)
		: m_Ptr(NULL)
		, m_bTru(true)
	{ (*this) = ptr; }

	//////////////////////////////////

	~CSmartPtrT(void)
	{ Dec(); }

	// ����
public:
	long GetRefCount()
	{ return CPtrManager::Instance().Get(m_Ptr); }

	// �Ƿ����й�
	void SetTrust(bool bTru = true) { m_bTru = bTru; }
	bool IsTrust() { return m_bTru; }

	void Inc()
	{ if (m_bTru) CPtrManager::Instance().Add<alloc_t, model_t>(m_Ptr); }
	void Dec()
	{ if (m_bTru) CPtrManager::Instance().Del(m_Ptr); }

	void Release()
	{
		CPtrManager::Instance().Del(m_Ptr, true);
		m_Ptr = NULL;
	}

	//////////////////////////////////

	CSmartPtrT& operator=(type_t ptr)
	{
		if ((*this) == ptr) return (*this);
		Dec();
		m_Ptr = ptr;
		Inc();
		return (*this);
	}
	CSmartPtrT& operator=(const CSmartPtrT& ptr)
	{ return ((*this) = ptr.m_Ptr); }

	bool operator==(type_t ptr) const
	{
		if( m_Ptr )
			return (m_Ptr == ptr);
		else if( !ptr )
			return true;
		else
			return false;
	}
	bool operator==(const CSmartPtrT& ptr) const
	{ return (m_Ptr == ptr.m_Ptr); }
	bool operator!=(type_t ptr) const
	{
		if( m_Ptr )
			return (m_Ptr != ptr);
		else if( ptr )
			return true;
		else
			return false;
	}
	bool operator!=(const CSmartPtrT& ptr) const
	{ return (m_Ptr != ptr.m_Ptr); }

	type_t operator+(DWORD offset) const
	{ return (m_Ptr + offset); }
	type_t operator-(DWORD offset) const
	{ return (m_Ptr - offset); }

	bool operator!() const
	{ return !m_Ptr; }

	type_t operator->() const
	{ return m_Ptr; }
	operator type_t() const
	{ return m_Ptr; }

	//////////////////////////////////

	template <typename Type2T>
	CSmartPtrT& operator=(Type2T ptr)
	{
		if ((*this) == ptr) return (*this);
		Dec();
		m_Ptr = (type_t)ptr;
		Inc();
		return (*this);
	}
	template <typename Type2T>
	CSmartPtrT& operator=(const CSmartPtrT<Type2T>& ptr)
	{ return ((*this) = ptr.m_Ptr); }

	template <typename Type2T>
	bool operator==(Type2T ptr) const
	{ return ((*this) == (type_t)ptr); }
	template <typename Type2T>
	bool operator==(const CSmartPtrT<Type2T>& ptr) const
	{ return (m_Ptr == (type_t)ptr.m_Ptr); }
	template <typename Type2T>
	bool operator!=(Type2T ptr) const
	{ return ((*this) != (type_t)ptr); }
	template <typename Type2T>
	bool operator!=(const CSmartPtrT<Type2T>& ptr) const
	{ return (m_Ptr != (type_t)ptr.m_Ptr); }

	template <typename Type2T>
	operator Type2T() const
	{ return (Type2T)m_Ptr; }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__SmartPtr_h__*/