// phisicalinfo.h
#pragma once


struct field_info
{
	enum field_cmp_type {ftText, ftNumeric};

	field_cmp_type type()	const { return (field_cmp_type)t; }
	DWORD length()			const { return l; }
	DWORD precession()		const { return p; }
	DWORD offsetInRecord()	const { return o; }

private:
	friend struct phisical_info;
	DWORD l	: 10;
	DWORD p	: 4;
	DWORD t	: 2;
	DWORD o	: 16;
};

struct idx_field_info
{
	DWORD numInTable() const {return dwNumInTable;}
	const field_info& info() const {return fInfo;}
private:
	friend struct phisical_info;
	field_info fInfo;
	DWORD dwNumInTable;
};

struct index_info
{
	DWORD fieldsCount()		const { return c; }
	DWORD keySize()			const { return s; }
	const idx_field_info* fields() const { return fi;}

private:
	friend struct phisical_info;
	DWORD c : 16;
	DWORD s : 16;
	idx_field_info fi[1];
};

// ��������� ��������� ���������� ��������� �������.
// ��� ���������:
// - �� ������ ���� ������� ����������, � ����� ������� ��� ������
//   � ������� ����� ���� � �������.
// - �� ������ ������� ������� ����������� ������ ����� �������,
//   �������� � ���� ������.
// - �������� ����� ������ ������ �������, ������ ������,
// - �������� � ����� ����� � ������.
struct phisical_info
{
	DWORD fieldsCount()				const {return fldCount;}
	DWORD indexesCount()			const {return idxCount;}
	const field_info& field(DWORD pos)	const {return m_pFields[pos];}
	const index_info* index(DWORD pos)	const {return m_ppIndexes[pos];}
	// �������� ����� ������ �������, � ������� ������ 1� ��������
	// ������ ������ ����� �������������������� ������.
	// ����� ����� ������ ���� �� �������, �� ���� ���� ���� ��� ��������
	// CStoreObj, �� ����� ��� ������ �������� ������������ �� ������ ������,
	// ������ ����� ���������� ������ �� ����� ������.
	const char* tableReadBufer()	const {return pTableBuf;}
	// ������ ������ ������ �������.
	DWORD recordSize()				const {return recSize;}

	// �������� ��� ���������, � ����� ���������� ������� ������ ���������� ����
	// � ������� ��������� � �������
	struct fld2idxIt
	{
		operator BOOL() {return ptr < end;}
		DWORD idxNum()	{return (*ptr) & 0xFFFF;}
		DWORD fldNum()	{return (*ptr) >> 16;}
		void operator++() {ptr++;}
	private:
		friend phisical_info;
		fld2idxIt(const DWORD* s, const DWORD* e) : ptr(s), end(e){}
		const DWORD *ptr, *end;
	};

	
	fld2idxIt fld2idx(DWORD fld)	const
	{
		DWORD d = m_pFields2Index[fld];
		const DWORD *pS = (DWORD*)m_pFields + LOWORD(d), *pE = pS + HIWORD(d);
		return fld2idxIt(pS, pE);
	}
	
private:
	friend class CVtabInfo;

	phisical_info(){}

	static phisical_info* buildInfo(CTableEx* pTable);

	const char* pTableBuf;
	DWORD recSize;
	DWORD fldCount;
	DWORD idxCount;
	DWORD* m_pFields2Index;
	index_info** m_ppIndexes;
	field_info m_pFields[1];
	//[field_info * nFields] [DWORD * nFields] [index_inf0* * nIndexes] [fld2idxIt * fldInIdx] [index_info~ * nIdexes]
};
