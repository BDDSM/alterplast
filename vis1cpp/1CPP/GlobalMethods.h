#ifndef GLOBALMETHODS_H
#define GLOBALMETHODS_H

#include "istrmap.h"

//#pragma warning(pop) 
//#pragma warning(push, 4)

void RegisterCBLContext(CRuntimeClass* pRuntimeClass, LPCSTR szClassName, BOOL bNativeRegistration = TRUE);

// ������ ��� �������� ���� ������, ���������� ��� bool
//(object)->IsKindOf(RUNTIME_CLASS(class_name))
#define IS_KINDOF_RUNTIME_CLASS(object, class_name) \
	((object != NULL) && (object)->IsKindOf(RUNTIME_CLASS(class_name)))

// ��������� �� ��������� �������� ?
#define IS_KINDOF_CGroupContext(pCont) (IS_KINDOF_RUNTIME_CLASS(pCont, CGroupContext))

#define UNDEFINE_TYPE_1C         0
#define NUMBER_TYPE_1C           1
#define STRING_TYPE_1C           2
#define DATE_TYPE_1C             3
#define ENUM_TYPE_1C            10
#define REFERENCE_TYPE_1C       11
#define DOCUMENT_TYPE_1C        12
#define CALENDAR_TYPE_1C        13
#define CALCULATIONKIND_TYPE_1C 14
#define ACCOUNT_TYPE_1C         15
#define SUBCONTOKIND_TYPE_1C    16
#define CHART_OF_ACC_TYPE_1C    17
#define AGREGATE_TYPE_1C        100
#define UNDEF_TYPE_MODIFIKATOR 20

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
//extern CComModule _Module;

// ��� ����� ����������� �������
static const char * pcstrNameOfDef = "defcls.prm";

//#include <atlcom.h>
//#include <atlctl.h>
#include <comdef.h>

#include "Preprocessor.h"

#include <Rpcdce.h>

// � 2.0.3 ����� ����.������
//#define VK_FormEx_Hooks

// � 2.5 ����� ����.������
#undef VK_FormEx_Hooks

typedef CArray<void *,void *> PValueItemArrayT;
//typedef CArray<class CValueItem *,class CValueItem *> PValueItemArrayT;
typedef CValue* PValueT;

/*typedef struct tagValueItemArray
{
	CValue	Value;
	struct tagValueItemArray**	NextLevel;
	DWORD	Index;
	BYTE	Type;
	int		Count;
	CString* Descr;
	BOOL	Checked;
	CValueItem* ValueItem;
	BOOL Selected;
} structVIA, *PstructVIA;

typedef union tagMessageMapFunctions 
{
	AFX_PMSG pfn;

	void (CBrowse::*pfn_my)(void);
	void (CChildFrame7::*pfn_mdi)(BOOL,CWnd *,CWnd *);
} FormEx_MessageMapFunctions;

struct CProcInfoFormEx
{
	int num;
	int type;
	int numarg;
};

struct CModuleInfoFormEx
{
	CMapStringToPtr cProcMap;
};


LRESULT __stdcall Mouse_Hook(int code, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall Key_Hook(int code, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall Wnd_Hook(int code, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall WndPre_Hook(int code, WPARAM wParam, LPARAM lParam);
void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime);
LRESULT FormEx_OnSetCursor(WORD nHittest);
*/
//void InitAllBoost();

//������� ��� ������ ��������� � ���� ��������� 1�
void ShowMsg(const char* format,MessageMarker marker=mmNone,...);
void Msg(const char *msg, ...);
void LogErr(const char* format,...);
//������� �������� �� ������, � ����������� 1� ��� ��������� ����������
void RuntimeError(const char *msg, ...);
void RuntimeErrorRes(UINT nID);

// ������� ������ ���� � �����������
void MessageBox(const char* format, ...);

