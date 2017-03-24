// contextimpl.hpp
#pragma once
#pragma warning(disable: 4786)	// ��������� �������� �� ������� ������� ������
/*
(�) ��������� ������� telepat@inbox.ru

������������ ���� ��� ���������� CBLContext'��� ��������.
�������������:
��� ���������� ������, ������������� ��������������� ��� �������������, �������� ���� ����� ���

  class �������� : public CContextImpl<��������>

����, ���� ����� �� ������ ����������� ����� �������������, �� ���:

  class �������� : public CContextImpl<��������, no_init_done>


����� ����������� ������� � ������� ��������� ����������� ������
	
	BL_BEGIN_CONTEXT("EnglishName", "����������");

�� ������ ����� ���� 1� ��� ����� ������ (������������ � ��������������) � ������
����������� ��� ���������� ������.

��� �������� ������� ������������ ������� ����������� ������� � ���������� ������:
BL_PROC - ��������� ��������� ���������
BL_FUNC - ��������� ������� ���������
BL_PROC_WITH_DEFVAL - ��������� ��������� ���������, ����������� ��������� �� ���������
BL_FUNC_WITH_DEFVAL - ��������� ������� ���������, ����������� ��������� �� ���������

� �������� ���������� ��� ������� ���������:
�������������			- ������ ���������� ��� ������ ���������
"����������"			- ������ ������� ��� ������ ���������
��������������������	- ������ ���������� ���������� ������

������ ������� ������� ����������� �������������� ��� ���������� CBLContext, � ��������� ������
	int �������������(CValue** ppParams) ��� ��������
	���
	int �������������(CValue& retVal, CValue** ppParams) ��� �������.
�� ���� ��������������� ����� ������� ����� ���� ��������� ; � ����������� ����� ��� ������,
���� ����� �������� ���������� ������.
��������:

	// � ��������� ������
	BL_PROC(Test, "�������", 0)
	{
		AfxMessageBox("Test");
		return TRUE;
	}

���� ���:

	// � ��������� ������
	BL_PROC(Test, "�������", 0);

// ��� ������:
int CMyContextClass::Test(CValue** ppParams)
{
	AfxMessageBox("Test");
	return TRUE;
}

��� �������, ����������� �������� �� ���������, ���������� �������� ������� ������ ����
int �������������_GetDefVal(int nParam, CValue* pVal)
��� ������ ������ ��������

  BL_DEFVAL_FOR(�������������)


��� ���������� ������� ��������� ����������� �������:

BL_PROP_RO	- ������ �������� ������ ��� ������
BL_PROP_WO	- ������ �������� ������ ��� ������
BL_PROP_RW	- ������ �������� ��� ������/������

��������� ��������:
�������������	- ������ ���������� ��� ��������
"����������"	- ������ ������� ��� ��������

������� ������� ����� ������������ ������� ���
int Get_�������������(CValue& value) const - ��� ������
int Set_�������������(CValue const& value) - ��� ������

����� ����� ������� ����� ���� ��������� ; � ����������� ���������� ��� ������,
���� ����� �������� ���������� ����������� - ��� BL_PROP_RO � BL_PROP_RW
���������� ������, ��� BL_PROP_WO - ���������� ������.
��� ���������� �������� ��� ������/������ ��� ��������� ����������� ������
����������� ������
BL_PROP_WRITE(�������������)

����� �������� ���� �������/������� ���������, �������� ������

 	BL_END_CONTEXT();

��� ��������������� ������ ������ ��� �������������, � ����� ����������
������ ����������� ������:

  BL_INIT_CONTEXT(���������������);

 ������ ������:
// ���� .h

class CTestBL : public CContextImpl<CTestBL>
{
public:
	BL_BEGIN_CONTEXT("TestBL", "������������");

	BL_PROC(Test, "����", 0);

	long m_a;

	BL_PROC(DoBox, "��������", 0)
	{
		AfxMessageBox("DoBox");
		return TRUE;
	}

	BL_FUNC(SomeFunc, "��������", 0)
	{
		retVal=1;
		return TRUE;
	}

	BL_PROC_WITH_DEFVAL(DefTest, "�����������", 1)
	{
		CString txt=ppParams[0]->GetString();
		if(!txt.IsEmpty())
			AfxMessageBox(txt);
		return TRUE;
	}
	BL_DEFVAL_FOR(DefTest)
	{
		if(nParam==0)
		{
			if(pValue)
				*pValue="Hello World!!!";
			return TRUE;
		}
		return FALSE;
	}

	BL_PROP_RO(SomeProp, "�����")
	{
		value="Test of Some Prop";
		return TRUE;
	}
	BL_PROP_WO(SomeWrite, "������������")
	{
		return TRUE;
	}

	BL_PROP_RW(ReadWrite, "���������")
	{
		value=m_a;
		return TRUE;
	}
	BL_PROP_WRITE(ReadWrite)
	{
		m_a=value.GetNumeric();
		return TRUE;
	}

	BL_END_CONTEXT();
};
.... 

//���� .cpp
BL_INIT_CONTEXT(CTestBL);

int CTestBL::Test(CValue** ppParams)
{
	AfxMessageBox("Test");
	return TRUE;
}

��� ������������� � ����������� ���� ������� ���������
��� ������������� ���������� �������� �����:
  context_obj::CContextBase::InitAllContextClasses();

��� �������� ���������� �������� �����:
  context_obj::CContextBase::DoneAllContextClasses()
*/

