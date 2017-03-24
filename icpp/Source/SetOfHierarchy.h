// SetOfHierarchy.h: interface for the ISetOfHierarchy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_)
#define AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ComponentClass.h"

// TODO #include "Preprocessor.h" - �������� !!
#include "Preprocessor.h"

#include <algorithm>
//#include "MetaDataOfClasses.h"
//#include "paramdefs.h"

// TODO ��� ������ ���������� ����� ����������� � ������ ����� !!
#define DLLEXPORT __declspec( dllexport )

//class CComponentClass;
class CMetaDataOfClasses;

// ������ ����� �������� �� ���������� ����� ��������� ������� ��� 1�.
// ��������� ��������� � �������������� ����������� ���������������� �������, ��� �� ����������� ������������� � ���������.
// ��������� ������� ������ ������������ � ������� � ��������� ���� � ���������� ������.

class ISetOfHierarchy  
{
public:
	// �������� ������������ ��������� ������ ISetOfHierarchy
	static ISetOfHierarchy* GetHierarchy();

	// ������� ������������ ��������� ������ ISetOfHierarchy
	static void DeInitHierarchy();

	static void MakeFullFileName(CString* name, LPCSTR sSecondPath = NULL);
	static const CMetaDataOfClasses& GetMetaDataOfClasses(void);

	virtual ~ISetOfHierarchy() {};

	virtual void Init() = 0;

	// artbear - ���� �������� ����� ������ ����� OpenForm/������������ ��� �������
	virtual inline void SetIsOpenCompForm(bool _bIsOpenCompForm) = 0;
	virtual inline bool IsOpenCompForm() const = 0;

	// ��������� ���� ���������� ������ � ��� ������������� ��������
	virtual bool CheckTypeOfParam(CComponentClass* pComponentClass,
		int nNumMeth,
		CValue** ppParamOfMeth, 
		CBLProcInfo* pProcInfo,
		CValue* pRetValue = NULL) = 0;

	// ������������� ��� ���������� �������� �� ��������� ����� ������� ������
	enum CParamDefValueEnum{
		pdve_DefValueNotExists = 0,
		pdve_DefValueIsExists = 1,
		pdve_ParamDefineWithoutDefValue = 2,
	};

	virtual CParamDefValueEnum GetParamDefValue(int iMethodNum, int iParamNum, CValue* pDefValue, 
		const CComponentClass* pComponentClass) = 0;

	virtual const CString& GetPathBase1C() const = 0;
	virtual const CString& GetPathBin1C() const = 0;

	//������� ��� �������������
	virtual CMapStringToPtr& GetMapOfSymbolOfPreprocessor() = 0;

	virtual bool TheClassIsDerive(const CString& derive, const CString& base) const = 0;

	virtual void ReadDefinesAndCompile(CString &str, const CString &strNameFile) = 0;

	virtual void LoadListNameOfIncludeFiles(CStringArray& arr) = 0;

	virtual CString GetDefSymbolOfPreproc(const char* sep = "\r\n") const = 0;
	virtual CString GetLoadIncludeFiles(const char* sep = "\r\n") const = 0;

	virtual void LoadNewClasssesDefine(CString &str, const CString &strNameFile) = 0;

	//virtual bool VerifyMethodForParamsByValueAndSetMetaInfoClassesDataForParamsByValue( const CString& strKeyOfUserClassMethod, const CBLProcInfo &procInfo) = 0;

	static CString GetKeyOfUserClassMethod(const CString& strClassName, LPCSTR strMethodName)
	{
		CString strKey = strClassName;
		strKey += ':';
		strKey += strMethodName;

		return strKey;
	};
};

//typedef ISetOfHierarchy CSetOfHierarchy; // ��� ������������� �� ������ ���������� �����

#endif // !defined(AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_)
