// BLModuleWrapper.cpp: implementation of the CBLModuleWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "addin.h"
#include "BLModuleWrapper.h"
#include "ExecuteModule.h"
#include "ModuleString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBLModuleWrapper::CBLModuleWrapper(const CString& strNameOfComponent, class CBLContext* pContext, CBLModuleWrapper* pMod)
: CBLModule7(pContext, NULL), m_strNameOfComponent(strNameOfComponent), m_bEnableThrow(false)
{
	ASSERT(pMod != NULL); // ����� (todo: refactoring ����������� ������)

	if (pMod)
	{
		m_csFileName = pMod->m_csFileName;

		m_nID = pMod->m_nID;
		SetModulePathForRightUseIntoDebugger(pMod->m_strModulePath);

		m_bModuleContValid = pMod->m_bModuleContValid;

		m_nCounter = 0;
		m_bIsProcessBroken = FALSE;

		AssignCompiled(*pMod);
	}
	else
	{
		m_csFileName = "";

		m_nID = -1;
		m_strModulePath = "";
		m_bModuleContValid = FALSE;

		m_nCounter = 0;
		m_bIsProcessBroken = FALSE;
	}

	// �������� ������� �� custom tracer.dll
	if(pMainAddIn->m_EnableDebugging)
		m_bModuleContValid = TRUE;
}

CBLModuleWrapper::CBLModuleWrapper(const CString& strNameOfComponent, CBLContext* pContext, CModuleString const& ModuleString, bool bEnableThrow)
: CBLModule7(pContext, (LPCSTR)ModuleString), m_strNameOfComponent(strNameOfComponent), m_bEnableThrow(bEnableThrow)
{
	m_csFileName = ModuleString.GetFileName();

	m_nID = ModuleString.GetModuleID();
	SetModulePathForRightUseIntoDebugger(ModuleString.GetModulePath());

	m_bModuleContValid = ModuleString.GetModuleContValid();

	m_nCounter = 0;
	m_bIsProcessBroken = FALSE;

	// �������� ������� �� custom tracer.dll
	if(pMainAddIn->m_EnableDebugging)
		m_bModuleContValid = TRUE;
}

CBLModuleWrapper::~CBLModuleWrapper()
{
  //OnStopExecution();
}

CString CBLModuleWrapper::GetModuleDescription()
{
	CString csDescr = "'";
	csDescr += m_csFileName.IsEmpty() ? "unknown" : m_csFileName;
	csDescr += "'";

	if (m_nID != -1)
	{
		csDescr += " (";
		csDescr += GetFullName();
		csDescr += ")";
	}

	CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();
	CComponentClass* pComp = CComponentClass::GetComponentClassCurExModule(this);
	if (pComp)
	{
		csDescr += " (����� ";
		csDescr += pComp->GetTypeString();
		csDescr += ")";
	}

	return csDescr;
}

int CBLModuleWrapper::OnSyntaxError(void)
{
	if (!CBLModule7::m_bDbgExprEvaluating)
		LogErr("=== Syntax error � ������ %s %i", (LPCSTR)GetModuleDescription());

  //int ret = CBLModule::OnSyntaxError();
  int ret = CBLModule7::OnSyntaxError(); // artbear TODO kms �����-�� �������� ����� �������

  /* ����� �� ����������� ���� � ����� � ���� ���������:
  // ����� ��� ������������� EvalDebugExpr �������� �������-������
  //(��������, � ������� - ��� ��������� �������� ����������, ����� ����� ���� �� ������������ � �� ������������. � ������� ���� ��� ��� ����������.)
  if (1 != CBLModule7::m_bDbgExprEvaluating) // artbear �� ������������ ����-�
	if (!m_strFileName.IsEmpty())
		pBkEndUI->DoMessageLine(m_strFileName, mmRedErr);
*/

  return ret;
}

//	artbear - ��� �������� ������� ���� ����� ������ ������ !
// // artbear �� ������������ ����-� - ������� � ���������� � OnSyntaxError
// // ��������� �������� � ������� ����� CGCBoost::WrapCompile ���������� ����� BOOL bNormalModule = 7 == pMod->GetKind();
// // �� ����� �� � ��� �������� ������������� :)
// //
// int CBLModuleWrapper::GetKind(void)const
// {
//  	// artbear - ������������ ��� ����������� �������� �� ��������� �� �������
//  	//	�� ����������� � ������������ � ����� ����� � ����� �������
//  	if(!pMainAddIn->m_EnableDebugging)
//  		return 8;
//
// 	// ����� ��� ������� ��������� ������
// 	return CBLModule7::GetKind();
// }

