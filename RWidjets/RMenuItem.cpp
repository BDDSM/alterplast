// RMenuItem.cpp: implementation of the CRMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "RMenuItem.h"
#include "MenuItem.h"

/////////////////////////////////////////////////////////////////////////////
extern CMenuItem theMenu;
extern CMenuItem theMenu;
extern CMapWordToPtr theMapIDtoMenuItem;

/////////////////////////////////////////////////////////////////////////////
BEGIN_BL_METH_MAP(CRMenuItem) 
    RBL_METH(Add,			"��������",				4,	DefaultValue2)
//.<destination>��������� ����� � ������������ ����.
//.<syntax>��������(����, �����, ����������, �������);
//.<return>������ ���� RMenuItem
//.<param>
	//.<@name>����
	//.<@destination>��� ������������ ������ ����
	//.<@optional>0
	//.<@type>�����
	//.<@default>
	//.<@values>1 - �������, 2 - �����������, 3 - �������
//.<param>
	//.<@name>�����	//.<@destination>������������� ������� RMenuItem
	//.<@optional>0
	//.<@type>������
	//.<@default>
	//.<@values>
//.<param>
	//.<@name>����������
	//.<@destination>������, ������������ � ����
	//.<@optional>1
	//.<@type>������
	//.<@default>
	//.<@values>�������� ����� ��������, ���� ��������� �����������.
//.<param>
	//.<@name>�������
	//.<@destination>����� �������, ������������ ��� ������ ������ ����
	//.<@optional>1
	//.<@type>������
	//.<@default>
	//.<@values>�������� ����� ��������, ���� ����������� ����������� ��� �������.
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
    RBL_METH(AddCommand,	"���������������",		3,	DefaultValue2)
//.<destination>��������� �������
//.<syntax>���������������(�����, ����������, �������);
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
//.<param>
	//.<@name>�������
	//.<@destination>����� �������, ������������ ��� ������ ������ ����
	//.<@optional>1
	//.<@type>������
	//.<@default>
	//.<@values>
//.<descr>����� ��������� ����� ����, ��� ������ �������� ����������� ��������� ������. ������ ����������� � ��������� ��������� �������� ��������� ���� ��� � ���������� ���������.
//.<descr>������������ ������ ���� ���������� �� ������� ������ ����. 
//.<descr>��� ���������� ������������� ���� ������� ��������� ����� ��������. ��� ������� ��� ����������� ���������� ��� �������� ���������� ������� ����.
//.<sample>������=�������������("����");
//.<sample>�����1=������.������������("����1", "&����1");
//.<sample>�����1.���������������("����1", "������ �������", "��������(""������ ������ �������"");");
//.<sample>�����1.���������������("����2", "������ �������", "���������2()");
//.<sample>������.��������();
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
//.<descr>������� ����� ���� � ��������������� ����� �� ������������� ����. ���� ��������� �������, �� ��������� � ��� ����������� �������� ����.
//.<descr>��� ���������� ������������� ���� ������� ��������� ����� ��������.
//.<sample>������=�������������("����");
//.<sample>�����1=������.������������("����1", "&����1");
//.<sample>������.�������("����1");
    RBL_METH(DeleteAll,		"����������",			0,	NULL)
//.<destination>������� ������������ ����.
//.<syntax>����������();
//.<return>
//.<descr>������� ��� ������ �� ������� ������ �������������� ���� ����� �� ����� ������������. 
//.<descr>��� ���������� ������������� ���� ������� ��������� ����� ��������.
//.<sample>������=�������������("����");
//.<sample>�����1=������.��������("����1");
//.<sample>�����1.����������();
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
//.<sample>�����1=������.��������("����1");
//.<sample>��� �=1 �� �����1.����������() ����
//.<sample>	�����=�����1.��������(�);
//.<sample>����������;
    RBL_METH(Count,			"����������",			0,	NULL)
