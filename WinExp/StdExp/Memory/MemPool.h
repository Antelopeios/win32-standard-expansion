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
// MemPool - �ڴ��
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-04-03
// Version:	1.3.0025.2220
//
// History:
//	- 1.2.0016.2345(2011-03-01)	^ �Ľ�MemPool���ڲ�ʵ�ַ�ʽ,���߼�,�Ż��㷨
//								+ MemPool֧��Clearʱ���м򵥵��ڴ�й©���
//	- 1.2.0018.1520(2011-03-02)	= ��static_cast��ָ��ת����ΪC���Ե�ǿ��ת����ʽ
//								+ ���һЩ�ؼ�λ�õ�inline����
//	- 1.3.0020.2110(2011-03-04)	^ ʹ��ObjPool�����ع��ڴ��,�������ڴ����������Ч��
//	- 1.3.0021.0233(2011-03-06)	# ����CMemPoolT::CLagPoolT::Size()���һ������
//	- 1.3.0022.1540(2011-03-07) ^ CMemPoolT::Alloc�ڲ����ö��ַ����ж�Ӧ��ObjPool
//	- 1.3.0023.1203(2011-03-08) ^ CMemPoolT::pool_policy_t�����Զ�������
//								^ CMemPoolT�ڲ��ṹ�����1�ֽ��ڴ�����Ż��ڴ�ռ��
//	- 1.3.0024.1014(2011-04-01) # ����CMemPoolT::pool_policy_t���ϴ��Ż����µı������
//	- 1.3.0025.2220(2011-04-03) # ����CMemPoolT::CLagPoolT::Alloc��δ��������ڴ�����ݳ�ʼ��Ϊ��,�����ڴ����������쳣
//////////////////////////////////////////////////////////////////

#ifndef __MemPool_h__
#define __MemPool_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Debugging/Trace.h"
#include "Memory/ObjPool.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename AllocT = EXP_MEMHEAP_ALLOC, typename ModelT = EXP_THREAD_MODEL>
struct _MemPoolPolicy;

template <typename PolicyT = _MemPoolPolicy<> >
class CMemPoolT : INonCopyable
{
public:
	typedef typename PolicyT::alloc_t alloc_t;
	typedef typename PolicyT::model_t model_t;
	typedef typename PolicyT::mutex_t mutex_t;
	typedef typename PolicyT::byte byte;

protected:
#pragma pack(1)
	// �ڴ���ǽ��
	struct block_t
	{
		IObjPool*	pPool;	// �����ָ��
		block_t*	pPrev;	// ��һ�����
		block_t*	pNext;	// ��һ�����

		block_t()
			: pPool(NULL)
			, pPrev(NULL)
			, pNext(NULL)
		{}
		~block_t()
		{
			pPrev = pNext = NULL;
			pPool = NULL;
		}
	};
#pragma pack()

	// �ڴ��������
	typedef class blk_list_t
	{
	protected:
		block_t* m_pList;	// ����ͷָ��

	public:
		blk_list_t()
			: m_pList(NULL)
		{}
		~blk_list_t()
		{ /*���������κ�������*/ }

	public:
		EXP_INLINE block_t* Push(block_t* pItem, block_t* pLast = NULL)
		{	// Ĭ�ϴ�β��������
			if( !pItem ) return NULL;
			if( (pItem->pPrev && pItem->pPrev->pNext == pItem) && 
				(pItem->pNext && pItem->pNext->pPrev == pItem) ) return NULL;
			if( pLast == pItem ) pLast = NULL;
			// ��������
			if( m_pList )
			{
				if( pLast == m_pList )
					m_pList = pItem;
				else
				if(!pLast )
					pLast = m_pList;
				// ������
				pItem->pNext = pLast;
				pItem->pPrev = pLast->pPrev;
				pLast->pPrev = pItem;
				pItem->pPrev->pNext = pItem;
			} else
				m_pList = pItem->pPrev = pItem->pNext = pItem;
			return pItem;
		}
		EXP_INLINE block_t* Pop(block_t* pItem = NULL)
		{	// Ĭ�ϴ�ͷ���������
			if(!pItem ) pItem = m_pList;
			if( pItem )
			{	// �������
				if(!pItem->pPrev && !pItem->pNext) return NULL;
				if( m_pList == pItem ) m_pList = m_pList->pNext;
				if( pItem->pPrev ) pItem->pPrev->pNext = pItem->pNext;
				if( pItem->pNext ) pItem->pNext->pPrev = pItem->pPrev;
				pItem->pPrev = NULL;
				pItem->pNext = NULL;
				if( m_pList == pItem ) m_pList = NULL;
			}
			return pItem;
		}

		EXP_INLINE void Clear()
		{	// ֱ���ÿ�,�����κ�������
			m_pList = NULL;
		}
		EXP_INLINE BOOL Empty()
		{ return (m_pList == NULL); }

		EXP_INLINE block_t* Head()
		{ return m_pList; }
		EXP_INLINE block_t* Tail()
		{ return m_pList; }
		EXP_INLINE block_t* Last()
		{ return m_pList ? m_pList->pPrev : NULL; }
	};

