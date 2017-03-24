'--------------------------------------------------------------------------------------
' �������� ������ Excel (������ ��� OpenConf'�)
' ������ ������ 1�-������ ������ ������ ����� �� ���������� ������ �� Excel,
' � ������� ������������ ����� ������� �������� (�� ��������� ���� ��������� � ������,
' � ������� �� 1�-�������, ����� ��������� ���� ����� � ������ �������� ������� � ����.)
' ���������� ��� �������� ������ TXT,MXL,ERT,SPL,PRM � ���������� TXT,MXL � ERT.
' � ��������� ����� � ��������� ������� ����������� �� �������. ��-�� ���������
' ������� OpenConfa 1.0.1.8 (Beta) �������� ���������� ������������ �. �. �������������
' � ������� ������ �����, �� ��� ����� ����� ���������.
'
' ��� ������ ����������, ����� �� ���������� ��� ���������� Excel.
'
' ���������: ����������� � ...\BIN\CONFIG\Scrips
'
' ������ �������������� � ��������� ��������: WinXP Sp2, 1Cv7.70.025, MS Offise 2003
' OpenConf 1.0.1.8 (Beta)
'
' aphanas@list.ru 
'--------------------------------------------------------------------------------------

Dim Excel
Dim WshShell
Dim FSO

Dim a
Dim s
Const HKEY_LOCAL_MACHINE = &H80000002


Sub Configurator_OnFileDialog(Saved, Caption, Filter, FileName, Answer)
	FilterIndex = 0
	ModeName = ""
	ExcelFilter = "��� �����,*.*"
    IF Filter = "����� (*.txt)|*.txt|����� - MS DOS ������ (*.txt)|*.txt|������� (*.mxl)|*.mxl|������� �����(���������) (*.ert)|*.ert|����� ������� (*.spl)|*.spl|����� �������� ������� (*.prm)|*.prm|1� ����� (*.txt, *.mxl, *.ert, *.spl, *.prm)|*.txt;*.mxl;*.ert;*.spl;*.prm|" Then
		ExcelFilter = ExcelFilter+",1� �����,*.txt;*.mxl;*.ert;*.spl;*.prm,������� ������ � ���������,*.ert,�������,*.mxl,��������� �����,*.txt,����� �������,*.spl,����� �������� �������,*.prm"
		ModeName = "Open file"
		FilterIndex = 2
	ElseIF Filter = "����� (*.txt)|*.txt|����� - MS DOS ������ (*.txt)|*.txt|��� ����� (*.*)|*.*|" Then
		ExcelFilter = ExcelFilter+",��������� �����,*.txt"
		ModeName = "Save TXT"
		FilterIndex = 2
	ElseIF Filter = "������� (*.mxl)|*.mxl|������� Excel (*.xls)|*.xls| HTML �������� (*.htm;*.html)|*.htm; *.html|��������� ���� (*.txt)|*.txt|" Then
		ExcelFilter = ExcelFilter+",�������,*.mxl,������� Excel,*.xls,HTML ���������,*.htm; *.html,��������� �����,*.txt"
		ModeName = "Save MXL"
		FilterIndex = 2
	ElseIF Filter = "������� �����(���������) (*.ert)|*.ert|" Then
		ExcelFilter = ExcelFilter+",������� ������ � ���������,*.ert"
		ModeName = "Save ERT"
		FilterIndex = 2
	Else
		Exit Sub
	End If
	
	If Saved Then
		FileName = Excel.GetSaveAsFilename(FileName,ExcelFilter,FilterIndex,Caption)
		If FileName = False Then
			Answer = mbaCancel
			Exit Sub
		End If
		
		If FSO.FileExists(FileName) Then
			Name = FSO.GetFileName(FileName)
			intButton = WshShell.Popup("���� """+Name+""" ��� ����������, ����������?",,"ExcelFileDialog (OpenConf script)",36)
			If intButton = 7 Then 'No
				Answer = mbaCancel
				Exit Sub
			End If
		End If
		
		Answer = mbaOk
	Else
		If ModeName = "" Then
			Exit Sub
		End If
		
		LastDir = ""
		ExcelDefaultPath = ""
		On Error Resume Next
		ExcelDefaultPath = WshShell.RegRead("HKCU\Software\Microsoft\Office\11.0\Excel\Options\DefaultPath")
		LastDir = WshShell.RegRead("HKCU\Software\1C\1Cv7\7.7\ExcelFileDialog\"+ModeName+"\LastDir")
		On Error GoTo 0
		
		If LastDir <> "" Then
			WshShell.RegWrite "HKCU\Software\Microsoft\Office\11.0\Excel\Options\DefaultPath",LastDir,"REG_EXPAND_SZ"
		End If
		
		If ModeName = "Open file" Then
			FileName = Excel.GetOpenFileName(ExcelFilter,FilterIndex,Caption,, False)
			Answer = mbaCancel
			If FileName = False Then
				Exit Sub
			End If
			Documents.Open(FileName)
		End If
		
		SlashInd = 0
		Poz = InStr(FileName,"\")
		do Until Poz = 0
			SlashInd = Poz
			Poz = InStr(Poz+1,FileName,"\")
		Loop
		DirName = Left(FileName,SlashInd)
		
		WshShell.RegWrite "HKCU\Software\1C\1Cv7\7.7\ExcelFileDialog\"+ModeName+"\LastDir",DirName,"REG_EXPAND_SZ"
		WshShell.RegWrite "HKCU\Software\Microsoft\Office\11.0\Excel\Options\DefaultPath",ExcelDefaultPath,"REG_EXPAND_SZ"
		
	End If
	
End Sub

Sub Init(Param)
	Set Excel = CreateObject("Excel.Application")
	Excel.Visible = True
	Excel.Visible = False
	
	Set FSO = CreateObject("Scripting.FileSystemObject")
	Set WshShell = CreateObject("WScript.Shell")
End Sub

Init 0
