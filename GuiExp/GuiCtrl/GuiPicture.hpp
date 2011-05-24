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
// GuiPicture - ��ͼ��ؼ�
//
// Author:	ľͷ��
// Blog:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2010-05-23
// Version:	1.0.0001.2236
//
// History:
//	- 1.0.0001.2236(2010-05-23)	+ ���CGuiPicture::IsUpdated()�ӿ�
//////////////////////////////////////////////////////////////////

#ifndef __GuiPicture_hpp__
#define __GuiPicture_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiPicture : public IGuiCtrlBase
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiPicture, IGuiCtrlBase)

protected:
	CGC m_GC;
	CImage m_Image;
	bool m_Updated;

public:
	CGuiPicture()
		: m_Updated(true)
	{
		// ����¼�����
		AddEvent((IGuiEvent*)ExGui(_T("CGuiPictureEvent"), &m_GC));
	}
	~CGuiPicture()
	{
		m_Image.Delete();
	}

public:
	// ��ÿؼ�״̬
	state_t* GetState(const CString& sType, CGC* pGC = NULL)
	{
		state_t* state = pGC ? ExMem::Alloc<state_t>(pGC) : state_t::Alloc();
		state->sta_typ = sType;
		if (sType == _T("image"))
			state->sta_arr.Add(&m_Image);
		return state;
	}
	void SetState(const CString& sType, void* pState)
	{
		if (sType == _T("image"))
		{
			m_Image.Delete();
			m_Image.Set(((CImage*)pState)->Get());
			m_Updated = true;
		}
		Refresh();
	}
	bool IsUpdated()
	{
		bool updt = m_Updated;
		m_Updated = false; // �ⲿһ����֪��ǰ״̬,�����״̬�Զ���λ
		return updt;
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiPicture, IGuiCtrlBase);

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiPicture_hpp__*/