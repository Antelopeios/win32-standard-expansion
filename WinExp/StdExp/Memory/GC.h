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
// GC - ����������
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-28
// Version:	1.1.0020.2357
//
// History:
//	- 1.1.0013.2300(2011-02-24)	^ �Ż�CGCT::Free()��ʵ��
//								+ ֧����SmartPtr����ָ��ͬʱʹ��
//	- 1.1.0014.2000(2011-03-01)	# ��GC�ڲ�Ҳ�������ü�������,�����Ǻ��Լ���;����SmartPtr�ͷ�ָ���,GC���ٴ��ͷ�ͬһ��ָ��
//	- 1.1.0015.0120(2011-04-04)	+ ExGC֧�ֲ�ʹ��GC�����ڴ����,�����ṩͳһ���ڴ����ӿ�
//	- 1.1.0015.0120(2011-04-04)	+ CGCAllocT֧����ExMemͬ���ķ�ʽֱ�ӷ����ڴ�
//								= ����ExGCΪExMem,ͳһ�����ڴ����ӿڵĵ��÷�ʽ
//	- 1.1.0017.1640(2011-05-19)	+ ���CGCT::Regist�ӿ�,֧�ִ��ⲿֱ��ע��ָ��
//								= CGCT::CheckAlloc()��CGCT::CheckFree()��Ϊ��̬����
//								= CGCAllocT�ڲ�ʹ���ⲿGC�������ʱ��Ȼ��ָ�������ע��ָ��(ͳһ���е�ָ�����)
//	- 1.1.0018.1742(2012-01-14)	+ ���CGCT::ReAlloc�ӿ�
//	- 1.1.0019.1800(2012-01-16)	+ ֧��ͨ��EXP_MANAGED_ALLPTR�ر�CGCAllocT���йܷ�GC�ڴ����
//	- 1.1.0020.2357(2012-01-28)	= CGCT������ģ����������ڴ������,ͨ��ͨ���ڴ���䷽ʽ�й��ڴ�
//////////////////////////////////////////////////////////////////

#ifndef __GC_h__
#define __GC_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/PtrManager.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename ModelT = EXP_THREAD_MODEL>
struct _GCPolicyT
{
	typedef ModelT model_t;
	typedef typename model_t::_LockPolicy mutex_policy_t;
	typedef CLockT<mutex_policy_t> mutex_t;

	static const DWORD DEF_SIZE = 100;
	static DWORD Expan(DWORD nSize)
	{ return nSize ? (nSize << 1) : 1; }
};

//////////////////////////////////////////////////////////////////

template <typename PolicyT = _GCPolicyT<> >
class CGCT : INonCopyable
{
public:
	typedef typename PolicyT::model_t model_t;
	typedef typename PolicyT::mutex_t mutex_t;

protected:
	mutex_t	m_Mutex;

	void**	m_BlockArray;	// �ڴ������
	DWORD	m_nSize;
	DWORD	m_nIndx;

public:
	CGCT(DWORD nSize = PolicyT::DEF_SIZE)
		: m_BlockArray	(NULL)
		, m_nSize		(0)
		, m_nIndx		(0)
	{ SetGCSize(nSize); }
	~CGCT()
	{
		Clear();
		del(m_BlockArray);
	}

public:
	template <typename TypeT>
	EXP_INLINE static TypeT* CheckAlloc(DWORD nCount = 1)
	{
#ifdef	EXP_MANAGED_ALLPTR
		TypeT* ptr = dbnew(TypeT, nCount);
		EXP_PTR_MANAGER.Add(ptr);
		return ptr;
#else /*EXP_MANAGED_ALLPTR*/
		return dbnew(TypeT, nCount);
#endif/*EXP_MANAGED_ALLPTR*/
	}

	template <typename TypeT>
	EXP_INLINE static TypeT* CheckReAlloc(void* pPtr, DWORD nCount)
	{
#ifdef	EXP_MANAGED_ALLPTR
		TypeT* ptr_new = realc(pPtr, TypeT, nCount);
		if (ptr_new != pPtr) EXP_PTR_MANAGER.Add(ptr_new);
		return ptr_new;
#else /*EXP_MANAGED_ALLPTR*/
		return realc(pPtr, TypeT, nCount);
#endif/*EXP_MANAGED_ALLPTR*/
	}

	EXP_INLINE static void CheckFree(void* pPtr)
	{
#ifdef	EXP_MANAGED_ALLPTR
		EXP_PTR_MANAGER.Del(pPtr);
#else /*EXP_MANAGED_ALLPTR*/
		del(pPtr);
#endif/*EXP_MANAGED_ALLPTR*/
	}

public:
	DWORD GetGCSize()
	{
		ExLock(m_Mutex, TRUE, mutex_t);
		return m_nSize;
	}
	void SetGCSize(DWORD nSize = PolicyT::DEF_SIZE)
	{
		ExLock(m_Mutex, FALSE, mutex_t);
		if (GetGCSize() >= nSize) return;
		m_BlockArray = renew(m_BlockArray, void*, nSize);
		m_nSize = nSize;
	}

	// ע���ڴ�
	void* Regist(void* pPtr)
	{
		ExLock(m_Mutex, FALSE, mutex_t);
		if (GetGCSize() == m_nIndx)
			SetGCSize(PolicyT::Expan(GetGCSize()));
		m_BlockArray[m_nIndx] = pPtr;
		ExAssert(m_BlockArray[m_nIndx]);
		return m_BlockArray[m_nIndx++];
	}
	// �����ڴ�
	template <typename TypeT>
	TypeT* Alloc(DWORD nCount = 1)
	{
		if (nCount == 0) return NULL;
		ExLock(m_Mutex, FALSE, mutex_t);
		return (TypeT*)Regist(CheckAlloc<TypeT>(nCount));
	}
	void* Alloc(DWORD nSize)
	{
		if (nSize == 0) return NULL;
		return (void*)Alloc<BYTE>(nSize);
	}
	template <typename TypeT>
	TypeT* ReAlloc(void* pPtr, DWORD nCount)
	{
		if (nCount == 0) return NULL;
		ExLock(m_Mutex, FALSE, mutex_t);
		TypeT* ptr_new = CheckReAlloc<TypeT>(pPtr, nCount);
		if (ptr_new == pPtr)
			return ptr_new;
		else
			return (TypeT*)Regist(ptr_new);
	}
	void* ReAlloc(void* pPtr, DWORD nSize)
	{
		if (nSize == 0) return NULL;
		return (void*)ReAlloc<BYTE>(pPtr, nSize);
	}
	// ���GC
	void Clear()
	{
		ExLock(m_Mutex, FALSE, mutex_t);
		if (!m_nIndx) return;
		// ����ȳ�
		for(DWORD i = (m_nIndx - 1); i > 0; --i)
			CheckFree(m_BlockArray[i]);
		CheckFree(m_BlockArray[0]);
		m_nIndx = 0;
	}
};

typedef CGCT<> CGC;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GC_h__*/