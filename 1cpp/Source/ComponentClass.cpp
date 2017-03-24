// ComponentClass.cpp: implementation of the CComponentClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "ComponentClass.h"

#include "System\GroupContextWrap.h"

extern CMetaDataCont * pMetaDataCont;

class CObjID CComponentClass::ObjID;

class CMapDefMethod CComponentClass::MapDef;

CComponentClass* CComponentClass::pCreatedCompModule = NULL;
bool CComponentClass::bFlagCreateWithoutConstructor = false;

  // ����� � ����������� �� ������ � �� ��������� �����������
  // ���� - ��������� �� ������, �������� - ��������� �� �������� CComponentClass
CMapPtrToPtr CComponentClass::m_MapOfModulesAndComp;

IMPLEMENT_DYNCREATE(CComponentClass, CBLContext)

class CDynamicPropertyManager
{
public:
// 	CDynamicPropertyManager();
// 	~CDynamicPropertyManager();

	BOOL IsEmpty() const { return Size() == 0;};
	int Size() const { return m_MapPropertyNumbers.GetCount(); };

	void Add(LPCTSTR sNewProperty, int iNewPropertyNumber);

	int FindNumberByName(LPCTSTR sProperty) const;
	LPCTSTR FindNameByNumber(const int iPropertyNumber) const;

	BOOL IsExist(const int iPropertyNumber) { return !FindNameByNumber(iPropertyNumber); };
protected:

private:
	CIStringMapToInt m_MapPropertyNumbers;
	vector<LPCTSTR> m_MapPropertyNames;
};

void CDynamicPropertyManager::Add(LPCTSTR sNewProperty, int iNewPropertyNumber)
{
	int iPropNumber = -1;
	if (m_MapPropertyNumbers.Lookup(sNewProperty, iPropNumber))
		RuntimeError("������������ �������� %s ��� ������ �����, ������ ��� �������� ������");

	//���������
	const int size = m_MapPropertyNames.size();
	if (iNewPropertyNumber < size)
		RuntimeError("���::����������������������������: ������ ������");

	m_MapPropertyNumbers.SetAt(sNewProperty, iNewPropertyNumber);
	LPCTSTR key;
	m_MapPropertyNumbers.LookupKey(sNewProperty, key);
	m_MapPropertyNames.resize(iNewPropertyNumber+1);
	m_MapPropertyNames[iNewPropertyNumber] =  key;
}
int CDynamicPropertyManager::FindNumberByName(LPCTSTR sProperty) const
{
	int iPropNumber = -1;
	m_MapPropertyNumbers.Lookup(sProperty, iPropNumber);
	return iPropNumber;
}

