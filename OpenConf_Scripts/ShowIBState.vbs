'	����� ������� �������������� ���� 1�.
'	������������ ��������� ��������:
'	1)	"� ���� ������ ���"
'	2)	"���� �������� ��� � ����������� ������ ��� ���� ������������ �������� � ����������� ������"
'	3)	"�������� ������������ ����� �� 1�, ����� ��������������"
'	4)	"1� �������� � ����������� ������"
'	
'	����������. 
'	1) ������ �������� � ��� ������� ������ WSH, � ��� ����������� ������ ��� OpenConf
'	2) �������� �������� ������ ��� DBF-��� ( ���� !!)
'	3) ���� �� ������� ��������� ����������� ����� ��� ������ ������ ������������ � ����������� ������
'   4) ������ ������� �� ������ ���������� "1S.StatusIB.wsc" ( ������ "1S.IBState" )
'	
'	Copyright (c) ����� ������� aka artbear, 2004
'	E-mail:		<artbear@bashnet.ru>
'	ICQ UIN:	265666057
'

strIBDir = "W:\�������������� ������������\���������� 1�++\"

on Error resume next
temp = WScript.Version
iErrNumber = Err.Number
on Error goto 0
if iErrNumber = 0 then
	ShowIBStatus()
	WScript.Quit(0)
end if

strIBDir = IBDir ' openconf

Sub ShowIBStatus()
	set ob = CreateObject("1S.IBState")
	Echo "������ ���� - "+ob.StringIBState(strIBDir)
End Sub

Sub Echo(str)

	on Error resume next
	temp = WScript.Version
	iErrNumber = Err.Number
	on Error goto 0
	if iErrNumber = 0 then
		WScript.Echo CStr(str) ' ������� ������
	else
		Message CStr(str) ' openconf-������
	end if
End Sub
