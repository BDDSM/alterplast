// TelePanel.h : header file
#ifndef AFX_TELEPANEL_H
#define AFX_TELEPANEL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTelePanel window
// POPUP ����, �������� ��� ������ �������/����������
// ��������� ���:
// 1 - ������ ����������� ��� MFC ��������� SelfOwnerDraw ��� ������
// (������ ��� �������� ��� ������ ��� �������� ����)
// 2 - ��� ������ POPUP ���� ������ ���� ��������� ���������
// ���� �������� �� "����������".
// ��� ���������� ����� � OnNcActivate �������� WM_NCACTVATE ��������� ����
class CTelePanel : public CWnd
{
// Construction
public:
	CTelePanel();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTelePanel)
	//}}AFX_VIRTUAL
// Implementation
public:
	// Generated message map functions
protected:
	//{{AFX_MSG(CTelePanel)
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
