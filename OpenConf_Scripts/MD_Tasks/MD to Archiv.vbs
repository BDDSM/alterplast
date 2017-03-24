$NAME �������� ������������ �� ��������
' ___________________________________________________________________________
'   �������� ������ � ����������� �������������� (�) ����� �������, 2002
'
'         ��� ����:   www.meleuz.ru/artbear
'         ��� e-mail: artbear@bashnet.ru
'
'   ����������� ����� ������������ 1� �� ��������, � ������ ����� � �.�.
'
' �����: ��� ��������� ���� � ���-������ ���������� ���������� ������
'        ������� � ��������� GetIniFileName(...)
'        ���, ��� �������� sIniFilesFolder = "D:\Cmd"
'
'������: $Revision: 1.4 $
'
Option Explicit

Dim gsIniFilesFolder ' �������, ��� �������� ���-�����
	gsIniFilesFolder = "D:\Cmd"

' ��� ���-������ ��� ���������� (����� ��������)
' ���� ���� � ����������� "ini",  ������ - "ini2"
Dim gsScriptName
	gsScriptName= "MD to Archiv"

' ��������� ��� ������ ��� ������ ���������� ���������
' ��� ��� ������ ����� � �������
Dim DebugFlag '����������� ���������� ����������
'DebugFlag = True '�������� ����� ���������� ���������

  Dim wshShell
  Dim fso 'as FileSystemObject

  Dim LogFile 'as File
  Dim ResDict 'as Dictionary
  Dim ConfDict 'as Dictionary
' ����� �� ��������� ����� ������������
' ���� � ini-����� ����� ��� ���� 1cv7.md, ����� �� ����� (0)
  Dim bNeedBackup


'  ��������� ��������� ����� ����� � ������� ���������� ���������
Sub ArchiveMDFile( )

	Echo "����������� ����� ������������ 1� �� ��������, � ������ ����� � �.�."
	Echo "_________________________________________________________________________"

' ������������� ��������
	if not Init(1) then
  	Exit Sub
	end if

	if not ArchiveConfig(1) then
		Exit Sub
	end if

End Sub 'ArchiveMDFile

'  ��������� ��������� ����� ����� � ������� ���������� ���������
Sub ArchiveMDFileWithDebugMessages( )
	DebugFlag = True '�������� ����� ���������� ���������
	ArchiveMDFile( )
End Sub ' ArchiveMDFileWithDebugMessages

' ������������� ��������
Function Init(NullParam)
  Init = false

  set wshShell = createObject("WScript.Shell")
  Set fso = CreateObject("Scripting.FileSystemObject")

' ������ ��� ini-�����
  Dim IniFileName
	IniFileName = GetIniFileName(".ini")
Debug "IniFileName", IniFileName

' ������ ��� ini-����� ������ ������������
  Dim ConfIniFileName
	ConfIniFileName = GetIniFileName(".ini2")
Debug "ConfIniFileName", ConfIniFileName

'����� ��������� ������ �� INI-�����
  if not GetDataFromIniFile(IniFileName, ResDict) then
    Exit Function
  end if
'����� ��������� ������ �� INI-�����
  if not GetConfDataFromIniFile(ConfIniFileName, ConfDict) then
    Exit Function
  end if

On Error Resume Next
  Dim sDebugFlag
  sDebugFlag = ResDict.Item("DebugFlag")
  Debug "sDebugFlag",sDebugFlag
  if sDebugFlag<>"" then
    DebugFlag = CBool(sDebugFlag)
  end if
  Debug "DebugFlag",DebugFlag
On Error Goto 0

' �������� ���-����
  LogFile = Null '�� �������� � ���-����, ���� �� ����� ���� � ����
  Dim sLogFile
  sLogFile = ResDict.Item("LogFile")
  if sLogFile<>"" then
    If (NOT blnOpenFile(sLogFile, LogFile)) Then
      Call Echo ("�� ���� ������� ���-����"+sLogFile+" .")
      Exit Function
    End If
  End If

  Init = true
