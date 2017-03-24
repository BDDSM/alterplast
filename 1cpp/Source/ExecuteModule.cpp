// ExecuteModule.cpp: implementation of the CExecuteModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "ExecuteModule.h"
#include "CodeDecodeFile.h"

#include ".\\System\\Informer.h"

#include "BLModuleWrapper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CMetaDataCont * pMetaDataCont;
extern CBkEndUI * pBkEndUI;

IMPLEMENT_DYNCREATE(CExecuteModule, CBLContext);


class CObjID CExecuteModule::ObjID;

struct CExecuteModule::paramdefs CExecuteModule::defFnNames[] = {
	{{"Start", "�����"},0,0},
	{{"AssignSource", "����������������"},0,1},
	{{"AssignContext", "�����������������"},0,1},
	{{"Compile", "�������������������"},1,0},
	{{"Execute", "���������������"},1,0},
	{{"AssignFriendModule", "�������������������������"},0,0},
	{{"HasRetVal", "������������������"},1,2},
	{{"GetDescr", "����������������"},0,2},
	{{"GetTextCurModules", "����������������������"},1,1},
	{{"WorkWithMD", "��������"},0,0},
	{{"GetInfo", "������������������"},0,1},
	{{"GetMethOfContext", "GetMethOfContext"},0,3},
	{{"RaiseError", "������������������"},0,1},
	{{"ErasePostState","�������������������������"},0,0},  
	{{"RestorePostState","�����������������������������"},0,0},    
	{{"ExInfo","ExInfo"},0,0},      
	{{"InfoAboutCurExeMeth","InfoAboutCurExeMeth"},0,0},      
	{{"GetExeption","������������������"},1,0},
	{{"Throw","�������������"},0,2},  
	{{"GetProcInfo","����������������������������"},1,4},  
	{{"CompileAndExecute", "�����������������������������"},0,0},
	{{"EnableExceptions", "�������������������"},0,0},
	
	{{"CallContextProcedure", "���������������������������"}, 1,3},
	{{"CallContextFunction", "�������������������������"}, 1,4},

	{{"EnableInsideCalling", "������������������������������"}, 0,0},
	{{"GetModuleText", "�������������������"}, 1,0},

	{NULL,NULL,0,0}
};

/*
BOOL CALLBACK EnumWindowsProc(HWND hWindow, LPARAM param)
{
	static char szWindowName[71];
	static char szClassName[31];
	static char szAddString[101];
	
    // Get the window name and class name
    //...................................
    GetWindowText( hWindow, szWindowName, 70 );
    GetClassName( hWindow, szClassName, 30 );
	
    // Build a string and add it to the list box.
    //...........................................
    wsprintf( szAddString, "%s - %s", szClassName, szWindowName );
	pBkEndUI->DoMessageLine(szAddString, mmBlueTriangle);	
    
	
    return( TRUE );
}
*/

CExecuteModule::CExecuteModule(int iPar):CBLContext(iPar), 
	//pBLModule (new CBLModuleWrapper("", NULL, NULL, NULL)),
	pBLModule (new CBLModuleWrapper("", NULL, NULL)), // artbear kms TODO
	pPoint(NULL), m_bEnableThrow(false), m_bInsideMethodCall(false), m_bEnableInsideCalling(false)
{
}

CExecuteModule::~CExecuteModule()
{
	delete pBLModule;
}

// class CInsideExecuteModuleGuard
// {
// private:
// 	CExecuteModule* m_pExecuteModuleCont;
// public:
// 	CInsideExecuteModuleGuard(CExecuteModule* pExecuteModuleCont) :
// 		m_pExecuteModuleCont(pExecuteModuleCont)
// 	{
// 		m_pExecuteModuleCont->m_bInsideMethodCall = true;
// 	};
// 	~CInsideExecuteModuleGuard()
// 	{
// 		m_pExecuteModuleCont->m_bInsideMethodCall = false;
// 	}
// };


