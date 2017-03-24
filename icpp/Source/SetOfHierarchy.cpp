// SetOfHierarchy.cpp: implementation of the CSetOfHierarchyImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "SetOfHierarchy.h"
#include <locale.h>
#include "istrmap.h"

#include "system\compare.h"
#include "MetaDataOfClasses.h"

//#include "Preprocessor.h"
////#include "OXY_Value.h"
//#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class CSetOfHierarchyImpl : public ISetOfHierarchy
{
public:

	CSetOfHierarchyImpl();
	~CSetOfHierarchyImpl();

	void Init();

	// artbear - ���� �������� ����� ������ ����� OpenForm/������������ ��� �������
	bool m_bIsOpenCompForm;
	inline void SetIsOpenCompForm(bool _bIsOpenCompForm) { m_bIsOpenCompForm = _bIsOpenCompForm; };
	inline bool IsOpenCompForm() const { return m_bIsOpenCompForm; };

	// ��������� ���� ���������� ������ � ��� ������������� ��������
	bool CheckTypeOfParam(CComponentClass* pComponentClass,
		int nNumMeth,
		CValue** ppParamOfMeth, 
		CBLProcInfo* pProcInfo,
		CValue* pRetValue = NULL);

	// // ������������� ��� ���������� �������� �� ��������� ����� ������� ������
	//enum CParamDefValueEnum{
	//	pdve_DefValueNotExists = 0,
	//	pdve_DefValueIsExists = 1,
	//	pdve_ParamDefineWithoutDefValue = 2,
	//};

	CParamDefValueEnum GetParamDefValue(int iMethodNum, int iParamNum, CValue* pDefValue, 
		const CComponentClass* pComponentClass);

	// �������� ������������ ��������� ������ ISetOfHierarchy
	static CSetOfHierarchyImpl* GetHierarchy();

	// ������� ������������ ��������� ������ ISetOfHierarchy
	static void DeInitHierarchy();

	const CString& GetPathBase1C() const { return pathBase1C; };
	const CString& GetPathBin1C() const { return pathBin1C; };

	CMapStringToPtr& GetMapOfSymbolOfPreprocessor()
	{  return m_SymbolOfPreprocessor; };

	bool TheClassIsDerive(const CString& derive, const CString& base) const
	{ return m_Meta.TheClassIsDerive(derive, base); };

public:  		
	void ReadDefinesAndCompile(CString &str, const CString &strNameFile);

	//static void MakeFullFileName(CString* name, LPCSTR sSecondPath = NULL);
	static void MakeFullFileName(CString* name, LPCSTR sSecondPath = NULL);

	void LoadListNameOfIncludeFiles(CStringArray& arr);

	CString GetDefSymbolOfPreproc(const char* sep = "\r\n") const;
	CString GetLoadIncludeFiles(const char* sep = "\r\n") const;

	//void InitParamDefsClass(CParamDefs * par, const CString& strClass);

	static const CMetaDataOfClasses& GetMetaDataOfClasses(void)
	{ return dynamic_cast<CSetOfHierarchyImpl*>(GetHierarchy())->m_Meta; }

	void LoadNewClasssesDefine(CString &str, const CString &strNameFile);

	//bool VerifyMethodForParamsByValueAndSetMetaInfoClassesDataForParamsByValue( const CString& strNamesClassAndMeth, const CBLProcInfo &procInfo);

private:
	CMetaDataOfClasses m_Meta;

	//������� ��� �������������
	CMapStringToPtr m_SymbolOfPreprocessor;

	static CSetOfHierarchyImpl* pGlobalHierarchy;

	// �������� ����� ����������� ������ ���������� include � �������� ������
	CMapStringToString m_LoadedIncludeFiles;

	// ������ ������ ����� ��������� ������ ����������, ��� ������� ���������� ���������� �������� �����.
	// ����� ��������� ����� ��������� � ������ ��������� �� �������� �� ��������� ������ = ""
	std::vector<int> m_VecNumOfParForPassCheckType;

private:

	// ��������� ����������� ���� ����������� 1� ���������� �� ����� ����������
	void RegistersContextClasses();

	// ����������� ��� ���������� ���������
	void ProcessDefines();

	//CParamDefs::CArrayOfMethodsParamData_PTR GetMethodParamData(const CComponentClass* pComponentClass, int iMethodNum) const;

	void ReadAllSystemAliasFromBin_Oxy_Ini_File();

	void ParsingString(CString& StringForParse, const CRangesOfDescr& RangesDescr);

	bool ProcessHeadDefinition(CString& strHead, CString& strResultClassName, int iNumberOfCurrentString, const CRangesOfDescr& RangesDescr );
	void ProcessMethodDefinition(const CString& NameOfClass, const CString& strDefMeth);

	void MessageBox(
		HWND hWnd ,
		const CString& lpText,
		const CString& lpCaption,
		UINT uType)
	{
		if(!m_Meta.m_bInsideDynamicAdd)
			::MessageBox(hWnd, lpText, lpCaption, uType);
		else
			RuntimeError("%s -- %s", lpCaption.operator LPCTSTR(), lpText.operator LPCTSTR());
	}

	// ���� � ������� ���� ������ 1�
	CString pathBase1C;

	// ���� � �������� � ������� ��������� ������������ ����� 1�
	CString pathBin1C;
};

ISetOfHierarchy* ISetOfHierarchy::GetHierarchy()
{
	return CSetOfHierarchyImpl::GetHierarchy();
}

void ISetOfHierarchy::DeInitHierarchy()
{
	CSetOfHierarchyImpl::DeInitHierarchy();
}

const CMetaDataOfClasses& ISetOfHierarchy::GetMetaDataOfClasses(void)
{
	return CSetOfHierarchyImpl::GetMetaDataOfClasses();
}

void ISetOfHierarchy::MakeFullFileName(CString* name, LPCSTR sSecondPath)
{
	CSetOfHierarchyImpl::MakeFullFileName(name, sSecondPath);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSetOfHierarchyImpl* CSetOfHierarchyImpl::pGlobalHierarchy = NULL;

CSetOfHierarchyImpl::CSetOfHierarchyImpl() : m_bIsOpenCompForm(false)
{
}

void CSetOfHierarchyImpl::Init()
{  
	CApp7* pApp7 = (CApp7*)AfxGetApp();
	CProfile7* pProfile7 = pApp7->GetProps();
	pathBin1C = pProfile7->GetStringProp(0x00);
	pathBase1C = pProfile7->GetStringProp(0x01);

	if (pathBin1C.IsEmpty())
		RuntimeError("�� ������� �������� �������� BinDir() ������� bin 1C!");

	ReadAllSystemAliasFromBin_Oxy_Ini_File();
	ProcessDefines();
	RegistersContextClasses();
}

CSetOfHierarchyImpl* CSetOfHierarchyImpl::GetHierarchy()
{
  if (pGlobalHierarchy == NULL)
    pGlobalHierarchy = new CSetOfHierarchyImpl;
  return pGlobalHierarchy;
}

void CSetOfHierarchyImpl::DeInitHierarchy()
{
  if (pGlobalHierarchy)
    delete pGlobalHierarchy;
}

CSetOfHierarchyImpl::~CSetOfHierarchyImpl()
{
	CString name;
	CString NameFile;
	//CFile file;  
	const CIStringMapToString& mapClassBindFile = pGlobalHierarchy->m_Meta.m_ClassBindFile;
	for (POSITION pos = mapClassBindFile.GetStartPosition(); pos != NULL;)
	{
		mapClassBindFile.GetNextAssoc( pos, name, NameFile);
		
		CSetOfHierarchyImpl::MakeFullFileName(&NameFile);
		
		//if (file.Open(NameFile, CFile::modeRead | CFile::shareDenyNone, NULL))
		if (FileExists(NameFile))
		{
			CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CComponentClass));      
			//file.Close();
		}    
	}    
}

