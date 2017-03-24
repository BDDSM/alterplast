//////////////////////////////////////////////////////////////////////
// SQLNumeric.h: interface for the CSQLNumeric class.
//
// �����������: ������� ������� aka DmitrO, mailto: dmitro@ezmail.ru
// ������: 1.0
//////////////////////////////////////////////////////////////////////

#pragma once

#include <sql.h>

class CSQLNumeric: public CNumeric
{
public:
    void Load(const SQL_NUMERIC_STRUCT *pnumeric);
    void Save(SQL_NUMERIC_STRUCT *pnumeric) const;
};