LPCTSTR CDynamicPropertyManager::FindNameByNumber(const int iPropertyNumber) const
{
	const int size = m_MapPropertyNames.size();
	if (size <= iPropertyNumber) return NULL;
	return m_MapPropertyNames[iPropertyNumber];
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//CComponentClass::CComponentClass() : CBLContext(1), m_pMod(NULL), m_pCurModule(),
CComponentClass::CComponentClass() : m_pMod(NULL), m_pCurModule(),
									defFnNames(NULL), m_bDonNotCallDestructor(false), m_pDynamicPropertyManager(new CDynamicPropertyManager)
{
  //AFX_MANAGE_STATE(AfxGetStaticModuleState());

  //m_pMod = NULL;
  //defFnNames = NULL;
  m_pCurModule = CBLModule::GetExecutedModule();
  //CSafeModulePtr pSafeMod(pCurModule);

  m_MethParamsDef.nNumberOfMeth = -1;
  m_MethParamsDef.nRealNumberOfMeth = -1;
  m_MethParamsDef.pContext = NULL;
  m_isCreateOptimizeMode = pMainAddIn->m_EnableOptimizate==0 ? false : true;

  if (!pImplicitParams)
	pImplicitParams = new CImplicitParams; // TODO ������ �����, ����� ����� ���� ����������� ������

}

CComponentClass::~CComponentClass()
{
	m_DebugInfo.Release();
//LogErr("~CComponentClass %s pCont = %d", GetTypeString(), this);
  POSITION pos;
  CString pkey;
  InfoUnlimitsParams *pInfo = NULL;

  for( pos = m_BindInstaceValueList.GetStartPosition(); pos != NULL; )
  {
    m_BindInstaceValueList.GetNextAssoc( pos, pkey, (void*&)pInfo );
    pInfo->cont_ValueList->DecrRef(); // TODO ����� �������� � ���������� ~InfoUnlimitsParams :)
    delete pInfo; // TODO ����� ��������� �� �����-������� :)
  }


  if (m_pMod)
  {
	//artbear �����������
	m_MapOfModulesAndComp.RemoveKey(m_pMod);

    if (!m_DebugInfo)
    {
      if (!pMainAddIn->m_EnableOptimizate && MapDef.GetModule(m_NameOfInstance).isValid())
        MapDef.DelModule(m_NameOfInstance);

	  //UnlinkContext();
//      delete m_pMod;
	  m_pMod = NULL;

    }

  }

  if(!pMainAddIn->m_EnableOptimizate && defFnNames.isValid())
  {
    if (MapDef.GetDefs(m_NameOfInstance).isValid())
    {
      MapDef.DelDefParam(m_NameOfInstance);
    }
    else
      if (!m_isCreateOptimizeMode)
        ; //delete defFnNames; // artbear
  }

  //delete pImplicitParams; // TODO ������, ����� �� ����� ������� ?
}

CSafeContextPtr<CBLContext> CComponentClass::FindBaseContextByMethodNumber(const int iMethodNum, int& iMethNumInSearchedContext) const
{
	iMethNumInSearchedContext = -1;
 	if (!IsMethodFromBaseContexts(iMethodNum)) // TODO ���. ��������, ������ �����, ����� !
 		return CSafeContextPtr<CBLContext>();

	int iMet = defFnNames->Size();

    CONST_ITER_CONT iEnd(vecOfBaseCont.end());
	for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
    {
      CSafeContextPtr<CBLContext> result = *iter;

      int nNMethods = result->GetNMethods();

      if (iMethodNum < iMet+nNMethods)
      {
		iMethNumInSearchedContext = iMethodNum - iMet;
        return result;
      }
      iMet += nNMethods;
    }

	return CSafeContextPtr<CBLContext>();
}

int CComponentClass::CallAsMethod(int iMethodNum, CValue **ppValue, bool bHasRetVal, CValue *pRetValue)
{
  int res = 0;
//  const char* szMethodName = GetMethodName(iMethNum, 0);
//  const char* szMethodName1 = GetMethodName(iMethNum, 1);
//LogErr("%s::CallAsMethod (%d) <%s, %s>", GetTypeString(), iMethNum, szMethodName, szMethodName1);

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

  if (IsMethodFromBaseContexts(iMethodNum))
  {
        int nMeth;
	  	CSafeContextPtr<CBLContext> pCont = FindBaseContextByMethodNumber(iMethodNum, nMeth);
		if (pCont)
			if (bHasRetVal)
			  res = pCont->CallAsFunc(nMeth, *pRetValue, ppValue);
			else
			  res = pCont->CallAsProc(nMeth, ppValue);
		return res;
  }

    if (!m_pMod)
		return 0;

	const CParamDefs::paramdefs& MethodParam = (*defFnNames)[iMethodNum];
      int iProc = MethodParam.nProcPos;

      CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();
      assert(pHierarchy!=NULL);

      if (pMainAddIn->m_EnabledCheckType)
      {
        // �������� ������� ��������� ������
        CBLProcInfo ProcInfo;
        m_pMod->GetProcInfo(iProc, ProcInfo);
        CString strTemp = pImplicitParams->m_strClassAndMethCurrent;
        pHierarchy->CheckTypeOfParam(this, iMethodNum, ppValue, &ProcInfo);
        pImplicitParams->m_strClassAndMethCurrent = strTemp;
      }


      bool bResOfImpParam = false;

      // ������ � ����������� ����������� �� �������� � ����� ��� ������������ ��������������
      ARRAY_VAL arrParamValOfMeth; //CArray<CValue*, CValue*> arrParamValOfMeth;

      CString strClassAndMeth;
      bool isUnLimParMethod = MethodParam.m_bIsMethodHaveUnlimitedCountOfParams;

      if (iMethodNum >= lastMethod)
      {
        if (isUnLimParMethod)
        {
          bResOfImpParam = pImplicitParams->GetImplicitParams(this, ppValue, iMethodNum);
          if (bResOfImpParam)
            strClassAndMeth = pImplicitParams->m_strClassAndMethImplicit;
        }
		// TODO ��� ������������� ���������� � CallAsFunc ��� CallAsProc �� ����� ��������������

        if (MethodParam.m_bIsMethodHaveParamsReceivedByValue)
          pImplicitParams->SaveValParam(this, iMethodNum, ppValue, &arrParamValOfMeth);
      }
      int nCountParams = pImplicitParams->m_CountParametersInCallMethod;

      if (bHasRetVal)
      {
        CValue locVal;
		res = m_pMod->CallAsFunc(iProc, locVal, MethodParam.NumberOfParams, ppValue);
        *pRetValue = locVal;
      }
      else
      {
        res = m_pMod->CallAsProc(iProc, MethodParam.NumberOfParams, ppValue);
      }

      pImplicitParams->m_CountParametersInCallMethod = nCountParams;

      if (bResOfImpParam)
        pImplicitParams->m_strClassAndMethImplicit = strClassAndMeth;

      if (MethodParam.m_bIsMethodHaveParamsReceivedByValue)
        pImplicitParams->RestoreValParam(this, iMethodNum, ppValue, &arrParamValOfMeth);

	  CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

      if (bHasRetVal && pMainAddIn && pMainAddIn->m_EnabledCheckType && res == 1)
      {
        // ����� �������� ������������ ��� ������
        CBLProcInfo ProcInfo;
        m_pMod->GetProcInfo(iProc, ProcInfo);
        pHierarchy->CheckTypeOfParam(this, iMethodNum, ppValue, &ProcInfo, pRetValue);
      }

	return res;
}

// artbear
void CComponentClass::GetFullTypeName(CBLContext *pCont, CString& rus, CString& eng,
		CString& rusKind, bool bAddKind)
{

  const CString strClassName(pCont->GetRuntimeClass()->m_lpszClassName);
  rus.Empty();
  eng.Empty();
  rusKind.Empty();
//LogErr("�� ��� ������ strClassName %s", strClassName);

  rus = pCont->GetTypeString();
//LogErr("�� ��� ������ rus %s", rus);

  //if (rus.IsEmpty() || strClassName == "CRegActContext" || strClassName == "CAllRegsContext")
  if (rus.IsEmpty() || !rus.CompareNoCase("�����������������") || strClassName == "CRegActContext" || strClassName == "CAllRegsContext")
  {
    // ����� ���������� ������� 1� ������ ����� ��� "����������" � "��������"
    const int meth = pCont->FindMethod("Kind");
    if (meth != -1) // ����� ����� ������
    {
      //�������� ������ �����
      CValue value;
      CValue ValueParam;
      CValue *pMassVal[1] = {&ValueParam};

      pCont->CallAsFunc(meth, value, pMassVal);

	  int meth = pCont->FindMethod("���������������");
	  bool bIsDoc = -1 != meth;

      //if (strClassName == "CDocContext")
      if (strClassName == "CDocContext" || bIsDoc)
      {
        rus = "��������";
        eng = "Document";
      }
      else if (strClassName == "CSubcContext")
      {
        rus = "����������";
        eng = "Reference";
      }
      else if (strClassName == "CRegActContext")
      {
        rus = "�������";
        eng = "Register";
      }

      CString sKind = value.GetString();
      if (!sKind.IsEmpty() && bAddKind)
		  rusKind = sKind;
    }
    else
    {
      if (strClassName == "CAllRegsContext")
      {
        rus = "��������";
        eng = "Registers";
      }
    }
  }
  else
  {
    eng = rus;
  }
  // ������� ��� ���������� ���������
	{
		CGetDoc7* pDoc = ::GetDoc7(pCont);
		if (pDoc)
		{
			CTypedCont *pContainer = pDoc->m_pTypedCont; // artbear

			CString strObjectType = "";
			CString strKind = "";
			if (pContainer) {
				CString csType;
				long lMetaID = CConfigCont::DocumentToID (pContainer, csType);

				CString str = "�����������������";
				if (lMetaID > 0)
				{
					str = GetModuleFullName (lMetaID, csType, 0, pMetaDataCont);

					int n = str.Find(".");
					if(n >= 0)
					{
						strKind = str.Mid(n+1);
						str = str.Left(n);
						int n = strKind.Find(".");
						if(n >= 0)
						{
							strKind = strKind.Left(n);
						}
					}
				}
				strObjectType = str;
			} else
				strObjectType = "������������";

			rus = strObjectType;
			eng = "";
			// rusKind = strKind;
		    if (!strKind.IsEmpty() && bAddKind)
				rusKind = strKind;

		}
	}
  if (rus.IsEmpty())
    RuntimeError("��� ������: %s ��� �������� �� ������� ������������ ��� � ������ CComponentClass::GetFullTypeName! ���������� �� ������������� � ������������.", strClassName);
}

void CComponentClass::GetFullTypeName(CBLContext *pCont, CString& rus, CString& eng, bool bAddKind)
{
	CString rusKind;
	GetFullTypeName(pCont, rus, eng, rusKind,bAddKind);
	if (!rusKind.IsEmpty())
	{
		rus += "."+rusKind;
		eng += "."+rusKind;
	}
}

int CComponentClass::GetBaseClass(const CValue &NameClass, CValue &rValue) const
{
  rValue = CNumeric(0);

  CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();
  assert(pHierarchy!=NULL);

  if (NameClass.IsEmpty())
  {
	CComponentClass* pCompClass = VerifyCurrentExecuteContextIsBaseClassForThisClass();
    if (pCompClass)
      return pCompClass->GetBaseClass(NameClass, rValue);

	if (!vecOfBaseCont.empty())
    {
        CBLContext *p = *vecOfBaseCont.begin();
        rValue.AssignContext(p);
    }
    else
        rValue = CNumeric(0);
    return 1;
  }

    if (!m_NameOfInstance.CompareNoCase(NameClass.GetString()))
    {
      rValue.AssignContext(const_cast<CComponentClass*>(this));
      return 1;
    }

	  CComponentClass* pCompClass = VerifyCurrentExecuteContextIsBaseClassForThisClass();
      if (pCompClass)
      {
        if (pCompClass->GetBaseClass(NameClass, rValue) == 1)
          return 1;
      }
      else
      {
        for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter)
        {
          CBLContext *pCBLContext = *iter;
          CString strRus;
          CString strEng;
          GetFullTypeName(pCBLContext, strRus, strEng);

          if (!strRus.CompareNoCase(NameClass.GetString()) || !strEng.CompareNoCase(NameClass.GetString()))
          {
            rValue.AssignContext(pCBLContext);
            return 1;
          }

		  // artbear ���� � ���� ��� ����������.��������������, � ����������� ����������
// 		  CString strRus;
// 		  CString strEng;
		  GetFullTypeName(pCBLContext, strRus, strEng, false);
		  if (!strRus.CompareNoCase(NameClass.GetString()) || (!strEng.IsEmpty() && !strEng.CompareNoCase(NameClass.GetString())))
		  {
			rValue.AssignContext(pCBLContext);
			return 1;
		  }
		  else if (IS_KINDOF_CGroupContext(pCBLContext) && !NameClass.GetString().CompareNoCase("�����������������"))
		  {
			rValue.AssignContext(pCBLContext);
			return 1;
		  }

		  else if(pCBLContext->GetRuntimeClass()==&classCComponentClass)
		  {
			CValue* params[1] = {const_cast<CValue*>(&NameClass)};

			if (pCBLContext->CallAsFunc(funcGetBaseClass, rValue, params) == 1)
			  return 1;
		  }
        }
      }

	return 0;
}

