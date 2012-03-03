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
// Executor - 界面脚本执行
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-30
// Version:	1.0.0000.1058
//////////////////////////////////////////////////////////////////

#ifndef __GuiExecutor_hpp__
#define __GuiExecutor_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////
// CGuiManagerT的显式实例化

template class CGuiManagerT<void>;
template class CGuiManagerT<CImage>;
template class CGuiManagerT<CText>;
template class CGuiManagerT<IGuiWnd>;
template class CGuiManagerT<IGuiCtl>;
template class CGuiManagerT<IGuiEvent>;
template class CGuiManagerT<IGuiEffect>;

//////////////////////////////////////////////////////////////////

EXP_IMPLEMENT_DYNAMIC_CLS(IExecutor, IBaseObject)

//////////////////////////////////////////////////////////////////

// 获得字符串颜色
EXP_API pixel_t ExStringToColor(CString sColor)
{
	COLORREF clr = ExRGBA(0, 0, 0, 0);
	if (sColor.Empty())
		return clr;
	else
	if (sColor[0] == _T('#'))
	{
		LPCTSTR c = ((LPCTSTR)sColor) + 1;
		TCHAR str_rgb[4][3]	= {0};
		UINT rgb[4]	= {0};
		if (sColor.GetLength() >= 9)
		{
			memcpy(str_rgb[0], c + 0, sizeof(str_rgb[0]));
			str_rgb[0][2] = _T('\0');
			memcpy(str_rgb[1], c + 2, sizeof(str_rgb[1]));
			str_rgb[1][2] = _T('\0');
			memcpy(str_rgb[2], c + 4, sizeof(str_rgb[2]));
			str_rgb[2][2] = _T('\0');
			memcpy(str_rgb[3], c + 6, sizeof(str_rgb[3]));
			str_rgb[3][2] = _T('\0');
			rgb[0] = _tcstol(str_rgb[0], NULL, 16);
			rgb[1] = _tcstol(str_rgb[1], NULL, 16);
			rgb[2] = _tcstol(str_rgb[2], NULL, 16);
			rgb[3] = _tcstol(str_rgb[3], NULL, 16);
			clr = ExRGBA(rgb[0], rgb[1], rgb[2], rgb[3]);
		}
		else
		if (sColor.GetLength() >= 7)
		{
			memcpy(str_rgb[0], c + 0, sizeof(str_rgb[0]));
			str_rgb[0][2] = _T('\0');
			memcpy(str_rgb[1], c + 2, sizeof(str_rgb[1]));
			str_rgb[1][2] = _T('\0');
			memcpy(str_rgb[2], c + 4, sizeof(str_rgb[2]));
			str_rgb[2][2] = _T('\0');
			rgb[0] = _tcstol(str_rgb[0], NULL, 16);
			rgb[1] = _tcstol(str_rgb[1], NULL, 16);
			rgb[2] = _tcstol(str_rgb[2], NULL, 16);
			clr = ExRGB(rgb[0], rgb[1], rgb[2]);
		}
	}
	return clr;
}

// 获得字符串数组
EXP_API int ExStringToArray(const CString s, CArrayT<CString> &sa, TCHAR cSpl/* = _T(',')*/, BOOL bTrim/* = TRUE*/)
{
	CString::iterator_t lst_pos, pos;
	BOOL is_continue = FALSE;

	sa.Clear();
	lst_pos = s.Head();
	do
	{
		is_continue = FALSE;
		pos = s.Find(lst_pos, s.Tail(), cSpl);
		if (pos != s.Tail())
		{
			sa.Add(s.Mid(lst_pos->Index(), pos->Index() - lst_pos->Index()));
			lst_pos->nIndx = pos->Index() + 1;
			if (lst_pos != s.Last()) is_continue = TRUE;
		}
	} while(is_continue);

	if (lst_pos != s.Tail())
		sa.Add(s.Mid(lst_pos->Index(), s.GetLength() - lst_pos->Index()));

	if (bTrim)
		for(int i = 0; i < (int)sa.GetCount(); ++i)
		{
			sa[i].Trim(_T(' '));
			sa[i].Trim(_T('\t'));
			sa[i].Trim(_T('\n'));
			sa[i].Trim(_T('\r'));
		}

	return (int)sa.GetCount();
}