	// �ڴ������
#pragma pack(1)
	template <DWORD SizeT>
	struct _TypeT
	{
		block_t Block;
		byte	Buff[SizeT];
	};
#pragma pack()

	// ����ز���
	template <DWORD SizeT>
	struct pool_policy_t : public PolicyT::pool_policy_t
	{ static const DWORD MAX_SIZE = ((((DWORD)~0) / sizeof(_TypeT<SizeT>)) >> 5); };

	// �����
	template <typename PolicyT>
	class CLagPoolT : INonCopyable, public IObjPool
	{
	public:
		typedef typename PolicyT::alloc_t alloc_t;

	protected:
		alloc_t m_Alloc;

	public:
		DWORD GetObjSize()			{ return 0; }
		BOOL Valid(void* pPtr)		{ return m_Alloc.Valid(pPtr); }
		DWORD Size(void* pPtr)		{ return m_Alloc.Size(pPtr); }
		void* Alloc(DWORD nSize)	{ return ZeroMemory(m_Alloc.Alloc(nSize), nSize); }
		void Free(void* pPtr)		{ m_Alloc.Free(pPtr); }
		void Clear()				{}
	};
	typedef CLagPoolT<typename PolicyT::pool_policy_t>		pool_00_t;
	typedef CObjPoolT<_TypeT<1>, pool_policy_t<1> >			pool_01_t;
	typedef CObjPoolT<_TypeT<2>, pool_policy_t<2> >			pool_02_t;
	typedef CObjPoolT<_TypeT<4>, pool_policy_t<4> >			pool_03_t;
	typedef CObjPoolT<_TypeT<8>, pool_policy_t<8> >			pool_04_t;
	typedef CObjPoolT<_TypeT<16>, pool_policy_t<16> >		pool_05_t;
	typedef CObjPoolT<_TypeT<32>, pool_policy_t<32> >		pool_06_t;
	typedef CObjPoolT<_TypeT<64>, pool_policy_t<64> >		pool_07_t;
	typedef CObjPoolT<_TypeT<128>, pool_policy_t<128> >		pool_08_t;
	typedef CObjPoolT<_TypeT<256>, pool_policy_t<256> >		pool_09_t;
	typedef CObjPoolT<_TypeT<512>, pool_policy_t<512> >		pool_10_t;
	typedef CObjPoolT<_TypeT<1024>, pool_policy_t<1024> >	pool_11_t;
	typedef CObjPoolT<_TypeT<2048>, pool_policy_t<2048> >	pool_12_t;
	typedef CObjPoolT<_TypeT<4096>, pool_policy_t<4096> >	pool_13_t;
	typedef CObjPoolT<_TypeT<8192>, pool_policy_t<8192> >	pool_14_t;
	typedef CObjPoolT<_TypeT<16384>, pool_policy_t<16384> >	pool_15_t;
	typedef CObjPoolT<_TypeT<32768>, pool_policy_t<32768> >	pool_16_t;

	// ���ת������
	EXP_INLINE block_t* BlockPtr(void* pPtr) { return pPtr ? (block_t*)(((BYTE*)pPtr) - sizeof(block_t)) : NULL; }
	EXP_INLINE void* PtrBlock(block_t* pPtr) { return pPtr ? (void*)(pPtr + 1) : NULL; }

protected:
	alloc_t		m_Alloc;
	mutex_t		m_Mutex;