namespace context_obj{
// �������� ���� ������������
int __declspec(selectany) defLang=0;

namespace str_find{
// ������� ���������������� ��������
DWORD __declspec(selectany) lotable[256]={0};
}//namespace str_find

// ������� ������� ������ ��������
#pragma data_seg(".blc$a")
#pragma data_seg(".blc$b")
#pragma data_seg(".blc$c")
#pragma data_seg(".blc$d")
#pragma data_seg(".blc$e")
#pragma data_seg()
// ������ ������� � �������� ����� ����� �������� .blc � .data
#pragma comment(linker, "/MERGE:.blc=.data")

typedef void (*P_VV)();

__declspec (allocate(".blc$a")) P_VV __declspec(selectany) pBLInit=NULL;
__declspec (allocate(".blc$c")) P_VV __declspec(selectany) pBLInitDone=NULL;
__declspec (allocate(".blc$e")) P_VV __declspec(selectany) pBLDone=NULL;

// ������� ���������� �������, ����������� ������ ��������� �����������
class CContextBase : public CBLContext
{
public:
	CContextBase():CBLContext(){}
	virtual char const*  GetCode(void)const		{return NULL;}
	virtual int  GetDestroyUnRefd(void)const	{return TRUE;}
	virtual CObjID GetID(void)const				{return CObjID();}
	virtual long  GetTypeID(void)const			{return 100;}
	virtual CType GetValueType(void)const		{return CType(100);}
	virtual int  IsExactValue(void)const		{return TRUE;}
	virtual int  IsOleContext(void)const		{return FALSE;}
	virtual int  IsSerializable(void)			{return FALSE;}
	virtual int  SaveToString(CString &)		{return FALSE;}
	virtual void InitObject(CType const &t)		{}
	virtual void InitObject(LPCSTR lpName)		{}
	

	static void InitAllContextClasses()
	{
		defLang=GetMetaData()->GetTaskDef()->GetDefaultLanguage();
		for(int i=0;i<256;i++)
			str_find::lotable[i]=(DWORD)CharLower((LPSTR)i);
		for(P_VV* ppInit=&pBLInit+1;ppInit<&pBLInitDone;ppInit++)
			if(*ppInit)
				(*ppInit)();
	}

	static void DoneAllContextClasses()
	{
		for(P_VV* ppDone=&pBLInitDone+1;ppDone<&pBLDone;ppDone++)
			if(*ppDone)
				(*ppDone)();
	}
};

/*
�������� ��������� ����������.
������� �� ������ �������� ���������� ������, � ������� ���������� �������
�������� �������� �����, ��� ������������� ������������ ������������.

������ �����/�������� ����������� ����� �����, ���������� ������������ ����������.
� ��� �������, � ��� ������� ��� ���������� ������:
- ������ ��� ������ ����� ������ prev �� ��� ����������� ������/��������:
	typedef ������������� prev;
- ������ ��� ������ ����� ������������ val, ������ val ����������� ���� ���� 1:
	enum{val=prev::val+1};
- ������ ��� ������ ����� ������:
	static LPCSTR GetRusName(); // ���������� ������� ��� ������/��������
	static LPCSTR GetEngName(); // ���������� ���������� ��� ������/��������

��� �������� ������� ��� ����� ����� ������ �����:
- ������������ IsFunc = 0 | 1, ����������, ��������� ��� ��� �������.
- ������������ IsDefVal = 0 | 1, ����������, ���� �� � ������ ��������� �� ���������.
- ������������ nParam, ����������, ������� ���������� � ������.
- ������:
	static int CallProc(T* ptr, CValue** ppParams);						// ����� ��� ���������
	static int CallFunc(T* ptr, CValue& retVal, CValue** ppParams);		// ����� ��� �������
	static int GetDefVal(const T* ptr, int nParam, CValue* pParam);		// �������� �������� �� ���������

��� �������� ���������� ��� ������ �����:
- ������������ IsRead	= 0 | 1, ����������, �������� �� �������� ��� ������
- ������������ IsWrite	= 0 | 1, ����������, �������� �� �������� ��� ������
- ������:
	static int Read(const T* ptr, CValue& value);	// ����� ��� ������
	static int Write(T* ptr, CValue const& value);	// ����� ��� ������

��� ������� ������/�������� � �������� ����������� ���� ������ ������������ ��� first_def.
� ����� ������ ������ ���� ���������� ���� endmeths ��� ������� � endprops ��� �������,
� ������� ����������� ������ prev �� ��� ���������� �� ����������� �������/�������,
� ������������ val, ������ val ���� ���������� ������/�������� ���� ����.

��� ��� ��������� �������� � ������� BL_BEGIN_CONTEXT, BL_PROC, BL_FUNC,
BL_END_CONTEXT � ��.

���������� ����� ��� T::endmeths � T::endprops �������� ������ � ������ �����
�������/������� ���������, � ����������� ��� ������� ���������, ����������
����������� ��� ���������� ����������� ������� CBLContext'�.
*/

// ������ �������������� ������ � ���������� �������/�������
struct first_def
{
	enum
	{
		val		=-1,
		IsFunc	=-1,
		nParam	=-1,
		IsDefVal=-1,
		IsRead	=-1,
		IsWrite	=-1,
	};
};

// ����� ���������� ��� ������������� ������ ������ �� ������
namespace str_find{

static inline DWORD symbol(LPCSTR ptr){return static_cast<DWORD>(static_cast<BYTE>(*ptr));}

// �������� ������ � ����� � ������ ��������
static inline void makelower(LPCSTR& ptr, LPSTR buf)
{
	for(;;)
	{
		DWORD s=lotable[symbol(ptr)];
		*buf=static_cast<char>(s);
		if(!s)
			return;
		buf++;
		ptr++;
	}
}
// ���������� �� ��������� ������, ��� ptr1 � ptr2 - ����� ������
static inline BOOL compareUU(LPCSTR ptr1, LPCSTR ptr2)
{
	while(symbol(ptr1) && lotable[symbol(ptr1)] == lotable[symbol(ptr2)])
		ptr1++,ptr2++;
	return symbol(ptr1)==symbol(ptr2);
}

// ���������� �� ��������� ������, ��� ptr1 - ������ � ������ ��������,
// � ptr2 - ����� ������
static inline BOOL compareLU(LPCSTR ptr1, LPCSTR ptr2)
{
	while(symbol(ptr1) && symbol(ptr1) == lotable[symbol(ptr2)])
		ptr1++,ptr2++;
	return symbol(ptr1)==symbol(ptr2);
}

// ���������� �� ��������� ������, ��� ptr1 � ptr2 - ������ � ������ ��������
static inline BOOL compareLL(LPCSTR ptr1, LPCSTR ptr2)
{
	while(symbol(ptr1) && symbol(ptr1) == symbol(ptr2))
		ptr1++,ptr2++;
	return symbol(ptr1)==symbol(ptr2);
}

// ��������� ������ ������ �������� �� ��� �����

// ��������� ��� ���������� ���������
template<typename T>
struct nofnd_pol
{
	static void OnInitFirstObject(){}
	static int Find(LPCSTR name){return -1;}
};

// ��������� ��� ������� ����� ������ ��������
template<typename T>
struct onefnd_pol
{
	static void OnInitFirstObject(){}
	static int Find(LPCSTR name)
	{
		if(name && (compareUU(name, T::prev::GetRusName()) ||
			compareUU(name, T::prev::GetEngName())))
			return 0;
		return -1;
	}
};

// ��������� ������ ��� ���������� ��������� �� 2 �� 3
template<typename T>
struct simplefnd_pol
{
	static void OnInitFirstObject(){}
	static int Find(LPCSTR name)
	{
		if(!name)
			return -1;
		char buf[512];
		makelower(name, buf);
		return find(buf, (T::prev*)NULL);
	}
	template<typename M>
	static int find(LPCSTR buf, M*)
	{
		if(compareLU(buf, M::GetRusName()) ||
			compareLU(buf, M::GetEngName()))
			return M::val;
		return find(buf, (M::prev*)NULL);
	}
	template<>static int find<first_def>(LPCSTR buf, first_def* p){return -1;}
};

// ��������� ��� ���������� ��������� ������ 3
struct hs_find_nearest_bigger
{
	template<int I>	struct hashsize{enum{val = hashsize<I + 1>::val};};
	#define HASH_SIZE(par) template<>struct hashsize<par>{enum{val=par};}
	HASH_SIZE(13); HASH_SIZE(19); HASH_SIZE(29); HASH_SIZE(37); HASH_SIZE(47); HASH_SIZE(53);
};

struct hs_simple_calc
{
	template<int I>struct hashsize{enum{ val = (I + I / 2) | 1};};
};

template<BOOL I> struct hs_selector { typedef hs_find_nearest_bigger type;};
template<> struct hs_selector<FALSE>{ typedef hs_simple_calc type;};

template<typename T, int I> struct hs_get {	enum{val = T::hashsize<I>::val};};

template<int I>
struct hashsizes
{
	enum{ val = hs_get<hs_selector<(I < 53 ? TRUE : FALSE)>::type, I>::val };
};


template<typename T>
struct hashfnd_pol
{
	enum{
		Count=T::val,
		HashSize=hashsizes<Count*2>::val,
	};
	struct Assoc // ��������� ��� �������� ������ � ����� ����-�����
	{
		Assoc(int p, BYTE* buf):pos(p), strKey(buf){}
		DWORD hash;
		BYTE *strKey;
		int pos;
		Assoc *next;
	};

