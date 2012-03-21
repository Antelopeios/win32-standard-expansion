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
// GuiXML - 界面配置文件接口(XML解析)
//
// Author:	木头云
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-03-02
// Version:	1.0.0009.1121
//
// History:
//	- 1.0.0000.1420(2011-06-10)	@ 开始构建GuiXML
//	- 1.0.0001.1205(2011-06-14)	@ 基本完成GuiXML的核心功能
//	- 1.0.0002.1926(2011-06-15)	# 修正当遇到换行与tab键时无法识别,并添加入字段中的问题
//								+ 添加一些数据获取接口
//	- 1.0.0003.1630(2011-09-15)	+ 添加XML注释的读取过程
//								+ 添加XML的添加;修改;删除及保存接口
//								+ 添加XML声明段的解析,并优化解析状态迁移
//	- 1.0.0004.1121(2011-09-16)	+ 添加CGuiXML::AddNode()接口重载,支持直接通过名字创建结点
//	- 1.0.0005.2056(2011-09-18)	^ 简化CGuiXML中的部分实现
//	- 1.0.0006.1256(2011-10-10)	+ CGuiXML支持UTF-8格式的xml解析
//	- 1.0.0007.1617(2012-01-08)	# 修正若干CGuiXML内部编码转换的错误
//	- 1.0.0008.1802(2012-02-02)	+ 添加CGuiXML::SetDefEnc()与CGuiXML::GetDefEnc()
//	- 1.0.0009.1121(2012-03-02)	+ 在CGuiXML中添加项名称与值的控制接口
//								= CGuiXML::AddAttr()改为CGuiXML::SetAttr()
//////////////////////////////////////////////////////////////////

#ifndef __GuiXML_h__
#define __GuiXML_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

EXP_BEG

//////////////////////////////////////////////////////////////////

class CGuiXML
{
public:
	typedef CMapT<CString, CString> map_t;
	typedef struct _Node
	{
		friend class CGuiXML;

		CString nam; // 名称
		CString val; // 值
		map_t	att; // 属性

	private:
		CString tmp; // 临时存储

	public:
		BOOL IsValid() const { return (!nam.Empty()); }
	} node_t;
	typedef CTreeT<node_t*> tree_t;
	typedef tree_t::iterator_t iterator_t;
	typedef tree_t::ite_list_t ite_list_t;