int CComponentClass::ReplaceInstBaseClasses(const CValue &NameClass, const CValue &Instance, CValue &rValue)
{
  int res = 0;
  rValue = CNumeric(0);
  if (!Instance.GetContext())
    RuntimeError("�������� ��������� �������� ������ ����� ������ �������� ��������� �������� ������������� ��� ���������� ������!",0);

  if (!NameClass.GetString().GetLength())
    RuntimeError("������ ���������� ������ ���� �� ������ ������!");

  {
    CString strNameClass(NameClass.GetString());
    CString strRusInstance;
    CString strEngInstance;
    CString strRusInstanceKind;
//LogErr("��� ������ strNameClass %s", strNameClass);
	// ��������� �������� true - ����� ��������� ����������� �������� ������������
	// �� �������������� "����������"
	// �������� ���� http://cvs.alterplast.ru/bugs/show_bug.cgi?id=2513
    GetFullTypeName(Instance.GetContext(), strRusInstance, strEngInstance,
		strRusInstanceKind);
	bool bInstanceIsGroupContext = IS_KINDOF_CGroupContext(Instance.GetContext());

    CComponentClass* pCompClass = VerifyCurrentExecuteContextIsBaseClassForThisClass(); // TODO

    if (!m_NameOfInstance.CompareNoCase(NameClass.GetString()))
	{
    // artbear ���� ���� �� ����� ��������, ���� ������ ������������ ������ ���� �� ����
    // ����� ����� ����� ������ ��� ���� �������.���������������������("������", ������������������);
	// ���� "����_���������������������_���������������" - ����� �����-����� ������ ������ � ������ ������� 1�++ :( !!
		if (!pCompClass)
			return 0;
        CString strRus;
        CString strEng;
        CString strRusKind;

        GetFullTypeName(this, strRus, strEng, strRusKind);
		bool bNeedReplace = false;
		if (strRusKind.IsEmpty())
		{
			bNeedReplace = !strRus.CompareNoCase(strRusInstance) ||
				!strEng.CompareNoCase(strEngInstance);
		}
		else
		{
			bNeedReplace = !strRusKind.CompareNoCase(strRusInstanceKind) &&
				(!strRus.CompareNoCase(strRusInstance) || (!strEng.IsEmpty() && !strEng.CompareNoCase(strEngInstance)));
		}

		if (bNeedReplace)
			return 1;
		else
			return 0;
	}
	else
    if (pCompClass)
    {
      if (pCompClass->ReplaceInstBaseClasses(NameClass, Instance, rValue) == 1)
      {
        res = 1;
		return res;
      }
    }
    else
    {
      for (ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter)
      {
        CBLContext* pCBLContext = *iter;
        CString strRus;
        CString strEng;
        CString strRusKind;
//LogErr("������� ����");

        GetFullTypeName(pCBLContext, strRus, strEng, strRusKind);

		// �������� ���� http://cvs.alterplast.ru/bugs/show_bug.cgi?id=2513

		bool bNeedReplace = false;
		if (strRusKind.IsEmpty())
		{
			bNeedReplace = !strRus.CompareNoCase(strNameClass) ||
				!strEng.CompareNoCase(strNameClass); // TODO
			if (bNeedReplace)
			{
				bNeedReplace = CSetOfHierarchy::GetHierarchy()->IsObject(strRus, strRusInstance) ||
					CSetOfHierarchy::GetHierarchy()->IsObject(strEng, strEngInstance);
			}
		}
		else
		{
			bNeedReplace = !(strRus+"."+strRusKind).CompareNoCase(strNameClass) ||
				!(strEng+"."+strRusKind).CompareNoCase(strNameClass);
			if (bNeedReplace)
				bNeedReplace = !strRusKind.CompareNoCase(strRusInstanceKind) &&
					(!strRus.CompareNoCase(strRusInstance) || (!strEng.IsEmpty() && !strEng.CompareNoCase(strEngInstance)));
		}
		if (!bNeedReplace && bInstanceIsGroupContext)
		{
			if (IS_KINDOF_CGroupContext(pCBLContext)
				&& !strNameClass.CompareNoCase("�����������������") && !strRus.CompareNoCase("�����������������"))
				bNeedReplace = true;
			else
				if (strRusKind.IsEmpty())
				{
					bNeedReplace = !strRus.CompareNoCase(strNameClass) ||
						!strEng.CompareNoCase(strNameClass);
				}
				else
				{
					bNeedReplace = !(strRus+"."+strRusKind).CompareNoCase(strNameClass) ||
						!(strEng+"."+strRusKind).CompareNoCase(strNameClass);
					if (bNeedReplace)
						bNeedReplace = !strRusKind.CompareNoCase(strRusInstanceKind) &&
							(!strRus.CompareNoCase(strRusInstance) || (!strEng.IsEmpty() && !strEng.CompareNoCase(strEngInstance)));
				}
		}

		if (bNeedReplace)
        {
			{
				*iter = Instance.GetContext();
			}

			rValue = CNumeric(1);
			res = 1;
			return res;
			break;
        }
        else if(pCBLContext->GetRuntimeClass()==&classCComponentClass)
        {
          CValue* params[2] = {const_cast<CValue*>(&NameClass), const_cast<CValue*>(&Instance)};
          if (pCBLContext->CallAsFunc(funcReplaceInstBaseClasses, rValue, params) == 1)
          {
            rValue = CNumeric(1);
            res = 1;
			return res;
            break;
          }
        }
      }
    }
  }
  return res;
}

int CComponentClass::IsObject(const CValue &NameClass, CValue &rValue) const
{

	if (CSetOfHierarchy::GetHierarchy()->IsObject(m_NameOfInstance, NameClass.GetString()))
		rValue = CNumeric(1);
	else
		rValue = CNumeric(0);

	return 1;
}

int  CComponentClass::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{
  int res = 0;

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

  switch(iMethNum)
	{


    case funcGetBaseClass: // ��������������������(<�������� ������>) : ������� ������ �� ������� �����
    {
      res = GetBaseClass(*ppValue[0], rValue);
    }
    break;


    case funcReplaceInstBaseClasses:
    case funcReplaceInstBaseClasses_SYN1:
    {
      res = ReplaceInstBaseClasses(*ppValue[0], *ppValue[1], rValue);
    }
    break;

	case funcIsObject: // ����������������(<�������� �������>) : ���������� 1 ��� 0
	{
	  res = IsObject(*ppValue[0], rValue);
	}
	break;

    case funcSendMessageOwnMod:
    {
      if (m_pCurModule)
      {
        int iProc = m_pCurModule->FindProc("������������������������", 1);
        if (iProc != -1)
        {
			CValue retValue; // ���� � ������� ���������� �������� rValue, ����� ��� � ������������ ���������
			res = m_pCurModule->CallAsFunc(iProc, retValue, (*defFnNames)[iMethNum].NumberOfParams, ppValue);
			rValue = retValue;
        }
        else
          RuntimeError("�� ������� ������� ��������� ������ ������������������������()", mmBlackErr); // ������� ���������� artbear
      }
      break;
    }
    case funcGetPathName:
      {
        CString str = CSetOfHierarchy::GetHierarchy()->GetNameOfFile(m_NameOfInstance);
        CSetOfHierarchy::MakeFullFileName(&str);
        rValue = str;
        res = 1;
      }
      break;
    case funcGetEnvContext:
      {
			CBLContext *pCont  = CBLModuleWrapper::GetContextFromModule(m_pCurModule);
			if (pCont)
			  rValue.AssignContext(pCont);
			else
			  rValue = CNumeric(0);
			res = 1;
      }
      break;
    case funcGetParamsList:
      {
        CBLContext *pContOfList = pImplicitParams->GetImplicitParamsList(this, ppValue[0]->GetString());
        if (pContOfList)
          rValue.AssignContext(pContOfList);
        else
          RuntimeError("������ �������� ������� GetParamsList � ������ � �������������� �������� ����������� (...)!");
        res = 1;
      }
      break;
    case funcSetOnIndexParams:
      {
        res = pImplicitParams->SetImplicitParamOfInd(this, ppValue[0]->GetString(), ppValue[1]->GetNumeric().operator long(), ppValue[2]);
        rValue = CNumeric(res);
      }
      break;

	default:
	  res = CallAsMethod(iMethNum, ppValue, true, &rValue);
	};

	return res;
}

int  CComponentClass::CallAsProc(int iMethNum,class CValue * * ppValue)
{

  int res = 0;

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

  switch(iMethNum)
	{

/*// ���� ���������� ������ ����� �������� ������������� 1�++ - artbear
// TODO ��������, � ���������� �� ������ ���� ������ ������ ������, � ������� �������
//
  case procAddBaseClass:
    {
    }
    break;*/
  case procOnOpen: // �������� ��� �������������
    {
      res = 1;
    }
    break;
  case procThrow:
    {
		CValue& value = *ppValue[0];
		if (2 == value.GetTypeCode()) // ������
			CRuntimeExceptionClass::RuntimeError(value, value.GetString());
		else
			CRuntimeExceptionClass::RuntimeError(value);
    }
    break;
  case procAddDynamicProperty:
    {
		// TODO �������� �������� ������ �������� � ��� �������-�����������, � �� ������ ��� �������� ���-�
		if (!defFnNames->IsDefineFreePropFunctions())
			RuntimeError("���::����������������������������: � ������ ������ ���� ����������� ��������������� ������ _����������������� � _�����������������");

		CValue& value = *ppValue[0];
		//CString errMsg;
		//errMsg.FormatV("%s.����������������������������: ���������� ���������� ������", GetTypeString());
		CString errMsg("���::����������������������������: ���������� ���������� ������");
		ValidateParamNotEmptyString(value, errMsg);

		CComponentClass* pHandlerClass = this;
		CComponentClass* pBaseClass = VerifyCurrentExecuteContextIsBaseClassForThisClass();
		if (pBaseClass)
			pHandlerClass = pBaseClass;

		int iNewPropertyNumber = pHandlerClass->GetNProps();

		pHandlerClass->m_pDynamicPropertyManager->Add(value.GetString(), iNewPropertyNumber);
      return 1;
    }
	break;

	default:
		res = CallAsMethod(iMethNum, ppValue);
	};
	return res;
}


int  CComponentClass::FindMethod(char const * lpMethodName)const
{
  int res = defFnNames->GetIndexByName(lpMethodName);

  if (-1 != res)
  {
	const CParamDefs::paramdefs& MethodParam = (*defFnNames)[res];
    if (MethodParam.m_bIsMethodHaveUnlimitedCountOfParams || MethodParam.m_bIsMethodHaveParamsReceivedByValue)
    {
      pImplicitParams->m_strClassAndMethCurrent = m_NameOfInstance;
      pImplicitParams->m_strClassAndMethCurrent += ":";
      pImplicitParams->m_strClassAndMethCurrent += lpMethodName;
    }
  }
  else
  {
    int nCountMeth = defFnNames->Size();
    for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter)
    {
      CBLContext *pCont = *iter;

      res = pCont->FindMethod(lpMethodName);

      if (res != -1)
      {
        res += nCountMeth;
        break;
      }
      nCountMeth += pCont->GetNMethods();
    }
  }
  return res;
}


char const *  CComponentClass::GetMethodName(int iMethodNum,int iMethodAlias)const
{
  if (IsMethodFromBaseContexts(iMethodNum))
  {
    int nMeth;
	CSafeContextPtr<CBLContext> pCont = FindBaseContextByMethodNumber(iMethodNum, nMeth);
	if (pCont)
         return pCont->GetMethodName(nMeth, iMethodAlias);
    return NULL;
  }
	else
		return (*defFnNames)[iMethodNum].Names[iMethodAlias];
}