// 获得字符串区域
EXP_API CRect ExStringToRect(const CString& sRect)
{
	CRect rc(0, 0, 0, 0);
	if (sRect.Empty()) return rc;
	CString r(sRect);
	r.Replace(_T(' '));
	CArrayT<CString> sa;
	if (ExStringToArray(r, sa, _T(',')) >= 4)
	{
		rc.Set(
			_tcstol(sa[0], NULL, 10), 
			_tcstol(sa[1], NULL, 10), 
			_tcstol(sa[2], NULL, 10), 
			_tcstol(sa[3], NULL, 10));
	}
	return rc;
}

// 获得资源
EXP_API BOOL ExReleaseBinary(HGLOBAL hData)
{
	if (!hData) return TRUE;
	// 释放资源内存块
	UnlockResource(hData);
	return FreeResource(hData);
}
EXP_API HGLOBAL ExGetBinary(UINT nID, LPCTSTR szType, BYTE*& btBuff, DWORD& dwSize, HMODULE hInstance/* = NULL*/)
{
	// 加载资源内存
	HRSRC info = ::FindResource(hInstance, MAKEINTRESOURCE(nID), szType);
	if( info == NULL ) return NULL;
	HGLOBAL data = ::LoadResource(hInstance, info);
	if( data == NULL ) return NULL;
	// 获得资源内存块
	dwSize = ::SizeofResource(hInstance, info);
	if( dwSize == NULL )
	{
		ExReleaseBinary(data);
		return NULL;
	}
	btBuff = (BYTE*)::LockResource(data);
	if( btBuff == NULL )
	{
		ExReleaseBinary(data);
		return NULL;
	}
	return data;
}

//////////////////////////////////////////////////////////////////
// 各种执行器
//////////////////////////////////////////////////////////////////

// 摘要
class _detail : public IExecutor
{
	EXP_DECLARE_DYNCREATE_CLS(_detail, IExecutor)

protected:
	detail_t m_Detail;

public:
	void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent)
	{
		m_Detail.name	 = xml.GetAttr(_T("name"),	  ite);
		m_Detail.author  = xml.GetAttr(_T("author"),  ite);
		m_Detail.home	 = xml.GetAttr(_T("home"),	  ite);
		m_Detail.email	 = xml.GetAttr(_T("email"),	  ite);
		m_Detail.version = xml.GetAttr(_T("version"), ite);
		m_Detail.date	 = xml.GetAttr(_T("date"),	  ite);
		ExReg<detail_t>(_T("detail"), &m_Detail);
		return NULL;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_detail, IExecutor)

//////////////////////////////////////////////////////////////////

// 字体
class _font : public IExecutor
{
	EXP_DECLARE_DYNCREATE_CLS(_font, IExecutor)

protected:
	CGC gc;

public:
	void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent)
	{
		LOGFONT lf = {0};
		CString t = xml.GetAttr(_T("face"), ite);
		if (t.Empty())
			::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(lf), &lf);
		else
			_tcscpy_s(lf.lfFaceName, t);
		t = xml.GetAttr(_T("size"), ite);
		if(!t.Empty())
			lf.lfHeight = _ttol(t);
		t = xml.GetAttr(_T("weight"), ite); t.Lower();
		if (t == _T("normal"))
			lf.lfWeight = FW_NORMAL;
		else
		if (t == _T("bold"))
			lf.lfWeight = FW_BOLD;
		else
		if (t == _T("heavy"))
			lf.lfWeight = FW_HEAVY;
		else
		if (t == _T("thin"))
			lf.lfWeight = FW_THIN;
		t = xml.GetAttr(_T("italic"), ite); t.Lower();
		if (t == _T("false"))
			lf.lfItalic = FALSE;
		else
		if (t == _T("true"))
			lf.lfItalic = TRUE;
		t = xml.GetAttr(_T("underline"), ite); t.Lower();
		if (t == _T("false"))
			lf.lfUnderline = FALSE;
		else
		if (t == _T("true"))
			lf.lfUnderline = TRUE;
		CText* txt = ExGet<CText>(xml.GetAttr(_T("name"), ite));
		if(!txt)
		{
			txt = gcnew(gc, CText);
			ExReg<CText>(xml.GetAttr(_T("name"), ite), txt);
		}
		txt->Create(&lf);
		txt->SetColor(ExStringToColor(xml.GetAttr(_T("color"), ite)));
		return NULL;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_font, IExecutor)

//////////////////////////////////////////////////////////////////

// 图片
class _image : public IExecutor
{
	EXP_DECLARE_DYNCREATE_CLS(_image, IExecutor)

protected:
	CGC gc;

