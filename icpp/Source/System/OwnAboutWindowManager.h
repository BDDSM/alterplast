//////////////////////////////////////////////////////////////////////
//
// �����������: ����� ������� aka artbear, mailto: artbear@inbox.ru
//
//////////////////////////////////////////////////////////////////////
//
// ��������� IOwnAboutWindowManager - ��� ����������� ����������� ����������� ���� "� ���������" � ����������� ���� 1� "� ���������"
//
// �����: �������� �������� �� ����������� ���������� ������� ������� ��� ������ ���� (���� ��������� ��� AboutDlgProc)
//
// ������ ����������� ������������ ���� ( ������ ���� ��������� 1�++ ��� FormEx )
//	
// 	CBLPtr<CBLContext> pCont;
// 	try
// 	{
// 		LPCSTR szClassName = "�����������";
// 		pCont.Create(szClassName);
// 	}
// 	catch(...)
// 	{
// 		try
// 		{
// 			LPCSTR szClassName = "������";
// 			pCont.Create(szClassName);
// 		}
// 		catch(...)
// 		{
// 		}
// 	}
// 
// 	if (pCont)
// 		CBLContext* pCont2 = pCont;
// 		IAddOwnAboutWindowManager *pAddOwnAboutWindowManagerContext = dynamic_cast<IAddOwnAboutWindowManager *>(pCont2);
// 		if (pAddOwnAboutWindowManagerContext)
// 			pAddOwnAboutWindowManagerContext->Add(GetOwnAboutWindowManager()); // ����������� ������ !
// 	}
// 
//
// �����: ������� ������� ������ ���� ������ ��������� ��������� �������
//
// LRESULT WINAPI AboutDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
// {
// 	if(uMsg == WM_CLOSE)
// 	{
// 		HWND hWnd = ::GetParent(hDlg);
// 		if(hWnd)
// 		{
// 			hWnd = ::GetParent(hWnd);
// 			if(hWnd)
// 			{
// 				hWnd = ::GetParent(hWnd);
// 				if(hWnd)
// 					::SendMessage(hWnd, uMsg, wParam, lParam);
// 			}
// 		}
// 	}
// 	else if(uMsg == WM_INITDIALOG)
// 	{
// 		return TRUE;
// 	}
// 	return FALSE;
// }

#if !defined(_OWN_ABOUT_WINDOW_MANAGER__INCLUDED_)
	#define _OWN_ABOUT_WINDOW_MANAGER__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IOwnAboutWindowManager
{
public:
	// ����� ��������
	virtual LPCSTR GetTabName() = 0;
	
	// ���������� ����� ��������, ������������ ����� � ������� ��������
	// ���� ������� NULL, �������� �� �����
	// ���� �� NULL, �� ����� ����� ������ ����� ���������� ������� ������, �.�. ���������� ������� �� ������� ����������
	virtual HBITMAP GetBitmapHandle() = 0; // ������ ���� ������������ ����� RGB(0x00,0x80,0x80)

	// ����� CreateOwnPage ������ ������� ����� new ���� CWnd ��� ��� ����������
	// ����� ��������� 	�������� ���� ����� ����� CWnd::Create()
	// ������ ����:
	// 	CWnd* pNewOwnPage = new CWnd;
	// 	if(pNewOwnPage->Create("STATIC","",WS_CHILDWINDOW, PageRect, pOwnerWindow, nID))
	//	����� ��� ���������� ���� ����������
	virtual CWnd* CreateOwnPage( CWnd* pOwnerWindow, const CRect& PageRect, UINT nID ) = 0;

	// ���������� ����, ����� ��������� � CreateOwnPage
	virtual CWnd* GetPage() = 0;

	// �������� ������ ����, � ���������� ������� ����� ������ ������ ��� ���������� ������ 1�
	virtual void DestroyOwnPage() = 0;
};

// ���� ��������� ����� ���������� �������� CService
class IAddOwnAboutWindowManager
{
public:
	// ���������� ��� ����������� �����, �� �� ������ �������� �� ������� ����� ��������� ������� !
	virtual void Add(IOwnAboutWindowManager* pOwnAboutWindowManager) = 0;
};

// ��������� IAddOwnAboutWindowManagerContext ������ �������������� ��� ���������� �������� ����������� ���� "� ���������"
// ���������� ��
// ������ ����� ������ CBLContext ��� ����������� ������������� ����������, ��������� � ������ ������ MS Visual C++
class IAddOwnAboutWindowManagerContext : public IAddOwnAboutWindowManager, public CBLContext
{
protected:
	virtual void Dummy() {};	
};


#endif