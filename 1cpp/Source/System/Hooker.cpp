// Hooker.cpp: implementation of the CHooker class.
// develop by artbear
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Hooker.h"

#include "GroupContextWrap.h"
#include "DllHooker.h"

#include "WrapAboutDlg.h"

//_____________________________________________________________________________
//
// CHooker Maps
//_____________________________________________________________________________
//
BEGIN_BL_METH_MAP(CHooker) 
    BL_METH_FUNC("HookEventsOfGroupContext", "��������������������", 2, procHookEventsOfGroupContext)
	//BL_METH_FUNC("CallOriginalEventOfGroupContext", "����������������������������", 3, funcCallOriginalEventOfGroupContext)

    BL_METH_FUNC("GetHookObjectForEventsOfGroupContext", "����������������������������", 1, funcGetHookObjectForEventsOfGroupContext)
	BL_METH_PROC("UnHookEventsOfGroupContext", "���������������������K", 1, procUnHookEventsOfGroupContext)

    BL_METH_FUNC("HookEventsOfGlobalModule", "�����������������������������������", 1, procHookEventsOfGlobalModule)
	//BL_METH_FUNC("CallOriginalEventOfGlobalModule", "�������������������������������������������", 2, funcCallOriginalEventOfGlobalModule)

    BL_METH_FUNC("GetHookObjectForEventsOfGlobalModule", "�������������������������������������������", 0, funcGetHookObjectForEventsOfGlobalModule)
	BL_METH_PROC("UnHookEventsOfGlobalModule", "�������������������������������������", 0, procUnHookOriginalEventOfGlobalModule)

	BL_METH_FUNC_WITH_UN_LIM_PARAMS("RunOriginalEventOfGroupContext", "������������������������������", 2, funcRunOriginalEventOfGroupContext)
	BL_METH_FUNC_WITH_UN_LIM_PARAMS("RunOriginalEventOfGlobalModule", "���������������������������������������������", 1, funcRunOriginalEventOfGlobalModule)

	BL_METH_FUNC_WITH_UN_LIM_PARAMS("RunEventOfGroupContext", "������������������", 3, funcRunEventOfGroupContext)
	BL_METH_FUNC_WITH_UN_LIM_PARAMS("RunEventOfGlobalModule", "���������������������������������", 2, funcRunEventOfGlobalModule)

	BL_METH_PROC("AddEvent_UnHandledError", "�����������������_��������������������", 0, procAddEvent_UnHandledError)
	BL_METH_PROC("AddEvent_ReportEventA", "�����������������_�������������������������������", 0, procAddEvent_ReportEventA)
	BL_METH_PROC("AddEvent_ReportUserEvent", "�����������������_������������������������������������������������", 0, procAddEvent_ReportUserEvent)
	
	BL_METH_PROC("AddVeryLongStringRestructure", "����������������������������������������������", 0, procAddEvent_WriteMessageEvent_DoRestructureVeryLongString)

END_BL_METH_MAP() 

BEGIN_BL_PROP_MAP(CHooker)
END_BL_PROP_MAP()

IMPLEMENT_MY_CREATE_CONTEXT(CHooker, "Hooker", "�����������");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CHooker::procHookEventsOfGroupContext(CValue &RetVal, CValue **ppValue)
{
	funcGetHookObjectForEventsOfGroupContext(RetVal, ppValue);

	CEventsWrapperManager::GetWrapper()->SetWrapper(*ppValue[0], *ppValue[1]);

	return TRUE;
}

BOOL CHooker::procHookEventsOfGlobalModule(CValue &RetVal, CValue **ppValue)
{
	funcGetHookObjectForEventsOfGlobalModule(RetVal, ppValue);

	CEventsWrapperManager::GetWrapper()->SetWrapperForGlobalModule(*ppValue[0]);

	return TRUE;
}

BOOL CHooker::funcGetHookObjectForEventsOfGroupContext(CValue &RetVal, CValue **ppValue)
{
	const CValue& GroupContValue = *ppValue[0];
	ValidateParamGroupContext(GroupContValue, "��������������������: ������ ���� ������� ��������� ��������");
	CGroupContext* pGroupContext = static_cast<CGroupContext*>(GroupContValue.GetContext());
	
	CComponentClass* pCont = const_cast<CComponentClass*>(CEventsWrapperManager::GetWrapper()->GetWrapper(pGroupContext));
	RetVal.AssignContext(pCont);
	
	return TRUE;
}

BOOL CHooker::funcGetHookObjectForEventsOfGlobalModule(CValue &RetVal, CValue **ppValue)
{
	CComponentClass* pCont = const_cast<CComponentClass*>(CEventsWrapperManager::GetWrapper()->GetWrapperForGlobalModule());
	RetVal.AssignContext(pCont);
	
	return TRUE;
}

// BOOL CHooker::funcCallOriginalEventOfGlobalModule(CValue &RetVal, CValue **ppValue)
// {
// 	int iRes = CEventsWrapperManager::GetWrapper()->CallOriginalEventOfGlobalModule(*ppValue[0], *ppValue[1]);
// 	RetVal = CNumeric(iRes);
// 
// 	return TRUE;
// }
// 
// BOOL CHooker::funcCallOriginalEventOfGroupContext(CValue &RetVal, CValue **ppValue)
// {
// 	int iRes = CEventsWrapperManager::GetWrapper()->CallOriginalEventOfGroupContext(*ppValue[0], *ppValue[1], *ppValue[2]);
// 	RetVal = CNumeric(iRes);
// 
// 	return TRUE;
// }