	void LoadImg(IFileObject*& file, CGC& exc_gc, CImage*& img, CGuiXML& xml, CGuiXML::iterator_t& ite)
	{
		ICoderObject* coder = CImgAnalyzer::GetCoder(file, &exc_gc);
		if (coder)
		{
			if(!img)
			{
				img = gcnew(gc, CImage);
				ExReg<CImage>(xml.GetAttr(_T("name"), ite), img);
			}
			img->Set(coder->Decode());
		}
	}

	void LoadRes(IFileObject*& file, CGC& exc_gc, CImage*& img, CGuiXML& xml, CGuiXML::iterator_t& ite)
	{
		file = gcnew(exc_gc, CMemFile);
		BYTE* buff = NULL; DWORD size = 0;
		HGLOBAL hres = ExGetBinary(_ttoi(xml.GetAttr(_T("path"), ite)), 
			_T("PNG"), buff, size, ::GetModuleHandle(NULL));
		if (hres)
		{
			((CMemFile*)file)->Open(buff, size);
			LoadImg(file, exc_gc, img, xml, ite);
			ExReleaseBinary(hres);
		}
	}

public:
	void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent)
	{
		CGC exc_gc;
		CImage* img = ExGet<CImage>(xml.GetAttr(_T("name"), ite));
		IFileObject* file = gcnew(exc_gc, CIOFile);
		((CIOFile*)file)->Open(xml.GetAttr(_T("path"), ite));
		if (file->Error())
		{
			CIOFile* xml_file = dynamic_cast<CIOFile*>(xml.GetFile());
			if (xml_file)
			{
				CString path(xml_file->GetFileName());
				path.GetAt(path.RevFind(_T('\\')) + 1) = _T('\0');
				path += xml.GetAttr(_T("path"), ite);
				((CIOFile*)file)->Open(path);
				if (file->Error())
					LoadRes(file, exc_gc, img, xml, ite);
				else
					LoadImg(file, exc_gc, img, xml, ite);
			}
			else
				LoadRes(file, exc_gc, img, xml, ite);
		}
		else
			LoadImg(file, exc_gc, img, xml, ite);
		return NULL;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_image, IExecutor)

//////////////////////////////////////////////////////////////////

// 样式
class _style : public IExecutor
{
	EXP_DECLARE_DYNCREATE_CLS(_style, IExecutor)

protected:
	CGC gc;

public:
	void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent)
	{
		style_t* sty = ExGet<style_t>(xml.GetAttr(_T("name"), ite));
		if(!sty)
		{
			sty = gcnew(gc, style_t);
			ExReg<style_t>(xml.GetAttr(_T("name"), ite), sty);
		}
		else
		{
			sty->font.Clear();
			sty->image.Clear();
		}
		CArrayT<CString> sa;
		ExStringToArray(xml.GetAttr(_T("font"), ite), sa);
		for(DWORD i = 0; i < sa.GetCount(); ++i)
			sty->font.Add(ExGet<CText>(sa[i]));
		ExStringToArray(xml.GetAttr(_T("color"), ite), sa);
		for(DWORD i = sty->color.GetCount(); i < sa.GetCount(); ++i)
			sty->color.Add(pixel_t());
		for(DWORD i = 0; i < sa.GetCount(); ++i)
			sty->color[i] = ExStringToColor(sa[i]);
		ExStringToArray(xml.GetAttr(_T("image"), ite), sa);
		for(DWORD i = 0; i < sa.GetCount(); ++i)
			sty->image.Add(ExGet<CImage>(sa[i]));
		return NULL;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_style, IExecutor)

//////////////////////////////////////////////////////////////////

