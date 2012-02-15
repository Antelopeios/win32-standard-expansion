// Copyright 2011-2012, 木头云
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
// Singleton - 单例
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-28
// Version:	1.0.0008.1430
//
// History:
//	- 1.0.0005.1710(2011-05-03)	# 采用懒汉方式实现CSingletonT::Instance(),避免出现全局变量之间的构造顺序冲突
//	- 1.0.0006.1909(2011-05-11)	= 重命名CSingletonT为ISingletonT
//	- 1.0.0007.1554(2011-05-19)	+ StdExp内部的单例调用支持由外部统一置换
//	- 1.0.0008.1430(2012-01-28)	+ ISingletonT支持外部传入ModelT作为内部实现的线程模型策略
//////////////////////////////////////////////////////////////////

#ifndef __Singleton_h__
#define __Singleton_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Thread/Lock.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename TypeT, typename ModelT = EXP_THREAD_MODEL>
interface ISingletonT
{
public:
	EXP_INLINE static TypeT& Instance()
	{
		static BOOL is_init = FALSE;
		if(!is_init)
		{
			is_init = TRUE;
			ISingletonT<CPtrManager, ModelT>::Instance();
		}
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

#ifndef EXP_SINGLETON
#define EXP_SINGLETON EXP::ISingletonT
#endif/*EXP_SINGLETON*/

template <typename TypeT>
EXP_INLINE TypeT& ExSingleton() { return EXP_SINGLETON<TypeT>::Instance(); }

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Singleton_h__*/