#include "stdafx.h"
#include "WrapperForSyntaxCheck.h"

#include "MyModule.h"
#include "Syntax.h"

const unsigned int WM_SETMESSAGESTRING = 0x0362;	// wParam = nIDS (or 0),
													// lParam = lpszOther (or NULL)

const unsigned int WM_COMMAND_SYNTAXCHECK = 33297; // "�������������� �������� ��������� ������ ������\n�������������� ��������"

//const unsigned int iResId_String_SyntaxErrors = 29172; // 	// "��� �������� ������ ���������� �������������� ������!" ���
const unsigned int iResId_String_SyntaxCheckOnlyExtFilesOrReportInMD = 5914; // 	// "�������� ���������� ����������� ������ ��� �������, �������� � ������������, � ������� ������� �������!"

//const unsigned int iResId_String_FullSyntaxMDCheck = 33884; // "������ �������������� �������� ��� ���� �������\n������ �������������� ��������"

// artbear TODO ���� ���-�� ����� ������������ ������ �� �������-��������, 
// ����� ������ � �������������� ��� ��������
//
// � ����� �� ������������ ���������� 1� ��� �������-�������� �������� ��������� ��������� �����
// � ������������ ������� ( ���� �� WH_CallWndProc � CWinApp::PreTranslateMessage)
// ������� �������-�������� �� ���������������, � ������������ ������
// �.�. � ��������� �������, ������������� ����� ��� WH_CallWndProc, ������ ����������
// ������� ����� ���� ������ ��������, ��� ����������� ���������� ������� ������
//
//	���������� �������� ������� ���������� �� ������ �����
// ��� ������� ��� ����, ����� ����� �������-�������� ��������� �� ������� �� ���������
// ���� �������� �� ������ �������, ��������� ����� �������
// ��� ��������� ���������� 1� ��������� �� ����������
//	����� ���� ��������� ������, ����� �� ���� ��������, ������������� ��������� (��� CMainMessageHandler::WriteMessage)
//
//  �������� �������
// ����� ������� �����      8
// 	��������� ����. 8
// 	��������� ����� 8
// ����� ������� ���        8
// 	��������� ����. 8
// 	��������� ����� 8
// 	��������� ����� 7
// ����� ����������         8
// 	��������� ����. 8
// 	��������� ����� 8
// 	��������� ����� 7
//                       
// ��������� �������    7
// 	���������
// 
// ��������� ����������     7
// 	���������
// 	
// ����������               7
// 	??
// 	���������
// 	
// ������� �����  ??          ?
// 	���������
// 
// ���� ����������� ������, ��� ����� �������-�������� ������� ����� ����������� ���� ���1�++ 
// � ����� �������� �������� (� ����.������� � ��� ����)
// 

bool CWrapperForSyntaxCheck::bInsideCompileHandler = false;

CTrap<CWrapperForSyntaxCheck::TFunc_CBLModule7_Compile> CWrapperForSyntaxCheck::trCompile;

int CWrapperForSyntaxCheck::CallOrigCBLModuleCompile(CBLModule7* pMod)
{
	CTrapGuard<TFunc_CBLModule7_Compile> trap(trCompile);
	return pMod->Compile();
}

int CWrapperForSyntaxCheck::Wrap_CBLModule_Compile() //int CBLModule7::Compile()
{
	if (bInsideCompileHandler)
		return true;

	bInsideCompileHandler = true;

	CTrapGuard<TFunc_CBLModule7_Compile> trap(trCompile);

	CBLModule7* pMod = reinterpret_cast<CBLModule7*>(this);
	return CSyntax::ModuleCompile(pMod, bInsideCompileHandler);
}

HHOOK CWrapperForSyntaxCheck::WH_CallWndProc_HookHandle = 0;

// ��� ��������� ������� ������ "�������-��������" �� ������ "��������� ��������"
LRESULT __stdcall CWrapperForSyntaxCheck::WH_CallWndProc_Hook(int code, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
	CWPSTRUCT* pMsg = (CWPSTRUCT* )lParam;
	if(pMsg->message==WM_COMMAND && WM_COMMAND_SYNTAXCHECK == pMsg->wParam) // 0x0111 273 // �������-��������
	{
		//LogErr("CallWndProc_Hook WM_COMMAND wParam %d, code %d", pMsg->wParam, code);
		// MessageBox("CallWndProc_Hook WM_COMMAND wParam %d, code %d", pMsg->wParam, code);

		// TODO ����� ���������� ������������� ��������� (���� �� WH_CallWndProc � CWinApp::PreTranslateMessage)
		CSyntax::SyntaxCheck();
	}

	return CallNextHookEx(WH_CallWndProc_HookHandle, code, wParam, lParam); // �� ������������ ����
}

