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
// Date:	2012-03-02
// Version:	1.0.0003.1130
//////////////////////////////////////////////////////////////////

#include "GuiCommon/GuiCommon.h"
#include "GuiSkin.h"
#include "GuiManager/Executors/Executor.h"
#include "GuiBoard/GuiBoard.h"

#include <ShellApi.h>

EXP_BEG

//////////////////////////////////////////////////////////////////

CArrayT<void*> CGuiSkin::m_NewDiv;
CArrayT<void*> CGuiSkin::m_NedDel;
CArrayT<void*> CGuiSkin::m_LstDiv;

void CGuiSkin::Exec(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent/* = NULL*/)
{
	IExecutor* exc = CExecutor::Get((*ite)->nam);
	void* ret = NULL;
	if (exc)
	{
		ret = exc->Execute(xml, ite, parent);
		IGuiWnd* brd = ExDynCast<IGuiWnd>(ret);
		if (brd) m_NewDiv.Add(brd);
		if (exc->m_pNedDel) m_NedDel.Add(exc->m_pNedDel);
	}
	CGuiXML::ite_list_t cld_lst = ite->Children();
	CGuiXML::ite_list_t::iterator_t i = cld_lst.Head();
	for(; i != cld_lst.Tail(); ++i) Exec(xml, *i, ret);
}

BOOL CGuiSkin::Parse(IFileObject* pFile)
{
	if (!pFile) return FALSE;
	CGuiXML xml;
	xml.SetDefEnc(_T("gb2312"));
	xml.SetFile(pFile);
	if (!xml.Decode()) return FALSE;
	// 开始解析脚本
	CGuiXML::iterator_t xml_ite = xml.GetRoot();
	while(xml.GetNode(_T("include"), xml_ite))
	{
		CString path(pFile->GetFileName());
		if(!path.Empty()) _tcsrchr(path, _T('\\'))[1] = _T('\0');
		path += xml.GetVal(xml_ite);
		LoadFile(path);
	}
	Exec(xml, xml.GetRoot());
	// 发送全局消息
	if (m_NewDiv.Empty())
	{
		for(CArrayT<void*>::iterator_t ite = m_LstDiv.Head(); ite != m_LstDiv.Tail(); ++ite)
		{
			IGuiWnd* brd = (IGuiWnd*)(*ite);
			if (::IsWindowVisible(brd->GethWnd()))
				brd->SendMessage(WM_SHOWWINDOW, 1);
		}
	}
	else
	{
		for(CArrayT<void*>::iterator_t ite = m_NewDiv.Head(); ite != m_NewDiv.Tail(); ++ite)
		{
			IGuiWnd* brd = (IGuiWnd*)(*ite);
			if (::IsWindowVisible(brd->GethWnd()))
				brd->SendMessage(WM_SHOWWINDOW, 1);
		}
		m_LstDiv = m_NewDiv;
		m_NewDiv.Clear();
	}
	// 清理垃圾对象
	for(CArrayT<void*>::iterator_t ite = m_NedDel.Head(); ite != m_NedDel.Tail(); ++ite)
		del(*ite);
	m_NedDel.Clear();
	return TRUE;
}

BOOL CGuiSkin::Parse(const CString& script)
{
	CStringT<char> s(script);
	CMemFile file((BYTE*)s.GetCStr(), s.GetLength());
	return Parse(&file);
}

BOOL CGuiSkin::LoadFile(const CString& path)
{
	CIOFile file(path);
	return Parse(&file);
}

BOOL CGuiSkin::Load(CXZip& zip)
{
	if (zip.IsClosed()) return FALSE;
	// 获得临时文件夹
	CString temp;
	if(!::GetTempPath(MAX_PATH, temp.GetCStr(MAX_PATH))) return FALSE;
	// 随机目录名
	ExRandomize();
	CString path;
	path.GetCStr(MAX_PATH);
	path.Format(_T("%sGuiExp_Skin_%d"), (LPCTSTR)temp, 1000 + ExRandom(9000));
	// 删除已存在目录
	path[path.GetCount()] = _T('\0');
	DelPath(path);
	// 创建解压目录
	::CreateDirectory(path, NULL);
	// 解压文件
	if(!zip.UnzipFile(path)) return FALSE;
	// 加载皮肤
	BOOL r1 = TRUE, r2 = TRUE, r3 = TRUE;
	if(!LoadFile(path + _T("\\ui.xml")))
	{
		r1 = LoadFile(path + _T("\\ui_res.xml"));
		r2 = LoadFile(path + _T("\\ui_sty.xml"));
		r3 = LoadFile(path + _T("\\ui_div.xml"));
	}
	// 删除解压目录
	DelPath(path);
	return (r1 || r2 || r3);
}

BOOL CGuiSkin::Load(const CString& path)
{
	CXZip zip(path);
	return Load(zip);
}

BOOL CGuiSkin::Load(BYTE* buff, DWORD size)
{
	CXZip zip(buff, size);
	return Load(zip);
}

BOOL CGuiSkin::DelPath(LPCTSTR sPath)
{
	// 创建文件对象
	SHFILEOPSTRUCT lpsh = {0};
	// 调整文件对象
	lpsh.fFlags	= FOF_SILENT | FOF_NOCONFIRMATION;
	lpsh.wFunc	= FO_DELETE;
	lpsh.pFrom	= sPath;
	lpsh.fAnyOperationsAborted = TRUE;
	// 删除文件
	return (SHFileOperation(&lpsh) == 0);
}

//////////////////////////////////////////////////////////////////

EXP_END