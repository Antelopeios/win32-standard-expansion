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
// Blog:	blog.csdn.net/markl22222
// E-Mail:	mark.lonr@tom.com
// Date:	2011-03-01
// Version:	1.2.0016.2345
//
// History:
//	- 1.2.0016.2345(2011-03-01)	^ �Ľ�MemPool���ڲ�ʵ�ַ�ʽ,���߼�,�Ż��㷨
//								+ MemPool֧��Clearʱ���м򵥵��ڴ�й©���
//////////////////////////////////////////////////////////////////

#ifndef __MemPool_h__
#define __MemPool_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Algorithm/NonCopyable.h"
#include "Debugging/Assertion.h"
#include "Debugging/Trace.h"
#include "Pattern/Singleton.h"
#include "Thread/Lock.h"
#include "Memory/MemHeap.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename AllocT = CMemHeapAlloc, typename ModelT = EXP_THREAD_MODEL>
struct _MemPoolPolicy;

template <typename PolicyT = _MemPoolPolicy<> >
class CMemPoolT : CNonCopyable
{
public:
	typedef typename PolicyT::alloc_t alloc_t;
	typedef typename PolicyT::model_t model_t;
	typedef typename PolicyT::mutex_t mutex_t;

protected:
	// ���������
	typedef class CBlockList
	{
	public:
		// ��ṹ����
		typedef struct _Block
		{
			_Block*	pPrev;	// ��һ�����
			_Block*	pNext;	// ��һ�����
		} block_t;

		enum { HeadSize = sizeof(block_t) };

	protected:
		block_t* m_pList;	// ����ͷָ��
		DWORD	 m_nCont;	// ������

	public:
		CBlockList()
			: m_pList(NULL)
			, m_nCont(0)
		{ }
		~CBlockList()
		{ /*���������κ�������*/ }

	public:
		block_t* Push(block_t* pItem, block_t* pLast = NULL)
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
			++m_nCont;
			return pItem;
		}
		block_t* Pop(block_t* pItem = NULL)
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
				--m_nCont;
			}
			return pItem;
		}

		void Clear()
		{	// ֱ���ÿ�,�����κ�������
			m_pList = NULL;
			m_nCont = 0;
		}

		bool Empty()
		{ return (m_pList == NULL); }
		DWORD Count()
		{ return m_nCont; }

		block_t* Head()
		{ return m_pList; }
		block_t* Tail()
		{ return m_pList; }
		block_t* Last()
		{ return m_pList ? m_pList->pPrev : NULL; }
	} blk_list_t;
	typedef typename blk_list_t::block_t block_t;
	typedef typename block_t fre_block_t;

	// �ڴ������
	typedef class CMemList : public blk_list_t
	{
	public:
		// �ڴ��ṹ
		typedef struct _MemBlock : public fre_block_t
		{
			DWORD	nSize;		// �ڴ���С
			bool	bHead;		// �Ƿ�Ϊ�ڴ��ͷ
			bool	bUsed;		// �Ƿ����ڱ�ʹ��
			fre_block_t fBlock;	// ����������
		} block_t;

		enum
		{
			HeadSize = sizeof(block_t), 
			MemBSize = HeadSize - blk_list_t::HeadSize
		};

	protected:
		DWORD	 m_nSize;	// �����С

	public:
		block_t* Push(block_t* pItem, block_t* pLast = NULL)
		{
			if (pItem = static_cast<block_t*>(blk_list_t::Push(pItem, pLast)))
				m_nSize += pItem->nSize;
			return pItem;
		}
		block_t* Pop(block_t* pItem = NULL)
		{
			if (pItem = static_cast<block_t*>(blk_list_t::Pop(pItem)))
				m_nSize -= pItem->nSize;
			return pItem;
		}
		block_t* Cut(DWORD nCutSize, block_t* pItem = m_pList)
		{
			if(!nCutSize ) return NULL;
			// Ĭ�ϴ�ͷ���ָ���
			if(!pItem ) return NULL;
			block_t* cut_pblk = NULL;
			if (pItem->nSize > ((HeadSize << 1) + nCutSize))	// �и�ʱ���ܲ�������ϸС���ڴ���Ƭ
			{
				// �������ڴ���β���и��ڴ��
				pItem->nSize -= (HeadSize + nCutSize);
				cut_pblk = (block_t*)(((BYTE*)pItem) + HeadSize + pItem->nSize);
				ZeroMemory(cut_pblk, HeadSize);
				cut_pblk->nSize = nCutSize;
				cut_pblk->pPrev = pItem;
				cut_pblk->pNext = pItem->pNext;
				if (pItem->pNext)
					pItem->pNext->pPrev = cut_pblk;
				pItem->pNext = cut_pblk;
				m_nSize -= HeadSize;
				++m_nCont;
			} else
				cut_pblk = pItem;
			return cut_pblk;
		}
		block_t* Merger(block_t* pMerg)
		{
			if(!pMerg || pMerg->bUsed ) return NULL;
			block_t* mer_pblk = static_cast<block_t*>(pMerg->pPrev);
			if(!mer_pblk || mer_pblk->bUsed || mer_pblk == pMerg ) return NULL;
			// �ϲ�������
			if( pMerg->bHead ) return NULL;
			if(((BYTE*)mer_pblk) + HeadSize + mer_pblk->nSize != (BYTE*)pMerg ) return NULL;
			mer_pblk->nSize += (HeadSize + pMerg->nSize);
			mer_pblk->pNext = pMerg->pNext;
			if (mer_pblk->pNext)
				mer_pblk->pNext->pPrev = mer_pblk;
			m_nSize += HeadSize;
			--m_nCont;
			return mer_pblk;
		}

		void Clear()
		{
			blk_list_t::Clear();
			m_nSize = 0;
		}

		DWORD Size()
		{ return m_nSize; }
	} mem_list_t;
	typedef typename mem_list_t::block_t mem_block_t;

	// ����ڴ������
	typedef class CBlockAlloc
	{
	protected:
		alloc_t m_Alloc;

	public:
		// ����ڴ��¼���Ӧ��ָ��
		void* PtrReal(void* pPtr)
		{ return pPtr ? (((BYTE*)pPtr) + mem_list_t::HeadSize) : NULL; }
		void* PtrBlock(mem_block_t* pPtr)
		{ return PtrReal((void*)pPtr); }

		// ���ָ���Ӧ���ڴ��¼��
		void* RealPtr(void* pPtr)
		{ return pPtr ? (((BYTE*)pPtr) - mem_list_t::HeadSize) : NULL; }
		mem_block_t* BlockPtr(void* pPtr)
		{ return (mem_block_t*)RealPtr(pPtr); }

		// ���м�¼�����ڴ��¼���ת��
		mem_block_t* BlockFre(fre_block_t* pFre)
		{ return pFre ? (mem_block_t*)(((BYTE*)pFre) - mem_list_t::MemBSize) : NULL; }
		fre_block_t* FreBlock(mem_block_t* pBlk)
		{ return pBlk ? &(pBlk->fBlock) : NULL; }

		// �ڴ�У��
		bool Valid(void* pPtr)
		{
			mem_block_t* block = BlockPtr(pPtr);
			return block ? block->bUsed : false;
		}
		DWORD Size(void* pPtr)
		{
			mem_block_t* block = BlockPtr(pPtr);
			return block ? block->nSize : 0;
		}

		// �ڴ����
		mem_block_t* Alloc(DWORD nSize)
		{
			if(!nSize ) return NULL;
			mem_block_t* pBlock = (mem_block_t*)ZeroMemory(
				m_Alloc.Alloc(mem_list_t::HeadSize + nSize), mem_list_t::HeadSize + nSize);
			pBlock->nSize = nSize;
			pBlock->bHead = true;
			return pBlock;
		}
		void Free(mem_block_t* pBlock)
		{
			if(!pBlock || !(pBlock->bHead) ) return;
			m_Alloc.Free(pBlock);
		}
	} blk_alloc_t;

