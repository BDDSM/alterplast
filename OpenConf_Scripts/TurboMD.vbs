' (c) Orefkov
'������ �������, ������������ ��������� � ����
'������� ������������� ����� � ������
'(���� ������ ������ ���������� ��� ���� �������)
'�� ������� ����.
'��� ���� ����������� ������� ��������� �����/������
'��� ������������� ��������� ������������
'
' �����: ��������� �������
'
' artbear: ���� ������� ����
' ����� - ������� �������� <shootnick2000@mail.ru>
' ����������� ������ ���������� md � ��������������� turboMD.prm � bak.
'
Dim BaseDir
BaseDir = IBDir & "unpack\" ' ������� ������� ��� ��������

' ��������� �������� ����� ���������
Sub MakeDir(Dir)
    Set fso = CreateObject("Scripting.FileSystemObject")
    If Left(Dir, 2) = "\\" Then
        'UNC Path
        pos = InStr(3, Dir, "\")    'Server name
        p = Left(Dir, pos)
        Dir = Mid(Dir, pos + 1)
    Else
        p = ""
    End If
    pos = 1
    While pos <> 0
        pos = InStr(Dir, "\")
        If pos = 0 Then
            p = p & Dir
        Else
            p = p & Left(Dir, pos)
            Dir = Mid(Dir, pos + 1)
        End If
        If fso.FolderExists(p) = False Then fso.CreateFolder p
    Wend
End Sub

'�������� ���������
Sub UnloadDoc(doc)
	'��������� ��� �����
    fName = BaseDir & Replace(doc.Name, ".", "\")
    If doc = docTable Then fName = fName & ".mxl" Else fName = fName & ".txt"
	'�� ����� ����� �������� �������
    lastdec = InStrRev(fName, "\")
    Dir = Left(fName, lastdec - 1)
	'� ������� ���� �������
    MakeDir Dir
	' ��������� �������� � ����
    doc.SaveToFile fName
	' ���������, ���� �� ���� �������� � turbomd.prm
    Set fso = CreateObject("Scripting.FileSystemObject")
    Set f = fso.OpenTextFile(IBDir & "turbomd.prm", 1, True)
    If f.AtEndOfStream = True Then t = "" Else t = f.ReadAll()
    If InStr(t, doc.Name) = 0 Then
        If MsgBox("�������� " & doc.Name & " � turbomd.prm ?", vbQuestion + vbYesNo) = vbYes Then
			' ��������� ������ � turbomd.prm
            f.Close
            Set f = fso.OpenTextFile(IBDir & "turbomd.prm", 8)
            If Len(t) > 0 And Right(t, 2) <> vbCrLf Then f.WriteBlankLines 1
            f.WriteLine doc.Name & " = " & Replace(fName, IBDir, "")
        End If
    End If
    f.Close 
	Status "TurboMD: ��������� �������� <" & doc.Name & ">"
End Sub

'���������� ������ ��� �������� ��������� ����
Sub UnloadCurrentWnd()
    Set w = Windows.ActiveWnd
    If w Is Nothing Then
        MsgBox "��� ��������� ����", vbOKOnly, "TurboMD"
        Exit Sub
    End If
    Set d = w.Document
    If d.ID < 2 Then
        MsgBox "���� �� �����, �� ������", vbOKOnly, "TurboMD"
        Exit Sub
    End If
    If d = docText Then     ' ������ ������
        UnloadDoc d
    Else
        If d = docWorkBook Then ' �����
            If MsgBox("��������� �����?", vbQuestion + vbYesNo, "TurboMD") = vbYes Then
                UnloadDoc d.Page(0) ' ��������� ������
            End If
			'ModuleText = d.Page(1).text
			'For i = 2 To d.CountPages - 1
				'UnloadMXL d.Page(i), ModuleText
				'UnloadDoc d.Page(i)
			'Next
            If MsgBox("��������� ������?", vbQuestion + vbYesNo, "TurboMD") = vbYes Then
				'strOldText = d.Page(1).text
				'd.Page(1).text = ModuleText
                UnloadDoc d.Page(1) ' ��������� ������
				'd.Page(1).text = strOldText
            End If
        End If
    End If
End Sub

'������ ��� �������� ���� ������������� ������ ������� � ������
Sub LoadFromFilesToMD()
    Set fso = CreateObject("Scripting.FileSystemObject")
    Set f = fso.OpenTextFile(IBDir & "turbomd.prm", 1, True)
    On Error Resume Next
    While f.AtEndOfStream = False
        t = f.ReadLine()
        eq = InStr(t, "=")
        If eq > 0 Then
            dName = Trim(Left(t, eq - 1))
            fName = Trim(Mid(t, eq + 1))
            If Mid(fName, 2, 1) <> ":" And Left(fName, 2) <> "\\" Then fName = IBDir & fName
            Set doc = Documents(dName)
            If Err <> 0 Then
                Message Err.Description, mRedErr
                Err.Clear
            Else
                If doc.LoadFromFile(fName) <> True Then
                    Message "�� ������� ��������� " & doc.Name & " �� " & fName, mBlackErr
                Else
                    Message doc.Name & " �������� �� " & fName, mInformation
                End If
                If Err <> 0 Then
                    Message Err.Description, mRedErr
                    Err.Clear
                End If
            End If
        End If
    Wend
End Sub

' ������ ��� �������� �������� ����� TurboMD.prm
Sub OpenTurboMDPrm()
    Documents.Open IBDir & "turbomd.prm"
End Sub

Sub SaveMD()
    MetaData.SaveMDToFile IBDir & "1cv7new.md", False
End Sub

'artbear
'����� - ������� �������� <shootnick2000@mail.ru>
' ����������� ������ ���������� md � ��������������� turboMD.prm � bak.
Sub RenameTurboMdPRM2Bak()

	Set FSO = CreateObject("Scripting.FileSystemObject")

	If FSO.FileExists(IBDir & "turbomd.prm") Then
		FSO.CopyFile IBDir & "turbomd.prm", IBDir & "turbomd.prm.bak", True
		FSO.DeleteFile(IBDir & "turbomd.prm")
		'MsgBox "turbomd.prm ������������ � turbomd.prm.bak"
	End If

End Sub

'����� ���������� - ��������� prm
'------------------------------------------------------------------
Sub Configurator_MetaDataSaved(FileNAme)
	RenameTurboMdPRM2Bak()
End Sub

' �������� ������ � ������ ������ ���� 
' 	���.���������������("��������������������");
' ��
'	���.���������������("���������������������������");
'
Sub UnloadMXL(docMXL, ModuleText)
  
Message "�������� ������� "+docMXL.Name ' ���������� ��� ����������� ������� �����,
'������ ��� ������� ������, ������.
    fName = BaseDir & Replace(docMXL.Name, ".", "\")
       
    lastdec = InStrRev(fName, "\")
    MXLName = Right(fName, Len(fName) - lastdec)
    
    fName = fName + ".mxl"
  
    Set RegExp = CreateObject("VBScript.RegExp")
    
    RegExp.MultiLine = 1
    RegExp.IgnoreCase = 1
    RegExp.Global = 1
    'RegExp.MultiLine = true
    'RegExp.IgnoreCase = true
    'RegExp.Global = true
    
    RegExp.Pattern = "\.���������������\(""" + MXLName + """\)"
    ModuleText = RegExp.Replace(ModuleText, ".���������������("""+ fName + """)")
'Message ModuleText

End Sub