int  CComponentClass::GetNMethods(void)const
{
  int nCountMeth = 0;
  if (defFnNames.isValid())
    nCountMeth = defFnNames->Size();

  CONST_ITER_CONT iEnd(vecOfBaseCont.end());
  for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
  {
    CBLContext *pCont = *iter;
    nCountMeth += pCont->GetNMethods();
  }
	return nCountMeth;
}

int  CComponentClass::HasRetVal(int iMethodNum)const
{
  if (IsMethodFromBaseContexts(iMethodNum))
  {
    int nMeth;
	CSafeContextPtr<CBLContext> pCont = FindBaseContextByMethodNumber(iMethodNum, nMeth);
	if (pCont)
         return pCont->HasRetVal(nMeth);
    return 0;
  }
  else
    return (*defFnNames)[iMethodNum].HasReturnValue;
}

int CComponentClass::GetNParams(int iMethodNum)const
{
  if (IsMethodFromBaseContexts(iMethodNum))
  {
    int nMeth;
	CSafeContextPtr<CBLContext> pCont = FindBaseContextByMethodNumber(iMethodNum, nMeth);
	if (pCont)
         return pCont->GetNParams(nMeth);
    return 0;
  }
  else
  {
    if (pImplicitParams && iMethodNum >= lastMethod)
    {
	  const CParamDefs::paramdefs& MethodParam = (*defFnNames)[iMethodNum];
      if (MethodParam.m_bIsMethodHaveUnlimitedCountOfParams)
        return pImplicitParams->GetNParams(this, iMethodNum, MethodParam.NumberOfParams);
      else
        return MethodParam.NumberOfParams;
    }
    else if (iMethodNum <= lastMethod)
      return (*defFnNames)[iMethodNum].NumberOfParams;
    else
      return -1;
    //return (*defFnNames)[iMethodNum].NumberOfParams;
  }
}

bool CComponentClass::VerifyMethodForParamsByValueAndSetMetaInfoClassesDataForParamsByValue( const CString& strNamesClassAndMeth, const CBLProcInfo &procInfo)
{
	bool bResult = false;

	const iParamByValue = 2;
	int nVars = procInfo.GetNVars();
	int iMethodNum = procInfo.GetProcPos(); //m_ProcID

	CCompiledModule* pCompiledModule = procInfo.GetModule()->GetInternalData()->pCompiledModule;
	CCompileProcInfo* pCompileProcInfo = pCompiledModule->ProcArray[iMethodNum];
	for (int i = 0; i < nVars;i++)
	{
		CBLModuleProcVarDescr* pVar = pCompileProcInfo->VarList[i];
		if (iParamByValue == pVar->Type){
			bResult = true;
			break;
		}
	}

	if (bResult){
		CMetaDataOfClasses& m_Meta = CSetOfHierarchy::GetHierarchy()->GetMetaDataOfClasses();
		CParamDefs::CArrayOfMethodsParamData_PTR pArr( CParamDefs::Create_ArrayOfMethodsParamData() ); //new CParamDefs::ARR_TYPE_NAME); // TODO ������� ������ � �������, ����� ��������� �� �����-�������

		for (int i = 0; i < nVars;i++)
		{
			CBLModuleProcVarDescr* pVar = pCompileProcInfo->VarList[i];
			bool bParamByValue = iParamByValue == pVar->Type;
			pArr->Add(CMethodsParamData::Create(pVar->Name, "", "", bParamByValue));
		}
		m_Meta.AddMethHaveParVal(strNamesClassAndMeth, pArr);
	}
	return bResult;
}

