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
// Singleton - 单例
//
// Author:	木头云
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-11
// Version:	1.0.0006.1909
//
// History:
//	- 1.0.0005.1710(2011-05-03)	# 采用懒汉方式实现CSingletonT::Instance(),避免出现全局变量之间的构造顺序冲突
//	- 1.0.0006.1909(2011-05-11)	= 重命名CSingletonT为ISingletonT
//////////////////////////////////////////////////////////////////

#ifndef __Singleton_h__
#define __Singleton_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Thread/Lock.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

template <typename TypeT>
class ISingletonT
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

template <typename TypeT>
EXP_INLINE TypeT& ExSingleton() { return ISingletonT<TypeT>::Instance(); }

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__Singleton_h__*/