End Function 'Init

Function ArchiveConfig(NullParam)
  Dim retval
  retval=false

  wshShell.CurrentDirectory = IBDir

  Dim FolderPath
	FolderPath = fso.GetFolder(".").Path
	'FolderPath = IBDir
	if Right(FolderPath, 1) = "\" then
		FolderPath = Left(FolderPath, Len(FolderPath)-1)
	End if
Debug "Folder path", FolderPath 'fso.GetFolder(".").Path

  Dim s

  s=ConfDict.Item(LCase(FolderPath))
  Debug "archiv name", s
  if s="" then
    echo "� ����� �� ������� �� ������ ������ ������������ !!"
    Exit Function
  end if

  retval=RarMD(s)
  ArchiveConfig = retval
End Function 'ArchiveConfig

Function RarMD(sFileName)
  Dim retval
  retval=false
Debug "RarMD", sFileName

  Dim CSConfFileName
  CSConfFileName = IBDir & "1cv7.md"
  CSConfFileName = "1cv7.md"
' echo ������ - %errorlevel%
  if not fso.FileExists(CSConfFileName) then
    echo "� ���� ����� �� ���������� ���� ������������ "+CSConfFileName+" !"
    Exit Function
  end if

  bNeedBackup=true
  If LCase(sFileName)=CSConfFileName Then
    bNeedBackup=false
  End If 'sFileName=SConfFileName
  If not bNeedBackup Then ' �������� �������� �� �����������
    'Statement if (sFileName=SConfFileName)is true
    '[-block]
' ������ ����� ����������� �� Zip
    retval = CopyArchive(sFileName)

    RarMD = retval
    Exit Function
  End If 'bNeedBackup

'  echo "_________________________________________________________"
  echo "�������� ������� ������"
'del /f /q %1
  if fso.FileExists(sFileName) then
    On Error Resume Next
    fso.DeleteFile sFileName, true
    if err.Number<>0 then
      echo "�� ������� ������� ���� ������ ������������ "+sFileName
      err.Clear()
      Exit Function
    end if
    On Error Goto 0
  End if

  echo "��������� MD-����� � ����� ..."
  Dim ArcCmdLineAdd
  ArcCmdLineAdd = ResDict.Item("ArcCmdLineAdd")
  Debug "ArcCmdLineAdd", ArcCmdLineAdd

  if (ArcCmdLineAdd="") or IsEmpty(ArcCmdLineAdd) then
    echo "������: �� ����� ���� � ���������-������������� Zip-�������"
    Exit Function
  end if
  Dim sArcCmdLineAdd
'  sArcCmdLineAdd=ArcCmdLineAdd+" "+sFileName+" "+CSConfFileName
  sArcCmdLineAdd=ExpandCmdLine(ArcCmdLineAdd, sFileName, "",CSConfFileName)
  Debug "sArcCmdLineAdd",  sArcCmdLineAdd
  Dim ExitCode
'  rar a -y %1 �SConfFileName >nul
  ExitCode = wshShell.Run(sArcCmdLineAdd,,true)
'if %errorlevel%== 0 goto Success
  if ExitCode<>0 then
    echo "��� ��������� md-����� � ����� "+sFileName+" �������� ������"
    Exit Function
  end if

  echo "�������� ������ ..."
  Dim ArcCmdLineTest
  ArcCmdLineTest = ResDict.Item("ArcCmdLineTest")
  Debug "ArcCmdLineTest", ArcCmdLineTest

  if (ArcCmdLineTest="") or IsEmpty(ArcCmdLineTest) then
    echo "������: �� ����� ���� � ���������-������������� Zip-�������"
    Exit Function
  end if
'rar t %1 >nul
  Dim sArcCmdLineTest
