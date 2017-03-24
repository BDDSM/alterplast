
#include "StdAfx.h"
#include "Factory.h"
#include "istrmap.h"
#include "mycontextbase.h"
#include "ComponentClass.h"

#include "System/CreationBoost.h"

BEGIN_BL_METH_MAP(CObjectFactory)
	BL_METH_FUNC_WITH_UN_LIM_PARAMS("New", "�����", 1, &funcNewObject)

//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	BL_METH_PROC("����������������������������_�����", "����������������������������_�����", 1, &procDisableFastCreationAllContexts)
//#endif

END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CObjectFactory)
END_BL_PROP_MAP()

IMPLEMENT_MY_CREATE_CONTEXT(CObjectFactory, "ObjectFactory", "���������������");

enum{nMethodsCount = sizeof(CObjectFactory::m_MethDef)/sizeof(CObjectFactory::m_MethDef[0]) - 1};

class CClassItem : public CCollectionItem
{
public:
    CRuntimeClass* m_pRuntimeClass;
    int m_nCtorParamsCount;
};

CCollection collClasses;

void CObjectFactory::RegisterClass(LPCSTR szClassName, CRuntimeClass* pRuntimeClass, int nCtorParamsCount)
{
    CClassItem* pClassItem = new CClassItem;
    pClassItem->SetName(szClassName);
    pClassItem->m_pRuntimeClass = pRuntimeClass;
    pClassItem->m_nCtorParamsCount = nCtorParamsCount;
    collClasses.Add(pClassItem);
}

void CObjectFactory::UnRegisterClass(LPCSTR szClassName)
{
	const int nIndex = collClasses.GetIndex(szClassName);
	if (-1 == nIndex)
		return;
	CCollectionItem* pClassItem = collClasses[nIndex];

	collClasses.Remove(nIndex);
	delete pClassItem;
}

CObjectFactory::CObjectFactory()
{
}

int CObjectFactory::GetNMethods(void)const
{
    return nMethodsCount + collClasses.GetCount();
}

int CObjectFactory::FindMethod(char const* szName)const
{
	int iRes = CMyContextBase::FindMethod(szName);
	if (-1 != iRes)
		return iRes;

	return collClasses.GetIndex(szName);
	
}

char const* CObjectFactory::GetMethodName(int nMethIndex, int bAlias)const
{
	if (nMethIndex < nMethodsCount)
	{
		return CMyContextBase::GetMethodName(nMethIndex, bAlias);
	}

    return collClasses[nMethIndex - nMethodsCount]->GetName();
}

int CObjectFactory::GetNParams(int nMethIndex)const
{
	if (nMethIndex < nMethodsCount)
	{
	    return CMyContextBase::GetNParams(nMethIndex);
	}
    
	return static_cast<CClassItem*>(collClasses[nMethIndex - nMethodsCount])->m_nCtorParamsCount;
}

int CObjectFactory::GetParamDefValue(int nMethIndex, int nParamIndex, CValue * pValue)const
{
	if (nMethIndex < nMethodsCount)
	{
	    return CMyContextBase::GetParamDefValue(nMethIndex, nParamIndex, pValue);
	}

    pValue->Reset();
	pValue->SetTypeCode(UNSPECIFIED_TYPE_1C);
    return 1;
}

int CObjectFactory::HasRetVal(int nMethIndex)const
{
	if (nMethIndex < nMethodsCount)
	{
	    return CMyContextBase::HasRetVal(nMethIndex);
	}
    return 1;
}

int CObjectFactory::CallAsProc(int nMethIndex, CValue** ppValues)
{
	if (nMethIndex < nMethodsCount)
	{
	    return CMyContextBase::CallAsProc(nMethIndex, ppValues);
	}
    return 0;
}

int CObjectFactory::CallAsFunc(int nMethIndex, CValue& rValue, CValue** ppValues)
{
	if (nMethIndex < nMethodsCount)
	{
	    return CMyContextBase::CallAsFunc(nMethIndex, rValue, ppValues);
	}

    CClassItem* pClassItem = static_cast<CClassItem*>(collClasses[nMethIndex - nMethodsCount]);

	// ����� ���� ������ ������ !
	// ������ �������� ������� - +1 IncrRef, rValue.AssignContext - +1 IncrRef
	// � ����� ������� ������ ����� 2, � ������ �������� � ������ !!
	// ������ �������� ������� - +1 IncrRef, rValue.AssignContext - +1 IncrRef
    //CBLContext* pBLContext = static_cast<CBLContext*>(pClassItem->m_pRuntimeClass->CreateObject());
     CBLPtr<CBLContext> pCont(*pClassItem->m_pRuntimeClass);
 	CBLContext* pBLContext = pCont;
	
    pBLContext->InitObject(pClassItem->GetName());
    
    BOOL bSuccess;

    if (pBLContext->IsKindOf(RUNTIME_CLASS(CComponentClass)))
    {
        int nIndex = pBLContext->FindMethod("_Create");
        if (nIndex != -1)
        {
            int nParamCount = pBLContext->GetNParams(nIndex);
            if (nParamCount == pClassItem->m_nCtorParamsCount)
                bSuccess = pBLContext->CallAsProc(nIndex, ppValues);
            else
                RuntimeError("�� ��������� ���������� ����������.");
        }
        else
            RuntimeError("�� ������ �����������.");
    }
    else
        //if object is internal, then he must be inherited by CMyContextBase
        bSuccess = static_cast<CMyContextBase*>(pBLContext)->_Create(ppValues);

    if (bSuccess)
        rValue.AssignContext(pBLContext);
    else
    {
        rValue.Reset();
        //pBLContext->DecrRef(); // ��� ������������� CBLPtr �� �����
    }

    return bSuccess;
}

