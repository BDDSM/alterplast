/*
fix_basic.inl
(c) ��������� �������
���� ��� ������� �������� ��� ������������� ����������� ��������
� basic.dll � �������, ������� ������ ������������ ��� ����� ������ 1���.
	���� ��������:
basic.dll � ����� ��������� DllMain ��� �������� ��������� ��������,
������������ �� ��, ��� CWinApp � 1cv7.exe ��� �����������������,
� ��� �������� - �� ��, ��� CWinApp � 1cv7.exe ��� �����������������.
��� ����������� �������� ���������������� ������ � basic.dll, �����
������ �������� ����������, DllMain � basic.dll ���������� 0,
�� CWinApp ��� �� ������������������, ������� ������ �� ����� ����������.

	������� - ���������� ��������, ����������� �������� VC++, � ������ ����� �
��������������� ������. ��� �������� ����� ���������� ���

#include "fix_basic.inl"

� stdafx.cpp

	��������� ��������:
������� ������� �� basic.dll ���������������� ��� ������ ������ �����
������� �� basic.dll. ���� ������ � ��������� ����� ��������� �� � �������,
� � ������������� ������, �� � ���� ������ ��� ������������� ������� �������
�� ����� ��������, � ��������� � ������ ����� ��������.
	�������-��������� ������ ������� �� �������� � basic.dll. ���� ���
���������� ������������ ���������� �������� ������ ���������, �� �����������
��� ����������. ��������� �����������, ���� �������� ����.
�������� ������� ���� ���������� - ��� ������ ������ ��� ������ ���
���������������� ��� ������� �������, � �� ���� �����. ��� ���� �����
��������� ����������� ��������� � ������������� �� ��� ������.
����� ����, ������ LoadLibrary ����������� GetModuleHandle, ��� ����,
����� �� ����������� ������� �������� ���, ����� ��� �� ����� �����������
�������, ��� �������� � �������� ��� ������ �� ���������. ����� �������,
�� ��������� ���������� � ������� basic.dll �� ����, ��� ��� �����
��������� ����� 1�.
*/

#include <delayimp.h>

#pragma comment(linker, "/delayload:basic.dll")

// ������� ������������� ������� �������.
// ����������� � ������� �� ������������ ����������.
// �������� �� ������������ ���� MS � VC98\Include\delayhlp.cpp
extern "C" FARPROC WINAPI __delayLoadHelper(PCImgDelayDescr pidd, FARPROC *ppfnIATEntry)
{
	HINSTANCE hInst = *pidd->phmod;
	if(!hInst)
	{
		// �������� ������
		hInst = GetModuleHandle(pidd->szName);
		if(!hInst)
		{
			MessageBox(NULL, "������� ��������� � basic.dll �� �� ��������", 0, 0);
			return NULL;
		}
		*pidd->phmod = hInst;
		// ������ ���� ��������� ��� ������� �������.
		// ���� ���� ������������ �������, ������ ��������� ��,
		// � ����� ��������� � ����� �� ���� �������, ������� GetProcAddress
		DWORD dwIdx = IndexFromPImgThunkData(PCImgThunkData(ppfnIATEntry), pidd->pIAT);
		DWORD dwImports = CountOfImports(pidd->pIAT);
		
		if(pidd->pBoundIAT && pidd->dwTimeStamp)
		{
			// bound imports exist...check the timestamp from the target image
			PIMAGE_NT_HEADERS pinh = PIMAGE_NT_HEADERS(PCHAR(hInst) + PIMAGE_DOS_HEADER(hInst)->e_lfanew);
			
			if(pinh->Signature == IMAGE_NT_SIGNATURE &&
				pinh->FileHeader.TimeDateStamp == pidd->dwTimeStamp &&
				DWORD(hInst) == pinh->OptionalHeader.ImageBase)
			{
				memcpy(pidd->pIAT, pidd->pBoundIAT, dwImports * sizeof IMAGE_THUNK_DATA);
				return FARPROC(pidd->pIAT[dwIdx].u1.Function);
			}
		}
		// �� ������� ���������� � ������������ �������.
		// ����� ����� GetProcAddres
		PImgThunkData pWrite = pidd->pIAT;
		PCImgThunkData pRead = pidd->pINT;
		for(DWORD idx = 0; idx < dwImports; idx++)
		{
			pWrite->u1.Function = (PDWORD)GetProcAddress(hInst,
				(pRead->u1.Ordinal & IMAGE_ORDINAL_FLAG) ?
				(LPCSTR) pRead->u1.Ordinal :
				(LPCSTR) pRead->u1.AddressOfData->Name);
			pRead++, pWrite++;
		}
		return FARPROC(pidd->pIAT[dwIdx].u1.Function);
	}
	return NULL;
}
