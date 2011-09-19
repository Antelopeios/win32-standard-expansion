#include "stdafx.h"
#include "Main/Tagger.h"

//////////////////////////////////////////////////////////////////

CData* CData::Instance()
{
	static CData data;
	return &data;
}

void CData::Init()
{
	m_File.Open(GET_GBL()->GetAppPath() + _T("Data.dat"), CIOFile::modeReadWrite | CIOFile::modeCreate);
	m_Data.SetFile(&m_File);
	m_Data.Decode();
}