//CBLModule* GetModuleByContext(const CGroupContext* pCont);
/*BOOL _CallFromGlobalModules(CGroupContext *pCont, CBLModule *pMod, const char* nameProc0, const char* nameProc1, int argNum, CValue** vArray, bool bCallSysProc = false);
void _CallModuleProc(class CGetDoc7* pDoc, const char* nameProc0,const  char* nameProc1, int argNum, CValue** vArray, BOOL bCallGlobal = TRUE);
void CallModuleProc(const char* nameProc0,const  char* nameProc1, int argNum, CValue** vArray);
CString PrintRTC(CObject* pObj);
void CopyMenu(CMenu* pMenuSrc, CMenu* pMenuDes, bool f = false);*/
CString strLastError(DWORD dwMessageID = 0);

#define DATA_SOURCE_TYPE_DBF    1
#define DATA_SOURCE_TYPE_SQL    2

const char TypeNamePrefix[] = "_1c_type_";

void GetRTCFull(CRuntimeClass* rtc, CString& Str);
void Trace0(const char* str);
void MessageFullRTC(CObject* obj);

/*
//������� ��� �������������� CValue � ���������������, �������������� � ����������������������. 
//���� � CValue ���������� �� ��� ���, �� ���������� NULL
CValueTable* CValue2VT(CValue const& Value);    //���������������
CPtrArray* CValue2VL(CValue const& Value);      //��������������
class CVTExtended* CValue2VTExt(CValue const& Value); //����������������������
*/

//������� ��� ��������� ������ �������/��������� �������
bool CallAsFunc(CBLContext* pContext, char const* sFuncName, CValue& RetVal, CValue** params);
bool CallAsProc(CBLContext* pContext, char const* sProcName, CValue** ppParams);
bool GetPropVal(CBLContext* pContext, char const* sPropName, CValue& RetVal);

// �����, ���������� �������������� �� char* ��� CString � WCHAR*
class CWChar
{
private:
    WCHAR* wStr;
public:
	CWChar(CString str)
	{
	    wStr = new wchar_t[str.GetLength()+1];
	    swprintf(wStr, L"%S", str);
	};

	CWChar(const char* str)
	{
	    wStr = new wchar_t[strlen(str)+1];
	    swprintf(wStr, L"%S", str);
	};
	operator LPCOLESTR() const
	{ return wStr; }

	~CWChar(void)
	{ 
		if(wStr) delete[] wStr;
	};
};

// �������� �� ������������� ����� ��� �����
bool FileExists(CString fileName);
bool FolderExists(CString folderName);
bool DirectoryExists(CString directoryName);

// �������� ��� ����� �� ������� ���� � �����
// ������������� ����� �� �����������
// ���� �� ������� �������� ��� �����, ������������ ""
//
// �����: ����� ������ ������������ �� ������ � �����!!
//
//	�� T:\����� �����\��� �����.txt ������ T:\����� �����\
//
CString ExtractFileDir(CString FileName);

// �������� ���������� ���� � �����\�����
// ����������� ������� "." ��� ".."
//
CString AbsoluteFilePath(CString sRelativePath);

/*
// �������� ������
CString GetVersionField(LPCSTR szField); // ��� ��
CString GetVersionField(LPCTSTR filePath, LPCTSTR szField); // ��� ������������� �����

// ���������� ������, ������ ������ 1�++, ��� ��� ��������  ��
// �������� "� ���������..."
// ��������, "2.0.2.0 Nightly build 2006-05-15" ��� "2.0.3.0"
//
CString GetFullVersion();
*/

// ������� ����� ������ �� ���������� ����������
// ��������, "IBDir"
// � ������ ������ ������ ���������� throw
//
// TODO �������� ����� ��� ������ � ������������ ���������� ���������� !!
//
LPCTSTR CallMethodOfGlobalContexts0(CString MethodName);
CValue CallMethodOfGlobalContexts1(CString MethodName, CValue& param1);

// ����� ����� ������ �� ���������� ����������
// ��������, "IBDir"
// ���������� ����� ���������� ������ 
//	��� -1, ���� ����� �� ������
//
// � �������� pBLContext ����������� ��������� �� ��������� ���������� ��������
// 
int FindMethodOfGlobalContexts(CString MethodName, CBLContext*& pBLContext);