void CBLModuleWrapper::OnErrorMessage(char const *pStr)
{
  CString strSyntaxErrDescr("",1000);
  CBLModule::GetSyntaxErrDescr(GetSyntaxErrCode(),strSyntaxErrDescr);

  char buffer[20];
  _itoa(GetSyntaxErrLineNum(), buffer, 10 );

  CString strSyntaxErrMarkedLine = "(";
  strSyntaxErrMarkedLine += buffer;
  strSyntaxErrMarkedLine += ")";
  strSyntaxErrMarkedLine += GetSyntaxErrMarkedLine();
  strSyntaxErrMarkedLine += "\t";
  strSyntaxErrMarkedLine += strSyntaxErrDescr;

  // ��������, ������ �� ����� ������ ������ � ������ �������
	if (m_bEnableThrow)
	{
		throw(strSyntaxErrMarkedLine); // artbear
	}

	CString strError;
	{
	  CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();

	if (!m_strNameOfComponent.IsEmpty())
	  {

		CString strNameOfComp = m_strNameOfComponent;
		CString strOldGetRuntimeErrDescr = strSyntaxErrDescr;

		strError += "�����: ";
		strError += strNameOfComp;
		strError += " - ������ ����������";

		strError += "\r\n";
		strError += "������ �";
		strError += pStr;

		strError += "\r\n{";
		CString strNameOfFile = pHierarchy->GetNameOfFile(strNameOfComp);
		CSetOfHierarchy::MakeFullFileName(&strNameOfFile);
		strError += strNameOfFile;

		char buffer[20];
		_itoa( GetSyntaxErrLineNum(), buffer, 10 );
		strError += "(";
		strError += buffer;
		strError += ")";
		strError += " }";
	  }
	}
	// ��� ������������� ���������� ���� ����� ����� ��������� �� ������ � �������������,
	// � ������� �������� ������ � 1�
	// ����� ��������� �������� �������� � ������ OnRuntimeError

    /*CString strModulePath_temp = m_strModulePath;
    strModulePath_temp.MakeLower();

	if (strModulePath_temp.Find(".ert") != -1) // ��� ����� ������� ������� ��������� ���� �� ������ ������������ ���������� � �������������
		CBLModule7::OnErrorMessageEx(pStr, m_nID, m_strModulePath,GetSyntaxErrLineNum());
	else
		CBLModule7::OnErrorMessage(pStr);
	*/

	if (strError.IsEmpty())
		CBLModule7::OnErrorMessageEx(pStr, m_nID, m_strModulePath,GetSyntaxErrLineNum());
	else
		CBLModule7::OnErrorMessageEx(strError, m_nID, m_strModulePath,GetSyntaxErrLineNum());

}