int  CExecuteModule::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{
	if (!m_bEnableInsideCalling && m_bInsideMethodCall && funcGetExeption != iMethNum)
		RuntimeError("��� ������ ����������������� ����������� ������ ���������");

	//CInsideExecuteModuleGuard guard(this);
	CFlagGuard guard(this->m_bInsideMethodCall, true);

	CValue* pValue = NULL;
	CBLContext* pContext = NULL;
	
	int iAnswer=-1;
	int iNumMethod=-1;
	
	try
	{
		switch(iMethNum)
		{
		case methCompile:
			{
// artbear
//			rValue = pBLModule->Compile();
			if (!m_bEnableThrow)
				rValue = pBLModule->Compile();
			else
				try{
					rValue = pBLModule->Compile();
				}
				catch(CString &err)
				{
					RuntimeError(err);
					return 1;
				}
				catch(...)
				{
					RuntimeError("����������� ������");
					return 1;
				}
			break;
			}
// ����������
		case methExecute:
// artbear
//			rValue = pBLModule->Execute();
//			break;
			{
			if (!m_bEnableThrow)
				rValue = pBLModule->Execute();
			else
				try{
					rValue = pBLModule->Execute();
				}
				catch(CString &err)
				{
					RuntimeError(err);
					//rValue = CValue(long(0));
					return 1;
				}
				catch(...)
				{
					RuntimeError("����������� ������", 0);
					//rValue = CValue(long(0));
					return 1;
				}
			break;
			}
// ����������
		case methHasRetVal:
			pValue = ppValue[0];
			pContext = pValue->GetContext();
			
			if (!pContext)
			{
				rValue = CNumeric(iAnswer); // ���������� ����������
				break;
			}
			pValue = ppValue[1];
			iNumMethod = pContext->FindMethod(pValue->GetString());
			if (iNumMethod != -1)
			{
				iAnswer = pContext->HasRetVal(iNumMethod);
				rValue = CNumeric(iAnswer);
			}
			break;
		case methGetTextModules:
			{
/*				
				CBLModule *pCurModule = NULL;
				pCurModule = CBLModule::GetExecutedModule();
				if (pCurModule == NULL) {
					pBkEndUI->DoMessageLine("��������� ������� �������� ������� ������!",mmRedErr);
					break;
				}
				//char str[] = "Message(""Good"");";
				//pCurModule->AssignSource(str);
				
				/*
				
				  CBLContext* pValueList = CBLContext::CreateInstance("��������������");
				  if (pValueList == NULL) {
				  pBkEndUI->DoMessageLine("��������� ������� ������� ������ ��������!",mmRedErr);
				  break;
				  }
				  int iNumMethod = pValueList->FindMethod("AddValue");
				  
					for (int ind = 0; ind < 10; ind++) {
					char c[100];
					sprintf(c, "����� ���� �%d", ind);
					pValue = new CValue(c);
					pValueList->CallAsProc(iNumMethod, &pValue);
					}
					iNumMethod = pValueList->FindMethod("���������������");
					//char charbuff[20];
					//_itoa( iNumMethod, charbuff, 10 );
					//pBkEndUI->DoMessageLine(charbuff, mmRedErr);
					
					  CValue ValReturn;
					  CValue* pMassPar[5] = {NULL};
					  pMassPar[0] = &rValue;
					  CValue ValCaption("Choose value!");
					  pMassPar[1] = &ValCaption;
					  CValue Pos(0L);
					  pMassPar[2] = &Pos;
					  CValue TimeOut(0L);
					  pMassPar[3] = &TimeOut;
					  CValue methodOfChoose(2L);
					  pMassPar[4] = &methodOfChoose;
					  
				pValueList->CallAsFunc(iNumMethod, ValReturn, pMassPar);*/
				
				//EnumWindows(EnumWindowsProc, NULL);
				/*ATOM atom1C = GlobalFindAtom("Afx:400000:0:0:0:c704b9");
				if (atom1C =! 0)
				pBkEndUI->DoMessageLine("Find atom 1C", mmExclamation3);	
				
				  char *pChar = NULL;
				  //pChar = 
				  HWND hCurWindow = FindWindow( atom1C, NULL); //"1�:����������� - ������������� ����, �������� 4.0");
				  if (hCurWindow == NULL) {
				  char buf[100];
				  sprintf(buf, "Error: %d", GetLastError());
				  pBkEndUI->DoMessageLine(buf, mmExclamation3);	
				  }
				  else {
				  EnumChildWindows(hCurWindow, EnumWindowsProc, NULL);
				  HWND hChildWindow = FindWindowEx(hCurWindow, NULL, NULL, "My window 1");
				  if (hChildWindow != NULL)
				  pBkEndUI->DoMessageLine("Find window: My window 1", mmExclamation3);	
				  else
				  pBkEndUI->DoMessageLine("Not find window: My window 1", mmExclamation3);	
				  
			}* /
				//ZeroMemory(charbuff, sizeof(charbuff));
				//_itoa(ValReturn.GetNumeric(), charbuff, 10 );
				//pBkEndUI->DoMessageLine(charbuff, mmExclamation3);
				
				
				//rValue.AssignContext(pValueList);
				
				//delete pValueList;
				
				
				CBLContext *pCurContext = ppValue[0]->GetContext();
				pMod->AssignContext(pCurContext);
				CString str(32, 500);
				str = "��������� ��(���1) \
											����� = \"�� � ������ ����� �������� ����������\"; \
											��������(\"��������� ��!\"+ strWrite); \
											��������(\"�������� 1 = \"+ ���1); \
											�������������� \
											������� ����(���1) \
											��������(\"������� ����!\"); \
											��������(\"�������� 1 = \"+ ���1); \
											������� 1; \
											������������ \
											��(\"����� ������ ���������� �����!\"); \
											";
				pMod->AssignSource(str);
				
				/*
				// int GetNCurSourceLines()
				// ���������� ���������� ����� � ������
				int countLine = pMod->GetNCurSourceLines();
				CString string(32,1000);
				for (int line = 1; line <= countLine; line++) {
				pMod->GetCurSourceLine(line, string);
				pBkEndUI->DoMessageLine(string,mmBlackErr);
				}* /
				
				pMod->AssignFriendModule(pCurModule);
				pMod->Compile();
				pMod->Execute();
				
				// FindProc(const char *, int) - ������� ����� ������� ��� ��������� � ������
				// 1: ������ � ��������� ������� ��� ���������
				// 2: 0 - ���������, 1 - �������
				int iProc = pMod->FindProc("��", 0);
				CValue *ppPar[1] = {new CValue("Try !")};
				// CallAsProc(int, int, CValue **) - �������� ��������� ������������������ ������
				// 1: ����� ���������
				// 2: ���������� ����������, ���� ���, �� 0
				// 3: ������ ���������� �� ���������
				// �������: 0 - ���� ����� �� �������
				int res = pMod->CallAsProc(iProc, 1, ppPar);
				if (res == 0)
					pBkEndUI->DoMessageLine("������ ������ ��������� � ������ ExecuteModule", mmExclamation3);	
				
				CValue value;
				iProc = pMod->FindFunc("����");
				//CallAsFunc(int, CValue&, int, CValue** )
				// 1: ����� ���������
				// 2: �������� ���������� �� ��������� �������
				// 3: ���������� ����������, ���� ���, �� 0
				// 4: ������ ���������� �� ���������
				// �������: 0 - ���� ����� �� �������
				
				res = pMod->CallAsFunc(iProc, value, 1, ppPar);
				if (res == 0)
					pBkEndUI->DoMessageLine("������ ������ ������� � ������ ExecuteModule", mmExclamation3);	
				
				pMod->Reset();
				pMod->Unload();
				
				// ������� ������� ��������� ������ ������ ������������ �����
				iProc = pCurModule->FindProc("�������������",0);
				res = pCurModule->CallAsProc(iProc, 1, ppPar);
				if (res == 0)
					pBkEndUI->DoMessageLine("������ ������ ��������� ��������� ������ � ������ ExecuteModule", mmExclamation3);	
				
*/				
				
			}
			break;
    case funcGetExeption:
		{
			CRuntimeExceptionClass::GetException(rValue);
			return 1;
		}
		break;
    case funcGetProcInfo:
        {
            int nIndex;
            if (ppValue[0]->GetNumeric().operator long())
                nIndex = pBLModule->FindProc(ppValue[1]->GetString(), 0);
            else
                nIndex = pBLModule->FindFunc(ppValue[1]->GetString());
            if (nIndex == -1)
                rValue = 0L;
            else
            {
                rValue = 1;
                CBLProcInfo ProcInfo;
                if (pBLModule->GetProcInfo(nIndex, ProcInfo))
                {
                    *ppValue[2] = ProcInfo.GetNParams();
                    *ppValue[3] = ProcInfo.IsExported();
                }
            }
			
        }
        break;
    case iFuncCallContextProcedure:
		{
			bool bRes = CallContextProcedure(*ppValue[0], *ppValue[1], *ppValue[2]);
			rValue = bRes ? 1L : 0L; 
		}
		break;
    case iFuncCallContextFunction:
		{
			bool bRes = CallContextFunction(*ppValue[0], *ppValue[1], *ppValue[2], *ppValue[3]);
			rValue = bRes ? 1L : 0L; 
		}
		break;
		
	case iFuncGetModuleText:
		{
			CString strModuleText = GetModuleText();
			rValue = strModuleText;
			return TRUE;
		}

    default:
			CBLProcInfo ProcInfo;
			pBLModule->GetProcInfo(iMethNum-lastMethod,ProcInfo);
			int j = ProcInfo.GetNParams();
			int i = pBLModule->CallAsFunc(iMethNum - lastMethod,rValue,j,ppValue);

			return i;
		};
		return 1;
	}
// artbear
  catch(CString &err)
  {
	RuntimeError(err);
  }
// ����������
// 	catch(...)
// 	{
// 
// 		pBkEndUI->DoMessageLine("Destroying ExecuteModule class",mmInformation);
// 	}
	return 1;
}