	// 读取状态
	enum state_t
	{
		sta_nor, // 正常
		sta_jud, // 判断
		sta_xml, // XML声明
		sta_not, // 注释
		sta_tag, // 标签
		sta_tco, // 标签内容
		sta_tce, // 标签内容完毕
		sta_wat, // 等待属性
		sta_avl, // 属性赋值
		sta_aco, // 属性内容
		sta_ace, // 属性内容完毕
		sta_end	 // 文件读取完毕
	};

protected:
	IFileObject* m_pFile;
	CMemFile	 m_mFile;
	tree_t		 m_xData;
	CGC			 m_GC;
	CString		 m_sTemp;
	CString		 m_sEnc;

public:
	CGuiXML()
		: m_pFile(NULL)
		, m_sEnc(_T("utf-8"))
	{ Clear(); }
	virtual ~CGuiXML()
	{}

protected:
	// 正常
	BOOL Nor(state_t& eSta, iterator_t& ite)
	{
		TCHAR buf = _T('\0');
		if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
		{
			eSta = sta_end;
			return TRUE;
		}
		switch (buf)
		{
		case _T('<'):	// 判断
			eSta = sta_jud;
			break;
		default:		// 正常
			eSta = sta_nor;
		}
		return TRUE;
	}
	// 判断
	BOOL Jud(state_t& eSta, iterator_t& ite)
	{
		TCHAR buf = _T('\0');
		if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
			return FALSE;
		// 注释判断
		if (sta_jud == eSta)
		{
			m_sTemp += buf;
			if (m_sTemp.GetLength() <= 4)
			{
				if (m_sTemp == _T("!--"))
				{
					m_sTemp = _T("");
					eSta = sta_not;
				}
				else
				if (m_sTemp == _T("?xml"))
					eSta = sta_xml;
				else
					eSta = sta_jud;
			}
			else
				eSta = sta_tag;
		}
		else
		{
			if (buf == _T('!') || 
				buf == _T('?'))
			{
				m_sTemp = buf;
				eSta = sta_jud;
			}
			else
			if (buf == _T('/'))
				eSta = sta_tce;
			else
			{
				m_sTemp = buf;
				eSta = sta_tag;
			}
		}
		return TRUE;
	}
	// XML声明
	BOOL Xml(state_t& eSta, iterator_t& ite)
	{
		m_xData.Add(gcnew(m_GC, node_t), ite);
		node_t* node = *ite;
		node->nam += m_sTemp;
		m_sTemp = _T("");
		eSta = sta_tag;
		return TRUE;
	}
	// 注释
	BOOL Not(state_t& eSta, iterator_t& ite)
	{
		TCHAR buf = _T('\0');
		if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
			return FALSE;
		switch (buf)
		{
		case _T('-'):
			if (m_sTemp == _T("-"))
				m_sTemp += buf;
			else
				m_sTemp = buf;
			eSta = sta_not;
			break;
		case _T('>'):
			if (m_sTemp == _T("--"))
			{
				m_sTemp = _T("");
				eSta = sta_nor;
			}
			else
			{
				m_sTemp = buf;
				eSta = sta_not;
			}
			break;
		default:
			m_sTemp = buf;
			eSta = sta_not;
		}
		return TRUE;
	}
	// 标签
	BOOL Tag(state_t& eSta, iterator_t& ite)
	{
		TCHAR buf = _T('\0');
		if (m_sTemp == _T(""))
		{
			if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
				return FALSE;
		}
		else
		{
			buf = m_sTemp[0];
			m_sTemp.Del(m_sTemp.Head());
		}
		switch (buf)
		{
		case _T(' '):	// 等待属性
		case _T('\r'):
		case _T('\n'):
		case _T('\t'):
			eSta = sta_wat;
			break;
		case _T('/'):
		case _T('?'):	// XML声明结束
			eSta = sta_tce;
			break;
		case _T('>'):
			eSta = sta_tco;
			break;
		default:
			if (eSta == sta_jud)
			{
				m_xData.Add(gcnew(m_GC, node_t), ite);
				node_t* node = *ite;
				node->nam += buf;
			}
			else
			if (eSta == sta_tag)
			{
				node_t* node = *ite;
				node->nam += buf;
			}
			eSta = sta_tag;
		}
		return TRUE;
	}
	// 标签内容
	BOOL Tco(state_t& eSta, iterator_t& ite)
	{
		if (ite == m_xData.Head()) return FALSE;
		TCHAR buf = _T('\0');
		if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
			return FALSE;
		switch (buf)
		{
		case _T('<'):
			eSta = sta_jud;
			break;
		default:
			if (buf >= 32)
			{
				node_t* node = *ite;
				node->val += buf;
			}
			eSta = sta_tco;
			break;
		}
		return TRUE;
	}
	// 标签内容完毕
	BOOL Tce(state_t& eSta, iterator_t& ite)
	{
		ite = ite->Parent();
		eSta = sta_nor;
		return TRUE;
	}
	// 等待属性
	BOOL Wat(state_t& eSta, iterator_t& ite)
	{
		TCHAR buf = _T('\0');
		if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
			return FALSE;
		switch (buf)
		{
		case _T('<'):
		case _T(' '):
		case _T('\r'):
		case _T('\n'):
		case _T('\t'):
		case _T('\"'):
			eSta = sta_wat;
			break;
		case _T('/'):
		case _T('?'):	// XML声明结束
			eSta = sta_tce;
			break;
		case _T('>'):
			eSta = sta_tco;
			break;
		case _T('='):
			{
				node_t* node = *ite;
				if (node->tmp.Empty())
					return FALSE;
				else
					node->att[node->tmp] = _T("");
			}
			eSta = sta_avl;
			break;
		default:
			{
				node_t* node = *ite;
				node->tmp += buf;
			}
			eSta = sta_wat;
			break;
		}
		return TRUE;
	}
	// 属性赋值
	BOOL Avl(state_t& eSta, iterator_t& ite)
	{
		TCHAR buf = _T('\0');
		if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
			return FALSE;
		switch (buf)
		{
		case _T('\"'):
			eSta = sta_aco;
			break;
		default:
			eSta = sta_avl;
			break;
		}
		return TRUE;
	}
	// 属性内容
	BOOL Aco(state_t& eSta, iterator_t& ite)
	{
		TCHAR buf = _T('\0');
		if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
			return FALSE;
		switch (buf)
		{
		case _T('\"'):
			eSta = sta_ace;
			break;
		default:
			{
				node_t* node = *ite;
				node->att[node->tmp] += buf;
			}
			eSta = sta_aco;
			break;
		}
		return TRUE;
	}
	// 属性内容完毕
	BOOL Ace(state_t& eSta, iterator_t& ite)
	{
		node_t* node = *ite;
		node->tmp.Clear();
		eSta = sta_wat;
		return TRUE;
	}

