// convert.cpp
#include "stdafx.h"
using namespace MSXML;
#define LOAD_XML_TYPE
#include "TypeNodes.h"
#include "..\zlibeng.tlh"

static const char LowerTable[257]=
"                                              . 0123456789       abcdefghijklmnopqrstuvwxyz    _ abcdefghijklmnopqrstuvwxyz                                             �               �       ����������������������������������������������������������������";

static CStringArray* stringsArray;
static CMapStringToPtr mapNames;
static CMapPtrToPtr typeNames;

char* TypeBlock;

STypeInfo* pTypeW;
SAttrInfo* pAttrW;
SMethInfo* pMethW;
SParaInfo* pParaW;
DWORD*	pAddBaseW;
DWORD*	pAddRetTypeW;
DWORD*	pAddParaTypeW;

DWORD TypeCnt=0;
DWORD MethCnt=0;
DWORD AttrCnt=0;
DWORD ParaCnt=0;
DWORD AddBaseCnt=0;
DWORD AddRetTypeCnt=0;
DWORD AddParaTypeCnt=0;

CDWordArray crTNames;
CDWordArray crCNames;

void Print(LPCTSTR format,...);

void SplitStrToArray(LPCTSTR str,CStringArray& arr,char delim)
{
	if(!str || !*str)
		return;
	LPCTSTR pRead=str,pStart=pRead;
	while(1)
	{
		while(*pRead && *pRead!=delim)
			pRead++;
		CString key=CString(pStart,pRead-pStart);
		key.TrimLeft(" \t\r\n");
		key.TrimRight(" \t\r\n");
		arr.Add(key);
		if(!*pRead)
			break;
		pStart=++pRead;
	}
}

static int GetStringNum(CString& txt)
{
	if(txt.IsEmpty())
		return 0;
	txt.Replace("\t","");
	txt.Replace("\r\n","\n");
	int num;
	if(!mapNames.Lookup(txt,(void*&)num))
	{
		num=stringsArray->GetSize();
		stringsArray->Add(txt);
		mapNames[txt]=(void*)num;
	}
	return num;
}

static int GetStringNum(_bstr_t& t)
{
	CString txt=(LPCTSTR)t;
	return GetStringNum(txt);
}

static bool IsBadIdent(DWORD dwName)
{
	LPCTSTR name=stringsArray->GetAt(dwName);
	while(*name)
	{
		if(LowerTable[(BYTE)*name]==' ')
			return true;
		name++;
	}
	return false;
}

CString GetStringFromNumber(DWORD num)
{
	return stringsArray->GetAt(num);
}

DWORD* DelimStrToDwords(IXMLDOMNamedNodeMapPtr& atr, BSTR name,DWORD& count, char delim=',')
{
	DWORD* ret=NULL;
	count=0;
	if(atr!=NULL)
	{
		IXMLDOMNodePtr val;
		atr->raw_getNamedItem(name,&val);
		if(val!=NULL)
		{
			CStringArray arr;
			SplitStrToArray(val->text,arr,delim);
			count=arr.GetSize();
			for(int i=0;i<count;i++)
			{
				if(arr[i].IsEmpty())
				{
					arr.RemoveAt(i--);
					count--;
				}
			}
			if(count)
			{
				ret=new DWORD[count];
				for(i=0;i<count;i++)
					ret[i]=GetStringNum(arr[i]);
			}
		}
	}
	return ret;
}

DWORD GetStringAtr(IXMLDOMNamedNodeMapPtr& atr,BSTR name)
{
	if(atr!=NULL)
	{
		IXMLDOMNodePtr val;
		atr->raw_getNamedItem(name,&val);
		if(val!=NULL)
			return GetStringNum(val->text);
	}
	return 0;
}

DWORD GetDwordAtr(IXMLDOMNamedNodeMapPtr& atr,BSTR name)
{
	if(atr!=NULL)
	{
		IXMLDOMNodePtr val;
		atr->raw_getNamedItem(name,&val);
		if(val!=NULL)
			return (DWORD)atol(val->text);
	}
	return 0;
}