// artbear
void CExecuteModule::GetMethodOfContext(class CValue * * ppValue) const
{
	CInformer::GetMethodOfContext(ppValue);
}

int  CExecuteModule::CallAsProc(int iMethNum,class CValue * * ppValue)
{
	if (!m_bEnableInsideCalling && m_bInsideMethodCall)
		RuntimeError("��� ������ ����������������� ����������� ������ ���������");

	//CInsideExecuteModuleGuard guard(this);
	CFlagGuard guard(this->m_bInsideMethodCall, true);

	CValue* pValue = NULL;
	CBLContext* pContext = NULL;
	char buff[200];
	CType* pType = NULL;
	int iNumMethod=-1;

	switch(iMethNum)
	{
	case methStart:
		//pBkEndUI->DoMessageLine("Proc is Start",mmInformation);
		{
			ULONG first = CBLContext::GetFirstLoadedContextID();
			
			for (ULONG ind = first; ind != 0; ind = CBLContext::GetNextLoadedContextID(ind))
			{
				CBLContext* pCont = CBLContext::GetLoadedContext(ind);
				if (pCont)
				{
					pBkEndUI->DoMessageLine(pCont->GetRuntimeClass()->m_lpszClassName, mmInformation);
					
					int nMeth = pCont->GetNMethods();
					if (nMeth > 0)
						pBkEndUI->DoMessageLine("==============================\r\nMethods\r\n==============================", mmInformation);
					for (int i = 0; i < nMeth; i++)
					{
						char buf[1000] = {0};
						char whatIs[100] = {0};
						if (pCont->HasRetVal(i)==1)
							strcpy(whatIs, "Function ");
						else
							strcpy(whatIs, "Procedure ");
						
						int nParams = pCont->GetNParams(i);
						
						sprintf(buf, "%d. %s [%s]  [%s] nParams = %d",i,whatIs,pCont->GetMethodName(i, 0), pCont->GetMethodName(i, 1), nParams);
						pBkEndUI->DoMessageLine(buf, mmInformation);
					}        
					int nProps = pCont->GetNProps();
					if (nProps > 0)
						pBkEndUI->DoMessageLine("==============================\r\nProperties\r\n==============================", mmInformation);
					
					for (i = 0; i < nProps; i++)
					{
						char buf[1000] = {0};
						char whatIs[100] = {0};
						
						if (pCont->IsPropReadable(i)==0)
							strcpy(whatIs, "Write only ");
						if (pCont->IsPropWritable(i)==0)
							strcpy(whatIs, "Read only ");
						
						//int nParams = pCont->GetNParams(i);
						
						sprintf(buf, "%d. [%s]  [%s]  %s",i,pCont->GetPropName(i, 0), pCont->GetPropName(i, 1), whatIs);
						pBkEndUI->DoMessageLine(buf, mmInformation);
					}
				}
			}
		}
		break;
	case methAssignSource:
		{
		/*CType type(3);    
		CString *str = new CString();
		*str = type.GetTypeTitle().operator LPCTSTR();
		pBkEndUI->DoMessageLine(*str,mmInformation);
			delete str;*/
			//::operator delete (str);
			
			//CString str = type.GetTypeTitle();
			
			//delete str;
			//const char* pstr = 
			
			
			pBLModule->AssignSource(ppValue[0]->GetString());
		}
		break;
	case methAssignContext:
		pBLModule->AssignContext(ppValue[0]->GetContext());
		break;
	case methAssignFriendModule:
		pBLModule->AssignFriendModule(CBLModule::GetExecutedModule());
		break;
	case methGetContextDescr:
		{
			pValue = ppValue[0];
			pContext = pValue->GetContext();
			iNumMethod = pContext->FindMethod("AddValue");
			if (iNumMethod != -1)
			{
				int i;
				for (i = 0;i<lastMethod;i++)
				{
					pValue = new CValue(defFnNames[i].Names[ppValue[1]->GetNumeric()]);
					pContext->CallAsProc(iNumMethod, &pValue);
				}
			}
			ppValue[0]->AssignContext(pContext);
			
			long lId = CBLModule::GetFirstLoadedModuleID();
			sprintf(buff,"Id loaded module = %d",lId);
			pBkEndUI->DoMessageLine(buff,mmInformation);
			CBLModule* pModule = CBLModule::GetLoadedModule(lId);
			//pBLModule = CBLModule::GetExecutedModule();
			//pBLModule->ResetExecuted();
			while(lId = CBLModule::GetNextLoadedModuleID(lId))
			{
				sprintf(buff,"Id loaded module = %d",lId);
				pBkEndUI->DoMessageLine(buff,mmInformation);
			}
			
			
			break;
		}
	case methWorkWithMD:
		{
			//CMetaDataCont - ����� ��� ������ � �������� MD
			//pMetaDataCont - ��������� �� ������� MD ������
			//CDocDef - ����� �������� ���������
			
			CMetaDataObjArray *pMetaDataObjArray = pMetaDataCont->GetDocDefs();
			
			if (pMetaDataObjArray == NULL) {
				pBkEndUI->DoMessageLine("��������� ������� �������� �������� ����������", mmExclamation3);	
				break;
			}
			int countDefDoc = pMetaDataObjArray->GetNItems();
			for (int ind = 0; ind < countDefDoc; ind++) {
				CMetaDataObj *pMetaDataObj = pMetaDataObjArray->operator[](ind);
				if (pMetaDataObj == NULL) continue;
				
				// char const* GetCode() - �������� ������ � ��������������� �������
				// char const* GetDescr() - �������� ������ � ������������ �������
				// char const* GetPresent() - �������� ������ � ��������� �������
				
				pBkEndUI->DoMessageLine(pMetaDataObj->GetCode(), mmInformation);
				pBkEndUI->DoMessageLine(pMetaDataObj->GetDescr(), mmInformation);
				long lIDObject = pMetaDataObj->GetID();
				char buf[20];
				sprintf(buf, "Id = %d", lIDObject);
				pBkEndUI->DoMessageLine(buf, mmInformation);
				pBkEndUI->DoMessageLine(pMetaDataObj->GetPresent(), mmInformation);
				
				//_itoa(objDocDef.GetDocNumDefID(), buf, 10);
				CDocDef *pDocDef = pMetaDataCont->GetDocDef(pMetaDataObj->GetCode());
				if (pDocDef == NULL) {
					pBkEndUI->DoMessageLine("��������� ������� �������� �������� ���������", mmExclamation3);	
					break;
				}
				
				_itoa(pDocDef->GetBookKeepingEnable(), buf, 10);
				pBkEndUI->DoMessageLine(buf, mmInformation);
				pBkEndUI->DoMessageLine("======================", mmInformation);
			}
			//CBLContext* pValueList = CBLContext::CreateInstance("ValueList");
			CBLContext* pValueList = CBLContext::CreateInstance("����������.�����1");
			pBkEndUI->DoMessageLine(pValueList->GetRuntimeClass()->m_lpszClassName, mmInformation);
		}
		break;
	case methGetInfo:
		{
			const CString& c_str = ppValue[0]->GetString();
			pBkEndUI->DoMessageLine(pMetaDataCont->GetRegDef(c_str)->GetRegActTableName(), mmInformation);
			pBkEndUI->DoMessageLine(pMetaDataCont->GetRegDef(c_str)->GetRegTableName(), mmInformation);
			//class CBLContext *pBL = ppValue[0]->GetContext();
			//if (pBL != NULL) {		      
			//}
			CBLModule * pBLM = CBLModule::GetExecutedModule();	//53
			
			
		}
		break;
	case procGetContextMeth:
		{
// artbear
			try{
				GetMethodOfContext(ppValue);
				break;
			}
			catch(...){
				RuntimeError("������ ��������� ������� � ������� ���������");
			}
	}
// end

	case procRaiseError:
		{
			RuntimeError(ppValue[0]->GetString());
		}
		break;
	case procErasePostState:
		{
			try
			{
				unsigned char **ppPoint = reinterpret_cast<unsigned char **>(0X2A133620);
				pPoint = *ppPoint;
				if (pPoint)
				{
					*ppPoint = NULL;
				}
			}
			catch (...)
			{
				pBkEndUI->DoMessageLine("� ErasePostState() �� ������� �������� � ������ �� ������ 0X2A133620", mmBlackErr);	
			}
		}
		break;
	case procRestorePostState:
		{      
			if (pPoint)
			{
				try
				{
					unsigned char **ppPoint = reinterpret_cast<unsigned char **>(0X2A133620);
					*ppPoint = pPoint;
					pPoint   = NULL;
				}
				catch (...)
				{
					pBkEndUI->DoMessageLine("� RestorePostState() �� ������� �������� � ������ �� ������ 0X2A133620", mmBlackErr);	
				}
			}      
		}
		break;
	case procExInfo: 
		{
//	/* artbear ������ ����� ����������������
			CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();      
			CComponentClass* pCompClass = CComponentClass::GetComponentClassCurExModule();
			if (pCompClass)
			{
				CBLModule *pMod = pCompClass->GetHostModule();
				// ������ ����������� �� ���� ������ ������ ��� ��� ������ ������ 1�
				pCompClass = NULL;
				pCompClass = CComponentClass::GetComponentClassCurExModule(pMod);
				
				CString strCode("",500);
				int nLineNum = pMod->GetExecutedLineNum();
				pMod->GetCurSourceLine(nLineNum, strCode);
				strCode.TrimLeft(); strCode.TrimRight();
				
				CString strInfo;
				CString strFullName;
				if (pCompClass)
				{
					// ������ ����������� ������          
					strFullName = pCompClass->GetTypeString();
					strFullName += " = ";
					strFullName += pHierarchy->GetNameOfFile(pCompClass->GetTypeString());
					nLineNum++;
				}
				else
				{
					// ��� 1� ������
#ifndef _DEBUG
					strFullName = static_cast<CBLModule7*>(pMod)->GetFullName();          
#else
					strFullName = "���� � ������ 1� � DEBUG mode ���������� ������";
#endif
				} 
				strInfo.Format("{%s(%d)}",strFullName,nLineNum);        
				pBkEndUI->DoMessageLine(strCode, mmInformation);	        
				pBkEndUI->DoMessageLine(strInfo, mmInformation);	        
			}
		}
		break;
	case procInfoAboutCurExeMeth:
		{
			CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();      
			CComponentClass* pCompClass = CComponentClass::GetComponentClassCurExModule();
			if (pCompClass)
			{        
				CString strRes = pCompClass->GetTypeString();
				strRes += "::";
				strRes += GetExecuteMethInfo(pCompClass->GetModule());
				pBkEndUI->DoMessageLine(strRes, mmInformation);	                          
			}
			/*CArray<CBLModule7*,CBLModule7*> *rModArr = (CArray<CBLModule7*,CBLModule7*>*)0x22132C98;
			for (int i = 0; i < rModArr->GetSize(); i++)
			{
			pBkEndUI->DoMessageLine(rModArr->GetAt(i)->GetFullName(), mmInformation);	                
		}*/
		}
		break;
	case procThrow:
		{
			CRuntimeExceptionClass::RuntimeError(*ppValue[0], ppValue[1]->GetString());
		}
		break;
// artbear
	case methCompileAndExecute:
		{
			bool bOldEnableThrow = pBLModule->m_bEnableThrow;
			pBLModule->m_bEnableThrow = true;
			try{
				CValue res = pBLModule->Compile();
				if (0 == res.GetNumeric)
					throw CString("������ �� �������������");
				CValue res1 = pBLModule->Execute();
				if (0 == res1.GetNumeric)
					throw CString("������ �� ��������");
				break;
			}
			catch(CString &err)
			{
				pBLModule->m_bEnableThrow = bOldEnableThrow;
				RuntimeError(err);
				return 1;
			}
			catch(...)
			{
				pBLModule->m_bEnableThrow = bOldEnableThrow;
				RuntimeError("����������� ������");
				return 1;
			}
			pBLModule->m_bEnableThrow = bOldEnableThrow;
		}
		break;
    case procEnableExceptions:
		{
			pBLModule->m_bEnableThrow = true;

			m_bEnableThrow = true;
		}
        break;

	case iProcEnableInsideCalling:
		m_bEnableInsideCalling = true;
        return TRUE;
// end
	default:
		CBLProcInfo ProcInfo;
		pBLModule->GetProcInfo(iMethNum-lastMethod,ProcInfo);
		int j = ProcInfo.GetNParams();
		int i = pBLModule->CallAsProc(iMethNum - lastMethod,j,ppValue);

		return i;
	};

	return 1;
}