	Assoc* ppHashTable[HashSize];	// ���-�������. ������ ���������� �� ����������� ������ � �������
	
	static DWORD GetHash(LPCTSTR key, DWORD& _hash, BYTE *buf)
	{
		_hash=0;
		for(;;)
		{
			DWORD symb=lotable[symbol(key)];
			*buf=static_cast<BYTE>(symb);
			if(!symb)
				return _hash % HashSize;
			_hash=(_hash<<1)^symb;
			buf++;
			key++;
		}
	}
	
	hashfnd_pol(){memset(ppHashTable, 0, sizeof(ppHashTable));}
	~hashfnd_pol()
	{
		int cnt=Count*2;
		for(int i=0;i<HashSize && cnt;i++)
		{
			Assoc* pAssoc=ppHashTable[i];
			while(pAssoc)
			{
				delete [] pAssoc->strKey;
				Assoc* pDel=pAssoc;
				pAssoc=pAssoc->next;
				delete pDel;
				cnt--;
			}
		}
	}
	int find(LPCSTR name)
	{
		if(!name)
			return -1;
		BYTE buf[512];
		DWORD hash;
		Assoc *pAssoc=ppHashTable[GetHash(name, hash, buf)];
		while(pAssoc)
		{
			if(pAssoc->hash==hash)
			{
				if(compareLL(reinterpret_cast<LPCSTR>(pAssoc->strKey), reinterpret_cast<LPCSTR>(buf)))
					return pAssoc->pos;
			}
			pAssoc=pAssoc->next;
		}
		return -1;
	}
	void Insert(LPCSTR name, int pos)
	{
		Assoc* pNew=new Assoc(pos, new BYTE[strlen(name)+1]);
		int posInTable=GetHash(name, pNew->hash, pNew->strKey);
		pNew->next=ppHashTable[posInTable];
		ppHashTable[posInTable]=pNew;
	}

	static hashfnd_pol<T>*& getme(){static hashfnd_pol<T>* ptr;return ptr;}

	template<typename M>
	static void AddNames(M* p=NULL)
	{
		getme()->Insert(M::GetRusName(), M::val);
		getme()->Insert(M::GetEngName(), M::val);
		AddNames((M::prev*)NULL);
	}
	template<> static void AddNames<first_def>(first_def* p){}


	static void OnInitFirstObject()
	{
		getme()=new hashfnd_pol<T>;
		AddNames((T::prev*)NULL);
	}
	static int Find(LPCSTR name){return getme()->find(name);}
};

}//namespace str_find

// ����-�������, �������� ��� ������ ������ �� ������,
// ������������ �� ���������� �������/�������.
// ������������: fndselector<T::endmeths>::type, fndselector<T::endprops>::type
template<typename T>
struct fndselector
{
	template<int K>struct tdef;
	template<>struct tdef<0>{typedef str_find::nofnd_pol<T>		type;};
	template<>struct tdef<1>{typedef str_find::onefnd_pol<T>	type;};
	template<>struct tdef<2>{typedef str_find::simplefnd_pol<T>	type;};
	template<>struct tdef<3>{typedef str_find::hashfnd_pol<T>	type;};

	enum{
		Count=T::val,
		val = Count==0 ? 0 : (Count==1 ? 1 : (Count<4 ? 2 : 3)),
	};
	typedef typename tdef<val>::type type;
};

// ������� ����� �� ������� ���� �� �������.
// ������ ���� ��������� ��� ������ ��������� � GetName.
template<typename T>
struct getname
{
	template<int Count>
	struct finder
	{
		LPCSTR names[Count][2];
		static finder<Count>* getme(){static finder<Count>* one=new finder<Count>;return one;}
		finder(){fill((T::prev*)NULL);}
		template<typename M>
		void fill(M*)
		{
			names[M::val][0]=M::GetEngName();
			names[M::val][1]=M::GetRusName();
			fill((M::prev*)NULL);
		}
		template<>void fill<first_def>(first_def*){}