// ��������� ����������� ���� ����������� 1� ���������� �� ����� ����������
void CSetOfHierarchyImpl::RegistersContextClasses()
{
	const CIStringMapToString* mapClassBindFile = &m_Meta.m_ClassBindFile;
	if (m_Meta.m_bInsideDynamicAdd)
		mapClassBindFile = &m_Meta.m_DynamicAddedClassBindFile;

  CString name;
  CString NameFile;
  //CFile file;  
  for (POSITION pos = mapClassBindFile->GetStartPosition(); pos != NULL;)
  {
    mapClassBindFile->GetNextAssoc( pos, name, NameFile);
    int npos = NameFile.Find("@MD");
    if (-1 == npos)
	    npos = NameFile.Find("@md");
    if (npos != -1)
    {
      CString nameRep = NameFile.Left(npos);
      CMetaDataCont *pMD = GetMetaData();
      CMetaDataObj* pObj = pMD->GetCalcVarDef(nameRep);  
      
	  if (pObj)
        RegisterCBLContext(RUNTIME_CLASS(CComponentClass), name);

      else
      {
        CString error;
        error = "�� ���������� ��������� � ������������: ";
        error += nameRep;
        error += " ��� ������: ";
        error += name;
        pBkEndUI->DoMessageLine(error,mmUnderlinedErr);
        pBkEndUI->DoMessageLine("����������� ������ ����������� ��������!",mmUnderlinedErr);
      }      
    }
    else
    {    
      CSetOfHierarchyImpl::MakeFullFileName(&NameFile);    
	  
	  if (FileExists(NameFile))
        RegisterCBLContext(RUNTIME_CLASS(CComponentClass), name);

	  // ���� --->
	  // ����� ������� � dll ������������ ��� ����
	  // ��������, � ������� ��� ����� ���� ����� ����������, �� ���� � ������ �����
	  // �������� ���
	  else if(NameFile.Find(".dll") != -1)
        RegisterCBLContext(RUNTIME_CLASS(CComponentClass), name);
	  // ���� <---
	  
      else
      {
        CString error;
        error = "�� ���������� �����: ";
        error += NameFile;
        error += " ��� ������: ";
        error += name;
        pBkEndUI->DoMessageLine(error,mmUnderlinedErr);
        pBkEndUI->DoMessageLine("����������� ������ ����������� ��������!",mmUnderlinedErr);
      }
    }
    
  }
}

CSetOfHierarchyImpl::CParamDefValueEnum CSetOfHierarchyImpl::GetParamDefValue(int iMethodNum, int iParamNum, CValue* pDefValue,
                                      const CComponentClass* pComponentClass)
{
  if (pComponentClass == NULL || pDefValue == NULL)
	return pdve_DefValueNotExists;  

  int nRealParamNum = iParamNum;
  if (pComponentClass->HasRetVal(iMethodNum) == 1)  
	iParamNum++; // ��� ���������� ������ ��� � ������� ������ ���� ������������ ��������  

	CParamDefs::CArrayOfMethodsParamData_PTR param = pComponentClass->GetMethodParamData(iMethodNum);
    if (!param)
		return pdve_DefValueNotExists;

    if (param->GetSize() <= iParamNum)      
		return pdve_DefValueNotExists;
  
	CMethodsParamData::Ptr NameBind = param->operator[](iParamNum);
	CString strVal  = NameBind->GetValue();
	const CString& strType = NameBind->GetType();      

	if (strVal.IsEmpty() || strType.IsEmpty())
		return pdve_ParamDefineWithoutDefValue;

    //if (!strVal.IsEmpty() && !strType.IsEmpty())
    //{
    int res = 1;
    //if (!strType.CompareNoCase("�����") || !strType.CompareNoCase("Number"))
	if (!FastCompareNoCase.Compare(strType, "�����") || !FastCompareNoCase.Compare(strType, "Number"))
    {          
      if (strVal != "\"\"")
      {
        CNumeric num;
        char** ppChar = NULL;
        num.FromString(strVal, ppChar);          
        *pDefValue = num;
        if (pDefValue->IsEmpty())
        {
          CBLModule* pM = pComponentClass->GetModule();
          CValue* masval[1];
          CValue val;
          masval[0] = &val;
          res = pM->EvalExpr(strVal, *pDefValue, &masval[0]);            
        }          
      }
      else
      {
        pDefValue->Reset();
        if (pMainAddIn && pMainAddIn->m_EnabledCheckType)            
          m_VecNumOfParForPassCheckType.push_back(nRealParamNum);             
      }
    }
    //else if (!strType.CompareNoCase("����") || !strType.CompareNoCase("Date"))
	else if (!FastCompareNoCase.Compare(strType, "����") || !FastCompareNoCase.Compare(strType, "Date"))
    {
      int year, month, day;
      year = 0; month = 0; day = 0;          
      if (strVal != "\"\"")
      {
        if (strVal.Find('\'') == -1) // TODO �������� �� ������ ���������
        {
          CBLModule* pM = pComponentClass->GetModule();
          CValue* masval[1];
          CValue val;
          masval[0] = &val;
          res = pM->EvalExpr(strVal, *pDefValue, &masval[0]);
        }
        else
        {
          strVal.Replace("'", NULL);          
          int nLen = strVal.GetLength();
          CString token;
          int what = 0;
          for (int i = 0; i < nLen; i++)
          {
            if (strVal[i] == '.')
            {
              if (what == 0 && !token.IsEmpty())
              {
                // ����
                day = atoi(token);
                what = 1;                
              }
              else if (what == 1 && !token.IsEmpty())
              {
                // �����
                month = atoi(token);
                what = 2;
              }
              token.Empty();
              continue;
            }
            token += strVal[i];
          }
          if (what == 2 && !token.IsEmpty())
            year = atoi(token); // ���
          
          CDate date(year, month, day);
          *pDefValue = date;
        }
      }
      else
      {
        pDefValue->Reset();
        if (pMainAddIn && pMainAddIn->m_EnabledCheckType)            
          m_VecNumOfParForPassCheckType.push_back(nRealParamNum);             
      }
      
    }
    //else if (!strType.CompareNoCase("������") || !strType.CompareNoCase("String"))
	else if (!FastCompareNoCase.Compare(strType, "������") || !FastCompareNoCase.Compare(strType, "String"))
    {
	  // ��������� ������ ���������������� ��� ������� ���� 2128 http://cvs.alterplast.ru/bugs/show_bug.cgi?id=2128
      //if (strVal != "\"\"")
      {            
        if (strVal[0] == '"' && strVal[strVal.GetLength()-1] == '"')
        {
          strVal.Replace("\"", NULL);          
          *pDefValue = strVal;            
        }
        else
        {
          CBLModule* pM = pComponentClass->GetModule();
          CValue* masval[1];
          CValue val;
          masval[0] = &val;
          res = pM->EvalExpr(strVal, *pDefValue, &masval[0]);
        }
      }
    }
    else
    {
      if (strVal != "\"\"")
      {
        if (strVal[0] == '"' && strVal[strVal.GetLength()-1] == '"')
        {
          strVal.Replace("\"", NULL);          
          pDefValue->CreateObject(strVal); 
        }
        else
        {
          CBLModule* pM = pComponentClass->GetModule();
          CValue* masval[1];
          CValue val;
          masval[0] = &val;
          res = pM->EvalExpr(strVal, *pDefValue, &masval[0]);
        }
      }
      else
      {
        pDefValue->Reset();
        if (pMainAddIn && pMainAddIn->m_EnabledCheckType)            
          m_VecNumOfParForPassCheckType.push_back(nRealParamNum);             
      }
    }
    if (res == 0)
    {          
      CString strError;
      strError = "�� ����� ������� ��������� �������� �� ���������(";
      strError += strVal;
      strError += ")\r\n��� ���������:";
      strError += NameBind->GetName();
      strError += " ������: ";
      CString NameOfClass = pComponentClass->GetTypeString();
      CString NameMeth = pComponentClass->GetMethodName(iMethodNum, 1);
      strError += NameOfClass + "::"+NameMeth;
      pBkEndUI->DoMessageLine(strError, mmBlackErr);	
    }
    return pdve_DefValueIsExists;
    //}

  return pdve_DefValueNotExists;
}