int  CExecuteModule::FindMethod(char const * lpMethodName)const
{
	int i;
	for (i = 0;i<lastMethod;i++){
		if (!stricmp(lpMethodName,defFnNames[i].Names[0]))
			return i;
		if (!stricmp(lpMethodName,defFnNames[i].Names[1]))
			return i;
	}
	i		= pBLModule->FindFunc(lpMethodName);
	i = pBLModule->FindFunc(lpMethodName);
	if (i>=0) 
		return i+lastMethod;
	i = pBLModule->FindProc(lpMethodName,0);
	if(i>=0)
		return i+lastMethod;
	else return i;
}

char const *  CExecuteModule::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	if (iMethodNum >= lastMethod) 
	{
		CBLProcInfo ProcInfo;
		pBLModule->GetProcInfo(iMethodNum-lastMethod, ProcInfo); //artbear
		return ProcInfo.GetName();
	}
	else
		return defFnNames[iMethodNum].Names[iMethodAlias];
}

int  CExecuteModule::GetNMethods(void)const
{
	return pBLModule->GetNProcs() + lastMethod;
}

int  CExecuteModule::HasRetVal(int iMethodNum)const
{
	CBLProcInfo ProcInfo;
	if (iMethodNum >= lastMethod) 
	{
		pBLModule->GetProcInfo(iMethodNum-lastMethod,ProcInfo);
		return ProcInfo.HasRetVal();
	}
	else
		return defFnNames[iMethodNum].HasReturnValue ;
}

