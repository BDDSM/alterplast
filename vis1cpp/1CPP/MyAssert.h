// MyAssert.h: interface for MyAssert
//
//	�����: ����� ������� aka artbear, 2008
//
//	��������� �������� � �������� assert, MFC-ASSERT, ATLASSERT � �.�. ��� ������������� � ���������� Visual �++ 6.0 � 1�
//	��� ����� ����� ASSERT, VERIFY � �.�. �� MFC � ATL �� ��������
//
//	��������: 
//	1. ��� ����������� ������������� ����� � ����� ������ StdAfx.h ������� #include "1CPP\\MyAssert.h"
//	���� �� #include ��������� ������
//	����� �� ����� �������� Assert-� � ��������� ��������
//
//	2. � ���������� debug-������ ���������� ������/��������� ���������� ������������� NDEBUG
//		� ���������� release-������ �� ����� ��������
//
//////////////////////////////////////////////////////////////////////

#include <afx.h>

#include <assert.h>

#ifdef NDEBUG
#else

	#undef ASSERT
	#undef VERIFY
	#undef ASSERT_VALID
	#undef DEBUG_ONLY

	#undef ATLASSERT

	#define ASSERT(exp) ( assert(exp) )

	#define VERIFY(f)          ASSERT(f)
	#define ASSERT_VALID(pOb)  ASSERT(pOb != NULL)
	#define DEBUG_ONLY(f)      (f)

	#define ATLASSERT(exp) ( assert(exp) )

#endif  /* NDEBUG */