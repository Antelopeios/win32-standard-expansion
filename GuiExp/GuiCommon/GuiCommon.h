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
// GuiCommon - ������չ�⹫�ö���
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-19
// Version:	1.0.0003.1553
//
// History:
//	- 1.0.0001.1135(2011-05-04)	+ ���wnd_t���Ͷ���
//	- 1.0.0002.1506(2011-05-11)	- �Ƴ�ATLThunk,����GWL_USERDATA��ʽ·�ɴ��ڹ���
//	- 1.0.0003.1553(2011-05-19)	^ �û���Ĭ�ϵĵ���,�ڴ����ȶ���ͳһ�ڱ�ģ�������
//////////////////////////////////////////////////////////////////

#ifndef __GuiCommon_h__
#define __GuiCommon_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

// Dll �����궨��
#ifdef EXP_EXPORTS
#define EXP_API __declspec(dllexport)
#else
#define EXP_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////

#include "../StdExp/Common/Common.h"
#include "../StdExp/Thread/Lock.h"

EXP_BEG

// ģ���ڲ���������
template <typename TypeT>
class EXP_API IGuiSingletonT
{
public:
	EXP_INLINE static TypeT& Instance()
	{
		static TypeT* instance = NULL;
		if (instance == NULL)
		{
			ExLockThis();
			if (instance == NULL)
			{
				static TypeT type;
				instance = &type;
			}
		}
		return (*instance);
	}
};

EXP_END

//////////////////////////////////////////////////////////////////

// ͼ�����
#define EXP_SINGLETON IGuiSingletonT // �û���Ĭ�ϵĵ���,�ڴ����ȶ���ͳһ�ڱ�ģ�������
#include "../ImgExp/ImgExp.h"

//////////////////////////////////////////////////////////////////

EXP_BEG

// ���Ͷ���

typedef HWND		wnd_t;

// ���ܶ���

#define ExGetX(lp)	((int)(short)LOWORD(lp))
#define ExGetY(lp)	((int)(short)HIWORD(lp))

EXP_END

//////////////////////////////////////////////////////////////////

// �����ӿڶ���
#include "GuiCommon/GuiInterface.h"

//////////////////////////////////////////////////////////////////

EXP_BEG

// ͨ�ö��󴴽��ӿ�
EXP_API IGuiObject* ExGui(LPCTSTR sGuiType, CGC* pGC = NULL);

EXP_END

//////////////////////////////////////////////////////////////////

#endif/*__GuiCommon_h__*/