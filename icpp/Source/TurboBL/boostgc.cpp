// boostgc.cpp
#include "stdafx.h"
#include "boostgc.h"
#include "gcboost.h"


BEGIN_BL_METH_MAP(CBoostGC)
	BL_METH("Boost", "��������" , 0, &DoBoost, NULL, NULL)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CBoostGC)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CBoostGC, "BoosterGC", "������������", 1, NULL, NULL, -1);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBoostGC::CBoostGC()
{
}

CBoostGC::~CBoostGC()
{
}

int CBoostGC::DoBoost(CValue**)
{
	// ����� �������� ��� �������������, �� ������ �� ������
	return 1;
}