'  sArcCmdLineTest=ArcCmdLineTest+" "+sFileName
  sArcCmdLineTest=ExpandCmdLine(ArcCmdLineTest, sFileName, ".",CSConfFileName)
  ExitCode = wshShell.Run(sArcCmdLineTest,,true)
  if ExitCode<>0 then
    echo "��� ��������� md-����� � ����� "+sFileName+" �������� ������"
    Exit Function
  end if

  echo "_________________________________________________________"
  echo "����� ������� ��������"
  echo "���� ������������ ������� � ����� "+sFileName

' ������ ����� ����������� �� Zip
  retval = CopyArchive(sFileName)

  RarMD = retval
End Function 'RarMD

' ��������� ����������� ������ �� ��������
Function CopyArchive(strArchiveName)

  CopyArchive = false

debug "CopyArchive",""
debug strArchiveName,""

Dim ArcCmdLineTest, CopyTo, CopyTo2, MoveTo
rem ArcCmdLineTest - ��������� ������ ������������ ������
'ArcCmdLineTest = "pkunzip -t"
rem CopyTo - ���� ���������� �����
'CopyTo = "r:"
rem CopyTo2 - 2 ������� ����� ��� ����������� ������
rem ( ��������, ���� ���� ������ ����������� ����������� ZIP )
'CopyTo2=""
rem MoveTo - ���� ����������� �����
'MoveTo = "r:\archiv"

CopyTo = ResDict.Item("CopyFolder1")
CopyTo2 = ResDict.Item("CopyFolder2")
ArcCmdLineTest = ResDict.Item("ArcCmdLineTest")
MoveTo = ResDict.Item("MoveFolder")
Debug "ArcCmdLineTest", ArcCmdLineTest

  If bNeedBackup Then ' �������� �������� �����������
    if (ArcCmdLineTest="") or IsEmpty(ArcCmdLineTest) then
      echo "������: �� ����� ���� � ���������-������������� Zip-�������"
      Exit Function
    end if
  end if

  Dim FileName
'  FileName = WScript.Arguments.Item(0)
  FileName = strArchiveName
  if not fso.FileExists(FileName) then
    echo "������: ���� "+FileName+" � ������� ����� �� ������"
    Exit Function
  end if

if (CopyTo="") or IsEmpty(CopyTo) then
  echo "---  ������: �� ������ �����, ���� ����������!  ---"
  Exit Function
end if

if Right(CopyTo,1)<>"\" then
  CopyTo = CopyTo +"\"
end if
if not fso.FolderExists(CopyTo) then
  if (CopyTo2="") or IsEmpty(CopyTo2) then
    echo "---  ������: ����������� ������ ��� ����� ��� �����������!  ---"
    Exit Function
  end if
  if Right(CopyTo2,1)<>"\" then
    CopyTo2 = CopyTo2 +"\"
  end if
  if fso.FolderExists(CopyTo2) then
    CopyTo = CopyTo2
  else
    echo "---  ������: ����������� ������ ��� ����� ��� �����������!  ---"
    Exit Function
  end if
end if

echo "�������� �������� ������ � ����� "+CopyTo+" ..."
on error resume next
'if exist "%CopyTo%\%3.zip" del /q /f "%CopyTo%\%3.zip"  >nul
fso.DeleteFile(CopyTo+strArchiveName)
err.Clear()
on error goto 0

echo "�������� ����������� ������ � ����� "+CopyTo+" ..."
'copy /y %1 "%CopyTo%"
On Error Resume Next
call fso.CopyFile(FileName,CopyTo,true)
on error goto 0
if err.Number<>0 then
  err.Clear()
'  echo "---  ������: �� ������� ����������� ����� � ����� "+CopyTo+"  ---"
'  Exit Function
  CopyTo = CopyTo2

  echo "�������� �������� ������ � ����� "+CopyTo+" ..."
  on error resume next
  'if exist "%CopyTo%\%3.zip" del /q /f "%CopyTo%\%3.zip"  >nul
  call fso.DeleteFile(CopyTo+strArchiveName)
  err.Clear()
  on error goto 0

  echo "�������� ����������� ������ � ����� "+CopyTo+" ..."
  'copy /y %1 "%CopyTo%"
  On Error Resume Next
  call fso.CopyFile(FileName,CopyTo,true)
  on error goto 0
  if err.Number<>0 then
    echo "---  ������: �� ������� ����������� ����� � ��� �����  ---"
    err.Clear()
    Exit Function
  end if