		static LPCSTR GetName(DWORD dwMethod, DWORD dwLang)
		{
			if(dwMethod < Count && dwLang < 2)
				return getme()->names[dwMethod][dwLang];
			return NULL;
		}
	};
	template<>
	struct finder<0>
	{
		static LPCSTR GetName(DWORD dwMethod, DWORD dwLang){return NULL;}
	};
	typedef typename finder<T::val> type;
};

namespace meta_func{
// ����� ��������� �����������, ���������� �� �������� �����
// �������� ������� � �������.

// �������� �� ��, ���� BL_END_CONTEXT() � �� � ���������� �����
template<typename T, int I>
struct test_last
{
	enum{val = I < T::val ? 1 : -1};
	// ���� � ��� ����� ������, ������, ������ BL_END_CONTEXT() � �����
	// ������ �����������, ���� �������� �� ����� ���������� ������/��������
	// ������� F4, ����� ����������, ���
	typedef char type[val];
};

// �������� �� ��, ��� ������ � ����������� �� ��������� ����� �� ������ 1 ���������
template<int I, BOOL bDef> struct check_defval
{
	enum{val = bDef && !I ? -1 : 1};
	// ���� ����� � ��� ������, ������, ��� ������ � ����������� �� ���������
	// ������ 0 ����������
	typedef char type[val];
};

// ����-�������, ������������, ������� ����� ������� �������� � �������.
// ������������:
// meth_count<T>::proc - ���������� ��������
// meth_count<T>::func - ���������� �������
template<typename T>
struct meth_count
{
	template<int I>
	struct counter
	{
		template<typename M>
		struct check
		{
			enum{
				next = M::prev::val == -1 ? 0 : 1,
				val = M::IsFunc + counter<next>::template check<M::prev>::val,
			};
		};
	};
	template<> struct counter<0>{template<typename M>struct check{enum{val = 0};};};

	enum{
		first	= T::endmeths::prev::val == -1 ? 0 : 1,
		func	= counter<first>::check<T::endmeths::prev>::val,
		proc	= T::endmeths::val - func,
	};
};

// ����-�������, ����������, �� ����-�� ������� ���������� ���������� ����������
// ������������: some_np<T>::val
// ����������: -1 - � ������� ������ ���������� ����������, N - �� ���� ������� N ����������
template<typename T>
struct some_np
{
	template<int I>	struct _some_np{template<typename M>struct check{enum{val=I};};};
	template<> struct _some_np<-1>{template<typename M>struct check{enum{val=-1};};};
	template<> struct _some_np<-2>
	{
		template<typename M>
		struct check
		{
			enum{
				stop = M::prev::val==-1? 1 : 0,
				some = M::nParam == M::prev::nParam ? 1 : 0,
				next = stop ? M::nParam : (some ? -2 : -1),
				val  = _some_np<next>::template check<M::prev>::val,
			};
		};
	};
	enum{
		first=T::endmeths::prev::val==-1 ? 0 : -2,
		val=_some_np<first>::check<T::endmeths::prev>::val
	};
};

// ����-�������, ������������, ������� ���������� �������� ��� ������/������.
// ������������:
// rw_count<T>::read	- ��������� ��� ������
// rw_count<T>::write	- ��������� ��� ������
// rw_count<T>::noread	- ����������� ��� ������
// rw_count<T>::nowrite	- ����������� ��� ������
template<typename T>
struct rw_count
{
	template<int I>
	struct rcounter
	{
		template<typename M>
		struct check
		{
			enum{
				next= M::prev::val==-1 ? 0 : 1,
				val = M::IsRead + rcounter<next>::template check<M::prev>::val,
			};
		};
	};
	template<>struct rcounter<0>{template<typename M>struct check{enum{val = 0};};};

	template<int I>
	struct wcounter
	{
		template<typename M>
		struct check
		{
			enum{
				next= M::prev::val==-1 ? 0 : 1,
				val = M::IsWrite + wcounter<next>::template check<M::prev>::val,
			};
		};
	};
	template<>struct wcounter<0>{template<typename M>struct check{enum{val = 0};};};

	enum{
		first=T::endprops::prev::val==-1 ? 0 : 1,
		read = rcounter<first>::check<T::endprops::prev>::val,
		write= wcounter<first>::check<T::endprops::prev>::val,
		noread = T::endprops::val - read,
		nowrite= T::endprops::val - write,
	};
};

}//namespace meta_func

template<typename T>
struct meth_pol
{
	enum{
		isFuncMin = meta_func::meth_count<T>::func <= meta_func::meth_count<T>::proc ? 1 : 0,
		nParamCmn = meta_func::some_np<T>::val,
	};

	static int HasRetVal(DWORD dwMethod)
		{return policy<isFuncMin>::HasRetVal(dwMethod, (T::endmeths::prev*)NULL);}

	static int GetNParams(DWORD dwMethod)
		{return nparam<nParamCmn>::GetNParams(dwMethod);}

	static int GetDefVal(const T* ptr, DWORD dwMethod, DWORD dwParam, CValue* pParam)
		{return GetDefVal(ptr, dwMethod, dwParam, pParam, (T::endmeths::prev*)NULL);}
	
	static int CallAsProc(T* ptr, DWORD dwMethod, CValue** ppParams)
	{
		return CallAsProc(ptr, dwMethod, ppParams, (T::endmeths::prev*)NULL);
	}
	static int CallAsFunc(T* ptr, DWORD dwMethod, CValue& ret, CValue** ppParams)
	{
		return CallAsFunc(ptr, dwMethod, ret, ppParams, (T::endmeths::prev*)NULL);
	}