protected:
	mutex_t		m_Mutex;	// �߳�ͬ���������
	blk_alloc_t m_Alloc;	// �ڴ������

	CMemList	m_MemList;	// �ڴ��¼������
	CBlockList	m_FreList;	// ���м�¼������

	DWORD		m_nMaxSize;	// �ڴ�ش�С����

public:
	CMemPoolT(DWORD nSize = PolicyT::s_nDefSize)
		: m_nMaxSize(PolicyT::s_nMaxSize)
	{ SetPoolSize(nSize); }
	~CMemPoolT()
	{ Clear(PolicyT::s_bDumpMemLeaks); }

public:
	// �ڴ�ش�С����
	DWORD GetPoolSize()
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_MemList.Size();
	}
	void SetPoolSize(DWORD nSize = PolicyT::s_nDefSize)
	{
		ExLock(m_Mutex, false, mutex_t);
		if( nSize > m_nMaxSize )
			nSize = m_nMaxSize;
		if( m_MemList.Size() >= nSize ) return;
		// �����µ��ڴ��
		mem_block_t* block = m_Alloc.Alloc(nSize - m_MemList.Size());
		// ����ڴ��¼��
		m_MemList.Push(block);
		// ��ӿ��м�¼��
		m_FreList.Push(m_Alloc.FreBlock(block));
	}
	DWORD GetMaxSize()
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_nMaxSize;
	}
	void SetMaxSize(DWORD nMaxSize)
	{
		ExLock(m_Mutex, false, mutex_t);
		m_nMaxSize = nMaxSize;
	}

	// �ڴ�Ч��
	bool Valid(void* pPtr)
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_Alloc.Valid(pPtr);
	}
	// �ڴ��С
	DWORD Size(void* pPtr)
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_Alloc.Size(pPtr);
	}
	// �����ڴ�
	void* Alloc(DWORD nSize)
	{
		if (nSize == 0) return NULL;
		mem_block_t* block = NULL;
		ExLock(m_Mutex, false, mutex_t);
		// ���ҿ����ڴ��
		if( !m_FreList.Empty() )
		{
			fre_block_t* item = m_FreList.Head();
			do
			{
				mem_block_t* temp = m_Alloc.BlockFre(item);
				if( temp->nSize >= nSize )
				{	// �ҵ����ʵ��ڴ��
					block = temp;
					break;
				}
				item = item->pNext;
			} while( item != m_FreList.Tail() );
		}
		// ��ǿ����ڴ��
		if( block )
		{	// �ҵ����ʵ��ڴ��
			if( block->nSize > nSize ) // �ڴ����Ҫ�ָ�
				block = m_MemList.Cut(nSize, block);
			m_FreList.Pop(m_Alloc.FreBlock(block));
		} else
		{	// �����µ��ڴ��
			block = m_Alloc.Alloc(nSize);
			// ����ڴ��¼��
			m_MemList.Push(block);
		}
		ExAssert(block && !(block->bUsed));
		if(!block || block->bUsed) return NULL;
		block->bUsed = true;
		// ���ط�����ڴ�
		return m_Alloc.PtrBlock(block);
	}
	// �����ڴ�
	void Free(void* pPtr)
	{
		if (!pPtr) return;
		ExLock(m_Mutex, false, mutex_t);
		// ��ö�Ӧ�ڴ��
		mem_block_t* block = m_Alloc.BlockPtr(pPtr);
		ExAssert(block && block->bUsed);
		if(!block || !(block->bUsed)) return;
		// ���ʹ���ڴ��
		block->bUsed = false;
		// �ͷſռ�
		if( m_MemList.Size() > m_nMaxSize )
		{	// �ͷŵ��ܹ��ͷŵ��ڴ��
			if( block->bHead )
			{
				m_MemList.Pop(block);
				m_Alloc.Free(block);
			}
		} else
		{
			// ����黹�ڴ�
			mem_block_t* prev = m_MemList.Merger(block);	// ��ǰ�ϲ�
			if (prev)
				block = prev;
			else
				m_FreList.Push(m_Alloc.FreBlock(block));
			mem_block_t* next = static_cast<mem_block_t*>(block->pNext);
			if (m_MemList.Merger(next))						// ���ϲ�
				m_FreList.Pop(m_Alloc.FreBlock(next));
		}
	}

