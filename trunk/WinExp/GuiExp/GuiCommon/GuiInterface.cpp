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
// GuiInterface - 界面公用接口
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-06-08
// Version:	1.0.0004.0047
//
// History:
//	- 1.0.0001.1730(2011-05-05)	= GuiInterface里仅保留最基本的公共接口
//	- 1.0.0002.1000(2011-05-19)	+ 添加IGuiBase界面对象基础类实现
//	- 1.0.0003.1000(2011-05-23)	+ 添加IGuiEffect效果对象基础类实现
//	- 1.0.0004.0047(2011-06-08)	^ 将IGuiBase移出并单独实现
//////////////////////////////////////////////////////////////////

#include "GuiCommon/GuiCommon.h"
#include "GuiInterface.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNAMIC_CLS(IGuiObject, IBaseObject)
EXP_IMPLEMENT_DYNAMIC_CLS(IGuiEvent, IGuiObject)
IGuiCtrl* IGuiEvent::s_MLMove = NULL;
EXP_IMPLEMENT_DYNAMIC_CLS(IGuiSender, IGuiObject)
EXP_IMPLEMENT_DYNAMIC_CLS(IGuiComp, IGuiObject)
EXP_IMPLEMENT_DYNAMIC_CLS(IGuiEffect, IGuiObject)

//////////////////////////////////////////////////////////////////

EXP_END
