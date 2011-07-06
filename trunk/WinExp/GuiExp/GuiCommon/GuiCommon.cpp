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
// GuiCommon - 界面拓展库公用定义
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-06-01
// Version:	1.0.0004.1130
//
// History:
//	- 1.0.0003.1354(2011-05-19)	= 调整ExGui()的内部实现,始终在模块内构造对象
//	- 1.0.0004.1130(2011-06-01)	= 将DllMain()入口点放入GuiCommon.cpp内
//////////////////////////////////////////////////////////////////

#include "GuiCommon.h"

#ifdef _DEBUG
#pragma comment(lib, "../../_Output/Debug/StdExp.lib")
#else
#pragma comment(lib, "../../_Output/Release/StdExp.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "../../_Output/Debug/ImgExp.lib")
#else
#pragma comment(lib, "../../_Output/Release/ImgExp.lib")
#endif

EXP_BEG

//////////////////////////////////////////////////////////////////

// 通用对象创建接口
EXP_API IGuiObject* ExGui(LPCTSTR sGuiType, CGC* pGC/* = NULL*/)
{
	return ExDynCast<IGuiObject>(ExDynCreate(sGuiType, pGC));
}

//////////////////////////////////////////////////////////////////

// GuiExp Dll 入口点
BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD ul_reason_for_call,
					  LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////

EXP_END
