// Copyright 2011, 木头云
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
// MemPool - 内存池
//
// Author:	木头云
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
	// 结点内存分配器
	class CBlockAlloc;
	// 内存块链表
	class CBlockList
	{
	public:
		// 内存块结构
		typedef struct _Block
		{
			typedef CObjPoolT<_Block, _ObjPoolPolicyT<CRegistAllocT<alloc_t>, _SingleModel> > ob_alloc_t;

			typedef struct { _Block* pLink; } ptr_t;
			enum { HeadSize = sizeof(ptr_t) };

			BYTE*	pBuff;	// 内存块指针
			DWORD	nSize;	// 内存块大小
			bool	bHead;	// 是否为内存块头
			bool	bUsed;	// 是否正在被使用
			_Block*	pConn;	// 关联结点
			_Block*	pPrev;	// 上一个结点
			_Block*	pNext;	// 下一个结点

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
		block_t*	m_pList;	// 链表头指针
		DWORD		m_nSize;	// 链表大小
		DWORD		m_nCont;	// 结点个数

		CBlockAlloc& m_Alloc;	// 结点内存分配器

	public:
		CBlockList(CBlockAlloc& alloc)
			: m_pList(NULL)
			, m_nSize(0)
			, m_nCont(0)
			, m_Alloc(alloc)
		{ }
		~CBlockList()
		{ /*析构不做任何清理工作*/ }

	public:
		block_t* Push(block_t* pItem, block_t* pLast = NULL)
		{	// 默认从尾部插入结点
			if( !pItem ) return NULL;
			if( pLast == pItem ) pLast = NULL;
			if( (pItem->pPrev && pItem->pPrev->pNext == pItem) && 
				(pItem->pNext && pItem->pNext->pPrev == pItem) ) return pItem;
			// 重设链表
			if( m_pList )
			{
				if( pLast == m_pList )
					m_pList = pItem;
				else
				if(!pLast )
					pLast = m_pList;
				// 插入结点
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
		{	// 顺序插入结点
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
		{	// 默认从头部弹出结点
			if(!pItem ) pItem = m_pList;
			if( pItem )
			{	// 弹出结点
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
			// 默认从头部分割结点
			if(!pItem ) pItem = m_pList;
			if(!pItem ) return NULL;
			block_t* pCutf = NULL;
			if (pItem->nSize > (nCutSize + block_t::HeadSize))
			{
				pCutf = m_Alloc.Alloc(nCutSize, pItem->pBuff);
				if(!pCutf ) return NULL;
				// 切割内存块
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
			// 弹出被合并的结点
			pMerg = Pop(pMerg);
			// 默认从头部合并结点
			if(!pItem ) pItem = m_pList;
			if(!pItem ) return NULL;
			if( bBack )
			{	// 合并到后面
				if( pItem->pBuff + pItem->nSize != block_t::RealPtr(pMerg->pBuff) ) return NULL;
			} else
			{	// 合并到前面
				if( pMerg->pBuff + pMerg->nSize != block_t::RealPtr(pItem->pBuff) ) return NULL;
				pItem->pBuff = pMerg->pBuff;
				pItem->bHead = pMerg->bHead;
			}
			pItem->nSize += (pMerg->nSize + block_t::HeadSize);
			m_nSize += (pMerg->nSize + block_t::HeadSize);
			return pMerg;
		}

		void Clear()
		{	// 直接置空,不做任何清理动作
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

	// 结点内存分配器
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
		// 获得指针对应的内存记录块
		block_t* BlockPtr(void* pPtr)
		{
			if(!pPtr ) return NULL;
			// 查找使用内存块
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
	// 内存分配
	block_alloc_t m_BlockAlloc;

	// 线程同步
	mutex_t	m_Mutex;

	list_t	m_BlocList;		// 内存块链表
	list_t	m_FreeList;		// 空闲内存块链表
	list_t	m_UsedList;		// 使用内存块链表
	bool	m_bAutoOrder;	// 自动整理内存池
	DWORD	m_nMaxSize;		// 内存池大小上限

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
	// 内存自动整理设置
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

	// 内存池大小设置
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

	// 内存效验
	bool Valid(void* pPtr)
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_BlockAlloc.Valid(pPtr);
	}
	// 内存大小
	DWORD Size(void* pPtr)
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_BlockAlloc.Size(pPtr);
	}
	// 分配内存
	void* Alloc(DWORD nSize)
	{
		if (nSize == 0) return NULL;
		block_t* pBlockUse = NULL;
		ExLock(m_Mutex, false, mutex_t);
		// 查找空闲内存块
		if( !m_FreeList.Empty() )
		{
			block_t* pItem = m_FreeList.Head();
			do
			{
				if( pItem->nSize >= nSize )
				{	// 找到合适的内存块
					pBlockUse = pItem;
					break;
				}
				pItem = pItem->pNext;
			} while( pItem != m_FreeList.Head() );
		}
		// 标记空闲内存块
		if( pBlockUse )
		{	// 找到合适的内存块
			if( pBlockUse->nSize > nSize )
			{	// 内存块需要分割
				ExAssert(pBlockUse->pConn);
				block_t* pBlockCon = pBlockUse->pConn;					// 获得关联内存块
				block_t* pBlockNew = m_BlocList.Cut(nSize, pBlockCon);	// 分割关联内存块
				ExAssert(pBlockNew);
				pBlockUse = m_FreeList.Cut(nSize, pBlockUse);			// 分割空闲内存块
				ExAssert(pBlockUse);
				pBlockNew->bUsed = pBlockUse->bUsed = true;				// 标记内存块使用
				// 关联内存块
				pBlockNew->Connect(pBlockUse);
				// 挂载内存块
				m_UsedList.Push(pBlockUse);
				m_BlocList.Push(pBlockNew, pBlockCon);
			} else
			{	// 内存块刚好合适
				m_FreeList.Pop(pBlockUse);
				pBlockUse->bUsed = true;
				ExAssert(pBlockUse->pConn);
				pBlockUse->pConn->bUsed = true;
				m_UsedList.Push(pBlockUse);
			}
		} else
		{	// 空闲内存块都不合适
			// 分配新的内存块
			pBlockUse = m_BlockAlloc.Alloc(nSize);
			pBlockUse->bHead = true;
			pBlockUse->bUsed = true;
			// 关联内存块
			block_t* pBlockNew = m_BlockAlloc.Clone(pBlockUse);
			pBlockNew->Connect(pBlockUse);
			// 挂载内存块
			m_UsedList.Push(pBlockUse);
			m_BlocList.Push(pBlockNew);
		}
		// 连接使用结点
		pBlockUse->Link();
		// 返回分配的内存
		return pBlockUse->pBuff;
	}
	// 回收内存
	void Free(void* pPtr)
	{
		if (!pPtr) return;
		// 获得对应内存块
		ExLock(m_Mutex, false, mutex_t);
		block_t* pBlockUse = m_BlockAlloc.BlockPtr(pPtr);
		ExAssert(pBlockUse && pBlockUse->bUsed);
		// 清理使用内存块
		if( pBlockUse && pBlockUse->bUsed )
		{	// 找到合适的内存块
			m_UsedList.Pop(pBlockUse);
			pBlockUse->bUsed = false;
			ExAssert(pBlockUse->pConn);
			pBlockUse->pConn->bUsed = false;
			// 释放空间
			if( GetPoolSize() > GetMaxSize() )
			{
				// 释放掉能够释放的内存块
				if( (pBlockUse->bHead) && 
					(pBlockUse->pConn->pNext->bHead) )
				{
					m_BlockAlloc.Free(pBlockUse);
					m_BlockAlloc.Free(m_BlocList.Pop(pBlockUse->pConn), true);
				}
			}
			else
			{
				// 整理归还内存
				m_FreeList.Push(m_bAutoOrder ? Order(pBlockUse) : pBlockUse);
				ExAssert(!m_FreeList.Empty());
			}
		}
	}
	// 整理内存
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
				// 与前一个结点合并, 并销毁不需要的内存块
				m_BlockAlloc.Free(m_FreeList.Merger(pItem, pPrev->pConn, true), true);
				m_BlockAlloc.Free(m_BlocList.Merger(pConn, pPrev, true), true);
				// 更新游标
				pConn = pPrev;
				pItem = pConn->pConn;
				bOrder = true;
			}
			if(pNext && !pNext->bHead && pNext->bUsed == false )
			{
				ExAssert(pNext->pConn);
				// 与后一个结点合并, 并销毁不需要的内存块
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
					// 合并空闲内存块, 并销毁不需要的内存块
					m_BlockAlloc.Free(m_FreeList.Merger(pItem->pNext->pConn, pItem->pConn, true), true);
					m_BlockAlloc.Free(m_BlocList.Merger(pItem->pNext, pItem, true), true);
				}
				pItem = pItem->pNext;
			} while( pItem != m_BlocList.Head() );
		}
		return NULL;
	}
	// 清空内存池
	void Clear()
	{
		block_t* pItemTemp = NULL;
		block_t* pItem = NULL;
		ExLock(m_Mutex, false, mutex_t);
		// 清理内存链表
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
		// 清理空闲链表
		if( !m_FreeList.Empty() )
		{
			pItem = m_FreeList.Head();
			do
			{	// 空闲链表不需要清理内存数据
				pItemTemp = pItem->pNext;
				m_BlockAlloc.Free(pItem, true);
				pItem = pItemTemp;
			} while( pItem != m_FreeList.Head() );
		}
		// 清理使用链表
		if( !m_UsedList.Empty() )
		{
			pItem = m_UsedList.Head();
			do
			{	// 使用链表不需要清理内存数据
				pItemTemp = pItem->pNext;
				m_BlockAlloc.Free(pItem, true);
				pItem = pItemTemp;
			} while( pItem != m_UsedList.Head() );
		}
		// 链表置空
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

	static const DWORD	s_nDefSize = 1024 * 1024;		// 初始大小1M
	static const DWORD	s_nMaxSize = 1024 * 1024 * 100;	// 最大大小100M
	static const bool	s_bAutoOrder = true;			// 自动整理内存池
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__MemPool_h__*/