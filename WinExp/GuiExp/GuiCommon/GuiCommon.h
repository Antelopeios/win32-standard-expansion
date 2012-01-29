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
// GuiCommon - ������չ�⹫�ö���
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-20
// Version:	1.0.0008.1712
//
// History:
//	- 1.0.0001.1135(2011-05-04)	+ ���wnd_t���Ͷ���
//	- 1.0.0002.1506(2011-05-11)	- �Ƴ�ATLThunk,����GWL_USERDATA��ʽ·�ɴ��ڹ���
//	- 1.0.0003.1553(2011-05-19)	^ �û���Ĭ�ϵĵ���,�ڴ����ȶ���ͳһ�ڱ�ģ�������
//	- 1.0.0004.1616(2011-06-08)	^ ��Ĭ�ϵ�Normal�˾��û�ΪCFilterOverlay,����ؼ���ͼ
//	- 1.0.0005.1438(2011-07-06)	^ �򻯽ṹ,ͳһEXP_API��ص���/�ṹ/�ӿ�����
//	- 1.0.0006.0120(2011-07-07)	= �ر���StdExp��RTTI�Ķ������C4275����
//	- 1.0.0007.1540(2011-07-20)	+ ���ExGC()�ӿ�,���ⲿ��ȡȫ��GC,��ֹ����DLL����ȫ��GC�������ڲ�һ�µ��µ������쳣
//	- 1.0.0007.1540(2012-01-20)	- �Ƴ�ExGC()�ӿ�,ʹ�ö����໥�йܷ�ʽ���GC����˳���µ��쳣
//////////////////////////////////////////////////////////////////

#ifndef __GuiCommon_h__
#define __GuiCommon_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

// �ر�һЩ����
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

// Dll �����궨��
#ifdef EXP_EXPORTS
#define EXP_API __declspec(dllexport)
#else
#define EXP_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////

// �û���Ĭ�ϵ�ȫ�ֵ���
#include "Common/Common.h"
#include "Thread/Lock.h"
EXP_BEG
template <typename TypeT, typename ModelT = EXP_THREAD_MODEL>
EXP_INTERFACE IGuiSingletonT
{
public:
	EXP_INLINE static TypeT& Instance()
	{
		static TypeT* instance = NULL;
		if (instance == NULL)
		{
			ExLockThis(typename ModelT::_LockPolicy);
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
#define EXP_SINGLETON IGuiSingletonT

// ͼ�����
#include "ImgExp.h"

//////////////////////////////////////////////////////////////////

// ����/���ܶ���
EXP_BEG
typedef HWND		wnd_t;
#define ExGetX(lp)	((int)(short)LOWORD(lp))
#define ExGetY(lp)	((int)(short)HIWORD(lp))
EXP_END

// �����ӿڶ���
#include "GuiCommon/GuiInterface.h"

EXP_BEG

// ͨ�ö��󴴽��ӿ�
EXP_API IGuiObject* ExGui(LPCTSTR sGuiType, CGC* pGC = NULL);

EXP_END

// Gui �����ӿڶ���
#include "GuiCommon/GuiBase.h"

//////////////////////////////////////////////////////////////////

#endif/*__GuiCommon_h__*/