// doctables.cpp
#include "StdAfx.h"
#include "doctables.h"

CDTTabInfo::CDTTabInfo(CStringArray& arrOfNames)
{
	CDocDef* pDocDef = pMetaDataCont->GetDocDef(arrOfNames[1]);
	if(!pDocDef)
	{
		setError("�������� %s �� ������.", (LPCSTR)arrOfNames[1]);
		return;
	}
	m_strTableName = pDocDef->GetTblTableName();
	m_pTable = static_cast<CTableEx*>(pDataDict->GetTable(m_strTableName));
	if(!m_pTable)
	{
		setError("�������� %s �� ����� ��������� �����.", (LPCSTR)arrOfNames[1]);
		return;
	}
	
	CNoCaseMap<CString> aliaces;
	fillNamesFromObjs(pDocDef->GetTables(), aliaces);
	fillTabInfo(aliaces);
}