BOOL CComponentClass::GetParamDefValueFromClassDefinitionOrModule(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{
	bool bIsExistParam = false;
	const CSetOfHierarchy::CParamDefValueEnum ParamDefValueEnum = CSetOfHierarchy::GetHierarchy()->GetParamDefValue(iMethodNum, iParamNum, pDefValue, this);
	if (ParamDefValueEnum == CSetOfHierarchy::pdve_DefValueIsExists)
		return TRUE;
	else if (ParamDefValueEnum == CSetOfHierarchy::pdve_ParamDefineWithoutDefValue)
		return FALSE;

	return GetParamDefValueFromClassModule(iMethodNum, iParamNum, pDefValue);
}

BOOL CComponentClass::GetParamDefValueFromClassModule(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{
	const CParamDefs::paramdefs& ParamDefs = (*defFnNames)[iMethodNum];
	int iRealMethodNum = ParamDefs.nProcPos;

	CCompiledModule* pCompiledModule = m_pMod->GetInternalData()->pCompiledModule;
	CBLModuleProcVarDescr* pVar = pCompiledModule->ProcArray[iRealMethodNum]->VarList[iParamNum];
	if(pVar->Type2 >= 0)
	{
		*pDefValue = pCompiledModule->ConstArray[pVar->Type2]->val;
		return TRUE;
	}
	return FALSE;
}

int  CComponentClass::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{
  // ������ ������� ���������� ����� ������� CallAsFunc ��� CallAsProc ���
  // ���������� ���� � �������� ���������� pDefValue

	// http://www.1cpp.ru/forum/YaBB.pl?num=1198844355/20#20
	// � ������ ����������� ��������� �������:
	// 1. ����� ������ ������ ����� ������ ��� ���().
	// 2. � ���������� ������ ������ ���� ��������, ��� �������� ������� �������� �� ���������.
	// ��! ������� �� � defcls.prm, � � ���� ������ (��������� �����, ���� ��� ��� ���������� ������)

	// ... ���� ��������� ������ ��������� �� pDefValue
	// � ����������� �� ������ ������� �� ����� ��������� (FALSE) ��� ��������� (TRUE) �������������
	// ���������, ���������� ��������������� � ����������� ������
	if (!pDefValue)
		return TRUE;

	if (IsMethodFromBaseContexts(iMethodNum))
  {
    bool bFind = false;
    int  nMeth = -1;
    CBLContext *pCont = NULL;
    if (m_MethParamsDef.nNumberOfMeth == iMethodNum && m_MethParamsDef.pContext != NULL)
    {
      nMeth = m_MethParamsDef.nRealNumberOfMeth;
      pCont = m_MethParamsDef.pContext;
      bFind = true;
    }
    else
    {
		pCont = FindBaseContextByMethodNumber(iMethodNum, nMeth);
		if (pCont)
			 bFind = true;
    }
    if (bFind && pCont)
    {
      m_MethParamsDef.nNumberOfMeth = iMethodNum;
      m_MethParamsDef.nRealNumberOfMeth = nMeth;
      m_MethParamsDef.pContext = pCont;

      return pCont->GetParamDefValue(nMeth, iParamNum, pDefValue);
    }
		return FALSE;
  }
  else
  {
    if (funcGetBaseClass == iMethodNum && iParamNum == 0)
    {
      pDefValue->Reset();
      return TRUE;
    }
    else if (funcGetBaseClass == procThrow)
    {
      if      (iParamNum == 0)
        pDefValue->Reset();
      else if (iParamNum == 1)
        *pDefValue = "";
      else
        return FALSE;

      return TRUE;
    }
    else
		return GetParamDefValueFromClassDefinitionOrModule(iMethodNum, iParamNum, pDefValue);
  }
	return FALSE; // ������ ��������� �� ��������� �� ��������������
}

void  CComponentClass::IncrRef(void)
{
	++m_RefCount;
}

void  CComponentClass::DecrRef(void)
{
	static bool _bAlreadyInsideDecRefCount = false;
	static CComponentClass* pExceptionInsideDestructorClass = NULL;

	if (m_RefCount != 1)
		--m_RefCount;
	else
	{
		if (m_pMod)
		{
			if (defFnNames->nPosDestructor != -1)
			{
				// ����� �������� ��� ������ � ������������ � ������������
				// ����� ������� ���������� DecrRef ������ ��� �� ���������� ������ ��� !!
				// � � ���� ������ ��� ����� ������ ������� ������ ��� ���������� ������ �����������
				// � ��� ��
				//
				//  ----------  ��  -----------------
				// �� �� ����. ���� (� ������� ��� ����� �������-���������� (��������, �������� ��� � ������� ���������)
				// 1� ����� ������
				// �� ���� ��� ��� �� ��� ������� :( -
				// �.�. ������������, ����� ���� ������� ��� ���������� ������  � � ����� ������� ��������� ���������� � ����������� !!
				//
				//		��������� ����������������������������������������������()
				//			������ = �������������("�����������������������������");
				//			������1 = �������������("�����������������������������");
				//
				//			������ = 0;
				//		��������������	// ���������������������������������
				//
				// � �� ����. ���� �� ������ !
				//
				// 1.	��������� ����������������������������������������������2()
				//			������ = �������������("�����������������������������");
				//			������1 = �������������("�����������������������������");
				//
				//		��������������	// ���������������������������������
				//
				// 2.	��������� ����������������������������������������������_�����������������������() �������
				//			������ = �������������("�����������������������������");
				//			������1 = �������������("�����������������������������");
				//
				//			������1 = 0; // ��� ������ = 0;
				//		��������������
				//
				// � �������� �����, ���� �������� ��� � �����, ������ �� �����! �.�. ��� ��� ��������� � ���� �������-���������� !

				// ���� ��� ������� �������� ����� ������ ����������� ������ (���� ����� ����)
				// ����� ������ ��������� ����� ���������� � ������������
				//

				bool bNeedExceptionHandler = false;
				if (_bAlreadyInsideDecRefCount && this == pExceptionInsideDestructorClass)
				{
					bNeedExceptionHandler = true;
				}
				if (!_bAlreadyInsideDecRefCount)
				{
					CRuntimeExceptionClass::SaveStatusOfTryingBlock();
				} // ��������
				//if (_bAlreadyInsideDecRefCount)
				if (!bNeedExceptionHandler && !m_bDonNotCallDestructor)
					int res = CBLModuleWrapper::CallAsProc0(m_pMod, defFnNames->nPosDestructor);

				if (!_bAlreadyInsideDecRefCount)
					{
						_bAlreadyInsideDecRefCount = true;
						pExceptionInsideDestructorClass =  this;

						CRuntimeExceptionClass::IfNeedExceptionThenThrowException(); // TODO

						_bAlreadyInsideDecRefCount = false;
					}
				//} // ��������
				if (bNeedExceptionHandler)
				{
					_bAlreadyInsideDecRefCount = false;
					pExceptionInsideDestructorClass = NULL;
				}
			}
		}

		ReplaceContextInModulesOfBaseClasses();

		ClearAllBasesClass();

		m_RefCount = 0;

		delete this;

	}
}

// ������ ������� ������������ ��� ������ ������(������������) � 1�
// �������� ������������ ������� _����������� ��� ���,
// ���� � ��� ������ ������ ����� ������ � ������������� �������
// ���� �� ������� ������������ ���, ������, ���� ����� �� ����� �������������
//
char const *  CComponentClass::GetCode(void) const
{
  if (m_pMod)
  {
	  // �������� ������������ ������� _�����������
	  // ������� ������� ����� ����� _����������� � ������� ������,
	  // ���� �� �����, ����� � ������� �������
	int nPosCode = FindMethod(m_pMod, "_�����������", "_GetCode");

    if (-1 != nPosCode)
    {
      CValue ret;
	  const_cast<CComponentClass*>(this)->CallAsMethod(nPosCode, NULL, true, &ret);
      return ret.GetString().operator LPCTSTR();
    }
  }
  return NULL;
}

class CObjID   CComponentClass::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  CComponentClass::GetInternalData(void)
{
	return CBLContext::GetInternalData();
}

long  CComponentClass::GetTypeID(void)const
{
	return 100;
}

char const *  CComponentClass::GetTypeString(void)const
{
	return m_NameOfInstance;
}

class CType   CComponentClass::GetValueType(void)const
{
	CType tType(100);
	return tType;
}

void  CComponentClass::InitObject(char const * strName)
{
	//m_NameOfInstance = strName;

	// ����� �������� ������ �������� �� ��� ������������ �������,
	// � ������� �� ����� �������� artbear
	CString strResult = CSetOfHierarchy::GetHierarchy()->GetClassNameFromDefinition(strName);

	if (strResult.IsEmpty())
		RuntimeError("������ ������� ����� ��� ����� ������");

	m_NameOfInstance = strResult;

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	InitInstanceComp();
}

int  CComponentClass::IsExactValue(void)const
{
	return 1;
}

int  CComponentClass::IsOleContext(void)const
{
  int regEBX = 0;
  __asm {
    mov regEBX, ebx // � ���� �������� 1� ������ ���������� ���������� ��� ������ ������
  }
  pImplicitParams->m_CountParametersInCallMethod = regEBX; // �������� ��� ��������
	return 0;
}

int  CComponentClass::IsPropReadable(int iPropNum)const
{
	return 1;
}

int  CComponentClass::IsPropWritable(int iPropNum)const
{
	return 1;
}

int  CComponentClass::IsSerializable(void)
{

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

  int iProc = m_pMod->FindFunc("����������������");
  iProc = iProc == -1 ? m_pMod->FindFunc("IsSerializable") : iProc;
  if (iProc != -1)
  {
       CValue retVal;
       int res = CBLModuleWrapper::CallAsFunc0(m_pMod, iProc, retVal);

	CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

    return retVal.GetNumeric();
  }
  else
    return 0;
}

int  CComponentClass::SaveToString(class CString & csStr)
{
#ifndef _DEBUG

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

  if (m_pMod)
  {
    int iProc = m_pMod->FindFunc("���������������������");
    iProc = iProc == -1 ? m_pMod->FindFunc("SaveToString") : iProc;
    if (iProc != -1)
    {
       CValue retVal;
       int res = CBLModuleWrapper::CallAsFunc0(m_pMod, iProc, retVal);

		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

      csStr = retVal.GetString();
    }
  }
#else
  ::ShowMsg("� _DEBUG ������ � ������� SaveToString �������� ������, ���� ��� �������� 1� ������!!!",mmInformation);
#endif
	return 1;
}

void  CComponentClass::SelectByID(class CObjID cID,long lNum)
{
	//CBLContext::SelectByID(cID,lNum);
}

int  CComponentClass::GetNProps(void)const
{

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	// ������������ �������� ����������� �����
	//   if (defFnNames->IsDefineFreePropFunctions())
	//   {
	//     static int nPosNProps = -1;
	// 	nPosNProps = m_pMod->FindFunc("_���������������������");
	//     if (-1 == nPosNProps)
	//       nPosNProps = m_pMod->FindFunc("_GetNDynamicProps");
	//     if (nPosNProps != -1)
	//     {
	//        CValue rValue;
	// 		CBLModuleWrapper::CallAsFunc0(m_pMod, nPosNProps, rValue);
	//
	// 		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();
	//
	//       return rValue.GetNumeric();
	//     }
	//   }
	//   else
	//   {

    int nCountProps = m_pMod->GetNStaticVars();
    for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter)
    {
      CBLContext *pCont = *iter;
      nCountProps += pCont->GetNProps();
    }

    nCountProps += m_pDynamicPropertyManager->Size();
	return nCountProps;

	//  }
  return 0;
}

char const *  CComponentClass::GetPropName(int A,int B)const
{

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	// ������������ �������� ����������� �����
	//   if (defFnNames->IsDefineFreePropFunctions())
	//   {
	//     static int nPosGetPropName = -1;
	// 		nPosGetPropName = m_pMod->FindFunc("_��������������������");
	//     if (-1 == nPosGetPropName)
	//       nPosGetPropName = m_pMod->FindFunc("_GetPropDynamicName");
	//     if (nPosGetPropName != -1)
	//     {
	//       CValue rValue;
	//       CBLModuleWrapper::CallAsFunc1(m_pMod, nPosGetPropName, rValue, A+1);
	//
	// 		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();
	//
	//       return rValue.GetString();
	//     }
	//   }
	//   else
	//   {

  if (defFnNames->IsDefineFreePropFunctions())
  {
	LPCTSTR szName = m_pDynamicPropertyManager->FindNameByNumber(A);
	if (szName)
		return szName;
  }

    int nCountProps = m_pMod->GetNStaticVars();
    if (nCountProps > A)
    {
      CBLVarInfo VarInfo;
	    m_pMod->GetStaticVarDescr(A,VarInfo);
	    return VarInfo.GetName();
    }
    else
    {
      CONST_ITER_CONT iEnd(vecOfBaseCont.end());
      for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
      {
        CBLContext *pCont = *iter;
        int nNProps = pCont->GetNProps();
        if (A < nCountProps + nNProps)
        {
          return pCont->GetPropName(A-nCountProps , B);
        }
        else
          nCountProps += nNProps;
      }
    }

	//}

  return NULL;
}

int  CComponentClass::GetPropVal(int iPropNum,class CValue & rValue)const
{

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	//   if (iPropNum > 65536)
	//   {
	//     POSITION pos;
	//     CString key;
	//     int *val = NULL;
	//     for (pos = defFnNames->mapNamesFreeProp.GetStartPosition(); pos != NULL;)
	//     {
	//       defFnNames->mapNamesFreeProp.GetNextAssoc( pos, key, (void*&)val );
	//       if ((int)val == iPropNum - 65536)
	//       {
	// 		int ret = CBLModuleWrapper::CallAsFunc1(m_pMod, defFnNames->nPosOnReadFreeProp, rValue, CValue(key));
	//
	// 		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();
	//
	//         return ret;
	//       }
	//     }
	//
	//   }
  if (defFnNames->IsDefineFreePropFunctions())
  {
	LPCTSTR szName = m_pDynamicPropertyManager->FindNameByNumber(iPropNum);
	if (szName)
	{
		int ret = CBLModuleWrapper::CallAsFunc1(m_pMod, defFnNames->nPosOnReadFreeProp, rValue, CValue(szName));

		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

		return ret;
	}
  }

  int nCountProps = m_pMod->GetNStaticVars();

  if (nCountProps > iPropNum)
  {
    int res = m_pMod->GetStaticVarValue(iPropNum,rValue,0);
    int iProc = -1;
    if (defFnNames.isValid())
      iProc = defFnNames->GetIndexPropetry(GetPropName(iPropNum, 0), m_pMod, false);

    if (iProc != -1)
    {
	  int res = CBLModuleWrapper::CallAsProc1(m_pMod, iProc, rValue);

		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

      return res;
    }
    return res;
  }
	else
  {
    CONST_ITER_CONT iEnd(vecOfBaseCont.end());
    for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
    {
      CBLContext *pCont = *iter;
      int nNProps = pCont->GetNProps();
      if (iPropNum < nCountProps + nNProps)
      {
        return pCont->GetPropVal(iPropNum - nCountProps, rValue);
      }
      else
        nCountProps += nNProps;
    }
    return 0;
  }
}

int  CComponentClass::SetPropVal(int iPropNum,class CValue const & vValue)
{

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	// ������������ �������� ����������� �����
	//   if (iPropNum > 65536)
	//   {
	//     POSITION pos;
	//     CString key;
	//     int *val = NULL;
	//     for (pos = defFnNames->mapNamesFreeProp.GetStartPosition(); pos != NULL;)
	//     {
	//       defFnNames->mapNamesFreeProp.GetNextAssoc( pos, key, (void*&)val );
	//       if ((int)val == iPropNum - 65536)
	//       {
	//
	// 	  int ret = CBLModuleWrapper::CallAsProc2(m_pMod, defFnNames->nPosOnWriteFreeProp, const_cast<LPSTR>(LPCSTR(key)), vValue);
	//
	// 		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();
	//
	//         return ret;
	//       }
	//     }
	//   }
  if (defFnNames->IsDefineFreePropFunctions())
  {
	LPCTSTR szName = m_pDynamicPropertyManager->FindNameByNumber(iPropNum);
	if (szName)
	{
		int ret = CBLModuleWrapper::CallAsProc2(m_pMod, defFnNames->nPosOnWriteFreeProp, const_cast<LPSTR>(szName), vValue);

		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

		return ret;
	}
  }

  int nCountProps = m_pMod->GetNStaticVars();

  if (nCountProps > iPropNum)
  {

    int iProc = -1;
    if (defFnNames.isValid())
      iProc = defFnNames->GetIndexPropetry(GetPropName(iPropNum, 0), m_pMod, true);

    if (iProc != -1)
    {

	  int ret = CBLModuleWrapper::CallAsProc1(m_pMod, iProc, vValue);

		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

      return ret;
    }
    return m_pMod->SetStaticVarValue(iPropNum,vValue,0);
  }
	else
  {
    CONST_ITER_CONT iEnd(vecOfBaseCont.end());
    for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
    {
      CBLContext *pCont = *iter;
      if (iPropNum >= nCountProps && iPropNum < nCountProps + pCont->GetNProps())
      {
        return pCont->SetPropVal(iPropNum - nCountProps, vValue);
      }
      else
        nCountProps += pCont->GetNProps();
    }
    return 0;
  }
}

int  CComponentClass::FindProp(char const * Name)const
{
	// ������������ �������� ����������� �����
	//   if (defFnNames->IsDefineFreePropFunctions())
	//   {
	//     int *i = NULL;
	//     int nCount = 0;
	//     if (!defFnNames->mapNamesFreeProp.Lookup(Name, (void*&)i))
	//     {
	//       nCount = defFnNames->mapNamesFreeProp.GetCount() + 1;
	//       defFnNames->mapNamesFreeProp[Name] = (int *)nCount;
	//     }
	//     else
	//       nCount = (int)i;
	//     return 65536 + nCount;
	//   }

  if (defFnNames->IsDefineFreePropFunctions())
  {
	const int iPropNumber = m_pDynamicPropertyManager->FindNumberByName(Name);
	if (-1 != iPropNumber)
		return iPropNumber;
  }

  int nNumProp = m_pMod->FindStaticVar(Name);
  if (nNumProp == -1)
  {
    int nCountProps = m_pMod->GetNStaticVars();

    CONST_ITER_CONT iEnd(vecOfBaseCont.end());
    for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
    {
      CBLContext *pCont = *iter;
      int res = pCont->FindProp(Name);
      if (res != -1)
        return nCountProps + res;
      nCountProps += pCont->GetNProps();
    }
  }
  else
  {
    CBLVarInfo VarInfo;
	  m_pMod->GetStaticVarDescr(nNumProp,VarInfo);
    if (VarInfo.IsExported() == 0)
      nNumProp = -1;
  }

	return nNumProp;
}

// ��� ������ ���������, ���������� ��������� ������� � �������������� ������
CComponentClassModulePtr CComponentClass::FirstInit(const CString& FullFileName)
{
    //CBLModule* pModLocale = NULL;
	CComponentClassModulePtr pModLocale(NULL);

	  CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();

	  CModuleString ModuleString;
	  if (!ModuleString.LoadModule(FullFileName, m_NameOfInstance))
	  {
		::ShowMsg(FullFileName, mmBlackErr);
		return NULL;
	  }
	  try
	  {
		CPreprocessor::Preprocess(true, ModuleString, pHierarchy->m_SymbolOfPreprocessor, CMapStringToString(), FullFileName);
	  }
	  catch (CPreprocessor::ExeptionPreprocessor &ex)
	  {
		::ShowMsg("� ������ ������: %s (%s)", mmBlackErr, m_NameOfInstance, FullFileName);
		::ShowMsg(ex.GetErrorString(), mmBlackErr);

		if (!ex.GetCodeString().IsEmpty())
		{
			::ShowMsg("(%d) %s", mmBlackErr, ex.GetNumString(), ex.GetCodeString());
		}
		pHierarchy->m_SymbolOfPreprocessor.RemoveKey("_NOW_PREPARE_CLASS");
		return NULL;
	  }

	  if (ModuleString.IsEmpty())
		RuntimeError("�����: %s �� ������������ ����� (%s) �� ������� ������!", m_NameOfInstance, FullFileName);

	  //pModLocale = new CBLModuleWrapper(m_NameOfInstance, NULL, str, FullFileName);
	  //pModLocale = new CComponentClassModule(m_NameOfInstance, NULL, str, FullFileName);
	  pModLocale = CComponentClassModulePtr(new CBLModuleWrapper(m_NameOfInstance, NULL, ModuleString));

	  int nComp = pModLocale->Compile();

	  if (nComp != 1) {
		::ShowMsg("�����: %s ����: %s ���������� �� ���������!", mmRedErr, m_NameOfInstance, FullFileName);
		return NULL;
	  }

	  //////////////////////////////////////////////////////////////////////
	  // �������� ������� � ������� ������ ��� 1� �����
	  // {{<"English_description">,<"�������_��������">},<0-��� ���������, 1-��� �������>,<���������� ����������>}
	  //////////////////////////////////////////////////////////////////////
	  // delete defFnNames; // artbear
	  //defFnNames = new CParamDefs();
	  //defFnNames = new CParamDefsWithRefCount();
	  defFnNames = CParamDefsWithRefCountPtr(new CParamDefs());

	  int iProc = pModLocale->FindProc("�����������", 0);
	  iProc = (iProc == -1) ? pModLocale->FindProc("Constructor", 1) : iProc;
	  defFnNames->nPosConstructor = iProc;
	  iProc = pModLocale->FindProc("����������", 0);
	  iProc = (iProc == -1) ? pModLocale->FindProc("Destructor", 1) : iProc;
	  defFnNames->nPosDestructor = iProc;

	  int nReadPos = pModLocale->FindFunc("_�����������������");
	  nReadPos = (nReadPos != -1) ? nReadPos : pModLocale->FindFunc("_OnReadProperty");
	  if (nReadPos != -1)
	  {
		int nWritePos = pModLocale->FindProc("_�����������������", 0);
		nWritePos = (nWritePos != -1) ? nWritePos :pModLocale->FindProc("_OnWriteProperty", 1);
		if (nWritePos != -1)
		{
		  defFnNames->nPosOnReadFreeProp  = nReadPos;
		  defFnNames->nPosOnWriteFreeProp = nWritePos;
		}
	  }

// artbear - �������� � Init
// 	  if(pMainAddIn->m_EnableOptimizate)  // ������� ������ � ���� ������� ��� ������� ������
// 		MapDef.AddModule(m_NameOfInstance, pModLocale, defFnNames);

	  int nProc = pModLocale->GetNProcs(); // ������� ����� ���������� �������� � ������

	  if (pHierarchy) // �������������� �������� ������ �������������� ����������� ���������� �� �������� ������, �� ��� �������
		pHierarchy->InitParamDefsClass(defFnNames.get(), m_NameOfInstance);



	  defFnNames->SetPoolSize(nProc + lastMethod + 1, pModLocale->GetNStaticVars());

	  defFnNames->AddParam("GetBaseClass", "��������������������", funcGetBaseClass, 1, 1);
	  defFnNames->AddParam("AssignFriendModule", "�������������������������",procAssignFriendModule, 0, 0);

	  // ���� ���������� ������ ����� ������� artbear
	  // TODO ��������, � ���������� �� ������ ���� ������ ������ ������, � ������� �������
	  //
	  //defFnNames->AddParam("AssignBaseClass", "���������������������", procAddBaseClass, 0, 1);

	  defFnNames->AddParam("SendMessageOwnMod", "���������������������������", funcSendMessageOwnMod, 1, 3);
	  defFnNames->AddParam("GetPathName", "������������", funcGetPathName, 1, 0);
	  defFnNames->AddParam("GetEnvContext", "�������������������������", funcGetEnvContext, 1, 0);
	  defFnNames->AddParam("GetParamsList", "������������������������", funcGetParamsList, 1, 1);

	  defFnNames->AddParam("Repla�eBaseObject", "���������������������", funcReplaceInstBaseClasses, 1, 2);  //recommended
	  defFnNames->AddParam("ReplaceInstBaseClasses", "��������������������������", funcReplaceInstBaseClasses_SYN1, 1, 2);  //deprecated

	  defFnNames->AddParam("SetOnIndexParams", "���������������������������", funcSetOnIndexParams, 1, 3);
	  defFnNames->AddParam("_OnOpen", "_�����������", procOnOpen, 0, 0);
	  defFnNames->AddParam("_Throw", "_�������������", procThrow, 0, 1);

	  defFnNames->AddParam("IsObject", "����������������", funcIsObject, 1, 1);

	  defFnNames->AddParam("AddDynamicProperty", "����������������������������", procAddDynamicProperty, 0, 1);


	  for (int count = 1; count < nProc; count++)
	  {
		char nameProc[255] = {0};
		CBLProcInfo procInfo;
		pModLocale->GetProcInfo(count, procInfo); // ������� ����������� ���. �� ���������� � ��������
		strcpy(nameProc, procInfo.GetName());
		int nParamas   = procInfo.GetNParams();
		int nVars      = procInfo.GetNVars();
		int hasRetVal  = procInfo.HasRetVal();
		int procPos    = procInfo.GetProcPos();

		if (procInfo.IsExported() == 1)
		{
		  CString strNamesClassAndMeth = m_NameOfInstance;
		  strNamesClassAndMeth +=":";
		  strNamesClassAndMeth +=nameProc;

			bool bIsMethodHaveParamByValue = pHierarchy->isMethHaveParVal(strNamesClassAndMeth);

// TODO ���� ����� ����� �������������� ����� ����������, ������, �� ����� ������ � defcls.prm � �� ������ ������ ����� ������ !!

			if (!bIsMethodHaveParamByValue)
				bIsMethodHaveParamByValue = VerifyMethodForParamsByValueAndSetMetaInfoClassesDataForParamsByValue(strNamesClassAndMeth, procInfo);


		  defFnNames->AddParam(NULL,nameProc, procPos, hasRetVal, nParamas,
			pHierarchy->ThisIsUnLimMethod(strNamesClassAndMeth), //������ ����� �� ����� ������� ���������
			bIsMethodHaveParamByValue); //������ ����� �� ����� ��������� ������������ �� ��������
		}
	  }

	  return pModLocale;
}

CBLModule* CComponentClass::Init(const char *file)
{
  // file - ��� �����, ��� ���������� ����� ������ ����������

  // ��������� �������� �� ������ ���� ������ MapDef ������ ������� �
  // ����������������� ������ ����������� � ������� ���������� ������,
  // �.�. ������ � ��� ������� (���������, �������, ����� ����������)
  // ����� ��� ���� ���������� ��������� ������. ��������� ����������,
  // ������ ���������� ������ ���� ��� ��� ������ �������� ������� ������.
  // ����������� ��������� ����� ����������� �������� �������
//   try
  {
    //CBLModule* pModLocale = NULL;
    CComponentClassModulePtr pModLocale(NULL);

    CString FullFileName(file);
    CSetOfHierarchy::MakeFullFileName(&FullFileName);

	//CParamDefs *pParDefs = MapDef.GetDefs(m_NameOfInstance);
	CParamDefsWithRefCountPtr pParDefs = MapDef.GetDefs(m_NameOfInstance);
    if (pMainAddIn->m_EnableOptimizate && pParDefs.isValid()) {
      defFnNames = pParDefs;
      pModLocale = MapDef.GetModule(m_NameOfInstance);
    }
    else
    {
       // ��� ������ ���������, ���������� ��������� ������� � �������������� ������
		pModLocale = FirstInit(FullFileName);

		if(pMainAddIn->m_EnableOptimizate && pModLocale.isValid())  // ������� ������ � ���� ������� ��� ������� ������
			MapDef.AddModule(m_NameOfInstance, pModLocale, defFnNames);
    }
    if (pModLocale.isValid())
    {
      if (m_pMod)
        m_pMod = NULL;

	  m_DebugInfo.OpenDebugForm(this);

// artbear kms ������ ����� �������
//       if (!m_pMod)
// 			m_pMod = * new CBLModuleWrapper(m_NameOfInstance, pCreatedCompModule, NULL, FullFileName);
//
//       m_pMod->AssignCompiled(*pModLocale);

       if (!m_pMod)
 			m_pMod = * new CBLModuleWrapper(m_NameOfInstance, pCreatedCompModule, pModLocale.get());

      if(!pMainAddIn->m_EnableOptimizate)
      {
		MapDef.DelDefParam(m_NameOfInstance);
        MapDef.DelModule(m_NameOfInstance);
        //delete pModLocale;
        pModLocale = CComponentClassModulePtr(NULL);
      }

      if (m_pCurModule)
		m_pMod->AssignFriendModule(m_pCurModule);

      //m_DebugInfo.AssignDebugModule(this, m_pMod);
	CSafeModulePtr pMod;
	m_DebugInfo.AssignDebugModule(this, pMod); // � kms � ������� ����� ��� !!
	if (pMod)
		m_pMod = pMod;


      if (m_pMod->Execute() != 1)
		RuntimeError("��������� ������ ������: %s ��� ������������� �� �������!", m_NameOfInstance);
    }
    if (m_pMod)
    {
      m_MapOfModulesAndComp[m_pMod] = this;
    }
    return m_pMod;
  }
//   catch (...)
//   {
// 	RuntimeError("����������� ����������! ��� ������������� ������: %s!", m_NameOfInstance);
//   }
  return NULL;
}

class GuardOfFlagCreateWithoutConstructor
{
private:
	bool m_bCreateWithoutConstructorSaved;
public:
	GuardOfFlagCreateWithoutConstructor() { m_bCreateWithoutConstructorSaved = CComponentClass::IsCreateWithoutConstructor(); };
	~GuardOfFlagCreateWithoutConstructor() { if (m_bCreateWithoutConstructorSaved) CComponentClass::SetFlagCreateWithoutConstructor(); };
};

void CComponentClass::InvokeConstructor()
{
  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

  {
	  GuardOfFlagCreateWithoutConstructor guard;
	  ClearFlagCreateWithoutConstructor();

	  CONST_ITER_CONT iEnd(vecOfBaseCont.end());
	  for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
	  {
		CBLContext *pCBLContext = *iter;
		if (IS_KINDOF_CComponentClass(pCBLContext))
		{
		  CComponentClass* pCont = static_cast<CComponentClass*>(pCBLContext);

		  pCont->InvokeConstructor();
		}
	  }
  }

  if (m_pMod)
  {
    if (-1 != defFnNames->nPosConstructor && !IsCreateWithoutConstructor())
    {

	  int res = CBLModuleWrapper::CallAsProc0(m_pMod, defFnNames->nPosConstructor);

		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

    }
  }
}

// CBLModule * CComponentClass::GetHostModule() const
// {
//   return pCurModule;
// }
//
// CBLModule * CComponentClass::GetModule() const
// {
//   return m_pMod;
// }

BOOL CComponentClass::GetBaseClass(const CRuntimeClass* pRTClass, CBLContext** ppBaseObj) const
{
    for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter)
    {
		CBLContext *pCont = *iter;
        if (pCont->GetRuntimeClass() == pRTClass)
        {
            *ppBaseObj = *iter;
            return TRUE;
        }
        else if (pCont->GetRuntimeClass() == RUNTIME_CLASS(CComponentClass))
        {
            if ((static_cast<CComponentClass*>(pCont))->GetBaseClass(pRTClass, ppBaseObj))
                return TRUE;
        }
    }
    return FALSE;
}


