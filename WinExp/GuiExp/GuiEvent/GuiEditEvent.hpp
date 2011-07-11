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
// GuiEditEvent - 编辑框控件事件
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-06-29
// Version:	1.0.0001.1956
//
// History:
//	- 1.0.0000.1055(2011-06-21)	@ 开始构建GuiEditEvent
//	- 1.0.0001.1956(2011-06-29)	@ 基本完成GuiEditEvent
//								= 根据CGuiEdit基类的调整,修改GuiEditEvent
//////////////////////////////////////////////////////////////////

#ifndef __GuiEditEvent_hpp__
#define __GuiEditEvent_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiCtrl/GuiCtrl.h"

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiEditEvent : public IGuiEvent
{
	EXP_DECLARE_DYNCREATE_CLS(CGuiEditEvent, IGuiEvent)

protected:
	bool m_ShiftDown, m_CtrlDown, m_MouseDown, m_bFlicker;
	UINT_PTR m_uFlicker;
	IGuiCtrl* m_Ctrl;
	CString::iterator_t m_iteFlicker, m_iteSelect, m_iteOffset;

	typedef CMapT<UINT_PTR, CGuiEditEvent*> map_t;
	static map_t s_TimerToEvent;

protected:
	static void CALLBACK FlickerTimer(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
	{
		CGuiEditEvent* pEvt = s_TimerToEvent[idEvent];
		if (!pEvt) return;
		pEvt->m_bFlicker = !(pEvt->m_bFlicker);
		if (pEvt->m_Ctrl) pEvt->m_Ctrl->Refresh();
	}

public:
	CGuiEditEvent()
		: m_ShiftDown(false)
		, m_CtrlDown(false)
		, m_MouseDown(false)
		, m_bFlicker(false)
		, m_uFlicker(0)
		, m_Ctrl(NULL)
	{}
	~CGuiEditEvent()
	{ ShowFlicker(false); }

public:
	// 获得属性
	CText* GetText(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("text"), pGC);
		if (!state) return NULL;
		return (CText*)(state->sta_arr[0]);
	}
	CString* GetEdit(CGC* pGC)
	{
		ExAssert(m_Ctrl);
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("edit"), pGC);
		if (!state) return NULL;
		return (CString*)(state->sta_arr[0]);
	}

	// 获得显示的文本内容,迭代器返回范围上限
	CString* GetShowEdit(CString::iterator_t& ite2, CGC* pGC)
	{
		ExAssert(m_Ctrl);

		// 获得属性
		CText* text = GetText(pGC);
		if (!text) return NULL;
		CString* edit = GetEdit(pGC);
		if (!edit) return NULL;
		if (!m_iteOffset->InThis(edit)) m_iteOffset = edit->Head();
		static CString show;
		show = ((LPCTSTR)(*edit) + m_iteOffset->Index());

		// 获得显示区域
		CRect rect;
		m_Ctrl->GetClientRect(rect);

		// 获得能显示的文本区域
		CString::iterator_t ite1 = text->Head(), ite3 = text->Head();
		ite1->nIndx = show.Head()->Index();
		ite3->nIndx = show.Tail()->Index();
		ite2 = text->Head();
		ite2->nIndx = ((ite1->nIndx + ite3->nIndx) >> 1);

		CGraph tmp_grp;
		tmp_grp.Create();
		CSize sz;

		text->SetString(show);
		if (text->Empty()) return NULL;
		text->GetSize(tmp_grp, sz);
		if (sz.cx < rect.Width())
			ite2->nIndx = ite3->nIndx;
		else
		do
		{
			text->SetString(show);
			if (text->Empty()) break;
			text->GetAt(ite2) = _T('\0');
			text->GetSize(tmp_grp, sz);
			if (sz.cx > rect.Width())
				ite3->nIndx = ite2->nIndx;
			else
			if (sz.cx < rect.Width())
				ite1->nIndx = ite2->nIndx;
			else
				break;
			ite2->nIndx = ((ite1->nIndx + ite3->nIndx) >> 1);
		} while(ite2->nIndx > ite1->nIndx);
		text->SetString(_T(""));
		tmp_grp.Delete();

		// 定位区域
		show.GetAt(ite2->nIndx) = _T('\0');
		return &show;
	}

	// 偏移显示位置
	void CheckFlicker()
	{
		if (m_iteFlicker->Index() > m_iteOffset->Index())
		{
			do
			{
				// 获得显示上限迭代器
				CGC gc;
				CString::iterator_t ite;
				GetShowEdit(ite, &gc);
				ite->nIndx += m_iteOffset->Index();
				// 判断是否超限并调整下限
				if (m_iteFlicker->Index() > ite->Index())
					++m_iteOffset;
				else
					break;
			} while (1);

			// 刷新界面
			m_bFlicker = true;
			m_Ctrl->Refresh();
		}
		else
		if (m_iteFlicker->Index() < m_iteOffset->Index())
		{
			m_iteOffset = m_iteFlicker;

			// 刷新界面
			m_bFlicker = true;
			m_Ctrl->Refresh();
		}
	}

	// 光标显示
	void ShowFlicker(bool bShow)
	{
		if (bShow)
		{
			if (m_uFlicker != 0) return;
			m_uFlicker = ::SetTimer(NULL, 0, 500, FlickerTimer);
			s_TimerToEvent[m_uFlicker] = this;
			m_bFlicker = true;
		}
		else
		{
			if (m_uFlicker == 0) return;
			m_bFlicker = false;
			::KillTimer(NULL, m_uFlicker);
			s_TimerToEvent.Del(m_uFlicker);
			m_uFlicker = 0;
		}
	}

	// 根据坐标定位光标
	void PosFlicker(const CPoint& ptPos)
	{
		ExAssert(m_Ctrl);
		CGC gc;

		// 获得属性
		CText* text = GetText(&gc);
		if (!text) return;
		CString* edit = GetEdit(&gc);
		if (!edit) return;

		// 拿到偏移量
		CSize sz_off;
		if (!m_iteOffset->InThis(edit)) m_iteOffset = edit->Head();
		if (m_iteOffset->Index() > edit->Head()->Index())
		{
			CGraph tmp_grp;
			tmp_grp.Create();

			text->SetString(*edit);
			text->GetAt(m_iteOffset->Index()) = _T('\0');
			text->GetSize(tmp_grp, sz_off);

			tmp_grp.Delete();
		}
		CPoint point(ptPos);
		point.x += sz_off.cx;

		// 搜索字符位置
		if (!m_iteFlicker->InThis(edit)) m_iteFlicker = edit->Tail();
		CString::iterator_t ite1 = text->Head(), ite2 = text->Head(), ite3 = text->Head();
		ite2->nIndx = m_iteFlicker->nIndx;
		ite3->nIndx = edit->Tail()->nIndx;

		CGraph tmp_grp;
		tmp_grp.Create();
		CSize sz;

		text->SetString(*edit);
		if (text->Empty()) return;
		text->GetSize(tmp_grp, sz);
		if (sz.cx < point.x)
			ite2->nIndx = ite3->nIndx;
		else
		do
		{
			text->SetString(*edit);
			if (text->Empty()) break;
			text->GetAt(ite2) = _T('\0');
			text->GetSize(tmp_grp, sz);
			if (sz.cx > point.x)
				ite3->nIndx = ite2->nIndx;
			else
			if (sz.cx < point.x)
				ite1->nIndx = ite2->nIndx;
			else
				break;
			ite2->nIndx = ((ite1->nIndx + ite3->nIndx) >> 1);
		} while(ite2->nIndx > ite1->nIndx);

		if (m_ShiftDown || m_MouseDown)
		{
			if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
			m_iteFlicker->nIndx = ite2->nIndx;
		}
		else
		{
			m_iteFlicker->nIndx = ite2->nIndx;
			m_iteSelect = m_iteFlicker;
		}
		text->SetString(_T(""));
		tmp_grp.Delete();
		CheckFlicker();
	}
	
	// 定位选区
	void PosSelect(CString::iterator_t& ite1, CString::iterator_t& ite2)
	{
		if (m_iteSelect->Index() < m_iteFlicker->Index())
		{
			ite1 = m_iteSelect;
			ite2 = m_iteFlicker;
		}
		else
		if (m_iteSelect->Index() > m_iteFlicker->Index())
		{
			ite1 = m_iteFlicker;
			ite2 = m_iteSelect;
		}
		else
			ite1 = ite2 = m_iteFlicker;
	}

	// 全选文本
	void SelectAll()
	{
		ExAssert(m_Ctrl);
		CGC gc;
		// 获得属性
		CString* edit = GetEdit(&gc);
		if (!edit) return;
		// 全选文本
		m_iteSelect = edit->Head();
		m_iteFlicker = edit->Tail();
		CheckFlicker();
	}

	// 按键输入
	void OnKey(TCHAR cInput)
	{
		ExAssert(m_Ctrl);

		if (cInput == VK_SHIFT)
			m_ShiftDown = true;
		else
		if (cInput == VK_CONTROL)
			m_CtrlDown = true;
		else
		{
			CGC gc;

			// 获得属性
			CString* edit = GetEdit(&gc);
			if (!edit) return;
			if (!m_iteFlicker->InThis(edit)) m_iteFlicker = edit->Tail();

			// 判断字符
			if (cInput == VK_HOME)
			{
				if (edit->Empty()) return;
				m_iteSelect = m_iteFlicker = edit->Head();
			}
			else
			if (cInput == VK_END)
			{
				if (edit->Empty()) return;
				m_iteSelect = m_iteFlicker = edit->Tail();
			}
			else
			if (cInput == VK_LEFT)
			{
				if (edit->Empty()) return;
				if (m_ShiftDown)
				{
					if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
					--m_iteFlicker;
				}
				else
				{
					--m_iteFlicker;
					m_iteSelect = m_iteFlicker;
				}
			}
			else
			if (cInput == VK_RIGHT)
			{
				if (edit->Empty()) return;
				if (m_ShiftDown)
				{
					if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
					++m_iteFlicker;
				}
				else
				{
					++m_iteFlicker;
					m_iteSelect = m_iteFlicker;
				}
			}
			else
			if (cInput == VK_DELETE)
			{
				if (edit->Empty()) return;
				// 定位选区
				if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
				CString::iterator_t ite1, ite2;
				PosSelect(ite1, ite2);
				// 删除选区
				if (ite1 == ite2)
					edit->Del(ite1);
				else
					edit->Del(ite1, ite2->Index() - ite1->Index());
				m_iteSelect = m_iteFlicker = ite1;
			}
			else
			if (m_CtrlDown)
			{
				if (cInput == 'C')
				{
					// 获得选区
					if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
					CString::iterator_t ite1, ite2;
					PosSelect(ite1, ite2);
					if (ite1 == ite2) return;
					// 拷贝选区文本
					CString temp(((LPCTSTR)(*edit)) + ite1->Index());
					temp[ite2->Index()] = _T('\0');
					// 操作剪切板
					if (!::OpenClipboard(NULL)) return;
					::EmptyClipboard();
					SIZE_T siz_buf = (temp.GetLength() + 1) * sizeof(TCHAR);
					HGLOBAL clp_buf = ::GlobalAlloc(GMEM_DDESHARE, siz_buf);
					TCHAR* buff = (TCHAR*)::GlobalLock(clp_buf);
					StringCchCopy(buff, temp.GetLength() + 1, (LPCTSTR)temp);
					::GlobalUnlock(clp_buf);
					::SetClipboardData(CF_UNICODETEXT, clp_buf);
					::CloseClipboard();
				}
				else
				if (cInput == 'V')
				{
					if (!::OpenClipboard(NULL)) return;
					HGLOBAL clp_buf = GetClipboardData(CF_UNICODETEXT);
					if (clp_buf)
					{
						TCHAR* buff = (TCHAR*)::GlobalLock(clp_buf);
						if (buff)
						{
							if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
							CString::iterator_t ite1, ite2;
							PosSelect(ite1, ite2);
							// 添加文本
							edit->Del(ite1, ite2->Index() - ite1->Index());
							edit->AddString(buff, ite1);
							m_iteSelect = m_iteFlicker = ite1 += _tcslen(buff);
							// 解锁句柄
							::GlobalUnlock(clp_buf);
						}
					}
					::CloseClipboard();
				}
				else
				if (cInput == 'X')
				{
					// 获得选区
					if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
					CString::iterator_t ite1, ite2;
					PosSelect(ite1, ite2);
					if (ite1 == ite2) return;
					// 拷贝选区文本
					CString temp(((LPCTSTR)(*edit)) + ite1->Index());
					temp[ite2->Index()] = _T('\0');
					// 操作剪切板
					if (!::OpenClipboard(NULL)) return;
					::EmptyClipboard();
					SIZE_T siz_buf = (temp.GetLength() + 1) * sizeof(TCHAR);
					HGLOBAL clp_buf = ::GlobalAlloc(GMEM_DDESHARE, siz_buf);
					TCHAR* buff = (TCHAR*)::GlobalLock(clp_buf);
					StringCchCopy(buff, temp.GetLength() + 1, (LPCTSTR)temp);
					::GlobalUnlock(clp_buf);
					::SetClipboardData(CF_UNICODETEXT, clp_buf);
					::CloseClipboard();
					// 删除选区内容
					edit->Del(ite1, ite2->Index() - ite1->Index());
					m_iteSelect = m_iteFlicker = ite1;
				}
				else
				if (cInput == 'A')
				{
					SelectAll();
				}
			}

			// 刷新界面
			CheckFlicker();
		}
	}

	// 字符输入
	void OnChar(TCHAR cInput)
	{
		ExAssert(m_Ctrl);
		CGC gc;

		// 获得属性
		CString* edit = GetEdit(&gc);
		if (!edit) return;
		if (!m_iteFlicker->InThis(edit)) m_iteFlicker = edit->Tail();

		// 判断字符
		if (cInput == VK_BACK)
		{
			if (edit->Empty()) return;
			// 定位选区
			if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
			CString::iterator_t ite1, ite2;
			PosSelect(ite1, ite2);
			// 删除选区
			if (ite1 == ite2)
				edit->Del(--ite1);
			else
				edit->Del(ite1, ite2->Index() - ite1->Index());
			m_iteSelect = m_iteFlicker = ite1;
		}
		else
		if (cInput >= VK_SPACE)
		{
			// 定位选区
			if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
			CString::iterator_t ite1, ite2;
			PosSelect(ite1, ite2);
			// 添加字符
			edit->Del(ite1, ite2->Index() - ite1->Index());
			edit->Add(cInput, ite1);
			++ite1; /*此处若在上面使用ite1++,当输入第一个字符之前edit为空,则++运算将被忽略*/
			m_iteSelect = m_iteFlicker = ite1;
		}

		// 刷新界面
		CheckFlicker();
	}

	// 界面绘图
	void OnPaint(CImage* mem_img)
	{
		ExAssert(m_Ctrl);
		if (!mem_img || mem_img->IsNull()) return;
		CGC gc;

		// 获得属性
		IGuiCtrl::state_t* state = m_Ctrl->GetState(_T("text"), &gc);
		if (!state) return;
		CText* text = (CText*)(state->sta_arr[0]);
		if (!text) return;
		CText tmp_text = (*text);

		state = m_Ctrl->GetState(_T("edit"), &gc);
		if (!state) return;
		CString* edit = (CString*)(state->sta_arr[0]);
		if (!edit) return;

		state = m_Ctrl->GetState(_T("txt_sel_color"), &gc);
		if (!state) return;
		pixel_t txt_sel_color = *(pixel_t*)(state->sta_arr[0]);

		state = m_Ctrl->GetState(_T("bkg_sel_color"), &gc);
		if (!state) return;
		pixel_t bkg_sel_color = *(pixel_t*)(state->sta_arr[0]);

		CRect rect;
		m_Ctrl->GetClientRect(rect);

		// 拿到偏移量
		CSize sz_off;
		if (!m_iteOffset->InThis(edit)) m_iteOffset = edit->Head();
		if (m_iteOffset->Index() > edit->Head()->Index())
		{
			CGraph tmp_grp;
			tmp_grp.Create();

			tmp_text.SetString(*edit);
			tmp_text[m_iteOffset->Index()] = _T('\0');
			tmp_text.GetSize(tmp_grp, sz_off);

			tmp_grp.Delete();
		}

		// 定位选区
		if (!m_iteSelect->InThis(edit)) m_iteSelect = m_iteFlicker;
		CString::iterator_t ite1, ite2;
		PosSelect(ite1, ite2);
		CSize sz_hed, sz_sel;
		if (ite1 != ite2)
		{
			CGraph tmp_grp;
			tmp_grp.Create();

			tmp_text.SetString(*edit);
			tmp_text[ite1->Index()] = _T('\0');
			tmp_text.GetSize(tmp_grp, sz_hed);

			tmp_text.SetString(*edit);
			tmp_text[ite2->Index()] = _T('\0');
			tmp_text.Del(tmp_text.Head(), ite1->Index());
			tmp_text.GetSize(tmp_grp, sz_sel);

			tmp_grp.Delete();
		}

		// 绘文字
		tmp_text.SetString(*edit);
		CImage txt_img(tmp_text.GetImage());
		if(!txt_img.IsNull())
		{
			CRect sel_rc(sz_hed.cx, 0, sz_hed.cx + sz_sel.cx, sz_sel.cy);
			if (!sel_rc.IsEmpty())
			{
				CFilterFill filter(ExRevColor(txt_sel_color), 0xf, true);
				CImgFilter::Filter(txt_img, sel_rc, &filter);
				filter.m_Const = ExRevColor(bkg_sel_color);
				filter.m_ClrMask = txt_sel_color;
				CImgFilter::Filter(txt_img, sel_rc, &filter);
			}
			CImgRenderer::Render(mem_img->Get(), txt_img, rect, CPoint(sz_off.cx, 0));
		}

		// 绘光标
		if (m_bFlicker)
		{
			// 获得光标高度
			tmp_text.SetString(_T("lq"));
			CGraph tmp_grp;
			tmp_grp.Create();
			CSize sz_flk;
			tmp_text.GetSize(tmp_grp, sz_flk);
			// 获得文字宽度
			tmp_text.SetString(*edit);
			tmp_text.GetAt(m_iteFlicker->Index()) = _T('\0');
			CSize sz_txt;
			tmp_text.GetSize(tmp_grp, sz_txt);
			tmp_grp.Delete();
			// 画线
			CImgDrawer::Line
				(
				mem_img->Get(), 
				CLine(sz_txt.cx - sz_off.cx, 0, sz_txt.cx - sz_off.cx, sz_flk.cy), 
				ExRGBA(0, 0, 0, EXP_CM)
				);
		}
	}

	// 消息响应
	void OnMessage(IGuiObject* pGui, UINT nMessage, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		m_Ctrl = ExDynCast<IGuiCtrl>(pGui);
		if (!m_Ctrl) return;

		// 处理消息
		switch( nMessage )
		{
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_IBEAM));
			break;
		case WM_LBUTTONDOWN:
			::SetCursor(::LoadCursor(NULL, IDC_IBEAM));
			m_Ctrl->SetCapture();
			// 鼠标点击时定位光标
			{
				CPoint point(ExLoWord(lParam), ExHiWord(lParam));
				PosFlicker(point);
			}
			m_MouseDown = true;
			break;
		case WM_LBUTTONUP:
			::SetCursor(::LoadCursor(NULL, IDC_IBEAM));
			m_MouseDown = false;
			m_Ctrl->ReleaseCapture();
			break;
		case WM_MOUSEMOVE:
			::SetCursor(::LoadCursor(NULL, IDC_IBEAM));
			// 鼠标移动时定位光标
			if (m_MouseDown)
			{
				POINT pt_tmp = {0};
				::GetCursorPos(&pt_tmp); /*此处有可能移动到窗口外部,而lParam不支持负坐标*/
				CPoint point(pt_tmp);
				m_Ctrl->GetBoard()->ScreenToClient(point);
				CRect rc_tmp(point, point);
				m_Ctrl->B2C(rc_tmp);
				point = rc_tmp.pt1;
			//	ExTrace(_T("(%d, %d)\n"), point.x, point.y);
				PosFlicker(point);
			}
			break;
		case WM_LBUTTONDBLCLK:
			// 双击全选
			SelectAll();
			break;
		case WM_KEYUP:
			if (wParam == VK_SHIFT)
				m_ShiftDown = false;
			else
			if (wParam == VK_CONTROL)
				m_CtrlDown = false;
			break;
		case WM_KEYDOWN:
			// 按键输入
			OnKey((TCHAR)wParam);
			break;
		case WM_CHAR:
			// 字符输入
			OnChar((TCHAR)wParam);
			break;
		case WM_SETFOCUS:
			// 获得焦点时显示光标
			ShowFlicker(true);
			break;
		case WM_KILLFOCUS:
			m_ShiftDown = false;
			m_CtrlDown = false;
			m_MouseDown = false;
			m_Ctrl->ReleaseCapture();
			// 失去焦点时隐藏光标
			ShowFlicker(false);
			break;
		case WM_PAINT:
			OnPaint((CImage*)lParam);
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNCREATE_CLS(CGuiEditEvent, IGuiEvent)
CGuiEditEvent::map_t CGuiEditEvent::s_TimerToEvent;

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiEditEvent_hpp__*/