//// ������ � ������ ��� �������� �� xml � ����������� � ������ ��������
// �������� ���� �� ���� ����
SNode* SNode::CreateNode(SNode* pParent,LPCTSTR xmlName)
{
	static LPCTSTR tags[]={
		"type",
		"attribs",
		"attrib",
		"methods",
		"method",
		"params",
		"param",
		"handlers",
		"return",
	};
	static const NodeTypes tags2type[]=
	{ndType,ndAttribs,ndAttrib,ndMethods,ndProc,ndParams,ndParam,ndHandlers,ndReturn};
	int tt=-1;
	for(int i=0;i<sizeof(tags)/sizeof(tags[0]);i++)
	{
		if(!stricmp(tags[i],xmlName))
		{
			tt=i;
			break;
		}
	}
	if(tt==-1)
		return NULL;
	static const BYTE tagsmap[12][9]=
	{
	//"type","attribs","attrib","methods","method","params","param","handlers","return",
	{0,		  1,		0,		 1,			0,		0,		0,		1,			0},//ndType,
	{0,		  0,		2,		 0,			0,		0,		0,		0,			0},//ndAttribs,
	{0,		  0,		0,		 0,			0,		0,		0,		0,			0},//ndAttrib,
	{0,		  0,		0,		 0,			0,		0,		0,		0,			0},//ndAttribInExpr,
	{0,		  0,		0,		 0,			3,		0,		0,		0,			0},//ndMethods,
	{0,		  0,		0,		 0,			0,		1,		0,		0,			4},//ndProc,
	{0,		  0,		0,		 0,			0,		1,		0,		0,			4},//ndFunc,
	{0,		  0,		0,		 0,			0,		1,		0,		0,			4},//ndFuncInExpr,
	{0,		  0,		0,		 0,			0,		0,		5,		0,			0},//ndParams,
	{0,		  0,		0,		 0,			0,		0,		0,		0,			0},//ndParam,
	{0,		  0,		0,		 0,			3,		0,		0,		0,			0},//ndHandlers,
	{0,		  0,		0,		 0,			0,		0,		0,		0,			0},//ndReturn,
	};
	SNode* pNode=NULL;
	if(!pParent)
	{
		if(!tt)
			pNode=new STypeNode(ndType);
	}
	else
	{
		switch(tagsmap[pParent->nodeType-ndType][tt])
		{
		case 1:
			pNode=new SNode(tags2type[tt],pParent);
			break;
		case 2:
			pNode=new SAtributNode(ndAttrib,pParent);
			break;
		case 3:
			pNode=new SMethodNode(ndProc,pParent);
			break;
		case 4:
			pNode=new SReturnNode(ndReturn,pParent);
			break;
		case 5:
			pNode=new SParamNode(ndParam,pParent);
			break;
		}
	}
	return pNode;
}

// ���������� ���� � ��������� � �������� ����
// ���������� ����� ������, �������, ��������
void SNode::SaveNodeWithChild()
{
	SNode* pNode=this;
	while(pNode)
	{
		// ����� ���� �������� ���� ������
		pNode->SaveNodeData();
		pNode=pNode->next;
	}
}

void GetTagPath(IXMLDOMNodePtr& item,CString& path)
{
	if(item==NULL)
		return;
	CString tag=(LPCTSTR)item->baseName;
	if(!tag.IsEmpty())
	{
		tag.Insert(0,"<");
		IXMLDOMNamedNodeMapPtr atr=item->attributes;
		if(atr!=NULL)
		{
			int cnt=atr->length;
			for(int i=0;i<cnt;i++)
			{
				IXMLDOMNodePtr val=atr->item[i];
				tag=tag+" "+(LPCTSTR)val->baseName+"=\""+(LPCTSTR)val->text+"\"";
			}
		}
		tag+=">";
		path.Insert(0,tag);
	}
	GetTagPath(item->parentNode,path);
}

void PrintTagError(IXMLDOMNodePtr& item,CString err)
{
	CString path;
	GetTagPath(item,path);
	Print("%s\n%s",path,err);
}

