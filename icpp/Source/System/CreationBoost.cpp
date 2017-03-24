#include "StdAfx.h"
#include "CreationBoost.h"
#include "MyAddinContext.h"

#include "../Interfaces/ICloneCreated.h"
using Utils1cpp::ICloneCreated;

struct ICreatedStrategy
{
	virtual ~ICreatedStrategy() {};
	virtual CBLContext* CreateObject(LPCSTR szClassName) = 0;
};

typedef ICreatedStrategy* ICreatedStrategyPtr;
typedef CIStringMap< ICreatedStrategyPtr, ICreatedStrategyPtr > CIStringMapToInfo;
typedef boost::shared_ptr<CIStringMapToInfo> CIStringMapToInfoPtr;

class CCreatedStrategyFromRuntimeClass : public ICreatedStrategy
{
private:
	CCreatedStrategyFromRuntimeClass(CRuntimeClass* pRuntimeClass): m_pRuntimeClass(pRuntimeClass)
	{ };
	CRuntimeClass* m_pRuntimeClass; // ��� ��������� �������� ����������

//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	bool m_bDontUseInitObject;
//	CCreatedStrategyFromRuntimeClass(CRuntimeClass* pRuntimeClass, bool bDontUseInitObject): m_pRuntimeClass(pRuntimeClass),
//		m_bDontUseInitObject(bDontUseInitObject)
//	{ };
//#endif

public:
	static ICreatedStrategyPtr Create(CRuntimeClass* pRuntimeClass)
	{ return ICreatedStrategyPtr(new CCreatedStrategyFromRuntimeClass(pRuntimeClass)); };

//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	static ICreatedStrategyPtr Create(CRuntimeClass* pRuntimeClass, bool bDontUseInitObject)
//	{ return ICreatedStrategyPtr(new CCreatedStrategyFromRuntimeClass(pRuntimeClass, bDontUseInitObject)); };
//#endif

	// ICreatedStrategy
	virtual CBLContext* CreateObject(LPCSTR szClassName) {
		CBLContext* pCont = static_cast<CBLContext*>(m_pRuntimeClass->CreateObject());
//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//		if (!m_bDontUseInitObject)
//#endif
			pCont->InitObject(szClassName);
		return pCont;
	}
};

//// ������� �������� ��������� ��������
//class CCreatedStrategyFromString : public ICreatedStrategy
//{
//private:
//	CCreatedStrategyFromString(LPCSTR szFastCreationString): m_szFastCreationString(szFastCreationString)
//	{};
//
//	// TODO ��� ������������ ����� ����� ������������ CString !
//	// ��� ������������� ����� (��������, ��������� � ������) ����� ������������ LPCSTR ��� ���������
//	//CString m_szFastCreationString; // ���� LPCSTR - ����� ������ �� �����������, � 1� ������
//	LPCSTR m_szFastCreationString; // ���� LPCSTR - ����� ������ �� �����������, � 1� ������
//
//	static CValue m_ValueForFastCreation;
//
//public:
//	static ICreatedStrategyPtr Create(LPCSTR szFastCreationString)
//	{ return ICreatedStrategyPtr(new CCreatedStrategyFromString(szFastCreationString)); };
//
//	// ICreatedStrategy
//	virtual CBLContext* CreateObject(LPCSTR szClassName)
//	{
//		LoadValueFromString(&m_ValueForFastCreation, m_szFastCreationString);
//		CBLContext* pCont = m_ValueForFastCreation.GetContext();
//
//		// ����� �������� �� ��� ������ ����� ������ �� ����.������
//		if (pCont)
//			pCont->IncrRef();
//
//		m_ValueForFastCreation.Reset(); // �������� ��������� ������
//		return pCont;
//	}
//
//};

class CCreatedStrategyFromCloneCreated : public ICreatedStrategy
{
private:
	CCreatedStrategyFromCloneCreated(ICloneCreated* pCloneCreatedContextObject, CBLContext* pContForCloneCreated): 
	   m_pCloneCreatedContextObject(pCloneCreatedContextObject), m_pContForCloneCreated(pContForCloneCreated)
	{ };

