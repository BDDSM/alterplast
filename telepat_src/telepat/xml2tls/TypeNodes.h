// TypeNodes.h
#ifndef TYPENODES_H
#define TYPENODES_H
/*
��������� ��������� ����� �������� ���������� � �����.
���� tls ���� ������� zip, ��������� � ������� zlibeng
����� ���������� ����� ���������:
DWORD - ���������� ��������� �������.
DWORD - ���������� �����
DWORD - ���������� ���������
DWORD - ���������� �������
DWORD - ���������� ����������
DWORD - ���������� ������
DWORD - ���������� ���������������
DWORD - ���������� �������� ����������
DWORD - ���������� ����������������
������
����
��������
������
���������
�������
��������������
�����������������
���������������

����� ���� ��������� ������, ����������� 0.
����� ������������ ���� ������, ������� � ���������.
������� ������ ������: ����, �������, ���������.
��� ��������� ������� ��� ���������� ������������ ���� ���� 0.
������ ������ ����:
BYTE - ��� ����
������ ���� (������� �� ���� ����)

type
	name,
	alias,
	abstract,	8 bit
	basecnt,	8 bit
	atrcount,	8 bit
	metcnt,		8 bit
	attribs,
	methods,
	bases,

attrib
	parenttype,
	name,
	alias,
	descr,
	typename,
	flags - InExprOnly,

method
	parenttype
	name,
	alias,
	descr,
	retdescr,
	flags - proc, func, funcinexpr, handler,
	paramcnt,		16 bit
	rettypecnt,		16 bit
	params,
	rettype

param
	name,
	default,
	descr,
	typecnt
	type

creatables
	type-name
bases
	type


��������� ��. �����
*********************************************************************
������ ����� ������ �������������� ���������� ��� �������� ���������.
������ ������ - ������ ������� � ������� �����.
*********************************************************************
������ ����������� � ������� ������, �� ����� ���������� ������
��� ���� �����, � ������� ����������� ��� ����, ���������������
���� �� ������. ������������ ������ ���� ���������� ����� ��� �����
�����. ������� ��������� delete � ������ ���� ����.
*/

/*
��������� �������� ����� ������.
������ ����� ������� �� ��������� �����.
������ ���� ����� ���� ���, ���������� � ������ �����.
� ����� ����� ������, ���������� ����� ����� ����.
*/
#pragma pack(push,1)

struct STypeInfo;
struct SAttrInfo;
struct SMethInfo;
struct SParaInfo;

struct SHeadType
{
	DWORD StrCount;		// ���������� ��������� �������.
	DWORD StrSize;		// ������ �����
	DWORD TypeCount;	// ���������� �����
	DWORD AttrCount;	// ���������� ���������
	DWORD MethCount;	// ���������� �������
	DWORD ParaCount;	// ���������� ����������
	DWORD AddBaseCount;	// ���������� ������
	DWORD AddRetCount;	// ���������� ���������������
	DWORD AddPTypeCount;// ���������� �������� ����������
	DWORD CrObCount;	// ���������� ����������������
};

struct STypeInfo
{
	DWORD		Name;		// ���������� ��� ����
	DWORD		Alias;		// ���������������� ��� ����
	BYTE		Flags;		// 0x1 - ��� �����������
	BYTE		BaseCnt;	// ���������� ������� �����
	BYTE		AttrCnt;	// ���������� ���������
	BYTE		MethCnt;	// ���������� �������
	DWORD		pBases;		// ������� ������ (���� BaseCnt==0, �� ��������������� ��������� �� ������� ���, ����� ��������� �� ������ ���������� �� ������� ����)
	BYTE		Components;	// ������������ � �����������
};

struct STTypeInfo;
struct STAttrInfo;
struct STMethInfo;

struct STTypeInfo
{
	DWORD		Name;		// ���������� ��� ����
	DWORD		Alias;		// ���������������� ��� ����
	BYTE		Flags;		// 0x1 - ��� �����������
	BYTE		BaseCnt;	// ���������� ������� �����
	BYTE		AttrCnt;	// ���������� ���������
	BYTE		MethCnt;	// ���������� �������
	STTypeInfo*	pBases;		// ������� ������ (���� BaseCnt==0, �� ��������������� ��������� �� ������� ���, ����� ��������� �� ������ ���������� �� ������� ����)
	STAttrInfo*	pAttribs;	// ��������
	STMethInfo*	pMethods;	// ������
};

struct SAttrInfo
{
	BYTE		Flags;	// 0x1 - InExprOnly
	DWORD		NameRus;
	DWORD		NameEng;
	DWORD		Descr;
	DWORD		strTypeName;
	BYTE		Components;
};

struct STAttrInfo
{
	BYTE		Flags;	// 0x1 - InExprOnly
	DWORD		NameRus;
	DWORD		NameEng;
	DWORD		Descr;
	DWORD		strTypeName;
	STTypeInfo*	ParentType;
};