	template<BOOL bFunc/*==TRUE*/>
	struct policy
	{
		template<typename M>
		static int HasRetVal(DWORD dwMethod, M*)
		{
			if(M::IsFunc && dwMethod==M::val)
				return TRUE;
			return HasRetVal(dwMethod, (M::prev*)NULL);
		}
		template<> static int HasRetVal<first_def>(DWORD, first_def*){return FALSE;}
	};
	template<>
	struct policy<FALSE>
	{
		template<typename M>
		static int HasRetVal(DWORD dwMethod, M*)
		{
			if(!M::IsFunc && dwMethod==M::val)
				return FALSE;
			return HasRetVal(dwMethod, (M::prev*)NULL);
		}
		template<> static int HasRetVal<first_def>(DWORD, first_def*){return TRUE;}
	};
	
	template<int I>struct nparam{static int GetNParams(DWORD){return I;}};

	template<>
	struct nparam<-1>
	{
		static int GetNParams(DWORD dwMethod)
		{
			if(dwMethod<T::endmeths::val)
				return _GetNParams(dwMethod, (T::endmeths::prev*)NULL);
			return -1;
		}
		template<typename M>
		static int _GetNParams(DWORD dwMethod, M*)
		{
			if(dwMethod==M::val)
				return M::nParam;
			return _GetNParams(dwMethod, (M::prev*)NULL);
		}
		template<>static int _GetNParams<first_def>(DWORD, first_def*){return -1;}
	};

	template<typename M>
	static int GetDefVal(const T* ptr, DWORD dwMethod, DWORD dwParam, CValue* pParam, M*)
	{
		if(M::IsDefVal && dwMethod==M::val && dwParam < M::nParam)
			return M::GetDefVal(ptr, dwParam, pParam);
		return GetDefVal(ptr, dwMethod, dwParam, pParam, (M::prev*)NULL);
	}
	template<>static int GetDefVal<first_def>(const T*, DWORD, DWORD, CValue*, first_def*){return FALSE;}

	template<typename M>
	static int CallAsProc(T* ptr, DWORD dwMethod, CValue** ppParams, M*)
	{
		if(!M::IsFunc && dwMethod==M::val)
			return M::CallProc(ptr, ppParams);
		return CallAsProc(ptr, dwMethod, ppParams, (M::prev*)NULL);
	}
	template<>static int CallAsProc<first_def>(T*, DWORD, CValue**, first_def*){return FALSE;}

	template<typename M>
	static int CallAsFunc(T* ptr, DWORD dwMethod, CValue& ret, CValue** ppParams, M*)
	{
		if(M::IsFunc && dwMethod==M::val)
			return M::CallFunc(ptr, ret, ppParams);
		return CallAsFunc(ptr, dwMethod, ret, ppParams, (M::prev*)NULL);
	}
	template<>static int CallAsFunc<first_def>(T*, DWORD, CValue&, CValue**, first_def*){return FALSE;}
};

template<typename T>
struct prop_pol
{
	enum{
		bReadMin  = meta_func::rw_count<T>::read < meta_func::rw_count<T>::noread ? 1 : 0,
		bWriteMin = meta_func::rw_count<T>::write < meta_func::rw_count<T>::nowrite ? 1 : 0,
	};
	static int IsPropReadable(DWORD dwProp)
		{return policy<bReadMin>::IsRead(dwProp, (T::endprops::prev*)NULL);}

	static int IsPropWritable(DWORD dwProp)
		{return policy<bWriteMin>::IsWrite(dwProp, (T::endprops::prev*)NULL);}
	
	static int ReadProp(const T* ptr, DWORD dwProp, CValue& val)
		{return ReadProp(ptr, dwProp, val, (T::endprops::prev*)NULL);}

	static int WriteProp(T* ptr, DWORD dwProp, CValue const& val)
		{return WriteProp(ptr, dwProp, val, (T::endprops::prev*)NULL);}

	template<BOOL I/*==TRUE*/>
	struct policy
	{
		template<typename M>
		static int IsRead(DWORD dwProp, M*) // �������� ������, ��� �� ��������
		{
			if(M::IsRead && dwProp == M::val)
				return TRUE;
			return IsRead(dwProp, (M::prev*)NULL);
		}
		template<>static int IsRead<first_def>(DWORD, first_def*){return FALSE;}

		template<typename M>
		static int IsWrite(DWORD dwProp, M*) // ������������ ������, ��� �� ������������
		{
			if(M::IsWrite && dwProp == M::val)
				return TRUE;
			return IsWrite(dwProp, (M::prev*)NULL);
		}
		template<>static int IsWrite<first_def>(DWORD, first_def*){return FALSE;}
	};
	template<>
	struct policy<FALSE>
	{
		template<typename M>
		static int IsRead(DWORD dwProp, M*) // �������� ������, ��� �� ��������
		{
			if(!M::IsRead && dwProp == M::val)
				return FALSE;
			return IsRead(dwProp, (M::prev*)NULL);
		}
		template<>static int IsRead<first_def>(DWORD, first_def*){return TRUE;}

		template<typename M>
		static int IsWrite(DWORD dwProp, M*) // ������������ ������, ��� �� ������������
		{
			if(!M::IsWrite && dwProp == M::val)
				return FALSE;
			return IsWrite(dwProp, (M::prev*)NULL);
		}
		template<>static int IsWrite<first_def>(DWORD, first_def*){return TRUE;}
	};

	template<typename M>
	static int ReadProp(const T* ptr, DWORD dwProp, CValue& val, M*)
	{
		if(M::IsRead && dwProp==M::val)
			return M::ReadProp(ptr, val);
		return ReadProp(ptr, dwProp, val, (M::prev*)NULL);
	}
	template<>static int ReadProp<first_def>(const T*, DWORD, CValue&, first_def*){return FALSE;}

