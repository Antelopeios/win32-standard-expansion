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
// GC - ����������
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-19
// Version:	1.1.0017.1640
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
//////////////////////////////////////////////////////////////////

#ifndef __GC_h__
#define __GC_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/PtrManager.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename AllocT = EXP_MEMORY_ALLOC, typename ModelT = EXP_THREAD_MODEL>
struct _GCPolicyT;

template <typename PolicyT = _GCPolicyT<> >
class CGCT : INonCopyable
{
public:
	typedef typename PolicyT::alloc_t alloc_t;
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
		alloc_t::Free(m_BlockArray);
	}

public:
	template <typename TypeT>
	EXP_INLINE static TypeT* CheckAlloc(DWORD nCount = 1)
	{
		TypeT* ptr = alloc_t::Alloc<TypeT>(nCount);
		EXP_PTR_MANAGER.Add<alloc_t, model_t>(ptr);
		return ptr;
	}
	EXP_INLINE static void CheckFree(void* pPtr)
	{
		EXP_PTR_MANAGER.Del(pPtr);
	}

public:
	template <typename TypeT>
	EXP_INLINE TypeT* Construct(void* pPtr)
	{
		ExLock(m_Mutex, false, mutex_t);
		return alloc_t::Construct<TypeT>(pPtr);
	}
	EXP_INLINE void* Destruct(void* pPtr)
	{
		ExLock(m_Mutex, false, mutex_t);
		return alloc_t::Destruct(pPtr);
	}

public:
	DWORD GetGCSize()
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_nSize;
	}
	void SetGCSize(DWORD nSize = PolicyT::DEF_SIZE)
	{
		ExLock(m_Mutex, false, mutex_t);
		if( GetGCSize() >= nSize ) return;
		void** pArray = alloc_t::Alloc<void*>(nSize);
		if (m_BlockArray)
		{
			if (m_nIndx)
				memcpy(pArray, m_BlockArray, sizeof(void*) * m_nIndx);
			alloc_t::Free(m_BlockArray);
		}
		m_BlockArray = pArray;
		m_nSize = nSize;
	}

	// �ڴ�Ч��
	bool Valid(void* pPtr)
	{
		ExLock(m_Mutex, true, mutex_t);
		return alloc_t::Valid(pPtr);
	}
	// �ڴ��С
	DWORD Size(void* pPtr)
	{
		ExLock(m_Mutex, true, mutex_t);
		return alloc_t::Size(pPtr);
	}
	// ע���ڴ�
	void* Regist(void* pPtr)
	{
		ExLock(m_Mutex, false, mutex_t);
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
		ExLock(m_Mutex, false, mutex_t);
		return (TypeT*)Regist(CheckAlloc<TypeT>(nCount));
	}
	void* Alloc(DWORD nSize)
	{
		if (nSize == 0) return NULL;
		return (void*)Alloc<BYTE>(nSize);
	}
	// �����ڴ�
	void Free(void* pPtr)
	{
		if (!pPtr) return;
		ExLock(m_Mutex, false, mutex_t);
		DWORD i = 0;
		for(; i < m_nIndx; ++i)
		{
			if (pPtr = m_BlockArray[i])
			{
				CheckFree(pPtr);
				memmove((void*)(m_BlockArray + i), (void*)(m_BlockArray + i + 1), sizeof(void*) * (m_nIndx - i - 1));
				break;
			}
		}
		if (i < m_nIndx) --m_nIndx;
	}
	// ���GC
	void Clear()
	{
		ExLock(m_Mutex, false, mutex_t);
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

template <typename GCT = CGC>
class CGCAllocT
{
public:
	typedef GCT gc_alloc_t;
	typedef typename gc_alloc_t::alloc_t alloc_t;

public:
	// alloc_t
	EXP_INLINE static bool Valid(void* pPtr)
	{ return alloc_t::Valid(pPtr); }
	EXP_INLINE static DWORD Size(void* pPtr)
	{ return alloc_t::Size(pPtr); }
	template <typename TypeT>
	EXP_INLINE static TypeT* Alloc(DWORD nCount = 1)
	{ return gc_alloc_t::CheckAlloc<TypeT>(nCount); }
	EXP_INLINE static void* Alloc(DWORD nSize)
	{ return gc_alloc_t::CheckAlloc<BYTE>(nSize); }
	EXP_INLINE static void Free(void* pPtr)
	{ gc_alloc_t::CheckFree(pPtr); }
	// gc_alloc_t
	EXP_INLINE static bool Valid(gc_alloc_t* alloc, void* pPtr)
	{ return alloc ? alloc->Valid(pPtr) : Valid(pPtr); }
	EXP_INLINE static DWORD Size(gc_alloc_t* alloc, void* pPtr)
	{ return alloc ? alloc->Size(pPtr) : Size(pPtr); }
	template <typename TypeT>
	EXP_INLINE static TypeT* Alloc(gc_alloc_t* alloc, DWORD nCount = 1)
	{ return alloc ? alloc->Alloc<TypeT>(nCount) : Alloc<TypeT>(nCount); }
	EXP_INLINE static void* Alloc(gc_alloc_t* alloc, DWORD nSize)
	{ return alloc ? alloc->Alloc(nSize) : Alloc(nSize); }
	EXP_INLINE static void Free(gc_alloc_t* alloc, void* pPtr)
	{ if (alloc) alloc->Free(pPtr); else Free(pPtr); }
};

typedef CGCAllocT<> CGCAlloc;
typedef CGCAlloc ExMem;

//////////////////////////////////////////////////////////////////

template <typename AllocT/* = EXP_MEMORY_ALLOC*/, typename ModelT/* = EXP_THREAD_MODEL*/>
struct _GCPolicyT
{
	typedef AllocT alloc_t;
	typedef ModelT model_t;
	typedef typename model_t::_LockPolicy mutex_policy_t;
	typedef CLockT<mutex_policy_t> mutex_t;

	static const DWORD DEF_SIZE = 100;
	static DWORD Expan(DWORD nSize)
	{ return nSize ? (nSize << 1) : 1; }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GC_h__*/