// TODO �����, ������ �����, ��������� � CComponentClass
//CParamDefs::CArrayOfMethodsParamData_PTR CSetOfHierarchyImpl::GetMethodParamData(const CComponentClass* pComponentClass, int iMethodNum) const
//{
//	//CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData pMapFunc = pComponentClass->GetDefFnNamesPtr()->GetPtrOfMapMethodsNameToArrayOfMethodsParamData();
//
//	//if (!pMapFunc)
//	//	return CParamDefs::CArrayOfMethodsParamData_PTR(); // NULL
//
//	//CParamDefs::CArrayOfMethodsParamData_PTR param = (*pComponentClass->GetDefFnNamesPtr())[iMethodNum].m_pArrayOfMethodsParamData;
//	//if (!param)
//	//{
//	//	CString NameMeth = pComponentClass->GetMethodName(iMethodNum, 1);
//	//	pMapFunc->Lookup(NameMeth, param);
//	//}
//
//	//return param;
//	return pComponentClass->GetMethodParamData(iMethodNum);
//}

// TODO ���� ����� ���� ������ ������������ ������ CComponentClass - artbear
bool CSetOfHierarchyImpl::CheckTypeOfParam (CComponentClass* pComponentClass, int nNumMeth,
                                        CValue** ppParamOfMeth, CBLProcInfo* pProcInfo, 
                                        CValue* pRetValue)
{  
	bool result = true;
	int num = pComponentClass->GetNParams(nNumMeth);
  
	class CLocalGuard
	{
		std::vector<int>& m_VecNumOfParForPassCheckType;
	public:
		CLocalGuard(std::vector<int>& VecNumOfParForPassCheckType) : 
			m_VecNumOfParForPassCheckType(VecNumOfParForPassCheckType) {};
		~CLocalGuard() { m_VecNumOfParForPassCheckType.clear(); };
	};
	CLocalGuard guard(m_VecNumOfParForPassCheckType);

  if (NULL == pRetValue && num <= 0)
	return result;
	
	CParamDefs::CArrayOfMethodsParamData_PTR param = pComponentClass->GetMethodParamData(nNumMeth);

  if (!param)
		return result;

    int nSize = param->GetSize();
    bool IsHasRetVal = false;
    
    if (pComponentClass->HasRetVal(nNumMeth) == 1)
    {
      num++;
      IsHasRetVal = true;
    }
    
    nSize =  num < nSize ? num : nSize;
    int nNumParam = 0;
    CStringList ListOfErrors;        
    bool IsRetValue = false;        
    for (int i = 0; i < nSize && !IsRetValue; i++, nNumParam++)
    {    
      
      const CMethodsParamData::Ptr NameBind = param->operator[](i);
      CString pName;
      CBLContext* pCont = NULL;
      
      if (NameBind->CompareWithType("...") == 0)
        break;          
      
      if (IsHasRetVal && i == 0 && pRetValue != NULL && !NameBind->CompareWithType("#return"))
      {
        // ��������� ��� ��������
        pName = pRetValue->GetTypeString();
        pCont = pRetValue->GetContext();
        nNumParam--;
        IsRetValue = true;                        
      }
      else if (IsHasRetVal && pRetValue == NULL && i == 0 && !NameBind->CompareWithType("#return"))
      {
        // ������� ���� ��������� � ������� ���������� �� ���������, ������� ����������
        // ���������� � ������������ ����, �.�. ������� ���� � ��� ������ � �������
        nNumParam--;
        continue;
      }          
      else
      {
        // �������� ��������� ������
        if (num < nNumParam)
          break;
        
		if (std::find(m_VecNumOfParForPassCheckType.begin(), m_VecNumOfParForPassCheckType.end(), nNumParam) != m_VecNumOfParForPassCheckType.end())
          continue;
        
        pName = ppParamOfMeth[nNumParam]->GetTypeString();
        pCont = ppParamOfMeth[nNumParam]->GetContext();
      }
      
      if ((NameBind->CompareWithType("��������������") == 0)||(NameBind->CompareWithType("Undefine") == 0)) // ������ ���������
        continue;
      
      if(pCont != NULL)
      {
        int meth = pCont->FindMethod("Kind");
        if (meth != -1 && stricmp("CComponentClass", pCont->GetRuntimeClass()->m_lpszClassName)) // ����� ����� ������
        {
          CValue value;
          CValue ValueParam;
          CValue *pMassVal[1] = {0};
          pMassVal[0] = &ValueParam;
          pCont->CallAsFunc(meth, value, &pMassVal[0]);
          const CString& Kind = value.GetString();
          if (!Kind.IsEmpty())
          {
            pName += '.';
            pName += Kind;
          }              
        }
      }
      else
      {
        
        if (pName.Find("����������") != -1 || pName.Find("��������") != -1 || pName.Find("������������") != -1 )
        {              
          long lll = 0;
          CValue *pCheckValue = IsRetValue ? pRetValue : ppParamOfMeth[nNumParam];
          pName = pMetaDataCont->GetFullName(pCheckValue->GetRealTypeID(), &lll, 1);                 
        }
        else if (pName.Find("Referens") != -1 || pName.Find("Document") != -1 || pName.Find("Enum") != -1)
        {
          long lll = 0;
          CValue *pCheckValue = IsRetValue ? pRetValue : ppParamOfMeth[nNumParam];
          pName = pMetaDataCont->GetFullName(pCheckValue->GetRealTypeID(), &lll, 0);
        }            
      }
      //if (pName.IsEmpty())
      //  continue;
      
      CString strNameEng;
      if (!FastCompareNoCase.Compare(pName, "������")) //if (!pName.CompareNoCase("������"))
      {
        strNameEng = "String";
      }
      else if (!FastCompareNoCase.Compare(pName, "����")) // else if (!pName.CompareNoCase("����"))
      {
        strNameEng = "Date";
      }
      else if (!FastCompareNoCase.Compare(pName, "�����")) // else if (!pName.CompareNoCase("�����"))
      {
        strNameEng = "Number";
      }          
      
      if (NameBind->CompareWithType(pName) != 0 && NameBind->CompareWithType(strNameEng) != 0)
      {	        
        // ����� ���������� ��������� ����������� ����, ��� ��� ������ �������� ����, � ����
        // ��� �����������. � ���� ������ ������������ �� �������� �������
        if (NameBind->CompareWithType("��������") == 0 && pName.Find("��������") >= 0)
          continue;
        if (NameBind->CompareWithType("Document") == 0 && pName.Find("Document") >= 0)
          continue;
        if (NameBind->CompareWithType("����������") == 0 && pName.Find("����������") >= 0)
          continue;
        if (NameBind->CompareWithType("Referens") == 0 && pName.Find("Referens") >= 0)
          continue;
        if (NameBind->CompareWithType("�������") == 0 && pName.Find("�������") >= 0)
          continue;
        if (NameBind->CompareWithType("Registr") == 0 && pName.Find("Registr") >= 0)              
          continue;
        if (NameBind->CompareWithType("�����������������") == 0 && pName.Find("�����������������") >= 0)              
          continue;
        if (NameBind->CompareWithType("GroupContext") == 0 && pName.Find("GroupContext") >= 0)              
          continue;
        if (NameBind->CompareWithType("������������") == 0 && pName.Find("������������") >= 0)
          continue;
        if (NameBind->CompareWithType("Enum") == 0 && pName.Find("Enum") >= 0)
          continue;
        
        if (m_Meta.TheClassIsDerive(pName, NameBind->GetType()))
          continue;
        CString NameOfClass = pComponentClass->GetTypeString();
        CString NameMeth = pComponentClass->GetMethodName(nNumMeth, 1);
        
        CString strErr = "� ������: ";
        strErr += NameMeth;
        
        strErr += " �� ��������� ���� � ";
        if (IsRetValue)
          strErr += "������������ ��������";
        else
        {
          strErr += "���������: ";
          //strErr += NameBind->name;              
          CBLVarInfo VarInfo;
          pProcInfo->GetVarDescr(nNumParam, VarInfo);              
          strErr += VarInfo.GetName();
        }
        strErr += ". ���������� ���: ";
        strErr += NameBind->GetType();
        strErr += "; ���������� ���: ";
        strErr += pName;
        ListOfErrors.AddTail(strErr);
        result = false;            
      }
    }
    if (!ListOfErrors.IsEmpty()) // ���� ������
    {
      CBLModule *pCurModule = CBLModule::GetExecutedModule();
      if (pCurModule) {
// 		  char buf[100];
// 		  CString str(32,500);
// 		  int nLine = pCurModule->GetExecutedLineNum();
// 		  pCurModule->GetCurSourceLine(nLine, str);
// 		  sprintf(buf, "������ � ������� ������! ������: %d", nLine);
// 		  pBkEndUI->DoMessageLine(buf, mmUnderlinedErr);
// 		  str.TrimLeft();
// 		  pBkEndUI->DoMessageLine(str, mmUnderlinedErr);
        CString Error;
        for(POSITION posList = ListOfErrors.GetHeadPosition(); posList != NULL; )
        {
          Error = ListOfErrors.GetNext( posList );
          pBkEndUI->DoMessageLine(Error, mmUnderlinedErr);	           
        }
        CString strError = "������: �������� ��� ������ � ������: ";            
        CString NameOfClass = pComponentClass->GetTypeString();
        strError += NameOfClass;
        strError += " ����: ";
        strError += m_Meta.m_ClassBindFile[NameOfClass];
        //pBkEndUI->DoMessageLine(strError, mmUnderlinedErr);
        CBLModule::RaiseExtRuntimeError(strError,0);
      }
    }        
  //}      
    //}
  //} 
  //m_VecNumOfParForPassCheckType.clear();
  return result;
}

