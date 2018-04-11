// RMsgWindow.cpp: implementation of the CRMsgWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RMsgWindow.h"

/////////////////////////////////////////////////////////////////////////////
BEGIN_BL_METH_MAP(CRMsgWindow) 
    RBL_METH(Message,		"��������",			3,	NULL)
//.<destination>���������� ���� ��������� �� ������.
//.<syntax>��������(���������������, ��������������, ����������������������);
//.<param>
	//.<@name>���������������
	//.<@destination>���������� ����� ���������.
	//.<@optional>1
	//.<@type>������
	//.<@default>������ ������
	//.<@values>
//.<param>
	//.<@name>��������������
	//.<@destination>����� � ��������� ���� ���������.
	//.<@optional>1
	//.<@type>������
	//.<@default>"���������"
	//.<@values>
//.<param>
	//.<@name>����������������������
	//.<@destination>������� ������������ ������ ���������.
	//.<@optional>1
	//.<@type>�����
	//.<@default>2 - �� ������
	//.<@values>1 - �� �����, 2 - �� ������, 3 - �� ������ ������� ����
//.<descr>���������� ���� ��������� �� ������. ���� ���� �� ������ ������ ���� ������, ��� ���������� �� ������. ���� ���� ��� �� ������, �� �������� ����� ���������, ��������� ����, ������������ � ������������ � ����������� �����������.
//.<sample>�����=�������������("RMsgWindow");
//.<sample>�����.��������("������!", "�������������� ���������", 1);
    RBL_METH(Close,			"�������",			0,	NULL)
//.<destination>��������� ���� ���������.
//.<syntax>�������();
//.<descr>��������� ���� ���������, �� �� ���������� ���. ��� ��������� ����������� ���� �� ������ ����� ������������ ����� ��������� ��������� ��������� ����, ������������, ������� ��������� � ���������.
    RBL_METH(Move,			"�����������",		4,	NULL)
//.<destination>���������� ���� ��������� � ��������� ������� � ������������� ��������� ������.
//.<syntax>�����������(�����, �����, �������, �������);
//.<return>
//.<param>
	//.<@name>�����
	//.<@destination>���������� ������ ������� ���� ���� ��������� �� �����������.
	//.<@optional>0
	//.<@type>�����
	//.<@default>
	//.<@values>0 - ������ ������ �� ������
//.<param>
	//.<@name>�����
	//.<@destination>���������� ������ ������� ���� ���� ��������� �� ���������.
	//.<@optional>0
	//.<@type>�����
	//.<@default>
	//.<@values>0 - ������ ������ �� ������
//.<param>
	//.<@name>�������
	//.<@destination>������ ���� ���������.
	//.<@optional>0
	//.<@type>�����
	//.<@default>
	//.<@values>0 - ...
//.<param>
	//.<@name>�������
	//.<@destination>������ ���� ���������.
	//.<@optional>0
	//.<@type>�����
	//.<@default>
	//.<@values>0 - ...
//.<descr>������ ��������� ��������� � ����� ������� ����. ���������� �������� � ��������. �� ��������� ���� ��������� ������������ � �������� ������.
END_BL_METH_MAP() 

BEGIN_BL_PROP_MAP(CRMsgWindow)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CRMsgWindow, "RMsgWindow", "RMsgWindow", 1, NULL, NULL, -1);
//.<destination> ������ ���� RMsgWindow ��������� ���������� �� ������ ���� ��� ��������� ���� ��������� � ����������� �������������� ���. ���������� � ���� ��������� ������������ ������� ������ ��������� ��������� ���� 1�:����������� � ������������ ��������� ����� �� ������, ������ ��� ������� ����� ����. ���� ������������ �� ������ �� ��� ��� ���� �� ����� ������ ����� �������(), ��������� ��������� ��������� ���� 'RMsgWindow' ��� ���������� ������ �� ������� ��������� � ���� ����� ���������� �������������. ������� ���� �� ������ ���� �������, ��� ���� ��������� ������������� ����� ��������� ������ ������, ���� ���� ���������� ������ ����� �������� ������� ��� ������������� �������� 'Esc', ����������� ��������� ���������� �������� ��� ����������� ����������� �������� ���� 'RMsgWindow'.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRMsgWindow::CRMsgWindow()
{
	msg = new CDlgMsg;
	msg->Create(AfxGetApp()->GetMainWnd()->GetSafeHwnd());
	msg->CenterWindow(CWnd::GetDesktopWindow()->GetSafeHwnd());
	hwndText = GetDlgItem(msg->m_hWnd, IDC_Text);
}

CRMsgWindow::~CRMsgWindow()
{
	msg->DestroyWindow();
	delete msg;
}

BOOL CRMsgWindow::funcMessage(CValue& RetVal, CValue **params)
{
	if(!msg->IsWindowVisible()) msg->ShowWindow(SW_SHOW);
	if(!params[1]->GetString().IsEmpty()) SetWindowText(msg->m_hWnd, params[1]->GetString());
	if((params[2]->GetNumeric()>0) && (params[2]->GetNumeric()<4)) 
	{
		::SetWindowLong(hwndText, GWL_STYLE, (::GetWindowLong(hwndText, GWL_STYLE)&~SS_CENTER&~SS_LEFT&~SS_RIGHT)|long(params[2]->GetNumeric())-1);
		::SetWindowPos(hwndText, 0, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER);
	};
	SetWindowText(hwndText, params[0]->GetString());
	msg->m_message = params[0]->GetString();
	
	return TRUE;
}

BOOL CRMsgWindow::funcClose(CValue& RetVal, CValue **params)
{
	msg->ShowWindow(SW_HIDE);
	
	return TRUE;
}

BOOL CRMsgWindow::funcMove(CValue& RetVal, CValue **params)
{
	RECT r;
	msg->GetWindowRect(&r);
	r.left	= params[0]->GetNumeric();
	r.top		= params[1]->GetNumeric();
	r.right	= r.left + params[2]->GetNumeric();
	r.bottom	= r.top + params[3]->GetNumeric();
	msg->MoveWindow(&r);
	
	return TRUE;
}

