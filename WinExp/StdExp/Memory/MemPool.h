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
// MemPool - �ڴ��
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-02-03
// Version:	1.3.0030.1800
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
//	- 1.3.0026.1445(2012-01-16) + ֧��ͨ��EXP_UNDUMPED_NAMESPACE��Debugģʽ�¹ر��ڴ�й©���
//	- 1.3.0027.1712(2012-01-20) + �����ڴ�й©��⼰�����
//								# ����CMemPoolT::Size()���ش�С���������
//	- 1.3.0028.0218(2012-01-27) ^ Ԥ�ȱ���CMemPoolT��ÿ��ObjPool�����ȴ�С,����Ҫʱֱ�ӻ�ȡ
//	- 1.3.0029.0451(2012-01-29) ^ ����ͳһ���͵Ķ������ΪCMemPoolT�ڲ��ּ��ڴ����,��CMemPoolT���ڲ�ʵ��
//								+ ͨ��CMemPoolAllocTʵ��һ����̬�ڴ�ط�����,������EXP_MEMPOOL_ALLOC��Ϊ����
//	- 1.3.0030.1800(2012-02-03) + ���EXP_BREAK_ALLOC��,֧���ⲿ����ʱ�Զ���ĳ�η���ʱ�ж�
//////////////////////////////////////////////////////////////////

#ifndef __MemPool_h__
#define __MemPool_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Debugging/Trace.h"
#include "Memory/BlockPool.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

// �ڴ�й©���궨��
#ifndef EXP_UNDUMPED_NAMESPACE
#ifdef	_DEBUG
#ifndef	EXP_DUMPING_MEMLEAKS
#define	EXP_DUMPING_MEMLEAKS
#endif/*EXP_DUMPING_MEMLEAKS*/
#endif/*_DEBUG*/
#endif/*EXP_UNDUMPED_NAMESPACE*/

#ifdef	EXP_DUMPING_MEMLEAKS
#ifndef	EXP_BREAK_ALLOC
#define	EXP_BREAK_ALLOC	-1
#endif/*EXP_BREAK_ALLOC*/
#endif/*EXP_DUMPING_MEMLEAKS*/

//////////////////////////////////////////////////////////////////

template <typename AllocT = EXP_BLKPOOL_ALLOC, typename ModelT = EXP_THREAD_MODEL>
struct _MemPoolPolicy
{
	typedef AllocT alloc_t;
	typedef ModelT model_t;
	typedef typename model_t::_LockPolicy mutex_policy_t;
	typedef CLockT<mutex_policy_t> mutex_t;

	typedef long byte;

	static const BOOL DUMP_MEM_LEAKS = TRUE;
};

//////////////////////////////////////////////////////////////////

template <typename PolicyT = _MemPoolPolicy<> >
class CMemPoolT : INonCopyable
{
public:
	typedef typename PolicyT::alloc_t alloc_t;
	typedef typename PolicyT::model_t model_t;
	typedef typename PolicyT::mutex_t mutex_t;
	typedef typename PolicyT::byte byte;

protected:
#ifdef	EXP_DUMPING_MEMLEAKS
	volatile LONG m_nTime;
#endif/*EXP_DUMPING_MEMLEAKS*/

public:
#pragma pack(1)
	// �ڴ���ǽ��
	struct block_t
	{
		alloc_t*	pPool;	// �����ָ��
#ifdef	EXP_DUMPING_MEMLEAKS
		LPCSTR		sFile;
		int			nLine;
		LONG		nTime;
#endif/*EXP_DUMPING_MEMLEAKS*/
		block_t*	pPrev;	// ��һ�����
		block_t*	pNext;	// ��һ�����

		block_t()
			: pPool(NULL)
#ifdef	EXP_DUMPING_MEMLEAKS
			, sFile(NULL)
			, nLine(0)
			, nTime(0)
#endif/*EXP_DUMPING_MEMLEAKS*/
			, pPrev(NULL)
			, pNext(NULL)
		{}
	};
#pragma pack()

protected:
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

public:
	// ���ת������
	EXP_INLINE static block_t* BlockPtr(void* pPtr) { return pPtr ? (block_t*)(((BYTE*)pPtr) - sizeof(block_t)) : NULL; }
	EXP_INLINE static void* PtrBlock(block_t* pPtr) { return pPtr ? (void*)(pPtr + 1) : NULL; }

protected:
	alloc_t		m_PoolList[17];
	DWORD		m_PoolSize[17];
	mutex_t		m_Mutex;
	blk_list_t	m_UsedList;

public:
	CMemPoolT()
#ifdef	EXP_DUMPING_MEMLEAKS
		: m_nTime(0)
#endif/*EXP_DUMPING_MEMLEAKS*/
	{
		alloc_t* pool = m_PoolList; DWORD* size = m_PoolSize;
		pool->SetObjSize(0);
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<1>));
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<2>));
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<4>));
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<8>));
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<16>));
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<32>));
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<64>));
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<128>));
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<256>));
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<512>));
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<1024>));
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<2048>));
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<4096>));
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<8192>));
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<16384>));
		*(size++) = (pool++)->GetObjSize();
		pool->SetObjSize(sizeof(_TypeT<32768>));
		*(size++) = (pool++)->GetObjSize();
	}
	~CMemPoolT()
	{
		Clear(PolicyT::DUMP_MEM_LEAKS);
	}

