// Syntax.cpp: implementation of the CSyntax class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <boost\scoped_ptr.hpp>

#include "vis1cpp.h"
#include "vis1cpp_my.h"

#include "Syntax.h"
#include "classinfo.h"
#include "mymodule.h"
#include "txtget.h"

#include "WrapperForSyntaxCheck.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#include <afxmt.h>
//#include <AFXPRIV.H>

CTypedPtrList<CPtrList, CSyntax*> CSyntax::m_ErrorList; //CPtrList CSyntax::m_ErrorList;

long CSyntax::m_TotalError=0;
bool CSyntax::m_silent=0;
boost::scoped_ptr<CMyModule> CSyntax::m_pGM; //CMyModule* CSyntax::m_pGM=NULL;
POSITION CSyntax::m_CurPos=NULL;

bool CSyntax::m_bInsideCompileAll = false;

void CSyntax::ShowError()
{
	const int iCountOfAddedLinesBySpecialFunctionIntoClassText = 3;
	const int iErrorLine = m_LineNum - iCountOfAddedLinesBySpecialFunctionIntoClassText;
	assert(m_LineNum > iCountOfAddedLinesBySpecialFunctionIntoClassText);

	CString txt;
	txt.Format("(%i) %s  :%s", iErrorLine, m_MarkedLine, m_Description);
	if(!m_Ident.IsEmpty())
		txt=txt+" ("+m_Ident+")";

	// TODO ���� ������������ ��������� �������, ��� ������� ������ � ���� ���������
	//		�� ������� ��� ��������� ������ ����� ���������� ������
	//		"����� �������� ������ ��� ������� � ������ ���� �����"
	// Msg(...)
	// �� ��� ����� ������ � �������������� ����. ������� �� ��������� ������� OmMsgBox
	//
//LogErr("m_pInfo->m_ImplementPath %s", m_pInfo->GetPath());
	CString strFilePath = m_pInfo->GetPath();
	int nCode = -1; // ������� ����

	if (!FileExists(strFilePath))
	{
		//strFilePath = GetInternalPath(m_pInfo->m_ImplementPath);
		strFilePath = "CalcVar";
//LogErr("strFilePath %s", strFilePath);

		//nCode = 41; // ����� ����������� �����������
		nCode = GetCalcVarMetaID(m_pInfo->GetPath()); // ����� ����������� �����������
	}

	pMod->OnErrorMessageEx(txt, nCode, strFilePath, m_LineNum);
}

CSyntax::CSyntax(CMyModule* pModA) : m_pInfo(pModA->m_pInfo), pMod(pModA)
{
	if(m_silent)
		return;
	m_Ident		=pMod->GetSyntaxErrIdent();
	m_LineNum	=pMod->GetSyntaxErrLineNum();
	m_MarkedLine=pMod->GetSyntaxErrMarkedLine();
	m_MarkedLine.TrimLeft();
	m_MarkedLine.TrimRight();
	pMod->GetSyntaxErrDescr(pMod->GetSyntaxErrCode(),m_Description);

	bool first=false;
	if(!m_ErrorList.GetCount())
		first=true;
	else if(m_pInfo!=(m_ErrorList.GetTail())->m_pInfo)
		first=true;
	if(first)
	{
		if(m_TotalError)
			Log("",mmNone);
		if(m_pInfo)
			Log("==== �����: %s  ����: %s ====",mmBlueTriangle,false,
				m_pInfo->GetDisplayName(),m_pInfo->GetPath());
		else
			Log("==== ���������� ������ ====",mmBlueTriangle);
	}
	m_TotalError++;

	m_ErrorList.AddTail(this);

	ShowError();
}

CSyntax::~CSyntax()
{
}