//.<destination>���������� ���������� ������� ����.
//.<syntax>����������();
//.<return>�����
//.<descr>���������� ���������� ������� ������������� ���� �� ������� ������.
//.<sample>������=�������������("����");
//.<sample>�����1=������.��������("����1");
//.<sample>��� �=1 �� �����1.����������() ����
//.<sample>	�����=�����1.��������(�);
//.<sample>����������;
    RBL_METH(ReadOnly,			"�����������",			1,	DefaultValueBase)
//.<destination>���������� �����������
//.<syntax>�����������(���������);
//.<return>�����
//.<param>
	//.<@name>���������
	//.<@destination>����� �������� �����������
	//.<@optional>1
	//.<@type>�����
	//.<@default>
	//.<@values>1 - ��������, 0 - ����������
//.<descr>������������� ����������� ������ ���� ��� ������. ����������� ����� ���������� �����. ���������� �������� ����������� �� ���������� ������. �� �������� ��� ������� �������� ���� ���������.
//.<sample>������=�������������("����");
//.<sample>��������=������.��������("����1");
//.<sample>������.��������("�����1");
//.<sample>������.�����������(0);
//.<sample>������.��������();
    RBL_METH(Check,				"�������",				1,	DefaultValueBase)
//.<destination>���������� �������
//.<syntax>�������(���������);
//.<return>�����
//.<param>
	//.<@name>���������
	//.<@destination>����� ��������
	//.<@optional>1
	//.<@type>�����
	//.<@default>
	//.<@values>1 - ������� �����������, 0 - ������� �����
//.<descr>������������� ������� ����� �� ������ ����. ���������� �������� ������� �� ���������� ������. �� �������� ��� ������� �������� ���� ���������.
//.<sample>������=�������������("����");
//.<sample>��������=������.��������("����1");
//.<sample>������.��������("�����1");
//.<sample>������.�������(1);
//.<sample>������.��������();
    RBL_METH(SetPicture,		"������������������",	3,	DefaultValue1)
//.<destination>���������� ������� ��� ������ ����
//.<syntax>������������������(���������, ������, �������);
//.<return>�����
//.<param>
	//.<@name>���������
	//.<@destination>�������� (������) ��� ������ ����
	//.<@optional>0
	//.<@type>����� ��� ����������� ��� 1� ������ �������� 
	//.<@default>
	//.<@values>
//.<param>
	//.<@name>������
	//.<@destination>������ ����������� � ������� ��������
	//.<@optional>1
	//.<@type>�����
	//.<@default>0
	//.<@values>
//.<param>
	//.<@name>�������
	//.<@destination>������ ��������
	//.<@optional>1
	//.<@type>�����
	//.<@default>���� �� ������, ������ ������� ������ ������
	//.<@values>
//.<descr>������������� �������� ��� ������ ����.
//.<descr>���� ������ ���������� �������� �����, �� �� ����� �������� 1CRCRUS.DLL ����������� �������� (������ ���� Bitmap) ��������������� ������ �������.
//.<descr>��-��������� ������ �������� ����������� ������ ������. ���� �������� ������� � ������� ��������� �����������, �� � ��������� ������ ��������� ��� ���������� �����.
//.<sample>������=�������������("����");
//.<sample>������=������.������������("����", "�������");
//.<sample>��������1=������.���������������("�1", "������� 1", "��������������(""������� 1"")");
//.<sample>// ��������� �������� �� ���������� ��������
//.<sample>�����=�������������("��������");
//.<sample>�����.���������("���������������");
//.<sample>��������1.������������������(�����, 3, 16);
//.<sample>// ��������� �������� �� �����
//.<sample>�����.���������(����������������()+"Pic.bmp");
//.<sample>��������1.������������������(�����, 3, 16);
//.<sample>// ��������� �������� � ��������������� 1037 �� ����� ��������
//.<sample>��������1.������������������(1037, 3, 16);
//.<sample>������.��������();
    RBL_METH(GetCommandID,			"�������������������",			0,	NULL)