void CSetOfHierarchyImpl::ReadDefinesAndCompile(CString &str, const CString &strNameFile)
{
  try 
  {
    CRangesOfDescr Ranges;
    //CPreprocessor::Preprocess(false, str, m_SymbolOfPreprocessor, m_LoadedIncludeFiles, strNameFile, &Ranges);
	CPreprocessor::PreprocessString(str, m_SymbolOfPreprocessor, m_LoadedIncludeFiles, strNameFile, &Ranges);
    ParsingString(str, Ranges); //m_Meta.ParsingString(str, Ranges);
  }
  catch (CPreprocessor::ExeptionPreprocessor &ex)
  {
	ShowMsg("� ������ �����: %s", mmBlackErr, strNameFile.operator LPCTSTR());	
    ShowMsg(ex.GetErrorString(), mmBlackErr);	
    if (!ex.GetCodeString().IsEmpty())
	if(!m_Meta.m_bInsideDynamicAdd)
		ShowMsg("(%d) %s", mmBlackErr, ex.GetNumString(), ex.GetCodeString().operator LPCTSTR());
	else
		RuntimeError("(%d) %s", mmBlackErr, ex.GetNumString(), ex.GetCodeString().operator LPCTSTR());
  }
}

void CSetOfHierarchyImpl::ProcessDefines()
{
  if (pathBase1C.IsEmpty())
  {
    pBkEndUI->DoMessageLine("�� ������� �������� �������� IBDir() ������� ��!", mmBlackErr);	
  }
  else
  {
    TRY 
    {     
      CString strDefFileName = pathBase1C;      
      strDefFileName += pcstrNameOfDef;
      
      // TODO �������� ����� �� �������� �������� http://cvs.alterplast.ru/bugs/show_bug.cgi?id=1605
      // fez TODO �������� ����� �� �������� �������� http://itland.ru/forum/index.php?showtopic=11696
      CString str;
      ReadFileToStringWithExceptions(strDefFileName, str);

      ReadDefinesAndCompile(str, strDefFileName);
    }
    CATCH(CFileException, pEx)
    {
      CModuleString str;
      CString strDefFileName = "defcls@MD";
      if (str.LoadModule(strDefFileName))
      {
        ReadDefinesAndCompile(str, strDefFileName);
        
      }
      else // fez TODO �������� �����-������ �����������, �� �� ������ ��� ���, ��� �������� ������ �� ���������� (��� ������ ������� defcls.prm)
      {
        //pBkEndUI->DoMessageLine("�� ������� ������� ���� ����������� �������", mmBlackErr);	
        //pBkEndUI->DoMessageLine(pathBase1C+pcstrNameOfDef, mmBlackErr);	
      }
      
    }
    AND_CATCH(CMemoryException, pEx)
    {
      pBkEndUI->DoMessageLine("�������� �� ������� ������ ��� �������� ������ � ������� CSetOfHierarchyImpl::CSetOfHierarchyImpl()", mmExclamation);	
    }
    END_CATCH
  }
}

//// TODO ����� ����������� ���� ������� !!
//void CSetOfHierarchyImpl::InitParamDefsClass(CParamDefs *par, const CString& strClass)
//{  
//  if (!m_Meta.m_MapClassNamesToMethodsMap.Lookup(strClass, par->GetPtrOfMapMethodsNameToArrayOfMethodsParamData()))
//    par->CleanPtrOfMapMethodsNameToArrayOfMethodsParamData(); //par->pMapFunc = NULL;
//}

