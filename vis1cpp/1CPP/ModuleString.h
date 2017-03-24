// ModuleString.h: interface for the CModuleString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODULESTRING_H__44088562_A489_4581_B7AC_D31694735690__INCLUDED_)
#define AFX_MODULESTRING_H__44088562_A489_4581_B7AC_D31694735690__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CModuleString : public CString  
{
public:
	CModuleString(){}
	
	CModuleString(const CString& stringSrc):CString(stringSrc) {}

	int getOffsetLines() const;

	// ������ ����� �������, ��������� � ���� ����� � ������� �������� //# ClassBegin � //# ClassEnd
	// ���������� �������� � ������� ����� � ������� ���� �� ������
	// �������, ����� //# ������ � ������ ������ � ���� ������� ��� ������
	//
	//#  ClassBegin  ����������������������������������������
	//#   ClassEnd  ����������������������������������������
	//
	bool DivideTextOnClasses(CMapStringToString* pMap);
	
	// ������ �������� ��� ������ �� ������
	//
	// ���� ���� ��������� //# ClassBegin ��������� � //# ClassEnd ���������, 
	//	�������� ��� ������ ���� ������
	// ����� �������� ��� ����� ������
	//
	void CropModuleToClass(const CString& strClassName);

	bool LoadModule(const CString& strFileName);
	bool LoadModule(const CString& strFileName, const CString& strClassName);

private:
	int m_OffsetLines;
	
	int DeleteWithLineCountTo(int posBegin);

	bool LoadModuleFromERT(const CString& strFileName);
	bool LoadModuleFromMD(const CString& strFileName);
	bool LoadModuleFromFile(const CString& strFileName);
	bool LoadModuleFromDLL(const CString& strFileName);
};

#endif // !defined(AFX_MODULESTRING_H__44088562_A489_4581_B7AC_D31694735690__INCLUDED_)