//.<destination>�������� CommandID ������ ����.
//.<syntax>�������������������();
//.<return>�����
//.<descr>���������� CommandID - ���������� ������������� �������, ����������� ��� ������ ������ ����. ��� ������� � ������������ ���������� 0. ���� �� ������������� ���������� � ��������� ������������������� ������ ���������� ��� ������ ������ ����.
//.<descr>CommandID ������������� ������� ���� ��� ���������� ������ ��������. ����� ������� ���������� ������ �������� CommandID ������ � ���� �� ������ ����� ���� ������. ������� �� ������������� ������� ��������� CommandID ������ ����, � ����� ��� ���������� � ���-��.
END_BL_METH_MAP() 

BEGIN_BL_PROP_MAP(CRMenuItem)
	BL_PROP("ID",		"����",		getID,		NULL)
//.<destination>������������� �������
//.<write>0
//.<return>������
//.<descr>���������� ���������� ������������� �������, ����������� ��� �������� ������ ����.
	BL_PROP("Type",		"���",		getType,		NULL)
//.<destination>��� ������ ����
//.<write>0
//.<return>�����
//.<descr>���������� ��� ������ ����. 1 - �������, 2 - �����������, 3 - �������
	RBL_PROP(Text,		"�����")
//.<destination>��������� ������ ����
//.<return>������
//.<descr>������������ ��������� ������ ����.
	RBL_PROP(Script,	"�������")
//.<destination>����������� ������
//.<return>������
//.<descr>������ �� ���������� ����� 1�, ����������� ��� ������ ������ ����. 
//.<descr>����� ��� ����� ���� ����� ���� #xxxx, ��� xxxx - �������� ������������� �������, ���������� �������.
	RBL_PROP(Tips,		"���������")
//.<destination>��������� ������ ����
//.<return>������
//.<descr>�����, ������� ����� ���������� � ������ ��������� �������� ���� ��������� ��� �������� ����� �� ����� ����. ��� �������, ������ ������ ��������� ������� �������� ��������, ������� ����� ��������� ����� ������ ����� ������ ����.
	//RBL_PROP(QHTML,		"�������������")
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CRMenuItem, "RMenuItem", "�����������", 0, NULL, NULL, -1); 
//.<destination>������ ���� RMenuItem ��������� ��������� ���������� ������� ������������� ����.
//.<destination>����� ���� ������ �������� ��������, ������������, �������������������, ��������������� �������� RMenu � RMenuItem. �� ����� ���� ������ �������� �������������.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRMenuItem::CRMenuItem()
{

}

CRMenuItem::~CRMenuItem()
{

}

BOOL CRMenuItem::getID(CValue& Value) const
{
    Value = m_pMenuItem->m_strID;
    return TRUE;
}

BOOL CRMenuItem::getType(CValue& Value) const
{
    Value = m_pMenuItem->itType + 1;
    return TRUE;
}

BOOL CRMenuItem::getText(CValue& Value) const
{
    Value = m_pMenuItem->m_strCaption;
    return TRUE;
}

BOOL CRMenuItem::setText(CValue const& Value)
{
	if(m_pMenuItem->itType == itMenu || m_pMenuItem->itType == itCommand)
	{
		if(m_pMenuItem->mParentMenuItem->mIsMainMenu == TRUE)
			m_pMenuItem->mParentMenuItem->ClearMainMenu();

		m_pMenuItem->m_strCaption = Value.GetString();
	};
    return TRUE;
}

BOOL CRMenuItem::getScript(CValue& Value) const
{
    Value = m_pMenuItem->m_strCode1C;
    return TRUE;
}

BOOL CRMenuItem::setScript(CValue const& Value)
{
	if(m_pMenuItem->itType == itCommand)
	    m_pMenuItem->m_strCode1C = Value.GetString();
    return TRUE;
}

