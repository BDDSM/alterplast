// RMenu.cpp: implementation of the CRMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "RMenu.h"
#include "MenuItem.h"

/////////////////////////////////////////////////////////////////////////////
extern CMenuItem theMenu;
extern CMenuItem theMenu;
extern CMapWordToPtr theMapIDtoMenuItem;
extern CMapStringToPtr theMapDefMenuItem;
extern stDefMenuItems theDefMenuItems[];

/////////////////////////////////////////////////////////////////////////////
BEGIN_BL_METH_MAP(CRMenu) 
    RBL_METH(Add,			"��������",				3,	DefaultValue2)
//.<destination>��������� ����� � ������������ ����.
//.<syntax>��������(����, �����, ����������);
//.<return>������ ���� RMenuItem
//.<param>
	//.<@name>����
	//.<@destination>��� ������������ ������ ����
	//.<@optional>0
	//.<@type>�����
	//.<@default>
	//.<@values>1 - �������, 2 - �����������
//.<param>
	//.<@name>�����
	//.<@destination>������������� ������� RMenuItem
	//.<@optional>0
	//.<@type>������
	//.<@default>
	//.<@values>
//.<param>
	//.<@name>����������
	//.<@destination>������, ������������ � ����. �������� ����� ��������, ���� ����������� �����������.
	//.<@optional>1
	//.<@type>������
	//.<@default>
	//.<@values>
//.<descr>������������ ������ ���� ���������� �� ������� ������ ����. 
//.<descr>��� ���������� ������������� ���� ������� ��������� ����� ��������. ��� ������� ��� ����������� ���������� ��� �������� ���������� ������� ����.
    RBL_METH(AddMenu,		"������������",			2,	NoDefaultValue)
//.<destination>��������� �������
//.<syntax>������������(�����, ����������);
//.<return>������ ���� RMenuItem
//.<param>
	//.<@name>�����
	//.<@destination>������������� ������� RMenuItem
	//.<@optional>0
	//.<@type>������
	//.<@default>
	//.<@values>
//.<param>
	//.<@name>����������
	//.<@destination>������, ������������ � ����
	//.<@optional>0
	//.<@type>������
	//.<@default>
	//.<@values>
//.<descr>������������ ������ ���� ���������� �� ������� ������ ����. 
//.<descr>��� ���������� ������������� ���� ������� ��������� ����� ��������. ��� ������� ��� ����������� ���������� ��� �������� ���������� ������� ����.
//.<sample>������=�������������("����");
//.<sample>�����1=������.������������("����1", "&����1");
//.<sample>�����1.���������������("����1", "������ �������", "��������(""������ ������ �������"");");
//.<sample>������.��������();

    RBL_METH(AddSeparator,	"�������������������",	1,	NoDefaultValue)
//.<destination>��������� ����������� ����
//.<syntax>�������������������(�����);
//.<return>������ ���� RMenuItem
//.<param>
	//.<@name>�����
	//.<@destination>������������� ������� RMenuItem
	//.<@optional>0
	//.<@type>������
	//.<@default>
	//.<@values>
//.<descr>������������ ������ ���� ���������� �� ������� ������ ����. 
//.<descr>��� ���������� ������������� ���� ������� ��������� ����� ��������. ��� ������� ��� ����������� ���������� ��� �������� ���������� ������� ����.
    RBL_METH(Delete,		"�������",				1,	NoDefaultValue)
//.<destination>������� ����� ����
//.<syntax>�������(�����);
//.<param>
	//.<@name>�����
	//.<@destination>������������� ������� RMenuItem
	//.<@optional>0
	//.<@type>������
	//.<@default>
	//.<@values>
//.<descr>������� ����� ���� � ��������������� ����� �� ������������� ����. �������� ������������ �� ������� ���� ��������� �����. ���� ��������� �������, �� ��������� � ��� ����������� �������� ����.
//.<sample>������=�������������("����");
//.<sample>�����1=������.������������("����1", "&����1");
//.<sample>������.�������("����1");
    RBL_METH(DeleteAll,		"����������",			0,	NULL)
//.<destination>������� ������������ ����.
//.<syntax>����������();
//.<return>
//.<descr>������� ��� ������ �������������� ���� � ������� ���� ��������� ������ �� ����� ������������. �������� ������������ �� ������� ���� ��������� �����.
//.<sample>������=�������������("����");
//.<sample>������.����������();
    RBL_METH(Get,			"��������",				1,	NoDefaultValue)
//.<destination>�������� ������ RMenuItem �� �������������� ��� �� ����������� ������
//.<syntax>��������(�����OrNum);
//.<return>������ ���� RMenuItem
//.<param>
	//.<@name>�����OrNum
	//.<@destination>������������� ������� RMenuItem ��� ���������� �����.
	//.<@optional>0
	//.<@type>������, �����
	//.<@default>
	//.<@values>���������� ����� �� 1 �� ����������()
//.<descr>���������� ��������� ����� ������, ��������������� ������ ������������� ����.
//.<sample>������=�������������("����");
//.<sample>�����1=������.������������("����1", "&����1");
//.<sample>������.��������();
//.<sample>������=0;
//.<sample>
//.<sample>������=�������������("����");
//.<sample>�����1=������.��������("����1");
//.<sample>�����1.���������������("����1", "������ �������", "��������(""������ ������ �������"");");
//.<sample>������.��������();
//.<sample>
//.<sample>������=�������������("����");
//.<sample>��� �=1 �� ������.����������() ����
//.<sample>	�����=������.��������(�);
//.<sample>����������;
    RBL_METH(GetSystem,		"�����������������",	1,	NoDefaultValue)
