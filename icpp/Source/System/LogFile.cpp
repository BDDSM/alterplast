// �������� ����� ����� Logfile.cpp:
// http://forum.vingrad.ru/index.php?act=module&module=vingradfaq&article=2279&spider_enabled=
// http://66.102.9.104/search?q=cache:I7UJKDATRaIJ:forum.vingrad.ru/index.php%3Fact%3Dmodule%26module%3Dvingradfaq%26article%3D2279%26spider_enabled%3D+CLogFile&hl=ru&ct=clnk&cd=4
//
// 	������ �������������
//#include "Logfile.h"
//
//int main(int argc, char* argv[])
//{
//   // ��������� ���� ����
//   g_Log.Open("test_log.txt");
//
//   // ������� � ������� ���������� ���������� ����
//   printf("Previous log contents:\n%s", g_Log.GetFullLog().c_str());
//
//   // ������� ���
//   g_Log.Clear();
//
//   // ���������� ������� ����� � ���� � ���
//   g_Log<<"Program started at "<<CURRDATETIME<<CRLF;
//
//   // �������� ��������� �����-�� ��������
//   for(int i=0; i<10; i++)
//   {
//       g_Log<<"i="<<i<<", time="<<CURRTIME<<CRLF;
//       Sleep(200);
//   }
//
//   // ������� � ��� ����� ��������� ������
//   g_Log<<"Program finished at: "<<CURRTIME<<CRLF;
//   g_Log<<"========================="<<CRLF<<CRLF;
//   return 0;
//}

// ��������������� ��������� ������, ���� �� ����������� VisualC++
#include "StdAfx.h"

#include "LogFile.h"

////////////////////////////////////////////////////////////////////
// �����������, ����������
////////////////////////////////////////////////////////////////////

CLogFile::CLogFile()
{
   m_hFile = NULL;
   InitializeCriticalSection(&m_cs);
}

CLogFile::~CLogFile()
{
   Close();
}


/////////////////////////////////////////////////////////////////////
// ������
/////////////////////////////////////////////////////////////////////


bool CLogFile::IsOpen()
{
   return (m_hFile != NULL)?true:false;
}

void CLogFile::Close()
{
   if(IsOpen())
       CloseHandle(m_hFile);
}

bool CLogFile::Open(const CString& csFileName)
{
   Close();
   m_hFile = CreateFile(csFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
   if(!m_hFile) return false;
   SetFilePointer(m_hFile, 0, 0, FILE_END);
   return true;
}

void CLogFile::Write(const CString& str)
{
   if(!IsOpen() || !str.GetLength()) return;
   EnterCriticalSection(&m_cs);
   DWORD dw; WriteFile(m_hFile, str, str.GetLength(), &dw, 0);
   LeaveCriticalSection(&m_cs);
}

void CLogFile::Clear()
{
   if(!IsOpen()) return;
   SetFilePointer(m_hFile, 0, 0, FILE_BEGIN);
   SetEndOfFile(m_hFile);
}

std::string CLogFile::GetFullLog()
{
   if(!IsOpen()) return "";
   EnterCriticalSection(&m_cs);

   DWORD dwCurrentFilePosition = SetFilePointer(m_hFile, 0, 0, FILE_CURRENT);
   DWORD len = SetFilePointer(m_hFile, 0, 0, FILE_END);
   SetFilePointer(m_hFile, 0, 0, FILE_BEGIN);

   char *buf = static_cast<char*>(malloc(len+1));
   DWORD dw; ReadFile(m_hFile, buf, len, &dw, 0);
   buf[len] = 0;
   std::string res = buf;
   free(buf);

   SetFilePointer(m_hFile, dwCurrentFilePosition, 0, FILE_BEGIN);
   LeaveCriticalSection(&m_cs);

   return res;
}


///////////////////////////////////////////////////////////////////
// ���������� ����������
///////////////////////////////////////////////////////////////////

// ����� ����� ���� ������ ����� ������������, �� ������
// ������ ����� ����� "��� �����" ���������� ���������� ����,
// ��� ��� ������ ������������ �, � ������ � �� ������������ wink.gif
CLogFile g_Log;


///////////////////////////////////////////////////////////////////
// ��������������� ��������� ���������
///////////////////////////////////////////////////////////////////


CLogFile &CLogFile::operator <<(const CString& val)
{
   if(IsOpen()) Write(val);
   return *this;
}

CLogFile &CLogFile::operator <<(unsigned int val)
{
   if(IsOpen())
   {
       char buf[12]; sprintf(buf, "%d", val);
       Write(buf);
   }
   return *this;
}

CLogFile &CLogFile::operator <<(int val)
{
   if(IsOpen())
   {
       char buf[12]; sprintf(buf, "%d", val);
       Write(buf);
   }
   return *this;
}

CLogFile &CLogFile::operator <<(DWORD val)
{
   if(IsOpen())
   {
       char buf[12]; sprintf(buf, "%d", val);
       Write(buf);
   }
   return *this;
}


/////////////////////////////////////////////////////////
// �������������� ������� ����, ������� ��� ����/�������
/////////////////////////////////////////////////////////


std::string CLogFile::GetDateOrTime(int type)
{
   char buf[128], *fmt;
   time_t time_curr; time(&time_curr);
   tm *tm_curr = localtime(&time_curr);
   switch(type)
   {
   case GET_CURR_DATE: fmt = "%a, %b %d, %Y"; break;
   case GET_CURR_TIME: fmt = "%H:%M:%S"; break;
   case GET_CURR_DATETIME: fmt = "%A, %B %d, %Y %H:%M:%S"; break;
   default: break;
   }
   strftime(buf, sizeof(buf), fmt, tm_curr);
   return buf;
}