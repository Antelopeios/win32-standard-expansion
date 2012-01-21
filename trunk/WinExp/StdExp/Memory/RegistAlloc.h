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
// RegistAlloc - �Ǽǲ��Զ�����/�������ڴ������
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-14
// Version:	1.0.0019.2102
//
// History:
//	- 1.0.0015.2359(2011-03-01)	# CRegistAllocT::Free()һ����ָ��ʱ�������ڴ��쳣
//	- 1.0.0017.2130(2011-03-03)	# CTraitsT::Construct()��CTraitsT::Destruct()��ӷ����쳣����
//								= CTraitsT<TypeT>��ΪCTraits,��ģ������Ա�����д���
//	- 1.0.0018.2110(2011-03-04)	= CTraits�Ľӿڵ���Ϊ�������������������ڴ���С��,�ڴ����й���/��������
//								= CRegistAllocT�ڷ����ڴ�ʱ�Զ���¼�ڴ����ĳ�ʼ���Ϳ�����,�����ݼ�¼��ֵ���ڴ�鹹��/����
//	- 1.0.0019.2102(2012-01-14)	+ ���CRegistAllocT::ReAlloc()
//								= ��_Regist�Ķ������CRegistAllocT�ⲿʵ��,�����ⲿ����_Regist�ڵĸ��ַ���
//								= ����ȡ�����ִ�RegistAlloc�ж�������
//////////////////////////////////////////////////////////////////

#ifndef __RegistAlloc_h__
#define __RegistAlloc_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/Traits.h"

EXP_BEG

//////////////////////////////////////////////////////////////////
// �����ڴ��ǼǵĽṹ����

struct _Regist
{
	typedef _Traits::traitor_t traitor_t;
	typedef void* (*maker_t)(void*, DWORD);

	traitor_t	destruct;
	DWORD		count;

	EXP_INLINE static void* RealPtr(void* pPtr)
	{
		return (((_Regist*)pPtr) - 1);
	}
	EXP_INLINE static void* PtrReal(void* pReal)
	{
		return (((_Regist*)pReal) + 1);
	}

	template <typename TypeT>
	static void* Maker(void* pPtr, DWORD size, _false_type)
	{
		DWORD count = size / sizeof(TypeT);
		_Regist* reg = (_Regist*)RealPtr(pPtr);
		reg->count = count;
		reg->destruct = (traitor_t)(_Traits::Destruct<TypeT>);
		return pPtr;
	}
	template <typename TypeT>
	static void* Maker(void* pPtr, DWORD size, _true_type)
	{
		return pPtr;
	}
	template <typename TypeT>
	static void* Maker(void* pPtr, DWORD size)
	{
		return Maker<TypeT>(pPtr, size, _TraitsT<TypeT>::is_POD_type());
	}
};

//////////////////////////////////////////////////////////////////
// �Ǽǲ��Զ�����/�������ڴ������

template <typename AllocT>
class CRegistAllocT
{
public:
	typedef _Regist::traitor_t traitor_t;
	typedef typename AllocT::alloc_t alloc_t;

public:
	enum { HeadSize = sizeof(_Regist) };

protected:
	alloc_t m_Alloc;

protected:
	EXP_INLINE void* RealPtr(void* pPtr)
	{
		return _Regist::RealPtr(pPtr);
	}
	EXP_INLINE void* PtrReal(void* pReal)
	{
		return _Regist::PtrReal(pReal);
	}

public:
	EXP_INLINE alloc_t& GetAlloc() { return m_Alloc; }

	template <typename TypeT>
	EXP_INLINE TypeT* Construct(void* pPtr)
	{
		ExAssert(pPtr);
		_Regist* real = (_Regist*)RealPtr(pPtr);
		real->destruct = (traitor_t)(_Traits::Destruct<TypeT>);
		return _Traits::Construct<TypeT>(pPtr, real->count);
	}
	EXP_INLINE void* Destruct(void* pPtr)
	{
		ExAssert(pPtr);
		_Regist* real = (_Regist*)RealPtr(pPtr);
		if (real->destruct)
		{
			real->destruct(pPtr, real->count);
			real->destruct = NULL;	// ����������һ��,��ֹ�߲����ʱ������ظ�����
		}							// ����Ҫ�ظ�����,���ֶ�����CTraits::Destruct<TypeT>
		return real;
	}

public:
	EXP_INLINE BOOL Valid(void* pPtr)
	{
		return m_Alloc.Valid(RealPtr(pPtr));
	}
	EXP_INLINE DWORD Size(void* pPtr)
	{
		return (m_Alloc.Size(RealPtr(pPtr)) - HeadSize);
	}

