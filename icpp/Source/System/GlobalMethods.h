#ifndef GLOBALMETHODS_H
#define GLOBALMETHODS_H

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;

#include <atlcom.h>
// #include <atlctl.h>
// #include <comdef.h>

//#include <Rpcdce.h>
#include "blptr.h"

//#pragma warning(pop)
//#pragma warning(push, 4)

void RegisterCBLContext(CRuntimeClass* pRuntimeClass, LPCSTR szClassName, BOOL bNativeRegistration = TRUE);

// ������ ��� �������� ���� ������, ���������� ��� bool
#define IS_KINDOF_RUNTIME_CLASS(object, class_name) \
	((((void*)(object)) != NULL) && (object)->IsKindOf(RUNTIME_CLASS(class_name)))

// ��������� �� ��������� �������� ?
template<class Type>
bool IS_KINDOF_CGroupContext(Type* pCont)
{
	return IS_KINDOF_RUNTIME_CLASS(pCont, CGroupContext);
};
template<class Type>
bool IS_KINDOF_CGroupContext(const CBLPtr<Type>& pCont)
{
	return IS_KINDOF_RUNTIME_CLASS(pCont.operator Type*(), CGroupContext);
};

// ��������� �� ����� ��� ?
template<class Type>
bool IS_KINDOF_CComponentClass(Type* pCont)
{
	return IS_KINDOF_RUNTIME_CLASS(pCont, CComponentClass);
};

template<class Type>
bool IS_KINDOF_CComponentClass(const CBLPtr<Type>& pCont)
{
	return IS_KINDOF_RUNTIME_CLASS(pCont.operator Type*(), CComponentClass);
};

typedef CArray<CValue*, CValue*> PValueItemArrayT;
//typedef CArray<CValueItem *,CValueItem *> PValueItemArrayT;
typedef CValue* PValueT;

void InitAllBoost();

//������� ��� ������ ��������� � ���� ��������� 1�
void ShowMsg(const char* format,MessageMarker marker=mmNone,...);
void Msg(const char *msg, ...);
void LogErr(const char* format,...);
//������� �������� �� ������, � ����������� 1� ��� ��������� ����������
void RuntimeError(const char *msg, ...);
void RuntimeErrorRes(UINT nID);
void AssertHResult(HRESULT hresult);

// ������� ������ ���� � �����������
void MessageBox(const char* format, ...);

CBLModule* GetModuleByContext(const CGroupContext* pCont);
/*BOOL _CallFromGlobalModules(CGroupContext *pCont, CBLModule *pMod, const char* nameProc0, const char* nameProc1, int argNum, CValue** vArray, bool bCallSysProc = false);
void _CallModuleProc(class CGetDoc7* pDoc, const char* nameProc0,const  char* nameProc1, int argNum, CValue** vArray, BOOL bCallGlobal = TRUE);
void CallModuleProc(const char* nameProc0,const  char* nameProc1, int argNum, CValue** vArray);
CString PrintRTC(CObject* pObj);
void CopyMenu(CMenu* pMenuSrc, CMenu* pMenuDes, bool f = false);*/
CString strLastError(DWORD dwMessageID = 0);

#define DATA_SOURCE_TYPE_DBF    1
#define DATA_SOURCE_TYPE_SQL    2

extern class CDataBase7* pDataBase7;
extern class CMetaDataCont* pMetaDataCont;
extern class CBkEndUI* pBkEndUI;
extern class boost::shared_ptr<class CAddIn> pMainAddIn;

// ��� ����� ����������� �������
extern const char* const pcstrNameOfDef;
extern const char* const TypeNamePrefix;
extern const char* const szDotSubst;

void GetRTCFull(CRuntimeClass* rtc, CString& Str);
void Trace0(const char* str);
void MessageFullRTC(CObject* obj);