//.<destination>�������� ��������� ���� ����, ��������, ������, ������.
//.<syntax>�����������������(�����);
//.<return>������ ���� RMenuItem
//.<param>
	//.<@name>�����
	//.<@destination>������������� ���������� ����.
	//.<@optional>0
	//.<@type>������
	//.<@default>
	//.<@values>����, ��������, ������, ������
//.<descr>���������� ������, ��������������� ���������� ������ ��������� ���� ���������. ��-��������� ������������ ������, �� ���������� ������ ����. �.�. ���� �������� ��������� ���� ����, �� � ��� �� ����� ������� �������, ��������� � �.�. ����� �� ���������� ���� ������, ��� ���������� ������ � ����������. ��� �������� �� �� �����, ����� �� � ������������� ���������� ���� ������ � ���������� ����. 
    RBL_METH(DeleteSystem,		"����������������",	1,	NoDefaultValue)
//.<destination>������� ��������� ���� ����, ��������, ������, ������ ��� ����� �������� ����, ������������ � �������������.
//.<syntax>����������������(�����);
//.<return>�����
//.<param>
	//.<@name>�����
	//.<@destination>������������� ���������� ����.
	//.<@optional>0
	//.<@type>������
	//.<@default>
	//.<@values>����, ��������, ������, ������
//.<descr>
    RBL_METH(Count,			"����������",			0,	NULL)
//.<destination>���������� ���������� ������� ����.
//.<syntax>����������();
//.<return>�����
//.<descr>���������� ���������� ������� ������������� ���� � ������� ���� ���������.
//.<sample>������=�������������("����");
//.<sample>��� �=1 �� ������.����������() ����
//.<sample>	�����=������.��������(�);
//.<sample>����������;
    RBL_METH(RedrawMenu,	"��������",	0,	NULL)
//.<destination>��������� ������� ���� ���������.
//.<syntax>��������();
//.<return>
//.<descr>��������� ������� ���� ��������� ����� ����������, �������� ��� �����-�� ������ ��������� ������� ������������� ����.
//.<sample>
//.<sample>������=�������������("����");
//.<sample>�����1=������.������������("����1", "&����1");
//.<sample>������.��������();
    RBL_METH(SendMessage,		"��������������",		1,	NoDefaultValue)
//.<destination>������� ������ ���������.
//.<syntax>��������������(��������);
//.<param>
	//.<@name>��������
	//.<@destination>�������� ������������� �������.
	//.<@optional>0
	//.<@type>�����
//.<descr>�������� ������� ���� �� ������, ������� ���������� ������� ��� ������ ������� ����, ������� ������ ������� ������������, ������� ������� ������, ��������, Ctrl+N.
//.<sample>// ���������� ������
//.<sample>��������� ������������()
//.<sample>	�����=�������������("����");
//.<sample>	�����.��������������(33726);
//.<sample>��������������
//.<sample>//
//.<sample>������=�������������("����");
//.<sample>�����=������.������������("���", "�����������");
//.<sample>�����.���������������("���1", "����������...", "������������()");
//.<sample>�����.���������������("���2", "�������", "������������(""����������.�������"")");
//.<sample>������.��������();
    RBL_METH(LoadInterface,		"������������������",		1,	NoDefaultValue)
//.<destination>��������� ����������� ��������� �� ������������.
//.<syntax>������������������(���������);
//.<param>
	//.<@name>���������
	//.<@destination>�������� ����������, ��� ��� ���������� � ������������.
	//.<@optional>0
	//.<@type>������
//.<descr>��������� ���� �� ����������, ������������� � ������������. ��� �������� ��������� ����������� ��������� ���� ����������. 
//.<descr>��������! ����������� ����� ������� ���� ����� �������� � ����� �����������. ��������� �������� ��� ������ ����, �������� ������� ����� ��������� �� ������������ ���� ��������, � ������ ����, ������� ��������� �� ���������� ����� ������������, ���������� � �.�. ��������� ������ ����� �������� �� ��������� ��� �� �������� ������.
//.<sample>������=�������������("����");
//.<sample>������.����������();
//.<sample>������.������������������("�������������");
//.<sample>������.��������();
    RBL_METH(GetList,		"�������������������������",	0,	NULL)
//.<destination>���������� ������ �����������.
//.<syntax>�������������������������();
//.<return>��������������
//.<descr>���������� ������ ���� ������ �������� � �������������� �����������.

/**/RBL_METH(Test,	"����",	1,	NULL)
END_BL_METH_MAP() 

BEGIN_BL_PROP_MAP(CRMenu)
END_BL_PROP_MAP()

////EVENT(OnGetCommand, "�������������������")
//.<destination>������� ��������� ��� ������ ������ ����, ������� ������ ������ ������������, ������� ������� ������.
//.<syntax>��������� �������������������(��������, ���������������������)
//.<syntax>��������������
//.<param>
	//.<@name>��������
	//.<@destination>������������� �������
	//.<@optional>1
	//.<@type>�����
//.<param>
	//.<@name>���������������������
	//.<@destination>���� � ���� ��������� ��������� ����� ��������� �������� 1, �� ����������� ��������� ������� �������� ����� ��������.
	//.<@optional>1
	//.<@type>�����
	//.<@values>1, 0
