  // ComponentClass.h: interface for the CComponentClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPONENTCLASS_H__5F1978A2_67C1_4AAB_8C12_44790D3700B2__INCLUDED_)
#define AFX_COMPONENTCLASS_H__5F1978A2_67C1_4AAB_8C12_44790D3700B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#include "MapDefMethod.h"
#include "CodeDecodeFile.h"
#include "paramdefs.h"
#include "AddIn.h"
#include "ModuleString.h"
#include "BLModuleWrapper.h"
#include "OXY_Value.h"
#include "SetOfHierarchy.h"
//#include "BaseClassContext.h"
	  
typedef boost::shared_ptr<CValue> CValue_SharedPtr;
typedef CArray<CValue_SharedPtr, CValue_SharedPtr&> ARRAY_VAL; //CArray<CValue*, CValue*> ARRAY_VAL;

//kms
#include "SafeContext.h"
#include "CastContext.h"

//extern CBkEndUI * pBkEndUI;

// class IV7SerializeToString
// {
// public:
// 	virtual BOOL LoadFromString(LPCSTR szSource) = 0;
// };

class IGetModule
{
public:
	virtual CBLModule * GetModule() const = 0;
	virtual CBLModule * GetHostModule() const = 0;
};

class IUserClassContext : public CBLContext, public IContextTree, public IGetModule //, public IV7SerializeToString
{
protected:
	IUserClassContext() : CBLContext(1) {};
public:
	virtual ~IUserClassContext() {};
	
	virtual int GetBaseClass(const CValue &NameClass, CValue & rValue) const = 0;
	virtual BOOL GetBaseClass(const CRuntimeClass* pRTClass, CBLContext** ppBaseObj) const = 0;
	
// 	virtual CBLModule * GetModule() const = 0;
// 	virtual CBLModule * GetHostModule() const = 0;
	
	virtual BOOL SendMessageOwnMod( CValue &rValue, CValue ** ppValue ) = 0;
	virtual void GetPathName( CValue &rValue ) = 0;
	virtual void GetEnvContext( CValue &rValue ) = 0;
	virtual void GetParamsList( CValue &rValue, CValue ** ppValue ) = 0;
	virtual BOOL SetOnIndexParams( CValue &rValue, CValue * * ppValue ) = 0;
	virtual void ThrowException(CValue ** ppValue) = 0;
	virtual void AddDynamicProperty(CValue ** ppValue) = 0;
	virtual int ReplaceInstBaseClasses(const CValue &NameClass, const CValue &Instance, CValue &rValue) = 0;
	virtual int IsObject(const CValue &NameClass, CValue &rValue) const = 0;
	
	virtual int  CallAsFunc(int,CValue &,CValue * *) = 0;	//935
	virtual int  CallAsProc(int,CValue * *) = 0;	//937
	virtual void  DecrRef(void) = 0;	//1086
	virtual int  FindMethod(char const *)const = 0;	//1366
	virtual int  FindProp(char const *)const = 0;	//1369
	virtual char const *  GetCode(void)const = 0;	//1544
	//virtual int  GetDestroyUnRefd(void)const;	//1657
	//virtual void  GetExactValue(CValue &);	//1708
	virtual class CObjID   GetID(void)const = 0;	//1804
	virtual class CBLContextInternalData *  GetInternalData(void) = 0;	//1826
	virtual char const *  GetMethodName(int,int)const = 0;	//1964
	virtual int  GetNMethods(void)const = 0;	//2005
	virtual int  GetNParams(int)const = 0;	//2008
	virtual int  GetNProps(void)const = 0;	//2015
	virtual int  GetParamDefValue(int,int,CValue *)const = 0;	//2122
	virtual char const *  GetPropName(int,int)const = 0;	//2187
	virtual int  GetPropVal(int,CValue &)const = 0;	//2221
	virtual long  GetTypeID(void)const = 0;	//2505
	virtual char const *  GetTypeString(void)const = 0;	//2513
	virtual CType   GetValueType(void)const = 0;	//2547
	virtual int  HasRetVal(int)const = 0;	//2657
	virtual void  IncrRef(void) = 0;	//2668
	// virtual void  InitObject(CType const &);	//2721 // artbear �� ����� - ��� ����� ����� ����� ������ �� �������� ������
	virtual void  InitObject(char const *) = 0;	//2722
	virtual int  IsExactValue(void)const = 0;	//2808
	virtual int  IsOleContext(void)const = 0;	//2846
	virtual int  IsPropReadable(int)const = 0;	//2863
	virtual int  IsPropWritable(int)const = 0;	//2865
	virtual int  IsSerializable(void) = 0;	//2874
	virtual int  SaveToString(CString &) = 0;	//3295
	virtual void  SelectByID(class CObjID,long) = 0;	//3350
	virtual int  SetPropVal(int,CValue const &) = 0;	//3694
	