end if

  If bNeedBackup Then ' �������� ��������
    echo "�������� ������ ..."
    Dim res
'On Error Resume Next
'res = WshShell.Run(ZipCmdLine+FileName+" >nul",6,true)
    Dim sArcCmdLineTest
    sArcCmdLineTest=ExpandCmdLine(ArcCmdLineTest, CopyTo+strArchiveName, "","")
   Debug sArcCmdLineTest,"rar Cmd line"
' Debug ArcCmdLineTest+" "+CopyTo+strArchiveName,"rar Cmd line"
' res = wshShell.Run(ArcCmdLineTest+" "+CopyTo+strArchiveName,,true)
    res = wshShell.Run(sArcCmdLineTest,,true)
   Debug CStr(err.Number),"err.Number"
   Debug Cstr(res),"res"
'on error goto 0
    if (err.Number<>0) or (res>1) then
      echo "-- ������: ��� ������������ ������ � ����� "+CopyTo+" ���������� ������  --"
      err.Clear()
      Exit Function
    end if
    echo "�������� ��������� �������"
    err.Clear()
  end if

CopyArchive = true
echo "����� "+strArchiveName+" ������� ���������� � ����� "+CopyTo

End Function 'CopyArchive

' �������� ������ �� INI-�����
' ResDict - ������ Dictionary, ��� �������� ���� ����/��������
Function GetDataFromIniFile(ByVal IniFileName, ByRef ResDict)
  GetDataFromIniFile = false

Dim IniDict 'as Dictionary
Set IniDict = CreateObject("Scripting.Dictionary")

' �������� ������ !! ������ !! �����
' ��� �������� ������ � INI-�����
IniDict.Add "ZipCmdLine",         "ZipCmdLine"
IniDict.Add "ArcCmdLineTest",     "ArcCmdLineTest"
IniDict.Add "CopyFolder1",         "CopyFolder1"
IniDict.Add "CopyFolder2",         "CopyFolder2"
IniDict.Add "MoveFolder",         "MoveFolder"
IniDict.Add "LogFile",            "LogFile"
IniDict.Add "DebugFlag",          "DebugFlag"
IniDict.Add "ArcCmdLineAdd",     "ArcCmdLineAdd"

' ����� �������������
Dim IniFile 'As TextStream

On Error Resume Next
Dim ForRead
ForRead =1
Set IniFile = fso.OpenTextFile(IniFileName,ForRead)
if err.Number<>0 then
  Err.Clear()
  echo "Ini-���� "& IniFileName &" �� ������� �������!"
  Exit Function
end if
on error goto 0

Set ResDict = CreateObject("Scripting.Dictionary")
Dim s, Matches, Match
Dim reg 'As RegExp
Set reg = new RegExp
Dim elem, index
elem = IniDict.Items()
'Dim CommentReg 'As RegExp
'Set CommentReg = new RegExp
'CommentReg.Pattern="^\s*[;']"
''CommentReg.Pattern="^[;']"
'CommentReg.IgnoreCase = True

Do While IniFile.AtEndOfStream <> True
  s = IniFile.ReadLine
' ���� �� ������-�����������
'  if not CommentReg.Test(s) then
  if not RegExpTest("^\s*[;']",s) then
  For index=0 To IniDict.Count-1
'    reg.Pattern="\s*"+elem(index)+"\s*=\s*(.+)"
' �������� ���� � �������� � Ini-�����, ����� ��������� �����������
    reg.Pattern="^\s*"+elem(index)+"\s*=\s*([^;']+)[;']?"
    reg.IgnoreCase = True
    Set Matches = reg.Execute(s)
    if Matches.Count>0 then
