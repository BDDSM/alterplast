// MetaDataOfClasses.h: interface for the CMetaDataOfClasses class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METADATAOFCLASSES_H__F1BE676F_903A_4D46_99F1_123B72722A0A__INCLUDED_)
#define AFX_METADATAOFCLASSES_H__F1BE676F_903A_4D46_99F1_123B72722A0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "SetOfHierarchy.h"
//#include "Preprocessor.h"
#include "istrmap.h"
#include "paramdefs.h"

//#include "system\\GlobalMethods.h"
void RuntimeError(const char *msg, ...);
void RemoveNeedlessBlank(CString &str);

//class CRangesOfDescr;

struct CUserClassMetaData
{
	//typedef boost::shared_ptr<CStringList> CStringList_SharedPtr;
	typedef std::vector<CString> CStringVector;
	typedef CStringVector::iterator CStringVectorIterator;
	typedef CStringVector::const_iterator CStringVectorConstIterator;
	typedef boost::shared_ptr<CStringVector> CStringList_SharedPtr;

	typedef CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData CPtrToMap;

	CString m_FileName;
	CStringList_SharedPtr m_BaseClassesList;
	CPtrToMap m_MethodsMap;
	CString m_FullFileNameOfDescrFile;
};

class CMetaDataOfClasses  
{
	//friend class ISetOfHierarchy;
	friend class CSetOfHierarchyImpl;
public:
	CMetaDataOfClasses();

public:
  // ������ ����� �������� ������� 1� (��� ����������� ������������ 
  // �������������(<���������>) � ������ � ������� ���������� ���������� ������ �������
	CIStringMapToString m_ClassBindFile;

  // ���� ����� ��� ����������� ����������� �������
	CIStringMapToString m_DynamicAddedClassBindFile;
	bool m_bInsideDynamicAdd;
	
  // ������ ������ �������� ������� � ��������� � ��� ������ �������� ������� �������
	// TODO ����� ����� ���������� �� vector ��� list
	//typedef boost::shared_ptr<CStringList> CStringList_SharedPtr;
	typedef CUserClassMetaData::CStringList_SharedPtr CStringList_SharedPtr;
	CIStringMap<CStringList_SharedPtr, CStringList_SharedPtr&> m_ClassesHierarchyMap; //CIStringMap<CStringList*, CStringList*> m_ClassesHierarchyMap;
  
  // �������� �������� ��������� �� ����� ini ����������
  CIStringMapToString m_AliasOfPaths;

  // �������� ���� ����� ������ � ������� ������� � �� ����������
  //typedef CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData CPtrToMap;
  typedef CUserClassMetaData::CPtrToMap CPtrToMap;
  CIStringMap<CPtrToMap, CPtrToMap&> m_MapClassNamesToMethodsMap;

  bool ThisIsUnLimMethod(const CString& strName) const;
  bool LookupUnLimMethod(const CString& strName, CParamDefs::CArrayOfMethodsParamData_PTR& param) const;

  void AddMethHaveParVal(const CString& strKeyOfUserClassMethod, CParamDefs::CArrayOfMethodsParamData_PTR param);
  bool LookupMethHaveParVal(const CString& strKeyOfUserClassMethod, CParamDefs::CArrayOfMethodsParamData_PTR& param) const;
  bool isMethHaveParVal(const CString& strKeyOfUserClassMethod) const 
	{ CParamDefs::CArrayOfMethodsParamData_PTR param; return LookupMethHaveParVal(strKeyOfUserClassMethod, param); }

  bool IsClassExist(const char *pcstrName) const;
  int GetCountClasses() const;
  int GetCountMethods() const;

  CString GetAllClassesNames(const char* sep = "\r\n") const;
  CString GetAllAliasAndPath(const char* sep = "\r\n") const;
  CString GetBaseClasses(const char* strNameCl, const char* sep = ",") const;
  CString GetNamesMethOfClass(const char *pNameClass, const char *sep = ",") const;

  CParamDefs::CArrayOfMethodsParamData_PTR GetInfoParamMeth(const char *pNameClass, const char *pNameMeth) const;
  bool IsMethExist(const char *pNameClass, const char *pNameMeth) const;

  CString GetClassNameFromDefinition(const CString& ClassName) const;
  CString GetPathFromAlias(const CString& strAls) const;

  // artbear ��� �������� �������� // 1 - ��������, 0 - ���
  bool VerifyRecursiveInBaseClasses(CString ClassName, CString BaseClassName, CString& sRecursiveHierarchy) const;

  CString GetNameOfFile(const CString &strNameOfMod) const;
  
  bool TheClassIsDerive(const CString& derive, const CString& base) const;
  bool IsObject(const CString& sClassName, const CString& sTryName) const;

  bool IsFunction(const CString& strKeyOfUserClassMethod) const;
private:

  // ���� - ��� ������ ���� ��� ������ � ������� ������������ �������� ������������ �� ��������
	CParamDefs::CMapMethodsNameToArrayOfMethodsParamData m_ValParamInMeth; //CIStringMapToOb m_ValParamInMeth;

  // ���� - ��� ������ ���� ��� ������ � ������� ������������ �������������� ���������� ���������� (...)
  CParamDefs::CMapMethodsNameToArrayOfMethodsParamData m_UnlimitsParams;//CIStringMapToOb m_UnlimitsParams;
  
  // �������� ������ ���� � ����� �������� ��� ������
  CIStringMapToString m_FullFileNameOfDescrFile;

  CIStringMapToString m_IsFunctionMap;
};

#endif // !defined(AFX_METADATAOFCLASSES_H__F1BE676F_903A_4D46_99F1_123B72722A0A__INCLUDED_)
