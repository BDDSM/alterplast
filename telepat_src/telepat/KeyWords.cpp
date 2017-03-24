// KeyWords.cpp: implementation of the CKeyWords class.
#include "stdafx.h"
#include "KeyWords.h"
#include "resource.h"
#include "tentrydrawing.h"
#include "TeleGroup.h"
#include "TeleList.h"
#include "TxtTrap.h"
#include "parser.h"
#include "option.h"
#include "plugin.h"

static CTeleGroup gr;
static CTeleGroup templ;

class CKWEntry:public CTeleEntry
{
public:
	CKWEntry(CKeyWords::SKWInfo* pInfo,int lang):
	  CTeleEntry(pInfo->name[lang]),m_pInfo(pInfo){}
	CKeyWords::SKWInfo* m_pInfo;
	void GetHint(CString& buf)
	{
		buf=m_Name+"\n�������� �����";
		if(m_pInfo->descr && strlen(m_pInfo->descr))
			buf=buf+"\n"+m_pInfo->descr;
	}
	void GetInsert(CString& buf)
	{
		buf=m_Name+m_pInfo->delim;
		if(m_pInfo->name[0]=="Return")
		{
			if(!(CTxtTrap::GetParser()->m_flags & CParser::InProc))
				buf+=" !";
			buf+=';';
		}
	}
	bool MakeInsert()
	{
		CTxtTrap* pEditor=CTeleList::m_pOneList->m_pEditor;
		CPoint ptStart,ptEnd;
		ptEnd=CTeleList::m_pOneList->Insert(this);
		if(IsWindow(pEditor->m_hWnd))
		{
			ptStart=ptEnd;
			ptStart.x=0;
			pEditor->MoveCaret(ptStart,ptEnd,0);
			pEditor->SendMessage(WM_COMMAND,33289);

			CTextDocument* pDoc=pEditor->GetDocument();
			CString txt;
			pDoc->GetSelectedText(txt);
			pDoc->GetSel(ptStart,ptEnd);
			pEditor->MoveCaret(ptEnd,ptEnd,0);
			if(m_pInfo->NeedCR)
				pEditor->OnChar('\r',0,0);
		}
		pEditor->GetMyParser()->m_NeedQuick=true;
		return true;
	}
};