' �������� ����� ����, �������� �� �������� ��������� � �����(� ������) �������
      ResDict.Add elem(index),Trim(replace(Matches(0).SubMatches(0),vbTab," "))
      Exit For ' ����� ����� ����, ������ ���������� �� ��������� �����
    end if
  Next 'index
  end if
Loop
IniFile.Close()

if ResDict.Count=0 then ' �� ����� �� ������ �����
  echo "�� ������� �������� ������ �� Ini-����� " & IniFileName
  GetDataFromIniFile = false
else
  GetDataFromIniFile = true
end if
End Function'GetDataFromIniFile

' �������� ������ �� INI-�����
' ResDict - ������ Dictionary, ��� �������� ���� ����/��������
Function GetConfDataFromIniFile(ByVal IniFileName, ByRef ResDict)
  GetConfDataFromIniFile = false

' ����� �������������
Dim IniFile 'As TextStream

On Error Resume Next
Dim ForRead
ForRead =1
Set IniFile = fso.OpenTextFile(IniFileName,ForRead)
if err.Number<>0 then
  Err.Clear()
  echo "Ini-���� "& IniFileName &" �� ������� �������!"
  Exit Function
end if
on error goto 0

Set ResDict = CreateObject("Scripting.Dictionary")
Dim s, Matches, Match
Dim reg 'As RegExp
Set reg = new RegExp
  reg.Pattern="^\s*([^=]+)\s*=\s*([^;']+)[;']?"
  reg.IgnoreCase = True

Dim elem, index

Do While IniFile.AtEndOfStream <> True
  s = IniFile.ReadLine
' ���� �� ������-�����������
  if not RegExpTest("^\s*[;']",s) then
'  For index=0 To IniDict.Count-1
'    reg.Pattern="\s*"+elem(index)+"\s*=\s*(.+)"
' �������� ���� � �������� � Ini-�����, ����� ��������� �����������
    Set Matches = reg.Execute(s)
    if Matches.Count>0 then
' �������� ����� ����, �������� �� �������� ��������� � �����(� ������) �������
'      ResDict.Add elem(index),Trim(replace(Matches(0).SubMatches(0),vbTab," "))
      ResDict.Add LCase(Trim(replace(Matches(0).SubMatches(0),vbTab," "))),Trim(replace(Matches(0).SubMatches(1),vbTab," "))
'      Exit For ' ����� ����� ����, ������ ���������� �� ��������� �����
    end if
'  Next 'index
  end if
Loop
IniFile.Close()

if ResDict.Count=0 then ' �� ����� �� ������ �����
  echo "�� ������� �������� ������ �� Ini-����� " & IniFileName
  GetConfDataFromIniFile = false
else
  GetConfDataFromIniFile = true
end if
End Function'GetConfDataFromIniFile

' �������� � ��������� ������ ������ %%a � %%f - ������ � ������ ��������
' ��������������
Function ExpandCmdLine(CmdLine, ArchiveName, Folder, Files)
'  const CSConfFileName="1cv7.md"
  Dim regEx

  Set regEx = New RegExp            ' Create regular expression.
  regEx.Pattern = "%%archive"            ' Set pattern.
  regEx.IgnoreCase = True            ' Make case insensitive.
  CmdLine = regEx.Replace(CmdLine, ArchiveName)   ' Make replacement.
  regEx.Pattern = "%%folder"            ' Set pattern.
  CmdLine = regEx.Replace(CmdLine, Folder)   ' Make replacement.
  regEx.Pattern = "%%files"            ' Set pattern.
  CmdLine = regEx.Replace(CmdLine, Files)   ' Make replacement.

  set regEx = Nothing
  ExpandCmdLine = CmdLine
End Function 'ExpandCmdLine

' ��������� �� ������������ �������
' ������� �������� �� �����
  Dim regExTest               ' Create variable.