// todo refactoring ����������� ������ (����. ��������� ����������)
int CBLModuleWrapper::OnRuntimeError(void)
{
// kms ������-�� ����������� ����� ���
// 	if (!*m_pDbgExprEvaluating)
// 	{
// 		LogErr("=== Runtime error � ������ %s", (LPCSTR)GetModuleDescription());
//
// 		//	CString strRuntimeErrSourceLine;
// 		//	GetRuntimeErrSourceLine(strRuntimeErrSourceLine);
//
// 		GetRuntimeErrDescr(GetRuntimeErrCode(), CComponentClass::strTextException);
// 		CComponentClass::strTextException.TrimLeft();
// 		CComponentClass::m_bExceptionStatus = TRUE;
//
// 	}
// 	return CBLModule7::OnRuntimeError();

  CRuntimeExceptionClass::Empty();
#ifdef _DEBUG
  CString strRuntimeErrSourceLine("", 1000);
  CString strGetRuntimeErrDescr("", 500);
#else
  CString strRuntimeErrSourceLine;
  CString strGetRuntimeErrDescr;
#endif

  GetRuntimeErrSourceLine(strRuntimeErrSourceLine);
  strRuntimeErrSourceLine.TrimLeft();

  GetRuntimeErrDescr(GetRuntimeErrCode(),strGetRuntimeErrDescr);
// artbear
	if ((16 == GetRuntimeErrCode()) && m_bEnableThrow)
	{
		// TODO ��������� !
		CRuntimeExceptionClass::ThrowException("������ ����������: "+strGetRuntimeErrDescr +"\r\n"+
			"<"+strRuntimeErrSourceLine+">");
	}
// ����������

  CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();

//  CComponentClass* pContInter = pHierarchy->GetComponentClassCurExModule(this);
  CComponentClass* pContInter = CComponentClass::GetComponentClassCurExModule(this);

  if (pContInter)
  {
    CString strError;

    CString strNameOfComp = pContInter->GetTypeString();
	CString strOldGetRuntimeErrDescr = strGetRuntimeErrDescr;

    strError += strNameOfComp;
    strError += "::";
    strError += CExecuteModule::GetExecuteMethInfo(this);
	strError += " : "+strOldGetRuntimeErrDescr;

    strError += "\r\n";
    strError += strRuntimeErrSourceLine;

    strError += "\r\n{";
    CString strNameOfFile = pHierarchy->GetNameOfFile(strNameOfComp);
    CSetOfHierarchy::MakeFullFileName(&strNameOfFile);

    strError += strNameOfFile;
    char buffer[20];
    _itoa( GetRuntimeErrLineNum(), buffer, 10 );
    strError += "(";
    strError += buffer;
    strError += ")";
    strError += " }";

    strGetRuntimeErrDescr = strError;
    strGetRuntimeErrDescr += "\r\n";
  }

	//CRuntimeExceptionClass::SetException(strGetRuntimeErrDescr);
  //CBLModule7::OnRuntimeError();
  {

    /*CString strModulePath_temp = m_strModulePath;
    strModulePath_temp.MakeLower();

	if (strModulePath_temp.Find(".ert") != -1) // ��� ����� ������� ������� ��������� ���� �� ������ ������������ ���������� � �������������
		CBLModule7::OnErrorMessageEx(strGetRuntimeErrDescr, m_nID, m_strModulePath, GetRuntimeErrLineNum());
	else
		//CBLModule7::OnErrorMessage(pStr);
		return GetRuntimeErrLineNum();
	*/

    // artbear ��������� � ����-��
	// ��� ������������� ���������� ���� ����� ����� ��������� �� ������ � �������������,
	// � ������� �������� ������ � 1�
	  //
	// ��� ��������� ������ � ���� ������ � ������������� ����� ���������� ������ "����� �������� ������ ��� ������� � "+����������
	// ���� ��������� ����� ��� �����������
	// 1. ������� ���� ��� ���������� ������ - �������� ������������ ������ �������� �  �������
	/* ���������� ��� (artbear)
		Dim bFlagInsideOnMsgBox
		bFlagInsideOnMsgBox = false
		Sub Configurator_OnMsgBox(text, style, def, Answer)
			if InStr(text, "����� �������� ������ ��� ������� �") > 0 then
				' �������� ����� 1�++
				if 	bFlagInsideOnMsgBox = false then
					strFileName = Replace(text, "����� �������� ������ ��� ������� � ", "")
					Set FSO = CreateObject("Scripting.FileSystemObject")
					if FSO.FileExists(strFileName) then
						bFlagInsideOnMsgBox = true
						Documents.Open ( strFileName )
						bFlagInsideOnMsgBox = false
						Answer = mbaOK
		    			Exit Sub
					end if
				end if
			end if
			bFlagInsideOnMsgBox = false
		End Sub

	2. ��������� �������� �������� ������ � ��������� ������ - ����� � ������ ������� (artbear)
	3. ������� ������ ������� ������������
	4. ����������� �����������, ����� ������� ����������� �������� ������ � �������������
		��� ������� ������ � ���� ��������� 1�:����������� - ���� ������/��� �������� ��� :(

	// ��� ������� ������ �������� �������� �������� ����� �������� ��������� ��������� �� ������
	// �.�. ���� ������� ����� �����1, � ��� ������ ����� ���1, � ��� ���2, ����� �������� 3 ���������
	// ������� ��� ���2, ����� ���1, ����� �����1 - � ������� ������ �� ������ �� ��������� ��� �������� ������������� ������� ������� �� ��������������� ���� � �������������
	//

*/
	//CBLModule7::OnErrorMessageEx(strGetRuntimeErrDescr, m_nID, m_strModulePath, GetRuntimeErrLineNum());
	CRuntimeExceptionClass::SetException(strGetRuntimeErrDescr, this, m_nID, m_strModulePath, GetRuntimeErrLineNum());
  }

	return GetRuntimeErrLineNum();
}

int CBLModuleWrapper::OnStartExecution(void)
{
	if (m_bModuleContValid)
		return CBLModule7::OnStartExecution();

	return 1;
}

int CBLModuleWrapper::OnNextLine(void)
{
	if (m_bModuleContValid)
		return CBLModule7::OnNextLine();

	return 1;
}

void CBLModuleWrapper::OnEnterProc(int n)
{
	if (m_bModuleContValid)
		CBLModule7::OnEnterProc(n);
}

void CBLModuleWrapper::OnExitProc(int n)
{
	if (m_bModuleContValid)
		CBLModule7::OnExitProc(n);
}

void CBLModuleWrapper::OnStopExecution(void)
{
	if (m_bModuleContValid)
		CBLModule7::OnStopExecution();
}
// enum EventType
// {
//   EventType_one,
//   EventType_two
// };

void CBLModuleWrapper::OnSetSpeller(class CValue const & val)
{
  CBLModule7::OnSetSpeller(val);
}

int CBLModuleWrapper::OnDoMessageBox(char const *pStr,unsigned int n1)
{
	return CBLModule7::OnDoMessageBox(pStr, n1);
}

void CBLModuleWrapper::OnGetErrorDescription(class CString &str)
{
  CBLModule7::OnGetErrorDescription(str);
}

CBLContext* CBLModuleWrapper::GetContextFromModule(CBLModule *pMod)
{
  CBLContext *pCont  = NULL;
  if(pMod)
  {
	pCont = pMod->GetInternalData()->pGeneralContext;
  }
  return pCont;
}

void CBLModuleWrapper::SetModulePathForRightUseIntoDebugger( const CString& strNewModulePath )
{
	m_strModulePath = strNewModulePath;
	m_strModulePath.MakeUpper(); // ����� � ��������� �� ������ ����� �������� ����� �������
}
