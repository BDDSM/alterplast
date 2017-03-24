#include "stdafx.h"

#include "MyAboutWindowManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef FORMEX_VK_DEFINED
	const int iResID = IDD_ABOUT_FORMEX;
#else
	const int iResID = IDD_ABOUT_1CPP;
	extern HINSTANCE hDllInstance;
#endif

static LRESULT WINAPI AboutDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_CLOSE)
	{
		HWND hWnd = ::GetParent(hDlg);
		if(hWnd)
		{
			hWnd = ::GetParent(hWnd);
			if(hWnd)
			{
				hWnd = ::GetParent(hWnd);
				if(hWnd)
					::SendMessage(hWnd, uMsg, wParam, lParam);
			}
		}
	}
	else if(uMsg == WM_INITDIALOG)
	{
		return TRUE;
	}
	return FALSE;
}

CWnd* CMyAboutWindowManager::CreateOwnPage( CWnd* pOwnerWindow, const CRect& PageRect, UINT nID )
{
	CWnd* pNewOwnPage = new CWnd;
	if(pNewOwnPage->Create("STATIC","",WS_CHILDWINDOW,PageRect, pOwnerWindow, nID))
	{
		if(!m_bLogoShowed)
		{
			AfxInitRichEdit();
			HWND hWnd = ::CreateDialog(hDllInstance,MAKEINTRESOURCE(iResID), pNewOwnPage->GetSafeHwnd(), (DLGPROC)AboutDlgProc);
			if(hWnd)
			{
				CWnd* pWnd = CWnd::FromHandle(hWnd);
				CRect rect(11,11,77,77);
				m_picLogo.CreateEx(WS_EX_NOPARENTNOTIFY,"Static","",
					WS_VISIBLE|WS_CHILDWINDOW|WS_TABSTOP|WS_BORDER|
					SS_BITMAP|SS_CENTERIMAGE,
					rect,pWnd,0x10FF);
				m_picLogo.SetBitmap(::LoadBitmap(hDllInstance,MAKEINTRESOURCE(IDB_LOGO)));
				
				CString text;
				
				pWnd->GetDlgItemText(IDC_ABOUT_STATIC, text.GetBufferSetLength(250), 250);
				text.ReleaseBuffer();
				CString sVersion = ::GetFullVersion();
				text.Replace("_VersionInfo_", sVersion);
				pWnd->SetDlgItemText(IDC_ABOUT_STATIC, text);
				
				// ���������� � �������������
				m_cRich.SubclassDlgItem(IDC_ABOUT_RICHEDIT, pWnd);
				m_cRich.SetBackgroundColor(FALSE, ::GetSysColor(COLOR_BTNFACE));
				text = "{\\rtf1\\ansi\\deff0\\deftab720"
					"\\fs20\\b1\\ul1������������:\\b0\\ul0\\par\\fs10\\par"
					"\\b1������� ��������\\b0\\par ����� ������� Rainbow, ������������ ����� ������� 1�\\par"
					"\\b1Carlos Antollini\\b0\\par odbccore.cpp\\par"
					"\\b1������� ����� aka Deb\\b0\\par �������� ������ 1�++ � ������ ����������������, �����������������, �������, ���������������, ���������, MetaInfoClasses, ���������� ������� ������� � ������ ����� ODBC\\par"
					"\\b1������ ������\\b0\\par ������ DynaValue, DynaCrypt\\par"
					"\\b1������� Ը����� aka ����\\b0\\par ���������� FormEx\\par"
					"\\b1��������� �������\\b0\\par ����� TurboBL, Visual1C++\\par"
					"\\b1����� ������� aka ToyPaul\\b0\\par �������������� ������ ���������� ������� ������� � ������ ����� ODBC\\par"
					"\\b1������� ������� aka DmitrO\\b0\\par ��������� ���������� ������� ������� � ������ ����� ODBC, ���������� ����, ����� �������������\\par"
					"\\b1������ ������������ aka spock\\b0\\par ���������� ������� ������� � ������ ����� OLE DB\\par"
					"\\b1������� ����� aka ADirks\\b0\\par ����� ����������������������\\par"
					"\\b1����� ����� aka fez\\b0\\par ����������� �������� �������\\par"
					"\\b1����� ������� aka artbear\\b0\\par ��������� ���, ������� �����������������, ����� � �.�.\\par"
					"\\b0� ����� ����� ������������ ������������ ����-������������ 1�++\\par"
					"\\fs20\\par\\b1\\ul1����-�������:\\b0\\ul0\\par\\fs10\\par"
					//****************************************************************************************
					// ������ ������ ����-��������
					"���� ������� aka ob36\\par"
					"������� ������� aka Varelchik\\par"
					"������ ������� aka marida\\par"
					"������ ��������\\par"
					"������� ������� aka GavaH\\par"
					"\\par"
					// ����� ������ ����-��������
					//****************************************************************************************
					" }";
				m_cRich.SetWindowText(text);
				m_cRich.SetTargetDevice(NULL, 0);
				//m_pWrapperForAboutDlg->m_cRich.SetEventMask(ENM_KEYEVENTS|ENM_UPDATE);
				//pWnd->SetDlgItemText(IDC_ABOUT_RICHEDIT, text);
				
				m_cHyperLink.SubclassDlgItem(IDC_STATIC, pWnd);
				
				char buffer[MAX_PATH];
				::GetModuleFileName(hDllInstance,buffer,MAX_PATH);
				pWnd->SetDlgItemText(IDC_DLLPATH, buffer);
				
			}
			
			m_bLogoShowed = true;
		}
	}

	m_pOwnPage = pNewOwnPage;

	return pNewOwnPage;
}