Function RegExpTest(ByVal patrn, ByVal strng)
  if IsEmpty(regExTest) then
    Set regExTest = New RegExp         ' Create regular expression.
  end if
  regExTest.Pattern = patrn         ' Set pattern.
  regExTest.IgnoreCase = true      ' disable case sensitivity.
  RegExpTest = regExTest.Test(strng)      ' Execute the search test.
'  regEx = null
End Function

Sub Echo(text)
  'WScript.Echo(text)
	Message text, mNone

  If IsObject(LogFile) then        'LogFile should be a file object
    LogFile.WriteLine text
  end if
End Sub'Echo

Sub Debug(ByVal title, ByVal msg)
on error resume next
  DebugFlag = DebugFlag
  if err.Number<>0 then
    err.Clear()
    on error goto 0
    Exit Sub
  end if
  if DebugFlag then
    if not (IsEmpty(msg) or IsNull(msg)) then
      msg = CStr(msg)
    end if
    if not (IsEmpty(title) or IsNull(title)) then
      title = CStr(title)
    end if
    If msg="" Then
      Echo(title)
    else
      Echo(title+" - "+msg)
    End If
  End If
on error goto 0
End Sub'Debug

Sub DebugVar(NameVar)
on error resume next
  Dim a
  a = Eval(NameVar)
  if err<>0 then
    err.Clear()
    Exit Sub
  end if
  if IsNull(a) or IsEmpty(a) then
    call Debug(NameVar,"null")
  else
    call Debug(NameVar,a)
  end if
on error goto 0
End Sub 'DebugVar

'********************************************************************
'*
'* Sub WriteLine()
'* Purpose: Writes a text line either to a file or on screen.
'* Input:   strMessage  the string to print
'*          objFile     an output file object
'* Output:  strMessage is either displayed on screen or written to a file.
'*
'********************************************************************
Sub WriteLine(ByVal strMessage, ByVal objFile)

    On Error Resume Next
    If IsObject(objFile) then        'objFile should be a file object
        objFile.WriteLine strMessage
    Else
        Call Echo( strMessage )
    End If

End Sub

'********************************************************************
'*
'* Function blnErrorOccurred()
'*
'* Purpose: Reports error with a string saying what the error occurred in.
'*
'* Input:   strIn               string saying what the error occurred in.
'*
'* Output:  displayed on screen
'*
'********************************************************************
Private Function blnErrorOccurred (ByVal strIn)

    If Err.Number Then
        Call Echo( "������ 0x" & CStr(Hex(Err.Number)) & ": " & strIn)
        If Err.Description <> "" Then
            Call Echo( "�������� ������: " & Err.Description)
        End If
        Err.Clear
        blnErrorOccurred = True
    Else
        blnErrorOccurred = False
    End If

End Function

'********************************************************************
'*
'* Function blnOpenFile
'*
'* Purpose: Opens a file.
'*
'* Input:   strFileName         A string with the name of the file.
'*
'* Output:  Sets objOpenFile to a FileSystemObject and setis it to
'*            Nothing upon Failure.
'*
'********************************************************************
Private Function blnOpenFile(ByVal strFileName, ByRef objOpenFile)

    ON ERROR RESUME NEXT

    If IsEmpty(strFileName) OR strFileName = "" Then
        blnOpenFile = False
        Set objOpenFile = Nothing
        Exit Function
    End If

    'fso.DeleteFile(strFileName)
    'Open the file for output
    Set objOpenFile = fso.CreateTextFile(strFileName, True)
    If blnErrorOccurred("���������� �������") Then
        blnOpenFile = False
        Set objOpenFile = Nothing
        Exit Function
    End If
    blnOpenFile = True

End Function

