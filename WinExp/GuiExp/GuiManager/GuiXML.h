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
// GuiXML - ���������ļ��ӿ�(XML����)
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2012-01-08
// Version:	1.0.0007.1617
//
// History:
//	- 1.0.0000.1420(2011-06-10)	@ ��ʼ����GuiXML
//	- 1.0.0001.1205(2011-06-14)	@ �������GuiXML�ĺ��Ĺ���
//	- 1.0.0002.1926(2011-06-15)	# ����������������tab��ʱ�޷�ʶ��,���������ֶ��е�����
//								+ ����һЩ���ݻ�ȡ�ӿ�
//	- 1.0.0003.1630(2011-09-15)	+ ����XMLע�͵Ķ�ȡ����
//								+ ����XML������;�޸�;ɾ��������ӿ�
//								+ ����XML�����εĽ���,���Ż�����״̬Ǩ��
//	- 1.0.0004.1121(2011-09-16)	+ ����CGuiXML::AddNode()�ӿ�����,֧��ֱ��ͨ�����ִ������
//	- 1.0.0005.2056(2011-09-18)	^ ��CGuiXML�еĲ���ʵ��
//	- 1.0.0006.1256(2011-10-10)	+ CGuiXML֧��UTF-8��ʽ��xml����
//	- 1.0.0007.1617(2012-01-08)	# ��������CGuiXML�ڲ�����ת���Ĵ���
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
		CString nam; // ����
		CString val; // ֵ
		map_t	att; // ����
		CString tmp; // ��ʱ�洢
	} node_t;
	typedef CTreeT<node_t*> tree_t;
	typedef tree_t::iterator_t iterator_t;

	// ��ȡ״̬
	enum state_t
	{
		sta_nor, // ����
		sta_jud, // �ж�
		sta_xml, // XML����
		sta_not, // ע��
		sta_tag, // ��ǩ
		sta_tco, // ��ǩ����
		sta_tce, // ��ǩ�������
		sta_wat, // �ȴ�����
		sta_avl, // ���Ը�ֵ
		sta_aco, // ��������
		sta_ace, // �����������
		sta_end	 // �ļ���ȡ���
	};

protected:
	IFileObject* m_pFile;
	CMemFile	 m_mFile;
	tree_t		 m_xData;
	CGC			 m_GC;
	CString		 m_sTemp;

public:
	CGuiXML()
		: m_pFile(NULL)
	{ Clear(); }
	virtual ~CGuiXML()
	{}