// �������� ���� �� xml-������
// ��������� ����� ������, �������� ������ ������� ������,
// ��������� �������� � ���������
SNode* SNode::LoadXMLNodeWithChild(SNode* pParent,IXMLDOMNodePtr& item)
{
	SNode *pFirstNode=NULL,*pLastNode=NULL,*pNode=NULL;
	while(item!=NULL)
	{
		CString tagName=(LPCTSTR)item->baseName;
		if(tagName.IsEmpty())
		{
			// ��� ��� ����� - ��������� ������
			if(pParent)
			{
				DWORD* pTextNum=pParent->WriteTextData();
				if(pTextNum)
					*pTextNum=GetStringNum(item->text);
			}
		}
		else
		{
			pNode=CreateNode(pParent,tagName);
			if(pNode)
			{
				// ����� ���� ��������� ���� ������
				int loaded=pNode->LoadNodeXMLData(item);
				if(loaded<=0)		// ���� �� ��������
				{
					if(!loaded)
						Print("������ � ������ ����. ��������");
					delete pNode;
				}
				else
				{
					if(loaded==1)	// ���� �������� � ������ ��������� ��� ��������
					{
						IXMLDOMNodePtr pChild=item->firstChild;
						pNode->child=LoadXMLNodeWithChild(pNode,pChild);
					}
					if(!pFirstNode)
						pFirstNode=pLastNode=pNode;
					else
						pLastNode=pLastNode->next=pNode;
				}
			}
		}
		item=item->nextSibling;
	}
	return pFirstNode;
}


// ���������� ��� ������ ������ ���� �� xml-������
// ���������� 0-���� �� ������� � ������, 1-���� ������, ������ ��������,
// 2 - �� �������� ��������, ��� �������.
void SComponentsNode::LoadComponents(IXMLDOMNodePtr& item)
{
	// ������� ����������
	IXMLDOMNamedNodeMapPtr atr=item->attributes;
	UsedComponents=(BYTE)GetDwordAtr(atr,L"components");
	if(!UsedComponents)
		UsedComponents=(BYTE)-1;
}

// ���������� ��� ������ ������ ���� �� xml-������
// ���������� 0-���� �� ������� � ������, 1-���� ������, ������ ��������,
// 2 - �� �������� ��������, ��� �������.
int STypeNode::LoadNodeXMLData(IXMLDOMNodePtr& item)
{
	IXMLDOMNamedNodeMapPtr atr=item->attributes;
	if(!(Name=GetStringAtr(atr,L"name")))
	{
		PrintTagError(item, "��� �� ����� �����");
		return 0;
	}
	if(IsBadIdent(Name))
	{
		PrintTagError(item,"������������ ��� ����");
		return 0;
	}
	void* val;
	if(typeNames.Lookup((void*)Name,val))
	{
		PrintTagError(item,"������ ����� ����");
		return 0;
	}
	typeNames[(void*)Name]=(void*)1;

	Alias=GetStringAtr(atr,L"alias");
	if(IsBadIdent(Alias))
	{
		PrintTagError(item,"������������ ��������� ����");
		return 0;
	}
	// ������� ����������
	LoadComponents(item);
	// ������� ������� ����
	baseTypes=DelimStrToDwords(atr,L"base",cntBase);
	// ������� ����� ��� �������������
	Creatable=DelimStrToDwords(atr,L"creatable",cntCreatable);
	// ������� �������������
	Abstract=GetDwordAtr(atr,L"abstract")?1:0;
	return 1;	// ���������� ��������
}

// ������� ������������ �����
void STypeNode::SetUsedStrings(DWORD* pStrings)
{
	pStrings[Name]=1;
	pStrings[Alias]=1;
	for(int i=0;i<cntCreatable;i++)
		pStrings[Creatable[i]]=1;
	for(i=0;i<cntBase;i++)
		pStrings[baseTypes[i]]=1;
	TypeCnt++;
	if(cntBase>1)
		AddBaseCnt+=cntBase;
}

// ������������� ������� �����
void STypeNode::CorrectStringNum(DWORD* pNumbers)
{
	Name=pNumbers[Name];
	Alias=pNumbers[Alias];
	for(int i=0;i<cntCreatable;i++)
	{
		if(IsBadIdent(pNumbers[Creatable[i]]))
			Print("������ ��� ��� �������������: %s. ���������",stringsArray->GetAt(pNumbers[Creatable[i]]));
		else
		{
			crTNames.Add(Name);
			crCNames.Add(pNumbers[Creatable[i]]);
		}
	}
	for(i=0;i<cntBase;i++)
		baseTypes[i]=pNumbers[baseTypes[i]];
}