	void Encode(iterator_t& ite, CString& enc)
	{
		typedef CListT<iterator_t> list_t;

		node_t* node = ite->Val();
		CString buf;

		if (node->nam == _T("?xml"))
		{
			// 写入名称
			buf = _T("<?xml");
			m_mFile.Write(buf.GetCStr(), buf.GetLength());
			// 写入属性
			for(map_t::iterator_t it = node->att.Head(); it != node->att.Tail(); ++it)
			{
				buf.Format(_T(" %s=\"%s\""), (LPCTSTR)(it->Key()), (LPCTSTR)(it->Val()));
				m_mFile.Write(buf.GetCStr(), buf.GetLength());
				if (it->Key() == _T("encoding")) enc = it->Val();
			}
			// 完毕
			buf = _T("?>\r\n");
			m_mFile.Write(buf.GetCStr(), buf.GetLength());
		}
		else
		if (node->nam != _T(""))
		{
			// 写入名称
			buf.Format(_T("<%s"), (LPCTSTR)(node->nam));
			m_mFile.Write(buf.GetCStr(), buf.GetLength());

			// 写入属性
			for(map_t::iterator_t it = node->att.Head(); it != node->att.Tail(); ++it)
			{
				buf.Format(_T(" %s=\"%s\""), (LPCTSTR)(it->Key()), (LPCTSTR)(it->Val()));
				m_mFile.Write(buf.GetCStr(), buf.GetLength());
			}

			if (node->val != _T(""))
			{	// 写入值
				buf.Format(_T(">%s</%s>"), (LPCTSTR)(node->val), (LPCTSTR)(node->nam));
				m_mFile.Write(buf.GetCStr(), buf.GetLength());
			}
			else
			{	// 写入子节点
				list_t list = ite->Children();
				if(!list.Empty())
				{
					buf = _T(">\r\n");
					m_mFile.Write(buf.GetCStr(), buf.GetLength());
					for(list_t::iterator_t it = list.Head(); it != list.Tail(); ++it)
						Encode(*it, enc);
					buf.Format(_T("</%s>\r\n"), (LPCTSTR)(node->nam));
					m_mFile.Write(buf.GetCStr(), buf.GetLength());
				}
				else
				{
					buf = _T("/>\r\n");
					m_mFile.Write(buf.GetCStr(), buf.GetLength());
				}
			}
		}
		else
		{	// 写入子节点
			list_t list = ite->Children();
			if(!list.Empty())
			{
				for(list_t::iterator_t it = list.Head(); it != list.Tail(); ++it)
					Encode(*it, enc);
			}
		}
	}

public:
	EXP_INLINE void SetFile(IFileObject* pFile)
	{ m_pFile = pFile; }
	EXP_INLINE IFileObject* GetFile() const
	{ return m_pFile; }

	EXP_INLINE void SetDefEnc(const CString& enc)
	{ m_sEnc = enc; }
	EXP_INLINE const CString GetDefEnc() const
	{ return m_sEnc; }

	EXP_INLINE void Clear()
	{
		m_xData.Clear();
		m_GC.Clear();
		m_xData.Add(gcnew(m_GC, node_t)); // 添加默认的根节点
		m_mFile.Open();
	}

