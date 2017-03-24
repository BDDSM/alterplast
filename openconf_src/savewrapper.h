// savewrapper.h
#ifndef SAVEWRAPPER_H
#define SAVEWRAPPER_H
class CSaveWrapper:public CDocument
{
public:
	int DoFileSaveMXL();
	int DoFileSaveDoc();	// ���������� ������� ��� ��������� ���������
	int DoFileSaveText();	// ���������� ��������� ������
	int DoFileSaveErt();	// ���������� ������
	int DoFileSaveToPath(LPCTSTR name,int param);	// ���������� ������� � ���������� ��������� (��� ������� � ����)
	static bool FastSaveMDToFile(LPCTSTR lpPath,bool FireEvent=false);
	static void Init();
};
#endif