// ���������� ��� ������ ������ ����
void STypeNode::SaveNodeData()
{
	SNode* pNode=child;
	BYTE atrCnt=0,metCnt=0;
	SAttrInfo* pAttrs=pAttrW;
	SMethInfo* pMeths=pMethW;
	// �������� ��������, ������, � ���������� ��
	while(pNode)
	{
		if(pNode->nodeType==ndAttribs)
		{
			SNode* pAtr=pNode->child;
			while(pAtr)
			{
				if(atrCnt++==0xFF)
				{
					Print("��� \"%s\" - ������� ����� ���������.",stringsArray->GetAt(Name));
					throw int(0);
				}
				pAtr->SaveNodeData();
				pAtr=pAtr->next;
			}
		}
		else if(pNode->nodeType==ndMethods || pNode->nodeType==ndHandlers)
		{
			SNode* pAtr=pNode->child;
			while(pAtr)
			{
				if(metCnt++==0xFF)
				{
					Print("��� \"%s\" - ������� ����� �������.",stringsArray->GetAt(Name));
					throw int(0);
				}
				pAtr->SaveNodeData();
				pAtr=pAtr->next;
			}
		}
		pNode=pNode->next;
	}
	pTypeW->Name=Name;
	pTypeW->Alias=Alias;
	pTypeW->Components=UsedComponents;
	pTypeW->Flags=Abstract?1:0;
	pTypeW->AttrCnt=atrCnt;	// ���������� ���������
	pTypeW->MethCnt=metCnt;	// ���������� �������
	pTypeW->BaseCnt=cntBase;
	pTypeW->pBases=0;
	if(cntBase==1)
		pTypeW->pBases=baseTypes[0];
	else
	{
		for(int i=0;i<cntBase;i++)
			*pAddBaseW++=baseTypes[i];
	}
	pTypeW++;
}



// ���������� ��� ������ ������ ���� �� xml-������
// ���������� 0-���� �� ������� � ������, 1-���� ������, ������ ��������,
// 2 - �� �������� ��������, ��� �������.
int SAtributNode::LoadNodeXMLData(IXMLDOMNodePtr& item)
{
	IXMLDOMNamedNodeMapPtr atr=item->attributes;
	if(!(NameRus=GetStringAtr(atr,L"name")))
	{
		PrintTagError(item,"������� �� ����� �����");
		return 0;
	}
	if(IsBadIdent(NameRus))
	{
		PrintTagError(item,"������ ��� ��������");
		return 0;
	}
	NameEng=GetStringAtr(atr,L"alias");
	if(IsBadIdent(NameEng))
	{
		PrintTagError(item,"������ ��������� ��������");
		return 0;
	}
	// �������� �� ����������
	LoadComponents(item);
	// �������� �� InExprOnly
	if(GetDwordAtr(atr,L"InExprOnly"))
		nodeType=ndAttribInExpr;
	// ��������� ���
	Type=GetStringAtr(atr,L"type");
	return 1;	// ���������� ��������
}


// ������� ������������ �����
void SAtributNode::SetUsedStrings(DWORD* pStrings)
{
	pStrings[NameRus]= 1;
	pStrings[NameEng]= 1;
	pStrings[Description]= 1;
	pStrings[Type]= 1;
	AttrCnt++;
}

// ������������� ������� �����
void SAtributNode::CorrectStringNum(DWORD* pNumbers)
{
	NameRus=pNumbers[NameRus];
	NameEng=pNumbers[NameEng];
	Description=pNumbers[Description];
	Type=pNumbers[Type];
}

// ���������� ��� ������ ������ ����
void SAtributNode::SaveNodeData()
{
	pAttrW->Components=UsedComponents;
	pAttrW->Flags=nodeType==ndAttrib?0:1;	// 0x1 - InExprOnly
	pAttrW->NameRus=NameRus;
	pAttrW->NameEng=NameEng;
	pAttrW->Descr=Description;
	pAttrW++->strTypeName=Type;
}

// ���������� ��� ������ ������ ���� �� xml-������
// ���������� 0-���� �� ������� � ������, 1-���� ������, ������ ��������,
// 2 - �� �������� ��������, ��� �������.
int SMethodNode::LoadNodeXMLData(IXMLDOMNodePtr& item)
{
	IXMLDOMNamedNodeMapPtr atr=item->attributes;
	if(!(NameRus=GetStringAtr(atr,L"name")))
	{
		PrintTagError(item,"����� �� ����� �����");
		return 0;
	}
	if(IsBadIdent(NameRus))
	{
		PrintTagError(item,"������ ��� ������");
		return 0;
	}
	NameEng=GetStringAtr(atr,L"alias");
	if(IsBadIdent(NameEng))
	{
		PrintTagError(item,"������ ��������� ������");
		return 0;
	}
	// ������� ����������
	LoadComponents(item);
	return 1;
}