BOOL CHooker::procUnHookEventsOfGroupContext(CValue **ppValue)
{
	CValue& GroupContValue = *ppValue[0];
	ValidateParamGroupContext(GroupContValue, "���������������������K: ���������� �������� ��������� ��������");
	CGroupContext* pGroupCont = static_cast<CGroupContext*>(GroupContValue.GetContext());

	CEventsWrapperManager::GetWrapper()->DestroyWrapper(pGroupCont);

	return TRUE;
}

BOOL CHooker::procUnHookOriginalEventOfGlobalModule(CValue **ppValue)
{
	CEventsWrapperManager::GetWrapper()->DestroyWrapper(::GetGlobalModule());

	return TRUE;
}

BOOL CHooker::funcRunOriginalEventOfGroupContext(int nParamsCount, CValue &RetVal, CValue **ppValue)
{
	CValue& ParamGroupContext = *ppValue[0];
	CValue& ParamEventName = *ppValue[1];

	ValidateParamGroupContext(ParamGroupContext, "���������� �������� ��������� ��������");
 	ValidateParamNotEmptyString(ParamEventName, "���������� �������� ��� ������� ���������� ���������");

	CGroupContext* pGroupContext = static_cast<CGroupContext*>(ParamGroupContext.GetContext());
	CString strEventName = ParamEventName.GetString();

	int iRes = CEventsWrapperManager::GetWrapper()->RunOriginalEventOfGroupContext(pGroupContext, strEventName, nParamsCount, RetVal, ppValue);
	RetVal = CNumeric(iRes);

	return TRUE;
}

BOOL CHooker::funcRunOriginalEventOfGlobalModule(int nParamsCount, CValue &RetVal, CValue **ppValue)
{
	CValue& ParamEventName = *ppValue[0];
	ValidateParamNotEmptyString(ParamEventName, "���������� �������� ��� ������� ���������� ���������");
	CString strEventName = ParamEventName.GetString();

	int iRes = CEventsWrapperManager::GetWrapper()->RunOriginalEventOfGlobalModule(strEventName, nParamsCount, RetVal, ppValue);
	RetVal = CNumeric(iRes);

	return TRUE;
}

BOOL CHooker::funcRunEventOfGroupContext(int nParamsCount, CValue &RetVal, CValue **ppValue)
{
	CValue& ParamEventHandler = *ppValue[0];
	CValue& ParamGroupContext = *ppValue[1];
	CValue& ParamEventName = *ppValue[2];

	ValidateParamGroupContext(ParamGroupContext, "���������� �������� ��������� ��������");
	ValidateParamNotEmptyString(ParamEventName, "���������� �������� ��� ������� ���������� ���������");

 	if (!ParamEventHandler.IsEmpty()) // �������� ���������� �� �����
		ValidateParamUserClass(ParamEventHandler, "���������� �������� ������ ������-�����������");

	CComponentClass* pEventHandlerCont = static_cast<CComponentClass*>(ParamEventHandler.GetContext());
	CGroupContext* pGroupContext = static_cast<CGroupContext*>(ParamGroupContext.GetContext());
	CString strEventName = ParamEventName.GetString();

	int iRes = CEventsWrapperManager::GetWrapper()->RunEventOfGroupContext(pEventHandlerCont, pGroupContext, strEventName, nParamsCount, RetVal, ppValue);
	RetVal = CNumeric(iRes);

	return TRUE;
}

BOOL CHooker::funcRunEventOfGlobalModule(int nParamsCount, CValue &RetVal, CValue **ppValue)
{
	CValue& ParamEventHandler = *ppValue[0];
	CValue& ParamEventName = *ppValue[1];

	ValidateParamNotEmptyString(ParamEventName, "���������� �������� ��� ������� ���������� ���������");

 	if (!ParamEventHandler.IsEmpty()) // �������� ���������� �� �����
		ValidateParamUserClass(ParamEventHandler, "���������� �������� ������ ������-�����������");

	CComponentClass* pEventHandlerCont = static_cast<CComponentClass*>(ParamEventHandler.GetContext());
	CString strEventName = ParamEventName.GetString();

	int iRes = CEventsWrapperManager::GetWrapper()->RunEventOfGlobalModule(pEventHandlerCont, strEventName, nParamsCount, RetVal, ppValue);
	RetVal = CNumeric(iRes);

	return TRUE;
}

BOOL CHooker::procAddEvent_UnHandledError(CValue **ppValue)
{
	CHookGroupContext::GetHooker()->AddEvent_UnHandledError();

	return TRUE;
}

BOOL CHooker::procAddEvent_ReportEventA(CValue **ppValue)
{
	CHookGroupContext::GetHooker()->AddEvent_ReportEventA();

	return TRUE;
}

BOOL CHooker::procAddEvent_ReportUserEvent(CValue **ppValue)
{
	CHookGroupContext::GetHooker()->AddEvent_ReportUserEvent();

	return TRUE;
}

BOOL CHooker::procAddEvent_WriteMessageEvent_DoRestructureVeryLongString(CValue **ppValue)
{
	CHookGroupContext::GetHooker()->AddEvent_WriteMessageEvent();
	
	return TRUE;
}

// ��������� IAddOwnAboutWindowManager
void CHooker::Add(IOwnAboutWindowManager* pOwnAboutWindowManager)
{
	CWrapAboutDlg::AddMyAboutWindowManager(pOwnAboutWindowManager);
}
