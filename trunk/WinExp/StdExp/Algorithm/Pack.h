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
// Pack - 通用数据包算法
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-10-17
// Version:	1.0.0001.1032
//////////////////////////////////////////////////////////////////

#ifndef __Pack_h__
#define __Pack_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Algorithm/CRC.h"
#include "Container/List.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CPack
{
public:
	// 数据类型
	enum typ_t
	{
		typ_byte, 
		typ_short, 
		typ_int, 
		typ_long, 
		typ_double, 
		typ_64, 
	};

	// 错误类型
	enum err_t
	{
		err_none, 
		err_check, 
		err_head, 
		err_magic, 
		err_version, 
		err_crc, 
	};

	static const uint32_t MAGIC = 0x20111017;	// 2011.10.17
	static const uint32_t VERSION = 0;

#pragma pack(1)
	// 包头
	struct head_t
	{
		uint32_t magic;
		uint32_t version;
		uint32_t data_count;
		uint32_t reserved;
		uint32_t size;	// 不包括头部大小
		uint32_t crc;
	};
	// 数据
	struct data_t
	{
		uint8_t type;
		uint32_t size;
	};
#pragma pack()

protected:
	err_t m_Error;
	BYTE* m_Pack;
	DWORD m_CurSize, m_BufSize, m_CurPos;

public:
	CPack()
		: m_Error(err_none)
		, m_Pack(NULL)
		, m_CurSize(0)
		, m_BufSize(0)
		, m_CurPos(0)
	{
		Init();
	}
	CPack(CPack& pack)
	{
		this->CPack::CPack();
		operator=(pack);
	}

	virtual ~CPack()
	{
		Term();
	}

public:
	EXP_INLINE void Init()
	{
	//	if (m_Pack) return;
	}
	EXP_INLINE void Term()
	{
		if(!m_Pack) return;
		del(m_Pack);
		m_Pack = NULL;
		m_Error = err_none;
		m_CurPos = m_BufSize = m_CurSize = 0;
	}

	CPack& operator=(CPack& pack)
	{
		Term();

		m_BufSize = m_CurSize = pack.GetPackSize();
		m_Pack = dbnew(BYTE, m_BufSize);

		memcpy(m_Pack, pack.GetPack(), sizeof(head_t));
		memcpy(m_Pack + sizeof(head_t), pack.GetPack() + sizeof(head_t) + pack.m_CurPos, GetDataSize());

		return (*this);
	}
	CPack* Clone()
	{
		CPack* pack = dbnew(CPack);
		*pack = *this;
		return pack;
	}

	EXP_INLINE BYTE* GetPack()
	{
		return m_Pack;
	}
	EXP_INLINE DWORD GetPackSize()
	{
		return sizeof(head_t) + GetDataSize();
	}
	EXP_INLINE BOOL SetPackCRC()
	{
		if (Check() != err_none) return FALSE;
		head_t* head = (head_t*)m_Pack;
		head->crc = ExCRC32((unsigned char*)(head + 1), head->size);
		return TRUE;
	}

	err_t Check(BYTE* pPack = NULL, BOOL bCheckCRC = FALSE)
	{
		if (!pPack)
		{
			pPack = m_Pack;
			if (!pPack) return m_Error = err_check;
			if (m_CurSize < sizeof(head_t))
				return m_Error = err_head;
		}
		head_t* head = (head_t*)pPack;
		if (head->magic != MAGIC)
			return m_Error = err_magic;
		if (head->version != VERSION)
			return m_Error = err_version;
		if (head->data_count == 0)
			return m_Error = err_check;
		if (head->reserved != 0)
			return m_Error = err_check;
		if (head->size <= sizeof(data_t))
			return m_Error = err_check;
		if (bCheckCRC && head->crc != ExCRC32((unsigned char*)(head + 1), head->size))
			return m_Error = err_crc;
		return m_Error = err_none;
	}
	EXP_INLINE err_t GetError()
	{
		err_t err = m_Error;
		m_Error = err_none;
		return err;
	}

	EXP_INLINE DWORD GetVersion()
	{
		if (!m_Pack)
		{
			m_Error = err_check;
			return 0;
		}
		return ((head_t*)m_Pack)->version;
	}
	EXP_INLINE DWORD GetDataCount()
	{
		if (!m_Pack)
		{
			m_Error = err_check;
			return 0;
		}
		return ((head_t*)m_Pack)->data_count;
	}
	EXP_INLINE DWORD GetDataSize()
	{
		if (!m_Pack)
		{
			m_Error = err_check;
			return 0;
		}
		return ((head_t*)m_Pack)->size;
	}

	BOOL PushData(typ_t type, void* data, size_t size)
	{
		if (!m_Pack)
		{
			m_BufSize = sizeof(head_t);
			m_Pack = dbnew(BYTE, m_BufSize);
			head_t* head = (head_t*)m_Pack;
			head->magic = MAGIC;
			head->version = VERSION;
			head->data_count = 0;
			head->reserved = 0;
			head->size = 0;
			head->crc = 0;
			m_CurSize = sizeof(head_t);
		}
		// 拓展内存块
		uint32_t old_size = GetPackSize();
		uint32_t new_size = old_size + sizeof(data_t) + size;
		if (m_BufSize < new_size)
		{
			m_BufSize = (new_size << 1);
			m_Pack = renew(m_Pack, BYTE, m_BufSize);
		}
		// 写入新数据
		data_t* new_data = (data_t*)(m_Pack + old_size);
		new_data->type = (uint8_t)type;
		new_data->size = size;
		memcpy(new_data + 1, data, size);
		// 改写头部信息
		head_t* head = (head_t*)m_Pack;
		head->size += (sizeof(data_t) + size);
		head->data_count += 1;
		m_CurSize = sizeof(head_t) + head->size;
		return TRUE;
	}
	BOOL PushData(CPack* pPack)
	{
		if (!pPack) return FALSE;
		typ_t t; size_t s = 0; void* b = NULL;
		while(pPack->PopData(t, s, b)) PushData(t, b, s);
		return TRUE;
	}
	BOOL PopData(typ_t& type, size_t& size, void*(& data))
	{
		if (!m_Pack)
		{
			m_Error = err_check;
			return FALSE;
		}
		head_t* head = (head_t*)m_Pack;
		if (head->size <= sizeof(data_t))
		{
			m_Error = err_check;
			return FALSE;
		}
		if (head->data_count == 0)
		{
			m_Error = err_check;
			return FALSE;
		}
		// 得到数据
		data_t* pop_data = (data_t*)(((BYTE*)(head + 1)) + m_CurPos);
		type = (typ_t)pop_data->type;
		size = pop_data->size;
		data = pop_data + 1;
		// 改写头部信息
		int pop_size = sizeof(data_t) + size;
		head->size -= pop_size;
		head->data_count -= 1;
		m_CurPos += pop_size;
		return TRUE;
	}
	BOOL PopData(typ_t type, void* buff, size_t size)
	{
		typ_t t; size_t s = 0; void* b = NULL;
		if (!PopData(t, s, b)) return FALSE;
		if (t != type || s == 0) return FALSE;
		switch(type)
		{
		case typ_byte:
		case typ_short:
		case typ_int:
		case typ_long:
		case typ_double:
		case typ_64:
			memcpy_s(buff, size, b, s);
			break;
		default:
			return FALSE;
		}
		return TRUE;
	}

	EXP_INLINE BOOL IsComplete()
	{
		return (GetDataSize() > sizeof(data_t)) 
			&& (GetPackSize() == m_CurSize) 
			&& (Check(NULL, TRUE) == err_none);
	}

	// 根据输入的chip对网络消息进行补全,返回本次补全使用到chip的字节数
	// 例如,chip有1024字节,但是CPack本身仅剩下24个字节没有补全,则Patch将返回24
	DWORD Patch(BYTE* chip, DWORD size)
	{
		if (!chip || size == 0 || IsComplete()) return 0;

		if (m_CurSize + size < sizeof(head_t))
		{	// 现有获得的数据小于sizeof(head_t),此时我们很难通过头部进行校验,或者获得整个消息的大小
			if(!m_Pack)
			{
				m_BufSize = sizeof(head_t);
				m_Pack = dbnew(BYTE, m_BufSize);
			}
			memcpy(m_Pack + m_CurSize, chip, size);
			m_CurSize += size;
			return size;
		}
		else
		if (0 == m_CurSize)
		{	// 这是第一个包,且已经超过了sizeof(head_t)
			head_t* head = (head_t*)chip;
			if (Check((BYTE*)head) != err_none)
			{
				Term();
				return 0;
			}
			m_BufSize = sizeof(head_t) + head->size;
			m_Pack = renew(m_Pack, BYTE, m_BufSize);
			m_CurSize = min(m_BufSize, size);
			memcpy(m_Pack, chip, m_CurSize);
			return m_CurSize;
		}
		else
		{	// 现有数据已经超过了sizeof(head_t),而且不是第一个包了
			DWORD un_copy = 0;
			head_t* head = (head_t*)m_Pack;
			// 之前已经积累了一些数据,但是尚未达到识别head_t的地步,需要重新分配m_Pack
			if (m_CurSize < sizeof(head_t))
			{
				un_copy = sizeof(head_t) - m_CurSize;
				memcpy(m_Pack + m_CurSize, chip, un_copy);
				if (Check((BYTE*)head) != err_none)
				{
					Term();
					return 0;
				}
				BYTE* old_pack = m_Pack;
				m_BufSize = sizeof(head_t) + head->size;
				m_Pack = renew(m_Pack, BYTE, m_BufSize);
			}
			// 正确的MsgSize已经被识别出来了,现在仅仅是补充数据而已
			un_copy = min(size, sizeof(head_t) + head->size - m_CurSize);
			memcpy(m_Pack + m_CurSize, chip, un_copy);
			m_CurSize += un_copy;
			return un_copy;
		}
	}
};

//////////////////////////////////////////////////////////////////

class CPackList
{
public:
	typedef CListT<CPack*> list_t;

protected:
	list_t m_PackList;
	CMutex m_PackLock;
	CPack* m_PackLast;

public:
	CPackList(void)
		: m_PackLast(NULL)
	{
	}
	virtual ~CPackList(void)
	{
		if (m_PackLast)
		{
			del(m_PackLast);
			m_PackLast = NULL;
		}
		for(list_t::iterator_t ite = m_PackList.Head(); ite != m_PackList.Tail(); ++ite)
			if (*ite) del(*ite);
	}

public:
	void PushPack(CPack* pack)
	{
		if (!pack) return;
		ExLock(m_PackLock);
		m_PackList.PushLast(pack->Clone());
	}
	CPack* PopPack()
	{
		ExLock(m_PackLock);
		if (m_PackLast) del(m_PackLast);
		if (m_PackList.Empty())
			m_PackLast = NULL;
		else
		{
			m_PackLast = m_PackList.HeadItem();
			m_PackList.PopHead();
		}
		return m_PackLast;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Pack_h__*/