	EXP_INLINE void* Alloc(DWORD nSize)
	{
		if (nSize == 0) return NULL;
		ExAssert(nSize <= ((DWORD)~0 - HeadSize));
		_Regist* real = (_Regist*)m_Alloc.Alloc(HeadSize + nSize);
		real->count = nSize;
		real->destruct = NULL;
		return PtrReal(real);
	}
	template <typename TypeT>
	EXP_INLINE TypeT* Alloc(DWORD nCount, _true_type)
	{
		return (TypeT*)Alloc(sizeof(TypeT) * nCount);
	}
	template <typename TypeT>
	EXP_INLINE TypeT* Alloc(DWORD nCount, _false_type)
	{
		if (nCount == 0) return NULL;
		_Regist* real = (_Regist*)m_Alloc.Alloc(HeadSize + (sizeof(TypeT) * nCount));
		real->count = nCount;
		return Construct<TypeT>(PtrReal(real));
	}
	template <typename TypeT>
	EXP_INLINE TypeT* Alloc(DWORD nCount = 1)
	{
		return Alloc<TypeT>(nCount, _TraitsT<TypeT>::is_POD_type());
	}

	EXP_INLINE void* ReAlloc(void* pPtr, DWORD nSize, BOOL bFree = TRUE)
	{
		// ���ָ���Ƿ�Ϊ��
		if (pPtr == NULL)
			return Alloc(nSize);
		// ����С�Ƿ�Ϊ��
		if (nSize == 0)
		{
			if (bFree) Free(pPtr);
			return NULL;
		}
		// ���ָ�����д�С
		DWORD ptr_siz = Size(pPtr);
		// �����д�С������������, ��ֱ�ӷ���
		if (ptr_siz >= nSize)
			return pPtr;
		// �����д�С�޷���������, ������µĿռ䲢����ԭ������
		void* ptr_new = Alloc(nSize);
		memcpy(ptr_new, pPtr, ptr_siz);
		// �ͷžɿռ�, �����¿ռ�
		if (bFree) m_Alloc.Free(RealPtr(pPtr));
		return ptr_new;
	}
	template <typename TypeT>
	EXP_INLINE TypeT* ReAlloc(void* pPtr, DWORD nCount, _true_type, BOOL bFree = TRUE)
	{
		return (TypeT*)ReAlloc(pPtr, sizeof(TypeT) * nCount, bFree);
	}
	template <typename TypeT>
	EXP_INLINE TypeT* ReAlloc(void* pPtr, DWORD nCount, _false_type, BOOL bFree = TRUE)
	{
		// ���ָ���Ƿ�Ϊ��
		if (pPtr == NULL)
			return Alloc<TypeT>(nCount);
		// ����С�Ƿ�Ϊ��
		if (nCount == 0)
		{
			if (bFree) Free(pPtr);
			return NULL;
		}
		// ���ָ�����ж�������
		_Regist* real = (_Regist*)RealPtr(pPtr);
		// ����������������������, ��ֱ�ӷ���
		if (real->count == nCount)
			return (TypeT*)pPtr;
		// ����������������������, �����ٶ������, ��ֱ�ӷ���
		if (real->count > nCount)
		{
			if (real->destruct)
				real->destruct(((TypeT*)pPtr) + nCount, real->count - nCount);
			real->count = nCount;
			return (TypeT*)pPtr;
		}
		// ����������С����������, ����ָ�����д�С
		DWORD ptr_siz = Size(pPtr);
		DWORD size = sizeof(TypeT) * nCount;
		// �����д�С������������, �����¶���, ��ֱ�ӷ���
		if (ptr_siz >= size)
		{
			_Traits::Construct<TypeT>(((TypeT*)pPtr) + real->count, nCount - real->count);
			real->count = nCount;
			return (TypeT*)pPtr;
		}
		// �����д�С�޷���������, ������µĿռ�, ����ԭ�ж�������, �������¶���
		_Regist* real_new = (_Regist*)m_Alloc.Alloc(HeadSize + size);
		real_new->count = nCount;
		real_new->destruct = (traitor_t)(_Traits::Destruct<TypeT>);
		void* ptr_new = PtrReal(real_new);
		memcpy(ptr_new, pPtr, sizeof(TypeT) * real->count);
		_Traits::Construct<TypeT>(((TypeT*)ptr_new) + real->count, nCount - real->count);
		// �ͷžɿռ�, �����¿ռ�
		if (bFree) m_Alloc.Free(RealPtr(pPtr));
		return (TypeT*)ptr_new;
	}
	template <typename TypeT>
	EXP_INLINE TypeT* ReAlloc(void* pPtr, DWORD nCount, BOOL bFree = TRUE)
	{
		return ReAlloc<TypeT>(pPtr, nCount, _TraitsT<TypeT>::is_POD_type(), bFree);
	}

	EXP_INLINE void Free(void* pPtr)
	{
		if (!pPtr) return;
		m_Alloc.Free(Destruct(pPtr));
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__RegistAlloc_h__*/