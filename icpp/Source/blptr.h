//______________________________________________________________________________
//
// ������� � ������� �����-��������� �� CBLContext
//
// �����: ����� �������� ���������� ���������
//		������� ��� ��������� ������� ������ ��� ��������� ����������� �������� DecrRef
//
// ������� ����� ������ ������������ ��� ����������, ����������� ��������������� � ���� ��
//
//	�����: ����� ������ ������������ ��� �������� ��������� ����������, 
//		�.�. �� ������������� �������� ����� ������������� � �������� ��������������� ����������
//
//	���������� �������� ������ ���������� ����� - ����� ��������� �� �������� CSafeContextPtr (SafeContext.h)
//		��� ����� ���������� �����, ������������� ����������� ����� ����������, 
//			� �.�. � ��������� ����������
//		����������� ����������� ������ � �������������� ������������� 
// 
//	����� DmitrO 
// ����� ��������� � ������� artbear, 2006

// kms:
// ��� ��������� ���������� ������ IncrRef /DecrRef �� ���������� - ��� �����������
//______________________________________________________________________________
//

#pragma once

#ifndef __BLPTR__INCLUDED
#define __BLPTR__INCLUDED

#include <boost/concept_check.hpp>

template <class T>
class CBLPtr
{
	template<class Y> friend class CBLPtr;
public:
	typedef T _PtrClass;
	CBLPtr() : p(NULL), m_bIsGroupContext(FALSE)
	{	}

	// ��������: ��� ���� ���������� �������� - �������� ������ ������
	//	CBLPtr<CExecuteModule> pContExecModule(new CExecuteModule);
	//
	// ���� ������������ ��� ������������ ���������, �� ����� ������������ ����.���
	//	CBLContext* pCont = new CExecuteModule;
	//	CBLPtr<CExecuteModule> pContExecModule(pCont);
	//	pCont.DecrRef();
	//
	// ������������: ������ ��������� �������� ����� CBLPtr<CExecuteModule> pContExecModule("");
	explicit CBLPtr(T* lp) : p(NULL), m_bIsGroupContext(FALSE)
	{
		_Assign(lp);
	}

	CBLPtr(const CBLPtr<T>& lp) : p(NULL), m_bIsGroupContext(FALSE)
	{
		_Assign(lp.p);
	}

	// explicit, ������ �����, �� ����� - artbear ���, ���-���� ����� ��� ���������� �������� ��������������
	template<class U>
	explicit CBLPtr(const CBLPtr<U>& lp) : p(NULL), m_bIsGroupContext(FALSE)
	{
		_Assign(lp.p); // ��������� ������������ ������ ������� �������������� - �� CBLPtr<���������_CBLContext> � CBLPtr<CBLContext>
	}

	// ������� ����� �RuntimeClass::CreateObject
	explicit CBLPtr(CRuntimeClass& RuntimeClassRef) : p(NULL), m_bIsGroupContext(FALSE)
	{
		T* newptr = static_cast<T*>(RuntimeClassRef.CreateObject());
		_Assign(newptr);
		newptr->DecrRef();
	}

	// ���� �������� ������ ������, ����� ������ �������� ���� ����, ��� �������� � �������
	//	����� �� ������ ��� ������ ����
	//		CBLPtr<CExecuteModule> pContExecModule();
	//		pContExecModule.Create();
	//	� ������
	//		CBLPtr<CExecuteModule> pContExecModule("");
	//
	// �������� ����� const CString& - ���� ����� LPCSTR, �� ����� ������ ��� �������� �� NULL
	// ��������, ������ ���������� �� CBLPtr<CBLContext> pCont(NULL);
    explicit CBLPtr(const CString& szTypeName) : p(NULL), m_bIsGroupContext(FALSE)
    {
		if (szTypeName.IsEmpty())
			Create();
		else
			Create(szTypeName);
    };
	~CBLPtr()
	{
		boost::function_requires< boost::ConvertibleConcept<T*, CBLContext*> >();

		Release();
	}
	operator T*() const
	{
		return get();
	}
	
	T* get() const
	{
		return p;
	}