	// 编码
	BOOL Encode()
	{
		if (!m_pFile) return FALSE;
		// 文件清空
		if (!m_pFile->Clear()) return FALSE;
		if (!m_mFile.Clear()) return FALSE;
		// 开始编码
		CString enc(m_sEnc);
		Encode(m_xData.Head(), enc);
		enc.Lower();
		// 判断文件编码并转换
		CGC gc; char* tmp_str = NULL; int len = (int)m_mFile.Size();
	#ifdef	_UNICODE
		if (enc == _T("utf-8"))
		{
			CString::WideCharToMultiByte(CP_UTF8, (LPCWSTR)(LPCVOID)m_mFile, 
				len / sizeof(wchar_t), tmp_str, len);
			gc.Regist(tmp_str);
		}
		else
		if (enc == _T("gb2312"))
		{
			CString::WideCharToMultiByte(CP_ACP, (LPCWSTR)(LPCVOID)m_mFile, 
				len / sizeof(wchar_t), tmp_str, len);
			gc.Regist(tmp_str);
		}
	#else /*_UNICODE*/
		if (enc == _T("utf-8"))
		{
			wchar_t* tmp_unc = NULL;
			CString::MultiByteToWideChar(CP_ACP, (LPCSTR)(LPCVOID)m_mFile, 
				len, tmp_unc, len);
			gc.Regist(tmp_unc);
			CString::WideCharToMultiByte(CP_UTF8, tmp_unc, len, tmp_str, len);
			gc.Regist(tmp_str);
		}
		else
		if (enc == _T("gb2312"))
			tmp_str = (LPCSTR)(LPCVOID)m_mFile;
	#endif/*_UNICODE*/
		return (m_pFile->Write(tmp_str, len, sizeof(char)) == len);
	}
	// 解码
	BOOL Decode()
	{
		if (!m_pFile) return FALSE;
		CFileSeeker seeker(m_pFile);
		if (!m_pFile->Seek(0, IFileObject::begin)) return FALSE;
		Clear();
		// 编码转换
		uint64_t len64 = m_pFile->Size();
		if (len64 > (1 << 30)) return FALSE;			// 忽略大于1G的文件
		{
			CString enc(m_sEnc);
			CStringT<char> buf; buf.GetCStr(1 << 19);	// 1M的缓存区(CStringT默认会将传入大小翻倍拓展)
			// 先确定编码类型
			while(m_pFile->Read(buf.GetCStr(), buf.GetSize(), sizeof(char)) != 0)
			{
				CStringT<char>::iterator_t ite_stt = buf.Find("<?xml");
				CStringT<char>::iterator_t ite_end = buf.Find(ite_stt, buf.Tail(), "?>");
				if (ite_stt != buf.Tail() && ite_end != buf.Tail())
				{
					CStringT<char> tmp(buf.Mid(ite_stt->Index(), ite_end->Index() - ite_stt->Index()));
					tmp.Lower();
					if (tmp.Find("encoding=\"utf-8\"") != tmp.Tail())
						enc = _T("utf-8");
					else
					if (tmp.Find("encoding=\"gb2312\"") != tmp.Tail())
						enc = _T("gb2312");
					break;
				}
			}
			// 开始转换编码
			if (!m_pFile->Seek(0, IFileObject::begin)) return FALSE;
			while(m_pFile->Read(buf.GetCStr(), buf.GetSize(), sizeof(char)) != 0)
			{
				CGC gc; TCHAR* tmp_str = NULL; int len = 0;
			#ifdef	_UNICODE
				if (enc == _T("utf-8"))
				{
					CString::MultiByteToWideChar(CP_UTF8, buf.GetCStr(), -1, tmp_str, len);
					gc.Regist(tmp_str);
				}
				else
				if (enc == _T("gb2312"))
				{
					CString::MultiByteToWideChar(CP_ACP, buf.GetCStr(), -1, tmp_str, len);
					gc.Regist(tmp_str);
				}
			#else /*_UNICODE*/
				if (enc == _T("utf-8"))
				{
					wchar_t* tmp_unc = NULL;
					CString::MultiByteToWideChar(CP_UTF8, buf.GetCStr(), -1, tmp_unc, len);
					gc.Regist(tmp_unc);
					CString::WideCharToMultiByte(CP_ACP, tmp_unc, -1, tmp_str, len);
					gc.Regist(tmp_str);
				}
				else
				if (enc == _T("gb2312"))
					tmp_str = buf.GetCStr();
			#endif/*_UNICODE*/
				if (m_mFile.Write(tmp_str, len) != len) break;
			}
			if (!m_mFile.Seek(0, IFileObject::begin)) return FALSE;
		}
		// 开始解析
		BOOL ret = FALSE;
		state_t cur_sta = sta_nor, old_sta = sta_nor;
		iterator_t ite = m_xData.Head();
		while(1)
		{
			state_t tmp_sta = cur_sta;
			cur_sta = old_sta;
			switch(tmp_sta)
			{
			case sta_nor:
				if (!Nor(cur_sta, ite))
					goto Return;
				break;
			case sta_jud:
				if (!Jud(cur_sta, ite))
					goto Return;
				break;
			case sta_xml:
				if (!Xml(cur_sta, ite))
					goto Return;
				break;
			case sta_not:
				if (!Not(cur_sta, ite))
					goto Return;
				break;
			case sta_tag:
				if (!Tag(cur_sta, ite))
					goto Return;
				break;
			case sta_tco:
				if (!Tco(cur_sta, ite))
					goto Return;
				break;
			case sta_tce:
				if (!Tce(cur_sta, ite))
					goto Return;
				break;
			case sta_wat:
				if (!Wat(cur_sta, ite))
					goto Return;
				break;
			case sta_avl:
				if (!Avl(cur_sta, ite))
					goto Return;
				break;
			case sta_aco:
				if (!Aco(cur_sta, ite))
					goto Return;
				break;
			case sta_ace:
				if (!Ace(cur_sta, ite))
					goto Return;
				break;
			case sta_end:
				ret = TRUE;
			default:
				goto Return;
			}
			old_sta = tmp_sta;
		}
	Return:
		return ret;
	}

