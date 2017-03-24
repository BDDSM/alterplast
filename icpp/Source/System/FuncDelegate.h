//////////////////////////////////////////////////////////////////////
//
//	FuncDelegate.h
//	Implementation of CFuncDelegate class
//	Design and code: artbear (����� �������)
//	Contacts: EMAIL artbear@inbox.ru
//
//	��������� ��������� �������-���������
//		CFuncDelegate0, CFuncDelegate1, CFuncDelegate2, CFuncDelegate3
//
//	��� ��������� �������� ��������� ����������� ������� (Add, Remove)
//	������ � ������� ���������� ����� Invoke
//	��� ������� ������ ����� Invoke ��������� �������������� ����� ����������
//
//	������ ������ ������������ ��� ��������� ������� ��� ������ ������������� �������
//	�� �������� ������ � ��� ������������.
//	�.�. ����������� ������ ������������ ������ � ����������� ���������� ����������
//	������ ������ ����������� � ����������, ������� ���� �������-�����������
//	� ������ ������������� ������� ��������� �� ������� � ������� ���������� ������� ������ �������-����������
//
//	�����:
//		1. ������������ �������� ����� �� ������������
//			����� �������, �� ����� ����. ������, ������ �������� � �������-����������� ��� ���� ��������
//			�� ��� ������� ������ ��������� ������������ ���� �������� (���� ���� ����� 0, ������, 1 ��� ������� ������)
//		2. ������ �������� ������� ������, ��� ����������� ���������� ������, ���� �� ���� ���������� !
//			�� ����� �� ����� � ������ ������� ������������� ����� �������� (���� ����� 0, ����� ������ ��������)
//
//	������ �������������:
//		typedef int(CBLModule::*PF_FF)(char const*); // ��� �������
//		// ������� ��� ������, ����� ��� �������, ����� ��������� �������
// 		CFuncDelegate1<CBLModule, PF_FF, char const*> Delegate; // �������
// 		bool b = Delegate.IsEmpty();
//
// 		Delegate.Add(CMyModule1::func1);
// 		Delegate.Add(CMyModule2::func2);
//
// 		CBLModule pMod = this; // ������-�������� �������������� ������
// 		Delegate.Invoke(pMod, "��������������"); // ������� func1 ����� func2
//
//  	Delegate.Remove(CMyModule1::func1);
// 		Delegate.Add(CMyModule1::func1);
//
// 		Delegate.Invoke(pMod, "��������������"); // ������� func2 ����� func1
// 		Delegate.Clear();
//
//
//////////////////////////////////////////////////////////////////////

#ifndef FUNCDELEGATE_H
#define FUNCDELEGATE_H

#pragma once

#include <list>
#include <algorithm>

using namespace std;

template<class TClass, class TFunction>
class CFuncDelegate0
{
public:
	void Add(TFunction func);
	void Remove(TFunction func);

	bool IsEmpty() const;
	void Clear(void);

	void Invoke(const TClass& object);
protected:
	list<DWORD> m_list; // ���� ������� ����� TFunction - �� ����������, ���� ������ � �������� :(
};

template<class TClass, class TFunction, typename Arg1>
class CFuncDelegate1 : public CFuncDelegate0<TClass, TFunction>
{
public:
	void Invoke(const TClass& object, Arg1& arg1);
};

template<class TClass, class TFunction, class Arg1, class Arg2>
class CFuncDelegate2 : public CFuncDelegate0<TClass, TFunction>
{
public:
	void Invoke(const TClass& object, Arg1& arg1, Arg2& arg2);
};

template<class TClass, class TFunction, class Arg1, class Arg2, class Arg3>
class CFuncDelegate3 : public CFuncDelegate0<TClass, TFunction>
{
public:
	void Invoke(const TClass& object, Arg1& arg1, Arg2& arg2, Arg2& arg3);
};

template<class TClass, typename TFunction>
inline void CFuncDelegate0<TClass, TFunction>::Add(TFunction func)
{
	DWORD pFunc = *reinterpret_cast<DWORD*>(&func);
	if (m_list.end() == find(m_list.begin(), m_list.end(), pFunc))
		m_list.push_back(pFunc);
}

template<class TClass, typename TFunction>
inline void CFuncDelegate0<TClass, TFunction>::Remove(TFunction func)
{
	DWORD pFunc = *reinterpret_cast<DWORD*>(&func);
	m_list.remove(pFunc);
}

template<class TClass, typename TFunction>
inline bool CFuncDelegate0<TClass, TFunction>::IsEmpty() const
{
	return m_list.empty();
}

template<class TClass, typename TFunction>
inline void CFuncDelegate0<TClass, TFunction>::Clear(void)
{
	m_list.clear();
}

template<class TClass, typename TFunction>
inline void CFuncDelegate0<TClass, TFunction>::Invoke(const TClass& object)
{
	list<DWORD>::const_iterator iter = m_list.begin();
	for (;iter != m_list.end(); ++iter)
	{
 		 // �������
 		TFunction pFunc = *reinterpret_cast<TFunction*>((void*)&*iter);
 		(object.*pFunc)();
	}
}

template<class TClass, class TFunction, class Arg1>
inline void CFuncDelegate1<TClass, TFunction, Arg1>::Invoke(const TClass& object, Arg1& arg1)
{
	list<DWORD>::const_iterator iter = m_list.begin();
	for (;iter != m_list.end(); ++iter)
	{
		 // �������
		TFunction pFunc = *(TFunction*)(void*)(&*iter);
		(object.*pFunc)(arg1);
	}
}

template<class TClass, class TFunction, class Arg1, class Arg2>
inline void CFuncDelegate2<TClass, TFunction, Arg1, Arg2>::Invoke(const TClass& object, Arg1& arg1, Arg2& arg2)
{
	list<DWORD>::const_iterator iter = m_list.begin();
	for (;iter != m_list.end(); ++iter)
	{
		 // �������
		TFunction pFunc = *(TFunction*)(void*)(&*iter);
		(object.*pFunc)(arg1, arg2);
	}
}

template<class TClass, class TFunction, class Arg1, class Arg2, class Arg3>
inline void CFuncDelegate3<TClass, TFunction, Arg1, Arg2, Arg3>::Invoke(const TClass& object, Arg1& arg1, Arg2& arg2, Arg2& arg3)
{
	list<DWORD>::const_iterator iter = m_list.begin();
	for (;iter != m_list.end(); ++iter)
	{
		 // �������
		TFunction pFunc = *(TFunction*)(void*)(&*iter);
		(object.*pFunc)(arg1, arg2, arg3);
	}
}


#endif