int CExecuteModule::GetNParams(int iMethodNum)const
{
	CBLProcInfo ProcInfo;
	if (iMethodNum >= lastMethod) 
	{
		pBLModule->GetProcInfo(iMethodNum-lastMethod,ProcInfo);
		return ProcInfo.GetNParams();
	} 
	else
		return defFnNames[iMethodNum].NumberOfParams;
}

int  CExecuteModule::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{
	if (iMethodNum >= lastMethod)
	{
		return 1;
	}
    else if (iMethodNum == procThrow && iParamNum == 1)
    {
        *pDefValue = "";
        return 1;
    }
    else if (iMethodNum == funcGetProcInfo && (iParamNum == 2 || iParamNum == 3))
    {
        pDefValue->Reset();
        return 1;
    }
	else if (iMethodNum == procGetContextMeth && (iParamNum == 1 || iParamNum == 2))
	{
		*pDefValue = 0L;
		return 1;
	}
    return 0;
}

/*
void  CExecuteModule::DecrRef(void)
{
	CBLContext::DecrRef();
}
*/

char const *  CExecuteModule::GetCode(void)const
{
	return 0;
}

int  CExecuteModule::GetDestroyUnRefd(void)const
{
	return 1;
}

/*
void  CExecuteModule::GetExactValue(class CValue & vParam)
{
	CBLContext::GetExactValue(vParam);
}
*/

