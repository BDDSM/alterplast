// dataprovider.h
#pragma once
//#define OPTIMIZE_FIELDS_CHANGE
#include "1cpp/V7DataProvider.h"
#include "database.h"
#include "resultloader.h"

class SQLiteDataProvider;

class SQLiteDataRow : public CDataRow, public CV7DataRow
{
public:
    //CDataRow
    virtual int GetFieldCount();
    virtual int GetFieldIndexOf(LPCSTR szFieldName);
    virtual LPCSTR GetFieldName(int nIndex);
    virtual DataType GetFieldType(int nIndex);
    virtual void FormatField(int nIndex, CString& strValue);
    virtual UINT GetRowIndex();

    //CV7DataRow
    virtual const CValue& GetValue(int nIndex) const;
protected:
	friend SQLiteDataProvider;
	friend class DataProviderResultLoader;
	SQLiteDataRow(SQLiteDataProvider* pParent);
	SQLiteDataRow(DWORD fc);
	~SQLiteDataRow();
	SQLiteDataProvider* m_pParent;
	CValue* m_pValues;
	DWORD m_fCount;
};
typedef CTypedPtrArray<CPtrArray, SQLiteDataRow*> RowsArray;


class SQLiteDataProvider : public CContextImpl<SQLiteDataProvider>
	, public CDataProvider, public CV7DataProvider
{
    //CDataProvider
public:
    virtual void QueryRows(CDataRow* pRowFrom, int nRowsCount, int nPageSizeHint);
    virtual CDataRow* Fetch();
    virtual BOOL RefreshRow(CDataRow* pRowFrom);
    virtual BOOL CompareRows(CDataRow* pRow1, CDataRow* pRow2) const;
    virtual BOOL GetRowCount(UINT* pCount);

    virtual void QueryField(LPCSTR szFieldName);
    virtual void ReleaseField(LPCSTR szFieldName);

    virtual DataType GetQuickSearchType(LPCSTR szFieldName);
    virtual void QuickSearch(stQuickSearchPattern& QSPattern);
protected:
    virtual void OnAttach();
    //CV7DataProvider
public:
    virtual CV7DataRow* GetV7DataRow(CDataRow* pDataRow) const;
    virtual void GetRowValue(CDataRow* pDataRow, CValue& value) const;
    virtual CDataRow* BuildRowByValue(const CValue& value);

	// ��� SQLiteDataRow
	int findField(LPCSTR strFieldName) const
	{
		field_info* pFI;
		if(m_fieldByName.Lookup(strFieldName, pFI))
			return pFI->posInQuery;
		return -1;
	}
	int fieldsCount() const				{ return m_countOfColumns; }
	LPCSTR fieldName(int iIndex)const	{ return m_queryFieldsNames[iIndex]; }

	SQLiteDataProvider();
	~SQLiteDataProvider()
	{
		clearAll();
	}

	BL_BEGIN_CONTEXT("SQLiteDataProvider", "SQLiteDataProvider");

	BL_PROC_WITH_DEFVAL(SetDataBase, "������������", 1);
	BL_DEFVAL_FOR(SetDataBase) {if(pValue) pValue->Reset(); return TRUE; }
	
	BL_PROC_WITH_DEFVAL(SetParam, "������������������", 3)
	{
		m_sqlParams.setSqlParam(ppParams[0]->GetString(), ppParams[1], ppParams[2]->GetNumeric());
		return TRUE;
	}
	BL_DEFVAL_FOR(SetParam)
	{
		if(nParam == 2)
		{
			if(pValue)
				pValue = 0L;
			return TRUE;
		}
		return FALSE;
	}
	BL_PROC(Substitute, "�����������", 2)
	{
		m_metaParser.setTextParam(ppParams[0]->GetString(), ppParams[1]);
		return TRUE;
	}
	BL_PROC_WITH_DEFVAL(SetQueryText, "����������������������", 3);
	BL_DEFVAL_FOR(SetQueryText)
	{
		if(nParam >= 2)
		{
			if(pValue)
				pValue->Reset();
			return TRUE;
		}
		return FALSE;
	}
	BL_PROC(Debug, "�������", 1)
	{
		m_debug = (long)ppParams[0]->GetNumeric() != 0;
		return TRUE;
	}
	BL_PROC(Reread, "����������", 0)
	{
		ResetData();
		return TRUE;
	}
	BL_PROC(NoDeleteFields, "�������������", 1)
	{
		setFlagOnFields(ppParams[0]->GetString(), field_info::NoAutoDelete | field_info::inUse);
		setColumnsChanged();
		return TRUE;
	}
	BL_PROC(QuickSearchFields, "������������������", 1)
	{
		setFlagOnFields(ppParams[0]->GetString(), field_info::qsPossible);
		return TRUE;
	}
	BL_FUNC(GetQueryText, "��������������������", 1);

	BL_END_CONTEXT();
private:
	enum qTypes{qDown, qUp, qTop, qBottom, qID, qCurRow, qQuickSearch, qLast};
	
	struct sqlParamStore
	{
		struct node
		{
			node(const CValue* pVal, int m) : value(*pVal), mod(m){}
			CValue value;
			int mod;
		};
		struct node_del{void operator()(node* p){delete p;}};
		struct setter
		{
			setter(SQLiteQuery* p) : pQ(p){}
			SQLiteQuery* pQ;
			void operator()(const CString& name, node* p)
			{
				pQ->setSqlParam(CValue(name), p->value, p->mod, FALSE);
			}
		};
		
		CNoCaseMap<node*> nodes;
		DWORD dirtyQueries;

		void setSqlParam(const CString& name, const CValue* pVal, int mod)
		{
			node* pNode;
			if(nodes.Lookup(name, pNode))
			{
				pNode->value = *pVal;
				pNode->mod = mod;
			}
			else
				nodes[name] = new node(pVal, mod);
			dirtyQueries = 0xFFFFFFFF;
		}
		inline void applyParamsToQuery(SQLiteQuery* pQ, int type)
		{
			if(type >= 0 && (dirtyQueries & (1 << type)) == 0)
				return;
			nodes.ForEachPair(setter(pQ));
			if(type >= 0)
				dirtyQueries &= ~(1<<type);
		}
		~sqlParamStore()
		{
			nodes.ForEachValue(node_del());
		}
	};

	struct field_info
	{
		DWORD posInQuery, flags;
		typesOfFields type;
		CString expression, alias, nameOfQueryColumn;
		enum{
			keyField = 1, idField = 2, NoAutoDelete = 4, inUse = 8, qsPossible,
		};
	};
	typedef CArray<field_info, field_info> FieldArray;

		
	SQLiteQuery*		getQuery(qTypes type);
	void				clearRows();
	void				clearQueriesData();
	void				clearAll();
	void				bindKeyValues(SQLiteQuery* pQuery, SQLiteDataRow* pRow);
	DWORD				fieldsInfoCount() const				{ return m_fieldsInfoCount; }
	const field_info&	fieldInfo(DWORD idx) const			{ return m_fieldsInfo[idx]; }
	DWORD				keyFieldsCount() const				{ return m_keyFieldCount;}
	const CString&		keySQLParamName(DWORD idx) const	{ return m_keyParamNames[idx]; }
	DWORD				idFieldPos() const					{ return m_idFieldPos; }
	void				setColumnsChanged();
	void				setFlagOnFields(const CString& fields, int flag);

	CSLDataBase*		m_pDataBase;		// ���� ������
	SQLiteQuery*		m_queries[qLast];	// ��� ����� �������������� �������
	MetaParser			m_metaParser;		// ��� �������� ��������� �����������
	sqlParamStore		m_sqlParams;		// ��� �������� sql-����������
	RowsArray			m_rows;				// ���������� ������ ������������ �������
	LPCSTR*				m_queryFieldsNames;	// ������ ����� ������� �������� �������
	CString*			m_keyParamNames;	// ����� SQL-���������� ��� ������
	field_info*			m_fieldsInfo;		// ������ � ��������� ���� ����� �������
	DWORD				m_fieldsInfoCount;	// ���������� ���� ����� �������
	DWORD				m_keyFieldCount;	// ���������� �������� �����
	DWORD				m_idFieldPos;		// ����� ��-���� � ������ ���� �����
	DWORD				m_countOfColumns;	// ���������� ������� � ������� �������
	BOOL				m_debug;			// ���� �������
	field_info*			m_lastQSColumn;		// ��� �������� ���� ����������� ������ �������� ������
	DWORD				m_QSColumn;			// ����� ������� � ������� �������� ������, � ������� ������
	CString				m_select;			// select-���� � ������, ��������� � ������� ������
	CString				m_from;				// ����� from �������
	CString				m_where;			// ����� where �������
	CNoCaseMap<field_info*>	m_fieldByName;	// ��� ������ ���� �� ����� �������
};

