' (c) Orefkov
' ��������� ��������� artbear@bashnet.ru
'

Dim LoadedMD	' ����� �������������� ��� ������������ �������, ���������� GComp
LoadedMD = ""

Dim cvs

' ������ GComp � �������� �������
Sub RunGComp(cmd)
	Message cmd,mInformation
    On Error Resume Next
    ret = Plugins("1CVS").GComp.Run(cmd)
    If Err <> 0 Then
        MsgBox "��������� ������ " & Err.Description
    Else
        Message "GComp �������� ������ � ����� " & CStr(ret),mInformation
    End If
End Sub

' ����������� ������� ��� ������������ �������.
' �� �����������
Sub Configurator_OnFileDialog(Saved, Caption, Filter, FileName, Answer)
    If LoadedMD <> "" Then
        FileName = LoadedMD
        Answer = mbaOK
    End If
End Sub

Sub Configurator_OnMsgBox(Text, Style, DefAnswer, Answer)
    If LoadedMD <> "" Then
        Message Text, mMetaData
        Message DefAnswer, mMetaData
        Answer = DefAnswer
    End If
End Sub

Sub Configurator_OnDoModal(hWnd, Caption, Answer)
    If LoadedMD <> "" Then anwser = mbaOK
End Sub

Sub DeCompileFile(FileName)
'    RunGComp ("-d -DD " & cvs.gcomp.SRCPath & " --meta-data -F """ & FileName & """")
    RunGComp ("-d -DD " & """" & cvs.gcomp.SRCPath & """" & " --meta-data -F """ & FileName & """")
End Sub

' �����
'   gcomp -c -DD "W:\Kp\New\kp102031md\Src_GComp �����" --meta-data -F 1cv7new.md
Sub CompileFile(FileName)
    RunGComp ("-c -DD " & """" & cvs.gcomp.SRCPath & """" & " --meta-data -F """ & FileName & """")
End Sub
' ����������

'��������� ������� ���������� ����������
Sub Configurator_MetaDataSaved(FileName)
    'If MsgBox("��������� GComp'��?", vbYesNo, "CVS") = vbYes Then
    '    'RunGComp "-d -F """ & FileName & """"
	'	DeCompileFile FileName
    'End If
End Sub

Sub TestCVSRun()
    On Error Resume Next
    Plugins("1CVS").CVS.Run ("checkout config")
    If Err <> 0 Then MsgBox Err.Description
End Sub

Sub TestRunGUI()
' �����	- �� ���� ��������� ������ CVS.GUI
'	Plugins("1CVS").CVS.RunGUI True
'	MsgBox "Close GUI"
    Plugins("1CVS").CVS.RunGUI False
End Sub

' ������������ ������� "�� ����". �� ��������� ��������� ������
Sub TestRunGCompDeCompile()
	Fname="1cv7.md"
    If MetaData.Modified <> 0 Then
        Status "����������� ������ ������ ����������"
        If MetaData.SaveMDToFile(IBDir & "new.md", False) = False Then
            Message "�� ������� ��������� ����������", mBlackErr
            Exit Sub
        End If
		Fname="new.md"
    End If
    Status "������ ����������..."

    'Message "-d -D "& cvs.gcomp.SRCPath & " --meta-data -F """ & IBDir & Fname & """", mMetaData

    'RunGComp ("-d -D " & cvs.gcomp.SRCPath & " --meta-data -F """ & IBDir & Fname & """")
	DeCompileFile IBDir & Fname
End Sub

' ������ ������� �� ������
' ���� �� ��� ������������ �������
Sub TestRunGCompCompile()
    On Error Resume Next

' -- ����� -- 
'    RunGComp "-c"
'   gcomp -c -DD "W:\Kp\New\kp102031md\Src_GComp �����" --meta-data -F 1cv7new.md
    CompileFile("1cv7new.md")
' -- ����������

End Sub

Sub GCompHelp()
	RunGComp ""
End Sub

Sub Init(param)
	set cvs=Plugins("1cvs")
	if Not cvs Is Nothing Then
		cvs.cvs.CVSPath = "C:\Program Files\cvsnt\cvs.exe"
		cvs.cvs.CVSRoot = ":local:T:\Repository"
		cvs.cvs.GUIPath = "C:\Program Files\WinCvs 1.3\wincvs.exe"

	  Set fso = CreateObject("Scripting.FileSystemObject")
	  if fso.FileExists("D:\Cmd\gcomp.exe") then
			cvs.gcomp.GCompPath = "D:\Cmd\gcomp.exe"
		elseif fso.FileExists("C:\Cmd\gcomp.exe") then
			cvs.gcomp.GCompPath = "C:\Cmd\gcomp.exe"
		end if

		cvs.gcomp.SRCPath = "src_GComp"
	End If
End Sub

Sub Configurator_AllPluginsInit()
	Init(0)
End Sub

Init(0)