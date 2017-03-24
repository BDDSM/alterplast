//	Delegate.cpp: implementation of the CDelegate class.
//
//	Based on Deb initial release
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "Delegate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//_____________________________________________________________________________
//
BEGIN_BL_METH_MAP(CDelegate) 
	BL_METH_PROC_DEF_PARAM("DefineType",		"�������������",	3,		&procDefineType, &defsDefineType)
	BL_METH_FUNC_DEF_PARAM("AddMethod",		"��������",	3,	&funcAdd, &defsAdd)
    BL_METH("RemMethod",		"�������",			2,		NULL,			&funcRemove,			NULL)
    BL_METH("RemMethodByKey",	"��������������",	1,		NULL,			&funcRemoveByKey,	NULL)
    BL_METH("Invoke",			"�������",			0,		NULL,			&funcInvoke,			NULL)
    BL_METH("InvokeVL",			"���������",		1,		NULL,			&funcInvokeVL,		NULL)
    BL_METH("InvokeByKey",		"��������������",	1,		NULL,			&funcInvokeByKey,	NULL)
    BL_METH("InvokeVLByKey",	"����������������",	2,		NULL,			&funcInvokeVLByKey,	NULL)
    BL_METH("Erase",			"��������",			0,		&procErase,		NULL,				NULL)
    BL_METH("IsEmpty",			"������",			0,		NULL,			&funcIsEmpty,		NULL)

    BL_METH_FUNC("GetHandlersTable",	"���������������������������",	0,		&funcGetHandlersTable)
    BL_METH_FUNC("GetHandlersTableByKey",	"����������������������������������",	1,		&funcGetHandlersTableByKey)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CDelegate)
//    BL_PROP("propEng", "propRus", propGet, propSet)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CDelegate, "Delegate", "�������", 1, NULL, NULL, -1);
//_____________________________________________________________________________
//
LPCSTR CDelegate::m_voidKey = "";
LPCSTR CDelegate::m_defaultKey = "__default";
//_____________________________________________________________________________
//
CDelegate::CDelegate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ResetType();

	int * pn = aInvokeMethods;
	*pn++ = FindMethod("Invoke");
	*pn++ = FindMethod("InvokeByKey");
}
//_____________________________________________________________________________
//
CDelegate::~CDelegate()
{
	Erase(false);
}
//_____________________________________________________________________________
//
// checks
//_____________________________________________________________________________
//
void CDelegate::CheckType() const
{
	if (m_nParamCount == -1)
		RuntimeError("����� �������������� �������� ���������� ���������� ��� ��� ������� �������::�������������()");
}

void CDelegate::CheckMethName(CString& sMethName, LPCSTR sFuncName, LPCSTR sParamNum)
{
	if (sMethName.IsEmpty())      
		RuntimeError("%s �������� ������ �������::%s ���������� ���������� �������� ������!", sFuncName, sParamNum);
}

void CDelegate::CheckContext(CBLContext *pCont, LPCSTR sFuncName, LPCSTR sParamNum)
{
	if (!pCont)
		RuntimeError("%s �������� ������ �������::%s ���������� ���������� ���������� ������� 1� (��������������: CreateObject())!", sFuncName, sParamNum);
}

int CDelegate::CheckFindMethod(CBLContext *pCont, CString& sMethName)
{
	int nMeth = pCont->FindMethod(sMethName);
	if (nMeth == -1)
		RuntimeError("� ������ '%s' �� ������ ����� '%s'", pCont->GetTypeString(), sMethName.operator LPCTSTR());

	return nMeth;
}
//_____________________________________________________________________________
//
// service
//_____________________________________________________________________________
//
LPCSTR CDelegate::ResolveKey(LPCSTR key) const
{
	if (!key)
		key = m_voidKey;

	return key;
}

LPCSTR CDelegate::ResolveKey(CValue* pcvKey) const
{
	LPCSTR key = NULL;

	if ( !pcvKey->IsEmpty() )
		key = pcvKey->GetString().operator LPCTSTR();
	else
		key = ResolveKey(key);

	return key;
}

