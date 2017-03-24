// �������� ����� ����� Logfile.h:
// http://forum.vingrad.ru/index.php?act=module&module=vingradfaq&article=2279&spider_enabled=
// http://66.102.9.104/search?q=cache:I7UJKDATRaIJ:forum.vingrad.ru/index.php%3Fact%3Dmodule%26module%3Dvingradfaq%26article%3D2279%26spider_enabled%3D+CLogFile&hl=ru&ct=clnk&cd=4

#if !defined(_LOG_FILE_INCLUDED)	
#define _LOG_FILE_INCLUDED	
//////////////////////////////////////////////////////////////////	
// ������������ �����	
//////////////////////////////////////////////////////////////////	
//#include <string>	
#include <time.h>	
#include <wtypes.h>	
#include <string>
//using namespace std;	
//////////////////////////////////////////////////////////////////	
// ����� CLogFile	
//////////////////////////////////////////////////////////////////	
class CLogFile	
{	
public:	
    CLogFile();	
    virtual ~CLogFile();	
protected:	
    HANDLE m_hFile;	
    CRITICAL_SECTION m_cs;	
    void Write(const CString& str);	
public:	
    bool Open(const CString& csFileName);	
    void Close();	
    void Clear();	
    bool IsOpen();	
	std::string GetFullLog();	
	std::string GetDateOrTime(int type);	
    CLogFile &operator<<(const CString& str);	
    CLogFile &operator<<(int val);	
    CLogFile &operator<<(unsigned int val);	
    CLogFile &operator<<(DWORD val);	
};	
//////////////////////////////////////////////////////////////////	
// ��������������� ���������� ���������� ����������	
//////////////////////////////////////////////////////////////////	
extern CLogFile g_Log;	
//////////////////////////////////////////////////////////////////	
// �������� ��� ������ � ���-������:	
// ����� ������� � ��� ����, ����� �������� g_Log<<CURRDATE;	
// ����� ������� ���� � �����: g_Log<<CURRDATETIME;	
// ��� �������� ������: g_Log<<CRLF;	
// �� ������:	
// g_Log<<"����: "<<CURRDATE<<CRLF<<"�����: "<<CURRTIME<<CRLF;	
//////////////////////////////////////////////////////////////////	
#define CRLF "\015\012"	
#define CURRTIME (g_Log.GetDateOrTime(GET_CURR_TIME))	
#define CURRDATE (g_Log.GetDateOrTime(GET_CURR_DATE))	
#define CURRDATETIME (g_Log.GetDateOrTime(GET_CURR_DATETIME))	
enum {GET_CURR_DATE, GET_CURR_TIME, GET_CURR_DATETIME};	
#endif