#ifdef	_DEBUG
#ifndef	EXP_DUMPING_MEMLEAKS
#define	EXP_DUMPING_MEMLEAKS
#endif/*EXP_DUMPING_MEMLEAKS*/
#endif/*_DEBUG*/

	// ����ڴ��
	void Clear(bool bDump = true)
	{
		ExLock(m_Mutex, false, mutex_t);
		// �����ڴ��¼����
#ifdef	EXP_DUMPING_MEMLEAKS
		DWORD dump_counter = 0;
		if (bDump)
			ExDPrintf(_T("\nDumping memory leaks...\n"));
#endif/*EXP_DUMPING_MEMLEAKS*/
		if( !m_MemList.Empty() )
		{
			// ��һ�α���,�Ƴ���ͷ���
			mem_block_t* item = static_cast<mem_block_t*>(m_MemList.Head());
			do
			{
				mem_block_t* temp = static_cast<mem_block_t*>(item->pNext);
#ifdef	EXP_DUMPING_MEMLEAKS
				if (bDump && item->bUsed && ++dump_counter)
				{
					BYTE* ptr = (BYTE*)m_Alloc.PtrBlock(item);
					ExDPrintf(_T("\nMemory leak at 0x%08X, %d bytes long.\nData:"), ptr, item->nSize);
					ExDPrintf(
						_T("%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n"), 
						ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7], 
						ptr[8], ptr[9], ptr[10], ptr[11], ptr[12], ptr[13], ptr[14], ptr[15]);
				}
#endif/*EXP_DUMPING_MEMLEAKS*/
				if (!(item->bHead)) m_MemList.Pop(item);
				item = temp;
			} while( item != m_MemList.Tail() );
			// �ڶ��α���,��������
			item = static_cast<mem_block_t*>(m_MemList.Head());
			do
			{
				mem_block_t* temp = static_cast<mem_block_t*>(item->pNext);
				m_Alloc.Free(item);
				item = temp;
			} while( item != m_MemList.Tail() );
			m_MemList.Clear();
			m_FreList.Clear();
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
			ExDPrintf(_T("Complete memory leak detection.\n"));
		}
#endif/*EXP_DUMPING_MEMLEAKS*/
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

template <typename AllocT/* = CMemHeapAlloc*/, typename ModelT/* = EXP_THREAD_MODEL*/>
struct _MemPoolPolicy
{
	typedef AllocT alloc_t;
	typedef ModelT model_t;
	typedef typename model_t::_LockPolicy mutex_policy_t;
	typedef CLockT<mutex_policy_t> mutex_t;

	static const DWORD	s_nDefSize = 1024 * 1024;		// ��ʼ��С1M
	static const DWORD	s_nMaxSize = 1024 * 1024 * 100;	// ����С100M
	static const bool	s_bDumpMemLeaks = true;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__MemPool_h__*/