CBLContext* CDelegate::GetContext(const CValue *pvCont) const
{
	return pvCont->GetContext();
}

CValue** CDelegate::GetVLParams(CValue const& val) const
{
	CheckType();
	
	CValueItemList* pa = CValue2VL(val);
	if (!pa)
		RuntimeError("�������� �������� 2 (��������� ������ ��������)");
	
	if (pa->GetSize() < m_nParamCount)
		RuntimeError("������������� ����� ���������� �������� � ������ ��������");
	
	CValue** vl_params = reinterpret_cast<CValue**>(pa->GetData());
	return vl_params;
}
//_____________________________________________________________________________
//
// frontend ����������� ����
//_____________________________________________________________________________
//
BOOL CDelegate::procDefineType(CValue **params)
{
	Erase(false);
	DefineType(params[0]->GetNumeric(), params[1]->GetNumeric(), params[2]->GetNumeric());

	return TRUE;
}

int	 CDelegate::defsDefineType(int nParam, CValue* param)const
{
	if(nParam == 2){
		*param = long(0);
		return TRUE;
	}
	
	return FALSE;
}

//_____________________________________________________________________________
//
// backend ����������� ����
//_____________________________________________________________________________
//
void CDelegate::DefineType(long nParamCount, long nIsFunc, long nHonorBreakRequests)
{
	m_nParamCount	= nParamCount;
	m_nIsFunc		= nIsFunc ? 1 : 0;
	m_bHonorBreakRequests = nHonorBreakRequests ? true : false;
}
//_____________________________________________________________________________
//
void CDelegate::ResetType()
{
	DefineType(-1, 0, 0);
}
//_____________________________________________________________________________
//
// frontend ���������� ������ � ������
//_____________________________________________________________________________
//
BOOL CDelegate::funcAdd(CValue &RetVal, CValue **params)
{
	LPCSTR key = ResolveKey(params[2]);

	RetVal = Add(params, key);
	return TRUE;
}
//_____________________________________________________________________________
//
// backend ���������� ������ �� �����
// ���������: [0]: ������, [1]; ��� ������; key: ����
//_____________________________________________________________________________
//
int	CDelegate::defsAdd(int nParam, CValue* param)const
{
	if(nParam == 2){
		*param = "";
		return TRUE;
	}
	
	return FALSE;
}

