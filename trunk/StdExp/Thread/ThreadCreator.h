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
// ThreadCreator - �̴߳�����
//
// Author:	ľͷ��
// Blog:	http://www.cnblogs.com/black-cat-ptr/
// E-Mail:	mark.lonr@tom.com
// Date:	2011-02-23
// Version:	1.0.0001.1124
//////////////////////////////////////////////////////////////////

#ifndef __ThreadCreator_h__
#define __ThreadCreator_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

// ����ͨ�̷߳���ת��Ϊ��̬�̷߳����������
template <typename CreatorT = _ThreadHeap>
class CThreadAdapterT : public CSingletonT<CreatorT>
{
public:
	typedef CreatorT creator_t;
	typedef typename creator_t::handle_t handle_t;
	typedef typename creator_t::call_t call_t;

	EXP_INLINE static creator_t& GetCreator()
	{ return Instance(); }

public:
	static handle_t Create(_IN_ call_t lpStartAddr, 
						   _IN_ LPVOID lpParam = NULL, 
						   _IN_ DWORD dwFlag = 0, 
						   _OT_ LPDWORD lpIDThread = NULL)
	{ return GetCreator().Create(lpStartAddr, lpParam, dwFlag, lpIDThread); }
	static bool Close(handle_t hTrd)
	{ return GetCreator().Close(hTrd); }

	static DWORD Suspend(handle_t hTrd)
	{ return GetCreator().Suspend(hTrd); }
	static DWORD Resume(handle_t hTrd)
	{ return GetCreator().Resume(hTrd); }

	static bool Terminate(handle_t hTrd, DWORD dwExitCode = 0)
	{ return GetCreator().Terminate(hTrd, dwExitCode); }
};

#ifndef EXP_THREAD_CREATOR
#define EXP_THREAD_CREATOR CThreadAdapterT<>
#endif/*EXP_THREAD_CREATOR*/

typedef EXP_THREAD_CREATOR ExThread;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__ThreadCreator_h__*/