	template<typename M>
	static int WriteProp(T* ptr, DWORD dwProp, CValue const& val, M*)
	{
		if(M::IsWrite && dwProp==M::val)
			return M::WriteProp(ptr, val);
		return WriteProp(ptr, dwProp, val, (M::prev*)NULL);
	}
	template<>static int WriteProp<first_def>(T*, DWORD, CValue const&, first_def*){return FALSE;}
};

// ��������� ��� ���������� GetRuntimeClass
template<typename T>
struct implement_rtc
{
	static CObject* PASCAL createobject_rtc(){return new T;}
	static CRuntimeClass* PASCAL GetBaseRTC(){return RUNTIME_CLASS(CBLContext);}
	static CRuntimeClass* GetRTC()
	{
		static CRuntimeClass RTC={"blclass", sizeof(T), 0, createobject_rtc, GetBaseRTC};
		return &RTC;
	}
};

template<typename T>
struct no_implement_rtc
{
	static CRuntimeClass* GetRTC()
	{
		return RUNTIME_CLASS(CBLContext);
	}
};

template<typename T>
struct rtc_selector
{
	template<BOOL I>
	struct sel
	{
		typedef typename implement_rtc<T> type;
	};
	template<>
	struct sel<FALSE>
	{
		typedef typename no_implement_rtc<T> type;
	};
};

// ��������� ��������� ��� ��������/�������� ����������

// �� ������ ������
struct no_init_done
{
	static void Init(){}
	static void Done(){}
};

// ���������������� ��� �������������
template<typename T>
struct reg_CreateObject
{
	static void Init()
	{
		CRuntimeClass* pRtc=T::GetRTC();
		CBLContext::RegisterContextClass(pRtc, T::GetRusTypeString(), CType(100));
		CBLContext::RegisterContextClass(pRtc, T::GetEngTypeString(), CType(100));
	}
	static void Done(){CBLContext::UnRegisterContextClass(T::GetRTC());}
};


// ����������, ��� ������� �����
// T - �������� �����-���������
// InitDonePolicy - ��������� ��������� ��� �������������/�������� ����������.
// �� ��������� ������������ reg_CreateObject, ������������ ����� ��� �������������.
// ����� ���� ���������������� ��������� - no_init_done - ������ �� ������.
// ����� ������ ���� ���������, ������� ������ ������������� ������
//	static void Init();
//	static void Done();

template<typename T, typename InitDonePolicy = reg_CreateObject<T>, BOOL bImplementRTC=TRUE>
class CContextImpl: public CContextBase
{
public:
	typedef T ImplType;
	typedef CContextImpl<T, InitDonePolicy, bImplementRTC> BaseType;
	typedef InitDonePolicy init_done_policy;

	// ���������� RunTimeClass
	static CRuntimeClass* GetRTC()
	{
		return rtc_selector<T>::sel<bImplementRTC>::type::GetRTC();
	}
	virtual CRuntimeClass* GetRuntimeClass()const{return GetRTC();}

	// ��� �������� ������� ������� �������������� ����� �������/�������
	CContextImpl(){static init_all_map iam;}
	virtual ~CContextImpl(){}


	template<typename P> struct end_def {typedef P prev;enum{val=prev::val+1};};
	// � �������� ������ ��� �������� �������� ����� ��������������, ������ �� ��������� �����/��������
	typedef end_def<first_def> endmeths;
	typedef end_def<first_def> endprops;

	// ���������� ����������� ������� ���������
	virtual int GetNMethods(void)const
		{return T::endmeths::val;}

	virtual int FindMethod(char const *name)const
		{return fndselector<T::endmeths>::type::Find(name);}

	virtual char const* GetMethodName(int nMethod, int nLang)const
		{return getname<T::endmeths>::type::GetName(nMethod, nLang);}

	virtual int HasRetVal(int nMethod)const
		{return meth_pol<T>::HasRetVal(nMethod);}

	virtual int GetNParams(int nMethod)const
		{return meth_pol<T>::GetNParams(nMethod);}

	virtual int GetParamDefValue(int nMethod, int nParam, CValue* pValue)const
		{return meth_pol<T>::GetDefVal(static_cast<const T*>(this), nMethod, nParam, pValue);}

	virtual int CallAsFunc(int nMethod, CValue& retVal,CValue** ppParams)
		{return meth_pol<T>::CallAsFunc(static_cast<T*>(this), nMethod, retVal, ppParams);}

	virtual int CallAsProc(int nMethod, CValue** ppParams)
		{return meth_pol<T>::CallAsProc(static_cast<T*>(this), nMethod, ppParams);}

	virtual int GetNProps(void)const
		{return T::endprops::val;}
	
	virtual char const* GetPropName(int nProp, int nLang)const
		{return getname<T::endprops>::type::GetName(nProp, nLang);}

	virtual int FindProp(char const *name)const
		{return fndselector<T::endprops>::type::Find(name);}
	
	virtual int IsPropReadable(int nProp)const
		{return prop_pol<T>::IsPropReadable(nProp);}

	virtual int GetPropVal(int nProp, CValue& value)const
		{return prop_pol<T>::ReadProp(static_cast<const T*>(this), nProp, value);}

	virtual int IsPropWritable(int nProp)const
		{return prop_pol<T>::IsPropWritable(nProp);}

	virtual int SetPropVal(int nProp, CValue const& value)
		{return prop_pol<T>::WriteProp(static_cast<T*>(this), nProp, value);}

	// ���� ����� ������, ������, �� ������ �������� � ���� �����
	// BL_BEGIN_CONTEXT("EnglishTypeName", "��������������");
	virtual char const* GetTypeString(void)const
		{return defLang ? T::GetRusTypeString() : T::GetEngTypeString();}

	// ������, �������� ����� ������� � �������. �������� ��� static � ������������
	// ��������� ������, �� ���� ���������� ����� ���� ��� �������� ������� �������
	struct init_all_map
	{
		init_all_map()
		{
			fndselector<T::endmeths>::type::OnInitFirstObject();
			fndselector<T::endprops>::type::OnInitFirstObject();
		}
	};

