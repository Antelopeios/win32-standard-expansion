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
// Date:	2011-02-22
// Version:	1.1.0014.1100
//////////////////////////////////////////////////////////////////

#ifndef __MemPool_h__
#define __MemPool_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Debugging/Assertion.h"
#include "Pattern/Singleton.h"
#include "Memory/ObjPool.h"

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
	// ����ڴ������
	class CBlockAlloc;
	// �ڴ������
	class CBlockList
	{
	public:
		// �ڴ��ṹ
		typedef struct _Block
		{
			typedef CObjPoolT<_Block, _ObjPoolPolicyT<CRegistAllocT<alloc_t>, _SingleModel> > ob_alloc_t;

			typedef struct { _Block* pLink; } ptr_t;
			enum { HeadSize = sizeof(ptr_t) };

			BYTE*	pBuff;	// �ڴ��ָ��
			DWORD	nSize;	// �ڴ���С
			bool	bHead;	// �Ƿ�Ϊ�ڴ��ͷ
			bool	bUsed;	// �Ƿ����ڱ�ʹ��
			_Block*	pConn;	// �������
			_Block*	pPrev;	// ��һ�����
			_Block*	pNext;	// ��һ�����

			_Block() { ZeroMemory(this, sizeof(_Block)); }

			static void* RealPtr(void* pPtr)
			{ return (((BYTE*)pPtr) - HeadSize); }

			static _Block* Alloc(alloc_t& alloc, ob_alloc_t& ob_alloc, DWORD nSize, BYTE* pBuff = NULL)
			{
				if(!nSize ) return NULL;
				_Block* pBlock = (_Block*)ob_alloc.Alloc();
				if (pBuff)
					pBlock->pBuff = pBuff;
				else
				{
					ptr_t* pPtr = (ptr_t*)alloc.Alloc(HeadSize + nSize);
					pBlock->pBuff = (BYTE*)(pPtr + 1);
				}
				pBlock->nSize = nSize;
				return pBlock;
			}
			static void Free(alloc_t& alloc, ob_alloc_t& ob_alloc, _Block* pBlock, bool bHoldBuff = false)
			{
				if(!pBlock ) return;
				if(!bHoldBuff && pBlock->bHead && pBlock->pBuff )
					alloc.Free(RealPtr(pBlock->pBuff));
				ob_alloc.Free(pBlock);
			}
			static _Block* Clone(ob_alloc_t& ob_alloc, _Block* pBlock)
			{
				if(!pBlock ) return NULL;
				return (_Block*)memcpy(ob_alloc.Alloc(), pBlock, sizeof(_Block));
			}

			void Link()
			{
				ptr_t* ptr = (ptr_t*)RealPtr(pBuff);
				ptr->pLink = this;
			}

			void Connect(_Block* pBlock)
			{
				if(!pBlock ) return;
				if( IsConnect(pBlock) ) return;
				Disconnect();
				pBlock->Disconnect();
				pConn = pBlock;
				pBlock->pConn = this;
			}
			void Disconnect()
			{
				if(!pConn ) return;
				pConn->pConn = NULL;
				pConn = NULL;
			}
			bool IsConnect(_Block* pBlock = NULL)
			{
				if( pBlock )
					return (pConn == pBlock && pBlock->pConn == this);
				else
					return (pConn != NULL);
			}
		} block_t;

	protected:
		block_t*	m_pList;	// ����ͷָ��
		DWORD		m_nSize;	// �����С
		DWORD		m_nCont;	// ������

		CBlockAlloc& m_Alloc;	// ����ڴ������

	public:
		CBlockList(CBlockAlloc& alloc)
			: m_pList(NULL)
			, m_nSize(0)
			, m_nCont(0)
			, m_Alloc(alloc)
		{ }
		~CBlockList()
		{ /*���������κ�������*/ }

	public:
		block_t* Push(block_t* pItem, block_t* pLast = NULL)
		{	// Ĭ�ϴ�β��������
			if( !pItem ) return NULL;
			if( pLast == pItem ) pLast = NULL;
			if( (pItem->pPrev && pItem->pPrev->pNext == pItem) && 
				(pItem->pNext && pItem->pNext->pPrev == pItem) ) return pItem;
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
			m_nSize += pItem->nSize;
			++m_nCont;
			return pItem;
		}
		block_t* PushOrder(block_t* pItem)
		{	// ˳�������
			block_t* pTemp = m_pList;
			if( pTemp )
			{
				do
				{
					if( pTemp->uSize >= pItem->uSize )
						break;
					pTemp = pTemp->pNext;
				} while( pTemp != m_pList );
			}
			return Push(pItem, pTemp);
		}
		block_t* Pop(block_t* pItem = NULL)
		{	// Ĭ�ϴ�ͷ���������
			if(!pItem ) pItem = m_pList;
			if( pItem )
			{	// �������
				if(!pItem->pPrev && !pItem->pNext) return pItem;
				if( m_pList == pItem ) m_pList = m_pList->pNext;
				if( pItem->pPrev ) pItem->pPrev->pNext = pItem->pNext;
				if( pItem->pNext ) pItem->pNext->pPrev = pItem->pPrev;
				pItem->pPrev = NULL;
				pItem->pNext = NULL;
				if( m_pList == pItem ) m_pList = NULL;
				m_nSize -= pItem->nSize;
				--m_nCont;
			}
			return pItem;
		}
		block_t* Cut(DWORD nCutSize, block_t*& pItem = m_pList)
		{
			if(!nCutSize ) return NULL;
			// Ĭ�ϴ�ͷ���ָ���
			if(!pItem ) pItem = m_pList;
			if(!pItem ) return NULL;
			block_t* pCutf = NULL;
			if (pItem->nSize > (nCutSize + block_t::HeadSize))
			{
				pCutf = m_Alloc.Alloc(nCutSize, pItem->pBuff);
				if(!pCutf ) return NULL;
				// �и��ڴ��
				pCutf->bHead = pItem->bHead;
				pItem->pBuff += (nCutSize + block_t::HeadSize);
				pItem->nSize -= (nCutSize + block_t::HeadSize);
				pItem->bHead = false;
				m_nSize -= (nCutSize + block_t::HeadSize);
			} else
			{
				block_t* pLast = pItem->pNext;
				pCutf = Pop(pItem);
				pItem = pLast;
			}
			return pCutf;
		}
		block_t* Merger(block_t* pMerg, block_t* pItem = NULL, bool bBack = false)
		{
			if(!pMerg ) return NULL;
			// �������ϲ��Ľ��
			pMerg = Pop(pMerg);
			// Ĭ�ϴ�ͷ���ϲ����
			if(!pItem ) pItem = m_pList;
			if(!pItem ) return NULL;
			if( bBack )
			{	// �ϲ�������
				if( pItem->pBuff + pItem->nSize != block_t::RealPtr(pMerg->pBuff) ) return NULL;
			} else
			{	// �ϲ���ǰ��
				if( pMerg->pBuff + pMerg->nSize != block_t::RealPtr(pItem->pBuff) ) return NULL;
				pItem->pBuff = pMerg->pBuff;
				pItem->bHead = pMerg->bHead;
			}
			pItem->nSize += (pMerg->nSize + block_t::HeadSize);
			m_nSize += (pMerg->nSize + block_t::HeadSize);
			return pMerg;
		}

		void Clear()
		{	// ֱ���ÿ�,�����κ�������
			m_pList = NULL;
			m_nSize = m_nCont = 0;
		}

		bool Empty()
		{ return (m_pList == NULL); }
		DWORD Size()
		{ return m_nSize; }
		DWORD Count()
		{ return m_nCont; }

		block_t* Head()
		{ return m_pList; }
		block_t* Tail()
		{ return m_pList ? m_pList->pPrev : NULL; }

		block_t* operator->() const
		{ return Head(); }
		operator block_t*() const
		{ return Head(); }
	};

	// ����ڴ������
	class CBlockAlloc
	{
	public:
		typedef typename CBlockList::block_t block_t;
		typedef typename block_t::ob_alloc_t ob_alloc_t;

	protected:
		alloc_t m_Alloc;
		ob_alloc_t m_ObAlloc;

	public:
		void* RealPtr(void* pPtr)
		{ return block_t::RealPtr(pPtr); }
		// ���ָ���Ӧ���ڴ��¼��
		block_t* BlockPtr(void* pPtr)
		{
			if(!pPtr ) return NULL;
			// ����ʹ���ڴ��
			block_t::ptr_t* real = (block_t::ptr_t*)RealPtr(pPtr);
			return real->pLink;
		}

		bool Valid(void* pPtr)
		{ return m_Alloc.Valid(RealPtr(pPtr)); }
		DWORD Size(void* pPtr)
		{
			block_t* block = BlockPtr(pPtr);
			return block ? block->nSize : 0;
		}

		block_t* Alloc(DWORD nSize, BYTE* pBuff = NULL)
		{ return block_t::Alloc(m_Alloc, m_ObAlloc, nSize, pBuff); }
		void Free(block_t* pBlock, bool bHoldBuff = false)
		{ block_t::Free(m_Alloc, m_ObAlloc, pBlock, bHoldBuff); }
		block_t* Clone(block_t* pBlock)
		{ return block_t::Clone(m_ObAlloc, pBlock); }
	};