	// TODO ������-�� ��� ������������� CBLPTr<CBLFont> � ������������� ����� ���������
	// ���� ���� �������� �� ���������� � h-�����, � � cpp-�����, ����� ������ ��������
	// ���� �� ������ ����� CBLContext& CSafeContextPtr<CBLFont>::operator*() const
	// � ������, ����������� ��� � �� ������� :(
	T& operator*() const
	{
		ATLASSERT(p!=NULL);
		if (p)
			return *p;

		// TODO � h-����� �� ������� ��������������� ����. �������
		// ���� ���������� ������������ �����, �������� ������ ����������
		//
		//::RuntimeError("CSafeContextPtr:: ������� ��������� � ��������������� ���������");
		_ErrorNULLObject(); // ������������� ����������

		return *p; // ��� ���������� �������������� �����������

	}

/* artbear ������� ������� �������� - ����� ������ ������ ����� �������� �������� ����������� ���������
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&()
	{
		ATLASSERT(p==NULL);
		return &p;
	}
*/    
	T* operator->() const
	{
		ATLASSERT(p!=NULL);
		if (p)
			return p;

		_ErrorNULLObject(); // ������������� ����������

		return p; // ��� ���������� �������������� �����������
	}
    
	CBLPtr& operator=(T* lp)
	{
		if (p != lp)
			Assign(lp);

	    return *this;
	}

	CBLPtr<T>& operator=(const CBLPtr<T>& lp)
	{
		if (&lp == this)
			return *this;

		return operator=(lp.p);
	}

	template<class U>
	CBLPtr<T>& operator=(const CBLPtr<U>& lp)
	{
		assert(static_cast<void*>(const_cast<CBLPtr<U>*>(&lp)) != static_cast<void*>(this));
		//if (static_cast<void*>(const_cast<CBLPtr<U>*>(&lp)) == static_cast<void*>(this))
		//	return *this;

		return operator=(lp.p); // ��������� ������������ ������ ������� �������������� - �� CBLPtr<���������_CBLContext> � CBLPtr<CBLContext>
	}

	bool operator!() const
	{
		return !operator bool();
	}

	operator bool() const
	{
		return (p != NULL);
	}

	bool operator==(T* pT) const
	{
		return p == pT;
	}

	bool operator!=(T* pT) const
	{
		return !operator ==(pT);
	}

    void Create()
    {
		Release();

		// �� ��������� ������� ������ �� ����� Assign
        p = new T();
		_CheckContext();
    };

    void Create(LPCSTR szTypeName)
    { // TODO ��� �������� ������������ ����������� ����� � ����� ������-�������

		Release();

		// ���� try �� ����� artbear - � ���� ��� ������������ ������� ����� :)
		// �� ��������� ������� ������ �� ����� Assign
		p = static_cast<T*>(CBLContext::CreateInstance(szTypeName)); //no cast is special: this only for CBLContext instance
		_CheckContext();
    };

	// �������� ��������� ����������
	inline BOOL IsGroupContext() const
	{
		return m_bIsGroupContext;
	};

	void reset()
	{
		CBLPtr<T>().swap(*this);
	}

	void swap(CBLPtr<T> & other) // never throws
	{
		std::swap(p, other.p);
		std::swap(m_bIsGroupContext, other.m_bIsGroupContext);
	}

	boost::shared_ptr<T> get_shared_ptr() const
	{
		p->IncrRef();
		return boost::shared_ptr<T>(p, CBLContext_deleter<T>());
	}

protected: // artbear
	//T* ptr;
	T* p;
	BOOL m_bIsGroupContext;

	void _CheckContext()
	{
		if (p)
			m_bIsGroupContext = p->IsKindOf( RUNTIME_CLASS( CGroupContext ));

		else
			m_bIsGroupContext = FALSE;
	};

	void _IncrRef()
	{
		if (p && !IsGroupContext())
			p->IncrRef();
	}

	void _DecrRef()
	{
		if (p && !IsGroupContext())
			p->DecrRef();
	}

	// ���������� ������
	void _Assign(T * pCont)
	{
		p = pCont;

		_CheckContext();
		_IncrRef();
	};

	void Assign(T * pCont)
	{
		Release();
		_Assign(pCont);
	};

	// ������������ ������
	void Release()
	{
		_DecrRef();
		//p = 0;
	};

	void _ErrorNULLObject(LPCSTR szErrorString = NULL) const; // ������������� ���������� � ��������� ����������

};

// ��������� ���������� � ��������� ����������
template <class T>
void CBLPtr<T>::_ErrorNULLObject(LPCSTR szErrorString) const
{
	if (!szErrorString)
		szErrorString = "CBLPtr<T> : ������� ��������� � ��������� NULL"; // TODO ���������� ��� ��������� ������ ��� �������� ��������

	::RuntimeError("%s", szErrorString); // TODO � ���������� ����� ������� ������ ��� ����������

};

#endif