struct SMethInfo
{
	BYTE		flags;// 0x1 func, 0x2 - funcinexpr, 0x4 - handler
	BYTE		ParamCnt;
	BYTE		retTypeCnt;
	DWORD		NameRus;
	DWORD		NameEng;
	DWORD		Descr;
	DWORD		retDescr;
	DWORD		strRetTypeName;
	BYTE		Components;
};

struct STMethInfo
{
	BYTE		flags;// 0x1 func, 0x2 - funcinexpr, 0x4 - handler
	BYTE		ParamCnt;
	BYTE		retTypeCnt;
	DWORD		NameRus;
	DWORD		NameEng;
	DWORD		Descr;
	DWORD		retDescr;
	DWORD		strRetTypeName;
	STTypeInfo*	ParentType;
	SParaInfo*	pParams;
};

struct SParaInfo
{
	DWORD	Name;
	DWORD	DefVal;
	DWORD	Descr;
	BYTE	TypeCnt;
	DWORD	strTypeName;
};

#ifdef LOAD_XML_TYPE

enum NodeTypes
{
	ndSimple=1,
	ndType,
	ndAttribs,
	ndAttrib,
	ndAttribInExpr,
	ndMethods,
	ndProc,
	ndFunc,
	ndFuncInExpr,
	ndParams,
	ndParam,
	ndHandlers,
	ndReturn,
	ndLast
};

// ������� ����� ���� ������ �����
struct SNode
{
	SNode(NodeTypes t=ndSimple,SNode* p=NULL,SNode* n=NULL,SNode* c=NULL)
		:nodeType(t),parent(p),next(n),child(c){}
	NodeTypes nodeType;		// ��� ����
	// ����� �����
	SNode* parent;
	SNode* next;
	SNode* child;

	virtual ~SNode(){if(child)delete child;if(next)delete next;}
	//// ������ � ������ ��� �������� �� xml � ����������� � ������ ��������
	// �������� ���� �� ���� ����
	static SNode* CreateNode(SNode* pParent,LPCTSTR xmlName);
	// �������� ���� �� xml-������
	// ��������� ����� ������, �������� ������ ������� ������,
	// ��������� �������� � ���������
	static SNode* LoadXMLNodeWithChild(SNode* pParent,IXMLDOMNodePtr& item);
	// ���������� ���� � ��������� � �������� ����
	// ���������� ����� ������, �������, ���������
	void SaveNodeWithChild();
	// ���������� ��� ������ ������ ���� �� xml-������
	// ���������� 0-������ ��������, 1-���� ������, ������ ��������,
	// 2 - �� �������� ��������, ��� �������, -1 - ���� ��������, �� �� ���������� � ������
	virtual int LoadNodeXMLData(IXMLDOMNodePtr& item){return 1;}
	// ��������� ��������� ������ ���� ��� �� ����������� �
	// �������� ������ ��� ��������
	virtual DWORD* WriteTextData(){return NULL;}
	// ������� ������������ ����� � ���������� ������� ������
	// � ������ ��������
	virtual void SetUsedStrings(DWORD* pStrings){}
	// ������������� ������� �����
	virtual void CorrectStringNum(DWORD* pNumbers){}
	// ���������� ��� ������ ������ ����
	virtual void SaveNodeData(){}
};
/*
������� ���� ��� ���������, ��������� �� ������� ���������
*/
struct SComponentsNode:public SNode
{
	SComponentsNode(NodeTypes t=ndSimple, SNode* p=NULL, SNode* n=NULL, SNode* c=NULL)
		:SNode(t,p,n,c){}
	BYTE UsedComponents;
	// ���������� ��� ������ ������ ���� �� xml-������
	// ���������� 0-���� �� ������� � ������, 1-���� ������, ������ ��������,
	// 2 - �� �������� ��������, ��� �������.
	void LoadComponents(IXMLDOMNodePtr& item);
};

/*
���� �������������� ���
*/
struct STypeNode:public SComponentsNode
{
	// ������ � ����
	DWORD Name;			// ���������� ��� ����
	DWORD Alias;		// ���������������� ��� ����
	DWORD Abstract;		// ��� �����������
	DWORD cntCreatable;	// ���������� ���� ��� �������������
	DWORD cntBase;		// ���������� ������� �������
	DWORD*	Creatable;		// ������ ����� � �������, ������������� � �������������
	DWORD*	baseTypes;		// ������ ����� ���� ������� �����

	STypeNode(NodeTypes t=ndSimple,SNode* p=NULL,SNode* n=NULL,SNode* c=NULL)
		:SComponentsNode(t,p,n,c),Name(0),Alias(0),baseTypes(NULL),Creatable(NULL){}
	~STypeNode(){if(baseTypes)delete[]baseTypes;if(Creatable)delete[]Creatable;}
	// ���������� ��� ������ ������ ����
	virtual void SaveNodeData();
	// ���������� ��� ������ ������ ���� �� xml-������
	// ���������� 0-���� �� ������� � ������, 1-���� ������, ������ ��������,
	// 2 - �� �������� ��������, ��� �������.
	virtual int LoadNodeXMLData(IXMLDOMNodePtr& item);
	// ���������� ������� ������ � ����� � ������� ������������ �����
	virtual void SetUsedStrings(DWORD* pStrings);
	// ������������� ������� �����
	virtual void CorrectStringNum(DWORD* pNumbers);
};