class CObjID   CExecuteModule::GetID(void)const
{
	return ObjID;
}

/*
class CBLContextInternalData *  CExecuteModule::GetInternalData(void)
{
	
	return CBLContext::GetInternalData();
	
}
*/

long  CExecuteModule::GetTypeID(void)const
{
	return 100;
}

char const *  CExecuteModule::GetTypeString(void)const
{
	return "�����������������";
}

class CType   CExecuteModule::GetValueType(void)const
{
	CType tType(100);
	
	return tType;
}

/*
void  CExecuteModule::IncrRef(void)
{
	CBLContext::IncrRef();
}

void  CExecuteModule::InitObject(class CType const & tType)
{
	CBLContext::InitObject(tType);
}

void  CExecuteModule::InitObject(char const * strName)
{
	CBLContext::InitObject(strName);
}*/

int  CExecuteModule::IsExactValue(void)const
{
	return 0;
}

int  CExecuteModule::IsOleContext(void)const
{
	return 0;
}

int  CExecuteModule::IsPropReadable(int iPropNum)const
{
	return 1;
}

int  CExecuteModule::IsPropWritable(int iPropNum)const
{
	return 1;
}

int  CExecuteModule::IsSerializable(void)
{
	return 0;
}

int  CExecuteModule::SaveToString(class CString & csStr)
{
	csStr = "DJK";
	return 1;
}

