// SetOfHierarchy.h: interface for the CSetOfHierarchy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_)
#define AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/***********************************************************
* ������������������ �������������
* CSetOfHierarchy *pHierarchy = CSetOfHierarchy::GetHierarchy(); // ��������� ��������� �� ��������� ���
* pHierarchy->Init("���� � �������� ������� ��","���� � �������� Bin 1�"); // ��������������� ����
* pHierarchy->ProcessDefines(); // ����� ������� ������ ����� �����������
************************************************************/


#define DLLEXPORT __declspec( dllexport )



// ������ ����� �������� �� ���������� ����� ��������� ������� ��� 1�.
// ��������� ��������� � �������������� ����������� ������� ���������, ��� �� ����������� ������������� � ���������.
// ��������� ������� ������ ����������� � ������� � ��������� ���� � ���������� ������.

class CSetOfHierarchy  
{
public:
///	typedef CArray<COXY_Value*, COXY_Value*> ARRAY_OXY_VAL;

	
  CComponentClass* GetComponentClassCurExModule(CBLModule* pModWrapper = NULL);

  void ClearImplicitParams(CComponentClass *pComp);

  CBLContext* GetImplicitParamsList(CComponentClass *pComp, const char* lpcsNameMeth);

  bool GetImplicitParams(CComponentClass *pComp, CValue **ppPar, int nNumMeth);
  int SetImplicitParamOfInd(CComponentClass *pComp, const char* lpcsNameMeth, int nIndex, CValue* pVal);

  int GetNParams(const CComponentClass *pComp, int nNumMeth, int nNParams);

  // ��������������� ��������� ������� ��������� � ����� �� �������� ����� ������
	void RestoreValParam(CComponentClass *pComponentClass, int nNumMeth, CValue **ppParam, ARRAY_OXY_VAL *arrValOfMeth);
  // ��������� ���������, ������� ��������� � ����� �� �������� ����� �������
	bool SaveValParam(CComponentClass* pComponentClass, int nNumMeth, CValue** ppParam, ARRAY_OXY_VAL *arrValOfMeth);
  // ���������� ������ � ����� � ������ ����� ���������� ������ �� ����������� ��������
	CString& GetNameOfFile(const CString& strNameOfMod);
  
  // �����������
	CSetOfHierarchy();
  // ����������
	~CSetOfHierarchy();

  // ��������� ������������� ���������� ���������� �� ������ ���������� ����������� �� ����� ������������� �������������
	bool InitInstanceComp(CComponentClass* pComponent);
  void DeInitInstanceComp(CBLModule *pModWrapper);

	// ��������� ����������� ���� ����������� 1� ���������� �� ����� ����������
	void RegistersContextClasses();
  
  // ��������� ���� ���������� ������ � ��� ������������� ��������
  bool CheckTypeOfParam(CComponentClass* pComponentClass,
					   int nNumMeth,
					   CValue** ppParamOfMeth, 
             CBLProcInfo* pProcInfo,
					   CValue* pRetValue = NULL);
  // ������������� ��� ���������� �������� �� ��������� ����� ������� ������
  int GetParamDefValue(int iMethodNum, int iParamNum, CValue* pDefValue, const CComponentClass* pComponentClass);

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

  // ��������� ��������� �� ���������� ������� � ����������� �� ������ �������� ��������� 
  // ���������� ������ ����� � ������ (...)
  struct InfoUnlimitsParams
  {
    CBLContext* cont_ValueList;
    vector<CValue*> vecPointOfPars;
    int         nNumAllParams;
    int         nNumExplicitParams;
  };
  
  static void RemoveNeedlessBlank(CString &str);
  
  typedef CArray<NameBindOfType*, NameBindOfType*> ARR_TYPE_NAME;
  
  bool TheClassIsDerive(const CString& derive, const CString& base) const;  

private:
	static void PrintError(const char* cstrError, const char* cstrFileName, int nNumLine);
  // ��������� ������� ������ ������� �� ������	
  
  CMetaDataOfClasses m_Meta;
  	
  //CMapPtrToPtr   m_BindInstaceValueList;  
  // ������ � ����������� ����������� �� �������� � ����� ��� ������������ ��������������
  //CArray<COXY_Value*, COXY_Value*> m_ParamValOfMeth;
  
  // ������ ��������� ������ CSetOfHierarchy
  static CSetOfHierarchy* pGlobalHierarchy;

  // ��������� �� ������� ����������� ������� ���
  // ��������� ��� ��������� ��� ��� ��������� �� ���� ������� ��������
  CComponentClass* pCreatedCompModule;

  // ����� � ����������� �� ������ � �� ��������� �����������
  // ���� - ��������� �� ������, �������� - ��������� �� �������� CComponentClass
  CMapPtrToPtr m_MapOfModulesAndComp;
   
  // �������� ����� ����������� ������ ���������� include � �������� ������
  CMapStringToString m_LoadedIncludeFiles;
  
  // ������ ������ ����� ��������� ������ ����������, ��� ������� ���������� ���������� �������� �����.
  // ����� ��������� ����� ��������� � ������ ��������� �� �������� �� ��������� ������ = ""
  vector<int> m_VecNumOfParForPassCheckType;

public:  		
	
  void Init(const char* IBDir = NULL, const char* BinDir = NULL);

	static void MakeFullFileName(CString* name);
	bool IsMethExist(const char* pNameClass, const char* pNameMeth) const;
	ARR_TYPE_NAME* GetInfoParamMeth(const char *pstrNameClass, const char *pstrNameMeth) const;
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
	void InitParamDefsClass(CParamDefs * par, CString strClass);
	bool isMethHaveParVal(CString strName);
	bool ThisIsUnLimMethod(CString strName);
	int m_CountParametersInCallMethod;
  
  // ����������� ��� ���������� ���������
  void ProcessDefines();

	CString GetPathFromAlias(CString strAls);
  // ������� ��� ������ � ������ ����������� ������ :
  CString m_strClassAndMethImplicit;
  CString m_strClassAndMethCurrent;  
};

#endif // !defined(AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_)