// ������ ������������ ��� ��������� ���� ���������� � �������, 
// ������� �������� ������������ � ���� 1�
// ���������� 1� ������������ !!
//
// ������������� � ������������� � ������� CallAsFunc � CallAsProc
// ������ �������������:
/*	
	int CMyContextBase::CallAsFunc(int iMethNum,CValue& rValue,CValue **ppValue)
	{
		TRY{
			// ��� ������ 
		}
		CATCH_ALL_AND_DO_RUNTIME_ERROR
	}
*/
//
#define CATCH_ALL_AND_DO_RUNTIME_ERROR \
	CATCH(CFileException, pEx) \
	{ \
		RuntimeError("����� <%s::%s>: ���������� - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"������ ������ � �������"); \
		 return TRUE; \
	} \
	AND_CATCH(CMemoryException, pEx) \
	{ \
		RuntimeError("����� <%s::%s>: ���������� - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"������ ������ � �������"); \
		 return TRUE; \
	} \
	AND_CATCH(CNotSupportedException, pEx) \
	{ \
		RuntimeError("����� <%s::%s>: ���������� - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"�������� �� ��������������"); \
		 return TRUE; \
	} \
	AND_CATCH(CResourceException, pEx) \
	{ \
		RuntimeError("����� <%s::%s>: ���������� - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"������ ������ � ���������"); \
		 return TRUE; \
	} \
	AND_CATCH(COleException, pEx) \
	{ \
		RuntimeError("����� <%s::%s>: ���������� - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"������ COleException"); \
		 return TRUE; \
	} \
	AND_CATCH(COleDispatchException, pEx) \
	{ \
		RuntimeError("����� <%s::%s>: ���������� - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"������ COleDispatchException"); \
		 return TRUE; \
	} \
	AND_CATCH(CUserException, pEx) \
	{ \
		RuntimeError("����� <%s::%s>: ���������� - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"������ CUserException"); \
		 return TRUE; \
	} \
	AND_CATCH_ALL(pEx) \
	{ \
		RuntimeError("����� <%s::%s>: ���������� - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"����������� ������"); \
		 return TRUE; \
	} \
	} }  \
	catch(...) \
	{ \
		CBLModule* pCurMod = CBLModule::GetExecutedModule(); \
		int iRuntimeErrCode = pCurMod->GetRuntimeErrCode(); \
		"����������� - ��� ���������� ����������� ������� CBLModule::RaiseExtRuntimeError ������ ���� ��"; \
		if (iRuntimeErrCode) \
		{  \
			CString strGetRuntimeErrDescr;  \
			pCurMod->GetRuntimeErrDescr(iRuntimeErrCode,strGetRuntimeErrDescr);  \
			RuntimeError((LPCTSTR)strGetRuntimeErrDescr);  \
		} \
		else{ \
			RuntimeError("����� <%s::%s>: ����������� ����������, ��� ������ <%d>", GetTypeString(), \
				GetMethodName(iMethNum, 1), iRuntimeErrCode);  \
		}; \
		return TRUE; \
	} \
	}

// �������� �������� ��������/�����
CGetDoc7* GetActiveDocument();

// �������� ���� � ���������/�����
CString GetDocumentPath(CGetDoc7* pDoc);

// �������� ����� ����������� ��������� ? ������
// ���� ���������� 0, ������, �������
//
long GetCalcVarMetaID(const CString& strFileNameA);

// �������� ���������� ���� � ���������, 
//	�������� ��� ���������� ��������� - ���������������������������@MD
//
CString GetInternalPath(const CString& strFileName);
CString GetInternalStoragePath(const CString& strFileNameA);

void ReadFileToStringWithExceptions(const CString& strFileName, CString& strDest);
bool ReadFileToStringWithoutExceptions(const CString& strFileName, CString& strDest);

#endif