BOOL CRMenuItem::getTips(CValue& Value) const
{
    Value = m_pMenuItem->m_strTips;
    return TRUE;
}

BOOL CRMenuItem::setTips(CValue const& Value)
{
	m_pMenuItem->m_strTips = Value.GetString();
    return TRUE;
}

BOOL CRMenuItem::funcAdd(CValue& RetVal, CValue **params)
{
	enum ItemType itType = (enum ItemType)(int)(params[0]->GetNumeric().GetDouble()-1);

	switch(itType){
	case itCommand:
		{
			funcAddCommand(RetVal, params);
			break;
		}
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
		ERR1(IDS_ERR_WRONGTYPE1, 0)
	};

	return TRUE;
}

BOOL CRMenuItem::funcAddMenu(CValue& RetVal, CValue **params)
{
	if(params[0]->GetString().IsEmpty()) ERR1(IDS_ERR_EMPTYID, 0)
	if(m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_REDEFINEID, params[0]->GetString())

	CMenuItem* pMenu = m_pMenuItem->AddMenuItem(params[0]->GetString());
	pMenu->SetMenu(params[1]->GetString());

	RetVal.AssignContext(pMenu->GetContext());

	return TRUE;
}

BOOL CRMenuItem::funcAddCommand(CValue& RetVal, CValue **params)
{
	if(params[0]->GetString().IsEmpty()) ERR1(IDS_ERR_EMPTYID, 0)
	if(m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_REDEFINEID, params[0]->GetString())

	CMenuItem* pMenu = m_pMenuItem->AddMenuItem(params[0]->GetString());
	pMenu->SetCommand(params[1]->GetString(), params[2]->GetString());

	RetVal.AssignContext(pMenu->GetContext());

	return TRUE;
}

BOOL CRMenuItem::funcAddSeparator(CValue& RetVal, CValue **params)
{
	if(params[0]->GetString().IsEmpty()) ERR1(IDS_ERR_EMPTYID, 0)
	if(m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_REDEFINEID, params[0]->GetString())

	CMenuItem* pMenu = m_pMenuItem->AddMenuItem(params[0]->GetString());
	pMenu->SetSeparator();

	RetVal.AssignContext(pMenu->GetContext());

	return TRUE;
}

BOOL CRMenuItem::funcDelete(CValue& RetVal, CValue **params)
{
	if(!m_pMenuItem->Find(params[0]->GetString())) ERR1(IDS_ERR_UNKNOWNID, params[0]->GetString())
	
	m_pMenuItem->RemoveAt(params[0]->GetString());
	RetVal = 1;

	return TRUE;
}

BOOL CRMenuItem::funcDeleteAll(CValue& RetVal, CValue **params)
{
	m_pMenuItem->ClearList();
	RetVal = 1;

	return TRUE;
}

