' -----------------------------------------------------------------------------
'		����� ��������/������� ��� ������ � ��������
'		������� �������� aka slavka
'		05/12/2004
'		e-mail: webmastert@mail.ru
'		ICQ:
' -----------------------------------------------------------------------------
Dim Names
Dim WshShell
Set WshShell = CreateObject("WScript.Shell")
'�������� � ������
'���������:
'name - ���� ��������. Name ������ ���������� � ������ �� �������� ������: HKEY_CURRENT_USER - HKCU,
' HKEY_LOCAL_MACHINE  - HKLM, HKEY_CLASSES_ROOT - HKCR,	HKEY_USERS - HKEY_USERS,
' HKEY_CURRENT_CONFIG - HKEY_CURRENT_CONFIG. ����. "HKCU\Software\1C\1cv7\7.7\Test\script name"
'Value -  �������� ���������
'strType - ��� ���������, ��������� �������� "REG_SZ", "REG_EXPAND_SZ", "REG_DWORD", "REG_BINARY" 
Sub RegWrite(name,Value,strType)
WshShell.RegWrite name,Value,strType
End Sub

'�������� �������� ���������
'strName - �������� ���������. strName ������ ���������� � ������ �� �������� ������: HKEY_CURRENT_USER - HKCU,
' HKEY_LOCAL_MACHINE  - HKLM, HKEY_CLASSES_ROOT - HKCR,	HKEY_USERS - HKEY_USERS,
' HKEY_CURRENT_CONFIG - HKEY_CURRENT_CONFIG. ����."HKCU\Software\1C\1cv7\7.7\Test\script name"
Function RegRead(strName)
strName = WshShell.RegRead(strName)
End Function

'������� ������/��������
'strName - �������� �������/���������. strName ������ ���������� � ������ �� �������� ������:
' HKEY_CURRENT_USER - HKCU, HKEY_LOCAL_MACHINE  - HKLM, HKEY_CLASSES_ROOT - HKCR,	HKEY_USERS - HKEY_USERS,
' HKEY_CURRENT_CONFIG - HKEY_CURRENT_CONFIG.
'��� �������� ������� StrName ������ ������������� "\" (����."HKCU\Software\1C\1cv7\7.7\Test\")
'��� �������� ��������� strName ������ ���� � ���� "HKCU\Software\1C\1cv7\7.7\Test\script name"
Sub RegDelete(strName)
WshShell.RegDelete(strName)
End Sub

' -----------------------------------------------------------------------------
'����������� �������� ������ �������:
'HKEY_CLASSES_ROOT &H80000000
'HKEY_CURRENT_USER &H80000001
'HKEY_LOCAL_MACHINE &H80000002
'HKEY_USERS &H80000003
'HKEY_CURRENT_CONFIG &H80000005
'�.�. � ������������� �������� ����� � ��������� "rasdel" ��������� (��������
'���� ����� ���������� � ����� HKEY_CURRENT_USER) �� ����� �������
'��� HKEY_CURRENT_USER ��� HKCU ��� &H80000001
' -----------------------------------------------------------------------------

'���������� � ���������� Names ��� ���������� � ��������� �������
'� ���� ������ � ������������ ";"
'����.: "AddTemplate;AutoCompleteSymb;DisableTemplate;EnumMethDlg;ParamInfo;"
Function EnumSubKey(rasdel, strKeyPath, Names)
	If rasdel = "HKEY_CURRENT_USER" OR rasdel =  "HKCU" OR rasdel =  "&H80000001" Then
		rasdel = &H80000001
	elseif rasdel = "HKEY_LOCAL_MACHINE" OR rasdel =  "HKLM" OR rasdel =  "&H80000002" Then
		rasdel = &H80000002
	elseif rasdel = "HKEY_USERS" OR rasdel =  "&H80000003" Then
		rasdel = &H80000003
	elseif rasdel = "HKEY_CURRENT_CONFIG" OR rasdel =  "&H80000005" Then
		rasdel = &H80000005
	End If
strComputer = "."
Set oReg=GetObject("winmgmts:{impersonationLevel=impersonate}!\\" &_
strComputer & "\root\default:StdRegProv")
oReg.EnumKey rasdel, strKeyPath, arrSubKeys
On Error Resume Next
For Each subkey In arrSubKeys
	Names =Names & subkey & ";"
Next
End Function

'���������� � ���������� Names ��������� � ��������� �������
'� ���� ������ � ������������ ";"
'����.: "lang;components;"
Function EnumValues(rasdel, strKeyPath, Names)
	If rasdel = "HKEY_CURRENT_USER" OR rasdel =  "HKCU" OR rasdel =  "&H80000001" Then
		rasdel = &H80000001
	elseif rasdel = "HKEY_LOCAL_MACHINE" OR rasdel =  "HKLM" OR rasdel =  "&H80000002" Then
		rasdel = &H80000002
	elseif rasdel = "HKEY_USERS" OR rasdel =  "&H80000003" Then
		rasdel = &H80000003
	elseif rasdel = "HKEY_CURRENT_CONFIG" OR rasdel =  "&H80000005" Then
		rasdel = &H80000005
	End If
strComputer = "."
Set oReg=GetObject("winmgmts:{impersonationLevel=impersonate}!\\" &_
strComputer & "\root\default:StdRegProv")
oReg.EnumValues rasdel, strKeyPath, arrValueNames
On Error Resume Next
For Each ValueNames In arrValueNames
	Names =Names & ValueNames & ";"
Next
End Function

'���������� � ���������� Names ��������� ,�� �������� � ���� � ��������� �������
'� ���� ������ "lang=2,REG_SZ;components=3,REG_DWORD;", �.�. 
'��������� lang ������������� �������� 2 ��� REG_SZ, � ��������� components
'������������� �������� 3 ��� REG_DWORD
Function EnumParamValues(rasdel, strKeyPath, Names)
const REG_SZ = 1
const REG_EXPAND_SZ = 2
const REG_BINARY = 3
const REG_DWORD = 4
const REG_MULTI_SZ = 7	
	If rasdel = "HKEY_CURRENT_USER" OR rasdel =  "HKCU" OR rasdel =  "&H80000001" Then
		ras = &H80000001
	elseif rasdel = "HKEY_LOCAL_MACHINE" OR rasdel =  "HKLM" OR rasdel =  "&H80000002" Then
		ras = &H80000002
	elseif rasdel = "HKEY_USERS" OR rasdel =  "&H80000003" Then
		ras = &H80000003
	elseif rasdel = "HKEY_CURRENT_CONFIG" OR rasdel =  "&H80000005" Then
		ras = &H80000005
	End If
strComputer = "."
Set oReg=GetObject("winmgmts:{impersonationLevel=impersonate}!\\" &_
strComputer & "\root\default:StdRegProv")
oReg.EnumValues ras, strKeyPath, arrValueNames, arrValueTypes
On Error Resume Next
For i=0 To UBound(arrValueNames)
	Names =Names & arrValueNames(i) & "="
	Select Case arrValueTypes(i)
        Case REG_SZ
            tip = "REG_SZ"
        Case REG_EXPAND_SZ
            tip = "REG_EXPAND_SZ"
        Case REG_BINARY
            tip = "REG_BINARY"
        Case REG_DWORD
            tip = "REG_DWORD"
        Case REG_MULTI_SZ
            tip = "REG_MULTI_SZ"
    End Select 
	str1 = rasdel & "\" & strKeyPath & "\" & arrValueNames(i)
	str = RegRead(str1)
	Names = Names & str1 & "," & tip & ";"
Next
End Function