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
// GuiManager - ���������
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-08-01
// Version:	1.0.0001.0440
//
// History:
//	- 1.0.0000.1622(2011-06-13)	@ ��ʼ����GuiManager�ӿ�
//	- 1.0.0001.0440(2011-08-01)	= ��GuiManager��Ϊģ����ʽ,�ṩ���ͻ��Ľ���Ԫ�ع���
//////////////////////////////////////////////////////////////////

#ifndef __GuiManager_h__
#define __GuiManager_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCommon/GuiCommon.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename TypeT, const DWORD SizeT = 101>
class CGuiManagerT
{
private:
	typedef CMapT<CString, TypeT*> key_map_t;
	EXP_INLINE static key_map_t& Instance()
	{
		static key_map_t* instance = NULL;
		if (instance == NULL)
		{
			ExLockThis();
			if (instance == NULL)
			{
				static key_map_t type(SizeT);
				instance = &type;
			}
		}
		return (*instance);
	}

public:
	static bool Reg(LPCTSTR c_key, TypeT* inf)
	{
		if (!c_key) return false;
		key_map_t& types = Instance();
		CString key(c_key);
		if (types.Locate(key) == types.Tail())
		{
			types.Add(key, inf);
			return true;
		}
		else
			return false;
	}
	static TypeT* Get(LPCTSTR c_key)
	{
		if (!c_key) return false;
		key_map_t& types = Instance();
		CString key(c_key);
		key_map_t::iterator_t ite = types.Locate(key);
		if (ite == types.Tail())
			return NULL;
		else
			return ite->Val();
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiManager_h__*/