/*
���� �������������� �������
type - ndAttrib | ndAttribInExpr
*/
struct SAtributNode:public SComponentsNode
{
	SAtributNode(NodeTypes t=ndSimple,SNode* p=NULL,SNode* n=NULL,SNode* c=NULL)
		:SComponentsNode(t,p,n,c){NameRus=NameEng=Description=0;}
	// ������ �� ��������
	DWORD NameRus;
	DWORD NameEng;
	DWORD Description;
	DWORD Type;
	// ���������� ��� ������ ������ ���� �� xml-������
	// ���������� 0-���� �� ������� � ������, 1-���� ������, ������ ��������,
	// 2 - �� �������� ��������, ��� �������.
	virtual int LoadNodeXMLData(IXMLDOMNodePtr& item);
	// ���������� ��� ������ ������ ����
	virtual void SaveNodeData();
	// ��������� ��������� ������ ���� ��� �� ����������� �
	// �������� ������ ��� ��������
	virtual DWORD* WriteTextData(){return &Description;}
	// ���������� ������� ������ � ����� � ������� ������������ �����
	virtual void SetUsedStrings(DWORD* pStrings);
	// ������������� ������� �����
	virtual void CorrectStringNum(DWORD* pNumbers);
};

/*
���� �������������� �����
type - ndProc | ndFunc | ndFuncInExpr
*/
struct SMethodNode:public SComponentsNode
{
	// �������� ������
	DWORD NameRus;			// ����� �������� �����
	DWORD NameEng;			// ����� ����������� �����
	DWORD Description;		// ����� ��������
	DWORD RetDescription;	// ����� �������� ������������� ��������
	DWORD RetTypesCount;	// ���������� ������������ �����
	DWORD* RetTypes;

	SMethodNode(NodeTypes t=ndSimple,SNode* p=NULL,SNode* n=NULL,SNode* c=NULL)
		:SComponentsNode(t,p,n,c),RetTypesCount(0),RetTypes(NULL){NameRus=NameEng=Description=RetDescription=0;}
	~SMethodNode(){if(RetTypes)delete [] RetTypes;}
	// ���������� ��� ������ ������ ����
	virtual void SaveNodeData();
	// ���������� ��� ������ ������ ���� �� xml-������
	// ���������� 0-���� �� ������� � ������, 1-���� ������, ������ ��������,
	// 2 - �� �������� ��������, ��� �������.
	virtual int LoadNodeXMLData(IXMLDOMNodePtr& item);
	// ��������� ��������� ������ ���� ��� �� ����������� �
	// �������� ������ ��� ��������
	virtual DWORD* WriteTextData(){return &Description;}
	// ���������� ������� ������ � ����� � ������� ������������ �����
	virtual void SetUsedStrings(DWORD* pStrings);
	// ������������� ������� �����
	virtual void CorrectStringNum(DWORD* pNumbers);
};

/*
����, �������������� ��������
*/
struct SParamNode:public SNode
{
	// �������� ���������
	DWORD Name;
	DWORD Default;
	DWORD Description;
	DWORD TypesCount;
	DWORD* Types;

	SParamNode(NodeTypes t=(NodeTypes)0,SNode* p=NULL,SNode* n=NULL,SNode* c=NULL)
		:SNode(t,p,n,c),TypesCount(0){Name=Default=Description=0;}
	// ���������� ��� ������ ������ ����
	virtual void SaveNodeData();
	// ���������� ��� ������ ������ ���� �� xml-������
	// ���������� 0-���� �� ������� � ������, 1-���� ������, ������ ��������,
	// 2 - �� �������� ��������, ��� �������.
	virtual int LoadNodeXMLData(IXMLDOMNodePtr& item);
	// ��������� ��������� ������ ���� ��� �� ����������� �
	// �������� ������ ��� ��������
	virtual DWORD* WriteTextData(){return &Description;}
	// ���������� ������� ������ � ����� � ������� ������������ �����
	virtual void SetUsedStrings(DWORD* pStrings);
	// ������������� ������� �����
	virtual void CorrectStringNum(DWORD* pNumbers);
};

/*
���� ��� return - ������������ ��� �������� �� XML
���������� ������ � ���� ������������� ������
*/
struct SReturnNode:public SNode
{
	SReturnNode(NodeTypes t=(NodeTypes)0,SNode* p=NULL,SNode* n=NULL,SNode* c=NULL)
		:SNode(t,p,n,c){}
	// ���������� ��� ������ ������ ���� �� xml-������
	// ���������� 0-���� �� ������� � ������, 1-���� ������, ������ ��������,
	// 2 - �� �������� ��������, ��� �������.
	virtual int LoadNodeXMLData(IXMLDOMNodePtr& item);
};
#endif

#pragma pack(pop)

#endif