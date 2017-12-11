// SubsView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SubsView.h"
#include "MyView.h"
#include "MyADO.h"
#include "SetupDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE hMyInst;
extern CMyADO* pAdo;

/////////////////////////////////////////////////////////////////////////////
// CSubsView

IMPLEMENT_DYNCREATE(CSubsView, CFormView)
CSubsView* CSubsView::m_pView=NULL;

CSubsView::CSubsView()
	: CFormView(CSubsView::IDD)
{
	m_pView = this;
	//{{AFX_DATA_INIT(CSubsView)
	//}}AFX_DATA_INIT
}

CSubsView::~CSubsView()
{
}

void CSubsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubsView)
	DDX_Control(pDX, IDC_SUB_LIST, m_ListCtrl);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CSubsView, CFormView)
//BEGIN_MESSAGE_MAP(CSubsView, CCmdTarget)
	//{{AFX_MSG_MAP(CSubsView)
	ON_WM_SIZE()
	ON_COMMAND(ID_BUTTON32771, OnSubsNew)	
	ON_COMMAND(ID_BUTTON32772, OnSubsRename)	
	ON_COMMAND(ID_BUTTON32773, OnSubsDelete)	
	ON_COMMAND(ID_BUTTON32774, OnSubsSettings)	
	ON_COMMAND(ID_BUTTON32775, OnSubsHelp)	
	ON_COMMAND(ID_BUTTON32779, SetModeMarkUp)	
	ON_COMMAND(ID_BUTTON32780, SetModeFiltred)	
	ON_COMMAND(ID_BUTTON32781, OnSubsSaveData)	
	ON_UPDATE_COMMAND_UI(ID_BUTTON32779, OnUpdateConrol)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32780, OnUpdateConrol)
	ON_LBN_SELCHANGE(IDC_SUB_LIST, OnSelchangeSubList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubsView diagnostics

#ifdef _DEBUG
void CSubsView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSubsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSubsView message handlers

void CSubsView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	pAdo->FillListSubs(m_ListCtrl);
	pAdo->SaveOrLoadSubsSelection(false);
}



void CSubsView::OnSubsNew(){
	int iID = 0;
	pAdo->SaveOrLoadSubsSelection();
	if (pAdo->SubsInsertNew(iID)){ 
		pAdo->FillListSubs(m_ListCtrl);
		OnSelectMetaData();
	}
	pAdo->SaveOrLoadSubsSelection(false);
	::SetFocus(m_ListCtrl.m_hWnd);
}

void CSubsView::OnSubsRename(){
	pAdo->SaveOrLoadSubsSelection();
	if (pAdo->SubsRename(m_ListCtrl)){ 
		pAdo->FillListSubs(m_ListCtrl);
		OnSelectMetaData();
	}
	pAdo->SaveOrLoadSubsSelection(false);
	::SetFocus(m_ListCtrl.m_hWnd);
}

void CSubsView::OnSubsDelete(){
	pAdo->SaveOrLoadSubsSelection();
	if (pAdo->SubsDelete(m_ListCtrl)){ 
		pAdo->FillListSubs(m_ListCtrl);
		OnSelectMetaData();
		pAdo->SaveOrLoadSubsSelection(false);
	}
	::SetFocus(m_ListCtrl.m_hWnd);

}

void CSubsView::OnSubsSettings(){
	CSetupDlg pSetDlg;
	if (pSetDlg.DoModal() == IDOK){ 
		if (CMyView::m_pView != NULL){ 
			CMyView::m_pView->OnInitialUpdate();
		}
	}
	::SetFocus(m_ListCtrl.m_hWnd);
}

void CSubsView::OnSubsSaveData()
{
	pAdo->SaveOrLoad(true,false);	
}