// ������� ������������ �����
void SMethodNode::SetUsedStrings(DWORD* pStrings)
{
	pStrings[NameRus]=1;
	pStrings[NameEng]=1;
	pStrings[Description]=1;
	pStrings[RetDescription]=1;
	for(int i=0;i<RetTypesCount;i++)
		pStrings[RetTypes[i]]=1;
	// ���� ������� ������ ���� params, ����� child �������� ����� �� ������ ��������
	if(child)
	{
		if(child->nodeType==ndParams)
		{
			SNode* pDel=child;
			child=pDel->child;
			pDel->child=NULL;
			delete pDel;
		}
		SNode* cc=child;
		while(cc)
		{
			cc->parent=this;
			cc=cc->next;
		}
	}
	MethCnt++;
	if(RetTypesCount>1)
	{
		//Print("many rettypes: %s::%s",stringsArray->GetAt(((STypeNode*)parent->parent)->Name),
		//	stringsArray->GetAt(NameRus));
		AddRetTypeCnt+=RetTypesCount;
	}
}

// ������������� ������� �����
void SMethodNode::CorrectStringNum(DWORD* pNumbers)
{
	NameRus=pNumbers[NameRus];
	NameEng=pNumbers[NameEng];
	Description=pNumbers[Description];
	RetDescription=pNumbers[RetDescription];
	for(int i=0;i<RetTypesCount;i++)
		RetTypes[i]=pNumbers[RetTypes[i]];
}

// ���������� ��� ������ ������ ����
void SMethodNode::SaveNodeData()
{
	char* pParams=(char*)pParaW;
	int cntParam=0;
	SNode* pNode=child;
	while(pNode)
	{
		if(cntParam++==0xFF)
		{
			Print("����� \"%s::%s\": ������� ����� ����������.",stringsArray->GetAt(((STypeNode*)parent)->Name),stringsArray->GetAt(NameRus));
			throw int(0);
		}
		pNode->SaveNodeData();
		pNode=pNode->next;
	}
	
	pMethW->Components=UsedComponents;
	pMethW->flags=(nodeType==ndFunc?1:(nodeType==ndProc?0:2)) | (parent->nodeType==ndHandlers?4:0);// 0x1 func, 0x2 - funcinexpr, 0x4 - handler
	pMethW->ParamCnt=cntParam;
	pMethW->NameRus=NameRus;
	pMethW->NameEng=NameEng;
	pMethW->Descr=Description;
	pMethW->retDescr=RetDescription;
	pMethW->retTypeCnt=RetTypesCount;
	if(RetTypesCount<2)
		pMethW->strRetTypeName=RetTypesCount?RetTypes[0]:0;
	else
	{
		pMethW->strRetTypeName=(DWORD)(((char*)pAddRetTypeW)-TypeBlock);
		for(int i=0;i<RetTypesCount;i++)
			*pAddRetTypeW++=RetTypes[i];
	}
	pMethW++;
}

// ���������� ��� ������ ������ ���� �� xml-������
// ���������� 0-���� �� ������� � ������, 1-���� ������, ������ ��������,
// 2 - �� �������� ��������, ��� �������.
int SParamNode::LoadNodeXMLData(IXMLDOMNodePtr& item)
{
	IXMLDOMNamedNodeMapPtr atr=item->attributes;
	if(!(Name=GetStringAtr(atr,L"name")))
	{
		PrintTagError(item,"�������� ������ �� ����� �����");
		return 0;
	}
	IXMLDOMNodePtr val;
	atr->raw_getNamedItem(L"default",&val);
	if(val!=NULL)
	{
		CString txt=(LPCTSTR)val->text;
		if(txt.IsEmpty())
			txt='_';
		Default=GetStringNum(txt);
	}
	Types=DelimStrToDwords(atr,L"type",TypesCount);
	return 1; // ���������� ��������
}

void SParamNode::SetUsedStrings(DWORD* pStrings)
{
	pStrings[Name]=1;
	pStrings[Default]=1;
	pStrings[Description]=1;
	for(int i=0;i<TypesCount;i++)
		pStrings[Types[i]]=1;
	ParaCnt++;
	if(TypesCount>1)
	{
		AddParaTypeCnt+=TypesCount;
		//Print("many param types: %s::%s::%s",stringsArray->GetAt(((STypeNode*)parent->parent->parent)->Name),
		//	stringsArray->GetAt((((SMethodNode*)parent)->NameRus)),stringsArray->GetAt(Name));
	}
}