CString CSetOfHierarchyImpl::GetLoadIncludeFiles(const char* sep) const
{
  CString key;
  POSITION pos;
  CString res;  
  CString val;    
  for (pos = m_LoadedIncludeFiles.GetStartPosition(); pos != NULL;)
  {
    m_LoadedIncludeFiles.GetNextAssoc( pos, key, val);    
    res += key;
    res += sep;        
  }  
  return res;
}

CString CSetOfHierarchyImpl::GetDefSymbolOfPreproc(const char* sep) const
{
  CString key;
  POSITION pos;
  CString res;  
  for (pos = m_SymbolOfPreprocessor.GetStartPosition(); pos != NULL;)
  {
    void *ptr;    
    m_SymbolOfPreprocessor.GetNextAssoc( pos, key, ptr);
    res += key;
    res += sep;
  }  
  return res;
  
}

void CSetOfHierarchyImpl::LoadListNameOfIncludeFiles(CStringArray &arr)
{
  CString key;
  POSITION pos;  
  CString val;    
  for (pos = m_LoadedIncludeFiles.GetStartPosition(); pos != NULL;)
  {
    m_LoadedIncludeFiles.GetNextAssoc( pos, key, val);
    arr.Add(key);
  }    
}

// �������� ������ ���� ����� ��� �� �������� �� 
// ���, ���� ���� �� ������ � �������� ��, ���� ������������ ����������� ��������
// ���� � ����� ���� �� ������, ������������ ���� ������������ �������� ��
//
void CSetOfHierarchyImpl::MakeFullFileName(CString* name, LPCSTR sSecondPath)
{
//Msg("%s %s", "name", (LPCTSTR) *name); 
  name->Replace("\"", ""); // artbear ����� �������

  if (name->Find(':') == -1) //if (name->Find(":") == -1)
  {
    if (name->Find("@MD") == -1)
    if (name->Find("@md") == -1)
    {
// artbear
//      if (name->GetLength() > 1 && (name->GetAt(0) != '\\' || name->GetAt(0) != '.') && name->GetAt(1) != '\\')
//        name->Insert(0, pGlobalHierarchy->pathBase1C);
	
      if (('.' == name->GetAt(0) && '\\' == name->GetAt(1)) || (name->GetLength() > 1 && (name->GetAt(0) != '\\') && name->GetAt(1) != '\\'))
	{
  		CString sPath =  *name;
		sPath.Insert(0, pGlobalHierarchy->pathBase1C);
		sPath = ::AbsoluteFilePath(sPath);
//Msg("%s %s", "sPath", (LPCTSTR) sPath); 

		bool bFileExists = ::FileExists((LPCTSTR)sPath);
		if (bFileExists || !sSecondPath)
			*name = sPath; // TODO ������� ����� ������� ����� ����. ������
		else
		{
			//Msg("������� ��� ������ ������������ �������� �� %s", (LPCTSTR)*name);  			
			CString sPath =  *name;
			sPath.Insert(0, sSecondPath);
			sPath = ::AbsoluteFilePath(sPath);
//Msg("%s %s", "sPath", (LPCTSTR) sPath); 

			bool bFileExists = ::FileExists((LPCTSTR)sPath);
			if (bFileExists)
				*name = sPath; // TODO ������� ����� ������� ����� ����. ������
		}
	}
// end
    }
  }
}

void CSetOfHierarchyImpl::LoadNewClasssesDefine(CString &str, const CString &strNameFile)
{
	CFlagGuard guard(m_Meta.m_bInsideDynamicAdd, true);
	try
	{
		ReadDefinesAndCompile(str, strNameFile);
		RegistersContextClasses();
	}
	catch(...)
	{
		m_Meta.m_DynamicAddedClassBindFile.RemoveAll();
		throw;
	}

	m_Meta.m_DynamicAddedClassBindFile.RemoveAll();
}

void CSetOfHierarchyImpl::ReadAllSystemAliasFromBin_Oxy_Ini_File()
{
	CFile file;
	CString strFileNameIni = pathBin1C;
	strFileNameIni += "OXY.ini";
	if (file.Open(strFileNameIni, CFile::modeRead | CFile::shareDenyNone))
	{          
		DWORD nLength = file.GetLength();
		file.Close();
		std::vector<char> pBuf(nLength+2); //char* pBuf = new char[nLength+2];
		int nSize = 0;
		nSize = GetPrivateProfileSection("alias_path", &pBuf[0], nLength+2, strFileNameIni);
		if (nSize > 0)
		{
			CString str;
			for (int i = 0; i < nSize; i++)
			{
				
				if (pBuf[i] == '\0')
				{         
					int nPosEq = str.Find('='); //("=");
					if (-1 != nPosEq)
					{
						CString strAlias       = str.Left(nPosEq);
						CString strPathOfAlias = str.Right(str.GetLength() - nPosEq - 1);
						strAlias.TrimLeft(); strAlias.TrimRight();
						strPathOfAlias.TrimLeft(); strPathOfAlias.TrimRight();
						m_Meta.m_AliasOfPaths[strAlias] = strPathOfAlias;
						//pBkEndUI->DoMessageLine(strAlies, mmBlackErr);	
						//pBkEndUI->DoMessageLine(strPathOfAlies, mmBlackErr);	
					}
					str.Empty();
					if (pBuf[i+1] == '\0')
						break; // ����� ������
					continue;
				}
				str += pBuf[i];        
			}
		}          
		//delete pBuf;
	}        
}

static void ReplaceCommentsForSymbolOne(CString& StringForParse, char cSymbolForReplacedComments)
{
	int posComment = 0;
	
	// ������ �����������
	int nStart = 0;
	const int nLength = StringForParse.GetLength();
	
	while ((posComment = StringForParse.Find("//", nStart)) != -1)
	{    
		int nCount = 0;    
		if ((nLength != posComment+2)&&(StringForParse[posComment+2] == '#'))
		{
			nCount = 3;
		}
		else
		{
			int i = posComment;
			while(i != nLength && StringForParse[i] != '\r')
			{
				++i;
				++nCount;
			}
		}    
		char* p = const_cast<char*>(StringForParse.operator LPCTSTR());
		memset(p+posComment, cSymbolForReplacedComments, nCount); 
	}
}

// ����� ��������� ������ ������ � ��������� ���������� ��������� ������. 
// ������ ������ �������� �� ���������� ������� � ��������� � ������ ������ 
// �������� ���������� � ������������� ��������� ���� �������

