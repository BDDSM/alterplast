// mycontextbase.h
// ������������ ���� ��� �������� ������, ������������ ���������������� CBLContext'�
// ��������������:
// - ��������������� ��������� ������ � CBLContext::RegisterContextClass
// - ��������� ���������������� ������� ��� ������ ����������� ������
// - ������ � ������ �������, ������� ����� ������ ������, ���������������
//		������ ������ � ����������, �������� � �����
// - ������ � ������ ���������� �������, ������� ����� ������ ��������,
//		���������/������ �������� ����� �����������, �������� � �����.
// �����������:
// - ������ ���� ������ � ������ ������������ ������ ������.

// robert: ������� RBL_METH � RBL_PROP ��� ���������� ����������� ������� � �������

#ifndef MYCONTEXTBASE_H
#define MYCONTEXTBASE_H
#include "blmap.h"
#include "blptr.h"

typedef int (CBLContext::*PF_GET_PROP)(CValue&)const;
typedef int (CBLContext::*PF_SET_PROP)(CValue const&);
typedef int (CBLContext::*PF_CALL_AS_PROC)(CValue**);
typedef int (CBLContext::*PF_CALL_AS_FUNC)(CValue&,CValue**);
typedef int (CBLContext::*PF_GET_PARAM_DEF)(int,CValue*)const;
typedef int (PF_CTOR_PARAM_DEF)(int,CValue*);

#define RDECLARE_MY_CONTEXT \
public:\
	static _S_MyContextBaseInfo		m_myCtxBaseInfo[]; \
	static const _S_MethDef			m_MethDef[]; \
	static const _S_PropDef			m_PropDef[]; \
	virtual _S_MyContextBaseInfo* GetBaseInfo()const{return m_myCtxBaseInfo;};


// ����������, ��� ������� �����
class CMyContextBase : public CBLContext  
{
public:
	CMyContextBase();
	virtual ~CMyContextBase();  
	virtual int  CallAsFunc(int,CValue&,CValue**);
	virtual int  CallAsProc(int,CValue**);
	virtual int  FindMethod(char const *)const;
	virtual int  FindProp(char const *)const;
    //virtual char const*  GetCode(void)const;
	virtual int  GetDestroyUnRefd(void)const;
	//virtual CObjID GetID(void)const;
	virtual char const*  GetMethodName(int,int)const;
	virtual int  GetNMethods(void)const;
	virtual int  GetNParams(int)const;
	virtual int  GetNProps(void)const;
	virtual int  GetParamDefValue(int,int,CValue*)const;
	virtual char const* GetPropName(int,int)const;
	virtual int  GetPropVal(int,CValue&)const;
	//virtual long  GetTypeID(void)const;
	virtual char const* GetTypeString(void)const;
	//virtual CType GetValueType(void)const;
	virtual int  HasRetVal(int)const;
	virtual int  IsExactValue(void)const;
	virtual int  IsOleContext(void)const;
	virtual int  IsPropReadable(int)const;
	virtual int  IsPropWritable(int)const;
	virtual int  IsSerializable(void);
	virtual int  SaveToString(CString &);
	virtual int  SetPropVal(int,CValue const&);

	virtual struct _S_MyContextBaseInfo* GetBaseInfo()const=0;
	static void InitAllContextClasses();
	static void DoneAllContextClasses();

    virtual BOOL _Create(CValue** ppValue) {return TRUE;};
};

// ������ ���������������

// �������� ����� ������ � ����� �������
struct _S_MethDef
{
	const char *Names[2];	// ����� ������
	DWORD NumberOfParam;
	PF_CALL_AS_PROC pDoProc;
	PF_CALL_AS_FUNC pDoFunc;
	PF_GET_PARAM_DEF pDefValProc;
};

// ��������� ����� ������ � ����� �������
struct _S_PropDef
{
	const char *Names[2];
	PF_GET_PROP pGetProp;
	PF_SET_PROP pSetProp;
};

// ��������� ���������� � ������-���������
typedef void (*PVVF)();

struct _S_MyContextBaseInfo
{
	// ���������, ������ ���������� ��� ����������
	const _S_MethDef* pMethodDef;	// ����� �������
	const _S_PropDef* pPropDef;		// ����� �������
	const char* EngTypeString;		// ��� ���� ����������
	const char* RusTypeString;		// ��� ���� �������
	BOOL RegToCreate;				// �������������� ����� ��� �������� ��������
	const CRuntimeClass* pRTC;		// ��� ����������� ��� ������������
	int methCount;					// ���������� �������
	int propCount;					// ���������� �������
	PVVF pInitFunc;					// ��������� �� �������-�������������
	PVVF pDoneFunc;					// ��������� �� �������-�������������
    int nFactoryCtorParamCount;     // ���������� ���������� ������������ �������
    //PF_CTOR_PARAM_DEF pCtorParamDef;// ��������� �� ������� ��������� �������� ��������� ��-��������� ��� ������������ �������

