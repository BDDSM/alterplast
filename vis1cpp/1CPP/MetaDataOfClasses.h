// MetaDataOfClasses.h: interface for the CMetaDataOfClasses class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METADATAOFCLASSES_H__F1BE676F_903A_4D46_99F1_123B72722A0A__INCLUDED_)
#define AFX_METADATAOFCLASSES_H__F1BE676F_903A_4D46_99F1_123B72722A0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "SetOfHierarchy.h"
#include "Preprocessor.h"

#include <boost\shared_ptr.hpp>

class CRangesOfDescr;

struct NameBindOfType {
    CString name;
    CString type;
    CString value;
    bool bIsVal;
    NameBindOfType (const CString& n, const CString& t, const CString& v = CString(""), bool bIs = false) : name(n), type(t), value(v), bIsVal(bIs) {}
};

struct CMetaDataOfClasses  
{

	CMetaDataOfClasses();
	~CMetaDataOfClasses();
  static void RemoveNeedlessBlank(CString &str);

  // ����� ��������� ������ ������ � ��������� ���������� ��������� ������. 
  // ������ ������ �������� �� ���������� ������� � ��������� � ������ ������ 
  // �������� ���������� � ������������� ��������� ���� �������	
  void ParsingString(CString& StringForParse, CRangesOfDescr& RangesDescr);
  // ������ ����� �������� ������� 1� (��� ����������� ������������ 
  // �������������(<���������>) � ������ � ������� ���������� ���������� ������ �������
	CMapStringToString m_ClassBindFile;
	
  // ������ ������ �������� ������� � ��������� � ��� ������ �������� ������� �������
	typedef boost::shared_ptr<CStringList> CBaseClassesListPtr;
	typedef CIStringMap<CBaseClassesListPtr, CBaseClassesListPtr> CBaseClassesMap;
	CBaseClassesMap m_ClassesHierarchyMap;
  
  // �������� �������� ��������� �� ����� ini ����������
  CMapStringToString m_AliasOfPaths;

  // �������� ���� ������� ������� ��� ������� ��������
  CMapStringToOb m_TypeOfClassMeth;  

  typedef CArray<NameBindOfType*, NameBindOfType*> ARR_TYPE_NAME;

  // ���� - ��� ������ ���� ��� ������ � ������� ������������ �������� ������������ �� ��������
  CMapStringToOb m_ValParamInMeth;

  // ���� - ��� ������ ���� ��� ������ � ������� ������������ �������������� ���������� ���������� (...)
  CMapStringToOb m_UnlimitsParams;
  
  // �������� ������ ���� � ����� �������� ��� ������
  CMapStringToString m_FullFileNameOfDescrFile;
  
  static CMetaDataOfClasses* m_oneMetadata;
  
  CMapStringToPtr m_SymbolOfPreprocessor;
  CMapStringToString m_LoadedIncludeFiles;
  CString pathBase1C;

  static void MakeFullFileName(CString* name, LPCSTR sSecondPath = NULL);

	// artbear ��� �������� ��������
	// 1 - ��������, 0 - ���
	bool VerifyRecursiveInBaseClasses(CString ClassName, CString BaseClassName, CString& sRecursiveHierarchy);

};

#endif // !defined(AFX_METADATAOFCLASSES_H__F1BE676F_903A_4D46_99F1_123B72722A0A__INCLUDED_)