long CDelegate::Add(CValue **params, LPCSTR key)
{
	CheckType();

	CBLContext *pCont = GetContext(params[0]);
	CheckContext(pCont, "��������()", "� ������");

	CString sMethName = params[1]->GetString();
	CheckMethName(sMethName, "��������()", "�� ������");

	int nMeth = CheckFindMethod(pCont, sMethName);

	if (pCont->HasRetVal(nMeth) != m_nIsFunc)
		RuntimeError("� ������ '%s' ����� '%s' �������� ��� %s",
		pCont->GetTypeString(), sMethName.operator LPCTSTR(), m_nIsFunc == 1 ? "���������, ���������� �������" : "�������, ���������� ���������");

	if (pCont->GetNParams(nMeth) != m_nParamCount)
	{
		LPSTR sParam;
		int nParamCount = m_nParamCount % 10;
		if (nParamCount == 0 || nParamCount > 4)
			sParam = "����������";
		else if (nParamCount == 1)
			sParam = "��������";   
		else
			sParam = "���������";   
		
		RuntimeError("� ������ '%s' ����� '%s' ������ ��������� %d %s!", pCont->GetTypeString(), sMethName.operator LPCTSTR(), m_nParamCount, sParam);
	}

	CValue *pvCont = new CValue(*params[0]);
	InfoEventHandlers info(pCont, nMeth, pvCont);
/*
	// ����������������� - ������ ������
	LPCSTR szTypeString = pCont->GetTypeString();
	if ( !m_map.Compare(szTypeString, "�����������������") || !m_map.Compare(szTypeString, "GroupContext"))
		m_pBLGroupContext = pCont;
*/
	key = ResolveKey(key);

	LIST *hl;
	if ( !m_map.Lookup(key, hl) || !hl)
	{
		hl = new LIST;
		m_map[key] = hl;
	}
	
	hl->insert(hl->end(), info);
	return 1;
}
//_____________________________________________________________________________
//
// frontend �������� ������ ������� �� ���� ������
// ���������: [0]: ������, [1]: ��� ������
//_____________________________________________________________________________
//
BOOL CDelegate::funcRemove(CValue &RetVal, CValue **params)
{
	CheckType();

	CBLContext *pCont = params[0]->GetContext();
	CheckContext(pCont, "�������()", "� ������");

	CString sMethName = params[1]->GetString();
	CheckMethName(sMethName, "�������()", "�� ������");

	int nMeth = CheckFindMethod(pCont, sMethName);

	LPCSTR key;
	LIST *hl;
	long bFound = 0;

	POSITION pos = m_map.GetStartPosition();
	while( pos )
	{
		m_map.GetNextAssoc(pos, key, hl);

		ITER iter;
		while ((iter = find(hl->begin(), hl->end(), InfoEventHandlers(pCont, nMeth, 0L))) != hl->end())
		{
			bFound = 1;

			hl->erase(iter);
		}
		if (hl->empty())
		{
			delete hl;
			m_map.RemoveKey(key);
		}
	}
	RetVal = bFound;
	return TRUE;
}
//_____________________________________________________________________________
//
// frontend �������� ���� ������� �� �����
// ���������: [0]: ����
//_____________________________________________________________________________
//

BOOL CDelegate::funcRemoveByKey(CValue &RetVal, CValue **params)
{
	CheckType();

	LPCSTR key = ResolveKey(params[0]);
	LIST *hl;
	long bFound = 0;

	if (m_map.Lookup(key, hl) && hl)
	{
		bFound = EmptyList(hl);

		m_map.RemoveKey(key);
	}
	RetVal = bFound;
	return TRUE;
}
//_____________________________________________________________________________
//
// frontend ����� ���� ������� ��� ����� �����
//_____________________________________________________________________________
//
BOOL CDelegate::funcInvoke(CValue &RetVal, CValue **params)
{
	Invoke(RetVal, params, m_bHonorBreakRequests);
	return TRUE;
}
//_____________________________________________________________________________
//
// frontend ����� ���� ������� ��� ����� �����, ��������� � ��
//_____________________________________________________________________________
//
BOOL CDelegate::funcInvokeVL(CValue &RetVal, CValue **params)
{
	CValue** vl_params = GetVLParams(*params[0]);
	Invoke(RetVal, vl_params, m_bHonorBreakRequests);
	return TRUE;
}
//_____________________________________________________________________________
//
// backend ����� ���� ������� ��� ����� �����
//_____________________________________________________________________________
//
void CDelegate::Invoke(CValue &RetVal, CValue **params, bool bHonorBreakRequests)
{
	CheckType();

	LPCSTR key = ResolveKey(static_cast<LPCSTR>(NULL));
 	LIST *hl;

	RetVal = 1L;

	POSITION pos = m_map.GetStartPosition();
	while( pos )
	{
		m_map.GetNextAssoc(pos, key, hl);

		if (!CallFromList(hl, RetVal, params, bHonorBreakRequests))
			return;
	}
}
//_____________________________________________________________________________
//
// frontend ����� ���� ������� �� �����
// ���������: [0]: ����, [...] ��������� ��������
//_____________________________________________________________________________
//
BOOL CDelegate::funcInvokeByKey(CValue &RetVal, CValue **params)
{
	CheckType();

	LPCSTR key = ResolveKey(params[0]);

	InvokeByKey(RetVal, params + 1, key, m_bHonorBreakRequests);
	return TRUE;
}
//_____________________________________________________________________________
//
// frontend ����� ���� ������� �� �����
// ���������: [0]: ����, [1] ��������� � ��
//_____________________________________________________________________________
//
BOOL CDelegate::funcInvokeVLByKey(CValue &RetVal, CValue **params)
{
	CheckType();
	
	LPCSTR key = ResolveKey(params[0]);

	CValue** vl_params = GetVLParams(*params[1]);
	InvokeByKey(RetVal, vl_params, key, m_bHonorBreakRequests);
	return TRUE;
}
//_____________________________________________________________________________
//
// backend ����� ���� ������� �� �����
// ���������: [...]: ��������� ��������
//_____________________________________________________________________________
//
void CDelegate::InvokeByKey(CValue &RetVal, CValue **params, LPCSTR key, bool bHonorBreakRequests)
{
 	LIST *hl;
	
	RetVal = 1L;
	
	if (m_map.Lookup(key, hl) && hl)
	{
		if (!CallFromList(hl, RetVal, params, bHonorBreakRequests))
			return;
	}
	else if (key != m_defaultKey)
	{
		InvokeByKey(RetVal, params, m_defaultKey, bHonorBreakRequests);
	}
}

