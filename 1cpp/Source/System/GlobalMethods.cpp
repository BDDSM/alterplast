// ���������� ������ � ����� ��� ����������

#include "stdafx.h"
#include "GlobalMethods.h"

#include "..\TurboBL\gcboost.h"
#include "LogFile.h"

extern CBkEndUI * pBkEndUI;

//������� ��� ������ ��������� � ���� ��������� 1�
void ShowMsg(const char* format,MessageMarker marker,...)
{
	va_list arg;
	va_start(arg,marker);
	CString txt;
	txt.FormatV(format,arg);
	va_end(arg);
	if(!pBkEndUI)
		AfxMessageBox(txt);
	else
		pBkEndUI->DoMessageLine(txt,marker);

	/*if(!g_Log.IsOpen()){
		g_Log.Open("C:\\1cpp.txt");
		g_Log.Clear();
		g_Log<<"Program started at "<<CRLF;
	}
	g_Log<<"ShowMsg "<<txt<<CRLF;*/
}

void Msg(const char *msg, ...)
{
	CString str;
	va_list ap;
	va_start(ap, msg);
	str.FormatV(msg, ap);
	va_end(ap);

	pBkEndUI->DoMessageLine((LPCSTR)str, mmBlueTriangle);

	/*if(!g_Log.IsOpen()){
		g_Log.Open("C:\\1cpp.txt");
		g_Log.Clear();
		g_Log<<"Program started at "<<CRLF;
	}
	g_Log<< "Msg " << str << CRLF;*/
}

void LogErr(const char* format,...)
{
	va_list arg;
	va_start(arg,format);
	CString txt;
	txt.FormatV(format,arg);
	
	CBkEndUI *pBkEndUI = GetBkEndUI();
	if (pBkEndUI)
		pBkEndUI->DoMessageLine(txt,mmBlackErr);
	else
		AfxMessageBox(txt);

	va_end(arg);

// 	if(!g_Log.IsOpen()){
// 		g_Log.Open("C:\\1cpp.txt");
// 		g_Log.Clear();
// 		g_Log<<"Program started at "<<CRLF;
// 	}
// 	g_Log << "1C++ LogErr " << txt << CRLF;
}

// ������� ������ ���� � �����������
void MessageBox(const char* format, ...)
{
	va_list arg;
	va_start(arg,format);
	CString txt;
	txt.FormatV(format,arg);
	va_end(arg);

	AfxMessageBox(txt);

	/*if(!g_Log.IsOpen()){
		g_Log.Open("C:\\1cpp.txt");
		g_Log.Clear();
		g_Log<<"Program started at "<<CRLF;
	}
	g_Log << "MessageBox " << txt << CRLF;*/
}

//������� �������� �� ������, � ����������� 1� ��� ��������� ����������
void RuntimeError(const char *msg, ...)
{
	CString str;
	va_list ap;
	va_start(ap, msg);
	str.FormatV(msg, ap);
	va_end(ap);

	/*if(!g_Log.IsOpen()){
		g_Log.Open("C:\\1cpp.txt");
		g_Log.Clear();
		g_Log<<"Program started at " << CRLF;
	}
	g_Log << "RuntimeError " << str << CRLF;*/

	// ����� �� ����� ���������� ���������� �� ����� ����������
	// �.�. ����� 1� ����� ������
	// � ��� ������ �������� ����� ����������� CBLModule::RaiseExtRuntimeError �
	// �������� ���� ������ ��� !
	//
	//CRuntimeExceptionClass::Empty(); 

	CBLModule::RaiseExtRuntimeError((LPCSTR)str, mmRedErr);
}

void RuntimeErrorRes(UINT nID)
{
	CString err;
	err.LoadString(nID);

	/*if(!g_Log.IsOpen()){
		g_Log.Open("C:\\1cpp.txt");
		g_Log.Clear();
		g_Log<<"Program started at " << CRLF;
	}
	g_Log << "RuntimeErrorRes " << err << CRLF;*/

	CBLModule::RaiseExtRuntimeError(err, mmRedErr);
}

void GetRTCFull(CRuntimeClass* rtc, CString& Str)
{
    if (rtc)
    {
        Str += "<-";
        Str += rtc->m_lpszClassName;
        GetRTCFull(rtc->m_pfnGetBaseClass(), Str);
    }
}

void Trace0(const char* str)
{
    GetBkEndUI()->DoMessageLine(str, mmNone);
}

void MessageFullRTC(CObject* obj)
{
    CString Str;
    GetRTCFull(obj->GetRuntimeClass(), Str);
    Trace0(Str);
}