	// ����� ��� ������ ����������������� ������������ � ����������� �����������
	// ������������ � ������ ���������������::�����()
	virtual void CallUserConstructor(const CString&  strConstructorName, int nParamsCount, CValue **ppValue) const = 0;
};

// ����� ��� ������ � ������������ 1�
class CRuntimeExceptionClass
{
private:
	CRuntimeExceptionClass() {}; // ����� ������ �������
	
	static bool isExceptionStatus;
	static CString strTextException;
	static CValue* ExceptionValue;
	
	static CSafeModulePtr pMod;
	static int iRuntimeErrLineNum;
	static CString m_strModulePath;
	static DWORD m_nID; // -1 ��� 0
	static bool m_bStatusOfTryingBlock; // ����, ��������� �� � ����� �������-����������
public:
  static void Init(void);
  static void Destroy(void);
  
  static void Empty(void);

  // ��������� ���������� � ������ � ���
  static void SetException(const CString& strExceptionA);
  static void SetException(const CString& strExceptionA, CBLModule7* pModA, 
	  DWORD m_nIDA, const CString& m_strModulePathA, int iRuntimeErrLineNumA);

  // ��������� ���������� ��� ���������������� ������� 1C++
  static void ThrowException(bool isThrow = true);
  static void ThrowException(const CString& strException);
  static void IfNeedExceptionThenThrowException(void);

  // ��������� ����������, �������� ���������� ������. ��������� �� ������ ����� ������
  static void RuntimeError(const CValue& param);
  // ��������� ����������, �������� ���������� ������. ��������� �� ������ ����� ����� ����������
  static void RuntimeError(const CValue& param, const CString& strTextException);
  // �������� ����� ����������� ������-����������
  static void GetException(class CValue & rValue);

  // �������� ����, ��������� �� � ����� �������-����������
  static void SaveStatusOfTryingBlock(void);
  static bool GetStatusOfTryingBlock(void);

  // ���������, �� ��������� �� �� � ����� �������-����������
  static bool IsInsideTryingBlock(void);

};

class CDynamicPropertyManager;

// ����� �++ ��� ������ � ����������������� �������� (���)
class CComponentClass : public IUserClassContext //CBLContext
{
friend class CImplicitParams;

DECLARE_DYNCREATE(CComponentClass);

public:

	CComponentClass();
	virtual ~CComponentClass();