class SimpleResultLoader :  public CBLContext, public ISQLiteResultLoader
{
public:
	virtual DWORD init(DWORD colCountInResult, sqlite3* pBase, CValue* pParam)
	{
		return m_count = colCountInResult;
	}
	virtual void setColumn(DWORD colIdx, const CString& colName, const CType& type){}
	virtual void assignRetValue(CValue& retVal){}

	void execute(SQLiteQuery* pQuery)
	{
		CValue me;
		me.type = 100;
		me.m_Context = this;
		pQuery->execute(&me, NULL, CValue());
		me.m_Context = NULL;
	}
protected:
	DWORD m_count;
};

class DataProviderResultLoader : public SimpleResultLoader
{
public:
	DataProviderResultLoader(SQLiteDataProvider* pProvider, RowsArray* pDest)
		:m_pProvider(pProvider), m_pDestination(pDest){}
	virtual void addValues(CValue** ppValues);
protected:
	SQLiteDataProvider* m_pProvider;
	RowsArray* m_pDestination;
};

class QuickSearchResultLoader : public SimpleResultLoader
{
public:
	QuickSearchResultLoader(const CString& p, CValue* keys, DWORD kc, DWORD tf)
		:m_pattern(p), m_pBestKey(keys), m_keysCount(kc),
		m_testField(tf), m_maxSymbols(0)
	{
	}
	virtual void addValues(CValue** ppValues);

	int find(SQLiteQuery* pQuery)
	{
		IncrRef();
		try{ execute(pQuery); } catch(int){}
		return m_maxSymbols;
	}

protected:
	const CString& m_pattern;
	CValue* m_pBestKey;
	int m_maxSymbols;
	DWORD m_keysCount, m_testField;
};
