// configsvcimpl.h
#ifndef CONFIGSVCIMPL_H
	#define CONFIGSVCIMPL_H

#include "config.h"

// options
//#define LOG_MSG
//#define CHANGE_TITLE



extern HINSTANCE g_hInst;
void DoMsgLine(const char* format,MessageMarker marker=mmNone,...);
void DoStsLine(const char* format,...);
void ExtractMacrosFromDispatch(IDispatch* pDisp,CStringArray& names,CDWordArray& dispIDs);
void SplitStr2Array(const CString& str,CStringArray& arr,char delim='.');
CString GetErrorDescription(DWORD err=0);

CString PrintRTC(CRuntimeClass* pClass);
CString PrintRTC(CObject* pObj);

// ����� ��� ������� � �������� ������ ������ CMultiDocTemplate
class CMyDocTemp:public CMultiDocTemplate
{
public:
	void PrintInfo(int num)
	{
		CString txt;
		txt.Format("\n--- %i",num);
		txt=txt+"\nTempl- "+PrintRTC(this);
		txt=txt+"\nDoc  - "+PrintRTC(m_pDocClass);
		txt=txt+"\nView - "+PrintRTC(m_pViewClass);
		txt=txt+"\nFrame- "+PrintRTC(m_pFrameClass);
		OutputDebugString(txt);
	}

	CString GetFrame(){return m_pFrameClass->m_lpszClassName;}
	CRuntimeClass* GetFrameClass(){return m_pFrameClass;}
	CRuntimeClass* GetDocClass(){return m_pDocClass;}
	CRuntimeClass* GetViewClass(){return m_pViewClass;}
	CString GetDocString(){CString ret=m_strDocStrings;ret.Replace('\n','\\');return ret;}
	UINT GetID(){return m_nIDResource;}
};

// ����� ��� ��������� ������� �������:
// 1. �������� ���� ������������
// 2. �������� ��������� ���� �������������. (� ������ �������� config.dll
// ��� ����� �����-�����)
typedef int(CWnd::*PFONCREATE)(LPCREATESTRUCT);
class CFakeWnd:public CWnd
{
public:
	int MyCreate(LPCREATESTRUCT);		// �������� ���� ������������
	AFX_MSGMAP* GetMap(){return (AFX_MSGMAP*)GetMessageMap();}	// ��� ������ protected �������
	static PFONCREATE m_pRealFunc;		// ��-�� �� �������� ���������� �������� ���� ������������ CTabMDFrame::OnCreate
	static PFONCREATE m_pCreateMain;	// ���������� ��� ��������� ����
	static void Init();					// ��������� ���������
	static HWND m_hTabWnd;				// ���� � ���������� � ���� ������������
	void SetSite(COleControlSite* pSite){m_pCtrlSite = pSite;}
};

typedef void(*PFUNCINIT)(void*);
typedef HRESULT (*PREGFUNC)();

// ����� ��������� �������
class CIConfig;
// class CConfigSvcImpl:public CConfigService
// {
// // interface
// public:
// 	CString IBDir();
// 	CString BinDir();
// 	CDocument* OpenFile(CString path);
// 	bool AddConfigTab(CString name,HICON hIcon,CRuntimeClass* pRTCDoc,CRuntimeClass* pRTCView);
// 	void AddConfigEventHandler(WORD wEventType,void* pHandler);
// 	bool LoadModule(CString,CString&);
// 	void LoadToolBar(HINSTANCE hInst,UINT nID,LPCTSTR lpszName,int count);
// 	void AddCmdTarget(CCmdTarget* pTarget);
// 	void RemoveCmdTarget(CCmdTarget* pTarget);
// 	void AddDynLink(AFX_EXTENSION_MODULE& pModule)
// 	{
// 		new CDynLinkLibrary(pModule);
// 	}
// // implementation
// public:
// 	bool RegisterPanelClass(LPCTSTR lpszName,CRuntimeClass* pWndClass,HICON hIcon);
// 	bool InitConfigSvc();
// 	static void InitTextDoc();
// 	CConfigSvcImpl();
// 	~CConfigSvcImpl();
// 	CPtrList* GetHandlersList(WORD wType);
// 	static CConfigSvcImpl* m_pTheService;
// 	CMapWordToOb m_HandlersMaps;
// 	CString m_IBPath;
// 	CString m_BinPath;
// 	CMyDocTemp* m_pTextDoc;
// 	ITypeLibPtr m_pTypeLib;
// 	CIConfig* m_Configurator;
// };

void RunMacros(CString modul,CString macros,bool IsScript);

void LogMsg(const char*format,...);
#ifdef LOG_MSG
#define LOG LogMsg
#else
#define LOG 1?void(0):LogMsg
#endif

#endif