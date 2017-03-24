#include "stdafx.h"
#include "RuntimeException.h"

//////////////////////////////////////////////////////////////////////////
// CRuntimeExceptionClass class (c) artbear 2007-2008
//////////////////////////////////////////////////////////////////////////

// ======================================================================
//
// begin of CRuntimeExceptionClass
//

bool CRuntimeExceptionClass::isExceptionStatus = false;
CString CRuntimeExceptionClass::strTextException;
CValue* CRuntimeExceptionClass::ExceptionValue = NULL;

//CBLModule7* CRuntimeExceptionClass::pMod = NULL;
CSafeModulePtr CRuntimeExceptionClass::pMod;

int CRuntimeExceptionClass::iRuntimeErrLineNum = 0;
CString CRuntimeExceptionClass::m_strModulePath;
DWORD CRuntimeExceptionClass::m_nID = 0;
bool CRuntimeExceptionClass::m_bStatusOfTryingBlock = false; // ����, ��������� �� � ����� �������-����������

void CRuntimeExceptionClass::Init(void)
{
	ExceptionValue = new CValue;
	Empty();
}

void CRuntimeExceptionClass::Destroy(void)
{
	delete ExceptionValue;
}

void CRuntimeExceptionClass::Empty(void)
{
	isExceptionStatus = false;
	strTextException.Empty();

	pMod = NULL;
	iRuntimeErrLineNum = 0;
	m_strModulePath.Empty();
	m_nID = 0;

}

// ��������� ���������� � ������ � ���
void CRuntimeExceptionClass::SetException(const CString& strExceptionA)
{
	Empty(); // TODO

	isExceptionStatus = true;
	strTextException = strExceptionA;
}

void CRuntimeExceptionClass::SetException(const CString& strExceptionA, CBLModule7* pModA, DWORD m_nIDA, const CString& m_strModulePathA, int iRuntimeErrLineNumA)
{
	// ��� ������ CBLModule7::OnErrorMessageEx(strGetRuntimeErrDescr, m_nID, m_strModulePath, GetRuntimeErrLineNum());

	SetException(strExceptionA);

	pMod = pModA;
	m_nID = m_nIDA;
	m_strModulePath = m_strModulePathA;
	iRuntimeErrLineNum = iRuntimeErrLineNumA;
}

// ��������� ���������� ��� ���������������� ������� 1C++
void CRuntimeExceptionClass::ThrowException(bool isThrow)
{
	CString strTextException_copy(strTextException);
	//static CString* p_strTextException_copy = new CString;
	//static CString& strTextException_copy ( *p_strTextException_copy );
	//strTextException_copy = strTextException;

	CBLModule7* pMod_copy = static_cast<CBLModule7*>(pMod.operator CBLModule*()); // TODO
	DWORD m_nID_copy = m_nID;

	CString m_strModulePath_copy( m_strModulePath );
	int iRuntimeErrLineNum_copy = iRuntimeErrLineNum;

	Empty();

	if (pMod_copy)
	{
		// ��� ������� ������ �������� �������� �������� ����� �������� ��������� ��������� �� ������
		// �.�. ���� ������� ����� �����1, � ��� ������ ����� ���1, � ��� ���2, ����� �������� 3 ���������
		// ������� ��� ���2, ����� ���1, ����� �����1 - � ������� ������ �� ������ �� ��������� ��� �������� ������������� ������� ������� �� ��������������� ���� � �������������
		//
		if (!GetStatusOfTryingBlock() && !IsInsideTryingBlock())
		{

			pMod_copy->OnErrorMessageEx(strTextException_copy, m_nID_copy, m_strModulePath_copy, iRuntimeErrLineNum_copy);

		}
		::RuntimeError(strTextException_copy) ;
	}
	else
	{
		if (isThrow)
			::RuntimeError(strTextException_copy) ;
		else
			::ShowMsg(strTextException_copy, mmExclamation);
	}
}

// ��������� ���������� ��� ���������������� ������� 1C++
void CRuntimeExceptionClass::IfNeedExceptionThenThrowException(void)
{
	if (IsNeedException()) ThrowException(); 
}

// ��������� ���������� ��� ���������������� ������� 1C++
void CRuntimeExceptionClass::ThrowException(const CString& strException)
{
	SetException(strException);
	ThrowException();
}

// ��������� ����������, �������� ���������� ������. ��������� �� ������ ����� ������
void CRuntimeExceptionClass::RuntimeError(const CValue& param)
{
	Empty();
	*ExceptionValue = param;
	::RuntimeError("");
}

// ��������� ����������, �������� ���������� ������. ��������� �� ������ ����� ����� ����������
void CRuntimeExceptionClass::RuntimeError(const CValue& param, const CString& strNewTextException)
{
	Empty();
	*ExceptionValue = param;
	::RuntimeError(strNewTextException);
}

// �������� ����� ����������� ������-����������
void CRuntimeExceptionClass::GetException(CValue & rValue)
{
	rValue = *ExceptionValue;
	ExceptionValue->Reset();
}

// �������� ����, ��������� �� � ����� �������-����������
void CRuntimeExceptionClass::SaveStatusOfTryingBlock(void)
{
	m_bStatusOfTryingBlock = IsInsideTryingBlock(); // ����, ��������� �� � ����� �������-����������
}

bool CRuntimeExceptionClass::GetStatusOfTryingBlock(void)
{
	return m_bStatusOfTryingBlock;
}

// ���������, �� ��������� �� �� � ����� �������-����������
bool CRuntimeExceptionClass::IsInsideTryingBlock(void)
{

	CBLModule* pCurModule = CBLModule::GetExecutedModule();
	if (!pCurModule) 
		return false;
	if (!pCurModule->pIntInfo)
		return false;

	CExecutedModule* pExecutedModule = pCurModule->pIntInfo->pExecutedModule;
	if (!pExecutedModule)
		return false;

	// ��� �������� ���� - �������� ��� �������
	bool bRetInsideTryingBlock = (pExecutedModule->m_DWordArray2.GetSize() > pExecutedModule->val_23);

	// ��� ������� - ������ ���������, �.�. � ��� �������������� ���� ��������� ������ �������
	CBLContext* pCont = CBLModuleWrapper::GetContextFromModule(pCurModule);

	if (pCont && !bRetInsideTryingBlock && IS_KINDOF_CComponentClass(pCont))
	{
		bRetInsideTryingBlock = m_bStatusOfTryingBlock;
	}

	return bRetInsideTryingBlock;
}

//
// end of CRuntimeExceptionClass
// ======================================================================