//.<descr>��������� ����� ���� ��������� � ���������� ��� ����� ��������� ������. ��� ������������� ������� ��������� ����������� � �������� ��������� ���������, ���� �� ����, � ������ � ����������. 
//.<descr>��������� ��������� ������������� ����� ������� ����, ������� ������� ������, ������� ������ ������� ������������ � ������� ���� ��������� � ����� ����������, ��������, �������. �� ������ �������� ���������� �������� ����������� �������� �������� ��������� ��������������������� �������� 1.
//.<sample>��������� �������������������(��������, ���������������������)
//.<sample>	��������("�������� ������� "+��������);
//.<sample>	
//.<sample>	���� ��������=32805 �����
//.<sample>		��������������("������ ����������� ������!");
//.<sample>		���������������������=1;
//.<sample>	���������;
//.<sample>��������������

IMPLEMENT_MY_CONTEXT(CRMenu, "RMenu", "����", 1, NULL, NULL, -1);
//.<destination>������ ���� RMenu ������������ ��� ���������� ������������� ���� � ��������� ���� ���������. ������ �������� ����������� ��� ���������� ����, �� �� ���������� ����. ���� ������� ������������ ��������� ���������� RMenu, �� ��� ��� ����� ��������� ����� � ��� �� ������������ ����. �������� ��������� RMenu �� ������ �� ����� �������� ������������� ���� �� ��������� ���� ���������.
//.<destination>������ ����� ������������� ���� �������� �������� ���� RMenuItem. ������ ����� ������������� ���� � ������� ���� �������� �������� RMenuItem ���� �������. ������ RMenuItem � ���� ������� ����� �������� ��������� ����������� �������� RMenuItem, ������� ����� ���� ������� ����, �������, ������������. ����� �������, �������� �������� �������������� ����. 
//.<destination>������ RMenuItem ���� ����� ���� ������������� �� �������� �� ���������� ����� 1�, ������� ����������� ��� ������ ����� ������.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRMenu::CRMenu()
{
	m_pMenuItem = &theMenu;
}

CRMenu::~CRMenu()
{
}

BOOL CRMenu::funcAdd(CValue& RetVal, CValue **params)
{
	enum ItemType itType = (enum ItemType)(int)(params[0]->GetNumeric().GetDouble()-1);

	switch(itType){
	case itSeparator:
		{
			funcAddSeparator(RetVal, params);
			break;
		}
	case itMenu:
		{
			funcAddMenu(RetVal, params);
			break;
		}
	default:
		ERR1(IDS_ERR_WRONGTYPE2, 0)
	};

	return TRUE;
}

BOOL CRMenu::funcAddMenu(CValue& RetVal, CValue **params)
{
	if(params[0]->GetString().IsEmpty()) ERR1(IDS_ERR_EMPTYID, 0)
	if(m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_REDEFINEID, params[0]->GetString())
	
	CMenuItem* pMenu = m_pMenuItem->AddMenuItem(params[0]->GetString());
	pMenu->SetMenu(params[1]->GetString());

	RetVal.AssignContext(pMenu->GetContext());

	return TRUE;
}

BOOL CRMenu::funcAddSeparator(CValue& RetVal, CValue **params)
{
	if(params[0]->GetString().IsEmpty()) ERR1(IDS_ERR_EMPTYID, 0)
	if(m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_REDEFINEID, params[0]->GetString())
	
	CMenuItem* pMenu = m_pMenuItem->AddMenuItem(params[0]->GetString());
	pMenu->SetSeparator();

	RetVal.AssignContext(pMenu->GetContext());

	return TRUE;
}

BOOL CRMenu::funcGetSystem(CValue& RetVal, CValue **params)
{
	CString str = params[0]->GetString();
	str.MakeUpper();
	
	stDefMenuItems* pDefMenuItem = (stDefMenuItems*)theMapDefMenuItem[str];

	if(pDefMenuItem)
		RetVal.AssignContext(pDefMenuItem->pMenuItem->GetContext());
	else
		ERR1(IDS_ERR_UNKNOWNID, params[0]->GetString());

	return TRUE;
}

BOOL CRMenu::funcDeleteSystem(CValue& RetVal, CValue **params)
{
	CString str = params[0]->GetString();
	str.MakeUpper();

	CAppFrame* pApp = (CAppFrame*)AfxGetApp();
	CMenu7* pV7Menu1 = (CMenu7*)(((DWORD*)pApp)+0x66);
	CMenu7* pV7Menu2 = (CMenu7*)(((DWORD*)pApp)+0x68);
	int nSubMenu = 0;

	stDefMenuItems* pDefMenuItem = (stDefMenuItems*)theMapDefMenuItem[str];

	if(pDefMenuItem)
	{
		nSubMenu = pV7Menu1->FindSubMenu(pDefMenuItem->Name);
		if(nSubMenu>=0)
			pV7Menu1->DeleteMenu(nSubMenu, MF_BYPOSITION);
		
		nSubMenu = pV7Menu2->FindSubMenu(pDefMenuItem->Name);
		if(nSubMenu>=0)
			pV7Menu2->DeleteMenu(nSubMenu, MF_BYPOSITION);
	}
	else
	{
		nSubMenu = pV7Menu1->FindSubMenu(params[0]->GetString());
		if(nSubMenu>=0)
		{
			pV7Menu1->DeleteMenu(nSubMenu, MF_BYPOSITION);

			nSubMenu = pV7Menu2->FindSubMenu(params[0]->GetString());
			if(nSubMenu>=0)
				pV7Menu2->DeleteMenu(nSubMenu, MF_BYPOSITION);
		}
		else
			ERR1(IDS_ERR_UNKNOWNID, params[0]->GetString());
	}

	return TRUE;
}