/*
void  CExecuteModule::SelectByID(class CObjID cID,long lNum)
{
	CBLContext::SelectByID(cID,lNum);
}
*/


int  CExecuteModule::GetNProps(void)const
{
	return pBLModule->GetNStaticVars();
}

char const *  CExecuteModule::GetPropName(int A,int B)const
{
	CBLVarInfo VarInfo;
	pBLModule->GetStaticVarDescr(A,VarInfo);
	return VarInfo.GetName();
}

int  CExecuteModule::GetPropVal(int iPropNum,class CValue & rValue)const
{
	return pBLModule->GetStaticVarValue(iPropNum,rValue,0);
}

int  CExecuteModule::SetPropVal(int iPropNum,class CValue const & vValue)
{
	return pBLModule->SetStaticVarValue(iPropNum,vValue,0);
}

int  CExecuteModule::FindProp(char const * Name)const
{
	return pBLModule->FindStaticVar(Name);
}


CString CExecuteModule::GetExecuteMethInfo(CBLModule *pModule)
{
	int res = 0;        
	int threePar = 0;
	CString strRet;
	
	if (pModule)
	{
		CBLProcInfo procInfo;
		
		res = pModule->GetCallLevelProcInfo(pModule->GetCallStackDepth()-1, procInfo, threePar);
		if (res != 0)
		{    
			CString strInfoPars;      
			for (int i = 0; i < procInfo.GetNParams(); ++i)
			{
				CBLVarInfo VarInfo;
				CString strInfoPar;
				procInfo.GetVarDescr(i, VarInfo);
				CValue ValRes;
				CValue *masVal[1] = {0};
				pModule->EvalExpr(VarInfo.GetName(),ValRes, masVal);            
				CString strType(ValRes.GetTypeString());
				
				if (ValRes.GetContext() == NULL)
					if (strType.Find("����������") != -1   || strType.Find("��������") != -1 || strType.Find("������������") != -1 )
					{              
						long lll = 0;                
						strType = pMetaDataCont->GetFullName(ValRes.GetRealTypeID(), &lll, 1);                 
					}
					else if (strType.Find("Referens") != -1 || strType.Find("Document") != -1 || strType.Find("Enum") != -1)
					{
						long lll = 0;                
						strType = pMetaDataCont->GetFullName(ValRes.GetRealTypeID(), &lll, 0);
					} 
					
					strInfoPar.Format("%s %s=%s",strType, VarInfo.GetName(), ValRes.GetString());                        
					if (procInfo.GetNParams() > i + 1)
						strInfoPar += ", ";              
					strInfoPars += strInfoPar;
			}    
			strRet.Format("%s(%s)", procInfo.GetName(), strInfoPars);
		}
	}
	return strRet;
}

