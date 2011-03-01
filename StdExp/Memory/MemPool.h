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
// Date:	2011-03-01
// Version:	1.2.0016.2345
//
// History:
//	- 1.2.0016.2345(2011-03-01)	^ 改进MemPool的内部实现方式,简化逻辑,优化算法
//								+ MemPool支持Clear时进行简单的内存泄漏检测
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
	// 块链表基类
	typedef class CBlockList
	{
	public:
		// 块结构基类
		typedef struct _Block
		{
			_Block*	pPrev;	// 上一个结点
			_Block*	pNext;	// 下一个结点
		} block_t;

		enum { HeadSize = sizeof(block_t) };

	protected:
		block_t* m_pList;	// 链表头指针
		DWORD	 m_nCont;	// 结点个数

	public:
		CBlockList()
			: m_pList(NULL)
			, m_nCont(0)
		{ }
		~CBlockList()
		{ /*析构不做任何清理工作*/ }

	public:
		block_t* Push(block_t* pItem, block_t* pLast = NULL)
		{	// 默认从尾部插入结点
			if( !pItem ) return NULL;
			if( (pItem->pPrev && pItem->pPrev->pNext == pItem) && 
				(pItem->pNext && pItem->pNext->pPrev == pItem) ) return NULL;
			if( pLast == pItem ) pLast = NULL;
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
			++m_nCont;
			return pItem;
		}
		block_t* Pop(block_t* pItem = NULL)
		{	// 默认从头部弹出结点
			if(!pItem ) pItem = m_pList;
			if( pItem )
			{	// 弹出结点
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
		{	// 直接置空,不做任何清理动作
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

	// 内存块链表
	typedef class CMemList : public blk_list_t
	{
	public:
		// 内存块结构
		typedef struct _MemBlock : public fre_block_t
		{
			DWORD	nSize;		// 内存块大小
			bool	bHead;		// 是否为内存块头
			bool	bUsed;		// 是否正在被使用
			fre_block_t fBlock;	// 空闲链表结点
		} block_t;

		enum
		{
			HeadSize = sizeof(block_t), 
			MemBSize = HeadSize - blk_list_t::HeadSize
		};

	protected:
		DWORD	 m_nSize;	// 链表大小

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
			// 默认从头部分割结点
			if(!pItem ) return NULL;
			block_t* cut_pblk = NULL;
			if (pItem->nSize > ((HeadSize << 1) + nCutSize))	// 切割时不能残留过于细小的内存碎片
			{
				// 从现有内存块的尾部切割内存块
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
			// 合并到后面
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

	// 结点内存分配器
	typedef class CBlockAlloc
	{
	protected:
		alloc_t m_Alloc;

	public:
		// 获得内存记录块对应的指针
		void* PtrReal(void* pPtr)
		{ return pPtr ? (((BYTE*)pPtr) + mem_list_t::HeadSize) : NULL; }
		void* PtrBlock(mem_block_t* pPtr)
		{ return PtrReal((void*)pPtr); }

		// 获得指针对应的内存记录块
		void* RealPtr(void* pPtr)
		{ return pPtr ? (((BYTE*)pPtr) - mem_list_t::HeadSize) : NULL; }
		mem_block_t* BlockPtr(void* pPtr)
		{ return (mem_block_t*)RealPtr(pPtr); }

		// 空闲记录块与内存记录块的转换
		mem_block_t* BlockFre(fre_block_t* pFre)
		{ return pFre ? (mem_block_t*)(((BYTE*)pFre) - mem_list_t::MemBSize) : NULL; }
		fre_block_t* FreBlock(mem_block_t* pBlk)
		{ return pBlk ? &(pBlk->fBlock) : NULL; }

		// 内存校验
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

		// 内存分配
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
	mutex_t		m_Mutex;	// 线程同步互斥对象
	blk_alloc_t m_Alloc;	// 内存分配器

	CMemList	m_MemList;	// 内存记录块链表
	CBlockList	m_FreList;	// 空闲记录块链表

	DWORD		m_nMaxSize;	// 内存池大小上限

public:
	CMemPoolT(DWORD nSize = PolicyT::s_nDefSize)
		: m_nMaxSize(PolicyT::s_nMaxSize)
	{ SetPoolSize(nSize); }
	~CMemPoolT()
	{ Clear(PolicyT::s_bDumpMemLeaks); }

public:
	// 内存池大小设置
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
		// 分配新的内存块
		mem_block_t* block = m_Alloc.Alloc(nSize - m_MemList.Size());
		// 添加内存记录块
		m_MemList.Push(block);
		// 添加空闲记录块
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

	// 内存效验
	bool Valid(void* pPtr)
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_Alloc.Valid(pPtr);
	}
	// 内存大小
	DWORD Size(void* pPtr)
	{
		ExLock(m_Mutex, true, mutex_t);
		return m_Alloc.Size(pPtr);
	}
	// 分配内存
	void* Alloc(DWORD nSize)
	{
		if (nSize == 0) return NULL;
		mem_block_t* block = NULL;
		ExLock(m_Mutex, false, mutex_t);
		// 查找空闲内存块
		if( !m_FreList.Empty() )
		{
			fre_block_t* item = m_FreList.Head();
			do
			{
				mem_block_t* temp = m_Alloc.BlockFre(item);
				if( temp->nSize >= nSize )
				{	// 找到合适的内存块
					block = temp;
					break;
				}
				item = item->pNext;
			} while( item != m_FreList.Tail() );
		}
		// 标记空闲内存块
		if( block )
		{	// 找到合适的内存块
			if( block->nSize > nSize ) // 内存块需要分割
				block = m_MemList.Cut(nSize, block);
			m_FreList.Pop(m_Alloc.FreBlock(block));
		} else
		{	// 分配新的内存块
			block = m_Alloc.Alloc(nSize);
			// 添加内存记录块
			m_MemList.Push(block);
		}
		ExAssert(block && !(block->bUsed));
		if(!block || block->bUsed) return NULL;
		block->bUsed = true;
		// 返回分配的内存
		return m_Alloc.PtrBlock(block);
	}
	// 回收内存
	void Free(void* pPtr)
	{
		if (!pPtr) return;
		ExLock(m_Mutex, false, mutex_t);
		// 获得对应内存块
		mem_block_t* block = m_Alloc.BlockPtr(pPtr);
		ExAssert(block && block->bUsed);
		if(!block || !(block->bUsed)) return;
		// 标记使用内存块
		block->bUsed = false;
		// 释放空间
		if( m_MemList.Size() > m_nMaxSize )
		{	// 释放掉能够释放的内存块
			if( block->bHead )
			{
				m_MemList.Pop(block);
				m_Alloc.Free(block);
			}
		} else
		{
			// 整理归还内存
			mem_block_t* prev = m_MemList.Merger(block);	// 向前合并
			if (prev)
				block = prev;
			else
				m_FreList.Push(m_Alloc.FreBlock(block));
			mem_block_t* next = static_cast<mem_block_t*>(block->pNext);
			if (m_MemList.Merger(next))						// 向后合并
				m_FreList.Pop(m_Alloc.FreBlock(next));
		}
	}

#ifdef	_DEBUG
#ifndef	EXP_DUMPING_MEMLEAKS
#define	EXP_DUMPING_MEMLEAKS
#endif/*EXP_DUMPING_MEMLEAKS*/
#endif/*_DEBUG*/

	// 清空内存池
	void Clear(bool bDump = true)
	{
		ExLock(m_Mutex, false, mutex_t);
		// 清理内存记录链表
#ifdef	EXP_DUMPING_MEMLEAKS
		DWORD dump_counter = 0;
		if (bDump)
			ExDPrintf(_T("\nDumping memory leaks...\n"));
#endif/*EXP_DUMPING_MEMLEAKS*/
		if( !m_MemList.Empty() )
		{
			// 第一次遍历,移除非头结点
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
			// 第二次遍历,清理数据
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

	static const DWORD	s_nDefSize = 1024 * 1024;		// 初始大小1M
	static const DWORD	s_nMaxSize = 1024 * 1024 * 100;	// 最大大小100M
	static const bool	s_bDumpMemLeaks = true;
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__MemPool_h__*/