BOOL CObjectFactory::funcNewObject(int nParamsCount, CValue &RetVal, CValue **ppValue)
{
	CValue& valTypeName = *ppValue[0];
	// TODO ����� �� ��������. ��� ��� �������� �������� ����� ������� !
	//ValidateParamNotEmptyString(valTypeName, "����� : ���������� �������� ������ � ��������� ����");

	CString strTypeName = valTypeName.GetString();
	
	CString  strConstructorName;
	if (nParamsCount >= 2)
	{
		// TODO ����� �� ��������. ��� ��� �������� �������� ����� ������� !
		//ValidateParamNotEmptyString(*ppValue[1], "����� : ���������� �������� ������ � ��������� ������������");
		strConstructorName = ppValue[1]->GetString();

		CComponentClass::SetFlagCreateWithoutConstructor();
	}

	CBLPtr<CBLContext> pCont;

	{ // ����. ���� ��� �������
		CComponentClass::GuardOfClearFlagCreateWithoutConstructor guard;

		pCont.Create(strTypeName); //CBLPtr<CBLContext> pCont(strTypeName);
	}

	//CComponentClass::ClearFlagCreateWithoutConstructor();

	if (!pCont)
		RuntimeError("�������.����� - ������ ��� �������� ������ %s", strTypeName.operator LPCTSTR());

	if (nParamsCount >= 2)
	{
		// ������� ��������� ����� ����� � �������-���������
		// ��� ���� ��������������� ��� ������ � ������� ���������, �� ������ � �������� ��� 1�++

		CValue** ppValueForCallMethod = &ppValue[2];

		if (IS_KINDOF_CComponentClass(pCont))
		{
			const IUserClassContext* pClassCont = static_cast<IUserClassContext*>(pCont.operator CBLContext*());
			pClassCont->CallUserConstructor(strConstructorName, nParamsCount-2, ppValueForCallMethod);
		}
		else
		{
			int iProc = pCont->FindMethod(strConstructorName);
			if (-1 == iProc)
				RuntimeError("� ������ <%s> �� ���������� ���������-������������ <%s>", strTypeName.operator LPCTSTR(), strConstructorName.operator LPCTSTR());

			if (pCont->HasRetVal(iProc))
				RuntimeError("� ������ <%s> �� ���������� ���������-������������ <%s>, �� ���� ����� �������", strTypeName.operator LPCTSTR(), strConstructorName.operator LPCTSTR());


			// TODO ����� ���� ��� ��� ������ ������ � ����������� �� ��������� - 
			//	��������, ��-�� ����� ������ ���� ������������::����_���������������������������������
			int nMethodParams = pCont->GetNParams(iProc);
			int nCallParams = nParamsCount - 2;
			
			// ��������, ��� �������� ������ ���������� ����������, � �������� �������� �� ���������
			if(nMethodParams < nCallParams)
				RuntimeError("%s::%s - ������� ����� ����������� ���������� ��� ������", strTypeName.operator LPCTSTR(), strConstructorName.operator LPCTSTR());
			
			std::vector<CValue*> ValueParams;
			int nUpperBound = nCallParams < nMethodParams ? nCallParams : nMethodParams;
			CValue** ppValueForCallMethod = ppValue + 2;

			ValueParams.reserve(nMethodParams);
			for(int i=0;i<nUpperBound;i++)
			{
				if((*ppValueForCallMethod)->type == -1)	// �������� �� ��������� // TODO ����������� �������� ������������ � -1 :(
				{
					if(!pCont->GetParamDefValue(iProc, i, *ppValueForCallMethod))
						RuntimeError("%s::%s - �������� ������������ �������� � %i", strTypeName.operator LPCTSTR(), strConstructorName.operator LPCTSTR(), i);
				}
				ValueParams.push_back(*ppValueForCallMethod++);
			}

			std::vector<CValue> pNewValues;

			if(nUpperBound < nMethodParams)
			{
				pNewValues.resize(nMethodParams - nUpperBound);
				CValue* pNewParams = &pNewValues[0];
				for(int i=nUpperBound; i<nMethodParams; i++)
				{
					if(!pCont->GetParamDefValue(iProc, i, pNewParams))
						RuntimeError("%s::%s - �������� ������������ �������� � %i", strTypeName.operator LPCTSTR(), strConstructorName.operator LPCTSTR(), i);
					ValueParams.push_back(pNewParams++);
				}
			}
			pCont->CallAsProc(iProc, &ValueParams[0]);
		}
	}

	RetVal.AssignContext(pCont);

	return TRUE;
}

//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//
//BOOL CObjectFactory::procDisableFastCreationAllContexts(CValue **ppValue)
//{
//	CCreateInstanceBoost::TheObject().TestDisableFastCreationAllContexts(ppValue[0]->GetNumeric());
//	return TRUE;
//}
//
//#endif