// 层
class _div : public IExecutor
{
	EXP_DECLARE_DYNCREATE_CLS(_div, IExecutor)

private:
	CListT<void*> m_DelLst;

public:
	~_div()
	{
		for(CListT<void*>::iterator_t ite = m_DelLst.Head(); 
			ite != m_DelLst.Tail(); ++ite) del(*ite);
	}

public:
	/*
		每个层(窗口或控件),包括外部的自定义层的Execute算法皆不同
		只有层自己才知道如何进行自身的Execute算法
		因此此处的Execute将调用层自身的Execute接口进行脚本解析
	*/
	void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent)
	{
		void* div = ExDynCreate(xml.GetAttr(_T("class"), ite));
		if(!div) return NULL;
		IGuiWnd* brd = ExDynCast<IGuiWnd>(div);
		if (brd)
		{
			m_pNedDel = ExGet<IGuiWnd>(xml.GetAttr(_T("name"), ite));
			if (m_pNedDel) m_DelLst.Replace(m_pNedDel);
			brd = (IGuiWnd*)brd->Execute(xml, ite, parent);
			if (brd)
			{
				IGuiWnd* pare = ExDynCast<IGuiWnd>(parent);
				if (pare) brd->SetParent(pare->GethWnd());
				m_DelLst.Add(brd);
				return ExReg<IGuiWnd>(xml.GetAttr(_T("name"), ite), brd);
			}
			else
				return NULL;
		}
		else
		{
			IGuiCtl* ctl = ExDynCast<IGuiCtl>(div);
			if (ctl)
			{
				m_pNedDel = ExGet<IGuiCtl>(xml.GetAttr(_T("name"), ite));
				CListT<void*>::iterator_t it = m_DelLst.Find(m_pNedDel);
				if (it == m_DelLst.Tail())
					m_pNedDel = NULL;
				else
					m_DelLst.Del(it);
				ctl = (IGuiCtl*)ctl->Execute(xml, ite, parent);
				if (ctl)
				{
					ctl->SetWindowRect(ExStringToRect(xml.GetAttr(_T("rect"), ite)));
					CString t = xml.GetAttr(_T("visible"), ite); t.Lower();
					if (t == _T("false"))
						ctl->SetVisible(FALSE);
					else
					if (t == _T("true"))
						ctl->SetVisible(TRUE);
					IGuiBase* pare = ExDynCast<IGuiBase>(parent);
					if (pare)
						pare->AddComp(ctl);
					else
						m_DelLst.Add(ctl);
					return ExReg<IGuiCtl>(xml.GetAttr(_T("name"), ite), ctl);
				}
				else
					return NULL;
			}
			else
			{
				del(div);
				return NULL;
			}
		}
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_div, IExecutor)

//////////////////////////////////////////////////////////////////

// 子项
class _items : public IExecutor
{
	EXP_DECLARE_DYNCREATE_CLS(_items, IExecutor)

public:
	void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent)
	{
		IGuiCtl* pre = ExDynCast<IGuiCtl>(parent);
		if (!pre) return NULL;
		IGuiCtl::items_t* items = (IGuiCtl::items_t*)pre->GetState(_T("items"));
		if (!items) return NULL;
		CString t(xml.GetAttr(_T("count"), ite)), 
				c(xml.GetAttr(_T("class"), ite)), 
				e(xml.GetAttr(_T("event"), ite));
		IGuiEvent* pe = ExGet<IGuiEvent>(e);
		for(int i = 0; i < _ttoi(t); ++i)
		{
			IGuiCtl* ctl = ExGui<IGuiCtl>(c);
			if (!ctl) return NULL;
			ctl = (IGuiCtl*)ctl->Execute(xml, ite, parent);
			if (!ctl) return NULL;
			ctl->AddEvent(pe);
			items->Add(ctl);
		}
		pre->SetState(_T("items"), items);
		return NULL;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_items, IExecutor)

//////////////////////////////////////////////////////////////////

// 事件
class _event : public IExecutor
{
	EXP_DECLARE_DYNCREATE_CLS(_event, IExecutor)

public:
	void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent)
	{
		CString name = xml.GetAttr(_T("name"), ite);
		IGuiBase* pre = ExDynCast<IGuiBase>(parent);
		if (!pre && name.Empty()) return NULL;
		IGuiEvent* evt = ExDynCast<IGuiEvent>(ExDynCreate(xml.GetAttr(_T("class"), ite)));
		if (!evt) return NULL;
		if (!name.Empty()) ExReg<IGuiEvent>(name, evt);
		if (pre) pre->AddEvent(evt);
		return NULL;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_event, IExecutor)

//////////////////////////////////////////////////////////////////

// 效果
class _effect : public IExecutor
{
	EXP_DECLARE_DYNCREATE_CLS(_effect, IExecutor)

public:
	void* Execute(CGuiXML& xml, CGuiXML::iterator_t& ite, void* parent)
	{
		CString name = xml.GetAttr(_T("name"), ite);
		IGuiCtl* pre = ExDynCast<IGuiCtl>(parent);
		if (!pre && name.Empty()) return NULL;
		IGuiEffect* eff = ExDynCast<IGuiEffect>(ExDynCreate(xml.GetAttr(_T("class"), ite)));
		if (!eff) return NULL;
		if (!name.Empty()) ExReg<IGuiEffect>(name, eff);
		if (pre) pre->SetEffect(eff);
		return NULL;
	}
};

EXP_IMPLEMENT_DYNCREATE_CLS(_effect, IExecutor)

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiExecutor_hpp__*/