// ������������� ������� �����
void SParamNode::CorrectStringNum(DWORD* pNumbers)
{
	Name=pNumbers[Name];
	Default=pNumbers[Default];
	Description=pNumbers[Description];
	for(int i=0;i<TypesCount;i++)
		Types[i]=pNumbers[Types[i]];
}

// ���������� ��� ������ ������ ����
void SParamNode::SaveNodeData()
{
	pParaW->DefVal=Default;
	pParaW->Descr=Description;
	pParaW->TypeCnt=TypesCount;
	if(TypesCount<2)
		pParaW->strTypeName=TypesCount?Types[0]:0;
	else
	{
		pParaW->strTypeName=(DWORD)(((char*)pAddParaTypeW)-TypeBlock);
		for(int i=0;i<TypesCount;i++)
			*pAddParaTypeW++=Types[i];
	}
	pParaW++->Name=Name;
}

// ���������� ��� ������ ������ ���� �� xml-������
// ���������� 0-���� �� ������� � ������, 1-���� ������, ������ ��������,
// 2 - �� �������� ��������, ��� �������.
int SReturnNode::LoadNodeXMLData(IXMLDOMNodePtr& item)
{
	// ���� ���� return, ������ ����� - �������
	SMethodNode* pMethod=(SMethodNode*)parent;
	pMethod->nodeType=ndFunc;
	IXMLDOMNamedNodeMapPtr atr=item->attributes;
	pMethod->RetTypes=DelimStrToDwords(atr,L"type",pMethod->RetTypesCount);
	if(GetDwordAtr(atr,L"InExprOnly"))
		pMethod->nodeType=ndFuncInExpr;
	IXMLDOMNodePtr pChild=item->firstChild;
	while(pChild!=NULL)
	{
		if(CString((LPCTSTR)pChild->baseName).IsEmpty())
		{
			pMethod->RetDescription=GetStringNum(pChild->text);
			break;
		}
		pChild=pChild->nextSibling;
	}
	return -1;
}

/*
� �������� �������� � ������ ����� ����� �������
�������������� ������, �� ���� � �������� ��������
�������� ����� ������, � ����� ��������� �� �������� ����.
������� ����� ��������� xml ����� ��������� ������ ��������
� ���������� ����������� �����, � ��������� ������.
������ ���� ������ � ���� ������� ��������� 1 �� ��������
����������� �� �����.
����� ������ ���������, � � ������ ���������� ������� ���������,
��� ��������� �� ������� �����, � � ��������� �������� ������������
����� ������ ����� � ������ ���������.
� ������, ���� ����� ����� ���� �������, ����� ��������� ������,
�������� ���� ��������������� ������ ����� �����.
*/

// ����� ������ ��� ������� ������������ �����
static void SetUsedStrings(SNode* pNode,DWORD* strings)
{
	while(pNode)
	{
		pNode->SetUsedStrings(strings);
		SetUsedStrings(pNode->child,strings);
		pNode=pNode->next;
	}
}

// ���� ������ ����� ����������, ������� ������,
// ����� ���� ����� ��������������� ������ ����� �����
static void CorrectStrings(SNode* pNode,DWORD* numbers)
{
	while(pNode)
	{
		pNode->CorrectStringNum(numbers);
		CorrectStrings(pNode->child,numbers);
		pNode=pNode->next;
	}
}
/*
���� 1 - ��������� xml
���� 2 - ����� � ������������� ������ ����� +
		��������� ���������� �����, ���������, �������, ����������, ����������, �����������
���� 3 - �������� ���.

*/