protected:
	// ����
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
		case _T('<'):	// �ж�
			eSta = sta_jud;
			break;
		default:		// ����
			eSta = sta_nor;
		}
		return TRUE;
	}
	// �ж�
	BOOL Jud(state_t& eSta, iterator_t& ite)
	{
		TCHAR buf = _T('\0');
		if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
			return FALSE;
		// ע���ж�
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
	// XML����
	BOOL Xml(state_t& eSta, iterator_t& ite)
	{
		m_xData.Add(ExMem::Alloc<node_t>(&m_GC), ite);
		node_t* node = *ite;
		node->nam += m_sTemp;
		m_sTemp = _T("");
		eSta = sta_tag;
		return TRUE;
	}
	// ע��
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
	// ��ǩ
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
		case _T(' '):	// �ȴ�����
		case _T('\r'):
		case _T('\n'):
		case _T('\t'):
			eSta = sta_wat;
			break;
		case _T('/'):
		case _T('?'):	// XML��������
			eSta = sta_tce;
			break;
		case _T('>'):
			eSta = sta_tco;
			break;
		default:
			if (eSta == sta_jud)
			{
				m_xData.Add(ExMem::Alloc<node_t>(&m_GC), ite);
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
	// ��ǩ����
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
	// ��ǩ�������
	BOOL Tce(state_t& eSta, iterator_t& ite)
	{
		ite = ite->Parent();
		eSta = sta_nor;
		return TRUE;
	}
	// �ȴ�����
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
		case _T('?'):	// XML��������
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
	// ���Ը�ֵ
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
	// ��������
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
	// �����������
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
			// д������
			buf = _T("<?xml");
			m_mFile.Write(buf.GetCStr(), buf.GetLength());
			// д������
			for(map_t::iterator_t it = node->att.Head(); it != node->att.Tail(); ++it)
			{
				buf.Format(_T(" %s=\"%s\""), (LPCTSTR)(it->Key()), (LPCTSTR)(it->Val()));
				m_mFile.Write(buf.GetCStr(), buf.GetLength());
				if (it->Key() == _T("encoding")) enc = it->Val();
			}
			// ���
			buf = _T("?>\r\n");
			m_mFile.Write(buf.GetCStr(), buf.GetLength());
		}
		else
		if (node->nam != _T(""))
		{
			// д������
			buf.Format(_T("<%s"), (LPCTSTR)(node->nam));
			m_mFile.Write(buf.GetCStr(), buf.GetLength());

			// д������
			for(map_t::iterator_t it = node->att.Head(); it != node->att.Tail(); ++it)
			{
				buf.Format(_T(" %s=\"%s\""), (LPCTSTR)(it->Key()), (LPCTSTR)(it->Val()));
				m_mFile.Write(buf.GetCStr(), buf.GetLength());
			}

			if (node->val != _T(""))
			{	// д��ֵ
				buf.Format(_T(">%s</%s>"), (LPCTSTR)(node->val), (LPCTSTR)(node->nam));
				m_mFile.Write(buf.GetCStr(), buf.GetLength());
			}
			else
			{	// д���ӽڵ�
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
		{	// д���ӽڵ�
			list_t list = ite->Children();
			if(!list.Empty())
			{
				for(list_t::iterator_t it = list.Head(); it != list.Tail(); ++it)
					Encode(*it, enc);
			}
		}
	}

public:
	void SetFile(IFileObject* pFile)
	{ m_pFile = pFile; }
	IFileObject* GetFile()
	{ return m_pFile; }

	void Clear()
	{
		m_xData.Clear();
		m_GC.Clear();
		m_xData.Add(ExMem::Alloc<node_t>(&m_GC)); // ����Ĭ�ϵĸ��ڵ�
		m_mFile.Open();
	}

	// ����
	BOOL Encode()
	{
		if (!m_pFile) return FALSE;
		// �ļ����
		if (!m_pFile->Clear()) return FALSE;
		if (!m_mFile.Clear()) return FALSE;
		// ��ʼ����
		CString enc(_T("utf-8"));
		Encode(m_xData.Head(), enc);
		enc.Lower();
		// �ж��ļ����벢ת��
		CGC gc; char* tmp_str = NULL; int len = (int)m_mFile.Size();
	#ifdef	_UNICODE
		if (enc == _T("utf-8"))
			CString::WideCharToMultiByte(CP_UTF8, (LPCWSTR)(LPCVOID)m_mFile, 
				len / sizeof(wchar_t), tmp_str, len, &gc);
		else
		if (enc == _T("gb2312"))
			CString::WideCharToMultiByte(CP_ACP, (LPCWSTR)(LPCVOID)m_mFile, 
				len / sizeof(wchar_t), tmp_str, len, &gc);
	#else /*_UNICODE*/
		if (enc == _T("utf-8"))
		{
			wchar_t* tmp_unc = NULL;
			CString::MultiByteToWideChar(CP_ACP, (LPCSTR)(LPCVOID)m_mFile, 
				len, tmp_unc, len, &gc);
			CString::WideCharToMultiByte(CP_UTF8, tmp_unc, len, tmp_str, len, &gc);
		}
		else
		if (enc == _T("gb2312"))
			tmp_str = (LPCSTR)(LPCVOID)m_mFile;
	#endif/*_UNICODE*/
		return (m_pFile->Write(tmp_str, len, sizeof(char)) == len);
	}
	// ����
	BOOL Decode()
	{
		if (!m_pFile) return FALSE;
		CFileSeeker seeker(m_pFile);
		if (!m_pFile->Seek(0, IFileObject::begin)) return FALSE;
		Clear();
		// ����ת��
		uint64_t len64 = m_pFile->Size();
		if (len64 > (1 << 30)) return FALSE;			// ���Դ���1G���ļ�
		{
			CString enc(_T("utf-8"));
			CStringT<char> buf; buf.GetCStr(1 << 19);	// 1M�Ļ�����(CStringTĬ�ϻὫ�����С������չ)
			// ��ȷ����������
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
			// ��ʼת������
			if (!m_pFile->Seek(0, IFileObject::begin)) return FALSE;
			while(m_pFile->Read(buf.GetCStr(), buf.GetSize(), sizeof(char)) != 0)
			{
				CGC gc; TCHAR* tmp_str = NULL; int len = 0;
			#ifdef	_UNICODE
				if (enc == _T("utf-8"))
					CString::MultiByteToWideChar(CP_UTF8, buf.GetCStr(), -1, tmp_str, len, &gc);
				else
				if (enc == _T("gb2312"))
					CString::MultiByteToWideChar(CP_ACP, buf.GetCStr(), -1, tmp_str, len, &gc);
			#else /*_UNICODE*/
				if (enc == _T("utf-8"))
				{
					wchar_t* tmp_unc = NULL;
					CString::MultiByteToWideChar(CP_UTF8, buf.GetCStr(), -1, tmp_unc, len, &gc);
					CString::WideCharToMultiByte(CP_ACP, tmp_unc, -1, tmp_str, len, &gc);
				}
				else
				if (enc == _T("gb2312"))
					tmp_str = buf.GetCStr();
			#endif/*_UNICODE*/
				if (m_mFile.Write(tmp_str, len) != len) break;
			}
			if (!m_mFile.Seek(0, IFileObject::begin)) return FALSE;
		}
		// ��ʼ����
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

	iterator_t GetRoot()
	{
		return m_xData.Head();
	}
	BOOL GetNode(_IN_ LPCTSTR sName, _IN_OT_ iterator_t& rIte)
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
	CString GetAttr(_IN_ LPCTSTR sAttr, _IN_ iterator_t& rIte)
	{
		if (!sAttr) return _T("");
		if (rIte == m_xData.Head() || 
			rIte == m_xData.Tail()) return _T("");
		map_t::iterator_t ite_att = (*rIte)->att.Locate(sAttr);
		if (ite_att == (*rIte)->att.Tail()) return _T("");
		return ite_att->Val();
	}

	BOOL AddNode(const node_t& node, iterator_t& ite)
	{
		node_t* p = ExMem::Alloc<node_t>(&m_GC);
		(*p) = node;
		return m_xData.Add(p, ite);
	}
	BOOL AddNode(LPCTSTR sName, iterator_t& ite)
	{
		node_t* p = ExMem::Alloc<node_t>(&m_GC);
		p->nam = sName;
		return m_xData.Add(p, ite);
	}
	BOOL DelNode(iterator_t& ite)
	{
		return m_xData.Del(ite);
	}

	BOOL AddAttr(LPCTSTR key, LPCTSTR val, iterator_t& ite)
	{
		if (!key) return FALSE;
		if (ite == m_xData.Head() || 
			ite == m_xData.Tail()) return FALSE;
		(*ite)->att[key] = val;
		return TRUE;
	}
	BOOL DelAttr(LPCTSTR key, iterator_t& ite)
	{
		if (!key) return FALSE;
		if (ite == m_xData.Head() || 
			ite == m_xData.Tail()) return FALSE;
		return (*ite)->att.Del(key);
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiXML_h__*/