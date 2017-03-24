#ifndef CREATION_BOOST_H_INCLUDED
#define CREATION_BOOST_H_INCLUDED

#pragma once

//#define TEST_FOR_COMPARE_CREATION_SPEED

/////////////////////////////////////////////////////////////////////////////
// CCreateInstanceBoost
//

// �����-�������� � ������������ - 
// �������� �������� ������� - �.�. ����� ������������ ����� �������������(����������)
// ����� ����� ������ ��� - ����������� ��������� ����������� ���� !
//	����� ��������� ��������� �������� ����� �������� � 1�
//
class CCreateInstanceBoost
{
	class CCreateInstanceBoostImpl;
public:	
	CBLContext* CreateInstanceWithoutBoosted(char const * szClassName);

	CCreateInstanceBoost();
	~CCreateInstanceBoost();

	static CCreateInstanceBoost& TheObject();

	void Init(void);
	void Destroy(void);

	void RegisterCBLContext(CRuntimeClass* pRuntimeClass, LPCSTR szClassName, BOOL bNativeRegistration);

	//void AddFastCreationObject(LPCSTR szClassName, LPCSTR szClassFastCreationString);

//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	void TestDisableFastCreationAllContexts(int param);
//#endif

private:
	//boost::scoped_ptr<CCreateInstanceBoostImpl> m_pImpl;
	CCreateInstanceBoostImpl* m_pImpl;
};

#endif