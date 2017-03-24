//______________________________________________________________________________
//
// �����-��������� �� CBLContext, ������������ ����� CValue
//
// ��� ����� ���������� �����, ������������� ����������� ����� ����������, 
//		� �.�. � ��������� ����������
//	����������� ����������� ������ � �������������� ������������� 
//
//	���������� �������� ��������� ����� - ����� ��������� �� �������� CBLPtr<T> (blptr.h)
//		�� �� �� ��� ��������� ��� ������������� ����������, ��������, ���������
// 
// artbear, 2006
//______________________________________________________________________________
//

#ifndef __SAFE__CONTEXT__INCLUDED
#define __SAFE__CONTEXT__INCLUDED

template <class TCont = CBLContext>
class CSafeContextPtr
{
// 	typedef CBLContext TCont;
	typedef TCont * TContPtr;
	typedef const TCont * ConstTContPtr;
private:

	CValue *m_pValue;
	
private:

	// ���������� ������
	void _Assign(ConstTContPtr pCont) // ��� ������������� Release (���� ��� ��������)
	{
		if (pCont)
		{
			m_pValue = new CValue;
			m_pValue->AssignContext(const_cast<TContPtr>(pCont));
			
		}
	};
	
	void Assign(ConstTContPtr pCont)
	{
		Release();
		_Assign(pCont);
	};

 	// ������������ ������
	void Release()
	{
		if (m_pValue)
		{
			delete m_pValue;
			
			m_pValue = NULL;
		}
	};

	// ��������� ���������� � ��������� ����������
	static void _ErrorNULLObject(LPCSTR szErrorString = NULL)
	{
		if (!szErrorString)
			szErrorString = "CSafeContextPtr : ������� ��������� � ��������� NULL"; // TODO ���������� ��� ��������� ������ ��� �������� ��������

		::RuntimeError("%s", szErrorString); // TODO � ���������� ����� ������� ������ ��� ����������

	};

public:

	// ��������������
	CSafeContextPtr() : m_pValue(NULL)
	{ };

	CSafeContextPtr(ConstTContPtr pCont) : m_pValue(NULL)
	{ 
		_Assign(pCont);
	};

	CSafeContextPtr(const CSafeContextPtr& ref) : m_pValue(NULL)
	{
		if (ref.m_pValue)
			_Assign(static_cast<ConstTContPtr>(ref.m_pValue->GetContext()));

	};

	// �������� ������� ��������� ����
	CSafeContextPtr(LPCSTR szName) : m_pValue(NULL)
	{
		Create(szName);
	};

	virtual ~CSafeContextPtr()
	{
		Release();
	};

	// �������� ��������� ����������
	inline bool IsGroupContext() const
	{
		return m_pValue && IS_KINDOF_RUNTIME_CLASS(m_pValue->GetContext(), CGroupContext );
	};
public:

	// ���������
	CSafeContextPtr& operator =(ConstTContPtr pCont)
	{
		if (operator TContPtr() != pCont)
			Assign(pCont);

		return *this;
	};
	CSafeContextPtr& operator=(const CSafeContextPtr& lp)
	{
		if (&lp == this)
			return *this;

		return operator=((TContPtr)lp);
	};

	operator bool() const
	{
		return (m_pValue && m_pValue->GetContext());
	};
	operator !() const
	{ 
		return !operator bool();
	};

	bool operator==(ConstTContPtr pT) const
	{
		return operator TContPtr() == pT;
	};
	bool operator!=(ConstTContPtr pT) const
	{
		return !operator ==(pT);
	};

	operator TContPtr() const
	{ 
		return !m_pValue ? NULL : static_cast<TContPtr>(m_pValue->GetContext());
	};
	TContPtr operator->() const
	{
		TContPtr pCont = operator TContPtr();
		ATLASSERT(pCont!=NULL);

		if (pCont)
			return pCont;

		_ErrorNULLObject();
		return pCont; // ����� �������������� �����������
	}
	
	// ��� ������ ������� � cpp-���� ��� ����������� runtime-���������� 1C � ������ ������������� ��������� NULL
	// ����� ���������� ��������
	// �� �������� �������� ������ ��� ����� ���������, ��������� � blptr.h
	//inline TCont& operator*() const;
	TCont& operator*() const
	{
		TContPtr pCont = operator TContPtr();
		ATLASSERT(pCont!=NULL);

		if (pCont)
			return *pCont;

		_ErrorNULLObject();
		return *pCont; // ����� �������������� �����������
	}