class CKWTempl:public CTeleEntry
{
public:
	int m_lang;
	CKWTempl(CKeyWords::SKWTemplates* pInfo,int lang):
	  CTeleEntry(pInfo->name[lang]),m_pInfo(pInfo),m_lang(lang){}
	CKeyWords::SKWTemplates* m_pInfo;
	void GetHint(CString& buf)
	{
		buf=m_pInfo->insert[m_lang];
		buf.Replace("\r\n","\n");
		buf.Replace("\t","    ");
		buf.Replace("!","�");
		if(m_pInfo->descr && strlen(m_pInfo->descr))
			buf=buf+"\n"+m_pInfo->descr;
	}
	bool MakeInsert()
	{
		CTxtTrap* pEditor=CTeleList::m_pOneList->m_pEditor;
		CPoint ptStart=CTeleList::m_pOneList->m_CaretPoint,ptEnd;
		ptStart.x-=CTeleList::m_pOneList->m_curPosInBuf;
		ptEnd=ptStart;
		ptEnd.x+=CTeleList::m_pOneList->m_buf.GetLength();

		CString ins=m_pInfo->insert[m_lang];
		if(CPlugin::m_pPlugin)
			CPlugin::m_pPlugin->FireInsert(m_pParent->m_Image,m_pInfo->name[m_lang],ins);
		CString line,tab="\r\n",tab1="";
		
		// ������� ������� ������
		CTextDocument* pDoc=pEditor->GetDocument();
		pDoc->GetLine(ptStart.y,line);
		// ������ ������� ������
		int ll=line.GetLength();
		if(line.Right(2)=="\r\n")
			line=line.Left(ll-=2);
		// �������� �� ��������� ������� �� ������ ������
		if(ll<ptStart.x)
		{
			for(int i=ll;i<ptStart.x;i++)
				tab1+=' ';
			ins=tab1+ins;
			ptStart.x=ll;
			ptEnd.x=ptStart.x+2;
		}
		// ���������� ������
		for(int i=0;i<ptStart.x;i++)
		{
			if(line[i]=='\t')
				tab+='\t';
			else
				tab+=' ';
		}
		// ���������� ����� ��� �������
		ins.Replace("\r\n",tab+tab1);
		ins.Replace("!","�");
		// ������� �����
		pDoc->SetSel(ptStart,ptEnd);
		pDoc->Replace(ins);
		// ��������, ������ �� ���������
		LPCTSTR ptr=ins;
		ptEnd=ptStart;
		while(*ptr)
		{
			if(*ptr=='\n')
			{
				ptEnd.y++;
				ptEnd.x=0;
			}
			else
				ptEnd.x++;
			ptr++;
		}
		// ������� ����������� ���� � ������������� ���
		pEditor->MoveCaret(ptStart,ptEnd,TRUE);
		pEditor->SendMessage(WM_COMMAND,33289);
		// ������ ����� ��� �������
		for(;ptStart.y<=ptEnd.y;ptStart.y++)
		{
			pDoc->GetLine(ptStart.y,line);
			ptStart.x=line.Find("�")+1;
			if(ptStart.x>0)
			{
				pDoc->DeleteSymb(1,ptStart);
				ptStart.x--;
				pEditor->MoveCaret(ptStart,ptStart,1);
				break;
			}
		}
		// ������ ������		
		CTeleList::m_pOneList->Hide();
		pEditor->GetMyParser()->m_NeedQuick=true;
		return true;
	}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CKeyWords::SKWInfo CKeyWords::keyWords[kwLast]={
	{{"AND","�"},"���������� �"," (!) ",false},
	{{"Break","��������"},"��������� ���������� �����",";",false},
	{{"Context","��������"},"�������� ���������� ��������� ��� ���������","",false},
	{{"Continue","����������"},"�������� ���������� � ������ �����",";",false},
	{{"Do","����"},"","",true},
	{{"Else","�����"},"","",true},
	{{"ElsIf","���������"},""," ",false},
	{{"EndDo","����������"},"",";",false},
	{{"EndFunction","������������"},"","",false},
	{{"EndIf","���������"},"��������� ��������� ��������� ��������� ����������",";",false},
	{{"EndProcedure","��������������"},"","",false},
	{{"EndTry","������������"},"",";",false},
	{{"Except","����������"},"","",true},
	{{"Export","�������"},"","",false},
	{{"For","���"},""," ",false},
	{{"Forward","�����"},"","",false},
	{{"Function","�������"},"���������� �������"," ",false},
	{{"GoTo","�������"},"����������� �������� ���������� �� ������ �������� ���������"," ~!;",false},
	{{"If","����"},"�������� ��������� ����������"," ",false},
	{{"NOT","��"},""," (!) ",false},
	{{"OR","���"},""," (!) ",false},
	{{"Procedure","���������"},"���������� ���������"," ",false},
	{{"Return","�������"},"���������� ��������� ��� �������","",false},
	{{"Then","�����"},"","",true},
	{{"To","��"},""," ",false},
	{{"Try","�������"},"","",true},
	{{"Val","����"},"�������� ��������� ������ �� ��������"," ",false},
	{{"Var","�����"},"���������� ����������"," !;",false},
	{{"While","����"},""," ",false},
};

CKeyWords::SKWTemplates CKeyWords::keyTemplates[]={
	{{"ElsIf_Then","���������_�����"},
		{
		"ElsIf ! Then",
		"��������� ! �����"
		},
		"������� ����� \"���������\""
	},
	{{"For_EndDo","���_����������"},
		{
		"For != To  Do\r\n"
		"EndDo;",
		"��� != ��  ����\r\n"
		"����������;"
		},
		"������� ����� ����� \"���\""
	},
	{{"Function_EndFunction","�������_������������"},
		{
		"Function !()\r\n"
		"EndFunction",
		"������� !()\r\n"
		"������������"
		},
		"������� ����� �������"
	},
	{{"If_EndIf","����_���������"},
		{
		"If ! Then\r\n"
		"EndIf;",
		"���� ! �����\r\n"
		"���������;"
		},
		"������� ����� �������"
	},
	{{"Procedure_EndProcedure","���������_��������������"},
		{
			"Procedure !()\r\n"
			"EndProcedure",
			"��������� !()\r\n"
			"��������������"
		},
		"������� ����� ���������"
	},
	{{"Try_EndTry","�������_������������"},
		{
			"Try\r\n"
			"\t!\r\n"
			"Except\r\n"
			"EndTry;",
			"�������\r\n"
			"\t!\r\n"
			"����������\r\n"
			"������������;"
		},
		"������� ����� �������"
	},
	{{"While_EndDo","����_����������"},
		{
			"While ! Do\r\n"
			"EndDo;",
			"���� ! ����\r\n"
			"����������;",
		},
		"������� ����� ����� \"����\""
	},
};

CKeyWords::CKeyWords():m_allowedWords(0)
{
}


void CKeyWords::FillGroup(CPtrArray& arr)
{
	gr.m_Image=IDI_KEYWORD_;
	templ.m_Image=IDI_TEMPLATES_;
	gr.ClearEntries();
	templ.ClearEntries();
	int lng=COption::m_pOption->GetLanguage();
	static const int tts[]={-1,-1,-1,-1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,1,-1,2,-1,3,-1,-1,4,-1,-1,-1,5,-1,-1,6,};
	for(int i=0;i<kwLast;i++)
	{
		if(m_allowedWords & (1<<i))
		{
			if(lng & 1)
			{
				CTeleEntry* pEntry=new CKWEntry(keyWords+i,0);
				gr.AddEntry(pEntry);
			}
			if(lng & 2)
			{
				CTeleEntry* pEntry=new CKWEntry(keyWords+i,1);
				gr.AddEntry(pEntry);
			}
			if(tts[i]>-1)
			{
				if(lng & 1)
				{
					CTeleEntry* pEntry=new CKWTempl(keyTemplates+tts[i],0);
					templ.AddEntry(pEntry);
				}
				if(lng & 2)
				{
					CTeleEntry* pEntry=new CKWTempl(keyTemplates+tts[i],1);
					templ.AddEntry(pEntry);
				}
			}
		}
	}
	if(gr.GetCount())
	{
		arr.Add(&gr);
		if(templ.GetCount())
			arr.Add(&templ);
	}
}
