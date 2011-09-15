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
// GuiXML - ���������ļ��ӿ�(XML����)
//
// Author:	ľͷ��
// Home:	dark-c.at
// E-Mail:	mark.lonr@tom.com
// Date:	2011-09-15
// Version:	1.0.0003.1024
//
// History:
//	- 1.0.0000.1420(2011-06-10)	@ ��ʼ����GuiXML
//	- 1.0.0001.1205(2011-06-14)	@ �������GuiXML�ĺ��Ĺ���
//	- 1.0.0002.1926(2011-06-15)	# ����������������tab��ʱ�޷�ʶ��,��������ֶ��е�����
//								+ ���һЩ���ݻ�ȡ�ӿ�
//	- 1.0.0003.1024(2011-09-15)	+ ���xmlע�͵Ķ�ȡ����
//								+ ���xml�����;�޸�;ɾ��������ӿ�
//////////////////////////////////////////////////////////////////

#ifndef __GuiXML_hpp__
#define __GuiXML_hpp__

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
	bool Nor(state_t& eSta, iterator_t& ite)
	{
		if (eSta != sta_nor && 
			eSta != sta_not && 
			eSta != sta_tce && 
			eSta != sta_wat) return false;
		TCHAR buf = _T('\0');
		if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
		{
			eSta = sta_end;
			return true;
		}
		switch (buf)
		{
		case _T('<'):	// ��ǩ
			eSta = sta_tag;
			break;
		default:		// ����
			eSta = sta_nor;
		}
		return true;
	}
	// ע��
	bool Not(state_t& eSta, iterator_t& ite)
	{
		if (eSta != sta_not && 
			eSta != sta_tag) return false;
		TCHAR buf = _T('\0');
		if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
			return false;
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
		return true;
	}
	// ��ǩ
	bool Tag(state_t& eSta, iterator_t& ite)
	{
		if (eSta != sta_tag && 
			eSta != sta_nor && 
			eSta != sta_tco && 
			eSta != sta_ace) return false;
		TCHAR buf = _T('\0');
		if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
			return false;

		if (sta_tag != eSta && buf == _T('!'))
		{
			m_sTemp = buf;
			eSta = sta_tag;
			return true;
		}
		else
		if (m_sTemp != _T(""))
		{
			m_sTemp += buf;
			if (m_sTemp.GetLength() < 3)
			{
				eSta = sta_tag;
				return true;
			}
			else
			if (m_sTemp == _T("!--"))
			{
				m_sTemp = _T("");
				eSta = sta_not;
				return true;
			}
		}

		switch (buf)
		{
		case _T('>'):	// ��ǩ����
			eSta = sta_tco;
			break;
		case _T(' '):	// �ȴ�����
		case _T('\r'):
		case _T('\n'):
		case _T('\t'):
			eSta = sta_wat;
			break;
		case _T('/'):	// ��ǩ���ݸ�ֵ���
			eSta = sta_tce;
			break;
		default:
			if (eSta == sta_nor || 
				eSta == sta_tco)
			{
				m_xData.Add(ExMem::Alloc<node_t>(&m_GC), ite);
				node_t* node = *ite;
				node->nam += buf;
			}
			else
			if (eSta == sta_tag)
			{
				node_t* node = *ite;
				if (m_sTemp == _T(""))
					node->nam += buf;
				else
				{
					node->nam += m_sTemp;
					m_sTemp = _T("");
				}
			}
			eSta = sta_tag;
		}
		return true;
	}
	// ��ǩ����
	bool Tco(state_t& eSta, iterator_t& ite)
	{
		if (eSta != sta_tco && 
			eSta != sta_tag && 
			eSta != sta_wat) return false;
		if (ite == m_xData.Head()) return false;
		TCHAR buf = _T('\0');
		if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
			return false;
		switch (buf)
		{
		case _T('<'):
			eSta = sta_tag;
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
		return true;
	}
	// ��ǩ�������
	bool Tce(state_t& eSta, iterator_t& ite)
	{
		if (eSta != sta_tag && 
			eSta != sta_wat) return false;
		ite = ite->Parent();
		eSta = sta_nor;
		return true;
	}
	// �ȴ�����
	bool Wat(state_t& eSta, iterator_t& ite)
	{
		if (eSta != sta_wat && 
			eSta != sta_tag) return false;
		TCHAR buf = _T('\0');
		if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
			return false;
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
			eSta = sta_tce;
			break;
		case _T('>'):
			eSta = sta_tco;
			break;
		case _T('='):
			{
				node_t* node = *ite;
				if (node->tmp.Empty())
					return false;
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
		return true;
	}
	// ���Ը�ֵ
	bool Avl(state_t& eSta, iterator_t& ite)
	{
		if (eSta != sta_avl && 
			eSta != sta_wat) return false;
		TCHAR buf = _T('\0');
		if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
			return false;
		switch (buf)
		{
		case _T('\"'):
			eSta = sta_aco;
			break;
		default:
			eSta = sta_avl;
			break;
		}
		return true;
	}
	// ��������
	bool Aco(state_t& eSta, iterator_t& ite)
	{
		if (eSta != sta_aco && 
			eSta != sta_avl) return false;
		TCHAR buf = _T('\0');
		if (m_mFile.Read(&buf, 1, sizeof(TCHAR)) != 1)
			return false;
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
		return true;
	}
	// �����������
	bool Ace(state_t& eSta, iterator_t& ite)
	{
		if (eSta != sta_aco) return false;
		node_t* node = *ite;
		node->tmp.Clear();
		eSta = sta_tag;
		return true;
	}

	void Encode(iterator_t& ite)
	{
		typedef CListT<iterator_t> list_t;

		node_t* node = ite->Val();
		CStringT<char> buf;

		if (node->nam != "")
		{
			// д������
			buf = "<";
			buf += node->nam;
			m_pFile->Write(buf.GetCStr(), buf.GetLength(), sizeof(char));

			// д������
			for(map_t::iterator_t it = node->att.Head(); it != node->att.Tail(); ++it)
			{
				buf = " ";
				buf += it->Key();
				buf += "=\"";
				buf += it->Val();
				buf += "\"";
				m_pFile->Write(buf.GetCStr(), buf.GetLength(), sizeof(char));
			}

			if (node->val != "")
			{	// д��ֵ
				buf = ">";
				buf += node->val;
				buf += "</";
				buf += node->nam;
				buf += ">";
				m_pFile->Write(buf.GetCStr(), buf.GetLength(), sizeof(char));
			}
			else
			{	// д���ӽڵ�
				list_t list = ite->Children();
				if(!list.Empty())
				{
					buf = ">\r\n";
					m_pFile->Write(buf.GetCStr(), buf.GetLength(), sizeof(char));
					for(list_t::iterator_t it = list.Head(); it != list.Tail(); ++it)
						Encode(*it);
					buf = "</";
					buf += node->nam;
					buf += ">\r\n";
					m_pFile->Write(buf.GetCStr(), buf.GetLength(), sizeof(char));
				}
				else
				{
					buf = "/>\r\n";
					m_pFile->Write(buf.GetCStr(), buf.GetLength(), sizeof(char));
				}
			}
		}
		else
		{	// д���ӽڵ�
			list_t list = ite->Children();
			if(!list.Empty())
			{
				for(list_t::iterator_t it = list.Head(); it != list.Tail(); ++it)
					Encode(*it);
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
		m_xData.Add(ExMem::Alloc<node_t>(&m_GC)); // ���Ĭ�ϵĸ��ڵ�
		m_mFile.Open();
	}

	// ����
	bool Encode()
	{
		if (!m_pFile) return false;
		if (!m_pFile->SetSize(0))
			return false;
		// ��ʼ����
		Encode(m_xData.Head());
		return true;
	}
	// ����
	bool Decode()
	{
		if (!m_pFile) return false;
		if (!m_pFile->Seek(0, IFileObject::begin))
			return false;
		Clear();
		// ����ת��
		uint64_t len64 = m_pFile->Size();
		if (len64 > (1 << 30)) return false;		// ���Դ���1G���ļ�
		char* buf = ExMem::Alloc<char>(1 << 20);	// 1M�Ļ�����
		while(m_pFile->Read(buf, 1 << 20, sizeof(char)) != 0)
		{
			CString oem(buf);
			if (m_mFile.Write(oem.GetCStr(), oem.GetLength()) != oem.GetLength())
				break;
		}
		ExMem::Free(buf);
		if(!m_mFile.Seek(0, IFileObject::begin))
			return false;
		// ��ʼ����
		bool ret = false;
		state_t cur_sta = sta_nor, old_sta = sta_nor;
		iterator_t ite = m_xData.Head();
		while(1)
		{
			switch(cur_sta)
			{
			case sta_nor:
				cur_sta = old_sta;
				if (!Nor(cur_sta, ite)) goto Return;
				old_sta = sta_nor;
				break;
			case sta_not:
				cur_sta = old_sta;
				if (!Not(cur_sta, ite)) goto Return;
				old_sta = sta_not;
				break;
			case sta_tag:
				cur_sta = old_sta;
				if (!Tag(cur_sta, ite)) goto Return;
				old_sta = sta_tag;
				break;
			case sta_tco:
				cur_sta = old_sta;
				if (!Tco(cur_sta, ite)) goto Return;
				old_sta = sta_tco;
				break;
			case sta_tce:
				cur_sta = old_sta;
				if (!Tce(cur_sta, ite)) goto Return;
				old_sta = sta_tce;
				break;
			case sta_wat:
				cur_sta = old_sta;
				if (!Wat(cur_sta, ite)) goto Return;
				old_sta = sta_wat;
				break;
			case sta_avl:
				cur_sta = old_sta;
				if (!Avl(cur_sta, ite)) goto Return;
				old_sta = sta_avl;
				break;
			case sta_aco:
				cur_sta = old_sta;
				if (!Aco(cur_sta, ite)) goto Return;
				old_sta = sta_aco;
				break;
			case sta_ace:
				cur_sta = old_sta;
				if (!Ace(cur_sta, ite)) goto Return;
				old_sta = sta_ace;
				break;
			case sta_end:
				ret = true;
			default:
				goto Return;
			}
		}
	Return:
		return ret;
	}

	iterator_t& GetRoot()
	{
		return m_xData.Head();
	}
	bool GetNode(_IN_ LPCTSTR sName, _IN_OT_ iterator_t& rIte)
	{
		if (!sName) return false;
		if (rIte == iterator_t())
			rIte = m_xData.Head();
		++rIte;
		for(; rIte != m_xData.Tail(); ++rIte)
		{
			if (!(*rIte)) continue;
			if ((*rIte)->nam == sName)
				return true;
		}
		return false;
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

	bool AddNode(const node_t& node, iterator_t& ite)
	{
		node_t* p = ExMem::Alloc<node_t>(&m_GC);
		(*p) = node;
		return m_xData.Add(p, ite);
	}
	bool DelNode(iterator_t& ite)
	{
		return m_xData.Del(ite);
	}

	bool AddAttr(LPCTSTR key, LPCTSTR val, iterator_t& ite)
	{
		if (!key) return false;
		if (ite == m_xData.Head() || 
			ite == m_xData.Tail()) return false;
		(*ite)->att[key] = val;
		return true;
	}
	bool DelAttr(LPCTSTR key, iterator_t& ite)
	{
		if (!key) return false;
		if (ite == m_xData.Head() || 
			ite == m_xData.Tail()) return false;
		return (*ite)->att.Del(key);
	}
};

//////////////////////////////////////////////////////////////////

EXP_END

#endif/*__GuiXML_hpp__*/