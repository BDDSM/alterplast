//////////////////////////////////////////////////////////////////////
// ����� CVTExtRow ��� ����������������������
//
// �����: ����� ������� aka ADirks
// ��������: e-mail: adirks@ngs.ru, ICQ: 13472890
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VTExtended.h"


HANDLE CVTExtHeap::hHeap = NULL;
static CVTExtHeap Heap;

CVTExtHeap::CVTExtHeap()
{
	if( !hHeap )
	{
		hHeap = HeapCreate(HEAP_NO_SERIALIZE, 0, 0);
	}
}

CVTExtHeap::~CVTExtHeap()
{
	if( hHeap )
		HeapDestroy(hHeap);
}


CValue* CVTExtRow::AllocMem(int nNewSize)
{
	// ���� �����, ����� ���� �����
	// �������������� ������� ��� ��������� ���������� �������
	#define EXESSIVE_ROWSIZE_HINT 2

	CValue *pV, *ptr;
	if( nNewSize )
	{
		HANDLE hHeap = CVTExtHeap::Get();
		pV = (CValue*) HeapAlloc(hHeap, HEAP_NO_SERIALIZE, sizeof(CValue) * (nNewSize + EXESSIVE_ROWSIZE_HINT));

		if ( pV )
		{
			ptr = pV;
			for ( int i =0; i < nNewSize; i++ )
				new( ptr++ ) CValue; //������������� �������� �����������
		}
		else
			RuntimeError("Error! Cannot allocate memory block");
	}
	else
		pV = NULL;

	return pV;
}

void CVTExtRow::FreeMem(CValue* pV, int nCurrentSize)
{
	if( pV )
	{
		CValue *ptr = pV;
		for( int i = 0; i < nCurrentSize; i++ )
			(ptr++)->~CValue();

		HANDLE hHeap = CVTExtHeap::Get();
		HeapFree(hHeap, HEAP_NO_SERIALIZE, (LPVOID)pV);
	}
}

// ��������� ������� ��� ���������� ������� ������
CValue* CVTExtRow::ReAllocMem(CValue* pCurrentV, int nCurrentSize, int nNewSize)
{
	CValue *pV, *ptr;
	if( pCurrentV && nNewSize )
	{
		HANDLE hHeap = CVTExtHeap::Get();

		if( HeapSize(hHeap, HEAP_NO_SERIALIZE, pCurrentV) >= nNewSize * sizeof(CValue) )
			// ������ ��� ��������; ��� ���������� ������� ������ ����������� �� �����
			pV = pCurrentV;
		else
			// ���������� ������� HEAP_REALLOC_IN_PLACE_ONLY - �������� � CNumeric
			pV = (CValue*) HeapReAlloc(hHeap, HEAP_REALLOC_IN_PLACE_ONLY | HEAP_NO_SERIALIZE, (LPVOID)pCurrentV, sizeof(CValue) * nNewSize);

		if( pV )
		{
			ptr = pV + nCurrentSize;
			for ( int i = nCurrentSize; i < nNewSize; i++ )
				new( ptr++ ) CValue; //������������� �������� �����������
		}
	}
	else
		pV = NULL;
	
	return pV;
}

CVTExtRow::CVTExtRow(int numValues)
{
	if( numValues > 0 )
	{
		nSize = numValues;
		pValues = AllocMem(nSize);
	}
	else
	{
		nSize = 0;
		pValues = NULL;
	}
}

CVTExtRow::~CVTExtRow()
{
	FreeMem(pValues, nSize);

	nSize = 0;
	pValues = NULL;
}

int CVTExtRow::NewColumn()
{
	int nNewCol = nSize++;
	CValue* pNewValues;

	pNewValues = ReAllocMem(pValues, nNewCol, nSize);

	if( !pNewValues )
	{
		// �� ������� �������� ������ - �������� ����������� ���������
		pNewValues = AllocMem(nSize);

		for( int i = 0; i < nNewCol; i++ )
			pNewValues[i] = pValues[i];
		
		FreeMem(pValues, nSize - 1);
	}
	pValues = pNewValues;

	return nNewCol;
}

void CVTExtRow::RemoveColumn(int nCol)
{
	CValue *ptr = pValues + nCol;
	
	nSize--;
	for( int i = nCol; i < nSize; i++, ptr++ )
		*ptr = *(ptr + 1);

	ptr->~CValue();
}

CVTExtRow& CVTExtRow::operator= (CVTExtRow& SrcRow)
{
	int nMinSz = min(nSize, SrcRow.nSize);
	for( int i = 0; i < nMinSz; i++ )
	{
		SetValue(i, SrcRow.GetValue(i));
	}
	return *this;
}


#ifdef _NDEBUG
CString CVTExtRow::Format()
{
	CString str = "";
	for( int nCol = 0; nCol < nSize; nCol++ )
	{
		str += GetValue(nCol).Format();
		str += "; ";
	}

	return str;
}
#endif