	CBLContext* m_pContForCloneCreated;
	ICloneCreated* m_pCloneCreatedContextObject;

public:
	static ICreatedStrategyPtr Create(ICloneCreated* pCloneCreatedContextObject, CBLContext* pContForCloneCreated)
	{ return ICreatedStrategyPtr(new CCreatedStrategyFromCloneCreated(pCloneCreatedContextObject, pContForCloneCreated)); };

	// ICreatedStrategy
	virtual CBLContext* CreateObject(LPCSTR szClassName)
	{
		return m_pCloneCreatedContextObject->Clone(szClassName);
	}

};

/////////////////////////////////////////////////////////////////////////////
// CCreateInstanceBoost
//

// �����-�������� � ������������ - 
// �������� �������� ������� - �.�. ����� ������������ ����� �������������(����������)
// ����� ����� ������ ��� - ����������� ��������� ����������� ���� !
//	����� ��������� ��������� �������� ����� �������� � 1�
//
class CCreateInstanceBoost::CCreateInstanceBoostImpl : public boost::noncopyable
{
private:
	typedef CBLContext* (*PF_CreateInstance)(char const *);

	CDllMethodWrapper<PF_CreateInstance> m_wrapperCreateInstance;

	CIStringMapToInfoPtr m_pClassNameHash;

	CCreateInstanceBoostImpl(CCreateInstanceBoostImpl&){};

	static CBLContext* CreateInstanceBoosted(char const * szClassName);
	CBLContext* Create(char const * szClassName);
	void AddCloneCreatedContext(ICloneCreated* pCloneCreatedContext, LPCSTR szClassName);

	void AutoGrowIfNeeded();

public:	
	
	CCreateInstanceBoostImpl() 
//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	: m_iTestDisable(0)
//#endif
	{};

	CBLContext* CreateInstanceWithoutBoosted(char const * szClassName);

	static CCreateInstanceBoostImpl& TheObject();

	void Init(void);
	void Destroy(void);

	void RegisterCBLContext(CRuntimeClass* pRuntimeClass, LPCSTR szClassName, BOOL bNativeRegistration);

	//void AddFastCreationObject(LPCSTR szClassName, LPCSTR szClassFastCreationString);

//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	void TestDisableFastCreationAllContexts(int param);
//
//private:
//	int m_iTestDisable;
//
//#endif
};

CCreateInstanceBoost& CCreateInstanceBoost::TheObject()
{
	static CCreateInstanceBoost object;
	return object;
}

CCreateInstanceBoost::CCreateInstanceBoostImpl& CCreateInstanceBoost::CCreateInstanceBoostImpl::TheObject()
{
	static CCreateInstanceBoostImpl object;
	return object;
}

CCreateInstanceBoost::CCreateInstanceBoost() : m_pImpl(&CCreateInstanceBoostImpl::TheObject())
{}

CCreateInstanceBoost::~CCreateInstanceBoost()
{}

//static LPCSTR szFastCreationValueList =  "{\"VL\",{}}";
//static LPCSTR szFastCreationTableList =  "{\"VT\",\"1\",{\"0\",{{\"\",\"0\",\"0\",\"0\",\"\",\"2\"}}}}";

//CValue CCreatedStrategyFromString::m_ValueForFastCreation;

void CCreateInstanceBoost::Init(void)
{
	m_pImpl->Init();
};

void CCreateInstanceBoost::CCreateInstanceBoostImpl::Init(void)
{
	m_pClassNameHash = CIStringMapToInfoPtr(new CIStringMapToInfo());

	m_wrapperCreateInstance.DoWrap(::GetModuleHandle("bkend.dll"), "?CreateInstance@CBLContext@@SAPAV1@PBD@Z", 
		(PF_CreateInstance)CCreateInstanceBoostImpl::CreateInstanceBoosted);

	// ������� �������� ���� �������� ������ ����������� ����� �������� CBLContext::InitObject
	//AddFastCreationObject("��������������", szFastCreationValueList);
	//AddFastCreationObject("���������������", szFastCreationTableList);
};