void CMyAboutWindowManager::DestroyOwnPage()
{
	if(m_bLogoShowed)
	{
		// 		if(m_bEditShowed)
		// 		{
		// 			cEdit.DestroyWindow();
		// 			bEditShowed = false;
		// 		}
		m_picLogo.DestroyWindow();
		m_bLogoShowed = false;
	}
	m_cRich.UnsubclassWindow();
	m_cHyperLink.UnsubclassWindow();
	
	delete m_pOwnPage;
}

#ifdef NDEBUG

CWnd* CMyAboutWindowManagerExample::CreateOwnPage( CWnd* pOwnerWindow, const CRect& PageRect, UINT nID )
{
	CWnd* pNewOwnPage = new CWnd;
	if(pNewOwnPage->Create("STATIC","",WS_CHILDWINDOW,PageRect, pOwnerWindow, nID)) // nID = 0x3000
	{
		if(!m_bLogoShowed)
		{
			AfxInitRichEdit();
			HWND hWnd = ::CreateDialog(hDllInstance,MAKEINTRESOURCE(IDD_ABOUT_FORMEX), pNewOwnPage->GetSafeHwnd(), (DLGPROC)AboutDlgProc);
			if(hWnd)
			{
				CWnd* pWnd = CWnd::FromHandle(hWnd);
				CRect rect(11,11,77,77);
				m_picLogo.CreateEx(WS_EX_NOPARENTNOTIFY,"Static","",
					WS_VISIBLE|WS_CHILDWINDOW|WS_TABSTOP|WS_BORDER|
					SS_BITMAP|SS_CENTERIMAGE,
					rect,pWnd,0x10FF);
				m_picLogo.SetBitmap(::LoadBitmap(hDllInstance,MAKEINTRESOURCE(IDB_LOGO)));
				
				CString text;
				
				pWnd->GetDlgItemText(IDC_ABOUT_STATIC, text.GetBufferSetLength(250), 250);
				text.ReleaseBuffer();
				CString sVersion = ::GetFullVersion();
				text.Replace("_VersionInfo_", sVersion);
				pWnd->SetDlgItemText(IDC_ABOUT_STATIC, text);
				
				// ���������� � �������������
				m_cRich.SubclassDlgItem(IDC_ABOUT_RICHEDIT, pWnd);
				m_cRich.SetBackgroundColor(FALSE, ::GetSysColor(COLOR_BTNFACE));
				text = "";
				m_cRich.SetWindowText(text);
				m_cRich.SetTargetDevice(NULL, 0);
				
				m_cHyperLink.SubclassDlgItem(IDC_STATIC, pWnd);
			}
			
			m_bLogoShowed = true;
		}
	}
	
	m_pOwnPage = pNewOwnPage;
	
	return pNewOwnPage;
}

#endif // #ifdef NDEBUG