void CSetOfHierarchyImpl::ParsingString( CString& StringForParse, const CRangesOfDescr& RangesDescr )
{
  if (StringForParse.IsEmpty())
    return;

  const int nLength = StringForParse.GetLength();
  const char cSymbolForReplacedComments = 1;
  ReplaceCommentsForSymbolOne(StringForParse, cSymbolForReplacedComments);

  CString token;
  for (int i = 0; i < nLength; i++)
  {
    CHAR symbol = StringForParse[i];
    
    if (symbol == ' ' || symbol == '\n' || symbol == '\r' || symbol == cSymbolForReplacedComments)
      continue;

    // ����� ���������� ����� �������� ����� class ��� �����
    token.Empty();
    while(symbol != ' ' && symbol != '\r' && symbol != '\t' && symbol != '\n')
    {            
      token += symbol;
      i++;
      if (i == nLength)
          break;
      symbol = StringForParse[i];
    }
	if (token.IsEmpty())
		continue;

	if (FastCompareNoCase.Compare(token, "class") != 0 && FastCompareNoCase.Compare(token, "�����") != 0)
	{
        token.Empty();
        continue;
	}

    // ���������� ����������� ������
    CString DefClass;
    //st = 0;
    bool isClasDef = false;
    for (;i < nLength; i++)
    {
      const CHAR symbol = StringForParse[i];                    
      if (symbol == '}')
      {
        //st = 1;
        isClasDef = true;
        DefClass += symbol;
        break;
      }
      //if (symbol == ';' && st == 1)
      //{
      //  isClasDef = true;
      //  break;
      //}
      if (symbol != cSymbolForReplacedComments)
        DefClass += symbol;
    }
    
	if (!isClasDef || DefClass.IsEmpty())
    {
		MessageBox(NULL, DefClass, "������: ������� ��������� �����", MB_ICONERROR);
		break;
    }
    
	// ����������� ������ ��������� � DefClass
	// �������� �� ��������� ������, ��� ���������� ��� ������, ���� ���������� � �� 
	// ����������� ������� � �������� ����������� � �������� �������

	int nPosFigOpen  = DefClass.Find('{'); //("{");
	int nPosFigClose = DefClass.Find('}');//("}");
	if (nPosFigClose == -1 || nPosFigOpen == -1)
	{
		MessageBox(NULL, DefClass, "������: �� ������� ������ } ��� {",MB_ICONERROR);
		break;
	}

	CString strHead    = DefClass.Left(nPosFigOpen+1);
	CString strMethodsDefinition = DefClass.Mid(nPosFigOpen+1, nPosFigClose - nPosFigOpen - 1);

	strHead.TrimRight();
	strHead.TrimLeft();

	if (strHead.IsEmpty())
	{
		MessageBox(NULL, DefClass, "������: ������������ �������� ������", MB_ICONERROR);
		break;
	}

	CString NameOfClass;
	if (!ProcessHeadDefinition(strHead, NameOfClass, i, RangesDescr ))
		continue;

	if (!strMethodsDefinition.IsEmpty())
		ProcessMethodDefinition(NameOfClass, strMethodsDefinition);
	}  // for
}

bool CSetOfHierarchyImpl::ProcessHeadDefinition(CString& strHead, CString& strResultClassName, int iNumberOfCurrentString, const CRangesOfDescr& RangesDescr )
{
    CString NameOfFile;
    CString strAlies;
    CString NameOfBaseClass;
    
	CUserClassMetaData::CStringList_SharedPtr pList(new CUserClassMetaData::CStringVector); //CStringList_SharedPtr pList(new CStringList);
    //strHead.Replace(" ", NULL); // TODO artbear ����� ����, ���-���� ����� ������� �������

    int nLen = strHead.GetLength();
	enum ParserState
	{
		stNameOfClass = 0,
		stNameOfFile = 1,
		stNameOfBaseClass = 2,
		stNameOfBaseClassFinish = 3,
		stAlies = 4,
		stAliesFinish = 5,
	};
    ParserState state = stNameOfClass; //0;
    for (int i2 = 0; i2 < nLen; i2++)
    {            
      if (strHead[i2] == ' ' || strHead[i2] == '\n' || strHead[i2] == '\t' || strHead[i2] == 9 || strHead[i2] == '\r')
        continue;
      
      if (strHead[i2] == '=')
      {
        state = stNameOfFile; //1;
        continue;
      }
      if (strHead[i2] == ':' && strHead[i2+1==nLen?i2:i2+1] != '\\')
      {
        state = stNameOfBaseClass; //2;
        continue;
      }
      if (strHead[i2] == ',' || strHead[i2] == '{')
        state = stNameOfBaseClassFinish; //3;
      
      if (strHead[i2] == '#' && state == stNameOfFile ) //1)
      {
        state = stAlies; //4;
        continue;
      }
      
      if (strHead[i2] == '\\' && state == stAlies ) //4)
        state = stAliesFinish; //5;

      switch (state)
      {
      case stNameOfClass: //0:
        strResultClassName += strHead[i2];
        break;               
      case stNameOfFile: //1:
        NameOfFile += strHead[i2];
        break;
      case stNameOfBaseClass: //2:
        NameOfBaseClass += strHead[i2];
        break;
      case stNameOfBaseClassFinish: //3:
        NameOfBaseClass.TrimLeft();
        NameOfBaseClass.TrimRight();
        if (!NameOfBaseClass.IsEmpty())
        {
          pList->push_back(NameOfBaseClass);//pList->AddTail(NameOfBaseClass);
          NameOfBaseClass.Empty();
        }
        state = stNameOfBaseClass; //2;
        break;
      case stAlies: //4:
        strAlies += strHead[i2];
        break;
      case stAliesFinish: { //5:
        state = stNameOfFile; //1;
        if (strAlies.IsEmpty())
			break;

        CString strPathOfAlies = m_Meta.m_AliasOfPaths[strAlies];
        if (strPathOfAlies.IsEmpty())
        {
			MessageBox(NULL, strAlies, "������: � ini ����� ��� ������ ����������", MB_ICONERROR);
			break;
        }

        NameOfFile += strPathOfAlies;
        if (strPathOfAlies[strPathOfAlies.GetLength()-1] != '\\')
			NameOfFile += '\\';                      
        }  
		break;
      default:
        break;
      } 
    }
//             strResultClassName.TrimLeft();
//             strResultClassName.TrimRight();
//             NameOfFile.TrimLeft();
//             NameOfFile.TrimRight();
    if (strResultClassName.IsEmpty()){
		::MessageBox("������: �� ������ ��� ������ � ������ \n%s", strHead.operator LPCTSTR());
		return false; //continue;
	}

	CString path_fileDescr(RangesDescr.GetNameOfFileDescrOnPos(iNumberOfCurrentString));
	if (!path_fileDescr.IsEmpty())
	{
		char drive[_MAX_DRIVE] = {0};
		char dir[_MAX_DIR] = {0};
		char fname[_MAX_FNAME] = {0};
		char ext[_MAX_EXT] = {0};

		_splitpath(path_fileDescr, drive, dir, fname, ext );
		m_Meta.m_FullFileNameOfDescrFile[strResultClassName] = path_fileDescr;
	}

	if (!path_fileDescr.IsEmpty())
	{
		// ������ ������ ��� ����� ��� �� �������� �� ��� ������������ �������� �������� defcls.prm
		CString sDefFileFolderPath = ::ExtractFileDir(path_fileDescr);
		CSetOfHierarchyImpl::MakeFullFileName(&NameOfFile, sDefFileFolderPath);
	}

	CString strResult;

	if (m_Meta.m_ClassBindFile.Lookup(strResultClassName, strResult))
	{
	  CString error;
	  //error += strResultClassName; error += " ��� ��� ��������� �����";
	  error.Format("����� � ������: %s ��� ��� ��������� �����", strResultClassName.operator LPCTSTR());
	  MessageBox(NULL, error, "������ ����������� ������", MB_ICONERROR);
	  // TODO artbear - ����� ������������ ����� ����� ���������� �������, �.�. ����� ������ ������ ������������ :(
	}
	else{
	  m_Meta.m_ClassBindFile[strResultClassName] = NameOfFile;
	  m_Meta.m_DynamicAddedClassBindFile[strResultClassName] = NameOfFile;
	}

	if ( !pList->empty() ) //if ( !pList->IsEmpty() && !strResultClassName.IsEmpty())
	  m_Meta.m_ClassesHierarchyMap[strResultClassName] = pList;
	
	return true;
}