void CCreateInstanceBoost::Destroy(void)
{
	m_pImpl->Destroy();
}

void CCreateInstanceBoost::CCreateInstanceBoostImpl::Destroy(void)
{
	if (m_pClassNameHash)
	{
		m_pClassNameHash.reset();
		m_wrapperCreateInstance.RestoreOrigAddress();
	}
}

void CCreateInstanceBoost::RegisterCBLContext(CRuntimeClass* pRuntimeClass, LPCSTR szClassName, BOOL bNativeRegistration /*= TRUE*/)
{
	m_pImpl->RegisterCBLContext(pRuntimeClass, szClassName, bNativeRegistration);
}

void CCreateInstanceBoost::CCreateInstanceBoostImpl::AutoGrowIfNeeded()
{
	if (m_pClassNameHash->GetCount() * 1.2 > m_pClassNameHash->GetHashTableSize())
	{
		CIStringMapToInfoPtr pNewHash(new CIStringMapToInfo());
		pNewHash->InitHashTable((m_pClassNameHash->GetCount() + 11) * 1.2);
		POSITION pos = m_pClassNameHash->GetStartPosition();
		CString strCurName;
		ICreatedStrategyPtr pObject;
		while (pos)
		{
			m_pClassNameHash->GetNextAssoc(pos, strCurName, pObject);
			pNewHash->operator[](strCurName) = pObject;
		}

		m_pClassNameHash = pNewHash;
	}
}

void CCreateInstanceBoost::CCreateInstanceBoostImpl::RegisterCBLContext(CRuntimeClass* pRuntimeClass, LPCSTR szClassName, BOOL bNativeRegistration /*= TRUE*/)
{
	AutoGrowIfNeeded();
	
//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	bool bDontUseInitObject = m_iTestDisable = 3;
//#endif

	if (bNativeRegistration)
		CBLContext::RegisterContextClass(pRuntimeClass, szClassName, CType(100));

//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	ICreatedStrategyPtr pInfo(CCreatedStrategyFromRuntimeClass::Create(pRuntimeClass, bDontUseInitObject)); // TODO ������ ������� �� ��������� - ������� ������ � �������, ����� ����� �����-�������
//#else
	ICreatedStrategyPtr pInfo(CCreatedStrategyFromRuntimeClass::Create(pRuntimeClass)); // TODO ������ ������� �� ��������� - ������� ������ � �������, ����� ����� �����-�������
//#endif

	m_pClassNameHash->operator[](szClassName) = pInfo;
}

void CCreateInstanceBoost::CCreateInstanceBoostImpl::AddCloneCreatedContext(ICloneCreated* pCloneCreatedContextObject, LPCSTR szClassName)
{
	AutoGrowIfNeeded();

	CBLContext* pContForCloneCreated = pCloneCreatedContextObject->Clone(szClassName);
	assert(1 == pContForCloneCreated->m_RefCount);

	ICloneCreated* pNewCloneCreatedContextObject = dynamic_cast<ICloneCreated*>(pContForCloneCreated);
	assert(NULL != pNewCloneCreatedContextObject);

	ICreatedStrategyPtr pInfo(CCreatedStrategyFromCloneCreated::Create(pNewCloneCreatedContextObject, pContForCloneCreated)); // TODO ������ ������� �� ��������� - ������� ������ � �������, ����� ����� �����-�������
	m_pClassNameHash->operator[](szClassName) = pInfo;
}

