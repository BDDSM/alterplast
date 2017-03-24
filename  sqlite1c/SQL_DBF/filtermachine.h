// filtermachine.h
#pragma once

enum OpCodes
{
	stop,
	// ���������� ������ ������
	selectIdx,
	// ������ ����������
	getArgText,
	getArgNumeric,
	getArgLong,
	// ������ � ��������� =
	setEqualString,
	setEqualNumeric,
	setEqualLong,
	
	cmpStringEqual_Equal,
	cmpNumericEqual_Equal,
	cmpLongEqual_Equal,

	// ������ � ��������� >
	cmpString_Min_Equal,
	cmpNumeric_Min_Equal,
	cmpLong_Min_Equal,
	
	setMinString,
	setMinNumeric,
	setMinLong,
	
	selMinString,
	selMinNumeric,
	selMinLong,

	// ������ � ��������� >=
	cmpString_MinEq_Equal,
	cmpNumeric_MinEq_Equal,
	cmpLong_MinEq_Equal,
	
	setMinEqString,
	setMinEqNumeric,
	setMinEqLong,
	
	selMinEqString,
	selMinEqNumeric,
	selMinEqLong,

	// ������ � ��������� <
	cmpString_Max_Equal,
	cmpNumeric_Max_Equal,
	cmpLong_Max_Equal,

	cmpString_Max_Minimum,
	cmpNumeric_Max_Minimum,
	cmpLong_Max_Minimum,

	setMaxString,
	setMaxNumeric,
	setMaxLong,

	selMaxString,
	selMaxNumeric,
	selMaxLong,

	// ������ � ��������� <=
	cmpString_MaxEq_Equal,
	cmpNumeric_MaxEq_Equal,
	cmpLong_MaxEq_Equal,

	cmpString_MaxEq_Minimum,
	cmpNumeric_MaxEq_Minimum,
	cmpLong_MaxEq_Minimum,

	setMaxEqString,
	setMaxEqNumeric,
	setMaxEqLong,

	selMaxEqString,
	selMaxEqNumeric,
	selMaxEqLong,
	
	// ����� ���������� ��� ������� ��� ������ ���� �������
	setFullKeyEqual,
	setFullKeyMinimum,
	setFullKeyMaximum,

	setPartKeyEqualText,
	setPartKeyEqualNumeric,
	setPartKeyEqualLong,

	setPartKeyMinimumText,
	setPartKeyMinimumNumeric,
	setPartKeyMinimumLong,
	
	setPartKeyMaximumText,
	setPartKeyMaximumNumeric,
	setPartKeyMaximumLong,

	// ��������� ����� ������ ��� �������� �� ������ ������
	storeRecNo,
	
	// ����� ���������� ��� ������� ��� ������ ����, �� ��������� � ������
	addTestEqualText,
	addTestEqualNumeric,
	addTestEqualLong,
	addTestEqualRecNo,

	addTestMinimumText,
	addTestMinimumNumeric,
	addTestMinimumLong,
	addTestMinimumRecNo,

	addTestMaximumText,
	addTestMaximumNumeric,
	addTestMaximumLong,
	addTestMaximumRecNo,



//// ����� ���������� � ��������� �����
/*
����� ����������� �����.
�������� ��������:
����� ���-������ ����
����� ������� ������ ����
����� ���-��������� ����
����� ������� ��������� ����
����� ���-������ ���� � ���-��������� ����
����� ���-������ ���� � ������� ��������� ����
����� ������� ������ ���� � ���-��������� ����
����� ������� ������ ���� � ������� ��������� ����.
����� ���������� ������ ����������, ��� �� ����� ����� �������������� ��� ���-����
�������� ������ - ���-������ ����, �������-������ ����, ���-��������� ����, ������� ��������� ����.
*/
	// ���������� ����������� ���� ������ ����������� �������
	setMinKey_FKMin,
	// ���������� ����������� ���� ������ �������� �������
	setMinKey_FKEqual,
	// ���������� ����������� ���� ������ ����������� ����������
	setMinKey_PKMin,
	// ���������� ����������� ���� ������ �������� ����������
	setMinKey_PKEqual,
	// ������� ����������� ���� �� ����������� ������� � ����������� ����������
	selMinKey_FKMin_PKMin,
	// ������� ����������� ���� �� ����������� ������� � �������� ����������
	selMinKey_FKMin_PKEqual,
	// ������� ����������� ���� �� �������� ������� � ����������� ����������
	selMinKey_FKEqual_PKMin,
	// ������� ����������� ���� �� �������� ������� � �������� ����������
	selMinKey_FKEqual_PKEqual,
// ����� ����������� ����� - ���������� �����������.
	// ���������� ������������ ���� ������ ������������� �������
	setMaxKey_FKMax,
	// ���������� ������������ ���� ������ �������� �������
	setMaxKey_FKEqual,
	// ���������� ������������ ���� ������ ������������� ����������
	setMaxKey_PKMax,
	// ���������� ������������ ���� ������ �������� ����������
	setMaxKey_PKEqual,
	// ������� ������������ ���� �� ����������� ������� � ����������� ����������
	selMaxKey_FKMax_PKMax,
	// ������� ������������ ���� �� ����������� ������� � �������� ����������
	selMaxKey_FKMax_PKEqual,
	// ������� ������������ ���� �� �������� ������� � ����������� ����������
	selMaxKey_FKEqual_PKMax,
	// ������� ������������ ���� �� �������� ������� � �������� ����������
	selMaxKey_FKEqual_PKEqual,

// ��������� ���� � ��� ������.
// ���� �������� ��������� � ����� ������, � ���� �� ��� ����� ������� ��� ����� �� ���� ���������
// ������, �� ���� ��������� �������������� �����, ��� ��� �� ���� ������ ��������
// �� ��� ���� ������ ���� �� ������ �������������. ���� � ����� ������ ��������� ��������������
// ����� ������ ����, �� ����� �������� ������ �� ����, ��� ��� ��� ���������� ��� ����������
// ������.
	cmpMinMaxKeys,

//// �������
	// ���� �� ������ �� �����
	gotoFirstToLast,
	// ���� �� ������ �� ������������� �����
	gotoFirstToMax,
	// ���� �� ������������ ����� �� �����
	gotoMinToLast,
	// ���� �� ������������ ����� �� ��������� �����
	gotoMinToMax,
	// ���� �� ����� �� ������
	gotoLastToFirst,
	// ���� �� ��������� ����� �� ������
	gotoMaxToFirst,
	// ���� �� ����� �� ������������ �����
	gotoLastToMin,
	// ���� �� ��������� ����� �� ������������ �����
	gotoMaxToMin,
	// ������� �� ������ ������
	gotoToRow,