void CSetOfHierarchyImpl::ProcessMethodDefinition(const CString& NameOfClass, const CString& strMethodsDefinition)
{
  //strMethodsDefinition.TrimRight();
  //strMethodsDefinition.TrimLeft();
  
    int nLen = strMethodsDefinition.GetLength();
    CStringList ListStrMeth;
    CString str;
    bool nQuotes = false;
    bool bIsExistCloseBroken = false;

    for (int pos = 0; pos < nLen; pos++)
    {              
		if (strMethodsDefinition[pos] == '\"' && !nQuotes)
			nQuotes = true;
		else if (strMethodsDefinition[pos] == '\"' && nQuotes)
			nQuotes = false;
      
		if (strMethodsDefinition[pos] == ';' && !nQuotes)
		{                
			str += strMethodsDefinition[pos];
			ListStrMeth.AddTail(str);
			str.Empty();                
		}
		else
			str += strMethodsDefinition[pos];
    }
	
	CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData pMapMeth;
    for(POSITION posList = ListStrMeth.GetHeadPosition(); posList != NULL; )
    {              
      str = ListStrMeth.GetNext( posList );
      str.TrimLeft();
      int nLen = str.GetLength();
      int st = 1;
      int nSemicolon = -1;

      int nPosOpenBracket  = str.Find('('); //("(");              
      bool nQuotes = false;
	  for (int n = nLen - 1; n >= nPosOpenBracket ; --n)
	  {			          
		  if (str[n] == '\"' && !nQuotes)
			  nQuotes = true;
		  else if (str[n] == '\"' && nQuotes)
			  nQuotes = false;
		  
		  if (str[n] == ';' && !nQuotes)
		  {
			  nSemicolon = n;
			  break;
		  }
		  
	  }
	  
      if (nSemicolon == -1)
      {
		MessageBox(NULL, str, "������: � ���������� ������ �� ������� ;", MB_ICONERROR);
        continue;
      }
      int nPosCloseBracket = -1;
      for (int nIter = nLen; nIter != 0; --nIter)
      {
        if (str[nIter-1] == ')')
        {
          nPosCloseBracket = nIter-1;
          break;
        }
      }
      
      if (nPosOpenBracket == -1)
      {
		MessageBox(NULL, str, "������: � ���������� ������ �� ������� (", MB_ICONERROR);
        continue;
      }
      if (nPosCloseBracket == -1)
      {
		MessageBox(NULL, str, "������: � ���������� ������ �� ������� )", MB_ICONERROR);
        continue;
      }

      if (!pMapMeth)
      {
		  pMapMeth = CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData(new CParamDefs::CMapMethodsNameToArrayOfMethodsParamData);
		  m_Meta.m_MapClassNamesToMethodsMap[NameOfClass] = pMapMeth;
      }              

      CString token;
      CString strNameOfMeth;
	  CParamDefs::CArrayOfMethodsParamData_PTR pMethodsMetaInfo(CParamDefs::Create_ArrayOfMethodsParamData() );
      st = 0;

	  bool bIsFunction = false;
	  CString strReturnValueType;
      
	  // ���������� ������ ����� ���������� ������, ��� ������� ��� � ��� ������������
      // ���������.
      bool bIsMethodHaveReturnValue = true;
      for (int pos = 0; pos < nPosOpenBracket; pos++)
      {

        if (str[pos] != ' ' && str[pos] != '\n' && str[pos] != 9)
          token += str[pos];
        else
          st = 1;

        if (st == 1 && !token.IsEmpty() && bIsMethodHaveReturnValue)
        {
          if (FastCompareNoCase.Compare(token,"void") != 0) //if (token.Compare("void") != 0)
          {
			  CMethodsParamData::Ptr pNewData = CMethodsParamData::Create(CString("#return"), token);
			  pMethodsMetaInfo->Add(pNewData);
			  
			  bIsFunction = true;
			  strReturnValueType = token;
          }
          token.Empty();
          st = 0;
          bIsMethodHaveReturnValue = false;
        }
      }
//           if (!token.IsEmpty())
//           {
	  if (token.IsEmpty())
      {
		  MessageBox(NULL, str, "������: � ���������� ������ �� ������� ��������", MB_ICONERROR);
		  continue;
      }
      
        pMapMeth-> operator [](token) = pMethodsMetaInfo;                
        strNameOfMeth = token;
        token.Empty();
//           }
//           else
//           {
//             MessageBox(NULL, str, "������: � ���������� ������ �� ������� ��������", MB_ICONERROR);
//             continue;
//           }
    
      // ���������� ��������� � �������
      CStringList ListStrOfParam;
      nQuotes = false;
      for(int pos = nPosOpenBracket+1; pos < nPosCloseBracket; pos++)
      {
        if (str[pos] == '\"' && !nQuotes)
          nQuotes = true;
        else if (str[pos] == '\"' && nQuotes)
          nQuotes = false;

        if (str[pos] == ',' && !token.IsEmpty() && !nQuotes)
        {
          token.TrimLeft();
          token.TrimRight();
          if (!token.IsEmpty())
          {
            ListStrOfParam.AddTail(token);                    
            token.Empty();
          }
        }
        else
        {
          token += str[pos];
        }
        
      }
      if (!token.IsEmpty())
      {
        token.TrimLeft();
        token.TrimRight();
        ListStrOfParam.AddTail(token);            
        token.Empty();
      }

      bool bExistParamReceivedByValue = false;
      
      bool bExistUnlimitsParams = false;

      for(POSITION posList2 = ListStrOfParam.GetHeadPosition(); posList2 != NULL; )
      {
        CString strParam = ListStrOfParam.GetNext( posList2 );
        RemoveNeedlessBlank(strParam);
        
        int nStrLen = str.GetLength();
        
        if (bExistUnlimitsParams)
          continue;                

        if (!bExistUnlimitsParams && "..." == strParam)                
          bExistUnlimitsParams = true;                                  
        
        int nPosEq = strParam.Find('='); //("=");
        
        if (nPosEq != -1)
        { // ������ �������, ���� ��� ����, ������ ����� �����
          if ((nPosEq+1 < nStrLen)&&strParam[nPosEq+1] == ' ')
            strParam.Delete(nPosEq+1);
          if ((nPosEq-1 >= 0)&&strParam[nPosEq-1] == ' ')
            strParam.Delete(nPosEq-1);
        }                
       
        int nPosSpace = strParam.Find(' '); //(" ");
		if ( -1 ==nPosSpace ){
			nPosEq = strParam.Find('='); //(" ");
			if ( -1 != nPosEq ){
				Msg("��� ������ %s::%s ���������� ������ �������� ������ � prm-�����.", NameOfClass.operator LPCTSTR(), strNameOfMeth.operator LPCTSTR());
				Msg("��������, ��� ��� ��������� �� ��������� �� ����� ��� �������� ��������� - < %s >. ������ �����, ��������� ����� - < �����/������/��������� %s >", strParam.operator LPCTSTR(), strParam.operator LPCTSTR());
			}
			CMethodsParamData::Ptr pNewData = CMethodsParamData::Create(CString(""), strParam);
			pMethodsMetaInfo->Add(pNewData);
		}

        else
        {
          CString masStr[3] = {CString(""),CString(""), CString("")};
          nStrLen = strParam.GetLength();
          bool isVal = false;
          int nPosBlank = 0;
          int Index = 0;
          CString strToken;
          st = 1;
          for (int i = 0; i < nStrLen; i++)
          {
            if (strParam[i] == '\"' && !st)
              st = 1;
            if (strParam[i] == '\"')
              st = 0;
            
            if (strParam[i] == ' ' && st)
            {
              strToken.TrimRight();
              if (!strToken.IsEmpty())
              {                        
                //if ((strToken.CompareNoCase("Val") == 0)||(strToken.CompareNoCase("����") == 0))
				if ((FastCompareNoCase.Compare(strToken, "Val") == 0)||(FastCompareNoCase.Compare(strToken, "����") == 0))
                {                          
                  isVal = true;
				  // TODO ��� - ���� � ������ ���� ���� �� ���� �������� �� ����, ��� ��������� ����� ����� ������������ �� ��������, � �� �� ������ :( !!
                  bExistParamReceivedByValue = true;
                }
                else if (Index < 2)
                {
                  masStr[Index++] = strToken;
                }                        
                strToken.Empty();
              }                        
              continue;
            }
            strToken += strParam[i];
          }
          if (!strToken.IsEmpty())
          {
            masStr[Index++] = strToken;
          }

		  int nPosEq0 = masStr[0].Find('='); //("=");
		  if ( (-1 != nPosEq0) && !masStr[0].IsEmpty() && masStr[1].IsEmpty()){
			Msg("��� ������ %s::%s ���������� ������ �������� ������ � prm-�����.", NameOfClass.operator LPCTSTR(), strNameOfMeth.operator LPCTSTR());
			Msg("��������, ��� ��� ��������� �� ��������� �� ����� ��� �������� ��������� - < %s >. ������ �����, ��������� ����� - < �����/������/��������� %s >", masStr[0].operator LPCTSTR(), masStr[0].operator LPCTSTR());
		  }

          int nPosEq = masStr[1].Find('='); //("=");
          if (nPosEq != -1)
          {
            CString strTemp     = masStr[1].Mid(0, nPosEq);
            CString strParamDef = masStr[1].Mid(nPosEq+1);                    
            strTemp.TrimLeft();
            strTemp.TrimRight();
            strParamDef.TrimLeft();
            strParamDef.TrimRight();
            masStr[1] = strTemp;
            masStr[2] = strParamDef;
          }
		  CMethodsParamData::Ptr pNewData = CMethodsParamData::Create(masStr[1], masStr[0], masStr[2], isVal);
          pMethodsMetaInfo->Add(pNewData);
        }                                                
      }

	  assert(!NameOfClass.IsEmpty());
	  assert(!strNameOfMeth.IsEmpty());
	  assert( ( bIsFunction && !strReturnValueType.IsEmpty() ) || ( !bIsFunction && strReturnValueType.IsEmpty() ) );
      //if ((bExistParamReceivedByValue || bExistUnlimitsParams) && !NameOfClass.IsEmpty() && !strNameOfMeth.IsEmpty())
	  if ( bExistParamReceivedByValue || bExistUnlimitsParams || bIsFunction)
      { 
		CString strKeyOfUserClassMethod = GetKeyOfUserClassMethod(NameOfClass, strNameOfMeth);

        if (bExistParamReceivedByValue)
          // ������� �������� ������ � �������� ������ ��� ��������� ������ ��� �������
          // �������, � ������� ���� ��������� ������������ �� ��������, �� ��� ����������
          // ����������� ����� �������, � ����� ������������.                  
          //m_ValParamInMeth[strKeyOfUserClassMethod] = pMethodsMetaInfo;
			m_Meta.AddMethHaveParVal(strKeyOfUserClassMethod, pMethodsMetaInfo);

        if (bExistUnlimitsParams)
          // ������� �������� ��� ������, ������� �������� �������������� ����������
          // ���������� (...)                  
          m_Meta.m_UnlimitsParams[strKeyOfUserClassMethod] = pMethodsMetaInfo;
	
		if (bIsFunction)
			m_Meta.m_IsFunctionMap[strKeyOfUserClassMethod] = strReturnValueType;
      }                              
    } // for
}

