#ifndef SIMPLE_CMN_H_INCLUDED
#define SIMPLE_CMN_H_INCLUDED

#include <exception>
#include "SQLITE1C/resultloader.h"

// flags
enum sc_flags
{
	scf_Readonly =	0x00000000,
	scf_Modify =	0x00000001,
	scf_Insert =	0x00000002,
	scf_Move =		0x00000004
};

// exceptions
struct sc_exception : public std::exception
{
	sc_exception(const char* szWhat = "������ ��� ���������� ������!") : std::exception(szWhat) {}
};

// conversions
namespace CSC_CMN
{
	typedef boost::shared_ptr<CValue> CValuePtr;

	template <typename REFTYPE>
	static void AssignItem(CValue& RetVal, REFTYPE const& Item)
	{
		RetVal = Item;
	}

	template <>
	static void AssignItem(CValue& RetVal, CBLContext* const& Item)
	{
		RetVal.AssignContext(Item);
	}

	template <>
	static void AssignItem(CValue& RetVal, CValuePtr const& Item)
	{
		RetVal = *Item;
	}

	template <typename ITEMTYPE>
	static ITEMTYPE GetItem(CValue const& val)
	{
		return static_cast<ITEMTYPE>(val);
	}

	template <>
	static int GetItem(CValue const& val)
	{
		if (val.GetTypeCode() == NUMBER_TYPE_1C)
			return (int)val.GetNumeric();
		else
			throw sc_exception("�������� ��� �������� (��������� �����)!");
	}

	template <>
	static LPCSTR GetItem(CValue const& val)
	{
		if (val.GetTypeCode() == STRING_TYPE_1C)
			return (LPCSTR)val.m_String;
		else
			throw sc_exception("�������� ��� �������� (��������� ������)!");
	}

	template <>
	static CBLContext* GetItem(CValue const& val)
	{
		if (val.GetTypeCode() == AGGREGATE_TYPE_1C)
			return val.GetContext();
		else
			throw sc_exception("�������� ��� �������� (��������� ��������)!");
	}

	template <>
	static CValuePtr GetItem(CValue const& val)
	{
		return CValuePtr(new CValue(val));
	}

	template <typename T>
	static void AssignEmpty(CValue& RetVal)
	{
		RetVal = T(0);
	}

	template <>
	static void AssignEmpty <LPCSTR> (CValue& RetVal)
	{
		RetVal = "";
	}

	static void LoaderDone(IResultLoader* p) { p->ldrDone(); }
	static void PublisherDone(IResultPublisher* p) { p->pubDone(); }

	static void DataExchange(IResultPublisher* pPublisher, IResultLoader* pLoader)
	{
		if (!pPublisher)
			throw sc_exception("�������� ��� ���������!");

		if (!pLoader)
			throw sc_exception("�������� ��� ���������!");

		// ��� ������-�������� ����� �������� ����� ���� ������������
		if (dynamic_cast<IResultLoader*>(pPublisher) == pLoader)
			throw sc_exception("�������� �� ����� ������������ �������� ����������!");

		DWORD nColumns = 0;
		DWORD nRows = 0;

		boost::shared_ptr<IResultPublisher> PublisherGd(pPublisher, &PublisherDone);
		if (FAILED(pPublisher->pubInit(nColumns, nRows)))
			throw sc_exception(pPublisher->GetLastError());

		boost::shared_ptr<IResultLoader> LoaderGd(pLoader, &LoaderDone);
		if (FAILED(pLoader->ldrInit(nColumns, nRows)))
			throw sc_exception(pLoader->GetLastError());

		for(int i = 0; i < nColumns; ++i)
		{
			LPCSTR szColName;
			CType* pType;
			if (FAILED(pPublisher->pubGetColumn(i, szColName, pType)))
				throw sc_exception(pPublisher->GetLastError());
			if (FAILED(pLoader->ldrAddColumn(szColName, pType)))
				throw sc_exception(pLoader->GetLastError());
		}

		for (;;)
		{
			CValue const* const* ppValues;
			HRESULT hr = pPublisher->pubGetRow(ppValues);
			if (FAILED(hr))
				throw sc_exception(pPublisher->GetLastError());
			else if (hr != NO_ERROR)
				break;
			hr = pLoader->ldrAddRow(ppValues);
			if (FAILED(hr))
				throw sc_exception(pLoader->GetLastError());
			else if (hr != NO_ERROR)
				break;
		}
	}
};
#endif