bool CDelegate::CallFromList(LIST *hl, CValue &RetVal, CValue **params, bool bHonorBreakRequests)
{
	if (!hl)
		return false;

	CValue vZero = CValue(0L);

	CONST_ITER iter;
	for (iter = hl->begin(); iter != hl->end(); iter++ )
	{
		//CBLContext *pBLCont = iter->pBLCont;
		CBLContext *pBLCont = GetContext(iter->pvCont.get());
		if (pBLCont)
		{
			int nMeth = iter->nNumMethOfCall;
			
			if (m_nIsFunc)
			{
				pBLCont->CallAsFunc(nMeth, RetVal, params);
				if (bHonorBreakRequests)
					if (RetVal == vZero)                  
						return false;
			}
			else
				pBLCont->CallAsProc(nMeth, params);
		}
	}
	return true;
}
//_____________________________________________________________________________
//
// frontend �������
//_____________________________________________________________________________
//
BOOL CDelegate::procErase(CValue **params)
{
	Erase(true);
	return TRUE;
}
//_____________________________________________________________________________
//
// backend �������
//_____________________________________________________________________________
//
void CDelegate::Erase(bool bResetType)
{
	LPCSTR key;
	LIST *hl;

	POSITION pos = m_map.GetStartPosition();
	while( pos )
	{
		m_map.GetNextAssoc(pos, key, hl);

		EmptyList(hl);
	}
	m_map.RemoveAll();

	if (bResetType)
		ResetType();
}

int CDelegate::EmptyList(LIST *hl)
{
	int bFound = 0;
 	REVERSE_ITER iter(hl->rbegin());  // artbear - �� �����
 	for (; iter != hl->rend(); iter++ )
	{
		bFound = 1;
		iter->pvCont = sh_ptr<CValue>(NULL); // ����� �������� ������� ��������-��������
	}
	delete hl;

	return bFound;
}
//_____________________________________________________________________________
//
// frontend ��������
//_____________________________________________________________________________
//
BOOL CDelegate::funcIsEmpty(CValue &RetVal, CValue **params)
{
	LPCSTR key;
	LIST *hl;

	POSITION pos = m_map.GetStartPosition();
	long bEmpty = 1;

	while( pos )
	{
		m_map.GetNextAssoc(pos, key, hl);
		if ( hl && !hl->empty() )
		{
			bEmpty = 0;
			break;
		}
	}

	RetVal = bEmpty;
	return TRUE;
}
//_____________________________________________________________________________
//
// �������� ����� ���������� ��������
//_____________________________________________________________________________
//
int CDelegate::GetDefinedParamCount() const
{
	return m_nParamCount;
}
//_____________________________________________________________________________
//
// context overload
//_____________________________________________________________________________
//
int CDelegate::GetNParams(int iMethodNum) const
{
	_S_MyContextBaseInfo* pInfo=GetBaseInfo();
	if(((DWORD)iMethodNum)>=pInfo->methCount)
		return 0;

	// ���� ������� ���������� �����
	int nParamCount = 0;
	for (int i = 0; i < nInvokeMethods; ++i)
	{
		if (iMethodNum == aInvokeMethods[i])
		{
			nParamCount = m_nParamCount;
			break;
		}
	}

	return pInfo->pMethodDef[iMethodNum].NumberOfParam + nParamCount;
}
//_____________________________________________________________________________
//

