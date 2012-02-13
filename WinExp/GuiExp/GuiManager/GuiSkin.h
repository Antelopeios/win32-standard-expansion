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
// CGuiSkin - 皮肤加载
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-02-13
// Version:	1.0.0002.1734
//
// History:
//	- 1.0.0000.1714(2011-05-31)	@ 开始构建GuiConfig
//	- 1.0.0001.0945(2012-01-31)	= GuiConfig改名为CGuiSkin
//	- 1.0.0002.1734(2012-02-13)	+ 添加皮肤包加载功能
//////////////////////////////////////////////////////////////////

#ifndef __GuiSkin_h__
#define __GuiSkin_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

EXP_CLASS CGuiSkin
{
protected:
	static CArrayT<void*> m_NewDiv;
	static CArrayT<void*> m_NedDel;
	static CArrayT<void*> m_LstDiv;

protected:
	static void Exec(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent = NULL);

public:
	// 以下为皮肤脚本加载
	static BOOL Parse(IFileObject* pFile);
	static BOOL Parse(const CString& script);
	// 以下为皮肤包的加载
	static BOOL Load(CXZip& zip);
	static BOOL Load(const CString& path);
	static BOOL Load(BYTE* buff, DWORD size);
	// 删除目录
	static BOOL DelPath(LPCTSTR sPath);
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiSkin_h__*/