// ��� �������������� �������� ����������
bool CExecuteModule::CallContextMethod(bool bIsFunction, CBLContext* pCont, const class CString& strMethodName, CValueListContext* pValueListCont, CValue* pRetValue) const
{
	// TODO �������� ����������� ���������� ���������� ��������� � ����� ������� ��� ���������

	int iMethodNum = pCont->FindMethod(strMethodName);
	if(-1 == iMethodNum)
		return false;

	if (!bIsFunction && pCont->HasRetVal(iMethodNum))
		RuntimeError("����� %s ����������, �� �������� ��������, � �������� ������� ���������", strMethodName);
	if (bIsFunction && !pCont->HasRetVal(iMethodNum))
		RuntimeError("����� %s ����������, �� �������� ��������, � �������� ������� ���������", strMethodName);

	int iMethodParams = pCont->GetNParams(iMethodNum);
	CCallWithValueListGuard guard(iMethodParams, pValueListCont);

	int iRes = 0;
	if (bIsFunction)
	{
		ASSERT(pRetValue);
		iRes = pCont->CallAsFunc(iMethodNum, *pRetValue, guard.GetData());
	}
	else
		iRes = pCont->CallAsProc(iMethodNum, guard.GetData());

	return iRes ? true : false;

}

bool CExecuteModule::CallContextProcedure(CBLContext* pCont, const class CString& strMethodName, CValueListContext* pValueListCont) const
{
	return CallContextMethod(false, pCont, strMethodName, pValueListCont);
}

bool CExecuteModule::CallContextFunction(CBLContext* pCont, const class CString& strMethodName, CValueListContext* pValueListCont, CValue& RetValue) const
{
	return CallContextMethod(true, pCont, strMethodName, pValueListCont, &RetValue);
}

// � �������������� ��������� ����������
bool CExecuteModule::CallContextMethod(bool bIsFunction, const CValue& ValueContext, const CValue& ValueMethodName, const CValue& ValueArgList, CValue* pRetValue) const
{
	ValidateParamContext(ValueContext, CString("����� <")+GetTypeString()+"> - ������ ���������� ������ ���� ������� ��������");
	ValidateParamNotEmptyString(ValueMethodName, CString("����� <")+GetTypeString()+"> - ������ �������� ������ ���� �������");
	ValidateParamValueListContext(ValueArgList, CString("����� <")+GetTypeString()+"> - ������ �������� ������ ���� ������� ��������");

	if (bIsFunction)
		return CallContextMethod(true, ValueContext.GetContext(), ValueMethodName.GetString(), (CValueListContext*)ValueArgList.GetContext(), pRetValue);
	else
		return CallContextMethod(false, ValueContext.GetContext(), ValueMethodName.GetString(), (CValueListContext*)ValueArgList.GetContext());
}

bool CExecuteModule::CallContextProcedure(const CValue& ValueContext, const CValue& ValueMethodName, const CValue& ValueArgList) const
{
	return CallContextMethod(false, ValueContext, ValueMethodName, ValueArgList);
}

bool CExecuteModule::CallContextFunction(const CValue& ValueContext, const CValue& ValueMethodName, const CValue& ValueArgList, CValue& RetValue) const
{
	return CallContextMethod(true, ValueContext, ValueMethodName, ValueArgList, &RetValue);
}

CCallWithValueListGuard::CCallWithValueListGuard(int iMethodParamsA, CValueListContext* pValListContA) :
	m_iMethodParams(iMethodParamsA), m_pValListCont(pValListContA), 
	m_iNumArgs(0)
{
	if(m_iMethodParams > 0)
	{
		CValueListContextData* pVLCD = ((CValueListContextData*)m_pValListCont->GetInternalData());
		PValueItemArrayT *pValueItemArray = (PValueItemArrayT*)pVLCD->GetValueList();
	
		if(m_iMethodParams >= pValueItemArray->GetSize()) 
			m_iNumArgs = pValueItemArray->GetSize();
		else 
			m_iNumArgs = m_iMethodParams;
		
		ppValues.resize(max(m_iMethodParams, m_iNumArgs));

		for(int i=0; i<m_iNumArgs; ++i) ppValues[i] = (CValue*)pValueItemArray->GetAt(i);

		for(i=m_iNumArgs; i<m_iMethodParams; ++i)
		{
			sh_ptr<CValue> pNewValue(new CValue);
			AddedValuesVector.push_back(pNewValue);
 			ppValues[i] = pNewValue.get();
		}
	}

}
CCallWithValueListGuard::~CCallWithValueListGuard()
{
}

CString CExecuteModule::GetModuleText() const
{
	return pBLModule->GetInternalData()->mSource;
}
