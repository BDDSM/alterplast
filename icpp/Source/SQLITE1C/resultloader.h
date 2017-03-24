#ifndef RESULT_LOADER_H_INCLUDED
#define RESULT_LOADER_H_INCLUDED

struct sqlite3;

// ��������� ��� �������� ���������� ������� � ������-��������
class ISQLiteResultLoader
{
public:
	virtual ~ISQLiteResultLoader(){}
	// ���������� ��������� � ������ �����������
	// ������ ���������� ���������� �������, ������� ����� ������� ��������.
	virtual DWORD init(DWORD colCountInResult, sqlite3* pBase, CValue* pParam) = 0;
	// ���������� ������� � ��������� �������
	virtual void setColumn(DWORD colIdx, const CString& colName, const CType& type) = 0;
	// ���������� ������ � ��������� �������.
	// �������� - ������ ���������� �� CValue, ����������� ������ ����������
	// ����������� �������
	virtual void addValues(CValue** ppValues) = 0;
	// ���������� ������������� ��������
	virtual void assignRetValue(CValue& retVal) = 0;
};

class IVTResultLoader : public ISQLiteResultLoader
{
public:
	IVTResultLoader() : m_pCont(NULL) {}
	~IVTResultLoader() { if(m_pCont) m_pCont->DecrRef(); }

	// ���������� ��������� � ������ �����������
	virtual DWORD init(DWORD colCountInResult, sqlite3*, CValue* pClear)
	{
		if(pClear->IsEmpty())
		{
			m_pTable->Clear(TRUE);
			return colCountInResult;
		}
		else if(1 == (long)pClear->GetNumeric())
			m_pTable->Clear(FALSE);
		DWORD colInTables = m_pTable->GetColumnCount();
		return colInTables < colCountInResult ? colInTables : colCountInResult;
	}
	// ���������� ������� � ��������� �������
	virtual void setColumn(DWORD colIdx, const CString& colName, const CType& type)
	{
		if(m_pTable->GetColumnCount() <= colIdx)
			m_pTable->AddColumn(colName, type, colName, 0, CString(), 0);
	}
	// ���������� ������ � ��������� �������.
	// �������� - ������ ���������� �� CValue, ����������� ������ ����������
	// ����������� �������
	virtual void addValues(CValue** ppValues)
	{
		m_pTable->AddRow(ppValues, -1);
	}
	// ���������� ������������� ��������
	virtual void assignRetValue(CValue& retVal)
	{
		retVal.AssignContext(m_pCont);
	}
	void setContext(CBLContext* pCont)
	{
		m_pCont = pCont;
		m_pCont->IncrRef();
		m_pTable = ((CValueTableContextData*)pCont->GetInternalData())->GetValueTable();
	}
protected:
	CBLContext* m_pCont;
	CValueTable* m_pTable;
};

class IVLResultLoader : public ISQLiteResultLoader
{
public:
	IVLResultLoader() : m_pCont(NULL) {}
	~IVLResultLoader()	{ if(m_pCont) m_pCont->DecrRef(); }
	virtual DWORD init(DWORD colCountInResult, sqlite3*, CValue* pClear)
	{
		if(pClear->IsEmpty())
		{
			CValueItem** ppItems = (CValueItem**)m_pList->GetData();
			for(DWORD c = m_pList->GetSize(); c--;)
				delete *ppItems++;
			m_pList->RemoveAll();
		}
		return m_colCount = colCountInResult;
	}
	// ���������� ������� � ��������� �������
	virtual void setColumn(DWORD, const CString&, const CType&)
	{
	}
	// ���������� ������ � ��������� �������.
	// �������� - ������ ���������� �� CValue, ����������� ������ ����������
	// ����������� �������
	virtual void addValues(CValue** ppValues)
	{
		DWORD s = m_pList->GetSize(), count = m_colCount;
		m_pList->SetSize(s + count);
		CValueItem** ppWriteValues = (CValueItem**)(m_pList->GetData()) + s;
		while(count--)
			*ppWriteValues++ = new CValueItem(**ppValues++, CString());

	}
	// ���������� ������������� ��������
	virtual void assignRetValue(CValue& retVal)
	{
		retVal.AssignContext(m_pCont);
	}
	
	void setContext(CBLContext* pCont)
	{
		m_pCont = pCont;
		m_pCont->IncrRef();
		m_pList = *(CPtrArray**)(((char*)m_pCont) + 0x30);
	}
protected:
	CBLContext* m_pCont;
	CPtrArray* m_pList;
	DWORD m_colCount;
};

class IScalarResultLoader : public ISQLiteResultLoader
{
public:
	virtual DWORD init(DWORD, sqlite3*, CValue*)
	{
		return m_column + 1;
	}
	// ���������� ������� � ��������� �������
	virtual void setColumn(DWORD, const CString&, const CType&)
	{
	}
	// ���������� ������ � ��������� �������.
	// �������� - ������ ���������� �� CValue, ����������� ������ ����������
	// ����������� �������
	virtual void addValues(CValue** ppValues)
	{
		m_value = *ppValues[m_column];
	}
	// ���������� ������������� ��������
	virtual void assignRetValue(CValue& retVal)
	{
		retVal = m_value;
	}
	void setCol(DWORD col) {m_column = col;}
protected:
	DWORD m_column;
	CValue m_value;
};

