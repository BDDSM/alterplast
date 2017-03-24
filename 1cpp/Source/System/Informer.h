#pragma once

#include "../mycontextbase.h"

class CInformer : public CMyContextBase
{
	DECLARE_DYNCREATE(CInformer);

public:

	BOOL funcGetMethodsTable(CValue &RetVal, CValue **ppValue);
	BOOL funcGetPropsTable(CValue &RetVal, CValue **ppValue);

	BOOL funcHaveContextMethod(CValue &RetVal, CValue **ppValue);
	BOOL funcHaveContextProp(CValue &RetVal, CValue **ppValue);

	// ����������������(������������������������������������, ����������������������������������)
	BOOL funcTheObjectIs(CValue &RetVal, CValue **ppValue);

	// ����������������(�����������������, ����������������������������������)
	BOOL funcIsFunction(CValue &RetVal, CValue **ppValue);

	// �������� ��������� ������ 1� (����� �� FormEx)
	BOOL  funcCommandLine(CValue& RetVal, CValue **params);
	// �������� ���� � ���������� 1�
	BOOL  funcGetAppPath(CValue& RetVal, CValue **params);

	// �������� ���������� ��� �������� ��� ��������� (�� RuntimeClass)
	// ��������, ��� ������� "��������" ���������� �������������� ������ "�����������������"
	BOOL  funcGetInternalTypeName(CValue& RetVal, CValue **params);

	// �������� ����� ������� ����������
	BOOL funcGetIdleTime(CValue &RetVal, CValue **ppValue);

	BOOL funcGetAddInDllTable(CValue &RetVal, CValue **ppValue);

	static bool GetMethodsTable(const CValue& ContextValue, CValue& RetVal);
	static bool GetPropsTable(const CValue& ContextValue, CValue& RetVal);

	static void GetMethodOfContext(class CValue * * ppValue);

	BOOL funcGetCurrentModuleText(CValue &RetVal, CValue **ppValue);
	BOOL funcGetModuleText(CValue &RetVal, CValue **ppValue);
	
public:

	DECLARE_MY_CONTEXT();

private:
	static bool GetTableFromContext(bool bNeedMethods, const CValue& ContextValue, CValue& VTValue);
	static bool GetTableFromContext(bool bNeedMethods, CBLContext* pCont, CValueTable* pVT);

	BOOL HaveContextMethodOrProp(bool bNeedMethods, CValue &RetVal, CValue **ppValue);
};
