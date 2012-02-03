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
// Date:	2012-01-31
// Version:	1.0.0001.0945
//////////////////////////////////////////////////////////////////

#include "GuiCommon/GuiCommon.h"
#include "GuiSkin.h"
#include "GuiManager/Executors/Executor.h"
#include "GuiBoard/GuiBoard.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

CArrayT<void*> CGuiSkin::m_NewDiv;
CArrayT<void*> CGuiSkin::m_NedDel;

void CGuiSkin::Exec(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent/* = NULL*/)
{
	IExecutor* exc = CExecutor::Get((*ite)->nam);
	void* ret = NULL;
	if (exc)
	{
		ret = exc->Execute(xml, ite, parent);
		IGuiBoard* brd = ExDynCast<IGuiBoard>(ret);
		if (brd) m_NewDiv.Add(brd);
		if (exc->m_pNedDel) m_NedDel.Add(exc->m_pNedDel);
	}
	CGuiXML::ite_list_t cld_lst = ite->Children();
	CGuiXML::ite_list_t::iterator_t i = cld_lst.Head();
	for(; i != cld_lst.Tail(); ++i) Exec(xml, *i, ret);
}

BOOL CGuiSkin::Load(IFileObject* pFile)
{
	if (!pFile) return FALSE;
	CGuiXML xml;
	xml.SetDefEnc(_T("gb2312"));
	xml.SetFile(pFile);
	if (!xml.Decode()) return FALSE;
	// 开始解析脚本
	Exec(xml, xml.GetRoot());
	// 发送全局消息
	for(CArrayT<void*>::iterator_t ite = m_NewDiv.Head(); ite != m_NewDiv.Tail(); ++ite)
	{
		IGuiBoard* brd = (IGuiBoard*)(*ite);
		if (::IsWindowVisible(brd->GethWnd()))
			brd->SendMessage(WM_SHOWWINDOW, 1);
	}
	m_NewDiv.Clear();
	// 清理垃圾对象
	for(CArrayT<void*>::iterator_t ite = m_NedDel.Head(); ite != m_NedDel.Tail(); ++ite)
		del(*ite);
	m_NedDel.Clear();
	return TRUE;
}

BOOL CGuiSkin::Load(LPCSTR script)
{
	CMemFile file((BYTE*)script, strlen(script) * sizeof(CHAR));
	return Load(&file);
}

//////////////////////////////////////////////////////////////////

EXP_END