void CSyntax::OpenNextError(bool prev)
{
	if(!m_TotalError)
		return;
	if(!m_CurPos)
	{
		if(prev)
			m_CurPos=m_ErrorList.GetTailPosition();
		else
			m_CurPos=m_ErrorList.GetHeadPosition();
	}
	else
	{
		if(prev)
			m_ErrorList.GetPrev(m_CurPos);
		else
			m_ErrorList.GetNext(m_CurPos);
	}
	if(!m_CurPos)
	{
		Log("����� �� %s ������ ������.",mmInformation,false,prev?"������":"�����");
		return;
	}
	CSyntax* pSynt = m_ErrorList.GetAt(m_CurPos);
	CDocument* pDoc=pSynt->m_pInfo->OpenClass();
	if(!pDoc)
	{
		Log("�� ������� ������� ������ ������ %s", mmExclamation, false, pSynt->m_pInfo->GetDisplayName());
		DeleteError(prev);
		OpenNextError(prev);
		return;
	}
	CBodyTextView* pEditor=CCPPView::FindTextView(pDoc);
	if(!pEditor)
	{
		Log("�� ������� ������� ������ ������ %s", mmExclamation, false, pSynt->m_pInfo->GetDisplayName());
		DeleteError(prev);
		OpenNextError(prev);
		return;
	}
	CBodyTextDoc* pTxt=pEditor->GetDocument();

	int line=pSynt->m_LineNum-1,curline=line;
	CString fnd=pSynt->m_MarkedLine,src;
	fnd=fnd.Mid(fnd.Find("<<?>>")+5);
	fnd.MakeLower();
	bool found=false;
	int delta=1,maxline=pTxt->GetLineCount();
	do{
		src=pTxt->GetLineAsString(curline);
		src.MakeLower();
		if(src.Find(fnd)!=-1)
			found=true;
		else
		{
			curline+=delta;
			if(curline>maxline || curline>line+20)
				curline=line-1,delta=-1;
			if(curline<0 || curline<line-20)
				break;
		}
	}while(!found);
	if(found)
		line=curline;
	
	pEditor->MoveCaret(CPoint(0,line),CPoint(pTxt->GetLineLength(line)-2,line),1);

	pSynt->ShowError();

	if(found)
		pSynt->m_LineNum=line+1;
	else
	{
		Log("�� ������� ����� � ������ ������ ������. ������ ������� �� ������.",mmInformation);
		DeleteError(prev);
	}
}

void CSyntax::DeleteGM(void)
{
// 	delete m_pGM;
// 	m_pGM = NULL;
	m_pGM.reset();
}

void CSyntax::CompileGM()
{
	pBkEndUI->DoStatusLine("�������� ��...");
	CTxtGet get;
	CString src = get.GetGMText();
	
	DeleteGM();
	m_pGM.reset( new CMyModule(src) ); // m_pGM = new CMyModule(src);

	pBkEndUI->DoStatusLine("�������� ��...");
	
	m_pGM->Compile();
}

void CSyntax::IfClassThenAddSpecialFunctionIntoClassText(CString* pClassText)
{
	const int iFirstFunction = pClassText->Find("�������");
	const int iFirstProc = pClassText->Find("���������");

	int iFirstMethod = -1;
	if ((-1 == iFirstFunction) || (-1 == iFirstProc)){
		iFirstMethod = max(iFirstProc, iFirstFunction);
	}
	else
		iFirstMethod = min(iFirstProc, iFirstFunction);
	
	if (-1 != iFirstMethod){
		CString srcNew;
		if (0 != iFirstMethod)
			srcNew = pClassText->Left(iFirstMethod); 
		
		srcNew += " \r\n������� ����() ������� 1; ������������ \r\n";
		srcNew += "������� �() ������� 1; ������������ \r\n";
		srcNew += pClassText->Mid(iFirstMethod);
		*pClassText = srcNew;
	}
}

