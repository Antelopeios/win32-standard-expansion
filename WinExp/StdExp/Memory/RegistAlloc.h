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
// Date:	2012-03-05
// Version:	1.0.0021.2317
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
//	- 1.0.0020.0451(2012-01-29)	= CRegistAllocTʹ�þ�̬�ڴ��������Ϊ����,Ĭ��ʹ��EXP_MEMPOOL_ALLOC
//								= CRegistAllocT�����дΪ��̬��
//								= ����CRegistAllocΪEXP_MEMORY_ALLOC,����ExMem����ΪCRegistAlloc�ı��
//	- 1.0.0021.2317(2012-03-05)	= ���ձ�׼Ҫ��,��Alloc�Ĵ�СΪ0ʱ������С�ڴ��
//////////////////////////////////////////////////////////////////

#ifndef __RegistAlloc_h__
#define __RegistAlloc_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Memory/MemPool.h"
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
	EXP_INLINE static void* Make(void* pReal, DWORD nCount, _false_type)
	{
		_Regist* reg = (_Regist*)pReal;
		reg->count = nCount;
		reg->destruct = (traitor_t)(_Traits::Destruct<TypeT>);
		return pReal;
	}
	template <typename TypeT>
	EXP_INLINE static void* Make(void* pReal, DWORD nCount, _true_type)
	{
		_Regist* reg = (_Regist*)pReal;
		reg->count = nCount;
		reg->destruct = NULL;
		return pReal;
	}
	template <typename TypeT>
	EXP_INLINE static void* Make(void* pReal, DWORD nCount)
	{
		return Make<TypeT>(pReal, nCount, _TraitsT<TypeT>::is_POD_type());
	}
};

//////////////////////////////////////////////////////////////////
// �Ǽǲ��Զ�����/�������ڴ������

template <typename AllocT = EXP_MEMPOOL_ALLOC>
class CRegistAllocT
{
public:
	typedef _Regist::traitor_t traitor_t;
	typedef typename AllocT::alloc_t alloc_t;

public:
	template <typename TypeT>
	EXP_INLINE static TypeT* Construct(_Regist* pReal)
	{
		return _Traits::Construct<TypeT>(_Regist::PtrReal(pReal), pReal->count);
	}
	EXP_INLINE static void* Destruct(_Regist* pReal)
	{
		if (pReal->destruct)
		{
			pReal->destruct(_Regist::PtrReal(pReal), pReal->count);
			pReal->destruct = NULL;	// ����������һ��,��ֹ�߲����ʱ������ظ�����
		}							// ����Ҫ�ظ�����,���ֶ�����CTraits::Destruct<TypeT>
		return pReal;
	}

public:
	EXP_INLINE static BOOL Valid(void* pPtr)
	{
		return alloc_t::Valid(_Regist::RealPtr(pPtr));
	}
	EXP_INLINE static DWORD Size(void* pPtr)
	{
		return alloc_t::Size(_Regist::RealPtr(pPtr)) - sizeof(_Regist);
	}

	EXP_INLINE static void* Alloc(DWORD nSize, LPCSTR sFile = NULL, int nLine = 0)
	{
		if (nSize == 0) nSize = 1;
		_Regist* real = (_Regist*)alloc_t::Alloc(sizeof(_Regist) + nSize);
		return _Regist::PtrReal(_Regist::Make<BYTE>(alloc_t::SetAlloc(real, sFile, nLine), nSize));
	}
	template <typename TypeT>
	EXP_INLINE static TypeT* Alloc(DWORD nCount = 1, LPCSTR sFile = NULL, int nLine = 0)
	{
		if (nCount == 0) return NULL;
		_Regist* real = (_Regist*)alloc_t::Alloc(sizeof(_Regist) + (sizeof(TypeT) * nCount));
		return Construct<TypeT>((_Regist*)_Regist::Make<TypeT>(alloc_t::SetAlloc(real, sFile, nLine), nCount));
	}

	EXP_INLINE static void* SetAlloc(void* p, LPCSTR sFile, int nLine)
	{
		alloc_t::SetAlloc(_Regist::RealPtr(p), sFile, nLine);
		return p;
	}

	EXP_INLINE static void* ReAlloc(void* pPtr, DWORD nSize, BOOL bFree = TRUE, LPCSTR sFile = NULL, int nLine = 0)
	{
		// ���ָ���Ƿ�Ϊ��
		if (pPtr == NULL)
			return Alloc(nSize, sFile, nLine);
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
		void* ptr_new = Alloc(nSize, sFile, nLine);
		memcpy(ptr_new, pPtr, ptr_siz);
		// �ͷžɿռ�, �����¿ռ�
		if (bFree) FreeNoDest(pPtr);
		return ptr_new;
	}
	template <typename TypeT>
	EXP_INLINE static TypeT* ReAlloc(void* pPtr, DWORD nCount, _true_type, 
									 BOOL bFree = TRUE, LPCSTR sFile = NULL, int nLine = 0)
	{
		return (TypeT*)ReAlloc(pPtr, sizeof(TypeT) * nCount, bFree, sFile, nLine);
	}
	template <typename TypeT>
	EXP_INLINE static TypeT* ReAlloc(void* pPtr, DWORD nCount, _false_type, 
									 BOOL bFree = TRUE, LPCSTR sFile = NULL, int nLine = 0)
	{
		// ���ָ���Ƿ�Ϊ��
		if (pPtr == NULL)
			return Alloc<TypeT>(nCount, sFile, nLine);
		// ����С�Ƿ�Ϊ��
		if (nCount == 0)
		{
			if (bFree) Free(pPtr);
			return NULL;
		}
		// ���ָ�����ж�������
		_Regist* real = (_Regist*)_Regist::RealPtr(pPtr);
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
		_Regist* real_new = (_Regist*)alloc_t::Alloc(sizeof(_Regist) + size);
		void* ptr_new = _Regist::PtrReal(_Regist::Make<TypeT>(alloc_t::SetAlloc(real_new, sFile, nLine), nCount));
		memcpy(ptr_new, pPtr, sizeof(TypeT) * real->count);
		_Traits::Construct<TypeT>(((TypeT*)ptr_new) + real->count, nCount - real->count);
		// �ͷžɿռ�, �����¿ռ�
		if (bFree) FreeNoDest(pPtr);
		return (TypeT*)ptr_new;
	}
	template <typename TypeT>
	EXP_INLINE static TypeT* ReAlloc(void* pPtr, DWORD nCount, BOOL bFree = TRUE, LPCSTR sFile = NULL, int nLine = 0)
	{
		return ReAlloc<TypeT>(pPtr, nCount, _TraitsT<TypeT>::is_POD_type(), bFree, sFile, nLine);
	}

	EXP_INLINE static void Free(void* pPtr)
	{
		if (!pPtr) return;
		alloc_t::Free(Destruct((_Regist*)_Regist::RealPtr(pPtr)));
	}
	EXP_INLINE static void FreeNoDest(void* pPtr)
	{
		if (!pPtr) return;
		alloc_t::Free(_Regist::RealPtr(pPtr));
	}
};

typedef CRegistAllocT<> CRegistAlloc;
typedef CRegistAlloc ExMem;

#ifndef EXP_MEMORY_ALLOC
#define EXP_MEMORY_ALLOC CRegistAlloc
#endif/*EXP_MEMORY_ALLOC*/

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__RegistAlloc_h__*/