	BOOL SendMessageOwnMod( CValue &rValue, CValue ** ppValue );
	void GetPathName( CValue &rValue );
	void GetEnvContext( CValue &rValue );
	void GetParamsList( CValue &rValue, CValue ** ppValue );
	BOOL SetOnIndexParams( CValue &rValue, CValue * * ppValue );
	void ThrowException(CValue ** ppValue);
	void AddDynamicProperty(CValue ** ppValue);
	int ReplaceInstBaseClasses(const CValue &NameClass, const CValue &Instance, CValue &rValue);
	int IsObject(const CValue &NameClass, CValue &rValue) const;

	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//935
	virtual int  CallAsProc(int,class CValue * *);	//937
	virtual void  DecrRef(void);	//1086
	virtual int  FindMethod(char const *)const;	//1366
	virtual int  FindProp(char const *)const;	//1369
	virtual char const *  GetCode(void)const;	//1544
	//virtual int  GetDestroyUnRefd(void)const;	//1657
	//virtual void  GetExactValue(class CValue &);	//1708
	virtual class CObjID   GetID(void)const;	//1804
	virtual class CBLContextInternalData *  GetInternalData(void);	//1826
	virtual char const *  GetMethodName(int,int)const;	//1964
	virtual int  GetNMethods(void)const;	//2005
	virtual int  GetNParams(int)const;	//2008
	virtual int  GetNProps(void)const;	//2015
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//2122
	virtual char const *  GetPropName(int,int)const;	//2187
	virtual int  GetPropVal(int,class CValue &)const;	//2221
	virtual long  GetTypeID(void)const;	//2505
	virtual char const *  GetTypeString(void)const;	//2513
	virtual class CType   GetValueType(void)const;	//2547
	virtual int  HasRetVal(int)const;	//2657
	virtual void  IncrRef(void);	//2668
	// virtual void  InitObject(class CType const &);	//2721 // artbear �� ����� - ��� ����� ����� ����� ������ �� �������� ������
	virtual void  InitObject(char const *);	//2722
	virtual int  IsExactValue(void)const;	//2808
	virtual int  IsOleContext(void)const;	//2846
	virtual int  IsPropReadable(int)const;	//2863
	virtual int  IsPropWritable(int)const;	//2865
	virtual int  IsSerializable(void);	//2874
	virtual int  SaveToString(class CString &);	//3295
	virtual void  SelectByID(class CObjID,long);	//3350
	virtual int  SetPropVal(int,class CValue const &);	//3694

	int GetBaseClass(const CValue &NameClass, CValue & rValue) const;
	BOOL GetBaseClass(const CRuntimeClass* pRTClass, CBLContext** ppBaseObj) const;
    
// 	template <class T>
//     BOOL GetBaseClass(T** ppT) const
//     {
//         for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter)
//         {
// 			CBLContext *pCont = *iter;
//             if ((pCont)->GetRuntimeClass() == RUNTIME_CLASS(CComponentClass))
//             {
//                 if (static_cast<CComponentClass*>(pCont)->GetBaseClass(ppT))
//                     return TRUE;
//             }
//             else
//             {
//                 T* p = dynamic_cast<T*>(pCont);
//                 if (p)
//                 {
//                     *ppT = p;
//                     return TRUE;
//                 }
//             }
//         }
//         return FALSE;
//     };
	
	template <class T>
		BOOL GetBaseClass(T** ppT) const
	{
		return !CastContext::Dynamic(const_cast<CComponentClass*>(this), *ppT) ? FALSE :  TRUE;
	};
	
	// IContextTree
	
	// �������� ���������� ������� ��������.
	virtual DWORD GetBaseObjCount();
	// �������� ��� �������� ������� �� �������.
	virtual CString GetBaseObjName(DWORD idx);
	// �������� ������� ������ �� �������.
	virtual CBLContext* GetBaseObj(DWORD idx);
	// ��� upcast'�
	virtual CBLContext* GetDerivedObj();
	
	// end IContextTree

	static CComponentClass* GetComponentClassCurExModule(CBLModule* pModWrapper = NULL);

	static bool StillInitInstanceComp() {return pCreatedCompModule != NULL;}

	static void ClearOptimizationData(void);

	// ����� ��� ������ ����������������� ������������ � ����������� �����������
	// ������������ � ������ ���������������::�����()
	void CallUserConstructor(const CString&  strConstructorName, int nParamsCount, CValue **ppValue) const;

	CParamDefsWithRefCountPtr GetDefFnNamesPtr(void) const
	{ return defFnNames; }
  
	CMapStringToPtr& GetBindInstanceValueList() const
	{ return m_BindInstaceValueList; }

	// IGetModule

	CBLModule * GetModule() const { return m_pMod; };
	CBLModule * GetHostModule() const { return m_pCurModule; };

	// end IGetModule

	// ���� �������� ��� ������ ���������� ������������, ���� ���� �� ���� � ������
	// ������ ��� ������������� � ������� ���������������::�����()
	//
	static void SetFlagCreateWithoutConstructor(void) { bFlagCreateWithoutConstructor = true; }
	static void ClearFlagCreateWithoutConstructor(void) { bFlagCreateWithoutConstructor = false; }
	static bool IsCreateWithoutConstructor() { return bFlagCreateWithoutConstructor; }

private:

