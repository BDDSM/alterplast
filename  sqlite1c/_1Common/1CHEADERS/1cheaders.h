/*--------------------------------------------------------------------------------
1cheaders.h
������ ���� ������������ ��� ��� ��������� � stdafx.h ������������� ��������,
����������� � ������� � 1� dll'���.
--------------------------------------------------------------------------------*/

#pragma once

// ����������� ������������ �����
#include <afxcview.h> 
#include <comdef.h>
#include <afxctl.h>
#include <afxtempl.h>
#include <functional>
#include <set>
#include <vector>
#include <map>

// ������� ������ 1� �������
#define IMPORT_1C __declspec(dllimport)
#include "type32.h"
#include "types.h"
#include "br32.h"
#include "bkend.h"
#include "frame.h"
#include "blang.h"
#include "userdef.h"
#include "seven.h"
#include "editr.h"
#include "basic.h"
#include "dbeng32.h"
#include "moxel.h"
#include "rgproc.h"
#include "txtedt.h"

#include "ext_header.h"

using namespace std;

// ������� ��������� �������� ���������� ��������
__declspec(selectany) CApp7*			pMainApp		= NULL;
__declspec(selectany) CBkEndUI*			pBkEndUI		= NULL;
__declspec(selectany) CMetaDataCont*	pMetaDataCont	= NULL;
//__declspec(selectany) SECMDIFrameWnd*	pMainFrame		= NULL;
__declspec(selectany) CMainFrame*		pMainFrame		= NULL;
__declspec(selectany) CDataBase7*		pDataBase7		= NULL;
__declspec(selectany) HINSTANCE			h1CResource		= NULL;
__declspec(selectany) HINSTANCE			hMyInst			= NULL;

void Init1CGlobal(HINSTANCE hInst);

// � ��������� �������� �������

// ����� ������ � ������ ��������� (���������)
void DoStsLine(LPCSTR format, ...);

// �������� �������� ������ Windows �� �� ����. ��� err==0, ������ �������� ��� GetLastError
CString GetErrorDescription(DWORD err = 0);

//����������� �������-����������
void RuntimeError(LPCSTR format, ...);
void RuntimeErrorRes(UINT nID);

// ����������������()
__forceinline const CString& BinDir()		{return pMainApp->GetProps()->GetStringProp(0);}
// ���������()
__forceinline const CString& IBDir()		{return pMainApp->GetProps()->GetStringProp(1);}
// �������������������()
__forceinline const CString& UserDir()		{return pMainApp->GetProps()->GetStringProp(2);}
// ����������������������()
__forceinline const CString& TempDir()		{return pMainApp->GetProps()->GetStringProp(3);}
// ��� ����
__forceinline const CString& IBName()		{return pMainApp->GetProps()->GetStringProp(0x0C);}
// ���������������()
__forceinline const CString& UserName()		{return pMainApp->GetProps()->GetStringProp(0x0D);}
// ���������������������()
__forceinline const CString& FullUserName()	{return pMainApp->GetProps()->GetStringProp(0x0E);}
// ������������������()
__forceinline const CString& RightsName()	{return pMainApp->GetProps()->GetStringProp(0x10);}
// ������������������()
__forceinline const CString& GUIName()		{return pMainApp->GetProps()->GetStringProp(0x11);}