	lastOpCode
};

struct cursor_data;
class select_iterator;
class CVtabInfo;

struct testConstraint
{
	virtual BOOL test(const cursor_data& cursor) = 0;
	virtual ~testConstraint() { delete next; }

	static BOOL doTest(testConstraint* ptr, const cursor_data& cursor)
	{
		while(ptr)
		{
			if(!ptr->test(cursor))
				return FALSE;
			ptr = ptr->next;
		}
		return TRUE;
	}
	testConstraint(testConstraint*& p) : next(p) {p = this;}
	static void append(testConstraint*& ret, testConstraint*& n1, testConstraint*& n2)
	{
		ret = n1;
		n1->next = n2;
		n2 = NULL;
		n1 = NULL;
	}
protected:
	testConstraint* next;
};


class FilterMachine
{
public:
	FilterMachine(const CVtabInfo& t, cursor_data& c)
		: m_pPKEqConstr(NULL), m_pPKMinConstr(NULL), m_pPKMaxConstr(NULL), m_pAdditionalTests(NULL),
		m_table(t), m_cursor(c)
	{
		m_hardCurMin =
		m_hardCurMax =
		m_hardFKMin  =
		m_hardFKMax  =
		m_hardPKMin  =
		m_hardPKMax  =
		m_hardTSMin  =
		m_hardTSMax  =
		m_minKType   =
		m_maxKType   = 0;
	}
	~FilterMachine()
	{
		delete m_pPKEqConstr;
		delete m_pPKMinConstr;
		delete m_pPKMaxConstr;
		delete m_pAdditionalTests;
	}
	
	select_iterator* process(const unsigned char* pByteCode, sqlite3_value** args);

	static void build(const class CVtabInfo& table, sqlite3_index_info* pIdx, const struct idx_node* pBest);
	static class select_iterator* parseFilter(struct cursor_data&, const class CVtabInfo&, const char*, sqlite3_value**);
protected:

	enum keySrc {keyFKEq, keyFKnEq, keyPKEq, keyPKnEq};

	select_iterator* GotoFirstToLast();
	select_iterator* GotoFirstToMax();
	select_iterator* GotoMinToLast();
	select_iterator* GotoMinToMax();
	select_iterator* GotoLastToFirst();
	select_iterator* GotoMaxToFirst();
	select_iterator* GotoLastToMin();
	select_iterator* GotoMaxToMin();
	select_iterator* GotoToRow();

	CString m_crntEqualText;
	CString m_crntMinimumText;
	CString m_crntMaximumText;

	double m_crntEqualNum;
	double m_crntMinimumNum;
	double m_crntMaximumNum;

	__int64 m_crntEqualLong;
	__int64 m_crntMinimumLong;
	__int64 m_crntMaximumLong;

	long m_recNo;
	
	CString m_fullKeyMin;
	CString m_fullKeyEq;
	CString m_fullKeyMax;
	CString m_partKeyMin;
	CString m_partKeyEq;
	CString m_partKeyMax;
	
	testConstraint* m_pPKEqConstr;
	testConstraint* m_pPKMinConstr;
	testConstraint* m_pPKMaxConstr;
	testConstraint* m_pAdditionalTests;

	const CVtabInfo& m_table;
	cursor_data& m_cursor;

	DWORD m_hardCurMin: 1;
	DWORD m_hardCurMax: 1;
	DWORD m_hardFKMin : 1;
	DWORD m_hardFKMax : 1;
	DWORD m_hardPKMin : 1;
	DWORD m_hardPKMax : 1;
	DWORD m_hardTSMin : 1;
	DWORD m_hardTSMax : 1;
	DWORD m_minKType  : 2;
	DWORD m_maxKType  : 2;

	const struct index_info* m_pIndexInfo;
	const struct idx_field_info* m_pFields;
	DWORD m_dwCurIdx;
};
