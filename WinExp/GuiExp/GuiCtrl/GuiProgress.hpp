// Copyright 2012, 木头云
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
// GuiProgress - 进度条控件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-20
// Version:	1.0.0000.1308
//
// History:
//	- 1.0.0000.1308(2012-03-20)	@ 开始构建GuiProgress
//////////////////////////////////////////////////////////////////

#ifndef __GuiProgress_hpp__
#define __GuiProgress_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiProgress : public CGuiPicture
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiProgress, CGuiPicture)

protected:
	CGuiPicture m_Prog;
	uint32_t m_nVal, m_nMax;
		
public:
	CGuiProgress()
		: m_nVal(0)
		, m_nMax(100)
	{
		// 添加事件对象
		InsEvent(_T("CGuiProgressEvent")); /*先让基类绘图*/
		SetState(_T("color"), (void*)ExRGBA(220, 220, 220, EXP_CM));
		m_Prog.SetState(_T("color"), (void*)ExRGBA(120, 120, 120, EXP_CM));
		// 添加控件对象
		InsComp(&m_Prog);
	}
	~CGuiProgress()
	{
		DelComp(&m_Prog, FALSE);
	}

public:
	BOOL Execute(const CString& key, const CString& val)
	{
		if (key.Left(3) == _T("pg_"))
		{
			CString type(key);
			type.TrimLeft(_T("pg_"));
			m_Prog.Execute(type, val);
		}
		else
		if (key == _T("val"))
			SetState(_T("val"), (void*)_ttol(val));
		else
		if (key == _T("max"))
			SetState(_T("max"), (void*)_ttol(val));
		else
			return EXP_BASE::Execute(key, val);
		return IGuiCtl::Execute(key, val);
	}

	void* GetState(const CString& sType, void* pParam = NULL)
	{
		CString type(sType);
		if (sType.Left(3) == _T("pg_"))
		{
			type.TrimLeft(_T("pg_"));
			return m_Prog.GetState(type, pParam);
		}
		if (sType == _T("pg"))
			return (void*)(&m_Prog);
		else
		if (sType == _T("val"))
			return (void*)m_nVal;
		else
		if (sType == _T("max"))
			return (void*)m_nMax;
		else
			return EXP_BASE::GetState(sType, pParam);
	}
	BOOL SetState(const CString& sType, void* pState, void* pParam = NULL)
	{
		CString type(sType);
		if (type.Left(3) == _T("pg_"))
		{
			type.TrimLeft(_T("pg_"));
			return m_Prog.SetState(type, pState, pParam);
		}
		else
		if (sType == _T("val"))
		{
			SET_STATE(uint32_t, m_nVal, 
				SendMessage(WM_SIZE, SIZE_RESTORED, (LPARAM)ExMakeLong(m_Rect.Width(), m_Rect.Height())))
		}
		else
		if (sType == _T("max"))
		{
			SET_STATE(uint32_t, m_nMax, 
				SendMessage(WM_SIZE, SIZE_RESTORED, (LPARAM)ExMakeLong(m_Rect.Width(), m_Rect.Height())))
		}
		else
			return EXP_BASE::SetState(sType, pState, pParam);
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiProgress, CGuiPicture)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiProgress_hpp__*/