BOOL CSyntax::Compile(CClassInfo* pInfo, bool bCallOrigCompile)
{
 	if(!pInfo)
		RuntimeError("�������� ����� CSyntax::Compile");

	CString src = pInfo->GetSourceText(); //pInfo->m_mod.GetSrc(true);

	IfClassThenAddSpecialFunctionIntoClassText(&src);

	CMyModule* pMod = new CMyModule(src, pInfo);
	pBkEndUI->DoStatusLine(CString("�������-�������� ������ <")+pInfo->GetDisplayName()+">");
	
	pMod->AssignFriendModule(m_pGM.get());
	
	if(!bCallOrigCompile)
		return pMod->Compile();

	return CWrapperForSyntaxCheck::CallOrigCBLModuleCompile(pMod);
}

struct CFunctor_CompileSomeClassesForClassInfo
{
	void operator() (const CClassInfo::CPtr& pClassInfo) const
	{
		pClassInfo->CompileSomeClassesWithFlagCompileAll();		
	}
};

void CSyntax::CompileAll()
{
	Clear();
	CString msg=pBkEndUI->GetStatusLine();
	m_silent=true;

	//CWrapperForSyntaxCheck::bShowSuccessMessage = false;
	CompileGM(); //Compile(NULL);

	if(!m_pGM->IsCompiled())
	{
		if(AfxMessageBox("���������� ������ � ��������. ���������� ��������?",MB_YESNO|MB_ICONEXCLAMATION)==IDNO)
			return;
	}
	m_silent=false;

	CClassInfo::ForEachClassInfo( CFunctor_CompileSomeClassesForClassInfo() );

	if(m_TotalError)
		Log("����� ������: %i",mmInformation,false,m_TotalError);
	else
		Log("�� ������ ��������",mmInformation);
	
	DeleteGM();
	
	pBkEndUI->DoStatusLine(msg);

	//CWrapperForSyntaxCheck::bShowSuccessMessage = true;
}

BOOL CSyntax::CompileOne(CClassInfo *pInfo, bool bWithGM, bool bCallOrigCompile)
{
	BOOL bRet = false;

	static bInsideHandlerCompileOne = false;
	if (bInsideHandlerCompileOne)
		return bRet;
	
	bInsideHandlerCompileOne = true;

	Clear();
	CString msg=pBkEndUI->GetStatusLine();
	m_silent=true;
	if(bWithGM)
	{
		//CWrapperForSyntaxCheck::bShowSuccessMessage = false;
		
		CompileGM(); //Compile(NULL);
		
		//CWrapperForSyntaxCheck::bShowSuccessMessage = true;

		if(!m_pGM->IsCompiled())
		{
//			if(AfxMessageBox("���������� ������ � ��������. ���������� ��������?",MB_YESNO|MB_ICONEXCLAMATION)==IDNO)
//				return;
			
			// TODO ����� �� ���������� false ?
		}
	}
	m_silent=false;
	
	bRet = Compile(pInfo, bCallOrigCompile);
	if (m_TotalError)
		bRet = false;

	if (!m_bInsideCompileAll)
		if(m_TotalError)
			Log("����� ������: %i",mmInformation,false,m_TotalError);
		else
			Log("����� %s: ���������. ��� ���.", mmInformation, false, pInfo->GetDisplayName());
	
	DeleteGM();
	
	pBkEndUI->DoStatusLine(msg);

	bInsideHandlerCompileOne = false;

	return bRet;
}