BOOL CRMenu::funcGet(CValue& RetVal, CValue **params)
{
	if(params[0]->GetTypeCode() == 1)
	{
		int index = params[0]->GetNumeric();
		if(index<=0 || index>m_pMenuItem->mListSubMenu.GetCount()) ERR1(IDS_WRONGINDEX, index);
		
		POSITION pos = m_pMenuItem->mListSubMenu.FindIndex(index-1);
		CMenuItem* pMenu = m_pMenuItem->mListSubMenu.GetAt(pos);
		RetVal.AssignContext(pMenu->GetContext());
	}
	else
	{
		if(!m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_UNKNOWNID, params[0]->GetString());
		
		CMenuItem* pMenu = m_pMenuItem->Get(params[0]->GetString());
		RetVal.AssignContext(pMenu->GetContext());
	}
	return TRUE;
}

BOOL CRMenu::funcDelete(CValue& RetVal, CValue **params)
{
	if(!m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_UNKNOWNID, params[0]->GetString())

	m_pMenuItem->ClearMainMenu();
	m_pMenuItem->RemoveAt(params[0]->GetString());
	m_pMenuItem->Build();
	RetVal = 1;

	return TRUE;
}

BOOL CRMenu::funcDeleteAll(CValue& RetVal, CValue **params)
{
	m_pMenuItem->ClearMainMenu();
	m_pMenuItem->ClearList();

	for(int i=0; i<4; i++)
		theDefMenuItems[i].pMenuItem->ClearList();

	RetVal = 1;
	
	return TRUE;
}

BOOL CRMenu::funcRedrawMenu(CValue& RetVal, CValue **params)
{
	try{
	m_pMenuItem->ClearMainMenu();
	m_pMenuItem->Build();

	CMDIChildWnd* wnd = GetMainFrame()->MDIGetActive();
	if(wnd) 
	{
		wnd->ActivateFrame(SW_HIDE);
		wnd->ActivateFrame(SW_SHOW);
	};

	AfxGetApp()->GetMainWnd()->DrawMenuBar();

	RetVal = 1;
	}catch(...){MSG("ERRRORRR funcRedrawMenu")}

	return TRUE;
}

BOOL CRMenu::funcSendMessage(CValue& RetVal, CValue **params)
{
	AfxGetApp()->GetMainWnd()->PostMessage(WM_COMMAND, params[0]->GetNumeric(), 0);
	return TRUE;
}