class IInterface
{
public:
	HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject) { return E_NOINTERFACE; };
	ULONG __stdcall AddRef() { return 0; };
	ULONG __stdcall Release() { return 0; };
};

class IErrorDescription : public IInterface
{
public:
	// �������� ������
	// ����� ����� ���� ������ ��������������� ����� ������������� ����������� ������
	// ������������ ��������� ������������ �� ���������� ������ ������ ������ �������, ������������ IErrorDescription
	virtual LPCSTR GetLastError() throw() = 0;
};

// ���� � ������� ��������� ������:
// S_OK:
// - ������: ��� ������
// - �������� ����������: ����������� �������� ������
// x >= S_FALSE:
// - ������: ���������� ����������� ���������� �������� ������
// - �������� ����������: ����������� �������� ������ � ������� ������
// FAILED(x) != FALSE
// - ������: ���������� ���������� ���������� �������� ������ (����������� ������)
// - �������� ����������: ����������� �������� ������ � ��������� ������, ��������� ���� ������, ������ ����������

class IResultLoader : public IInterface, public IErrorDescription
{
public:
	// ���������� ��������� � ������ �����������
	//
	// �������� �������� � nColCount ���������� ������� � ������ ������ (0 < nColCount)
	// �������� ���������� � nColCount ���������� �������, ������� �� ����� ������� (0 < nColCount_ret <= nColCount)
	//
	// �������� �������� � nRowCount ���������� ����� � ������ ������ ( 0 <= nRowCount)
	// nRowCount == 0 ��������, ��� ���������� ����� � ������ ������ �� ����������
	// �������� ���������� � nRowCount ���������� �����, ������� �� ����� ������� (0 <= nRowCount_ret)
	// nRowCount_ret == 0 ��������, ��� ����� �����, ������� ����� ������� ��������, �� ����������
	//
	// ���� �������� �� ����� ������� ������, �� ���������� ��� ������
	//
	// nRowCount ������������ ������ ��� ����������� �������������; �������� ���������� �����
	// �������� � �������� ������������ ��������������, �� ��������� �� ������ �������
	virtual HRESULT ldrInit(DWORD& nColCount, DWORD& nRowCount) throw() = 0;

	// ���������� ����������
	// �������������� ���������� ��� ���������, �������� � Init
	// ��� ������ ������ ���� S_OK
	virtual HRESULT ldrDone() throw() = 0;

	// ���������� ������� � ��������� �������
	// szColName can be NULL (��� ������������), pType can be NULL (�� ����� ���)
	virtual HRESULT ldrAddColumn(LPCSTR szColName, CType const* pType) throw() = 0;

	// ���������� ������ � ��������� �������
	// �������� - ������ ���������� �� CValue, ����������� ������ ���������� ����������� �������
	virtual HRESULT ldrAddRow(CValue const* const* ppValues) throw() = 0;
};

class IResultPublisher : public IInterface, public IErrorDescription
{
public:
	// ���������� ��������� � ���������� �����������
	//
	// �������� �������� � nColCount ��������� ���������� ������� � ������ ������ (0 <= nColCount)
	// nColCount == 0 ��������, ��� ���������� �������, ������� ����� ������� ��������, �� ����������
	// �������� ���������� � nColCount ���������� �������, ������� �� ����� �������� (0 < nColCount_ret <= nColCount)
	//
	// �������� �������� � nRowCount ��������� ���������� ����� � ������ ������ ( 0 <= nRowCount)
	// nRowCount == 0 ��������, ��� ���������� ����� � ������ ������ �� ����������
	// �������� ���������� � nRowCount ���������� �����, ������� �� ����� �������� (0 <= nRowCount_ret)
	// nRowCount == 0 ��������, ��� ����� �����, ������� ����� �������� ��������, �� ����������
	//
	// ���� �������� �� ����� �������� ������, �� ���������� ��� ������
	//
	// nRowCount ������������ ������ ��� ����������� �������������; �������� ���������� �����
	// �������� � �������� ������������ ��������������, �� ��������� �� ������ �������
	virtual HRESULT pubInit(DWORD& nColCount, DWORD& nRowCount) throw() = 0;

	// ���������� ����������
	// �������������� ���������� ��� ���������, �������� � Init
	// ��� ������ ������ ���� S_OK
	virtual HRESULT pubDone() throw() = 0;

	// ��������� ���������� � �������
	// szColName can be NULL (��� ������������), pType can be NULL (�� ����� ���)
	// (0 <= nIndex < nColCount_ret)
	virtual HRESULT pubGetColumn(DWORD nIndex, LPCSTR& szColName, CType*& pType) throw() = 0;

	// ��������� ������
	// �������� - ������ ���������� �� CValue, ����������� ������ ���������� ������������� ������� (nColCount_ret)
	// ���� �������� �� ����� �������� ������, �� ���������� ��� ������
	virtual HRESULT pubGetRow(CValue const* const*& ppValues) throw() = 0;
};

#endif
