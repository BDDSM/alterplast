// ctxtree.h
#pragma once

// ��������� ���������� ��� ������ ����������� ��������� ��������.
// ��� �����, ����������� CBLContext, ������ ����������� � ���� ���������.
struct IContextTree
{
	// �������� ���������� ������� ��������.
	virtual DWORD GetBaseObjCount() = 0;
	// �������� ��� �������� ������� �� �������.
	virtual CString GetBaseObjName(DWORD idx) = 0;
	// �������� ������� ������ �� �������.
	virtual CBLContext* GetBaseObj(DWORD idx) = 0;
	// ��� upcast'�
	virtual CBLContext* GetDerivedObj() = 0;
};

// ����� ��������� ������� ���������� ����������
struct CastContext
{
	// ����� ���������� ������ ������ � ��������� ����� ������������ ���������
	template<typename Op>
	static CBLContext* DoCast(CBLContext* pCont, Op& op)
	{
		if(op.CheckContext(pCont))
			return pCont;
		IContextTree* pTree;
		try{
			if(!(pTree = dynamic_cast<IContextTree*>(pCont)))
				return NULL;
		}catch(...){ return NULL; }
		
		for(DWORD idx = 0, nBases = pTree->GetBaseObjCount(); idx < nBases ; idx++)
		{
			CBLContext* pBaseObj = pTree->GetBaseObj(idx);
			if(op.CheckBase(pTree, idx, pBaseObj))
				return pBaseObj;
			if(pBaseObj = DoCast(pBaseObj, op))
				return pBaseObj;
		}
		return NULL;
	}
	// ������ �������
	struct by_empty 
	{
		BOOL CheckContext(CBLContext* pCont) {return FALSE; }
		BOOL CheckBase(IContextTree* pTree, DWORD idx, CBLContext* pCont) { return FALSE; }
	};
	// ������� ��� ������ ��������� �� CRunTimeClass
	struct by_rtc : by_empty
	{
		const CRuntimeClass* pRTC;
		by_rtc(const CRuntimeClass* p) : pRTC(p){}
		BOOL CheckContext(CBLContext* pCont) {return pCont->GetRuntimeClass()->IsDerivedFrom(pRTC); }
	};
	// ������� ��� ������ ��������� �� �������� ������ � CRunTimeClass
	struct by_rtc_name : by_empty
	{
		const CString& strName;
		by_rtc_name(const CString& s) : strName(s){}
		BOOL CheckContext(CBLContext* pCont) {return 0 == strName.CompareNoCase(pCont->GetRuntimeClass()->m_lpszClassName); }
	};
	// ������� ��� ������ ��������� �� �������� �������� ������
	// ��������: ����� �������������� ����� � ������� �������, ��� ����� �� ��� �� �����������
	struct by_base_name : by_empty
	{
		const CString& strName;
		by_base_name(const CString& s) : strName(s){}
		BOOL CheckBase(IContextTree* pTree, DWORD idx, CBLContext* pCont)
			{ return 0 == strName.CompareNoCase(pTree->GetBaseObjName(idx)); }
	};
	// ������� ��� ������ ���������, ������������ ���������, ��������� ����� �����������
	template<typename T>
	struct by_dynamic_cast : by_empty
	{
		T*& pResult;
		by_dynamic_cast(T*& p) : pResult(p){}
		BOOL CheckContext(CBLContext* pCont)
		{
			try{
				pResult = dynamic_cast<T*>(pCont);
			}catch(...){ return FALSE; }
			return pResult != NULL;
		}
	};

	// ����������, ������ ����������
	
	// Upcast - �������� �������� �����.
	static CBLContext* Up(CBLContext* pCont)
	{
		IContextTree* pTree;
		for(;;)
		{
			try{
				if(!(pTree = dynamic_cast<IContextTree*>(pCont)))
					break;
			}catch(...){ break; }
			CBLContext* pDerived = pTree->GetDerivedObj();
			if(!pDerived)
				break;
			pCont = pDerived;
		}
		return pCont;
	}

	// ���������� �� CRunTimeClass'�
	static CBLContext* ByRTC(CBLContext* pCont, const CRuntimeClass* pRTC)
	{
		return DoCast(pCont, by_rtc(pRTC));
	}
	// ���������� �� �������� � CRunTimeClass'�
	static CBLContext* ByRTCName(CBLContext* pCont, const CString& strName)
	{
		return DoCast(pCont, by_rtc_name(strName));
	}
	// ���������� �� ����� �������� ������
	static CBLContext* ByBaseName(CBLContext* pCont, const CString& strName)
	{
		return DoCast(pCont, by_base_name(strName));
	}
	// ���������� ����� dynamic_cast � ��������� ����
	template<typename T>
	static CBLContext* Dynamic(CBLContext* pCont, T*& pResult)
	{
		return DoCast(pCont, by_dynamic_cast<T>(pResult));
	}
};