CVTExtended* CValue2VTExt(CValue const& Value)
{
	if( Value.GetTypeCode() != AGREGATE_TYPE_1C ) return NULL;

    CBLContext* pVTContext = Value.GetContext();
	if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CVTExtended") == 0 )
	{
		return (CVTExtended*)pVTContext;
	}
	else if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CComponentClass") == 0 )
	{
		CValue vBaseClass;
		if( static_cast<CComponentClass*>(pVTContext)->GetBaseClass("����������������������", vBaseClass) )
		{
			return (CVTExtended*)(vBaseClass.GetContext());
		}
	}

	return NULL;
}

CValueTable* CValue2VT(CValue const& Value)
{
	if( Value.GetTypeCode() != AGREGATE_TYPE_1C ) return NULL;

    CBLContext* pVTContext = Value.GetContext();
	if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CValueTableContext") == 0 )
	{
		return ((CValueTableContextData*)pVTContext->GetInternalData())->GetValueTable();
	}
	else if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CComponentClass") == 0 )
	{
		CValue vBaseClass;
		if( static_cast<CComponentClass*>(pVTContext)->GetBaseClass("���������������", vBaseClass) )
		{
			pVTContext = vBaseClass.GetContext();
			return ((CValueTableContextData*)pVTContext->GetInternalData())->GetValueTable();
		}
	}

	return NULL;
}

CPtrArray* CValue2VL(CValue const& Value)
{
	if( Value.GetTypeCode() != AGREGATE_TYPE_1C ) return NULL;

    CBLContext* pVTContext = Value.GetContext();
	if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CValueListContext") == 0 )
	{
		return (CPtrArray*)(((CValueListContext*)(pVTContext))->m_pValueItemList);
	}
	else if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CComponentClass") == 0 )
	{
		CValue vBaseClass;
		if( static_cast<CComponentClass*>(pVTContext)->GetBaseClass("��������������", vBaseClass) )
		{
			pVTContext = vBaseClass.GetContext();
			return (CPtrArray*)(((CValueListContext*)(pVTContext))->m_pValueItemList);
		}
	}

	return NULL;
}

CString strLastError(DWORD dwMessageID /* = 0*/)
{
	void* cstr = NULL;
	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwMessageID==0 ? GetLastError() : dwMessageID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &cstr,
		0,
		NULL
		);
	CString res = (char*)cstr;
	::LocalFree(cstr);
	return res;
}

// �������������/����� �������������� ����� HKEY_CLASSES_ROOT � ����� HKEY_CURRENT_USER

typedef LONG (APIENTRY f_LONG_HKEY_HKEY)(HKEY, HKEY);
void RedirectHKCR(BOOL bRedirect)
{
	static f_LONG_HKEY_HKEY* pRegOverridePredefKey = NULL;
	static BOOL bUnsupported = FALSE;

	if (!pRegOverridePredefKey && !bUnsupported)
	{
// 		BYTE dwWindowsMajorVersion = LOBYTE(LOWORD(GetVersion()));
// 		if (dwWindowsMajorVersion < 5) // �����, ��� Windows 2000
		if (!IsSystem_Windows2000AndHigher())
			bUnsupported = TRUE;
		else
		{
			HMODULE hm = GetModuleHandle("ADVAPI32.DLL");
			if (hm)
				pRegOverridePredefKey = reinterpret_cast<f_LONG_HKEY_HKEY*>(GetProcAddress(hm, "RegOverridePredefKey"));

			if (!pRegOverridePredefKey)
				bUnsupported = TRUE;
		}
	}
	if (bUnsupported)
		return;

	if (bRedirect)
	{
		HKEY hNewHKey; 
		RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Classes", 0, KEY_READ|KEY_WRITE , &hNewHKey  ); 
		(*pRegOverridePredefKey)(HKEY_CLASSES_ROOT, hNewHKey); 
		RegCloseKey(hNewHKey);
	} else
		(*pRegOverridePredefKey)(HKEY_CLASSES_ROOT, NULL); 
}

// �������� �� ������������� ����� ��� �����
bool FileExists(CString fileName)
{
	CString sAbsoluteFilePath(::AbsoluteFilePath(fileName));
	DWORD tRes = GetFileAttributes((LPCSTR)sAbsoluteFilePath);
/*	if (tRes != 0xFFFFFFFF)
		if (!(tRes & FILE_ATTRIBUTE_DIRECTORY))
			bFileExists = true;*/
	return tRes != 0xFFFFFFFF && !(tRes & FILE_ATTRIBUTE_DIRECTORY);
}