void CSubsView::OnSubsHelp(){
	CString strHelp = "��������";
	strHelp += "\r\n������ ��� OpenConf - ������� ����������.";
	strHelp += "\r\n(c) ������ �. �. as trdm 2006 ICQ 308-779-620 email trdmval@gmail.com";
	strHelp += "\r\n";
	strHelp += "\r\n����� ������������� ����� ���������� �� �����������, ";
	strHelp += "\r\n�������� ���� �� ����������. ������� � ����� ������ ";
	strHelp += "\r\n� ������ �������� ���������, �������������� �����������";
	strHelp += "\r\n� �.�. ";
	strHelp += "\r\n";
	strHelp += "\r\n���������� ������ ���������� � �����������.";
	strHelp += "\r\n����� ������ ����������, ������ ������ ���������.";
	strHelp += "\r\n����������� ����� ���� F6 or Tab.";
	strHelp += "\r\n";
	strHelp += "\r\n������ ������ ������������:";
	strHelp += "\r\n- ����� ����������, (���� � ������ ����������)";
	strHelp += "\r\n- ������������� ���������� (���� � ����������.)";
	strHelp += "\r\n- ������� ���������� (������������� ����)";
	strHelp += "\r\n-";
	strHelp += "\r\n- �������� ����� ������� (��������� � ��������)";
	strHelp += "\r\n- �������� ����� ���������� (����� ������� �����... �� ������� ���� ���� ��� �� �������)";
	strHelp += "\r\n-";
	strHelp += "\r\n- ��������� (���� � ���������)";
	strHelp += "\r\n- ������ (��������... ������)";
	strHelp += "\r\n";
	strHelp += "\r\n	� ������ ���������� ���������� ������������, ������� �������� ";
	strHelp += "\r\n�� ��������, �� ��������..";
	strHelp += "\r\n������������ ����� �������� �������������� �������������:";
	strHelp += "\r\n		- \"�������� ����� �������\"  (��������� � ��������)";
	strHelp += "\r\n		- \"�������� ����� ����������\" (����� ������� �����...)";
	strHelp += "\r\n� ���� ������ ������ ���������, ��� ������� �� ���������� ������"; 
	strHelp += "\r\n	������ ����������� ������� ���������� � ������������ � �������.";
	strHelp += "\r\n	�.�. ���� �� � ������ ���������� �������� �������, � � ��� ������� ";
	strHelp += "\r\n	����� ���. �������, ������ ������� ��� ��� �� ���� � ������ ";
	strHelp += "\r\n	���������� ���������. � �������������� � ������ ���������� ���"; 
	strHelp += "\r\n	��������� ������ � ������ ��������� ������ ������� ��� ��� �� ";
	strHelp += "\r\n	����������� � ������� ����� ������ � ���������� ���������������.";
	strHelp += "\r\n";
	strHelp += "\r\n���� �� �����, ������ ������ ������ � ������� SubData.dat.";
	strHelp += "\r\n�� ��������� ������� ������ ))), ������ �������� ������� ������� ";
	strHelp += "\r\nCArhive....";
	AfxMessageBox(strHelp);
	::SetFocus(m_ListCtrl.m_hWnd);

}

void CSubsView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	CRect pRect;
	GetWindowRect(&pRect);
	CWnd* pWnd = GetDlgItem(IDC_SUB_LIST);
	if (pWnd != NULL){ 
		::MoveWindow(pWnd->m_hWnd,0,0,cx,cy,true);
		
	}
	
	
	// TODO: Add your message handler code here
	
}

void CSubsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
}

BOOL CSubsView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN){ 
		if (pMsg->wParam == VK_INSERT)		{ 		
			OnSubsNew();
		} else if (pMsg->wParam == VK_DELETE){ 
			OnSubsDelete();
		} else if (pMsg->wParam == VK_F2){ 
			OnSubsRename();
		} else if (pMsg->wParam == VK_ESCAPE){ 
			pAdo->ActivateStandartView();

		} else if (pMsg->wParam == VK_F6 || pMsg->wParam == VK_TAB){ 
			if (CMyView::m_pView != NULL){ 
				CTreeCtrl& tree = CMyView::m_pView->GetTreeCtrl();
				::SetFocus(tree.m_hWnd);
				
			}
		}
	}
	
	return CFormView::PreTranslateMessage(pMsg);
}

void CSubsView::SetModeMarkUp()
{
	pAdo->m_StateSubs = 0;
	if (CMyView::m_pView != NULL){
		CMyView::m_pView->OnInitialUpdate();	 
	}
}

void CSubsView::SetModeFiltred()
{
	pAdo->m_StateSubs = 1;
	if (CMyView::m_pView != NULL){
		CMyView::m_pView->OnInitialUpdate();	 
	}

}

void CSubsView::OnUpdateConrol(CCmdUI *pCmdUI)
{
	if (pCmdUI->m_nID == ID_BUTTON32779){ 
		pCmdUI->SetRadio( (pAdo->m_StateSubs == 0)?true:false);
	} else if (pCmdUI->m_nID == ID_BUTTON32780){ 
		pCmdUI->SetRadio( (pAdo->m_StateSubs != 0)?true:false);
	}
}

void CSubsView::OnSelectMetaData() {
	if (CMyView::m_pView == NULL){ 
		return;
	}
	if (pAdo->m_StateSubs == 0 ){  // �������
		CTreeCtrl& tree = CMyView::m_pView->GetTreeCtrl();
		HTREEITEM item=tree.GetSelectedItem();
		if (item == NULL){ 
			return;
		}
		// ������� ��������� � ������
		int nWhatIsIt = m_ListCtrl.GetCount();
		for (int i=0;i<nWhatIsIt; i++)		{
			m_ListCtrl.SetSel(i,false);
		}

		CMetaDataObj* pObj=(CMetaDataObj*)tree.GetItemData(item);
		if(!pObj){
			return;
		}
		// ���������, ��� �� ������, ���� �� ������� ��� ����, ����� �����������....
		nWhatIsIt = pObj->WhatIsIt();
		if (!pAdo->IsStoredObj(nWhatIsIt)){ 
			return;
		}

		// �� ������? ��������� ��, ���� �� ���� ����������, �������� ��....
		CString strSubs, key, strSubOne;
		key.Format("%d",pObj->GetID());
		if (pAdo->m_ObjToSub.Lookup(key,strSubs)){ 
			if (!strSubs.IsEmpty()){ 
				nWhatIsIt = strSubs.GetLength();
				CMapStringToString pTempMap;
				int i;
				for (i = 0; i<nWhatIsIt; i++)	{
					if (strSubs[i] != ','){ 
						strSubOne += strSubs[i];
					} else {
						if (strSubOne.GetLength()>0){ 
							// ����� �������
							pTempMap.SetAt(strSubOne,strSubOne);
							strSubOne = "";
						}
					}
				}
				strSubOne = "";
				// �������� ��� ���������� � �����, ����������� ������, �������� �����
				nWhatIsIt = m_ListCtrl.GetCount();
				for (i=0; i<nWhatIsIt; i++)	{
					strSubOne.Format("%d",m_ListCtrl.GetItemData(i));
					if (pTempMap.Lookup(strSubOne,strSubs)){ 
						m_ListCtrl.SetSel(i,TRUE);
					}

				}
			}
		}

	} else if (pAdo->m_StateSubs == 1 ){  
		// ����������, ������ �����
		// � ������ �������� ������...

	}
}



