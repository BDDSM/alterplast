//////////////////////////////////////////////////////////////////////
//
//	System.h
//	Interface for CSystem class (debug & troubleshooting methods)
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../mycontextbase.h"
#include "../MethodsWrapper.h"

#ifdef _DEBUG_PURE
#include "pure.h"
#endif

// PSDK definitions
#define BELOW_NORMAL_PRIORITY_CLASS       0x00004000
#define ABOVE_NORMAL_PRIORITY_CLASS       0x00008000

#define nPriorityClassCnt sizeof(dwPriorityClasses) /sizeof(DWORD)

class CSerialize;

// ����� ��������� �������
class CSystem : public CMyContextBase
{
public:
	DECLARE_DYNCREATE(CSystem);

//	CSystem() {};
//	~CSystem() {};

	// ��������� �������
	LONG crtwalk(bool bShowMsg);
	LONG heapwalk(HANDLE hHeap, bool bShowMsg);

	CGetDoc7* sub_GetDoc7(CValue *cv);
	int CSystem::sub_LookupBatch(CGetDoc7* pDoc);

	// ��������� ������� ������� ______________________________________________
	BOOL funcMemInfo(CValue& RetVal, CValue **params);
	BOOL funcPriority(CValue& RetVal, CValue **params);
	BOOL funcPurify(CValue& RetVal, CValue **params);
	BOOL funcTest(CValue& RetVal, CValue **params);
	BOOL defsMemInfo(int nParam, CValue* param) const;

	BOOL funcGetOlePic(CValue& RetVal, CValue **params);
	BOOL defsGetOlePic(int nParam, CValue* param) const;
	BOOL procSavePic(CValue **ppValue);
	BOOL defsSavePic(int nParam, CValue* param) const;

	static DWORD dwPriorityClasses[];

	// ������ ���������� ����� ________________________________________________
	// ���������� ����� ����� � ��������� ���������
	BOOL funcEnableNewLine(CValue& RetVal, CValue **params);

	// ����� /���� (����������� �������)
	BOOL funcStartBatch(CValue& RetVal, CValue **params);
	BOOL funcEndBatch(CValue& RetVal, CValue **params);
	BOOL defsEndBatch(int nParam, CValue* param) const;

	// ��������� ��������� ��������� �� ������ ��������� ������� (�������� �������)
	BOOL funcEnableActivation(CValue& RetVal, CValue **params);

	// ������ ��������� �� ������ ��������� �������
	BOOL funcActivate(CValue& RetVal, CValue **params);

	// �������� ��������� ������ 1� (����� �� FormEx)
	BOOL  funcCommandLine(CValue& RetVal, CValue **params);
	// �������� ���� � ���������� 1�
	BOOL  funcGetAppPath(CValue& RetVal, CValue **params);

	// ����������� �������� �����, �� � ���������� ���� ���������� �������
	BOOL funcCloseForm(CValue& RetVal, CValue **ppValue);
	BOOL defsCloseForm(int nParam, CValue* param) const;

	// �������������� ������� ���������� � HKCU
	BOOL procRegisterAddinsInHKCU(CValue **ppValue);
	BOOL defsRegisterAddinsInHKCU(int nParam, CValue* param) const;

	//// �������� ������-������� �� ������
	//BOOL funcResString(CValue& RetVal, CValue **ppValue);

	// ��������� Sleep
	BOOL procSleep(CValue **ppValue);

	BOOL RefreshGroupContextData(CValue **ppValue);
	
	BOOL funcSerialize(CValue& RetVal, CValue **ppValue);

private:

	void ValidateParamGroupContext(const CValue& GroupContValue) const;
	void ValidateParamNumeric(const CValue& NumericValue) const;

public:

private:
	// ���������
	typedef void (CGetDoc7::*PF_CGetDoc7_OnEnter)();
	static CDllMethodWrapper<PF_CGetDoc7_OnEnter> w_CGetDoc7_OnEnter;

	typedef void (CDocForm::*PF_CDocForm_OnEnter)();
	static CDllMethodWrapper<PF_CDocForm_OnEnter> w_CDocForm_OnEnter;

	typedef int (CEditGet::*PF_CEditGet_Activate)();
	static CDllMethodWrapper<PF_CEditGet_Activate> w_CEditGet_Activate;

	// ����������� ����������
	void Wrap_CGetDoc7_OnEnter();
	void Wrap_CDocForm_OnEnter();
	int Wrap_CEditGet_Activate();

	// ����� ��������������� ����������� ������� ������� ��� ���������
	static CString m_strActivationRequested;
	static BOOL m_bActivationRequested;

	// ����� CGetDoc7 -> nRefCount
	static CMapPtrToPtr m_BatchGuard_map;

	boost::shared_ptr<CSerialize> m_Serialize;

	DECLARE_MY_CONTEXT();
};

// ����� ��� ������ ������������������ ����������
// �������������:
// Start();
// ... ����������� �������� ...
// Stop();
// Report(__���������);

class CTSC
{
public:
	CTSC( LPSTR pMsg = NULL ) : m_pMsg(pMsg) { Start(); }
	void Start() { m_result = 0; m_tsc = ReadTSC(); }
	void Stop() { m_result = ReadTSC() - m_tsc; }
	void Report(LPCSTR pMsg = NULL)
	{
		if ( !pMsg )
			pMsg = m_pMsg;
		if ( pMsg )
			Msg("%s: 0x%012I64X", pMsg, m_result);
		else
			Msg("0x%012I64X", m_result);
	}
	UINT64 Get() { return m_result; };
protected:
	UINT64 ReadTSC();

private:
	UINT64 m_tsc;
	UINT64 m_result;
	LPCSTR m_pMsg;
};

__declspec(naked)
UINT64 CTSC::ReadTSC()
{
	_asm
	{
		rdtsc;
		ret;
	}
};
