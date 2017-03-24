// calcjournal.cpp
#include "StdAfx.h"
#include "calcjournal.h"

CCJTabInfo::CCJTabInfo(CStringArray& arrOfNames)
{
	CCJDef* pCJ = pMetaDataCont->GetCJDef(arrOfNames[1]);
	if(!pCJ)
	{
		setError("������ �������� %s �� ������.", (LPCSTR)arrOfNames[1]);
		return;
	}
	m_strTableName = pCJ->GetTableName();
	m_pTable = static_cast<CTableEx*>(pDataDict->GetTable(m_strTableName));
	if(!m_pTable)
	{
		setError("������� ������� ������� %s �� �������.", (LPCSTR)arrOfNames[1]);
		return;
	}
	CNoCaseMap<CString> aliaces;
	fillNamesFromObjs(pCJ->GetCJParamDefs(), aliaces);
	//aliaces["ORDER"] = "_ORDER";
	fillTabInfo(aliaces);
}