	enum {
	  funcGetBaseClass,
	  procAssignFriendModule,

	  // ���� ���������� ������ ����� �������� ������������� 1�++ - artbear
	  // TODO ��������, � ���������� �� ������ ���� ������ ������ ������, � ������� �������
	  //
	  //procAddBaseClass,

	  funcSendMessageOwnMod,
	  funcGetPathName,

	  funcGetEnvContext,
	  funcGetParamsList,

	  funcReplaceInstBaseClasses,
	  funcReplaceInstBaseClasses_SYN1,

	  funcSetOnIndexParams,
	  procOnOpen,
	  procThrow,
	  funcIsObject,
	  procAddDynamicProperty,
	

	  lastMethod
	};

	static void GetFullTypeName(CBLContext *pCont, CString& rus, CString& eng, bool bAddKind = true);
	static void GetFullTypeName(CBLContext *pCont, CString& rus, CString& eng,
		CString& rusKind, bool bAddKind = true); // artbear

	//inline int ReplaceInstBaseClasses(const CValue &NameClass, const CValue &Instance, CValue &rValue);

	//inline int IsObject(const CValue &NameClass, CValue &rValue) const;

	void InvokeConstructor();

	virtual CBLModule* Init(const char* file);
	virtual CComponentClassModulePtr FirstInit(const CString& FullFileName);
	
	static bool bFlagCreateWithoutConstructor;

	CSafeModulePtr m_pCurModule; // ������, � ������� ������ ��� ������

	// ����� �������, ������������ ��� �����������
	static class CMapDefMethod MapDef;

	// ���������, �������� �� ������� ����������� ������ ������� �������� ������ ��� �������� ������
	// ������������ ��� ���������� ��������
	CComponentClass* VerifyCurrentExecuteContextIsBaseClassForThisClass() const;

	// ��������� ������������� ���������� ������ �� ������ ���������� ����������
	// �� ����� ������������� �������������
	void InitInstanceComp(void);

	friend class CInitGuard;
	void OnStartInit(void);
	void OnEndInit(bool bIsBadExit = false);
	void FinishInit(void);

	bool m_bDonNotCallDestructor;
	void SetFlagDonNotCallDestructor() { m_bDonNotCallDestructor = true; };

	CSafeModulePtr m_pMod;
	mutable CMapStringToPtr   m_BindInstaceValueList;

	static class CObjID ObjID;

	bool AddBaseClass(const CString& name);
	void ClearAllBasesClass();
	
	void ReplaceContextInModulesOfBaseClasses(CComponentClass* pMainCont =  NULL);

  // ����� ����� ������ �� ��������
  // ������� ������ � ������ ������ �� ������, ���� ����� ��� �������
  // � ����� � �������������� ������� ������, ����� � �����������
  //
  // ������� ������ ������� ���, � ����� ����������
  //
  static int FindMethod(CBLContext* pContext, CBLModule *pMod, LPCTSTR sNameRus, LPCTSTR sNameEng);

  // ���� �����, �� ��� �������� ������
  inline int FindMethod(CBLModule *pMod, LPCTSTR sNameRus, LPCTSTR sNameEng) const
  {
	return CComponentClass::FindMethod((CBLContext*)this, pMod, sNameRus, sNameEng);
  };

  BOOL GetParamDefValueFromClassDefinitionOrModule(int iMethodNum,int iParamNum,class CValue * pDefValue)const;
  BOOL GetParamDefValueFromClassModule(int iMethodNum,int iParamNum,class CValue * pDefValue)const;


	CString m_NameOfInstance;

  vector<CSafeContextPtr<CBLContext> > vecOfBaseCont;
  typedef vector<CSafeContextPtr<CBLContext> >::const_iterator CONST_ITER_CONT;
  typedef vector<CSafeContextPtr<CBLContext> >::iterator ITER_CONT;

  CParamDefsWithRefCountPtr defFnNames;
  struct MethParamsDef
  {
    int nNumberOfMeth;
    int nRealNumberOfMeth;
    CBLContext* pContext;
  };
  mutable MethParamsDef m_MethParamsDef;

