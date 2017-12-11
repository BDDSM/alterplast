// config.h
// ������� ������ ����������
#ifndef CONFIG_H
#define CONFIG_H

// ��� ��������� �� �������-���������� �������� ���� 
typedef void(*HANDLER_ON_CREATE_WND)(CWnd*);
// ���� ������� �������������
#define CFG_ON_CREATE_MAIN_WND		0x1		// �������� ��������� ����
#define CFG_ON_CREATE_CFG_WND		0x2		// �������� ���� "������������"

// ��������� ������, ���������������� ������
class CConfigService{
public:
	virtual CString IBDir()=0;
	virtual CString BinDir()=0;
	virtual CDocument* OpenFile(CString path)=0;
	virtual bool AddConfigTab(CString name,HICON hIcon,CRuntimeClass* pRTCDoc,CRuntimeClass* pRTCView)=0;
	virtual void AddConfigEventHandler(WORD wEventType,void* pHandler)=0;
	virtual bool LoadModule(CString,CString&)=0;
	virtual void LoadToolBar(HINSTANCE hInst,UINT nID,LPCTSTR lpszName,int count)=0;
	virtual void AddCmdTarget(CCmdTarget* pTarget)=0;
	virtual void RemoveCmdTarget(CCmdTarget* pTarget)=0;
	virtual void AddDynLink(AFX_EXTENSION_MODULE& pModule)=0;
	virtual bool RegisterPanelClass(LPCTSTR lpszName,CRuntimeClass* pWndClass,HICON hIcon)=0;

};
/*
��� ��������� � ������ ���� dll ������ �������������� ������� "SetConvigService"
extern "C" _declspec(dllexport) void SetConvigService(void* p);
���� ����� ������� ����� ������� � dll, ��������� �������
�� � �������� ��������� �� ������ ������ CConfigService, �����
������� � ����� ����� �������� �������.
�� ������ ������ ��������������� ��������� ������:

CString IBDir();
���������� ������� ���� ������ (�� ������ � �����)

CString BinDir();
���������� ������� ���������� ������ (�� ������ � �����)

CDocument* OpenFile(CString path);
��������� ���� � �������������.
���������� ��������� �� �������� �����.
��� ert � mxl - CWorkBookDoc,
��� ��������� - CBodyTextDoc.
���������:
path - ���� � �����.

bool AddConfigTab(CString name,HICON hIcon,CRuntimeClass* pRTCDoc,CRuntimeClass* pRTCView);
������� �������� � ���� ������������.
���������� true � ������ ��������� ��������, ����� - false.
���������:
name		- ������� ��������
hIcon		- ������ ��������
pRTCDoc		- ��������� �� CRuntimeClass ���������.
pRTCView	- ��������� �� CRuntimeClass �����������.
� �������� �������� ��������� ��������� ������ view'� � ����������.

void AddConfigEventHandler(WORD wEventType,void* pHandler);
���������������� �������-����������� ��������� ������� � �������������.
���������:
wEventType	- ��� �������
pHandler	- ��������� �� �������-����������.
�� ������ ������ ���� ���������� ��� ���� �������:
CFG_ON_CREATE_MAIN_WND	- �������� ��������� ���� �������������.
CFG_ON_CREATE_CFG_WND	- �������� ���� "������������".
��� ����� ������� �������-���������� ������ ����� ���
void (CWnd*)
� �������� ��������� ���������� ��������� �� CWnd ������������ ����.
*/
#endif/////////////////////////////////////////////////////////////////////////////
// IConfigurator wrapper class

class IConfigurator : public COleDispatchDriver
{
public:
	IConfigurator() {}		// Calls COleDispatchDriver default constructor
	IConfigurator(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IConfigurator(const IConfigurator& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetIBName();
	void SetIBName(LPCTSTR lpszNewValue);
	CString GetIBDir();
	CString GetBinDir();
	CString GetCommandLine();
	void Message(LPCTSTR msgLine, long marker);
	void Status(LPCTSTR text);
	LPDISPATCH OpenOLEForm(LPCTSTR progID, LPCTSTR Title);
	long GetVersion();
	LPDISPATCH GetMetaData();
	void SendCommand(long cmd);
	BOOL IsConfigWndOpen();
	LPDISPATCH GetPlugins();
	LPDISPATCH GetScripts();
	LPDISPATCH GetWindows();
	void Quit(BOOL PromptSave);
	void LogMessageWrite(long EventCategory, LPCTSTR Message, LPCTSTR strEventType, LPCTSTR Event);
	void Run1CApp(long mode);
	LPDISPATCH GetDocuments();
	LPDISPATCH GetCfgTimer();
	CString GetAppProps(long idx);
};