// CValueListContext accessor
class CValueListContextFriend : public CValueListContext
{
public:
	typedef CValueListContext BASE;
	void CValueListContextFriend::P_RemoveAll(CValue** ppVal) {	BASE::P_RemoveAll(ppVal); }
	void CValueListContextFriend::P_Add(CValue** ppVal)	{ BASE::P_Add(ppVal); }
	int CValueListContextFriend::GetSize() { return BASE::GetPValueList()->GetSize(); }
	int CValueListContextFriend::IsEmpty() { return !GetSize();	}
	CValue F_FromDelimStr(CValue * * ppValues)	{ return BASE::F_FromDelimStr(ppValues); }
	CValue F_ToDelimStr(CValue * *ppValues)		{ return BASE::F_ToDelimStr(ppValues); }
};

//������� ��� �������������� CValue � ���������������, �������������� � ����������������������.
//���� � CValue ���������� �� ��� ���, �� ���������� NULL
CValueTable* CValue2VT(CValue const& Value); //���������������
CValueListContextFriend* CValue2VLC(CValue const& Value); //��������������
CValueItemList* CValue2VL(CValue const& Value); //��������������
class CVTExtended* CValue2VTExt(CValue const& Value); //����������������������

//������� ��� ��������� ������ �������/��������� �������
bool CallAsFunc(CBLContext* pContext, char const* sFuncName, CValue& RetVal, CValue** params, bool bEnableException = true);
bool CallAsProc(CBLContext* pContext, char const* sProcName, CValue** ppParams, bool bEnableException = true);
bool GetPropVal(CBLContext* pContext, char const* sPropName, CValue& RetVal, bool bEnableException = true);

// �����, ���������� �������������� �� char* ��� CString � WCHAR*
class CWChar
{
private:
    WCHAR* wStr;
public:
	CWChar(CString const& str)
	{
		size_t nLen = str.GetLength() + 1;
	    wStr = new wchar_t[nLen];
		std::swprintf(wStr, L"%S", str.operator LPCTSTR());
	};

	CWChar(const char* str)
	{
		size_t nLen = strlen(str) + 1;
	    wStr = new wchar_t[nLen];
		std::swprintf(wStr, L"%S", str);
	};
	operator LPCOLESTR() const
	{ return wStr; }

	~CWChar(void)
	{
		if(wStr) delete[] wStr;
	};
};

// �������������/����� �������������� ����� HKEY_CLASSES_ROOT � ����� HKEY_CURRENT_USER
void RedirectHKCR(BOOL bRedirect);

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

// �������� ������
CString GetVersionField(LPCSTR szField); // ��� ��
CString GetVersionField(LPCTSTR filePath, LPCTSTR szField); // ��� ������������� �����

// ���������� ������, ������ ������ 1�++, ��� ��� ��������  ��
// �������� "� ���������..."
// ��������, "2.0.2.0 Nightly build 2006-05-15" ��� "2.0.3.0"
//
CString GetFullVersion();

// ������� ����� ������ �� ���������� ����������
// ��������, "IBDir"
// � ������ ������ ������ ���������� throw
//
// TODO �������� ����� ��� ������ � ������������ ���������� ���������� !!
//
LPCTSTR CallMethodOfGlobalContexts0(const CString& MethodName);
CValue CallMethodOfGlobalContexts1(const CString& MethodName, CValue& param1);

// ����� ����� ������ �� ���������� ����������
// ��������, "IBDir"
// ���������� ����� ���������� ������
//	��� -1, ���� ����� �� ������
//
// � �������� pBLContext ����������� ��������� �� ��������� ���������� ��������
//
int FindMethodOfGlobalContexts(const CString& MethodName, CBLContext*& pBLContext);

// �������� �������� ����� �� ���������
CFormAllCtrlsContext* GetFormContext(const CBLContext* pCont);
CGetDoc7* GetDoc7(const CBLContext* pCont);

// �������� ������ ���������� ��������� �� ��������� ���������
CDocActModule* GetDocumentTransactionModule(CDocContext* pDoc);

// �������� �������� ��������� �� ����� ���������
CDocContext* GetDocContextFromCGetDoc7(CGetDoc7* pDoc);

