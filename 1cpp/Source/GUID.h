
//////////////////////////////////////////////////////////////////////
// GUID.h: interface for CGUID class.
//
// �����������: ������� ������� aka DmitrO, mailto: dmitro@russian.ru
// ������: 1.0
//////////////////////////////////////////////////////////////////////

#pragma once

class CGUID: public CBLContext
{
DECLARE_DYNCREATE(CGUID);
public:
    CGUID();
    ~CGUID();
    
private:
//CBLContext interface
public:
    virtual int  CallAsFunc(int,class CValue &,class CValue * *);
	virtual int  CallAsProc(int,class CValue * *);
	virtual void  DecrRef(void);
	virtual int  FindMethod(char const *)const;
	virtual int  FindProp(char const *)const;
	virtual char const *  GetCode(void)const;
	virtual int  GetDestroyUnRefd(void)const;
	virtual void  GetExactValue(class CValue &);
	virtual class CObjID   GetID(void)const;
	virtual class CBLContextInternalData *  GetInternalData(void);
	virtual char const *  GetMethodName(int,int)const;
	virtual int  GetNMethods(void)const;
	virtual int  GetNParams(int)const;
	virtual int  GetNProps(void)const;
	virtual int  GetParamDefValue(int,int,class CValue *)const;
	virtual char const *  GetPropName(int,int)const;
	virtual int  GetPropVal(int,class CValue &)const;
	virtual long  GetTypeID(void)const;
	virtual char const *  GetTypeString(void)const;
	virtual class CType   GetValueType(void)const;
	virtual int  HasRetVal(int)const;
	virtual void  IncrRef(void);
	virtual void  InitObject(class CType const &);
	virtual void  InitObject(char const *);
	virtual int  IsExactValue(void)const;
	virtual int  IsOleContext(void)const;
	virtual int  IsPropReadable(int)const;
	virtual int  IsPropWritable(int)const;
	virtual int  IsSerializable(void);
	virtual int  SaveToString(class CString &);
	virtual void  SelectByID(class CObjID,long);
	virtual int  SetPropVal(int,class CValue const &);

	static class CObjID ObjID;
private:
    static CParamDefs defFnNames;
    GUID m_guid;
    mutable CString m_StringView;
    int ConvertFormStr(const char* ptr, GUID* guid);
public:
    void Set(const GUID* guid) { memcpy(&m_guid, guid, sizeof(GUID)); m_StringView.Empty();};
    void Get(GUID* guid) const { memcpy(guid, &m_guid, sizeof(GUID)); };
    void GetBinaryString(CString &str) const;
	void CreateNew(void);//spock: 2005-12-11 ����, � ���������� �� ������� ����� ������
};