bool FolderExists(CString folderName)
{
	DWORD tRes = GetFileAttributes((LPCSTR)folderName);
	return tRes != 0xFFFFFFFF && tRes & FILE_ATTRIBUTE_DIRECTORY;
}

bool DirectoryExists(CString directoryName)
{
	return FolderExists(directoryName);
}

// �������� ��� ����� �� ������� ���� � �����
// ������������� ����� �� �����������
// ���� �� ������� �������� ��� �����, ������������ ""
//
// �����: ����� ������ ������������ �� ������ � �����!!
//
//	�� "T:\����� �����\��� �����.txt" ������ T:\����� �����\
//
CString ExtractFileDir(CString FileName)
{
	int i = 0;
	const char PathDelim = '\\';
	const char DriveDelim = ':';
	LPCSTR pName = (LPCSTR)FileName;
	LPCSTR s = strrchr(pName, PathDelim);
	if (!s) return "";

	int count = (s-pName)+1+1; // 1 ���� ��� '\0'
	vector<char> buf(count); //char* _ret = new char[count];
	char* _ret = &buf[0];
	lstrcpyn(_ret, pName, count);
	CString ret = _ret;
	//delete [] _ret;
	return ret;
}

// �������� ���������� ���� � �����\�����
// ����������� ������� "." ��� ".."
//
CString AbsoluteFilePath(const CString sRelativePath)
{
	vector<char> buf(_MAX_PATH); //LPSTR fullpath = new char[_MAX_PATH];
	LPSTR fullpath = &buf[0];
	if (_fullpath( fullpath, (LPCTSTR)sRelativePath, _MAX_PATH ) == NULL )
		return ""; // TODO ��� ��������� ����������
	CString ret(fullpath);

	//delete [] fullpath;
	return ret;
}

// �������� ������ ������������� �����
CString GetVersionField(LPCTSTR filePath, LPCTSTR szField)
{
	if (!FileExists(filePath))
		AfxThrowFileException(CFileException::fileNotFound);

    CString sRet;
	LPTSTR BufFileName = const_cast<LPTSTR>(filePath);

    DWORD z,size;
    size = GetFileVersionInfoSize(BufFileName, &z);
	//if (!size) throw CException();
    if(size)
    {
		vector<char> buf(size); //char* pVersion = new char[size];
		char* pVersion = &buf[0];
        GetFileVersionInfo(BufFileName, 0, size, pVersion);
        DWORD* translate;
        UINT len;
        VerQueryValue(pVersion, "\\VarFileInfo\\Translation", (void**)&translate, &len);
        CString blockName;
        blockName.Format("\\StringFileInfo\\%04x%04x\\", LOWORD(translate[0]), HIWORD(translate[0]));
        LPCTSTR valuebuf;
        CString fdName = blockName + szField;
        if (VerQueryValue(pVersion, (char*)(LPCTSTR)fdName, (void**)&valuebuf, &len))
			sRet = valuebuf;
        //delete pVersion;
    }
    return sRet;
}

// �������� ������
CString GetVersionField(LPCSTR szField)
{
	vector<char> buf(256); //LPTSTR BufFileName = new char[256];
	LPTSTR BufFileName = &buf[0];
    GetModuleFileName(_Module.m_hInst, BufFileName, 256);
	CString ret = ::GetVersionField(BufFileName, szField);
	//delete [] BufFileName;
	return ret;
}

// ���������� ������, ������ ������ 1�++, ��� ��� ��������  ��
// �������� "� ���������..."
// ��������, "2.0.2.0 Nightly build 2006-05-15" ��� "2.0.3.0"
//
CString GetFullVersion()
{
	CString sVersion = ::GetVersionField("FileVersion");
	sVersion.Replace(" ", "");
	sVersion.Replace(',', '.');
	CString sPrivateBuild = ::GetVersionField("PrivateBuild");
	if (!sPrivateBuild.IsEmpty())
	{
		sVersion += ' ';
		sVersion += sPrivateBuild;
	}
	CString sSpecialBuild = ::GetVersionField("SpecialBuild");
	if (!sSpecialBuild.IsEmpty())
	{
		sVersion += ' ';
		sVersion += sSpecialBuild;
	}
	return sVersion;
}

// ������� ����� ������ �� ���������� ����������
// ��������, "IBDir"
// � ������ ������ ������ ���������� throw
LPCTSTR CallMethodOfGlobalContexts0(CString MethodName)
{
	CBLContext* pCont = NULL;
	int nMethNum = ::FindMethodOfGlobalContexts(MethodName, pCont);
    if (nMethNum != -1)
    {
	  CValue retval;
      CValue* MasPar[1] = {0};
      CValue Par;
      MasPar[0] = &Par;

      if (pCont->CallAsFunc(nMethNum, retval, &MasPar[0])!=0)
      {
        const char *pC = retval.GetString().operator LPCSTR();
        //pathBase1C = pC;
        //bFindMethod = true;
		return pC;
      }
    }
	//throw LPCTSTR(NULL);
	RuntimeError("�� ����� ����� 1� <%s>", MethodName);
	return "";
}