	iterator_t GetRoot() const
	{
		return m_xData.Head();
	}

	BOOL GetNode(_IN_ LPCTSTR sName, _IN_OT_ iterator_t& rIte) const
	{
		if (!sName) return FALSE;
		if (rIte == iterator_t())
			rIte = m_xData.Head();
		++rIte;
		for(; rIte != m_xData.Tail(); ++rIte)
		{
			if (!(*rIte)) continue;
			if ((*rIte)->nam == sName)
				return TRUE;
		}
		return FALSE;
	}
	CString GetAttr(_IN_ LPCTSTR sAttr, _IN_ iterator_t& rIte) const
	{
		if (!sAttr) return _T("");
		if (rIte == m_xData.Head() || 
			rIte == m_xData.Tail()) return _T("");
		map_t::iterator_t ite_att = (*rIte)->att.Locate(sAttr);
		if (ite_att == (*rIte)->att.Tail()) return _T("");
		return ite_att->Val();
	}
	CString GetName(_IN_ iterator_t& rIte) const
	{
		if (rIte == m_xData.Head() || 
			rIte == m_xData.Tail()) return _T("");
		return (*rIte)->nam;
	}
	CString GetVal(_IN_ iterator_t& rIte) const
	{
		if (rIte == m_xData.Head() || 
			rIte == m_xData.Tail()) return _T("");
		return (*rIte)->val;
	}

	BOOL AddNode(const node_t& node, iterator_t& ite)
	{
		if(!node.IsValid()) return FALSE;
		node_t* p = gcnew(m_GC, node_t);
		(*p) = node;
		return m_xData.Add(p, ite);
	}
	BOOL AddNode(LPCTSTR sName, iterator_t& ite)
	{
		if (CString(sName).Empty()) return FALSE;
		node_t* p = gcnew(m_GC, node_t);
		p->nam = sName;
		return m_xData.Add(p, ite);
	}
	BOOL DelNode(iterator_t& ite)
	{
		return m_xData.Del(ite);
	}

	BOOL SetAttr(LPCTSTR key, LPCTSTR val, iterator_t& ite)
	{
		if (CString(key).Empty()) return FALSE;
		if (ite == m_xData.Head() || 
			ite == m_xData.Tail()) return FALSE;
		(*ite)->att[key] = val;
		return TRUE;
	}
	BOOL DelAttr(LPCTSTR key, iterator_t& ite)
	{
		if (CString(key).Empty()) return FALSE;
		if (ite == m_xData.Head() || 
			ite == m_xData.Tail()) return FALSE;
		return (*ite)->att.Del(key);
	}

	BOOL SetName(LPCTSTR sName, iterator_t& ite)
	{
		if (CString(sName).Empty()) return FALSE;
		if (ite == m_xData.Head() || 
			ite == m_xData.Tail()) return FALSE;
		(*ite)->nam = sName;
		return TRUE;
	}
	BOOL SetVal(LPCTSTR sVal, iterator_t& ite)
	{
		if (CString(sVal).Empty()) return FALSE;
		if (ite == m_xData.Head() || 
			ite == m_xData.Tail()) return FALSE;
		(*ite)->val = sVal;
		return TRUE;
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiXML_h__*/