// Silenser.h: interface for the CSilenser class.
#ifndef AFX_SILENSER_H
#define AFX_SILENSER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSilenser:public CFileDialog
{
public:
	static void Done();
	static void SetSilense(bool bSilence);
	static bool IsSilence();
	// �������� ������ ��������
	typedef int (CSilenser::*PDoModal)();
	int DoModalDialog();
	int OnInitDlgTrap();
	// �������� ������ �������� ��������
	int DoModalFileDialog();

	// �������� ������ �������
	static PDoModal	 m_pFDlgDoModal;
	static PDoModal  m_pDlgDoModal;
	// �������� AfxMessageBox
	typedef int (AFXAPI *PAfxMessageBoxT)(LPCTSTR,UINT,UINT);
	static int AFXAPI AfxMessageBoxT(LPCTSTR,UINT,UINT);
	// ������ (��������) ���� �������
	static PAfxMessageBoxT m_pAfxMsgBoxT;

	static void Init();
};

#endif