BOOL CRMenu::funcGetList(CValue& RetVal, CValue **params)
{
	CBLContext* pValueList = CBLContext::CreateInstance("��������������");
	int iNumMethod = pValueList->FindMethod("AddValue");
	

	CString strMDFileName = static_cast<CApp7*>(AfxGetApp())->GetProps()->GetStringProp(0x08);

	IStoragePtr pStgRoot, pStgUserDef, pStrInterfaces, pStrTheInterface;

	IStorage* pStorage = NULL;
	HRESULT hr;
	USES_CONVERSION;

	hr = ::StgOpenStorage(
		T2COLE(strMDFileName),
		NULL,
		STGM_READ | STGM_SHARE_DENY_WRITE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
	pStgRoot.Attach(pStorage, false);

	hr = pStgRoot->OpenStorage(T2COLE("UserDef"), 
		NULL,
		STGM_READ | STGM_SHARE_EXCLUSIVE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
	pStgUserDef.Attach(pStorage, false);

	hr = pStgUserDef->OpenStorage(T2COLE("Page.1"), 
		NULL,
		STGM_READ | STGM_SHARE_EXCLUSIVE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
	pStrInterfaces.Attach(pStorage, false);

	IStreamPtr pStream;
	IStream *pIStr = NULL;

	hr = pStrInterfaces->OpenStream(T2COLE("Container.Contents"),0,STGM_READ | STGM_SHARE_EXCLUSIVE,0,&pIStr);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
	pStream.Attach(pIStr, false);    

	STATSTG statstg;
	hr = pStream->Stat(&statstg, STATFLAG_NONAME);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
	ULONG cbStreamSize = statstg.cbSize.LowPart;
	CString strCont;
	ULONG cbRead;
	hr = pStream->Read(strCont.GetBufferSetLength(cbStreamSize), cbStreamSize, &cbRead);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
	strCont.ReleaseBuffer();

	int i = 0;
	CString strName;
	CItemList lst;
	// ���� ��������� �� �����
	lst.LoadFromString(strCont, &i);
	for(int j = 1; j < lst.GetNItems(); j++)
	{
		CItem* pItem = (CItem*)lst.GetItemAt(j);
		CItem* pItem1 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(1);
		CItem* pItem2 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(2);

		CValue* pValue = new CValue(pItem2->GetFunctor());
		pValueList->CallAsProc(iNumMethod, &pValue);
	}

	RetVal.AssignContext(pValueList);
	
	return TRUE;
}

BOOL CRMenu::funcTest(CValue& RetVal, CValue **params)
{

	//CGetDoc7* pDoc7 = CGetDlg::GetTopmostDocunent();
	//MSG1("%x", pDoc7);
	

	CWinApp* pApp = AfxGetApp();
	CMenu7* pV7Menu1 = (CMenu7*)(((DWORD*)pApp)+0x66);
	CMenu7* pV7Menu2 = (CMenu7*)(((DWORD*)pApp)+0x68);
/*

	CMenu* pMenu = AfxGetApp()->GetMainWnd()->GetMenu();
	while(pMenu->GetMenuItemCount()>0)
		pMenu->DeleteMenu(0, MF_BYPOSITION);
	
	CMenu7::CopySMenu(pV7Menu1, pMenu, 0);
	AfxGetApp()->GetMainWnd()->DrawMenuBar();
*/

	DUMPMENU(pV7Menu1)
	DUMPMENU(pV7Menu2)

	RetVal = 1;
	
	return FALSE;
}

BOOL CRMenu::funcCount(CValue& RetVal, CValue **params)
{
	RetVal = m_pMenuItem->mListSubMenu.GetCount();
	return TRUE;
}


BOOL CRMenu::NoDefaultValue(int nParam, CValue* param) const
{
	//*param = -1;
	//*param = CDate(0,0,0);
	return FALSE;
}

BOOL CRMenu::DefaultValue2(int nParam, CValue* param) const
{
	if(nParam>1)
	{
		*param = "";
		return TRUE;
	}
	else 
		return FALSE;
}

void CRMenu::LoadMenuFromMD(IStream* pStream, CMenuItem* pParentMenuItem, CMap<WORD, WORD, LoadedItemData, LoadedItemData>& mapCmdID2Command, CString strID="")
{
	HRESULT hr;
	ULONG cbRead;
	DWORD wdCntMenuItems;
	DWORD wdCommandID;
	BYTE btTextSize;
	CString strText;
	CString strNewID;
	CString strCode1C;
	CMenuItem* pMenuItem;

	hr = pStream->Read(&wdCntMenuItems, 4, &cbRead);
	if(FAILED(hr)) ERR("�� ������� ��������� MD ����");

	for(int i=0; i<wdCntMenuItems; i++)
	{
		hr = pStream->Read(&wdCommandID, 4, &cbRead);
		if(FAILED(hr)) ERR("�� ������� ��������� MD ����");
		
		if(wdCommandID) // ���� 0, �� ��� ����������� 
		{
			hr = pStream->Read(&btTextSize, 1, &cbRead);
			if(FAILED(hr)) ERR("�� ������� ��������� MD ����");
			
			hr = pStream->Read(strText.GetBufferSetLength(btTextSize), btTextSize, &cbRead);
			if(FAILED(hr)) ERR("�� ������� ��������� MD ����");
			
			strText.ReleaseBuffer();
			//MSG4("%2d %X %s %s", i+1, wdCommandID, strText, mapCmdID2Command[wdCommandID]);
			
			if(wdCommandID == 0xFFFFFFFF)
			{
				if(!strText.CompareNoCase("__1CFixName_File__"))
				{
					pMenuItem = theDefMenuItems[0].pMenuItem->AddMenuItem(strNewID);
					pMenuItem->SetSeparator();
					LoadMenuFromMD(pStream, theDefMenuItems[0].pMenuItem, mapCmdID2Command, strNewID+"\\"+strText);
				}
				else if(!strText.CompareNoCase("__1CFixName_Oper__"))
				{
					pMenuItem = theDefMenuItems[1].pMenuItem->AddMenuItem(strNewID);
					pMenuItem->SetSeparator();
					LoadMenuFromMD(pStream, theDefMenuItems[1].pMenuItem, mapCmdID2Command, strNewID+"\\"+strText);
				}
				else if(!strText.CompareNoCase("__1CFixName_Service__"))
				{
					pMenuItem = theDefMenuItems[2].pMenuItem->AddMenuItem(strNewID);
					pMenuItem->SetSeparator();
					LoadMenuFromMD(pStream, theDefMenuItems[2].pMenuItem, mapCmdID2Command, strNewID+"\\"+strText);
				}
				else if(!strText.CompareNoCase("__1CFixName_Help__"))
				{
					pMenuItem = theDefMenuItems[3].pMenuItem->AddMenuItem(strNewID);
					pMenuItem->SetSeparator();
					LoadMenuFromMD(pStream, theDefMenuItems[3].pMenuItem, mapCmdID2Command, strNewID+"\\"+strText);
				}
				else
				{
					strNewID.Format("%s\\%d", strID, i);
					pMenuItem = pParentMenuItem->AddMenuItem(strNewID);
					pMenuItem->SetMenu(strText);

					LoadMenuFromMD(pStream, pMenuItem, mapCmdID2Command, strNewID);
				}
			}
			else
			{
				LoadedItemData lddItemData;
				if(mapCmdID2Command.Lookup(wdCommandID, lddItemData))
				{
					strNewID.Format("%s\\%d", strID, i);
					pMenuItem = pParentMenuItem->AddMenuItem(strNewID);

					pMenuItem->SetCommand(strText, lddItemData.Code);
					pMenuItem->m_strTips = lddItemData.Tips;
				}
			}
		}
		else
		{
			strNewID.Format("%s\\%d", strID, i);
			pMenuItem = pParentMenuItem->AddMenuItem(strNewID);
			pMenuItem->SetSeparator();
		}
	}
}

void CRMenu::LoadSubInterface(CString strSubIntName, CString& strTheInterfaceStrName, CItemList& lst, IStoragePtr pStrInterfaces, CList<CString, CString>& lstExclCommands)
{
	HRESULT hr;
	IStorage* pStorage = NULL;
	IStream *pIStr = NULL;
	ULONG cbRead;
	USES_CONVERSION;
	
	// ���� ����������� ��������� �� �����
	for(int j = lst.FindGenericItem("SubUsersInterfaceType", -1); j >= 0; j = lst.FindGenericItem("SubUsersInterfaceType", ++j))
	{
		CItem* pItem = (CItem*)lst.GetItemAt(j);
		CItem* pItem1 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(1);
		CItem* pItem2 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(2);
		if(!strSubIntName.CompareNoCase(pItem2->GetFunctor()))
			strTheInterfaceStrName=pItem1->GetFunctor();
	}
	
	if(strTheInterfaceStrName.IsEmpty())
		ERR1("��������� %s �� ������.", strSubIntName);
	
	// ����������� ��������� ������
	IStoragePtr pStrTheInterface;
	IStreamPtr pStreamCmd;
	
	hr = pStrInterfaces->OpenStorage(T2COLE((LPCTSTR)strTheInterfaceStrName), 
		NULL,
		STGM_READ | STGM_SHARE_EXCLUSIVE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR("�� ������� ��������� MD ����.");
	pStrTheInterface.Attach(pStorage, false);
	
	hr = pStrTheInterface->OpenStream(T2COLE("Commands"),0,STGM_READ | STGM_SHARE_EXCLUSIVE,0,&pIStr);
	if(FAILED(hr)) ERR("�� ������� ��������� MD ����.");
	pStreamCmd.Attach(pIStr, false);    
	
	DWORD dwCntCmd;
	BYTE bt[9];
	CString strParentIntName;
	
	// �������� ��� ������������� ����������
	hr = pStreamCmd->Read(&bt, 5, &cbRead);
	if(FAILED(hr)) ERR("�� ������� ��������� MD ����.");
	hr = pStreamCmd->Read(strParentIntName.GetBufferSetLength(bt[4]), bt[4], &cbRead);
	if(FAILED(hr)) ERR("�� ������� ��������� MD ����.");
	strParentIntName.ReleaseBuffer();

	//MSG2("%s ??%s", strParentIntName, strTheInterfaceStrName);
	BOOL bMainInterfaceIsFound = FALSE;

	for(j = lst.FindGenericItem("UsersInterfaceType", -1); j >= 0; j = lst.FindGenericItem("UsersInterfaceType", ++j))
	{
		CItem* pItem = (CItem*)lst.GetItemAt(j);
		CItem* pItem1 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(1);
		CItem* pItem2 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(2);
		if(!strParentIntName.CompareNoCase(pItem2->GetFunctor()))
		{
			strTheInterfaceStrName=pItem1->GetFunctor();
			bMainInterfaceIsFound = TRUE;
			//MSG2("%s->%s", strParentIntName, strTheInterfaceStrName);
		}
	}

	if(!bMainInterfaceIsFound)
		LoadSubInterface(strParentIntName, strTheInterfaceStrName, lst, pStrInterfaces, lstExclCommands);
	
	hr = pStreamCmd->Read(&bt, 9, &cbRead);
	if(FAILED(hr)) ERR("�� ������� ��������� MD ����.");
	// ���������� �������
	hr = pStreamCmd->Read(&dwCntCmd, 4, &cbRead);
	if(FAILED(hr)) ERR("�� ������� ��������� MD ����.");
	
	for(int i=0; i<dwCntCmd; i++)
	{
		hr = pStreamCmd->Read(&bt, 1, &cbRead);
		if(FAILED(hr)) ERR("�� ������� ��������� MD ����.");
		CString strExclCmd;
		hr = pStreamCmd->Read(strExclCmd.GetBufferSetLength(bt[0]+1), bt[0]+1, &cbRead);
		if(FAILED(hr)) ERR("�� ������� ��������� MD ����.");
		strExclCmd.ReleaseBuffer();
		
		strExclCmd.TrimLeft();
		strExclCmd.TrimRight();
		
		lstExclCommands.AddTail(strExclCmd);
		//MSG1("lstExclCommands.AddTail %s", strExclCmd);
	}
}

BOOL CRMenu::funcLoadInterface(CValue& RetVal, CValue **params)
{
	CString strMDFileName = static_cast<CApp7*>(AfxGetApp())->GetProps()->GetStringProp(0x08);

	IStoragePtr pStgRoot, pStgUserDef, pStrInterfaces, pStrTheInterface;

	IStorage* pStorage = NULL;
	HRESULT hr;
	USES_CONVERSION;

	hr = ::StgOpenStorage(
		T2COLE(strMDFileName),
		NULL,
		STGM_READ | STGM_SHARE_DENY_WRITE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
	pStgRoot.Attach(pStorage, false);

	hr = pStgRoot->OpenStorage(T2COLE("UserDef"), 
		NULL,
		STGM_READ | STGM_SHARE_EXCLUSIVE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
	pStgUserDef.Attach(pStorage, false);

	hr = pStgUserDef->OpenStorage(T2COLE("Page.1"), 
		NULL,
		STGM_READ | STGM_SHARE_EXCLUSIVE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
	pStrInterfaces.Attach(pStorage, false);

	IStreamPtr pStream;
	IStream *pIStr = NULL;

	hr = pStrInterfaces->OpenStream(T2COLE("Container.Contents"),0,STGM_READ | STGM_SHARE_EXCLUSIVE,0,&pIStr);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
	pStream.Attach(pIStr, false);    

	STATSTG statstg;
	hr = pStream->Stat(&statstg, STATFLAG_NONAME);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
	ULONG cbStreamSize = statstg.cbSize.LowPart;
	CString strCont;
	ULONG cbRead;
	hr = pStream->Read(strCont.GetBufferSetLength(cbStreamSize), cbStreamSize, &cbRead);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
	strCont.ReleaseBuffer();

	CList<CString, CString> lstExclCommands;
	CString strTheInterfaceStrName = "";
	int i = 0;
	CItemList lst;
	// ���� ��������� �� �����
	lst.LoadFromString(strCont, &i);
	for(int j = lst.FindGenericItem("UsersInterfaceType", -1); j >= 0; j = lst.FindGenericItem("UsersInterfaceType", ++j))
	{
		CItem* pItem = (CItem*)lst.GetItemAt(j);
		CItem* pItem1 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(1);
		CItem* pItem2 = (CItem*)((CGenericItem*)pItem)->m_array.GetAt(2);
		if(!params[0]->GetString().CompareNoCase(pItem2->GetFunctor()))
			strTheInterfaceStrName=pItem1->GetFunctor();
	}

	if(strTheInterfaceStrName.IsEmpty())
		LoadSubInterface(params[0]->GetString(), strTheInterfaceStrName, lst, pStrInterfaces, lstExclCommands);
	

	hr = pStrInterfaces->OpenStorage(T2COLE((LPCTSTR)strTheInterfaceStrName), 
		NULL,
		STGM_READ | STGM_SHARE_EXCLUSIVE,
		NULL,
		0,
		&pStorage);
	if(FAILED(hr)) ERR1("pStrInterfaces �� ������� ��������� ����: %s", strMDFileName);
	pStrTheInterface.Attach(pStorage, false);


	IStreamPtr pStreamCmd;
	hr = pStrTheInterface->OpenStream(T2COLE("Commands"),0,STGM_READ | STGM_SHARE_EXCLUSIVE,0,&pIStr);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
	pStreamCmd.Attach(pIStr, false);    

	DWORD dwCntCmd;
	LARGE_INTEGER offset;

	offset.QuadPart = 13;
	hr = pStreamCmd->Seek(offset, STREAM_SEEK_SET, NULL);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
	hr = pStreamCmd->Read(&dwCntCmd, 4, &cbRead);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);


	CMap<WORD, WORD, CString, CString> mapCmdID2Command;
	CMap<WORD, WORD, LoadedItemData, LoadedItemData> mapCmdID2Command2;
	WORD wCmdID;
	BYTE btSize[4];
	CString strCmd, strFormName, strTips;
	LoadedItemData lddItemData;
	int iSize;

	for(i=0; i<dwCntCmd; i++)
	{
		// Command ID
		hr = pStreamCmd->Read(&wCmdID, 2, &cbRead);
		if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
		// ������ ���������
		// ������ ������ ��� �����
		hr = pStreamCmd->Read(&btSize, 3, &cbRead);
		if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);

		if(btSize[2]!=0x00)
		{
			iSize = btSize[2]; // ����� ������ ���� ���� ������ ����
		}
		else
		{
			hr = pStreamCmd->Read(&btSize, 1, &cbRead);
			if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);

			if(btSize[0]==0xFF)
			{
				hr = pStreamCmd->Read(&btSize, 2, &cbRead);
				if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
				iSize = 256*btSize[1]+btSize[0];
			}
			else
				iSize = btSize[0];
		}

		// ������ ���������
		hr = pStreamCmd->Read(strTips.GetBufferSetLength(iSize), iSize, &cbRead);
		if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
		strTips.ReleaseBuffer();
		lddItemData.Tips = strTips;

		// ������ ������ ����� ���������
		hr = pStreamCmd->Read(&btSize, 1, &cbRead);
		if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
		iSize = btSize[0];

		if(iSize)
		{
			offset.QuadPart = iSize;
			hr = pStreamCmd->Seek(offset, STREAM_SEEK_CUR, NULL);
			if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
		}

		// ������ ������ ���� ����
		hr = pStreamCmd->Read(&btSize, 3, &cbRead);
		if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);

		if(btSize[2]!=0x00)
		{
			iSize = btSize[2]; // ����� ������ ���� ���� ������ ����
		}
		else
		{
			hr = pStreamCmd->Read(&btSize, 1, &cbRead);
			if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);

			if(btSize[0]==0xFF)
			{
				hr = pStreamCmd->Read(&btSize, 2, &cbRead);
				if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
				iSize = 256*btSize[1]+btSize[0];
			}
			else
				iSize = btSize[0];
		}

		// ������ �������
		hr = pStreamCmd->Read(strCmd.GetBufferSetLength(iSize), iSize, &cbRead);
		if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
		strCmd.ReleaseBuffer();

		//MSG4("%3d %X %x %s", i, wCmdID, iSize, strCmd);

		// ������ ������ ����� �������
		hr = pStreamCmd->Read(&btSize, 1, &cbRead);
		if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
		iSize = btSize[0];

		strFormName.Empty();
		if(iSize)
		{
			hr = pStreamCmd->Read(strFormName.GetBufferSetLength(iSize), iSize, &cbRead);
			if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
			strFormName.ReleaseBuffer();
		}
			
		//MSG4("%3d %X %s :: %s", i, wCmdID, strCmd, strFormName);

		// ������������� 
		CString strWord[3];
		for(int j=0; j<3; j++) strWord[j].Empty();
		int lastpos = 0, pos = 0;
		for(j=0; j<3; j++)
		{
			pos = strCmd.Find(".", lastpos);

			if(pos < 0)
			{
				strWord[j] = strCmd.Mid(lastpos);
				j = 77;
			}
			else
			{
				strWord[j] = strCmd.Mid(lastpos, pos - lastpos);
				lastpos = pos+1;
			}
		}

		CString strCode1C;
		if(!strWord[0].CompareNoCase("���������"))
		{
			strCode1C="#33727";
		}
		else if(!strWord[0].CompareNoCase("����������"))
		{
			if(!strWord[1].CompareNoCase("�������"))
				strCode1C="#33726";
			else
				strCode1C.Format("������������(\"����������.%s%s\")", strWord[1], strFormName.IsEmpty()?"":"."+strFormName);
		}
		else if(!strWord[0].CompareNoCase("��������"))
		{
			if(!strWord[1].CompareNoCase("������"))
				strCode1C="��������������(\"�������� �� ��������������.\")";
			else
				strCode1C.Format("������������(\"��������.%s%s\")", strWord[1], strFormName.IsEmpty()?"":"."+strFormName);
		}
		else if(!strWord[0].CompareNoCase("������"))
		{
			if(!strWord[1].CompareNoCase("�������"))
				strCode1C="#33725";
			else if(!strWord[1].CompareNoCase("������� ������"))
				strCode1C="#33725";
			else if(!strWord[1].CompareNoCase("������� ������"))
				strCode1C="#33725";
			else
				strCode1C.Format("������������(\"������.%s%s\")", strWord[1], strFormName.IsEmpty()?"":"."+strFormName);
		}
		else if(!strWord[0].CompareNoCase("�����"))
		{
			if(!strWord[1].CompareNoCase("�������"))
				strCode1C="#33856";
			else if(!strWord[1].CompareNoCase("��������������"))
				strCode1C="#57601";
			else
				strCode1C.Format("������������(\"�����.%s\")", strWord[1]);
		}
		else if(!strWord[0].CompareNoCase("���������"))
		{
			if(!strWord[1].CompareNoCase("�������"))
				strCode1C="#33859";
			else
				strCode1C.Format("������������(\"���������.%s\")", strWord[1]);
		}
		else if(!strWord[0].CompareNoCase("��������������"))
		{
			if(!strWord[1].CompareNoCase("�������"))
				strCode1C="#33857";
			else if(!strWord[1].CompareNoCase("������� �� �������"))
				strCode1C="#33857";
			else if(!strWord[1].CompareNoCase("������� ������"))
				strCode1C="#33857";
			else if(!strWord[1].CompareNoCase("������� �� ���������"))
				strCode1C="#33857";
			else
				strCode1C.Format("������������(\"��������������.%s\")", strWord[1]);
		}
		else if(!strWord[0].CompareNoCase("���������"))
		{
			if(!strWord[1].CompareNoCase("�������"))
				strCode1C="#33862";
			else if(!strWord[1].CompareNoCase("���������"))
				strCode1C="#33752";
			else
				strCode1C="#33862";
		}
		else if(!strWord[0].CompareNoCase("����������"))
		{
			if(!strWord[1].CompareNoCase("�������"))
				strCode1C="#33860";
			else
				strCode1C.Format("������������(\"����������.%s\")", strWord[1]);
		}
		else if(!strWord[0].CompareNoCase("��������"))
		{
			if(!strWord[1].CompareNoCase("�������������"))
				strCode1C="#33861";
			else if(!strWord[1].CompareNoCase("������"))
				strCode1C="#33861";
			else if(!strWord[1].CompareNoCase("���������������������"))
				strCode1C="#33786";
			else if(!strWord[1].CompareNoCase("���������������"))
				strCode1C="#33786";
		}
		else if(!strWord[0].CompareNoCase("��������"))
		{
			if(!strWord[1].CompareNoCase("�������������"))
				strCode1C="#33788";
			else if(!strWord[1].CompareNoCase("������������������"))
				strCode1C="#33764";
		}
		else if(!strWord[0].CompareNoCase("������"))
		{
			if(!strWord[1].CompareNoCase("�����������������������������"))
				strCode1C="#33665";
			else if(!strWord[1].CompareNoCase("�������������������������������"))
				strCode1C="#33761";
			else if(!strWord[1].CompareNoCase("��������������������������"))
				strCode1C="#32897";
			else if(!strWord[1].CompareNoCase("��������������������"))
				strCode1C="#32899";
			else if(!strWord[1].CompareNoCase("��������������������"))
				strCode1C="#32924";
			else if(!strWord[1].CompareNoCase("���������"))
				strCode1C=strFormName;
		}

		lddItemData.Code = strCode1C;

		
		if(lstExclCommands.Find(strCmd) == NULL)
		{
			//MSG2("+ %X %s count=%d", wCmdID, strCmd, lstExclCommands.GetCount());
			//mapCmdID2Command.SetAt(wCmdID, strCode1C);
			mapCmdID2Command2.SetAt(wCmdID, lddItemData);
		}
	}

	IStreamPtr pStreamInt;
	hr = pStrTheInterface->OpenStream(T2COLE("Page.1"),0,STGM_READ | STGM_SHARE_EXCLUSIVE,0,&pIStr);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);
	pStreamInt.Attach(pIStr, false);    

	DWORD wdHeader;
	hr = pStreamInt->Read(&wdHeader, sizeof(wdHeader), &cbRead);
	if(FAILED(hr)) ERR1("�� ������� ��������� ����: %s", strMDFileName);

	LoadMenuFromMD(pStreamInt, &theMenu, mapCmdID2Command2);
	
	return true;    
}