public:
	typedef CBlockList list_t;
	typedef typename list_t::block_t block_t;
	typedef CBlockAlloc block_alloc_t;

protected:
	// �ڴ����
	block_alloc_t m_BlockAlloc;

	// �߳�ͬ��
	mutex_t	m_Mutex;

	list_t	m_BlocList;		// �ڴ������
	list_t	m_FreeList;		// �����ڴ������
	list_t	m_UsedList;		// ʹ���ڴ������
	bool	m_bAutoOrder;	// �Զ������ڴ��
	DWORD	m_nMaxSize;		// �ڴ�ش�С����

public:
	CMemPoolT(DWORD nSize = PolicyT::s_nDefSize)
		: m_BlocList(m_BlockAlloc)
		, m_FreeList(m_BlockAlloc)
		, m_UsedList(m_BlockAlloc)
		, m_bAutoOrder(PolicyT::s_bAutoOrder)
		, m_nMaxSize(PolicyT::s_nMaxSize)
	{ SetPoolSize(nSize); }
	~CMemPoolT()
	{ Clear(); }

public:
	// �ڴ��Զ���������
	bool IsAutoOrder()
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_bAutoOrder;
	}
	void SetAutoOrder(bool bAutoOrder)
	{
		ExLock(m_Mutex, false, mutex_t);
		m_bAutoOrder = bAutoOrder;
	}

	// �ڴ�ش�С����
	DWORD GetPoolSize()
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_BlocList.Size();
	}
	void SetPoolSize(DWORD nSize = PolicyT::s_nDefSize)
	{
		ExLock(m_Mutex, false, mutex_t);
		if( nSize > m_nMaxSize )
			nSize = m_nMaxSize;
		if( m_BlocList.Size() >= nSize ) return;
		Free(Alloc(nSize - m_BlocList.Size()));
	}
	DWORD GetFreeSize()
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_FreeList.Size();
	}
	DWORD GetUsedSize()
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_UsedList.Size();
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
		return m_BlockAlloc.Valid(pPtr);
	}
	// �ڴ��С
	DWORD Size(void* pPtr)
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_BlockAlloc.Size(pPtr);
	}
	// �����ڴ�
	void* Alloc(DWORD nSize)
	{
		if (nSize == 0) return NULL;
		block_t* pBlockUse = NULL;
		ExLock(m_Mutex, false, mutex_t);
		// ���ҿ����ڴ��
		if( !m_FreeList.Empty() )
		{
			block_t* pItem = m_FreeList.Head();
			do
			{
				if( pItem->nSize >= nSize )
				{	// �ҵ����ʵ��ڴ��
					pBlockUse = pItem;
					break;
				}
				pItem = pItem->pNext;
			} while( pItem != m_FreeList.Head() );
		}
		// ��ǿ����ڴ��
		if( pBlockUse )
		{	// �ҵ����ʵ��ڴ��
			if( pBlockUse->nSize > nSize )
			{	// �ڴ����Ҫ�ָ�
				ExAssert(pBlockUse->pConn);
				block_t* pBlockCon = pBlockUse->pConn;					// ��ù����ڴ��
				block_t* pBlockNew = m_BlocList.Cut(nSize, pBlockCon);	// �ָ�����ڴ��
				ExAssert(pBlockNew);
				pBlockUse = m_FreeList.Cut(nSize, pBlockUse);			// �ָ�����ڴ��
				ExAssert(pBlockUse);
				pBlockNew->bUsed = pBlockUse->bUsed = true;				// ����ڴ��ʹ��
				// �����ڴ��
				pBlockNew->Connect(pBlockUse);
				// �����ڴ��
				m_UsedList.Push(pBlockUse);
				m_BlocList.Push(pBlockNew, pBlockCon);
			} else
			{	// �ڴ��պú���
				m_FreeList.Pop(pBlockUse);
				pBlockUse->bUsed = true;
				ExAssert(pBlockUse->pConn);
				pBlockUse->pConn->bUsed = true;
				m_UsedList.Push(pBlockUse);
			}
		} else
		{	// �����ڴ�鶼������
			// �����µ��ڴ��
			pBlockUse = m_BlockAlloc.Alloc(nSize);
			pBlockUse->bHead = true;
			pBlockUse->bUsed = true;
			// �����ڴ��
			block_t* pBlockNew = m_BlockAlloc.Clone(pBlockUse);
			pBlockNew->Connect(pBlockUse);
			// �����ڴ��
			m_UsedList.Push(pBlockUse);
			m_BlocList.Push(pBlockNew);
		}
		// ����ʹ�ý��
		pBlockUse->Link();
		// ���ط�����ڴ�
		return pBlockUse->pBuff;
	}
	// �����ڴ�
	void Free(void* pPtr)
	{
		if (!pPtr) return;
		// ��ö�Ӧ�ڴ��
		ExLock(m_Mutex, false, mutex_t);
		block_t* pBlockUse = m_BlockAlloc.BlockPtr(pPtr);
		ExAssert(pBlockUse && pBlockUse->bUsed);
		// ����ʹ���ڴ��
		if( pBlockUse && pBlockUse->bUsed )
		{	// �ҵ����ʵ��ڴ��
			m_UsedList.Pop(pBlockUse);
			pBlockUse->bUsed = false;
			ExAssert(pBlockUse->pConn);
			pBlockUse->pConn->bUsed = false;
			// �ͷſռ�
			if( GetPoolSize() > GetMaxSize() )
			{
				// �ͷŵ��ܹ��ͷŵ��ڴ��
				if( (pBlockUse->bHead) && 
					(pBlockUse->pConn->pNext->bHead) )
				{
					m_BlockAlloc.Free(pBlockUse);
					m_BlockAlloc.Free(m_BlocList.Pop(pBlockUse->pConn), true);
				}
			}
			else
			{
				// ����黹�ڴ�
				m_FreeList.Push(m_bAutoOrder ? Order(pBlockUse) : pBlockUse);
				ExAssert(!m_FreeList.Empty());
			}
		}
	}
	// �����ڴ�
	block_t* Order(block_t* pItem = NULL)
	{
		ExLock(m_Mutex, false, mutex_t);
		if( m_BlocList.Count() <= 1 ) return pItem ? pItem : NULL;
		if( pItem )
		{
			if( pItem->bUsed ) return NULL;
			block_t* pConn = pItem->pConn;
			ExAssert(pConn);
			if( pConn->bUsed ) return NULL;
			bool bOrder = false;
			block_t* pPrev = (pConn == m_BlocList.Head() ? NULL : pConn->pPrev);
			block_t* pNext = pConn->pNext;
			if(!pConn->bHead && pPrev && pPrev->bUsed == false )
			{
				ExAssert(pPrev->pConn);
				// ��ǰһ�����ϲ�, �����ٲ���Ҫ���ڴ��
				m_BlockAlloc.Free(m_FreeList.Merger(pItem, pPrev->pConn, true), true);
				m_BlockAlloc.Free(m_BlocList.Merger(pConn, pPrev, true), true);
				// �����α�
				pConn = pPrev;
				pItem = pConn->pConn;
				bOrder = true;
			}
			if(pNext && !pNext->bHead && pNext->bUsed == false )
			{
				ExAssert(pNext->pConn);
				// ���һ�����ϲ�, �����ٲ���Ҫ���ڴ��
				m_BlockAlloc.Free(m_FreeList.Merger(pItem, pNext->pConn, false), true);
				m_BlockAlloc.Free(m_BlocList.Merger(pConn, pNext, false), true);
				bOrder = true;
			}
			return bOrder ? pNext->pConn : pItem;
		} else
		{
			pItem = m_BlocList.Head();
			do
			{
				if(!(pItem->pNext->bHead || pItem->pNext->bUsed) )
				{	
					ExAssert(pItem->pNext->pConn);
					// �ϲ������ڴ��, �����ٲ���Ҫ���ڴ��
					m_BlockAlloc.Free(m_FreeList.Merger(pItem->pNext->pConn, pItem->pConn, true), true);
					m_BlockAlloc.Free(m_BlocList.Merger(pItem->pNext, pItem, true), true);
				}
				pItem = pItem->pNext;
			} while( pItem != m_BlocList.Head() );
		}
		return NULL;
	}
	// ����ڴ��
	void Clear()
	{
		block_t* pItemTemp = NULL;
		block_t* pItem = NULL;
		ExLock(m_Mutex, false, mutex_t);
		// �����ڴ�����
		if( !m_BlocList.Empty() )
		{
			pItem = m_BlocList.Head();
			do
			{
				pItemTemp = pItem->pNext;
				m_BlockAlloc.Free(pItem);
				pItem = pItemTemp;
			} while( pItem != m_BlocList.Head() );
		}
		// �����������
		if( !m_FreeList.Empty() )
		{
			pItem = m_FreeList.Head();
			do
			{	// ����������Ҫ�����ڴ�����
				pItemTemp = pItem->pNext;
				m_BlockAlloc.Free(pItem, true);
				pItem = pItemTemp;
			} while( pItem != m_FreeList.Head() );
		}
		// ����ʹ������
		if( !m_UsedList.Empty() )
		{
			pItem = m_UsedList.Head();
			do
			{	// ʹ��������Ҫ�����ڴ�����
				pItemTemp = pItem->pNext;
				m_BlockAlloc.Free(pItem, true);
				pItem = pItemTemp;
			} while( pItem != m_UsedList.Head() );
		}
		// �����ÿ�
		m_BlocList.Clear();
		m_FreeList.Clear();
		m_UsedList.Clear();
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
	static const bool	s_bAutoOrder = true;			// �Զ������ڴ��
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__MemPool_h__*/