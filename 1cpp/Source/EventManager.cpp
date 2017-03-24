// EventManager.cpp: implementation of the CEventManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "EventManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const iNumberOfSendEvent = 3;

BEGIN_BL_METH_MAP(CEventManager) 
     BL_METH_FUNC("IsEmpty",	"������",	0,		funcIsEmpty)
     BL_METH_FUNC("AddEventHandle",	"�������������������������",	2,		funcAddEventHandle)
     BL_METH_FUNC("RemEventHandle",	"������������������������",	2,		funcRemEventHandle)
     BL_METH_FUNC("SendEvent",	"��������������",	2,		funcSendEvents)
	 //BL_METH_FUNC_WITH_UN_LIM_PARAMS("SendEvent",	"��������������",	2,		funcSendEvents)
	 
     BL_METH_FUNC("SetDelegate",	"������������������",	1,		funcSetDelegate)
     
	 BL_METH_PROC("Clear",	"��������",	0,		procClear)
	 BL_METH_PROC("SetCountOfParams",	"������������������������������",	1,		procSetCountOfParams)

    BL_METH_FUNC("GetHandlersTable",	"���������������������������",	0,		funcGetHandlersTable)
END_BL_METH_MAP()

BEGIN_EMPTY_BL_PROP_MAP(CEventManager)

IMPLEMENT_MY_CREATE_CONTEXT(CEventManager, "EventManager", "���������������");

CEventManager::CEventManager() : m_pDelegate("")
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	

	m_pDelegate->DefineType(2, 1, 1);
}

BOOL CEventManager::funcIsEmpty(CValue& RetVal, CValue **params)
{
	return m_pDelegate->funcIsEmpty(RetVal, params);
}

BOOL CEventManager::funcAddEventHandle(CValue& RetVal, CValue **params)
{
	ValidateParamNotEmptyString(*params[1], "�� ������ �������� AddEventHandle ���������� ���������� �������� ������!");
	
	ValidateParamContext(*params[0], 
		"� ������ �������� AddEventHandle ���������� ���������� ��������� ������ 1C++ (��������������: CreateObject())!");

	RetVal = m_pDelegate->Add(params, NULL);
	return TRUE;
}

BOOL CEventManager::funcRemEventHandle(CValue& RetVal, CValue **params)
{
	ValidateParamNotEmptyString(*params[1], "�� ������ �������� RemEventHandle ���������� ���������� �������� ������!");
	
	ValidateParamContext(*params[0], 
		"� ������ �������� RemEventHandle ���������� ���������� ��������� ������ 1C++ (��������������: CreateObject())!");
	
	return (m_pDelegate->funcRemove(RetVal, params));
}

BOOL CEventManager::funcSendEvents(CValue& RetVal, CValue **params)
{
	m_pDelegate->Invoke(RetVal, params, true);
	return TRUE;
}

BOOL CEventManager::funcSetDelegate(CValue& RetVal, CValue **params)
{
	ValidateParamContext(*params[0], "CDelegate", 
		"� SetDelegate(InstanceOfDelegate) � �������� ��������� ���������� ��������� ��������� ������ �������!");
	
	RetVal.Reset();
	RetVal.AssignContext(m_pDelegate);

 	m_pDelegate = static_cast<CDelegate *>(params[0]->GetContext());

	return TRUE;
}

BOOL CEventManager::procClear(CValue **params)
{
	m_pDelegate->Erase(false);
	return TRUE;
}

BOOL CEventManager::procSetCountOfParams(CValue **params)
{
	m_pDelegate->DefineType(params[0]->GetNumeric(), 1, 1);
	return TRUE;
}

int CEventManager::GetNParams(int iMethodNum)const
{	
	if (iMethodNum == iNumberOfSendEvent)
		return m_pDelegate->GetDefinedParamCount();
	else
		return CMyContextBase::GetNParams(iMethodNum);
}

char const *  CEventManager::GetTypeString(void)const
{
	//return "EventManager";
	return GetBaseInfo()->EngTypeString;
}

BOOL CEventManager::funcGetHandlersTable(CValue& RetVal, CValue **params)
{
	return m_pDelegate->funcGetHandlersTable(RetVal, params);
}