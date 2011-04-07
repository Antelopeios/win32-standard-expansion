// Copyright 2010, 木头云
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
// TargetVer - Win32 平台版本定义
//
// Author:	木头云
// Blog:	http://www.cnblogs.com/black-cat-ptr/
// E-Mail:	mark.lonr@tom.com
// Date:	2010-08-21
// Version:	1.1.0002.2030
//////////////////////////////////////////////////////////////////

#ifndef __TargetVer_h__
#define __TargetVer_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////

// 以下宏定义要求的最低平台。要求的最低平台
// 是具有运行应用程序所需功能的 Windows、Internet Explorer 等产品的
// 最早版本。通过在指定版本及更低版本的平台上启用所有可用的功能，宏可以
// 正常工作。

// 如果必须要针对低于以下指定版本的平台，请修改下列定义。
// 有关不同平台对应值的最新信息，请参考 MSDN。

#ifndef _WIN32_WCE

#ifndef WINVER					// 指定要求的最低平台是 Windows XP。
#define WINVER			0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT			// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT	0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS			// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS	0x0410	// 将此值更改为适当的值，以指定将 Windows Me 或更高版本作为目标。
#endif

#ifndef _WIN32_IE				// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE		0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。
#endif

#else /*_WIN32_WCE*/

// 注意 - 这个值与作为目标的 Windows CE OS 版本的关联性并不强
#ifndef WINVER
#define WINVER _WIN32_WCE
#endif

// 屏蔽 _CE_ACTIVEX 编译警告
#define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA

#endif/*_WIN32_WCE*/

//////////////////////////////////////////////////////////////////

#endif/*__TargetVer_h__*/
