// MyADO.h: interface for the CMyADO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYADO_H__1F860084_0FEB_4E85_80CF_54E0EAD0AFA9__INCLUDED_)
#define AFX_MYADO_H__1F860084_0FEB_4E85_80CF_54E0EAD0AFA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct strSubs_ {
	int m_ID;		// ������������� ����������
	int m_Check;	// ����������
	int m_IDParent; // ������������� �������� ����������
	CString m_Name; // ��� ���������� 
} SubsStorage;

typedef struct strSubsGetSettings {
	int		EnableDDGroup;
	int		EnableFullPathDD;	// ������������ ����� �������������� �������� ����...
	int		ShowColComm;
	int		ShowColID;
	int		ShowColType;
	int		ShowSubPanePos;		// ���������� ������ ��������� 0-������, 1-�����, 2-�����, 3-������.
	int		Show_ID;			// ���������� ID - ������� � ������

	CString strCharsTerminator; 
} SSubsGenSettings;

typedef struct strSubsDDSettings {
	bool	bCtrl;		// ����� Ctrl
	bool	bShift;		// ����� Shift
	int		iTypeMDObj; // ��� �������
	CString	strBefore;	// ��������� �����
	CString strAfter;	// ��������� �����
} SubsDDSettings;



class CMyADO
{
//	friend class CXBaseContext;
public:
	CMyADO();
	virtual ~CMyADO();

public:
	int m_StateSubs;	// ��������� ������ (0-�������, 1-����������)
	int m_iVersion;		// ������ �������������� ������
	
	CMapStringToString m_ObjToSub;	// � ��������� � ������� ��������
	CMapStringToOb m_SubSelfs;		// ���� ����������.
	CMapStringToString m_SubToSub;	// ��������� ���������� ��� �������.....

	// ���������� m_ObjToSub � m_SubSelfs
	CString m_sIBDir;
	CString m_sBinDirOC;
	CString m_sFileDataName;
	CString m_sFileSettingsName;
// ���������
	SSubsGenSettings m_Settings;

public:
	void ActivateStandartView();
	void SaveOrLoadSettings (bool bSave = true);
	void SaveOrLoadSubsSelection (bool bSave = true);
	void BuildMarkUpSubs();
	bool IsStoredObj(long nWhatIsIt);
	int m_LastIDSubs;
	bool SubsDelete (class CListBox& nListBox);
	bool SubsRename (class CListBox& nListBox);
	void FillListSubs (class CListBox& nListBox);
	bool SaveOrLoad (bool bSave, bool bClearAll = true);
	bool Init();
	bool SubsInsertNew( int nID );
	bool LineHaveMarkUpSubs (CString sSubsFromObj);
};

#endif // !defined(AFX_MYADO_H__1F860084_0FEB_4E85_80CF_54E0EAD0AFA9__INCLUDED_)