    void Create(const CString& szTypeName)
	{
		Release();

		// � ������������� ���������� ���������, ����� ������� ������� ������, ������� ������� �� ����� ������
		CBLPtr<TCont> pCont(szTypeName); //no cast is special: this only for CBLContext instance

		if (pCont)
			Assign(pCont);
		else
			if (!szTypeName.CompareNoCase("�����������������") || !szTypeName.CompareNoCase("GroupContext"))
			{
				TContPtr pCont = static_cast<CGroupContext*>(RUNTIME_CLASS(CGroupContext)->CreateObject());
			
				// � ������ ��� IncrRef
				Assign(pCont);

				pCont->DecrRef(); // �� ��������� ������� ������
			}

	};


	CFormAllCtrlsContext* GetFormContext(void)
	{
		if(!IsGroupContext())
			return NULL;

		return GetFormContext(operator TContPtr());
	}

	CGetDoc7* GetDoc7(void)
	{
		return GetDoc7(operator TContPtr());
	}

};

typedef CSafeContextPtr<CBLContext> CSafeContextPtrDef;

// ���������� �����-��������� �� CBLModule7
//	����� ����������� ��������/�������� ������� 1�
//  � ����������� ������ CBLModule7 ���������� ��� ���������-������� ������, 
// ������� ��������� �� ��������� ������
//
//	����� ����� ��������� ���������� ����� CBLContext - CVlalue
//
//	� ����� ���������� ��������� ������� ��������� �� ������, �� �������� ��������� � ������ ����� ��� �����������
//		��������, �������� CComponentClass::pCurModule � ����� ������������������������� 
//	( ����������, �� ������ ���������� ���������, �����)
//
//	��� CBLModuleWrapper* ������� ����� � ��������� ������ ��� ����������� �������� ���������, ����������� ����� new
//
class CSafeModulePtr
{
private:
	typedef CBLModule T; // ? CBLModule7 ?
	typedef CBLModule* TPtr;
	typedef const CBLModule* ConstTPtr;

	TPtr m_pMod;
	bool m_bIsAutoDelete;

private:

	void Link(ConstTPtr pMod);
	void Unlink();

	// ���������� ������
	void _Assign(ConstTPtr pMod) // ��� ������������� Release (���� ��� ��������)
	{ 
		Link(pMod);

		m_pMod = const_cast<TPtr>(pMod);
		
		if (m_pMod)
			if (m_bIsAutoDelete)
			{	
				int iRefCount;
				if (m_mapOfRefModules.Lookup(m_pMod, iRefCount))
					m_mapOfRefModules.SetAt(m_pMod, ++iRefCount);
				else
					m_mapOfRefModules.SetAt(m_pMod, 1);
			}
	}
	
	void Assign(ConstTPtr pMod)
	{
		Release();
		m_bIsAutoDelete = false;
		_Assign(pMod);
	};

	void _Assign(const CBLModuleWrapper* pNewMod)
	{
		m_bIsAutoDelete = true;
		_Assign((ConstTPtr)pNewMod);
	};
	
	void _Assign(const CSafeModulePtr& rp)
	{
		m_bIsAutoDelete = rp.m_bIsAutoDelete;
		_Assign(rp.m_pMod);
	};
	

	template <class T>
	void Assign(const T& rp)
	{
		Release();
		_Assign(rp);
	};

	// ������������ ������
	void Release()
	{ 
		Unlink();

		if (m_pMod)
			if (m_bIsAutoDelete)
			{
				int iRefCount;
				if (m_mapOfRefModules.Lookup(m_pMod, iRefCount))
				{
					m_mapOfRefModules.SetAt(m_pMod, --iRefCount);
					if (!iRefCount)
					{
						m_mapOfRefModules.RemoveKey(m_pMod);
						m_bIsAutoDelete = false;
						delete m_pMod;
					}
				}
			}
		m_pMod = NULL;
	}

	// ��������� ���������� � ��������� ����������
	void _ErrorNULLObject(LPCSTR szErrorString = NULL) const;

public:

	// �������� �������� ������������� � ����������� CBLModule7 ��� ����������� �����
	static void InitHook();
	static void DestroyHook();