// �������� �������� ��������� �� ���������� ���������
CDocContext* GetDocContextFromGroupContext(CGroupContext* pGroupCont);

// ��������� ��������� CValue
void ValidateParamContext(const CValue& ContValue, const CString& strErrorMessage);
void ValidateParamContext(const CValue& ContValue, const CString& strContextType, const CString& strErrorMessage);
void ValidateParamGroupContext(const CValue& GroupContValue, const CString& strErrorMessage);
void ValidateParamNumeric(const CValue& NumericValue, const CString& strErrorMessage);
void ValidateParamString(const CValue& StringValue, const CString& strErrorMessage);
void ValidateParamNotEmptyString(const CValue& StringValue, const CString& strErrorMessage);
void ValidateParamValueListContext(const CValue& ValueListValue, const CString& strErrorMessage);
void ValidateParamUserClass(const CValue& UserClassContValue, const CString& strErrorMessage);

// ����� ��� ����������� �������� �������� ����������, ���������� �� ����������
template <class T>
class CValueGuard
{
private:
	T& m_Value;
	T m_SavedValue;
public:
	CValueGuard(T& bSavedValue, const T& newValue) :
		m_Value(bSavedValue)
	{
		m_SavedValue = m_Value;
		m_Value = newValue;
	};
	~CValueGuard()
	{
		m_Value = m_SavedValue;
	}
};

// ����� ��� ����������� �������� �������� ������� ����������, ���������� �� ����������
typedef CValueGuard<bool> CFlagGuard;

// ��� SQL-������ ?
bool IsSQLVersion(void);

// ������� ��� �������� ������ ������� ������ - ������ ������ ����������� ���
// ���� ���������� � ��������� ����� CallAsFunc ��������������� retVal, ����� ��������� ������ ���������� ������ -
// ����-���� "TurboBL�����::����������������������������������"
inline int CallAsFunc(CBLModule* pMod, int iProcNum, CValue& retVal, int iParamsCount, CValue** ppParams)
{
	CValue locVal;
	int ret = pMod->CallAsFunc(iProcNum, locVal, iParamsCount, ppParams);
	retVal = locVal;
	return ret;
}

// Windows 2000/�� ��� ����� ?
bool IsSystem_Windows2000AndHigher();

void ReadFileToStringWithExceptions(const CString& strFileName, CString& strDest);
bool ReadFileToStringWithoutExceptions(const CString& strFileName, CString& strDest);

void DumpContext(CBLContext* pCont);
void CallIsOleContext(CBLContext* pCont, int nParams);

// ���� ������� ��� ���������� � ����� ��� �������� �����, �� ����� ���, ��������, � CFlagGuard :(
#include "Preprocessor.h" 

template <typename T>
struct null_deleter
{
	void operator()(T *) const
	{
	}
};

template <typename T>
struct CBLContext_deleter
{
	void operator()(T * p) const
	{
		boost::function_requires< boost::ConvertibleConcept<T*, CBLContext*> >();
		if (p)
			p->DecrRef();
	}
};

class CBLMyEx:public CBLExportContext
{
public:
	static long DispParamToValueEx(CValue** ppVal, DISPPARAMS* pDisp,unsigned int* pUI)
	{
		return static_cast<CBLMyEx*>(NULL)->DispParamToValue(ppVal, pDisp,pUI);
	}
	int  TryReturnByRefEx(CValue ** ppVal,DISPPARAMS* pDisp)
	{
		return TryReturnByRef(ppVal,pDisp);
	}
	static int  ValueToVariantEx(CValue& Val,tagVARIANT * pVariant){
		return static_cast<CBLMyEx*>(NULL)->ValueToVariant(Val,pVariant);
	}
};

void DispatchToValue(IUnknown* pUnk, CValue* pVal);

CString GetModuleVersionInfo(HMODULE hModule, CString VerInfoName);
CString GetModuleVersionInfo(CString ModuleName, CString VerInfoName);
void TraceMenu(CMenu* Menu, CString Indent="");

#endif