// ����� ����� ������ �� ��������
// ������� ������ � ������ ������ �� ������, ���� ����� ��� �������
// � ����� � �������������� ������� ������, ����� � �����������
//
// ������� ������ ������� ���, � ����� ����������
//
inline int CComponentClass::FindMethod(CBLContext* pContext, CBLModule *pMod, LPCTSTR sNameRus, LPCTSTR sNameEng)
{
	int nPosCode = -1;

	// ������� � ������ �������� ������ �����
	// ������ ����� , ��������� ��� "�������" ��� � �������
    if (-1 == nPosCode)
		nPosCode = pMod->FindFunc(sNameRus);

	// ����� �������� �� �����������
    if (-1 == nPosCode)
		nPosCode = pContext->FindMethod(sNameRus);

	// ������� � ������ �������� ������ �����
	// ������ ����� , ��������� ��� "�������" ��� � �������
    if (-1 == nPosCode)
		nPosCode = pMod->FindFunc(sNameEng);

	// ����� �������� �� �����������
    if (-1 == nPosCode)
	      nPosCode = pContext->FindMethod(sNameEng);

	return nPosCode;
}

// ���������, �������� �� ������� ����������� ������ ������� �������� ������ ��� �������� ������
// ������������ ��� ���������� ��������
CComponentClass* CComponentClass::VerifyCurrentExecuteContextIsBaseClassForThisClass() const
{

	CComponentClass* const pCompClass = CComponentClass::GetComponentClassCurExModule();

	CBLContext* const pContExecMod = CBLModuleWrapper::GetContextFromModule(CBLModule::GetExecutedModule());
    if (pCompClass != NULL && pCompClass != this
      && (CBLContext*)this == pContExecMod)
		return pCompClass;

	return NULL;

}