// ������� ����� ������ �� ���������� ����������
// ��������, "IBDir"
// � ������ ������ ������ ���������� throw
CValue CallMethodOfGlobalContexts1(CString MethodName, CValue& param1)
{
  CValue retval = CNumeric(0);

	CBLContext* pCont = NULL;
	int nMethNum = ::FindMethodOfGlobalContexts(MethodName, pCont);
    if (nMethNum != -1)
    {
	  CValue retval;
      const CValue *pParams[1] = {0};
      pParams[0] = &param1;
      //return pMod->CallAsProc(iProcNum, 1, const_cast<CValue**>(pParams));

      if (pCont->CallAsFunc(nMethNum, retval, const_cast<CValue**>(pParams)))
      {
		return retval;
      }
    }
	//throw LPCTSTR(NULL);
	RuntimeError("�� ����� ����� 1� <%s>", MethodName);
	return retval;
}

// ����� ����� ������ �� ���������� ����������
// ��������, "IBDir"
// ���������� ����� ���������� ������
//	��� -1, ���� ����� �� ������
//
// � �������� pBLContext ����������� ��������� �� ��������� ���������� ��������
//
int FindMethodOfGlobalContexts(CString MethodName, CBLContext*& pBLContext)
{
    ULONG first = CBLContext::GetFirstLoadedContextID();
    for (ULONG ind = first; ind != 0; ind = CBLContext::GetNextLoadedContextID(ind))
    {
		CBLContext* pContLoaded = CBLContext::GetLoadedContext(ind);

		int nMethNum = pContLoaded->FindMethod((LPCTSTR)MethodName);
		if (nMethNum != -1)
		{
			pBLContext = pContLoaded;
			return nMethNum;
		}
	}
	return -1;
}

CBLModule* GetModuleByContext(const CGroupContext* pCont)
{
//   CBLModule * pMod = NULL;
// 	if(pCont->m_FlagAutoDestroy > 100)
//   {
//     pMod = ((CGCBoost::SGCInfo*)pCont->m_FlagAutoDestroy)->m_pMod;
//   }
//   return pMod;
	return CGCBoost::GetModuleByContext(pCont);
}

// �������� �������� ����� �� ���������
CFormAllCtrlsContext* GetFormContext(const CBLContext* pCont)
{
	if (!pCont)
		return NULL;

	CBLContext* pCtxForm = NULL;//TContPtr pCtxForm = NULL;
	if(IS_KINDOF_RUNTIME_CLASS(pCont, CGroupContext))
	{
		int nForm=pCont->FindProp("�����");
		if(nForm>=0)
		{
			CValue vForm;
			pCont->GetPropVal(nForm, vForm);
			pCtxForm=vForm.GetContext();
		}
	}
	else
	{
// 		pCtxForm = const_cast<TContPtr>(pCont);
		pCtxForm = const_cast<CBLContext*>(pCont);
	}

	if(pCtxForm && !strcmp(pCtxForm->GetRuntimeClass()->m_lpszClassName, "CFormAllCtrlsContext"))
	{
		return static_cast<CFormAllCtrlsContext*>(pCtxForm);
	}

	return NULL;
}

CGetDoc7* GetDoc7(const CBLContext* pCont)
{
	CFormAllCtrlsContext* pFormContext = GetFormContext(pCont);
	return !pFormContext ? NULL : pFormContext->m_GetDoc;
}

// �������� ������ ���������� ��������� �� ��������� ���������
CDocActModule* GetDocumentTransactionModule(CDocContext* pDoc)
{
	CDocTransacter* pDocTransacter = pDoc->m_pDocTransacter;
	//CValue rValue;
	//pDoc->CallAsFunc(38, rValue, NULL); // ���.��������
	//pDoc->CallAsFunc(39, rValue, NULL); // ���.��������������������
	if (!pDocTransacter)
		return NULL;
	
	return pDocTransacter->GetAt(-1);
}

// �������� �������� ��������� �� ���������� ���������
CDocContext* GetDocContextFromGroupContext(CGroupContext* pGroupCont)
{
	if(pGroupCont)
	{
		for(int i = 0; i<pGroupCont->GetSize(); i++)
		{
			CBLContext *pCont = pGroupCont->GetContext(i);
			if(IS_KINDOF_RUNTIME_CLASS(pCont, CDocContext))
				return (CDocContext*)pCont;
		}
	}
	return NULL;
}