public:
	// �ڴ�Ч��
	BOOL Valid(void* pPtr)
	{
		ExLock(m_Mutex, TRUE, mutex_t);
		block_t* block = BlockPtr(pPtr);
		ExAssert(block);
		return block->pPool->Valid(block);
	}
	// �ڴ��С
	DWORD Size(void* pPtr)
	{
		ExLock(m_Mutex, TRUE, mutex_t);
		block_t* block = BlockPtr(pPtr);
		ExAssert(block);
		return block->pPool->Size(block) - sizeof(block_t);
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
			DWORD size = m_PoolSize[mid];
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
				if (m_PoolSize[++mid] >= nSize)
					break;
				else
					left = mid;
			} else
			{
				size = m_PoolSize[--mid];
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
		block = (block_t*)m_PoolList[mid].Alloc(nSize);
		// ���ر���ڴ��
		ExAssert(block);
		block->block_t::block_t();
		block->pPool = m_PoolList + mid;
#ifdef	EXP_DUMPING_MEMLEAKS
		block->nTime = ++m_nTime;
		ExAssert(m_nTime != EXP_BREAK_ALLOC);
#endif/*EXP_DUMPING_MEMLEAKS*/
		m_UsedList.Push(block);
		return PtrBlock(block);
	}
	EXP_INLINE void* SetAlloc(void* p, LPCSTR sFile, int nLine)
	{
#ifdef	EXP_DUMPING_MEMLEAKS
		if (!p) return NULL;
		block_t* block = BlockPtr(p);
		block->sFile = sFile;
		block->nLine = nLine;
#endif/*EXP_DUMPING_MEMLEAKS*/
		return p;
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
		block->pPool->Free(block);
	}
	// ����ڴ��
	void Clear(BOOL bDump = TRUE)
	{
		ExLock(m_Mutex, FALSE, mutex_t);
		// �����ڴ��¼����
		if( !m_UsedList.Empty() )
		{
#ifdef	EXP_DUMPING_MEMLEAKS
			DWORD dump_counter = 0;
			if (bDump)
				ExDPrintf(_T("\nDetected memory leaks!\nDumping objects ->\n"));
#endif/*EXP_DUMPING_MEMLEAKS*/
			block_t* item = m_UsedList.Head();
			do
			{
				block_t* next = item->pNext;
				alloc_t* pool = item->pPool;
				ExAssert(pool);
#ifdef	EXP_DUMPING_MEMLEAKS
				if (bDump && ++dump_counter)
				{
					BYTE* ptr = (BYTE*)PtrBlock(item);
					if (item->nTime > 0)
						ExDPrintf(_T("{%d, 0x%08X, %d bytes}"), item->nTime, ptr, pool->Size(item));
					else
						ExDPrintf(_T("{0x%08X, %d bytes}"), ptr, pool->Size(item));
					ExDPrintf(_T("\t[%02X %02X %02X %02X %02X %02X %02X %02X]"), 
						ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);
					if (item->sFile)
					{
						ExDPrintf(_T("\t"));
						OutputDebugStringA(item->sFile);
						ExDPrintf(_T("\t(%d)"), item->nLine);
					}
					ExDPrintf(_T("\n"));
				}
#endif/*EXP_DUMPING_MEMLEAKS*/
				m_UsedList.Pop(item);
				pool->Free(item);
				item = next;
			} while(!m_UsedList.Empty());
#ifdef	EXP_DUMPING_MEMLEAKS
			if (bDump)
			{
				if (dump_counter == 0)
					ExDPrintf(_T("No Object has been found!\n"));
				else
				if (dump_counter == 1)
					ExDPrintf(_T("1 Object has been found!\n"));
				else
					ExDPrintf(_T("%d Objects have been found!\n"), dump_counter);
				ExDPrintf(_T("Object dump complete.\n\n"));
			}
#endif/*EXP_DUMPING_MEMLEAKS*/
		}
		// ��������
		for(int i = 0; i < 17; ++i)
			m_PoolList[i].Clear();
	}
};

typedef CMemPoolT<> CMemPool;

//////////////////////////////////////////////////////////////////

template <typename AllocT = EXP_BLKPOOL_ALLOC, typename ModelT = EXP_THREAD_MODEL>
class CMemPoolAllocT
{
public:
	typedef CMemPoolAllocT alloc_t;
	typedef CMemPoolT<_MemPoolPolicy<AllocT, ModelT> > mem_alloc_t;

	EXP_INLINE static mem_alloc_t& GetPool()
	{
		return EXP_SINGLETON<mem_alloc_t, ModelT>::Instance();
	}

public:
	EXP_INLINE static BOOL Valid(void* pPtr)
	{ return GetPool().Valid(pPtr); }
	EXP_INLINE static DWORD Size(void* pPtr)
	{ return GetPool().Size(pPtr); }

	EXP_INLINE static void* Alloc(DWORD nSize)
	{ return GetPool().Alloc(nSize); }
	EXP_INLINE static void* SetAlloc(void* p, LPCSTR sFile, int nLine)
	{ return GetPool().SetAlloc(p, sFile, nLine); }
	EXP_INLINE static void Free(void* pPtr)
	{ GetPool().Free(pPtr); }

	EXP_INLINE static void Check()
	{ base_alloc_t::Check(); }
};

typedef CMemPoolAllocT<> CMemPoolAlloc;

#ifndef EXP_MEMPOOL_ALLOC
#define EXP_MEMPOOL_ALLOC CMemPoolAlloc
#endif/*EXP_MEMPOOL_ALLOC*/

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__MemPool_h__*/