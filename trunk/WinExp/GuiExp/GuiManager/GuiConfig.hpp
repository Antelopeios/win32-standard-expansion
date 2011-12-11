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
// GuiConfig - 界面配置文件接口
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-05-31
// Version:	1.0.0000.1714
//
// History:
//	- 1.0.0000.1714(2011-05-31)	@ 开始构建GuiConfig
//////////////////////////////////////////////////////////////////

#ifndef __GuiConfig_hpp__
#define __GuiConfig_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiConfig
{
public:
	// 摘要
	struct detail_t
	{
		CString name;		// 名称
		CString author;		// 作者
		CString home;		// 主页
		CString email;		// 邮箱
		CString version;	// 版本
		CString date;		// 日期
	};

protected:
	CGuiXML	 m_XML;
	detail_t m_Detail;

public:
	CGuiConfig()
	{}
	virtual ~CGuiConfig()
	{}

protected:
	BOOL CreateUI()
	{
	}

public:
	BOOL Load(IFileObject* pFile)
	{
		if (!pFile) return FALSE;
		m_XML.SetFile(pFile);
		if (!m_XML.Decode()) return FALSE;
		CGuiXML::iterator_t ite;
		if (!m_XML.GetNode(_T("detail"), ite)) return FALSE;
		// 开始读取配置文件内容
		m_Detail.name	 = m_XML.GetAttr(_T("name"),	ite);
		m_Detail.author	 = m_XML.GetAttr(_T("author"),	ite);
		m_Detail.home	 = m_XML.GetAttr(_T("home"),	ite);
		m_Detail.email	 = m_XML.GetAttr(_T("email"),	ite);
		m_Detail.version = m_XML.GetAttr(_T("version"),	ite);
		m_Detail.date	 = m_XML.GetAttr(_T("date"),	ite);
		// 开始创建界面对象
		return TRUE;
	}

	const detail_t& GetDetail() { return m_Detail; }
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiConfig_hpp__*/