//bool CSetOfHierarchyImpl::VerifyMethodForParamsByValueAndSetMetaInfoClassesDataForParamsByValue( const CString& strKeyOfUserClassMethod, const CBLProcInfo &procInfo)
//{
//	bool bResult = false;
//	
//	const int iParamByValue = 2;
//	int nVars = procInfo.GetNVars();
//	int iMethodNum = procInfo.GetProcPos(); //m_ProcID
//	
//	CCompiledModule* pCompiledModule = procInfo.GetModule()->GetInternalData()->pCompiledModule;
//	CCompileProcInfo* pCompileProcInfo = pCompiledModule->ProcArray[iMethodNum];
//	for (int i = 0; i < nVars;i++)
//	{
//		CBLModuleProcVarDescr* pVar = pCompileProcInfo->VarList[i];
//		if (iParamByValue == pVar->Type){
//			// TODO ��� - ���� � ������ ���� ���� �� ���� �������� �� ����, ��� ��������� ����� ����� ������������ �� ��������, � �� �� ������ :( !!
//			bResult = true; 
//			break;
//		}
//	}
//	
//	if (bResult){
//		//CParamDefs::CArrayOfMethodsParamData_PTR pArr( CParamDefs::Create_ArrayOfMethodsParamData() );
//		CParamDefs::CArrayOfMethodsParamData_PTR pArr;
//		if (GetMetaDataOfClasses().ThisIsUnLimMethod(strKeyOfUserClassMethod)){
//			if (!GetMetaDataOfClasses().LookupUnLimMethod(strKeyOfUserClassMethod, pArr))
//				pArr = CParamDefs::Create_ArrayOfMethodsParamData();
//		}
//		else
//			pArr = CParamDefs::Create_ArrayOfMethodsParamData();
//		
//		pArr->resize(max(pArr->size(), nVars));
//
//		for (int i = 0; i < nVars;i++)
//		{
//			CBLModuleProcVarDescr* pVar = pCompileProcInfo->VarList[i];
//			bool bParamByValue = iParamByValue == pVar->Type;
//			CMethodsParamData::Ptr pNewData = CMethodsParamData::Create(pVar->Name, "", "", bParamByValue);
//			//pArr->Add(pNewData);
//			
//			//pArr->SetAtGrow(i, pNewData);
//			pArr->SetAt(i, pNewData); // TODO ��������, ���-���� SetAtGrow
//		}
//
//		m_Meta.AddMethHaveParVal(strKeyOfUserClassMethod, pArr);
//	}
//	return bResult;
//}