	static void Init(){init_done_policy::Init();}
	static void Done(){init_done_policy::Done();}
};

template<typename T>
struct variable_arguments
{
	static int GetNParams(DWORD dwMethod, const DWORD& dwCallParams)
	{
		if(dwMethod<T::endmeths::val)
			return _GetNParams(dwMethod, dwCallParams, (T::endmeths::prev*)NULL);
		return -1;
	}
	template<typename M>
	static int _GetNParams(DWORD dwMethod, const DWORD& dwCallParams, M*)
	{
		if(dwMethod==M::val)
		{
			if(0==(M::nParam >> 16))
				return M::nParam;
			else if (dwCallParams== -1)
				return M::nParam & 0xFFFF;
			return dwCallParams > (M::nParam & 0xFFFF) ? dwCallParams : (M::nParam & 0xFFFF);
		}
		return _GetNParams(dwMethod, dwCallParams, (M::prev*)NULL);
	}
	template<>static int _GetNParams<first_def>(DWORD, const DWORD&, first_def*){return -1;}
};

// ������� ����� ��� ����������, ������� � ����� ������� ������ � ��������������
// ����������� ����������. ��� ������� ������, �������� �������������� ����������
// ����������, � �������� ���������� ������� ����������� ��� ������� ���������� ����������
// VRBL_ARG(����������� ���������� ����������)
// ��������, BL_PROC(Test, "����", VRBL_ARG(2));
// � ������-����������� ���������� ���������� ���������� ����� ������� �� m_dwCallParamCount
template<typename T, typename InitDonePolicy = reg_CreateObject<T>, BOOL bImplementRTC=TRUE>
class CContextVrblArgs: public CContextImpl<T, InitDonePolicy, bImplementRTC>
{
protected:
	mutable DWORD m_dwCallParamCount;
public:
	CContextVrblArgs(){}
	virtual int IsOleContext(void)const
	{
		DWORD regEBX = 0;
		__asm
		{
			mov regEBX, ebx // � ���� �������� 1� ������ ���������� ���������� ��� ������ ������
		}
		m_dwCallParamCount = regEBX;
		return FALSE;
	}

	virtual int GetNParams(int nMethod)const
	{
		return variable_arguments<T>::GetNParams(nMethod, m_dwCallParamCount);
	}
	
};
#define VRBL_ARG(x) (0x10000 | (x))


///////////////////////////////////////////////////////////////////////////////
// ������ ��� ����������� ���������� � ���������� ����������

// ������ ����� ������ ��� ������� "���������" ��������� (������� ������� � �������),
// � ��������� ��� ������, ��������� � ������� �������/�������, ����������� �� ����������,
// ����� ������������� ��������.
// �������������:
// ��������� ��������� -
// template<typename T>
// class CMyContext : public CDeclareCtx<CMyContext>
// {
//		typedef T ImplType;
//		... ���������� �������/�������
// };
// ���������� ���������:
// class CImplMyContext : public CDeclCtxImpl<CImplMyContext, CMyContext<CImplMyContext> ...>
// {
//		... ���������� ������� ���������.
// }

template<typename T>
class CDeclareCtx : public CContextBase
{
public:
	typedef T DeclType;
	typedef CDeclareCtx<T> BaseDeclType;

	CDeclareCtx() { static init_all_map iam; }
	~CDeclareCtx(){}
	
	
	template<typename P> struct end_def {typedef P prev;enum{val=prev::val+1};};
	// � �������� ������ ��� �������� �������� ����� ��������������, ������ �� ��������� �����/��������
	typedef end_def<first_def> endmeths;
	typedef end_def<first_def> endprops;
	
	// ���������� ����������� ������� ���������
	virtual int GetNMethods(void)const
		{return T::endmeths::val;}
	
	virtual int FindMethod(char const *name)const
		{return fndselector<T::endmeths>::type::Find(name);}
	
	virtual char const* GetMethodName(int nMethod, int nLang)const
		{return getname<T::endmeths>::type::GetName(nMethod, nLang);}
	
	virtual int HasRetVal(int nMethod)const
		{return meth_pol<T>::HasRetVal(nMethod);}
	
	virtual int GetNParams(int nMethod)const
		{return meth_pol<T>::GetNParams(nMethod);}
	
	virtual int GetNProps(void)const
		{return T::endprops::val;}
	
	virtual char const* GetPropName(int nProp, int nLang)const
		{return getname<T::endprops>::type::GetName(nProp, nLang);}
	
	virtual int FindProp(char const *name)const
		{return fndselector<T::endprops>::type::Find(name);}
	
	virtual int IsPropReadable(int nProp)const
		{return prop_pol<T>::IsPropReadable(nProp);}
	
	virtual int IsPropWritable(int nProp)const
		{return prop_pol<T>::IsPropWritable(nProp);}
	
	// ���� ����� ������, ������, �� ������ �������� � ���� �����
	// BL_BEGIN_CONTEXT("EnglishTypeName", "��������������");
	virtual char const* GetTypeString(void)const
		{return defLang ? T::GetRusTypeString() : T::GetEngTypeString();}
	
	// ������, �������� ����� ������� � �������. �������� ��� static � ������������
	// ��������� ������, �� ���� ���������� ����� ���� ��� �������� ������� �������
	struct init_all_map
	{
		init_all_map()
		{
			fndselector<T::endmeths>::type::OnInitFirstObject();
			fndselector<T::endprops>::type::OnInitFirstObject();
		}
	};
};

// ������ ��� ��������� ���������� ���������
template<typename T, typename D, typename InitDonePolicy = reg_CreateObject<T>, BOOL bImplementRTC=TRUE>
class CDeclCtxImpl : public D
{
public:
	typedef D DeclType;
	typedef CDeclCtxImpl<T, D, InitDonePolicy, bImplementRTC> BaseType;
	typedef InitDonePolicy init_done_policy;

	// ���������� RunTimeClass
	static CRuntimeClass* GetRTC()
	{
		return rtc_selector<T>::sel<bImplementRTC>::type::GetRTC();
	}
	virtual CRuntimeClass* GetRuntimeClass()const{return GetRTC();}
	
	virtual int GetParamDefValue(int nMethod, int nParam, CValue* pValue)const
		{return meth_pol<T>::GetDefVal(static_cast<const T*>(this), nMethod, nParam, pValue);}
	
	virtual int CallAsFunc(int nMethod, CValue& retVal,CValue** ppParams)
		{return meth_pol<T>::CallAsFunc(static_cast<T*>(this), nMethod, retVal, ppParams);}
	
	virtual int CallAsProc(int nMethod, CValue** ppParams)
		{return meth_pol<T>::CallAsProc(static_cast<T*>(this), nMethod, ppParams);}
	
	virtual int GetPropVal(int nProp, CValue& value)const
		{return prop_pol<T>::ReadProp(static_cast<const T*>(this), nProp, value);}
	