''********************************************************************
''*
''* Sub      VerifyHostIsCscript()
''*
''* Purpose: Determines which program is used to run this script.
''*
''* Input:   None
''*
''* Output:  If host is not cscript, then an error message is printed
''*          and the script is aborted.
''*
''********************************************************************
'Sub VerifyHostIsCscript()
'
'    ON ERROR RESUME NEXT
'
'    'Define constants
'    CONST CONST_ERROR                   = 0
'    CONST CONST_WSCRIPT                 = 1
'    CONST CONST_CSCRIPT                 = 2
'
'    Dim strFullName, strCommand, i, j, intStatus
'
'    strFullName = WScript.FullName
'
'    If Err.Number then
'        Call Echo( "Error 0x" & CStr(Hex(Err.Number)) & " occurred." )
'        If Err.Description <> "" Then
'            Call Echo( "Error description: " & Err.Description & "." )
'        End If
'        intStatus =  CONST_ERROR
'    End If
'
'    i = InStr(1, strFullName, ".exe", 1)
'    If i = 0 Then
'        intStatus =  CONST_ERROR
'    Else
'        j = InStrRev(strFullName, "\", i, 1)
'        If j = 0 Then
'            intStatus =  CONST_ERROR
'        Else
'            strCommand = Mid(strFullName, j+1, i-j-1)
'            Select Case LCase(strCommand)
'                Case "cscript"
'                    intStatus = CONST_CSCRIPT
'                Case "wscript"
'                    intStatus = CONST_WSCRIPT
'                Case Else       'should never happen
'                    Call Echo( "An unexpected program was used to " _
'                                       & "run this script." )
'                    Call Echo( "Only CScript.Exe or WScript.Exe can " _
'                                       & "be used to run this script." )
'                    intStatus = CONST_ERROR
'                End Select
'        End If
'    End If
'
'    If intStatus <> CONST_CSCRIPT Then
'        Call Echo( "Please run this script using CScript." & vbCRLF & _
'             "This can be achieved by" & vbCRLF & _
'             "1. Using ""CScript SystemAccount.vbs arguments"" for Windows 95/98 or" _
'             & vbCRLF & "2. Changing the default Windows Scripting Host " _
'             & "setting to CScript" & vbCRLF & "    using ""CScript " _
'             & "//H:CScript //S"" and running the script using" & vbCRLF & _
'             "    ""SystemAccount.vbs arguments"" for Windows NT/2000/XP." )
'        WScript.Quit(0)
'    End If
'End Sub 'VerifyHostIsCscript

Function GetIniFileName(IniFileExtension)

	if Right(gsIniFilesFolder,1) <> "\" then
		gsIniFilesFolder = gsIniFilesFolder + "\"
	end if

  Set fso = CreateObject("Scripting.FileSystemObject")

' ������ ��� ini-�����
  Dim IniFileName
	Dim ScriptName, ScriptFullName

	ScriptName = gsScriptName + ".ini" ' "MD to Archiv.ini"
'	ScriptFullName = "D:\Cmd\MD to Archiv.ini"
	ScriptFullName = gsIniFilesFolder + ScriptName

' ���� ini-���� ���� � �������� ������������, ������������ ���
  Dim sConfNameInBase
  sConfNameInBase=".\"+Replace(LCase(ScriptName),".ini", IniFileExtension)
  if fso.FileExists(sConfNameInBase) then
    IniFileName = sConfNameInBase
  else
    IniFileName = Replace(LCase(ScriptFullName),".ini", IniFileExtension)
  end if
'Debug "IniFileName", IniFileName

	GetIniFileName = IniFileName

End Function ' GetIniFileName

' ������ ��� �������� �������� ����� MD to Archiv.ini2
' ��������� ������������ ��� �������� MD-����� �� ��������
Function OpenUnLoadConfigsIniFile( )
	Documents.Open GetIniFileName(".ini2")
End Function

' ������ ��� �������� �������� ����� MD to Archiv.ini
' ��������� �������� (��������� ������, ����� ��������, ���-����)
Function OpenParametersIniFile( )
	Documents.Open GetIniFileName(".ini")
End Function

' ������ ��� �������� �������� ����� Manage1C_Config.ini
' ��������� �������� (��������� ������, ����� ��������, ���-����)
Function OpenLoadIniFile( )
	Documents.Open "D:\Cmd\Manage1C_Config.ini"
End Function