/////////////////////////////////////////////////////////////////////
// V7Control.h : interface of all 1C++ controls.
//
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro-75@mail.ru
// Copyright (c) 2006-2009. All Rights Reserved.
// Version: 1.0
//
//////////////////////////////////////////////////////////////////////

//��� ���� ����� ����� ��� V7 ��������� �� ������ ����������� ���� ���������
//�������� �� ������������ ������� ������������::������������������������()
#pragma once

class CV7Control
{
public:
    virtual BOOL CreateControlWnd(CWnd* pParent, CGetDoc7* pDoc, CGetField* pGetField, CBLContext* pUDC) = 0;
    //������� ���� ��.
    //���������:
    // - pParent - ������������ ���� (�����);
    // - pDoc - �������� ������������ ������;
    // - pGetField - �������� ����� - ��� ������� ����� ���������� ��;
    // - pUDC - �������� ����-���������� ������ ��, ����� ���� NULL.
    //����������: TRUE - ������, FALSE - ��������.

	static void BeginDrag(const CValue& vDragValue);
	static BOOL IsDragOn();
	static void GetDragValue(CValue& vDragValue, COleDataObject* pDataObject);
	static void EndDrag();
};

class CV7ControlEx
{
public:
	virtual BOOL InitControlEx(CBLContext* pForm, CGetDoc7* pDoc, CGetField* pField, CBLContext* pUDC, CValue* pCreateParam) = 0;
    // - pForm - OF.�����;
    // - pDoc - �������� ������������ ������;
    // - pField - �������� ����� - ������ �������� ���������� ����������� ��;
    // - pUDC - �������� ����-���������� ������ ��, ����� ���� NULL.
    // - pCreateParam - �������� � ���������� ��������
    virtual BOOL CreateControlWndEx(CWnd* pParent) = 0;
    //������� ���� ��.
    //���������:
    // - pParent - ������������ ���� (�����);
    //����������: TRUE - ������, FALSE - ��������.

	CString m_strLastError;
};

class IV7ControlContext
{
public:
	virtual CBLContext* GetControlContext() const = 0;
};