	virtual int SetPropVal(int nProp, CValue const& value)
		{return prop_pol<T>::WriteProp(static_cast<T*>(this), nProp, value);}
};


// ������� ��� ���������� ����������� ��������.

// �������� ����� ���� ���������.
#define BL_BEGIN_CONTEXT(parNameEng, parNameRus)\
	static LPCSTR GetRusTypeString(){return parNameRus;}\
	static LPCSTR GetEngTypeString(){return parNameEng;}\
	template<BOOL bMeth, int I>struct prev_type{typedef typename prev_type<bMeth, I-1>::type type;};\
	template<> struct prev_type<TRUE, __LINE__>{typedef context_obj::first_def type;};\
	template<> struct prev_type<FALSE, __LINE__>{typedef context_obj::first_def type;}

#define BEGIN_STRUCT(parNameE, parNameR, parMeth)\
	struct parNameE##_def\
	{\
		typedef prev_type<parMeth, __LINE__-1>::type prev;\
		enum{val=prev::val+1};\
		static LPCSTR GetEngName(){return #parNameE;}\
		static LPCSTR GetRusName(){return  parNameR;}

#define END_STRUCT(parNameE, parMeth)\
	};friend struct parNameE##_def; template<> struct prev_type<parMeth, __LINE__>{typedef parNameE##_def type;};

#define BL_METHOD_DEF(parNameEng, parNameRus, parNumParam, parCallFunc, parCallProc, parDefProc, parIsFunc, parIsDef)\
	BEGIN_STRUCT(parNameEng, parNameRus, TRUE)\
		static int CallProc(ImplType* p, CValue** pp){return parCallProc;}\
		static int CallFunc(ImplType* p, CValue& r, CValue** pp){return parCallFunc;}\
		static int GetDefVal(const ImplType* p, int n, CValue* v){return parDefProc;}\
		enum{nParam=parNumParam, IsFunc=parIsFunc, IsDefVal=parIsDef};\
		void test(){context_obj::meta_func::check_defval<nParam, IsDefVal>::type t={0};\
			context_obj::meta_func::test_last<endmeths, val>::type t2={0};}\
	END_STRUCT(parNameEng, TRUE)

#define BL_DEFVAL_FOR(parNameEng) int parNameEng##_GetDefVal(int nParam, CValue* pValue)const

#define COMMA ,

#define BL_PROC_(parNameEng,parNameRus,parNumParam, parDefVal, parIsDef)\
	BL_METHOD_DEF(parNameEng, parNameRus, parNumParam, FALSE, p->parNameEng(pp), parDefVal, 0, parIsDef)\
	int parNameEng(CValue** ppParams)

#define BL_FUNC_(parNameEng,parNameRus,parNumParam, parDefVal, parIsDef)\
	BL_METHOD_DEF(parNameEng, parNameRus, parNumParam, p->parNameEng(r COMMA pp), FALSE, parDefVal, 1, parIsDef)\
	int parNameEng(CValue& retVal, CValue** ppParams)

// �������� ����� - ��������� ���������, ��� ���������� �� ���������
#define BL_PROC(parNameEng, parNameRus, parNumParam) \
	BL_PROC_(parNameEng, parNameRus, parNumParam, FALSE, 0)

// �������� ����� - ��������� ���������, ������� ��������� �� ���������
#define BL_PROC_WITH_DEFVAL(parNameEng,parNameRus,parNumParam) \
	BL_PROC_(parNameEng, parNameRus, parNumParam, p->parNameEng##_GetDefVal(n COMMA v), 1)

// �������� ����� - ������� ���������, ��� ���������� �� ���������
#define BL_FUNC(parNameEng, parNameRus, parNumParam) \
	BL_FUNC_(parNameEng, parNameRus, parNumParam, FALSE, 0)

// �������� ����� - ������� ���������, ������� ��������� �� ���������
#define BL_FUNC_WITH_DEFVAL(parNameEng,parNameRus,parNumParam) \
	BL_FUNC_(parNameEng, parNameRus, parNumParam, p->parNameEng##_GetDefVal(n COMMA v), 1)

#define BL_PROP_(parNameE, parNameR, parGet, parSet, parR, parW)\
	BEGIN_STRUCT(parNameE, parNameR, FALSE)\
		static int ReadProp(const ImplType* p, CValue& v){return parGet;}\
		static int WriteProp(ImplType* p, CValue const& v){return parSet;}\
		enum{IsRead=parR, IsWrite=parW};\
		void test(){context_obj::meta_func::test_last<endprops, val>::type t2={0};}\
	END_STRUCT(parNameE, FALSE)

#define BL_PROP_READ(parName)	int Get_##parName(CValue& value)const
// �������� ���������� ������ �������� ��� �������� � �������/�������
#define BL_PROP_WRITE(parName)	int Set_##parName(CValue const& value)

// �������� �������� ���������, ������ ��� ������
#define BL_PROP_RO(parNameEng, parNameRus)\
	BL_PROP_(parNameEng, parNameRus, p->Get_##parNameEng(v), FALSE, 1, 0)\
	BL_PROP_READ(parNameEng)

// �������� �������� ���������, ������ ��� ������
#define BL_PROP_WO(parNameEng, parNameRus)\
	BL_PROP_(parNameEng, parNameRus, FALSE, p->Set_##parNameEng(v), 0, 1)\
	BL_PROP_WRITE(parNameEng)

// �������� �������� ���������, ������/������
#define BL_PROP_RW(parNameEng, parNameRus)\
	BL_PROP_(parNameEng, parNameRus, p->Get_##parNameEng(v), p->Set_##parNameEng(v), 1, 1)\
	BL_PROP_READ(parNameEng)


// �������� ��������� ������� � ������� ���������
#define BL_END_CONTEXT()\
	typedef end_def<prev_type<TRUE,  __LINE__>::type> endmeths;\
	typedef end_def<prev_type<FALSE, __LINE__>::type> endprops


// ����������� ����-������������� ������ ���������
#define BL_INIT_CONTEXT(parClassName)\
	__declspec (allocate(".blc$b"))\
	context_obj::P_VV init##parClassName = &parClassName::Init;\
	__declspec (allocate(".blc$d"))\
	context_obj::P_VV done##parClassName = &parClassName::Done

}//namespace context_obj
using context_obj::CContextImpl;
using context_obj::CContextVrblArgs;
using context_obj::no_init_done;