	IObjPool*	m_PoolList[17];
	blk_list_t	m_UsedList;

public:
	CMemPoolT()
	{
		int i = 0;
		m_PoolList[i]	= (IObjPool*)CTraits::Construct<pool_00_t>(m_Alloc.Alloc(sizeof(pool_00_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_01_t>(m_Alloc.Alloc(sizeof(pool_01_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_02_t>(m_Alloc.Alloc(sizeof(pool_02_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_03_t>(m_Alloc.Alloc(sizeof(pool_03_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_04_t>(m_Alloc.Alloc(sizeof(pool_04_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_05_t>(m_Alloc.Alloc(sizeof(pool_05_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_06_t>(m_Alloc.Alloc(sizeof(pool_06_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_07_t>(m_Alloc.Alloc(sizeof(pool_07_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_08_t>(m_Alloc.Alloc(sizeof(pool_08_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_09_t>(m_Alloc.Alloc(sizeof(pool_09_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_10_t>(m_Alloc.Alloc(sizeof(pool_10_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_11_t>(m_Alloc.Alloc(sizeof(pool_11_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_12_t>(m_Alloc.Alloc(sizeof(pool_12_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_13_t>(m_Alloc.Alloc(sizeof(pool_13_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_14_t>(m_Alloc.Alloc(sizeof(pool_14_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_15_t>(m_Alloc.Alloc(sizeof(pool_15_t)));
		m_PoolList[++i]	= (IObjPool*)CTraits::Construct<pool_16_t>(m_Alloc.Alloc(sizeof(pool_16_t)));
	}
	~CMemPoolT()
	{
		Clear(PolicyT::DUMP_MEM_LEAKS);
		int i = 0;
		m_Alloc.Free(CTraits::Destruct<pool_00_t>(m_PoolList[i]));
		m_Alloc.Free(CTraits::Destruct<pool_01_t>(m_PoolList[++i]));
		m_Alloc.Free(CTraits::Destruct<pool_02_t>(m_PoolList[++i]));
		m_Alloc.Free(CTraits::Destruct<pool_03_t>(m_PoolList[++i]));
		m_Alloc.Free(CTraits::Destruct<pool_04_t>(m_PoolList[++i]));
		m_Alloc.Free(CTraits::Destruct<pool_05_t>(m_PoolList[++i]));
		m_Alloc.Free(CTraits::Destruct<pool_06_t>(m_PoolList[++i]));
		m_Alloc.Free(CTraits::Destruct<pool_07_t>(m_PoolList[++i]));
		m_Alloc.Free(CTraits::Destruct<pool_08_t>(m_PoolList[++i]));
		m_Alloc.Free(CTraits::Destruct<pool_09_t>(m_PoolList[++i]));
		m_Alloc.Free(CTraits::Destruct<pool_10_t>(m_PoolList[++i]));
		m_Alloc.Free(CTraits::Destruct<pool_11_t>(m_PoolList[++i]));
		m_Alloc.Free(CTraits::Destruct<pool_12_t>(m_PoolList[++i]));
		m_Alloc.Free(CTraits::Destruct<pool_13_t>(m_PoolList[++i]));
		m_Alloc.Free(CTraits::Destruct<pool_14_t>(m_PoolList[++i]));
		m_Alloc.Free(CTraits::Destruct<pool_15_t>(m_PoolList[++i]));
		m_Alloc.Free(CTraits::Destruct<pool_16_t>(m_PoolList[++i]));
	}

public:
	// �ڴ�Ч��
	BOOL Valid(void* pPtr)
	{
		ExLock(m_Mutex, TRUE, mutex_t);
		block_t* block = BlockPtr(pPtr);
		if (!block) return FALSE;
		IObjPool* pool = block->pPool;
		if (!pool) return FALSE;
		return pool->Valid(block);
	}
	// �ڴ��С
	DWORD Size(void* pPtr)
	{
		ExLock(m_Mutex, TRUE, mutex_t);
		block_t* block = BlockPtr(pPtr);
		if (!block) return 0;
		IObjPool* pool = block->pPool;
		if (!pool) return 0;
		return pool->Size(block);
	}
	// �����ڴ�
	void* Alloc(DWORD nSize)
	{
		if (nSize == 0) return NULL;
		ExLock(m_Mutex, FALSE, mutex_t);
		// ��λObjPool
		nSize += sizeof(block_t);
		block_t* block = NULL;
		int mid = 0, left = 1, right = 16;
		while (left <= right)
		{
			mid = (left + right) / 2;
			DWORD size = m_PoolList[mid]->GetObjSize();
			if (size == nSize)
				break;
			else
			if (size < nSize)
			{
				if (mid >= 16)
				{
					mid = 0;
					break;
				} else
				if (m_PoolList[++mid]->GetObjSize() >= nSize)
					break;
				else
					left = mid;
			} else
			{
				size = m_PoolList[--mid]->GetObjSize();
				if (size == nSize)
					break;
				else
				if (size < nSize)
				{
					++mid;
					break;
				} else
					right = mid;
			}
		}
		block = (block_t*)m_PoolList[mid]->Alloc(nSize);
		// ���ر���ڴ��
		ExAssert(block);
		block->pPool = m_PoolList[mid];
		m_UsedList.Push(block);
		return PtrBlock(block);
	}
	// �����ڴ�
	void Free(void* pPtr)
	{
		if (!pPtr) return;
		ExLock(m_Mutex, FALSE, mutex_t);
		// ��ñ�ǿ�
		block_t* block = BlockPtr(pPtr);
		// ������ǿ�
		m_UsedList.Pop(block);
		// ���ٱ�ǿ�
		IObjPool* pool = block->pPool;
		ExAssert(pool);
		if (!pool) return;
		pool->Free(block);
	}

#ifdef	_DEBUG
#ifndef	EXP_DUMPING_MEMLEAKS
#define	EXP_DUMPING_MEMLEAKS
#endif/*EXP_DUMPING_MEMLEAKS*/
#endif/*_DEBUG*/

	// ����ڴ��
	void Clear(BOOL bDump = TRUE)
	{
		ExLock(m_Mutex, FALSE, mutex_t);
		// �����ڴ��¼����
#ifdef	EXP_DUMPING_MEMLEAKS
		DWORD dump_counter = 0;
		if (bDump)
			ExDPrintf(_T("\nDumping memory leaks...\n"));
#endif/*EXP_DUMPING_MEMLEAKS*/
		if( !m_UsedList.Empty() )
		{
			block_t* item = m_UsedList.Head();
			do
			{
				block_t* temp = item->pNext;
				IObjPool* pool = item->pPool;
				ExAssert(pool);
#ifdef	EXP_DUMPING_MEMLEAKS
				if (bDump && ++dump_counter)
				{
					BYTE* ptr = (BYTE*)PtrBlock(item);
					ExDPrintf(_T("\nMemory leak at 0x%08X, %d bytes long.\nData:"), ptr, pool->Size(item));
					ExDPrintf(
						_T("%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n"), 
						ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7], 
						ptr[8], ptr[9], ptr[10], ptr[11], ptr[12], ptr[13], ptr[14], ptr[15]);
				}
#endif/*EXP_DUMPING_MEMLEAKS*/
				m_UsedList.Pop(item);
				pool->Free(item);
				item = temp;
			} while( item != m_UsedList.Tail() );
		}
#ifdef	EXP_DUMPING_MEMLEAKS
		if (bDump)
		{
			if (dump_counter == 0)
				ExDPrintf(_T("No memory leak has been found!\n"));
			else
			if (dump_counter == 1)
				ExDPrintf(_T("\n1 memory leak has been found!\n"));
			else
				ExDPrintf(_T("\n%d memory leaks have been found!\n"), dump_counter);
			ExDPrintf(_T("Complete memory leak detection.\n\n"));
		}
#endif/*EXP_DUMPING_MEMLEAKS*/
		// ��������
		for(int i = 0; i < 17; ++i)
			m_PoolList[i]->Clear();
	}
};

typedef CMemPoolT<> CMemPool;

//////////////////////////////////////////////////////////////////

class CMemPoolAlloc
{
public:
	typedef CMemPool alloc_t;
};

typedef CRegistAllocT<CMemPoolAlloc> _MemPool;

//////////////////////////////////////////////////////////////////

template <typename AllocT/* = EXP_MEMHEAP_ALLOC*/, typename ModelT/* = EXP_THREAD_MODEL*/>
struct _MemPoolPolicy
{
	typedef AllocT alloc_t;
	typedef ModelT model_t;
	typedef typename model_t::_LockPolicy mutex_policy_t;
	typedef CLockT<mutex_policy_t> mutex_t;

	typedef _ObjPoolPolicyT<alloc_t, _SingleModel> pool_policy_t;
	typedef DWORD byte;

	static const BOOL DUMP_MEM_LEAKS = TRUE;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__MemPool_h__*/