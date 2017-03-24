#if !defined(_RUNTIME_EXCEPTION_H__INCLUDED_)
#define _RUNTIME_EXCEPTION_H__INCLUDED_

//////////////////////////////////////////////////////////////////////////
// CRuntimeExceptionClass class (c) artbear 2007-2008
//////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ����� ��� ������ � ������������ 1�
class CRuntimeExceptionClass
{
private:
	CRuntimeExceptionClass() {}; // ����� ������ �������

	static bool isExceptionStatus;
	static CString strTextException;
	static CValue* ExceptionValue;

	//static CBLModule7* pMod;
	static CSafeModulePtr pMod;
	static int iRuntimeErrLineNum;
	static CString m_strModulePath;
	static DWORD m_nID; // -1 ��� 0
	static bool m_bStatusOfTryingBlock; // ����, ��������� �� � ����� �������-����������
public:
	static void Init(void);
	static void Destroy(void);

	static bool IsNeedException() { return isExceptionStatus; };
	static void Empty(void);

	// ��������� ���������� � ������ � ���
	static void SetException(const CString& strExceptionA);
	static void SetException(const CString& strExceptionA, CBLModule7* pModA,
		DWORD m_nIDA, const CString& m_strModulePathA, int iRuntimeErrLineNumA);

	// ��������� ���������� ��� ���������������� ������� 1C++
	static void IfNeedExceptionThenThrowException(void);

	// ��������� ����������, �������� ���������� ������. ��������� �� ������ ����� ������
	static void RuntimeError(const CValue& param);
	// ��������� ����������, �������� ���������� ������. ��������� �� ������ ����� ����� ����������
	static void RuntimeError(const CValue& param, const CString& strTextException);
	// �������� ����� ����������� ������-����������
	static void GetException(CValue & rValue);

	// �������� ����, ��������� �� � ����� �������-����������
	static void SaveStatusOfTryingBlock(void);
	static bool GetStatusOfTryingBlock(void);

	// ���������, �� ��������� �� �� � ����� �������-����������
	static bool IsInsideTryingBlock(void);

private:
	static void ThrowException(bool isThrow = true);
	static void ThrowException(const CString& strException);
};


#endif // _RUNTIME_EXCEPTION_H__INCLUDED_