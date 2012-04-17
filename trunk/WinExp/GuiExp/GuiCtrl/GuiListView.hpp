// Copyright 2011-2012, ľͷ��
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
// GuiListView - �б���ͼ�ؼ�
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-01
// Version:	1.0.0011.1239
//
// History:
//	- 1.0.0000.1543(2011-06-30)	@ ��ʼ����GuiListView
//	- 1.0.0001.1532(2011-07-21)	= ��CGuiListView�ڲ�items_t�ṹ��ָ���Ϊ����,������ø��Ӷ�
//	- 1.0.0002.1744(2011-08-05)	+ ���GuiListView����ʱĬ���б���ı���ͼ
//	- 1.0.0003.1508(2011-08-10)	= ��Ĭ���б���ı���ͼ��ͼƬ��ΪPic�ؼ�
//	- 1.0.0004.1730(2011-08-11)	= �б����ͼ��ƫ�Ʋ��õ���������(ico_off)����
//	- 1.0.0005.1030(2011-08-12)	= ��Ĭ���б���ı���ͼ��ͼƬ��Ϊ��̬��ť�ؼ�,����֧�־Ź���ʽ�Ľ���ͼƬ
//	- 1.0.0006.1606(2011-08-15)	^ ��CGuiLVItem�ľ�������ת�Ƶ�CGuiButton��ʵ��,������������¼���Ӧ
//	- 1.0.0007.1623(2011-08-26)	# ������GuiListView�����й������޸��б���ʱ,�����Զ���ʽ���б���λ�õ�����
//	- 1.0.0008.2317(2011-08-30)	+ GuiListView���align_top����,֧���б����϶���/�¶������
//	- 1.0.0009.1517(2011-09-28)	# �������ⲿ���ٿؼ�����ʱ,GuiListView���ڲ���������˳�����⵼�µ��ڴ�����쳣
//	- 1.0.0010.1740(2012-02-29)	- ���йع��������Ƶ�������Դ�GuiListView���Ƴ�,ͳһ���ɵײ����
//	- 1.0.0011.1239(2012-03-01)	= ��������CGuiPicture����ΪCGuiList,�������Խ���CGuiList��ʵ��
//////////////////////////////////////////////////////////////////

#ifndef __GuiListView_hpp__
#define __GuiListView_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiLVItem : public CGuiListItem /*CGuiListView�ڲ�ʹ�õ��б���*/
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiLVItem, CGuiListItem)

public:
	CGuiLVItem()
	{
		// ����¼�����
		PopEvent(FALSE);
		InsEvent(_T("CGuiLVItemEvent")); /*���û����ͼ*/
	}
};

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiLVItem, CGuiListItem)

//////////////////////////////////////////////////////////////////

class CGuiListView : public CGuiList
{
	EXP_DECLARE_DYNCREATE_MULT(CGuiListView, CGuiList)

protected:
	BOOL m_AlignTop;

public:
	CGuiListView()
		: m_AlignTop(TRUE)
	{
		// ����¼�����
		PopEvent(FALSE);
		InsEvent(_T("CGuiLVEvent")); /*���û����ͼ*/
	}
	~CGuiListView()
	{
	}

public:
	BOOL Execute(const CString& key, const CString& val)
	{
		if (key == _T("align_top"))
		{
			CString temp(val);
			temp.Lower();
			if (temp == _T("false"))
				SetState(_T("align_top"), (void*)FALSE);
			else
			if (temp == _T("true"))
				SetState(_T("align_top"), (void*)TRUE);
		}
		else
			return EXP_BASE::Execute(key, val);
		return IGuiCtl::Execute(key, val);
	}

	// ��ÿؼ�״̬
	void* GetState(const CString& sType, void* pParam = NULL)
	{
		if (sType == _T("align_top"))
			return (void*)m_AlignTop;
		else
			return EXP_BASE::GetState(sType, pParam);
	}
	BOOL SetState(const CString& sType, void* pState, void* pParam = NULL)
	{
		CString type(sType);
		if (sType == _T("align_top"))
			SET_STATE(BOOL, m_AlignTop, SendMessage(WM_SIZE))
		else
			return EXP_BASE::SetState(sType, pState, pParam);
	}
};

EXP_IMPLEMENT_DYNCREATE_MULT(CGuiListView, CGuiList)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiListView_hpp__*/