	// ���������, ����������� � ����������� ��� �������������
	CBLMap* pMethMap;	// ������� ����� ������ ������
	CBLMap* pPropMap;	// ������� ����� ������ ��������
};

// ������ ��� ������������ ���������� ������-���������
#define DECLARE_MY_CONTEXT() \
public:\
	static _S_MyContextBaseInfo		m_myCtxBaseInfo[]; \
	static const _S_MethDef			m_MethDef[]; \
	static const _S_PropDef			m_PropDef[]; \
	virtual _S_MyContextBaseInfo* GetBaseInfo()const{return m_myCtxBaseInfo;}

#define IMPLEMENT_MY_CONTEXT(parClassName, parEngTypeString, parRusTypeString, parRegToCreate, parInitFunc, parDoneFunc, parFactoryCtorParamCount) \
	_S_MyContextBaseInfo parClassName::m_myCtxBaseInfo[]= \
	{\
		parClassName::m_MethDef, \
		parClassName::m_PropDef, \
		parEngTypeString, \
		parRusTypeString, \
		parRegToCreate, \
		RUNTIME_CLASS(parClassName), \
		sizeof(parClassName::m_MethDef)/sizeof(_S_MethDef) - 1, \
		sizeof(parClassName::m_PropDef)/sizeof(_S_PropDef) - 1, \
		parInitFunc, \
		parDoneFunc, \
        parFactoryCtorParamCount,\
		NULL, NULL \
	};\
	IMPLEMENT_DYNCREATE(parClassName, CBLContext)\
	__declspec (allocate(".blc$b"))\
	static const _S_MyContextBaseInfo *init##parClassName = parClassName::m_myCtxBaseInfo

#define BEGIN_BL_METH_MAP(parClassName) \
	const _S_MethDef parClassName::m_MethDef[]={

#define RBL_METH(parNameEng,parNameRus,parNumberOfParam,parDefValProc) \
	{{#parNameEng,parNameRus},parNumberOfParam,static_cast<PF_CALL_AS_PROC>(NULL),static_cast<PF_CALL_AS_FUNC>(func##parNameEng),static_cast<PF_GET_PARAM_DEF>(parDefValProc)},

#define BL_METH(parNameEng,parNameRus,parNumberOfParam,parDoProc,parDoFunc,parDefValProc) \
	{{parNameEng,parNameRus},parNumberOfParam,static_cast<PF_CALL_AS_PROC>(parDoProc),static_cast<PF_CALL_AS_FUNC>(parDoFunc),static_cast<PF_GET_PARAM_DEF>(parDefValProc)},

#define END_BL_METH_MAP() {{NULL,NULL},0,0,NULL,NULL}};


#define BEGIN_BL_PROP_MAP(parClassName) \
	const _S_PropDef parClassName::m_PropDef[]={

#define RBL_PROP(parNameEng,parNameRus) \
	{{#parNameEng,parNameRus},static_cast<PF_GET_PROP>(get##parNameEng),static_cast<PF_SET_PROP>(set##parNameEng)},

#define BL_PROP(parNameEng,parNameRus,parGet,parSet) \
	{{parNameEng,parNameRus},static_cast<PF_GET_PROP>(parGet),static_cast<PF_SET_PROP>(parSet)},

#define END_BL_PROP_MAP() {{NULL,NULL},NULL,NULL}};

#define IS_BLTYPE(Value, class_name) (Value.GetTypeCode() == AGREGATE_TYPE_1C && Value.GetContext()->GetRuntimeClass() == RUNTIME_CLASS(##class_name))
#define CHECK_BLTYPE(Value, class_name)\
    if (!IS_BLTYPE(Value, class_name))\
    {\
        CBLModule::RaiseExtRuntimeError("������������ �������� ��������.", 0);\
        return FALSE;\
    }

#define GET_ENUM_PROP(type, first, last)\
    type _enum = (type)Value.GetNumeric().operator long();\
    if (_enum < first || _enum > last)\
    {\
        CBLModule::RaiseExtRuntimeError("������������ �������� ��������.", 0);\
        return FALSE;\
    }

// ���������� ��������-������ ��� ������
#define DECLARE_PROPERTY_ARRAY(PropertyName, PropertyType, PropertyKeyType) \
	__declspec(property(get=get_##PropertyName, put=put_##PropertyName)) PropertyType PropertyName []; \
	BOOL get_##PropertyName(PropertyKeyType key);  \
	void put_##PropertyName(PropertyKeyType key, PropertyType newValue);

// ���������� ������� �������� ��� ������
#define DECLARE_ONE_PROPERTY(Property, PropertyType) \
	__declspec(property(get=get_##Property, put=put_##Property)) PropertyType Property; \
	BOOL get_##Property();  \
	void put_##Property(PropertyType newValue);

// ������� ������� ��� ��������, ����� �������� __declspec allocate
#pragma data_seg(".blc$b")
#pragma data_seg()

// ������ ������� � �������� ����� ����� �������� .blc � .data
#pragma comment(linker, "/MERGE:.blc=.data")
#endif