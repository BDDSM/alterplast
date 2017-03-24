// SetOfHierarchy.cpp: implementation of the CSetOfHierarchy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "SetOfHierarchy.h"
#include <locale.h>
#include "istrmap.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CMetaDataCont * pMetaDataCont;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSetOfHierarchy* CSetOfHierarchy::pGlobalHierarchy = NULL;

CSetOfHierarchy::CSetOfHierarchy() : bIsOpenCompForm(false)
{
  //AfxMessageBox("Hello world");
}

void CSetOfHierarchy::Init(const char* IBDir, const char* BinDir)
{  

  pathBase1C = BinDir;
  pathBin1C  = IBDir;
  
  if (IBDir == NULL || BinDir == NULL)
  {
	try
	{
		pathBase1C = ::CallMethodOfGlobalContexts0("IBDir");
		pathBin1C = ::CallMethodOfGlobalContexts0("BinDir");
	}
	catch(...) // todo ��������, ������ � �� �����
	{
	}
  }
  
  if (pathBin1C.IsEmpty())
    RuntimeError("�� ������� �������� �������� BinDir() ������� bin 1C!");

    CFile file;
    CString strFileNameIni = pathBin1C;
    strFileNameIni += "OXY.ini";
    if (file.Open(strFileNameIni, CFile::modeRead | CFile::shareDenyNone))
    {          
      DWORD nLength = file.GetLength();
      file.Close();
      vector<char> pBuf(nLength+2); //char* pBuf = new char[nLength+2];
      int nSize = 0;
      nSize = GetPrivateProfileSection("alias_path", &pBuf[0], nLength+2, strFileNameIni);
      if (nSize > 0)
      {
        CString str;
        for (int i = 0; i < nSize; i++)
        {
          
          if (pBuf[i] == '\0')
          {         
            int nPosEq = str.Find("=");
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

CSetOfHierarchy::~CSetOfHierarchy()
{
}

CSetOfHierarchy* CSetOfHierarchy::GetHierarchy()
{
  if (pGlobalHierarchy == NULL)
    pGlobalHierarchy = new CSetOfHierarchy;
  return pGlobalHierarchy;
}

void CSetOfHierarchy::DeInitHierarchy()
{
  if (pGlobalHierarchy)
  {
    CString name;
    CString NameFile;
    //CFile file;  
    for (POSITION pos = pGlobalHierarchy->m_Meta.m_ClassBindFile.GetStartPosition(); pos != NULL;)
    {
      pGlobalHierarchy->m_Meta.m_ClassBindFile.GetNextAssoc( pos, name, NameFile);
      
      CSetOfHierarchy::MakeFullFileName(&NameFile);
      
      //if (file.Open(NameFile, CFile::modeRead | CFile::shareDenyNone, NULL))
	  if (FileExists(NameFile))
      {
        CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CComponentClass));      
        //file.Close();
      }    
    }    
    delete pGlobalHierarchy;
  }
}

// artbear ��� �������� ��������
// 1 - ��������, 0 - ���
bool CSetOfHierarchy::VerifyRecursiveInBaseClasses(CString ClassName, CString BaseClassName, CString& sRecursiveHierarchy)
{
	return m_Meta.VerifyRecursiveInBaseClasses(ClassName, BaseClassName, sRecursiveHierarchy);
}


// ��������� ����������� ���� ����������� 1� ���������� �� ����� ����������
void CSetOfHierarchy::RegistersContextClasses()
{
	CIStringMapToString* mapClassBindFile = &m_Meta.m_ClassBindFile;
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
      CMetaDataObj* pObj=(CMetaDataObj*)pMD->GetCalcVarDef(nameRep);  
      
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
      CSetOfHierarchy::MakeFullFileName(&NameFile);    
	  
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

CSetOfHierarchy::CParamDefValueEnum CSetOfHierarchy::GetParamDefValue(int iMethodNum, int iParamNum, CValue* pDefValue,
                                      const CComponentClass* pComponentClass)
{
  if (pComponentClass == NULL || pDefValue == NULL)
	return pdve_DefValueNotExists;  

  int nRealParamNum = iParamNum;
  if (pComponentClass->HasRetVal(iMethodNum) == 1)  
	iParamNum++; // ��� ���������� ������ ��� � ������� ������ ���� ������������ ��������  

	CParamDefs::CArrayOfMethodsParamData_PTR param = GetMethodParamData(pComponentClass, iMethodNum);
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
    if (!strType.CompareNoCase("�����") || !strType.CompareNoCase("Number"))
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
    else if (!strType.CompareNoCase("����") || !strType.CompareNoCase("Date"))
    {
      int year, month, day;
      year = 0; month = 0; day = 0;          
      if (strVal != "\"\"")
      {
        if (strVal.Find("'") == -1)
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
    else if (!strType.CompareNoCase("������") || !strType.CompareNoCase("String"))
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
CParamDefs::CArrayOfMethodsParamData_PTR CSetOfHierarchy::GetMethodParamData(const CComponentClass* pComponentClass, int iMethodNum) const
{
	CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData pMapFunc = pComponentClass->GetDefFnNamesPtr()->GetPtrOfMapMethodsNameToArrayOfMethodsParamData();

	if (!pMapFunc)
		return CParamDefs::CArrayOfMethodsParamData_PTR(); // NULL

	CParamDefs::CArrayOfMethodsParamData_PTR param = (*pComponentClass->GetDefFnNamesPtr())[iMethodNum].m_pArrayOfMethodsParamData;
	if (!param)
	{
		CString NameMeth = pComponentClass->GetMethodName(iMethodNum, 1);
		pMapFunc->Lookup(NameMeth, param);
	}

	return param;
}

// TODO ���� ����� ���� ������ ������������ ������ CComponentClass - artbear
bool CSetOfHierarchy::CheckTypeOfParam (CComponentClass* pComponentClass, int nNumMeth,
                                        CValue** ppParamOfMeth, CBLProcInfo* pProcInfo, 
                                        CValue* pRetValue)
{  
	bool result = true;
	int num = pComponentClass->GetNParams(nNumMeth);
  
	class CLocalGuard
	{
		vector<int>& m_VecNumOfParForPassCheckType;
	public:
		CLocalGuard(vector<int>& VecNumOfParForPassCheckType) : 
			m_VecNumOfParForPassCheckType(VecNumOfParForPassCheckType) {};
		~CLocalGuard() { m_VecNumOfParForPassCheckType.clear(); };
	};
	CLocalGuard guard(m_VecNumOfParForPassCheckType);

  if (NULL == pRetValue && num <= 0)
	return result;
	
	CParamDefs::CArrayOfMethodsParamData_PTR param = GetMethodParamData(pComponentClass, nNumMeth);

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
      
      CMethodsParamData::Ptr NameBind = param->operator[](i);
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
        
        if (find(m_VecNumOfParForPassCheckType.begin(), m_VecNumOfParForPassCheckType.end(), nNumParam) != m_VecNumOfParForPassCheckType.end())
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
            pName += ".";
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
      if (!pName.CompareNoCase("������"))
      {
        strNameEng = "String";
      }
      else if (!pName.CompareNoCase("����"))
      {
        strNameEng = "Date";
      }
      else if (!pName.CompareNoCase("�����"))
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
        
        if (TheClassIsDerive(pName, NameBind->GetType()))
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

bool CSetOfHierarchy::TheClassIsDerive(const CString& derive, const CString& base) const
{
	return m_Meta.TheClassIsDerive(derive, base);
}

bool CSetOfHierarchy::IsObject(const CString& sClassName, const CString& sTryName) const
{
	return m_Meta.IsObject(sClassName, sTryName);
}  

CString CSetOfHierarchy::GetNameOfFile(const CString &strNameOfMod)
{
  return m_Meta.GetNameOfFile(strNameOfMod);
}

void CSetOfHierarchy::RemoveNeedlessBlank(CString &str)
{
  CMetaDataOfClasses::RemoveNeedlessBlank(str);
}

void CSetOfHierarchy::PrintError(const char *cstrError, const char *cstrFileName, int nNumLine)
{
  CString strTextMess;
  strTextMess.Format("%s (%d):%s",cstrFileName,nNumLine,cstrError);
  pBkEndUI->DoMessageLine(strTextMess, mmBlackErr);	
}

CString CSetOfHierarchy::GetPathFromAlias(const CString& strAls)
{
	return m_Meta.GetPathFromAlias(strAls);
}

void CSetOfHierarchy::ReadDefinesAndCompile(CString &str, const CString &strNameFile)
{
  try 
  {
    CRangesOfDescr Ranges;
    CPreprocessor::Preprocess(false, str, m_SymbolOfPreprocessor, m_LoadedIncludeFiles, strNameFile, &Ranges);
    m_Meta.ParsingString(str, Ranges);        
  }
  catch (CPreprocessor::ExeptionPreprocessor &ex)
  {
	ShowMsg("� ������ �����: %s", mmBlackErr, strNameFile);	
    ShowMsg(ex.GetErrorString(), mmBlackErr);	
    if (!ex.GetCodeString().IsEmpty())
	if(!m_Meta.m_bInsideDynamicAdd)
		ShowMsg("(%d) %s", mmBlackErr, ex.GetNumString(), ex.GetCodeString());
	else
		RuntimeError("(%d) %s", mmBlackErr, ex.GetNumString(), ex.GetCodeString());
  }
}

void CSetOfHierarchy::ProcessDefines()
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
      pBkEndUI->DoMessageLine("�������� �� ������� ������ ��� �������� ������ � ������� CSetOfHierarchy::CSetOfHierarchy()", mmExclamation);	
    }
    END_CATCH
  }
}



bool CSetOfHierarchy::ThisIsUnLimMethod(const CString& strName)
{
	return m_Meta.ThisIsUnLimMethod(strName);
}

bool CSetOfHierarchy::isMethHaveParVal(const CString& strName)
{
	return m_Meta.isMethHaveParVal(strName);
}

// TODO ����� ����������� ���� ������� !!
void CSetOfHierarchy::InitParamDefsClass(CParamDefs *par, const CString& strClass)
{  
  if (!m_Meta.m_MapClassNamesToMethodsMap.Lookup(strClass, par->GetPtrOfMapMethodsNameToArrayOfMethodsParamData()))
    par->CleanPtrOfMapMethodsNameToArrayOfMethodsParamData(); //par->pMapFunc = NULL;
}

bool CSetOfHierarchy::IsClassExist(const char *pcstrName)
{
	return m_Meta.IsClassExist(pcstrName);
}

int CSetOfHierarchy::GetCountClasses() const
{
	return m_Meta.GetCountClasses();
}

int CSetOfHierarchy::GetCountMethods() const
{
	return m_Meta.GetCountMethods();
}

CString CSetOfHierarchy::GetLoadIncludeFiles(const char* sep) const
{
  CString key;
  POSITION pos;
  CString res;  
  for (pos = m_LoadedIncludeFiles.GetStartPosition(); pos != NULL;)
  {
    CString val;    
    m_LoadedIncludeFiles.GetNextAssoc( pos, key, val);    
    res += key;
    res += sep;        
  }  
  return res;
}

CString CSetOfHierarchy::GetDefSymbolOfPreproc(const char* sep) const
{
  CString key;
  POSITION pos;
  CString res;  
  for (pos = m_SymbolOfPreprocessor.GetStartPosition(); pos != NULL;)
  {
    void *ptr;    
    m_SymbolOfPreprocessor.GetNextAssoc( pos, key, (void*&)ptr);
    res += key;
    res += sep;
  }  
  return res;
  
}

CString CSetOfHierarchy::GetAllClassesNames(const char* sep) const
{
	return m_Meta.GetAllClassesNames(sep);
}

CString CSetOfHierarchy::GetAllAliasAndPath(const char* sep) const
{
	return m_Meta.GetAllAliasAndPath(sep);
}

void CSetOfHierarchy::LoadListNameOfIncludeFiles(CStringArray &arr)
{
  CString key;
  POSITION pos;  
  for (pos = m_LoadedIncludeFiles.GetStartPosition(); pos != NULL;)
  {
    CString val;    
    m_LoadedIncludeFiles.GetNextAssoc( pos, key, val);
    arr.Add(key);
  }    
}

CString CSetOfHierarchy::GetBaseClasses(const char* strNameCl, const char* sep) const
{
	return m_Meta.GetBaseClasses(strNameCl, sep);
}

CString CSetOfHierarchy::GetNamesMethOfClass(const char *pNameClass, const char *sep) const
{
	return m_Meta.GetNamesMethOfClass(pNameClass, sep);
}

CParamDefs::CArrayOfMethodsParamData_PTR CSetOfHierarchy::GetInfoParamMeth(const char *pNameClass, const char *pNameMeth) const
{  
	return m_Meta.GetInfoParamMeth(pNameClass, pNameMeth);
}

bool CSetOfHierarchy::IsMethExist(const char *pNameClass, const char *pNameMeth) const
{
	return m_Meta.IsMethExist(pNameClass, pNameMeth);
}

// �������� ������ ���� ����� ��� �� �������� �� 
// ���, ���� ���� �� ������ � �������� ��, ���� ������������ ����������� ��������
// ���� � ����� ���� �� ������, ������������ ���� ������������ �������� ��
//
void CSetOfHierarchy::MakeFullFileName(CString* name, LPCSTR sSecondPath)
{
//Msg("%s %s", "name", (LPCTSTR) *name); 
  name->Replace("\"", ""); // artbear ����� �������

  if (name->Find(":") == -1)
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

// ����� �������� ������ ��� ��������������(���) �������� �� ��� ������������ ������� � �������������(����������),
// � ������� �� ����� �������� - artbear
// ������������ � CComponentClass::InitObject - �.�. ����� � 1� ����� �������������(����������)
//
CString CSetOfHierarchy::GetClassNameFromDefinition(const CString& ClassName) const
{
	return m_Meta.GetClassNameFromDefinition(ClassName);
}

void CSetOfHierarchy::LoadNewClasssesDefine(CString &str, const CString &strNameFile)
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