BOOL CRMenuItem::funcGet(CValue& RetVal, CValue **params)
{
	if(params[0]->GetTypeCode() == 1)
	{
		int index = params[0]->GetNumeric();
		if(index<=0 || index>m_pMenuItem->mListSubMenu.GetCount()) ERR1(IDS_WRONGINDEX, index);
		
		CMenuItem* pMenu = m_pMenuItem->mListSubMenu.GetAt(m_pMenuItem->mListSubMenu.FindIndex(index-1));
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

BOOL CRMenuItem::funcCount(CValue& RetVal, CValue **params)
{
	RetVal = m_pMenuItem->mListSubMenu.GetCount();
	return TRUE;
}

BOOL CRMenuItem::funcGetCommandID(CValue& RetVal, CValue **params)
{
	RetVal = m_pMenuItem->m_nID;
	return TRUE;
}

BOOL CRMenuItem::funcReadOnly(CValue& RetVal, CValue **params)
{
	RetVal = m_pMenuItem->m_bEnabled;
	
	if(m_pMenuItem->itType != itCommand) return FALSE;

	int NewVal = params[0]->GetNumeric();
	if(NewVal == 0 || NewVal == 1)
		m_pMenuItem->m_bEnabled = NewVal;

	return TRUE;
}

BOOL CRMenuItem::funcCheck(CValue& RetVal, CValue **params)
{
	RetVal = m_pMenuItem->m_bCheck;
	
	if(m_pMenuItem->itType != itCommand) return FALSE;

	int NewVal = params[0]->GetNumeric();
	if(NewVal == 0 || NewVal == 1)
		m_pMenuItem->m_bCheck = NewVal;

	return TRUE;
}

BOOL CRMenuItem::funcSetPicture(CValue& RetVal, CValue **params)
{
	CBitmap* pBmp = NULL;
	CBitmap bmp;
	CSize size;
	BOOL bSuccess = FALSE;

	if(m_pMenuItem->m_pPictureHolder7)
		delete m_pMenuItem->m_pPictureHolder7;

    if (params[0]->GetTypeCode() == AGREGATE_TYPE_1C)
	{
		if(params[0]->GetContext())
			if(strcmp(params[0]->GetContext()->GetRuntimeClass()->m_lpszClassName,"CPictureContext") == 0)
				{
					CPictureHolder7& PictureHdr = static_cast<CPictureContext*>(params[0]->GetContext())->m_PictureHolder7;

					m_pMenuItem->m_pPictureHolder7 = new CPictureHolder7(PictureHdr);

					OLE_HANDLE handle;
					m_pMenuItem->m_pPictureHolder7->GetPicture()->get_Handle(&handle);
					pBmp = CBitmap::FromHandle(*(HBITMAP*)&handle);

					size = m_pMenuItem->m_pPictureHolder7->GetSize();
					bSuccess = TRUE;
				}
	}
	else
	{
		m_pMenuItem->m_pPictureHolder7 = new CPictureHolder7();
		
		if(bmp.LoadBitmap((UINT)params[0]->GetNumeric()))
		{
			pBmp = &bmp;
			BITMAP bt;
			bmp.GetBitmap(&bt);
			size = CSize(bt.bmWidth, bt.bmHeight);
			bSuccess = TRUE;
		}
		else
			ERR1(IDS_ERR_NORES, (UINT)params[0]->GetNumeric());
	}
		
	if(bSuccess)
	{
		if((int)params[2]->GetNumeric() > 0)
			size.cx = params[2]->GetNumeric();
		else 
			size.cx = size.cy;

		CImageList RowsImages;
		RowsImages.Create(size.cx, size.cy, ILC_MASK, 1, 10);
		RowsImages.Add(pBmp, RGB(0x00,0x80,0x80)); // RGB(0xC0,0xC0,0xC0)
		
		HICON icon;
		if(params[1]->GetNumeric()>0 && params[1]->GetNumeric()<=RowsImages.GetImageCount())
			icon = RowsImages.ExtractIcon((int)params[1]->GetNumeric() - 1);
		else
			icon = RowsImages.ExtractIcon(0);
		
		m_pMenuItem->m_pPictureHolder7->CreateFromIcon(icon);

		RetVal = 1;
		return TRUE;
	}


	ERR(IDS_ERR_NOPICTURE);
	RetVal = 0L;
	return TRUE;
}



BOOL CRMenuItem::NoDefaultValue(int nParam, CValue* param) const
{
	//*param = -1;
	//*param = CDate(0,0,0);
	return FALSE;
}

BOOL CRMenuItem::DefaultValue2(int nParam, CValue* param) const
{
	if(nParam>1)
	{
		*param = "";
		return TRUE;
	}
	else 
		return FALSE;
}

BOOL CRMenuItem::DefaultValue1(int nParam, CValue* param) const
{
	if(nParam>0)
	{
		*param = -1;
		return TRUE;
	}
	else 
		return FALSE;
}

BOOL CRMenuItem::DefaultValueBase(int nParam, CValue* param) const
{
	*param = -1;
	return TRUE;
}