CComponentClass* CComponentClass::GetComponentClassCurExModule(CBLModule* pModWrapper)
{
  CBLModule* pMod = pModWrapper == NULL ? CBLModule::GetExecutedModule() : pModWrapper;
  return static_cast<CComponentClass*>(m_MapOfModulesAndComp[pMod]);
}

bool CComponentClass::AddBaseClass(const CString& name)
{
	  CSafeContextPtr<CBLContext> pCont(name);
      if (pCont)
      {
		// ��������� ����������� ��������� � ������ ����������
		//  ��� ���������� ������������� ������������

		vecOfBaseCont.push_back(pCont);

		return true;
      }
	  return false;
}

void CComponentClass::ClearAllBasesClass()
{
	// ����� ����������� ���������� ������ � �������� ������� ������������ ������ �������������
	while(!vecOfBaseCont.empty())
		vecOfBaseCont.pop_back();
}

void CComponentClass::ReplaceContextInModulesOfBaseClasses(CComponentClass* pMainCont)
{
	CONST_ITER_CONT iEnd(vecOfBaseCont.end());
	for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
	{
		CBLContext *pCBLContext = *iter;
		if (IS_KINDOF_CComponentClass(pCBLContext))
		{
			CComponentClass* pBaseClassContext = static_cast<CComponentClass*>(pCBLContext);
			// artbear - ����� �������� ��� ��� ����������� ������� ������, ��� ������
			// ��. ����� ������������������������������� � �������������������������������2
			//
			// TODO �������� ����������� � ��������� m_RefCount �
			//		���������� ������, ���� m_RefCount ���� ������, ��� m_RefCount �������
			//
			if (!pMainCont)
			{
				pBaseClassContext->m_pMod->GetInternalData()->pGeneralContext = pCBLContext;
				pBaseClassContext->ReplaceContextInModulesOfBaseClasses(pBaseClassContext);
			}
			else
				pBaseClassContext->m_pMod->GetInternalData()->pGeneralContext = pMainCont;
		}
	}

}