BOOL CSyntax::CompileSomeClasses(CClassInfo *pInfo, bool bWithGM, bool bCallOrigCompile)
{
//	CSyntax::CompileOne(pInfo, true);

	// TODO ����� ���������� �� ��������� ������ �� ��������� �����
	// ��. ����
	CModuleString src = pInfo->GetSourceText(); //pInfo->m_mod.GetSrc(true);

	// ������ ����� �������, ��������� � ���� ����� � ������� �������� //# ClassBegin � //# ClassEnd
	boost::scoped_ptr<CMapStringToString> pClassMap ( new CMapStringToString );

	bool bSuccess = src.DivideTextOnClasses(pClassMap.get());
	if (!bSuccess)
		return CSyntax::CompileOne(pInfo, true);

	//CWrapperForSyntaxCheck::DisableHideMessageWindow();

	BOOL bRet = true;

	// �������������� ����� ������� � ����� �����
	CString strClassName, strClassSource;
	for(POSITION pos = pClassMap->GetStartPosition();pos;)
	{
		pClassMap->GetNextAssoc(pos, strClassName, strClassSource);

		CClassInfo::CPtr pInfo = CClassInfo::FindElem(strClassName);
		if (!pInfo)
		{
			//CWrapperForSyntaxCheck::EnableHideMessageWindow();
			Msg("������ �������� ���� //#ClassBegin � ClassEnd ��� ������ <%s>", strClassName);
			return false;
		}

		pInfo->SetSourceText(strClassSource); // ����� ��� ���������� ������ ������ ����� ������
		bRet = CSyntax::CompileOne(pInfo.get(), true) ? bRet : false;

		// ����� ��� ��������� ���������� ������ pInfo->GetSourceText() ���� ����� ������ ������ ������
		pInfo->SetSourceText("");
	}
	//CWrapperForSyntaxCheck::EnableHideMessageWindow();

	//delete pClassMap;

	return bRet;
}

// ���������� true, ���� ������������� �����, false - ���� �����
bool CSyntax::CompileIfClass(const CString& strDocFullPath1, bool bDisableSystemAddCompile, BOOL& bCompileResult)
{
	CString strDocFullPath = strDocFullPath1;
//LogErr("1 strFullPath %s", strDocFullPath);
//MessageBox("1 strFullPath %s", strDocFullPath);

	if (!FileExists(strDocFullPath))
	{
		CString strTemp = strDocFullPath;
		strTemp.Replace("���������.","");
		strTemp.Replace(".�����.������","@md"); 
		strTemp.Replace(".�����","@md"); 

		// TODO ��������� �������������������
		strDocFullPath = strTemp;
//LogErr("2 strFullPath %s", strDocFullPath);
//MessageBox("2 strFullPath %s", strDocFullPath);
	}

	if (strDocFullPath.IsEmpty())
		return false;

	CClassInfo* pInfo = NULL;
	//if (!CClassInfo::m_ClassFileNameMap.Lookup(strDocFullPath, pInfo))
	if (!CClassInfo::LookupByFileName(strDocFullPath, pInfo))
		return false;

//LogErr("pInfo->m_DisplayName %s", pInfo->m_DisplayName);
//MessageBox("pInfo->m_DisplayName %s", pInfo->m_DisplayName);

	if(bDisableSystemAddCompile)
	{
		CSyntax::bEnableCompileGM = !bDisableSystemAddCompile;
		CSyntax::bEnableCompileModule = !bDisableSystemAddCompile;
// 				CWrapperForSyntaxCheck::bEnableCompileGM = !bDisableSystemAddCompile;
// 				CWrapperForSyntaxCheck::bEnableCompileModule = !bDisableSystemAddCompile;
	}

	// TODO �������� ���������� ��������
	// 1. ����� ��������� ������� � ����� ����� (ClassBegin � ClassEnd)
	// 2. ����� � ����� ���-����� � �����, � ������������ ��������� (_NOW_PREPARE_CLASS)

	bCompileResult = CompileSomeClasses(pInfo, true);

	if(bDisableSystemAddCompile)
	{
		CSyntax::bEnableCompileGM = bDisableSystemAddCompile;
		CSyntax::bEnableCompileModule = bDisableSystemAddCompile;
// 				CWrapperForSyntaxCheck::bEnableCompileGM = bDisableSystemAddCompile;
// 				CWrapperForSyntaxCheck::bEnableCompileModule = bDisableSystemAddCompile;
	}

	//if (!pMod->Compile())
	//	Msg("���� ������");
	return true;
}

