// config.h
// ������� ������ ����������
#ifndef CONFIG_H
#define CONFIG_H

// ��� ��������� �� �������-���������� �������� ����
typedef void(*HANDLER_ON_CREATE_WND)(CWnd*);
// ���� ������� �������������
#define CFG_ON_CREATE_MAIN_WND		0x1		// �������� ��������� ����
#define CFG_ON_CREATE_CFG_WND		0x2		// �������� ���� "������������"

// ��������� ������, ���������������� ������
class CConfigService{
public:
	virtual CString IBDir()=0;
	virtual CString BinDir()=0;
	virtual CDocument* OpenFile(CString path)=0;
	virtual bool AddConfigTab(CString name,HICON hIcon,CRuntimeClass* pRTCDoc,CRuntimeClass* pRTCView)=0;
	virtual void AddConfigEventHandler(WORD wEventType,HANDLER_ON_CREATE_WND pHandler)=0;
};
/*
��� ��������� � ������ ���� dll ������ �������������� ������� "SetConvigService"
extern "C" _declspec(dllexport) void SetConvigService(void* p);
���� ����� ������� ����� ������� � dll, ��������� �������
�� � �������� ��������� �� ������ ������ CConfigService, �����
������� � ����� ����� �������� �������.
*/
#endif