void CComponentClass::OnStartInit(void)
{
	if (!pCreatedCompModule)
	{
	  // ���������� ����������� ������ ���, ��� ������ ����� ������-�������������
	  pCreatedCompModule = this;

	  CDebug::SetDebugObject(this);
	}

}

void CComponentClass::OnEndInit(bool bIsBadExit)
{
	pCreatedCompModule=NULL;
	if (bIsBadExit){
		for (ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter){
			CBLContext* pCBLContext = *iter;
			if (IS_KINDOF_CComponentClass(pCBLContext))
			  static_cast<CComponentClass*>(pCBLContext)->SetFlagDonNotCallDestructor();
		}
		vecOfBaseCont.erase(vecOfBaseCont.begin(), vecOfBaseCont.end());
	}
}

void CComponentClass::FinishInit(void)
{
	if (this == pCreatedCompModule)
  {
    // ������ ������ ��� ��������� ������, ����� ������� ��� ��� ������������
	OnEndInit();

    InvokeConstructor();
    m_DebugInfo.ClearDebugObject();

  }
}

class CInitGuard
{
private:
	CComponentClass* m_pComponentClass;
	bool m_bIsBadExit;
public:
	CInitGuard(CComponentClass* pComp) :
		m_pComponentClass(pComp), m_bIsBadExit(true)
	{
		m_pComponentClass->OnStartInit();
	}
	~CInitGuard()
	{
		if (m_bIsBadExit) // ���� ���� ��������� ������ �� ����������� � �������� ������
			m_pComponentClass->OnEndInit(true);
	}

	void FinishInit(void) // ���� ����� ������ ���� ������ � ������ ����������� ����������
	{
		m_bIsBadExit = false;
		m_pComponentClass->FinishInit();
	}
};

// ��������� ������������� ���������� ������ �� ������ ���������� ����������
// �� ����� ������������� �������������
void CComponentClass::InitInstanceComp(void)
{
	CInitGuard InitGuard(this); // ������ ������������ ��������

	CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();
	CMetaDataOfClasses& m_Meta = pHierarchy->GetMetaDataOfClasses();

	CString NameOfComponent = GetTypeString();
	CString NameOfFile = m_Meta.m_ClassBindFile[NameOfComponent];
	if (NameOfFile.IsEmpty() )
		//return false;
		RuntimeError("�� ������ ��� ����� ��� ������ %s!", NameOfComponent);

	if ( !Init(NameOfFile) )
	  RuntimeError("��������! ������������� ������: %s ����������� ��������!", NameOfComponent);

	CMetaDataOfClasses::CStringList_SharedPtr pList = m_Meta.m_ClassesHierarchyMap[NameOfComponent]; //CStringList* pList = (CStringList*)m_Meta.m_ClassesHierarchyMap[NameOfComponent];
	if (pList)
	{
		// ������� ��������
		CString sRecursiveHierarchy = "";
		if (pHierarchy->VerifyRecursiveInBaseClasses(NameOfComponent, NameOfComponent, sRecursiveHierarchy))
			RuntimeError("������ �������� ������ " + NameOfComponent + "\r\n������� �������� (������-�������): "+sRecursiveHierarchy);

		for(POSITION posList = pList->GetHeadPosition(); posList != NULL; )
		{
		  CString str = pList->GetNext( posList );

		  if (!AddBaseClass(str))
			RuntimeError("������� �����: %s ��� ������: %s �� ������!", str, NameOfComponent);
		}
	}

	InitGuard.FinishInit();
}

void CComponentClass::ClearOptimizationData(void)
{
	//CComponentClass::MapDef.Clear();
	CComponentClass::MapDef.ClearWithoutDelete();
}

// ����� ��� ������ ����������������� ������������ � ����������� �����������
// ������������ � ������ ���������������::�����()
void CComponentClass::CallUserConstructor(const CString&  strConstructorName, int nParamsCount, CValue **ppValue) const
{
	if (m_pMod)
	{
		int iProc = m_pMod->FindProc(strConstructorName, 0);
		iProc = -1 == iProc ? m_pMod->FindProc(strConstructorName, 1) : iProc;
		if (-1 == iProc)
			RuntimeError("� ������ ������ <%s> �� ������� ����� ���������-����������� <%s>", GetTypeString(), strConstructorName);

		// ������������ �� ������ ����� ������� ������� ��� ������� ������������
		int iPublicProc = FindMethod(strConstructorName);
		if (-1 != iPublicProc)
			RuntimeError("����������� %s::%s �� ������ ����� ������� �������", GetTypeString(), strConstructorName);

		CBLProcInfo info;
		m_pMod->GetProcInfo(iProc, info);
		int iNParams = info.GetNParams();
		if (iNParams < nParamsCount)
			RuntimeError("������������ %s::%s �������� ������� ������� ����� ���������� - ����� %d, � �������� %d",
				GetTypeString(), strConstructorName, iNParams, nParamsCount);

		if (iNParams > nParamsCount)
			RuntimeError("������������ %s::%s �������� ������� ����� ����� ���������� - ����� %d, � �������� %d",
				GetTypeString(), strConstructorName, iNParams, nParamsCount);

		int iRes = m_pMod->CallAsProc(iProc, nParamsCount, ppValue);
	}

}

BOOL CComponentClass::SendMessageOwnMod( CValue &rValue, CValue ** ppValue )
{
	return FALSE; // TODO ���� ������ ��������
}

void CComponentClass::GetPathName( CValue &rValue )
{
	return; // TODO ���� ������ ��������
}

void CComponentClass::GetEnvContext( CValue &rValue )
{
	return; // TODO ���� ������ ��������
}

void CComponentClass::GetParamsList( CValue &rValue, CValue * * ppValue )
{
	return; // TODO ���� ������ ��������
}

BOOL CComponentClass::SetOnIndexParams( CValue &rValue, CValue * * ppValue )
{
	return FALSE; // TODO ���� ������ ��������
}

void CComponentClass::ThrowException(CValue ** ppValue)
{
	return; // TODO ���� ������ ��������
}

void CComponentClass::AddDynamicProperty(CValue ** ppValue)
{
	return; // TODO ���� ������ ��������
}

// �������� ���������� ������� ��������.
DWORD CComponentClass::GetBaseObjCount()
{
	return vecOfBaseCont.size();
}

// �������� ��� �������� ������� �� �������.
CString CComponentClass::GetBaseObjName(DWORD idx)
{
	// TODO ��������� ������ �� �������
	return vecOfBaseCont[idx]->GetTypeString();
}

// �������� ������� ������ �� �������.
CBLContext* CComponentClass::GetBaseObj(DWORD idx)
{
	// TODO ��������� ������ �� �������
	return vecOfBaseCont[idx];
}

// ��� upcast'�
CBLContext* CComponentClass::GetDerivedObj()
{
	// TODO ������� ���������� ������
	return NULL;
}