void CSyntax::Clear()
{
	DeleteGM();

	for(POSITION pos=m_ErrorList.GetHeadPosition();pos;)
		delete m_ErrorList.GetNext(pos);
	m_ErrorList.RemoveAll();
	m_CurPos=NULL;
	m_TotalError=0;
}

void CSyntax::DeleteError(bool prev)
{
	if(!m_CurPos)
		return;
	POSITION pos=m_CurPos;
	CSyntax* pSynt;
	if(prev)
		pSynt = m_ErrorList.GetPrev(m_CurPos);
	else
		pSynt = m_ErrorList.GetNext(m_CurPos);
	m_ErrorList.RemoveAt(pos);
	m_TotalError--;
	delete pSynt;
}

BOOL CSyntax::SyntaxCheck()
{
	CGetDoc7* pDoc = GetActiveDocument();
	if (pDoc)
	{
		CString strDocFullPath = GetDocumentPath(pDoc);
		BOOL bCompileResult = false;
		if (CSyntax::CompileIfClass(strDocFullPath, true, bCompileResult))
			return bCompileResult;
	}

	return false;
}

//bool CSyntax::bInsideCompileHandler = false;
bool CSyntax::bEnableCompileGM = true;
bool CSyntax::bEnableCompileModule = true;

int CSyntax::ModuleCompile(CBLModule7* pMod, bool& bInsideCompileHandler)
{
	CString FullFileName = pMod->GetFullName();
//LogErr("ModuleCompile pMod %d <%s> pMod->GetKind() %d", pMod, FullFileName, pMod->GetKind());
//MessageBox("pMod %d <%s> pMod->GetKind() %d", pMod, FullFileName, pMod->GetKind());
//	if (FullFileName.IsEmpty() && IS_KINDOF_RUNTIME_CLASS(pMod, CMyModule))
//		FullFileName = ((CMyModule*)pMod)->m_pInfo->GetImplementPath();
	bool bIsCMyModule = 8 == pMod->GetKind();
	CMyModule* pMyModule = bIsCMyModule ? static_cast<CMyModule*>(pMod) : NULL;

	bool bGM = "���������� ������" == FullFileName || (bIsCMyModule && !pMyModule->m_pInfo);

	if (!bIsCMyModule) // CMyModule
		if (!FullFileName.IsEmpty() && !bGM)
		{
				//CString FullFileName = pMod->GetFullName();
				BOOL bCompileResult = false;
				if (CSyntax::CompileIfClass(FullFileName, false, bCompileResult))
				{
					bInsideCompileHandler = false;
					return bCompileResult;
				}

		}		

	bInsideCompileHandler = false;
	//return pMod->Compile();
	bool bEnable = bGM && bEnableCompileGM;
	if (!bEnable)
		bEnable = !bGM && bEnableCompileModule;

	int ret = true;
	if (bEnable)
	{
// if (FullFileName.IsEmpty())
// {
// 	if (((CMyModule*)pMod)->m_pInfo)
// 		LogErr("����������� <%s>, pMod->GetKind() <%d>!", ((CMyModule*)pMod)->m_pInfo->GetImplementPath(), pMod->GetKind());
// }
// else
// 	LogErr("����������� <%s>, pMod->GetKind() <%d>!", FullFileName, pMod->GetKind());

		ret = pMod->Compile();
		if (bIsCMyModule)
		{
			if (bGM)
				bEnableCompileGM = false;
			else
				bEnableCompileModule = false;
		}

// 		if (ret)
// 			if(bShowSuccessMessage || (bGM && !bIsCMyModule))
// 				Msg(szSyntaxCheckNoError);
	}
	else
	{
		bEnableCompileGM = true;
		bEnableCompileModule = true;
	}
	
	return ret;
}

void CSyntax::CompileSomeClassesWithFlagCompileAll( CClassInfo* pClassInfo )
{
	// ����� �� ���� ��������� �� ������ ����������� ����
	EnableFlagCompileAll();
		CompileSomeClasses(pClassInfo, true);
	DisableFlagCompileAll();
}