int DoLayout(SNode* pNode)
{
	int sType=TypeCnt*sizeof(STypeInfo);
	int sMeth=MethCnt*sizeof(SMethInfo);
	int sAttr=AttrCnt*sizeof(SAttrInfo);
	int sPara=ParaCnt*sizeof(SParaInfo);
	int sAddBaseCnt=AddBaseCnt*sizeof(DWORD);
	int sAddRetTypeCnt=AddRetTypeCnt*sizeof(DWORD);
	int sAddParaTypeCnt=AddParaTypeCnt*sizeof(DWORD);
	//int sCreatable=2*crTNames.GetSize()*sizeof(DWORD);
	int sTotal=sType+sMeth+sAttr+sPara+sAddBaseCnt+sAddRetTypeCnt+sAddParaTypeCnt;//+sCreatable;
	TypeBlock=new char[sTotal];

	pTypeW=(STypeInfo*)TypeBlock;
	pAttrW=(SAttrInfo*)(TypeBlock+sType);
	pMethW=(SMethInfo*)(TypeBlock+sType+sAttr);
	pParaW=(SParaInfo*)(TypeBlock+sType+sAttr+sMeth);
	pAddBaseW=(DWORD*)(TypeBlock+sType+sAttr+sMeth+sPara);
	pAddRetTypeW=(DWORD*)(TypeBlock+sType+sAttr+sMeth+sPara+sAddBaseCnt);
	pAddParaTypeW=(DWORD*)(TypeBlock+sType+sAttr+sMeth+sPara+sAddBaseCnt+sAddRetTypeCnt);

	pNode->SaveNodeWithChild();

	return sTotal;
}