// �������� �������� ��������� �� ����� ���������
CDocContext* GetDocContextFromCGetDoc7(CGetDoc7* pDoc)
{
	return GetDocContextFromGroupContext(pDoc->GetGroupContext());
}

// ��������� ��������� CValue
void ValidateParamContext(const CValue& ContValue, const CString& strErrorMessage)
{
	CBLContext* pCont = ContValue.GetContext();

	if (!pCont)
		RuntimeError(strErrorMessage);
}

void ValidateParamContext(const CValue& ContValue, const CString& strContextType, const CString& strErrorMessage)
{
	CBLContext* pCont = ContValue.GetContext();

	if (!pCont)
		RuntimeError(strErrorMessage);

	if (strcmp(pCont->GetRuntimeClass()->m_lpszClassName, strContextType))
		RuntimeError(strErrorMessage);
}

void ValidateParamGroupContext(const CValue& GroupContValue, const CString& strErrorMessage)
{
	CBLContext* pGroupCont = GroupContValue.GetContext();

	if (!IS_KINDOF_CGroupContext(pGroupCont))
		RuntimeError(strErrorMessage);
}

void ValidateParamNumeric(const CValue& NumericValue, const CString& strErrorMessage)
{
	if (1 != NumericValue.GetTypeCode())
		RuntimeError(strErrorMessage);
}

void ValidateParamString(const CValue& StringValue, const CString& strErrorMessage)
{
	if (2 != StringValue.GetTypeCode())
		RuntimeError(strErrorMessage);
}

void ValidateParamNotEmptyString(const CValue& StringValue, const CString& strErrorMessage)
{
	if (2 != StringValue.GetTypeCode())
		RuntimeError(strErrorMessage);

	CString strPar = StringValue.GetString();
	if (strPar.IsEmpty())
		RuntimeError(strErrorMessage);
}

void ValidateParamValueListContext(const CValue& ValueListValue, const CString& strErrorMessage)
{
	ValidateParamContext(ValueListValue, "CValueListContext", strErrorMessage);
}

void ValidateParamUserClass(const CValue& UserClassContValue, const CString& strErrorMessage)
{
	CBLContext* pUserClassCont = UserClassContValue.GetContext();

	if (!IS_KINDOF_CComponentClass(pUserClassCont))
		RuntimeError(strErrorMessage);
}

extern CDataBase7 * pDataBase7;
bool IsSQLVersion(void)
{
  return "CODBCDB7" == CString(pDataBase7->GetRuntimeClass()->m_lpszClassName);

};

// AddParams_IntoValueArray
//
// � ����������� ������� � ����� �������� ��������� ���������� ����������
//
void AddParams_IntoValueArray(CValuePtrArray& AddedParams, unsigned int nParamsCount, CValue** ParamsArray)
{
	if (!nParamsCount)
		return;

	for(int i=0;i<nParamsCount;i++)
		AddedParams.Add(*ParamsArray++);
};

// Windows 2000/�� ��� ����� ?
bool IsSystem_Windows2000AndHigher()
{
	const BYTE dwMajorVersion_Windows2000AndHigher = 5;

	BYTE dwWindowsMajorVersion = LOBYTE(LOWORD(GetVersion()));
	if (dwWindowsMajorVersion < dwMajorVersion_Windows2000AndHigher)
		return false;
	
	return true;
}

static CFile::OpenFlags ofFileClassesReadMode = static_cast<CFile::OpenFlags>(CFile::modeRead | CFile::shareDenyNone);

static void _ReadFileToString(CFile& file, const CString& strFileName, CString& strDest)
{
	DWORD dwLength = file.GetLength();
	LPTSTR buff = strDest.GetBuffer(dwLength);
	
	file.Read(buff, dwLength);
	
	strDest.ReleaseBuffer(dwLength);
	file.Close();
}

// ����� ������������� ����������
void ReadFileToStringWithExceptions(const CString& strFileName, CString& strDest)
{
	CFile file(strFileName, ofFileClassesReadMode);
	_ReadFileToString(file, strFileName, strDest);
}

// ��� ����������, false - ���������, true - �������
bool ReadFileToStringWithoutExceptions(const CString& strFileName, CString& strDest)
{
	CFile file;
	if (!file.Open(strFileName, ofFileClassesReadMode))
		return false;

	_ReadFileToString(file, strFileName, strDest);
	return true;
}