BOOL CSubsView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (((LPNMHDR)lParam)->code == TVN_SELCHANGED ){ 
		if (CSubsView::m_pView != NULL){ 
			OnSelectMetaData();
		}
	}	
	return CFormView::OnNotify(wParam, lParam, pResult);
}

void CSubsView::OnSelchangeSubList() 
{
	// ������� ����� ��� ������� ������� � ���� ����������
	long lngID;
	int lLong, nCount = m_ListCtrl.GetCount();
	int nCurSel;
	CString tmpStr, strSubs, key;

	if (nCount == 0){ 
		return;
	} else {
		//����� �������� ����� ��� �� �����...
		nCurSel = m_ListCtrl.GetCurSel();
		lLong = m_ListCtrl.GetItemData(nCurSel);
		key.Format("%d", lLong);
		SubsStorage* nSubsStg = NULL;
		if (pAdo->m_SubSelfs.Lookup(key,(CObject*&)nSubsStg)){ 
			nSubsStg->m_Check = m_ListCtrl.GetSel(nCurSel);
		}

	}
	if (pAdo->m_StateSubs == 1 ){
		return;
	}
	if (CMyView::m_pView == NULL){ 
		return;
	}

	CMultiTree& pMTree = (CMultiTree&) CMyView::m_pView->GetTreeCtrl();
	int sc = pMTree.GetSelectedCount();
	if (sc == 0){ 
		return;
	}
	CTreeItemList list;
	pMTree.GetSelectedList(list);
	if (list.IsEmpty()){ 
		return;
	}
	CMetaDataObj* pObj;
	POSITION pos = list.GetHeadPosition();
	HTREEITEM hStartItem = list.GetTail();
	while (pos != NULL)	{

		pObj=(CMetaDataObj*)pMTree.GetItemData(hStartItem);
		if(pObj != NULL){
			// ���������, ��� �� ������, ���� �� ������� ��� ����, ����� �����������....
			lngID = pObj->WhatIsIt();
			if (pAdo->IsStoredObj(lngID)){ 
				lngID = pObj->GetID();


				key.Format("%d",lngID);
				if (pAdo->m_ObjToSub.Lookup(key,strSubs)){ 
					nCurSel = m_ListCtrl.GetCurSel();
					if (nCurSel == LB_ERR) 
						return;
					tmpStr.Format(",%d,", lLong);
					if (m_ListCtrl.GetSel(nCurSel)>0){  // ��������, ������� ���� ����
						if (strSubs.Find(tmpStr)==-1){ 
							strSubs += tmpStr;
						}
					} else { // �� ��������, ������...
						if (strSubs.Find(tmpStr) != -1){ 
							strSubs.Replace(tmpStr,"");
						}
					}
					pAdo->m_ObjToSub.SetAt(key,strSubs);
				} else { 
					// �� ����� � m_ObjToSub....
					strSubs = "";

					for (int i=0; i<nCount; i++){
						if (m_ListCtrl.GetSel(i)>0){ 
							lLong = m_ListCtrl.GetItemData(i);
							tmpStr.Format(",%d,", lLong);
							strSubs = strSubs + tmpStr;
						}
					}
					if (!strSubs.IsEmpty()){ 
						pAdo->m_ObjToSub.SetAt(key,strSubs);
					}
				}
			}
		}
		hStartItem = list.GetNext(pos);
	}
}

long CSubsView::GetCurentStoredID()
{
	long lRetVal = 0;
	if (CMyView::m_pView == NULL){ 
		return 0;
	}
	if (pAdo->m_StateSubs == 0 ){  // �������
		CTreeCtrl& tree = CMyView::m_pView->GetTreeCtrl();
		HTREEITEM item=tree.GetSelectedItem();
		if (item == NULL){ 
			return 0;
		}
		CMetaDataObj* pObj=(CMetaDataObj*)tree.GetItemData(item);
		if(!pObj)
			return 0;
		// ���������, ��� �� ������, ���� �� ������� ��� ����, ����� �����������....
		lRetVal = pObj->WhatIsIt();
		if (!pAdo->IsStoredObj(lRetVal)){ 
			return 0;
		}
		lRetVal = pObj->GetID();
	}
	return lRetVal;
}