// ����������� ����� xml � �������� ���� ��������
void DoConvert(CString path, CString dest)
{
	IXMLDOMDocumentPtr pDoc;
	bool Error=false;
	CString ErrorText;
	SNode* pRootNode=NULL;
	try
	{
		// ������� ������
		HRESULT hr = pDoc.CreateInstance(CLSID_DOMDocument);
		if(pDoc==NULL)
		{
			ErrorText="�� ������� ������� Microsoft XML parser.";
			throw int(0);
		}
		// ��������� xml ����
		pDoc->validateOnParse=VARIANT_FALSE;//VARIANT_TRUE;
		pDoc->preserveWhiteSpace=VARIANT_FALSE;
		hr=pDoc->load((LPCTSTR)path);
		if(!hr)
		{
			IXMLDOMParseErrorPtr pParsingErr=pDoc->parseError;
			int line=pParsingErr->line,pos=pParsingErr->linepos;
			ErrorText.Format("XML parser. ��� 0x%08X",pParsingErr->errorCode);
			if(line)
			{
				CString txt;
				txt.Format("������ %i, ������� %i",line,pos);
				ErrorText=ErrorText+"\n"+txt;
			}
			ErrorText=ErrorText+"\n"+(LPCTSTR)pParsingErr->reason;
			throw int(0);
		}
		CStringArray strings;	// ����� ����� ��������� ������
		strings.Add("");
		stringsArray=&strings;
		// �������� ������ ������� ��� ���������
		IXMLDOMNodePtr item=pDoc->documentElement->firstChild;
		// � ���������� ���.
		pRootNode=SNode::LoadXMLNodeWithChild(NULL,item);
		if(pRootNode)
		{
			// ������ �������� �� "������" ������ � ���������� ������ ������ ��� ��������
			int StringsCount=strings.GetSize();
			DWORD* StringsNum=new DWORD[StringsCount];
			memset(StringsNum,0,StringsCount*sizeof(DWORD));
			SetUsedStrings(pRootNode,StringsNum);	// ����� ������
			StringsNum[0]=1;
			int strSize=0;
			for(int i=0,j=0;i<StringsCount;i++)		// �������� �� ������� �������
			{
				if(StringsNum[i])	// ������ ������������. ������ �� ����� � �������� �������
				{
					StringsNum[i]=j++;
					strSize+=strings[i].GetLength()+1;
				}
				else				// ������ �� ������������. ������ ��
					strings.RemoveAt(j);
			}
			//if(j<StringsCount)	// ������������ �� ��� ������. �������������
			//{
				StringsCount=j;
				CorrectStrings(pRootNode,StringsNum);
			//}
			delete [] StringsNum;	// ������ ��������� ������
			// ������ ���� ������� ������ � IStream
			IStreamPtr pStream;
			if(S_OK!=CreateStreamOnHGlobal(NULL,TRUE,&pStream) || pStream==NULL)
			{
				ErrorText="�� ������� ������� �������� �����";
				throw int(0);
			}
			char magic[16]={(char)0xad,(char)0xe5,(char)0xba,(char)0xca,(char)0x95,(char)0x00,
				(char)0x47,(char)0x58,(char)0x88,(char)0x81,(char)0xc8,(char)0xe7,(char)0x50,
				(char)0x6c,(char)0x20,(char)0x7e};
			pStream->Write(magic,16,NULL);
			
			// ������� ���������� �����
			pStream->Write(&StringsCount,4,NULL);
			pStream->Write(&strSize,4,NULL);
			// ������� ����
			pStream->Write(&TypeCnt,4,NULL);
			pStream->Write(&AttrCnt,4,NULL);
			pStream->Write(&MethCnt,4,NULL);
			pStream->Write(&ParaCnt,4,NULL);
			pStream->Write(&AddBaseCnt,4,NULL);
			pStream->Write(&AddRetTypeCnt,4,NULL);
			pStream->Write(&AddParaTypeCnt,4,NULL);
			DWORD crCnt=crTNames.GetSize();
			pStream->Write(&crCnt,4,NULL);

			// � ������ ������ ���� ������
			for(i=0;i<StringsCount;i++)
			{
				CString txt=strings[i];
				pStream->Write((LPCTSTR)txt,txt.GetLength()+1,NULL);
			}
			// ������ ������� ����
			int sizeInfo=DoLayout(pRootNode);
			pStream->Write(TypeBlock,sizeInfo,NULL);
			// ������ ����������� ����
			for(i=0;i<crCnt;i++)
			{
				DWORD val=crTNames[i];
				pStream->Write(&val,4,NULL);
				val=crCNames[i];
				pStream->Write(&val,4,NULL);
			}
			STATSTG stg;
			pStream->Stat(&stg,STATFLAG_NONAME);
			DWORD UnCompressSize=stg.cbSize.QuadPart,CompressSize=0;

			// � ������ � ������� zlibeng ������� ������������ ������
			IzlibEnginePtr zlibeng;
			if(S_OK!=zlibeng.CreateInstance(CLSID_zlibEngine) || zlibeng==NULL)
			{
				ErrorText="�� ������� ������� ������ zlibeng";
				throw int(0);
			}
			IStreamPtr pZipStream;
			if(S_OK!=CreateStreamOnHGlobal(NULL,TRUE,&pZipStream) || pZipStream==NULL)
			{
				ErrorText="�� ������� ������� �������� ����� ��� ������";
				throw int(0);
			}
			// �������� � ������ ������
			LARGE_INTEGER liZero;
			liZero.QuadPart=0;
			pStream->Seek(liZero,STREAM_SEEK_SET,NULL);
			// � ������ ���
			if(S_OK!=zlibeng->pkCompress(pStream,pZipStream))
			{
				ErrorText="�� ������� ����� ������";
				throw int(0);
			}
			pStream=pZipStream;

			HGLOBAL hStream=NULL;
			if(S_OK!=GetHGlobalFromStream(pStream,&hStream))
			{
				ErrorText="�� ������� ��������� ������ � ����";
				throw int(0);
			}
			if(hStream)
			{
				pStream->Stat(&stg,STATFLAG_NONAME);
				LPVOID lpMemStream=GlobalLock(hStream);
				CFile f(dest,CFile::modeWrite|CFile::modeCreate);
				f.Write(lpMemStream,CompressSize=stg.cbSize.QuadPart);
				f.Close();
				GlobalUnlock(hStream);
			}
			ErrorText.Format("*************************************************\n"
				"���� \"%s\" ������������ � \"%s\"\n"
				"���������� ��������� ������� %i\n"
				"����� ������ �����.......... %i\n"
				"���������� �����............ %i\n"
				"���������� ���������........ %i\n"
				"���������� �������.......... %i\n"
				"���������� ����������....... %i\n"
				"���������� ����������� ����� %i\n"
				"������ ����� ������......... %i\n"
				"����� ������ �� ������...... %i\n"
				"������ ����� ������......... %i\n",
				path,dest,StringsCount,strSize,
				TypeCnt,AttrCnt,MethCnt,ParaCnt,
				crCnt,
				sizeInfo,UnCompressSize,CompressSize);
		}
	}
	catch(_com_error err)
	{
		ErrorText=err.ErrorMessage();
		Error=true;
	}
	catch(CFileException* pE)
	{
		pE->GetErrorMessage(ErrorText.GetBuffer(1000),1000);
		ErrorText.ReleaseBuffer();
		pE->Delete();
		Error=true;
	}
	catch(int){Error=true;}
	catch(...){ErrorText="����������� ������";Error=true;}
	if(Error)
		Print("��������� ������");
	if(!ErrorText.IsEmpty())
		Print(ErrorText);
}