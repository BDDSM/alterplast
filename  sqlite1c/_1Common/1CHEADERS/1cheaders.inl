/*
1cheaders.inl
������ ���� �������� ����������� ��������� ���������� ��������,
����������� ������ ������������ � 1�-��������, � ��������� ��������
�������.
������������� ���������� � ����������� lib'�/
����� ����� �������� ��� � stdafx.cpp � ������ ��������
*/

// ����� ������� �������� ���������
#pragma comment(linker,"/FILEALIGN:512")

// ��������� ����
// ���� � ����� ������ ��������� �� �������� �������� �������, � �� �� �������� ����� �����
// �� ��������� ������� ������� �� ����� ������ � ��������� _1Common
// ���� ��� �� ���, �� ����� ���������� 1cheaders.inl ����� ���������� ������ HEADER1C_PATH
#ifndef HEADER1C_PATH
	#define HEADER1C_PATH "../_1Common/1cheaders"
#endif

#pragma comment (lib,HEADER1C_PATH "/libs/basic.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/bkend.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/blang.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/br32.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/dbeng32.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/editr.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/frame.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/moxel.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/rgproc.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/seven.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/txtedt.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/type32.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/userdef.lib")

void DoStsLine(LPCSTR format,...)
{
	va_list arg;
	va_start(arg,format);
	CString txt;
	txt.FormatV(format,arg);
	pBkEndUI->DoStatusLine(txt);
}

CString GetErrorDescription(DWORD err/*=0*/)
{
	if(!err)
		err=GetLastError();
	CString errMsg;
	errMsg.Format("��� ������ Windows: 0x%X",err);
	LPTSTR lpMsgBuf;
	if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,err,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf,0,NULL))
	{
		errMsg=errMsg+"\r\n"+lpMsgBuf;
		LocalFree(lpMsgBuf);
	}
	return errMsg;
}

// ������������� �������� ���������� ����������
void Init1CGlobal(HINSTANCE hInst)
{
	pMainApp		= (CApp7*)AfxGetApp();
	pBkEndUI		= GetBkEndUI();
	pMetaDataCont	= GetMetaData();
	pDataBase7		= CDataBase7::GetDefault();
	h1CResource		= AfxGetResourceHandle();
	hMyInst			= hInst;
	// � ������ ������ ������������� ��� �������� �������
	// �������� ���� ��� �� �������, pMainFrame ���� �����-��������
	// �������� ���� ���������� � ������� ON_CREATE_MAIN_WND
	pMainFrame		= (CMainFrame*)pMainApp->m_pMainWnd;
//	pMainFrame		= (SECMDIFrameWnd*)pMainApp->m_pMainWnd;
}

void RuntimeError(LPCSTR format, ...)
{
        va_list arg;
        va_start(arg, format);
        CString txt;
        txt.FormatV(format, arg);
        va_end(arg);
        CBLModule::RaiseExtRuntimeError((LPCSTR)txt, mmRedErr);
}

void RuntimeErrorRes(UINT nID)
{
	CString err;
	err.LoadString(nID);
	CBLModule::RaiseExtRuntimeError(err, mmRedErr);
}
