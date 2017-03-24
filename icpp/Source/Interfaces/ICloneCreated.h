// Vector.h: interface ICloneCreated.
//
//	����� - ����� ������� aka artbear, 2008
//
//	��������� ICloneCreated ����� �������������� ��� �������� �������� ����������� ���������� �������� 1�++ � ������ ��
//	������������ ������� ������������ ��� ����������
//	������ ����� ������ ��� ����� ��������� ����� ��������
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ICloneCreated__INCLUDED_)
#define _ICloneCreated__INCLUDED_

#pragma once

namespace Utils1cpp
{

class ICloneCreated
{
public:
	virtual ~ICloneCreated(void) {}

	virtual CBLContext* Clone(LPCSTR szClassName) const = 0;
};

} // namespace

#endif