BOOL CDelegate::funcGetHandlersTable(CValue& RetVal, CValue **params)
{
	GetHandlersTable(RetVal);
	return TRUE;
}

void CDelegate::GetHandlersTable(CValue& RetVal) const
{
	sh_array<CValue> pv;
	sh_array<CValue*> ppv;
	CValueTable* pVT = PrepareHandlersTable(RetVal, pv, ppv);

	LPCSTR key = ResolveKey(static_cast<LPCSTR>(NULL));
 	LIST *hl;

	POSITION pos = m_map.GetStartPosition();
	while( pos )
	{
		m_map.GetNextAssoc(pos, key, hl);

		SaveToHandlersTable(hl, pVT, pv, ppv);
	}
}

// ���������: [0]: ����
BOOL CDelegate::funcGetHandlersTableByKey(CValue& RetVal, CValue **params)
{
	LPCSTR key = ResolveKey(params[0]);

	GetHandlersTableByKey(RetVal, key);

	return TRUE;
}

void CDelegate::GetHandlersTableByKey(CValue &RetVal, LPCSTR key) const
{
	sh_array<CValue> pv;
	sh_array<CValue*> ppv;
	CValueTable* pVT = PrepareHandlersTable(RetVal, pv, ppv);

	LIST *hl;
	
	if (m_map.Lookup(key, hl) && hl)
	{
		SaveToHandlersTable(hl, pVT, pv, ppv);
	}
	else if (key != m_defaultKey)
	{
		RetVal.Reset();
		GetHandlersTableByKey(RetVal, m_defaultKey);
	}
}

CValueTable* CDelegate::PrepareHandlersTable(CValue& RetVal, sh_array<CValue>& pv, sh_array<CValue*>& ppv) const
{
	RetVal.CreateObject("���������������");
	CValueTable* pVT = static_cast<CValueTableContextData*>(RetVal.GetContext()->GetInternalData())->GetValueTable();
	if(!pVT)
		RuntimeError("�� ������� �������� ������� ��������");

	CString str;
	CType ctStr(STRING_TYPE_1C);
	CType ctAGREGATE(AGREGATE_TYPE_1C);

	pVT->Clear(TRUE);
	pVT->AddColumn(str = "������",				ctAGREGATE, str, 0, "", 0);
	pVT->AddColumn(str = "������������������",	ctStr,		str, 0, "", 0);
	pVT->AddColumn(str = "������������������",	ctStr,		str, 0, "", 0);

  	pv = sh_array<CValue>(new CValue [3]);
  	ppv = sh_array<CValue*>(new CValue* [3]);

	for (int i = 0; i < 3; ++i)
		ppv[i] = &pv[i];

	return pVT;
}

void CDelegate::SaveToHandlersTable(LIST *hl, CValueTable* pVT, sh_array<CValue>& pv, sh_array<CValue*>& ppv) const
{
	if (!hl)
		return;

	CONST_ITER iter;
	for (iter = hl->begin(); iter != hl->end(); iter++ )
	{
		CBLContext *pCont = GetContext(iter->pvCont.get());
		if (pCont)
		{
			pv[0] = *iter->pvCont;
			pv[1] = pCont->GetTypeString();
			pv[2] = pCont->GetMethodName(iter->nNumMethOfCall, 1);

			pVT->AddRow(ppv.get(), -1);
		}
	}
}