	// ��������������
	CSafeModulePtr() : m_pMod(NULL), m_bIsAutoDelete(false)
	{ };

public:

	CSafeModulePtr(ConstTPtr pMod) : m_pMod(NULL), m_bIsAutoDelete(false)
	{ 
		InitHook();
		_Assign(pMod);
	};

	// ��������� ������ ���� ������� ����� new !!
	// ������ const CBLModuleWrapper*& ����� ��� ����, ����� ��������� ���������������� � ����. ���������� =
	// ��������, ��� ������ ��� CSafeModulePtr pMod(NULL); �� ��������������
	//CSafeModulePtr(const CBLModuleWrapper*& pMod) : m_pMod(NULL), m_bIsAutoDelete(false)
	CSafeModulePtr(const CBLModuleWrapper& pMod) : m_pMod(NULL), m_bIsAutoDelete(false)
	{ 
		InitHook();
		_Assign(&pMod);
	};

	CSafeModulePtr(const CSafeModulePtr& rp) : m_pMod(NULL), m_bIsAutoDelete(false)
	{
		InitHook();
		// TODO
		_Assign(rp);
	};

	~CSafeModulePtr()
	{ Release(); };

	// �������� ��������� ����������
	inline bool IsGroupContext() const
	{
		return false; //return m_pValue && IS_KINDOF_RUNTIME_CLASS(m_pValue->GetContext(), CGroupContext );
	};

	// ���������
	CSafeModulePtr& operator =(ConstTPtr  pMod)
	{
		//if (operator TPtr () != pMod)
		Assign(pMod);

		return *this;
	};

	// ��������� ������ ���� ������� ����� new !!
	// ������ const CBLModuleWrapper*& ����� ��� ����, ����� ��������� ���������������� � ����. ���������� =
	// ��������, ��� ������ ��� pMod = NULL; �� ��������������
	CSafeModulePtr& operator=(const CBLModuleWrapper& pMod)
	{ 
		Assign(&pMod);

		return *this;
	};
	
	CSafeModulePtr& operator=(const CSafeModulePtr& rp)
	{
		if (&rp == this)
			return *this;

		Assign(rp);

		return *this;

	};
public:

	operator bool() const
	{
		return m_pMod != NULL; // return (m_pValue && m_pValue->GetContext());
	};
	operator !() const
	{ 
		return !operator bool();
	};

	bool operator==(TPtr pT) const
	{
		return operator TPtr() == pT;
	};
	bool operator!=(TPtr pT) const
	{
		return !operator ==(pT);
	};

	operator TPtr() const
	{ 
		return m_pMod; // return !m_pValue ? NULL : m_pValue->GetContext();
	};
	TPtr operator->() const
	{
		TPtr pMod = operator TPtr();
		ATLASSERT(pMod!=NULL);

		if (pMod)
			return pMod;

		_ErrorNULLObject();
		return pMod; // ����� �������������� �����������
	}
	
	// ��� ������ ������� � cpp-���� ��� ����������� runtime-���������� 1C � ������ ������������� ��������� NULL
	// ����� ���������� ��������
	// �� �������� �������� ������ ��� ����� ���������, ��������� � blptr.h
	//inline CBLContext& operator*() const;
	T& operator*() const
	{
		TPtr pMod = operator TPtr();
		ATLASSERT(pMod!=NULL);

		if (pMod)
			return *pMod;

		_ErrorNULLObject();
		return *pMod; // ����� �������������� �����������
	}
	
	typedef vector<CSafeModulePtr*> CVectorModulePtr;
	typedef CMap<ConstTPtr, ConstTPtr, CVectorModulePtr*, CVectorModulePtr*> CModulesMap;

	typedef CMap<CBLModule*, CBLModule*, int, int> CRefModules;
	
private:
	CBLModule* WrapCBLModuleConstructor1(class CBLModule const &);
	CBLModule* WrapCBLModuleConstructor2(class CBLContext *,char const *);
	void WrapCBLModuleDestructor(void);

	static bool bAlreadyInit; 

	static CModulesMap m_mapOfLinkModules; // ����� ��������� �������

	// ����� �������, ������� �� ���� ������ ������� (CBLModuleWrapper)
	static CRefModules m_mapOfRefModules; // ����� ��������� �������

	static void ClearModulePtr(CSafeModulePtr* ptr);

};

#endif //#ifndef __SAFE__CONTEXT__INCLUDED
