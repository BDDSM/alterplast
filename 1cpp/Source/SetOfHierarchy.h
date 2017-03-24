// SetOfHierarchy.h: interface for the CSetOfHierarchy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_)
#define AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ComponentClass.h"
#include "Preprocessor.h"
#include "OXY_Value.h"
#include <algorithm>
#include "MetaDataOfClasses.h"

/***********************************************************
* ������������������ �������������
* CSetOfHierarchy *pHierarchy = CSetOfHierarchy::GetHierarchy(); // ��������� ��������� �� ��������� ���
* pHierarchy->Init("���� � �������� ������� ��","���� � �������� Bin 1�"); // ��������������� ����
* pHierarchy->ProcessDefines(); // ����� ������� ������ ����� �����������
************************************************************/


extern CBkEndUI * pBkEndUI;

#define DLLEXPORT __declspec( dllexport )

// ������ ����� �������� �� ���������� ����� ��������� ������� ��� 1�.
// ��������� ��������� � �������������� ����������� ������� ���������, ��� �� ����������� ������������� � ���������.
// ��������� ������� ������ ����������� � ������� � ��������� ���� � ���������� ������.

class CSetOfHierarchy  
{
public:
  
	// ���������� ������ � ����� � ������ ����� ���������� ������ �� ����������� ��������
	CString GetNameOfFile(const CString& strNameOfMod);
  
  // �����������
	CSetOfHierarchy();
  // ����������
	~CSetOfHierarchy();

	// artbear ��� �������� ��������
	// 1 - ��������, 0 - ���
	bool VerifyRecursiveInBaseClasses(CString ClassName, CString BaseClassName, CString& sRecursiveHierarchy);

	// artbear - ���� �������� ����� ������ ����� OpenForm/������������ ��� �������
  bool bIsOpenCompForm;
  inline void SetIsOpenCompForm(bool _bIsOpenCompForm) { bIsOpenCompForm = _bIsOpenCompForm; };
  inline bool IsOpenCompForm() { return bIsOpenCompForm; };

  // ��������� ����������� ���� ����������� 1� ���������� �� ����� ����������
	void RegistersContextClasses();
  
  // ��������� ���� ���������� ������ � ��� ������������� ��������
  bool CheckTypeOfParam(CComponentClass* pComponentClass,
					   int nNumMeth,
					   CValue** ppParamOfMeth, 
             CBLProcInfo* pProcInfo,
					   CValue* pRetValue = NULL);

  // ������������� ��� ���������� �������� �� ��������� ����� ������� ������
  enum CParamDefValueEnum{
		pdve_DefValueNotExists = 0,
		pdve_DefValueIsExists = 1,
		pdve_ParamDefineWithoutDefValue = 2,
  };
  
  CParamDefValueEnum GetParamDefValue(int iMethodNum, int iParamNum, CValue* pDefValue, 
	  const CComponentClass* pComponentClass);

  // �������� ������������ ��������� ������ CSetOfHierarchy
  static CSetOfHierarchy* GetHierarchy();
  
  // ������� ������������ ��������� ������ CSetOfHierarchy
  static void DeInitHierarchy();
  
  // ���� � ������� ���� ������ 1�
  CString pathBase1C;
  
  // ���� � �������� � ������� ��������� ������������ ����� 1�
  CString pathBin1C;

  //������� ��� �������������
  CMapStringToPtr m_SymbolOfPreprocessor;
  
  static void RemoveNeedlessBlank(CString &str);
  
  bool TheClassIsDerive(const CString& derive, const CString& base) const;  
  bool IsObject(const CString& sClassName, const CString& sTryName) const;  

private:
	static void PrintError(const char* cstrError, const char* cstrFileName, int nNumLine);
  // ��������� ������� ������ ������� �� ������	
  
  CMetaDataOfClasses m_Meta;
  	
  // ������ � ����������� ����������� �� �������� � ����� ��� ������������ ��������������
  //CArray<COXY_Value*, COXY_Value*> m_ParamValOfMeth;
  
  // ������ ��������� ������ CSetOfHierarchy
  static CSetOfHierarchy* pGlobalHierarchy;

  // �������� ����� ����������� ������ ���������� include � �������� ������
  CMapStringToString m_LoadedIncludeFiles;
  
  // ������ ������ ����� ��������� ������ ����������, ��� ������� ���������� ���������� �������� �����.
  // ����� ��������� ����� ��������� � ������ ��������� �� �������� �� ��������� ������ = ""
  vector<int> m_VecNumOfParForPassCheckType;

public:  		
	void ReadDefinesAndCompile(CString &str, const CString &strNameFile);
	
  void Init(const char* IBDir = NULL, const char* BinDir = NULL);

	static void MakeFullFileName(CString* name, LPCSTR sSecondPath = NULL);
	bool IsMethExist(const char* pNameClass, const char* pNameMeth) const;
	CParamDefs::CArrayOfMethodsParamData_PTR GetInfoParamMeth(const char *pstrNameClass, const char *pstrNameMeth) const;
	CString GetNamesMethOfClass(const char* pNameClass, const char* sep = ",") const;
  // ����� ���������� ������ � ���������� ������� ������� ��� strNameCl, ������������ sep
	CString GetBaseClasses(const char* strNameCl, const char* sep = ",") const;
	void LoadListNameOfIncludeFiles(CStringArray& arr);
	CString GetAllAliasAndPath(const char* sep = "\r\n") const;
	CString GetAllClassesNames(const char* sep = "\r\n") const;
	CString GetDefSymbolOfPreproc(const char* sep = "\r\n") const;
	CString GetLoadIncludeFiles(const char* sep = "\r\n") const;
	int GetCountMethods() const;
	int GetCountClasses() const;
	bool IsClassExist(const char*);
	void InitParamDefsClass(CParamDefs * par, const CString& strClass);
	bool isMethHaveParVal(const CString& strName);
	bool ThisIsUnLimMethod(const CString& strName);
  
  // ����������� ��� ���������� ���������
  void ProcessDefines();

	CString GetPathFromAlias(const CString& strAls);
  // ������� ��� ������ � ������ ����������� ������ :

  CString GetClassNameFromDefinition(const CString& ClassName) const;

  CMetaDataOfClasses& GetMetaDataOfClasses(void)
  { return m_Meta; }

  void LoadNewClasssesDefine(CString &str, const CString &strNameFile);

private:
	CParamDefs::CArrayOfMethodsParamData_PTR GetMethodParamData(const CComponentClass* pComponentClass, int iMethodNum) const;
};

#endif // !defined(AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_)