  int CallAsMethod(int iMethNum, CValue * *ppValue, bool bHasRetVal = false, CValue * pRetValue = NULL);

  bool m_isCreateOptimizeMode;

  // ����� � ����������� �� ������ � �� ��������� �����������
  // ���� - ��������� �� ������, �������� - ��������� �� �������� CComponentClass
  static CMapPtrToPtr m_MapOfModulesAndComp;

	// ��������� �� ������� ����������� ������� ���
	// ��������� ��� ��������� ��� ��� ��������� �� ���� ������� ��������
	static CComponentClass* pCreatedCompModule;

	// ������ � ��������
	class CDebug
	{
	public:
		static void SetDebugObject(CComponentClass *pComp);
		static void ClearDebugObject();

		bool IsDebug() const
		{ return NULL != pDebugContext; }

		operator bool() const
		{ return IsDebug(); }

		bool operator !() const
		{ return !IsDebug(); }

		void Release(void);

		void OpenDebugForm(CComponentClass* pComponent);

		//void AssignDebugModule(CBLContext* pComponent, CBLModule*& pMod);
		void AssignDebugModule(CBLContext* pComponent, CSafeModulePtr& pMod);
	private:
 		CSafeContextPtr<CBLContext> pDebugContext;

		static CComponentClass* m_CurDebugObject;

	};

	CDebug m_DebugInfo;

	  // ��������� ��������� �� ���������� ������� � ����������� �� ������ �������� ��������� 
	  // ���������� ������ ����� � ������ (...)
	  struct InfoUnlimitsParams
	  {
		CBLContext* cont_ValueList;
		vector<CValue*> vecPointOfPars;
		int         nNumAllParams;
		int         nNumExplicitParams;
	  };

	// CComponentClass::CImplicitParams - ������ � �������������� ������ ����������
	class CImplicitParams
	{
	private:
	public:

		CString m_strClassAndMethImplicit;
		CString m_strClassAndMethCurrent;
		int m_CountParametersInCallMethod;
		
	public:
		CImplicitParams();

	  CBLContext* GetImplicitParamsList(CComponentClass *pComp, const char* lpcsNameMeth);

	  bool GetImplicitParams(CComponentClass *pComp, CValue **ppPar, int nNumMeth);
	  int SetImplicitParamOfInd(CComponentClass *pComp, const char* lpcsNameMeth, int nIndex, CValue* pVal);

	  int GetNParams(const CComponentClass *pComp, int nNumMeth, int nNParams);

	  // ��������������� ��������� ������� ��������� � ����� �� �������� ����� ������
	  void RestoreValParam(CComponentClass *pComponentClass, int nNumMeth, CValue **ppParam, ARRAY_VAL *arrValOfMeth);
	  // ��������� ���������, ������� ��������� � ����� �� �������� ����� �������
	  bool SaveValParam(CComponentClass* pComponentClass, int nNumMeth, CValue** ppParam, ARRAY_VAL *arrValOfMeth);

	private:
	  BOOL LookupInBindInstanceValueList(const CComponentClass* pComp, const CString& m_strClassAndMethCurrent, InfoUnlimitsParams *&pInfo) const;
	  //InfoUnlimitsParams * 
	  void* GetImplicitParamsInfo(CComponentClass *pComp, const char* lpcsNameMeth) const;
	};

	static CImplicitParams* pImplicitParams;

	bool IsMethodFromBaseContexts(const int iMethodNum) const { return iMethodNum >= defFnNames->Size(); };
	CSafeContextPtr<CBLContext> FindBaseContextByMethodNumber(const int iMethNum, int& iMethNumInSearchedContext) const;

	boost::shared_ptr<CDynamicPropertyManager> m_pDynamicPropertyManager;

	bool VerifyMethodForParamsByValueAndSetMetaInfoClassesDataForParamsByValue( const CString& strNamesClassAndMeth, const CBLProcInfo &procInfo);
};

#endif // !defined(AFX_COMPONENTCLASS_H__5F1978A2_67C1_4AAB_8C12_44790D3700B2__INCLUDED_)