CWrapperForSyntaxCheck::PPRETRANSLATE CWrapperForSyntaxCheck::m_pPreTranslate;

// ��� ��������� ������� F4 - ������� � ��������� ������
//		TODO ���� ������ ���, ��������� ������ � ����
//
// ��� ��������� ������ "�������-��������" ����� ������ ���������
//
BOOL CWrapperForSyntaxCheck::PreTranslateMessage(MSG* pMsg)
{
	UINT message = pMsg->message;
	WPARAM wParam = pMsg->wParam;

	if ( WM_COMMAND_SYNTAXCHECK == wParam && 
		(message == WM_SETMESSAGESTRING || message == WM_COMMAND)
		)
	{
		//LogErr("pMsg->message %d", message);

		// TODO ����� ���������� ������������� ��������� (���� �� WH_CallWndProc � CWinApp::PreTranslateMessage)
		CSyntax::SyntaxCheck();
	}
	else
		if(message == WM_KEYDOWN)
		{
			if(LOWORD(wParam) == VK_F4)
			{
				SHORT stCtrl = GetKeyState(VK_CONTROL);
				if(!(stCtrl & 0x10000))
				{
					stCtrl=GetKeyState(VK_SHIFT);
					CSyntax::OpenNextError((stCtrl & 0x10000)!=0);
					return TRUE;
				}
			}
			/*if(LOWORD(wParam) == 'A')
			{
				SHORT stCtrl = GetKeyState(VK_CONTROL);
				if(stCtrl & 0x10000)
				{
					AfxGetMainWnd()->SendMessage(WM_COMMAND,33188,0);
					return TRUE;
				}
			}
			*/
		}

	return (((CWnd*)this)->*m_pPreTranslate)(pMsg);
}

CTrap<CWrapperForSyntaxCheck::TFunc_CMainMessageHandler_WriteMessage> CWrapperForSyntaxCheck::TrWrap_CMainMessageHandler_WriteMessage;

// �������� ������ � ���� ���������
void CWrapperForSyntaxCheck::Wrap_CMainMessageHandler_WriteMessage(char const * szString, enum MessageMarker marker, int i1)
{
	CTrapGuard<TFunc_CMainMessageHandler_WriteMessage> trap(TrWrap_CMainMessageHandler_WriteMessage);
	//LogErr("����������� ����� :<%s>, marker = <%d>, i1 = <%d>", szString, (int)marker, i1);

	if (!ResString(iResId_String_SyntaxCheckOnlyExtFilesOrReportInMD).CompareNoCase(szString))
		return;

	CMainMessageHandler* pMainMessageHandler = (CMainMessageHandler*) this;

	pMainMessageHandler->WriteMessage(szString, marker, i1);
}

void CWrapperForSyntaxCheck::SetHooks()
{
	trCompile.SetTrapOnFunc((TFunc_CBLModule7_Compile)&CBLModule::Compile, &Wrap_CBLModule_Compile);

	CWinApp* pApp = pMainApp;
	WH_CallWndProc_HookHandle = SetWindowsHookEx(WH_CALLWNDPROC, WH_CallWndProc_Hook, pApp->m_hInstance, pApp->m_nThreadID);

	DWORD* pVtable=*(DWORD**)pMainApp;
	pVtable+=0x60/4;
	DWORD old;
	VirtualProtect(pVtable,4,PAGE_EXECUTE_READWRITE,&old);
	PPRETRANSLATE pPre=(PPRETRANSLATE)&PreTranslateMessage;
	//DWORD* pNew=(DWORD*)&pPre,*pOld=(DWORD*)&m_pPreTranslate;
	DWORD* pNew=(DWORD*)&pPre;
	DWORD *pOld=(DWORD*)&m_pPreTranslate;
	*pOld=*pVtable;
	*pVtable=*pNew;

	TrWrap_CMainMessageHandler_WriteMessage.SetTrapOnFunc((TFunc_CMainMessageHandler_WriteMessage)&CMainMessageHandler::WriteMessage, &Wrap_CMainMessageHandler_WriteMessage);
	
}

void CWrapperForSyntaxCheck::DestroyHooks()
{
	
	trCompile.Swap();

	UnhookWindowsHookEx(WH_CallWndProc_HookHandle);

	TrWrap_CMainMessageHandler_WriteMessage.Swap();
	
}