//void CCreateInstanceBoost::AddFastCreationObject(LPCSTR szClassName, LPCSTR szClassFastCreationString)
//{
//	m_pImpl->AddFastCreationObject(szClassName, szClassFastCreationString);
//}
//
//// ��� �������� �������� ��������� ��������
//void CCreateInstanceBoost::CCreateInstanceBoostImpl::AddFastCreationObject(LPCSTR szClassName, LPCSTR szClassFastCreationString)
//{
//	ICreatedStrategyPtr pInfo(CCreatedStrategyFromString::Create(szClassFastCreationString)); // TODO ������ ������� �� ��������� - ������� ������ � �������, ����� ����� �����-�������
//	m_pClassNameHash->operator[](szClassName) = pInfo;
//}

CBLContext* CCreateInstanceBoost::CreateInstanceWithoutBoosted(char const * szClassName)
{
	return m_pImpl->CreateInstanceWithoutBoosted(szClassName);
}

CBLContext* CCreateInstanceBoost::CCreateInstanceBoostImpl::CreateInstanceWithoutBoosted(char const * szClassName)
{
	CDllMethodWrapperGuard<PF_CreateInstance> wrapper(TheObject().m_wrapperCreateInstance);

	return CBLContext::CreateInstance(szClassName);
}

// �������� �������� ������� - �.�. ����� ������������ ����� �������������(����������)
// ����� ����� ������ ��� - ����������� ��������� ����������� ���� !
//	����� ��������� ��������� �������� ����� �������� � 1�
//
CBLContext* CCreateInstanceBoost::CCreateInstanceBoostImpl::CreateInstanceBoosted(char const * szClassName)
{
	return TheObject().Create(szClassName);
}

CBLContext* CCreateInstanceBoost::CCreateInstanceBoostImpl::Create(char const * szClassName)
{
//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	if (1 == m_iTestDisable)
//		return CreateInstanceWithoutBoosted(szClassName);
//#endif

	ICreatedStrategyPtr pInfo;

	if (m_pClassNameHash->Lookup(szClassName, pInfo))
		return pInfo->CreateObject(szClassName);

	CBLContext* pBLContext = CreateInstanceWithoutBoosted(szClassName);

	if (!pBLContext)
		return NULL;

	int iIsOleContext = pBLContext->IsOleContext();

// TODO ������ � ����������� ������������ ICloneCreated �������� ��������� �� ���������� ��������� ��������� � �������,
// ��������, ��� ������� ���
//	if (!iIsOleContext)
////#ifdef TEST_FOR_COMPARE_CREATION_SPEED
////	if (0 == m_iTestDisable)
////#endif
//	{
//		ICloneCreated* pCloneCreatedContext = safe_dynamic_cast<ICloneCreated*>(pBLContext);
//		if (NULL != pCloneCreatedContext)
//		{
//			AddCloneCreatedContext(pCloneCreatedContext, szClassName);
//			return pBLContext;
//		}
//	}

	const int iCAddInContextLen = strlen("Addin.");
	const LPCTSTR szCAddIn = "Addin.";

	// �������� ������� �������� ��, ��������� �� ������� ���������� �������� ��, �� ���� ������-������
	// ������ �������� ���� �� ������, �.�. ���� ��� ���������� ������ ���� ��� ��� �������� ������� ������ ����
	// ����� ������������ InitObject - ��. ����
	// ����� ��� ��� ����������� ����������� ������������ ����������� ������� �� �������� ��������
	if (!strnicmp(szClassName, szCAddIn, iCAddInContextLen))
		return CAddInContext_MyProxy::CreateWithNewObject(szClassName, static_cast<CAddInContext*>(pBLContext));

	if (!iIsOleContext)
		RegisterCBLContext(pBLContext->GetRuntimeClass(), szClassName, FALSE);

	return pBLContext;
}

//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//
//void CCreateInstanceBoost::TestDisableFastCreationAllContexts(int param)
//{
//	m_pImpl->TestDisableFastCreationAllContexts(param);
//}
//
//void CCreateInstanceBoost::CCreateInstanceBoostImpl::TestDisableFastCreationAllContexts(int param)
//{
//	m_iTestDisable = param;
//	m_pClassNameHash->RemoveKey("����������������������");
//	m_pClassNameHash->RemoveKey("���������");
//}
//
//#endif