//______________________________________________________________________________
//
// �����-��������� �� CBLContext, ������������ ����� CValue
//______________________________________________________________________________
//

#include "stdafx.h"
#include "blptr.h"
#include "SafeContext.h"
#include "MethodsWrapper.h"

bool CSafeModulePtr::bAlreadyInit = false; 

//CMapPtrToPtr CSafeModulePtr::m_mapOfBadModules; // ����� ��� ��������� �������

CSafeModulePtr::CModulesMap CSafeModulePtr::m_mapOfLinkModules; // ����� ��������� �������

// ����� �������, ������� �� ���� ������ ������� (CBLModuleWrapper)
CSafeModulePtr::CRefModules CSafeModulePtr::m_mapOfRefModules; // ����� ��������� �������

void CSafeModulePtr::Link(ConstTPtr pMod)
{
	if (!pMod)
		return;

	CVectorModulePtr* pVector = NULL;
	bool bNotLookup = !m_mapOfLinkModules.Lookup(pMod, pVector);
	if (bNotLookup || !pVector)
	{
//		if (!pVector)
//		{
			pVector = new CVectorModulePtr; // TODO ������� ������ � �������, ����� ��������� �� �����-�������
			m_mapOfLinkModules.SetAt(pMod, pVector);
//		}
	}
	pVector->push_back(this);
}

void CSafeModulePtr::Unlink()
{
	if (m_pMod)
	{
		CVectorModulePtr* pVector = NULL;
		if (m_mapOfLinkModules.Lookup(m_pMod, pVector))
		{
			// TODO �������� �� ���� ��������� ���������� CSafeModulePtr � ������� ��
			if (pVector)
				////remove_if(pVector->begin(), pVector->end(), bind2nd(equal_to<CSafeModulePtr*>(), this));
				//pVector->erase(remove(pVector->begin(), pVector->end(), this)); // TODO
				// ��� ����� ���������, remove ������ ������ �������������� ������. ������� erase !!
				pVector->erase(remove(pVector->begin(), pVector->end(), this), pVector->end()); 
		}
	}
}

typedef CBLModule* (CBLModule::*PF_CBLModuleConstructor1)(class CBLModule const &);
CDllMethodWrapper<PF_CBLModuleConstructor1> wrapperConstructor;

typedef void (CBLModule::*PF_CBLModuleConstructor2)(class CBLContext *,char const *);
CDllMethodWrapper<PF_CBLModuleConstructor2> wrapperConstructor2;

typedef CBLModule* (CBLModule::*PF_CBLModuleDestructor)(void);
CDllMethodWrapper<PF_CBLModuleDestructor> wrapperDestructor;

CBLModule* CSafeModulePtr::WrapCBLModuleConstructor1(class CBLModule const & module)
{
	CDllMethodWrapperGuard<PF_CBLModuleConstructor1> wrapper(wrapperConstructor);
	CBLModule* pMod = (CBLModule*) this;
	
//	m_mapOfBadModules.RemoveKey(pMod);

	m_mapOfLinkModules[pMod] = NULL;
//LogErr("WrapCBLModule7Constructor1 ������ %d", pMod);

	(pMod->*wrapper.GetOrigMethod())(module);

	return pMod;
}

CBLModule* CSafeModulePtr::WrapCBLModuleConstructor2(class CBLContext *pCont, char const * szString)
{
	CDllMethodWrapperGuard<PF_CBLModuleConstructor2> wrapper(wrapperConstructor2);
	CBLModule* pMod = (CBLModule*) this;

//	m_mapOfBadModules.RemoveKey(pMod);
	m_mapOfLinkModules[pMod] = NULL;
//LogErr("WrapCBLModule7Constructor2 ������ %d", pMod);

	(pMod->*wrapper.GetOrigMethod())(pCont, szString);

	return pMod;
}

void CSafeModulePtr::ClearModulePtr(CSafeModulePtr* ptr)
{
//LogErr("�� CSafeModulePtr ������ ������ %d", ptr->m_pMod);
	if (ptr)
		ptr->m_pMod = NULL;
}

void CSafeModulePtr::WrapCBLModuleDestructor(void)
{
	CDllMethodWrapperGuard<PF_CBLModuleDestructor> wrapper(wrapperDestructor);
	CBLModule* pMod = (CBLModule*) this;
	
//	m_mapOfBadModules[pMod] = NULL;

//LogErr("WrapCBLModule7Destructor ������ %d", pMod);

	CVectorModulePtr* pVector;
	if (m_mapOfLinkModules.Lookup(pMod, pVector))
	{
		// TODO �������� �� ���� ��������� ���������� CSafeModulePtr � �������� ��
		if (pVector)
			for_each(pVector->begin(), pVector->end(), ClearModulePtr);
		delete pVector; // TODO ������� ������ � �������, ����� ��������� �� �����-�������
		m_mapOfLinkModules.RemoveKey(pMod);
	}

	(pMod->*wrapper.GetOrigMethod())();
}

void CSafeModulePtr::InitHook()
{
	if (bAlreadyInit)
		return;

	HINSTANCE hBlang=GetModuleHandle("blang.dll");

	wrapperConstructor.DoWrap(hBlang, "??0CBLModule@@QAE@ABV0@@Z", (PF_CBLModuleConstructor1)WrapCBLModuleConstructor1);
	wrapperConstructor2.DoWrap(hBlang, "??0CBLModule@@QAE@PAVCBLContext@@PBD@Z", (PF_CBLModuleConstructor2)WrapCBLModuleConstructor2);
	
	wrapperDestructor.DoWrap(hBlang, "??1CBLModule@@UAE@XZ", (PF_CBLModuleDestructor)WrapCBLModuleDestructor);

	bAlreadyInit = true;
}

void CSafeModulePtr::DestroyHook()
{
	if (!bAlreadyInit)
		return;

	wrapperConstructor.RestoreOrigAddress();
	wrapperConstructor2.RestoreOrigAddress();
	
	wrapperDestructor.RestoreOrigAddress();

//	m_mapOfBadModules.RemoveAll();

}
// ��������� ���������� � ��������� ����������
void CSafeModulePtr::_ErrorNULLObject(LPCSTR szErrorString) const
{
	if (!szErrorString)
		szErrorString = "CSafeModulePtr : ������� ��������� � ��������� NULL"; // TODO ���������� ��� ��������� ������ ��� �������� ��������
//LogErr("%s", szErrorString);

	::RuntimeError("%s", szErrorString); // TODO � ���������� ����� ������� ������ ��� ����������

};
