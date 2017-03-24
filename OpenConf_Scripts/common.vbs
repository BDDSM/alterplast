$NAME Common
' -----------------------------------------------------------------------------
'         ����� ����� �������� ��� ������������� � ��������
'
'         ��� e-mail: artbear@bashnet.ru
'         ��� ICQ: 265666057
' -----------------------------------------------------------------------------
'
' 	� ����� �������� ����� ������������ ��� ���:
'		CommonScripts = scripts("common")
'		set doc = CommonScripts.GetTextDoc(0)
'
' -----------------------------------------------------------------------------

Dim QuietMode
Dim LastErrorStr

QuietMode = false
LastErrorStr = ""

Function SetQuietMode(bNewMode)
	SetQuietMode = QuietMode
	QuietMode = bNewMode
End Function

Sub MyMsgBox(Text)
	If QuietMode Then
		LastErrorStr = Text
	Else
		MsgBox Text
	End If
End Sub

Function GetLastError(Param)
	GetLastError = LastErrorStr
End Function

Sub Echo(text)
  Message text, mNone
End Sub ' Echo

' -----------------------------------------------------------------------------
Sub Error(text)
  Message text, mError
End Sub ' Error

' -----------------------------------------------------------------------------
Sub EchoError(text)
  Message text, mRedErr
End Sub ' EchoError

' -----------------------------------------------------------------------------
' �������� ������ �������� ���� (��� ������), ���� ���� �������� ������ �� �������
' � ������ ������ ���������� ������ TextDoc
' � ������ ������ ���������� Nothing
' param  ������ ��������, ����� �� ������� � �������
'
Function GetTextDoc(param)

  set GetTextDoc = Nothing

  If Windows.ActiveWnd Is Nothing Then
     MyMsgBox "��� ��������� ����"
     Exit Function
  End If
  Set doc = Windows.ActiveWnd.Document
  If doc=docWorkBook Then Set doc=doc.Page(1)

  If doc<>docText Then
     MyMsgBox "���� �� ���������"
     Exit Function
  End If

  set GetTextDoc = doc
End Function ' GetTextDoc

' -----------------------------------------------------------------------------
' �������� �������� ������ (����� ��� ������)
' � ������ ������ ���������� ������ TextDoc
' � ������ ������ ���������� Nothing
' param  ������ ��������, ����� �� ������� � �������
' bActivateTextModule = 1 -> ���� �� ����� � ���� ������, ��������� � ����
' bActivateTextModule = 0 -> ���� �� ����� � ���� ������, �������� ������
'
' ������ ������������ � ������� ��������� �� ������ (������ - �������� �� ������.vbs)
'
Function GetTextDocIfOpened(bActivateTextModule)

  set GetTextDocIfOpened = Nothing

  If Windows.ActiveWnd Is Nothing Then
     MyMsgBox "��� ��������� ����"
     Exit Function
  End If
  Set doc = Windows.ActiveWnd.Document
  If doc=docWorkBook Then
	 	if (bActivateTextModule = 0) and (Doc.ActivePage <> 1) then
    	MyMsgBox "���� �� ���������"
    	Exit Function
    elseif (bActivateTextModule = 1) then
  		Doc.ActivePage = 1
  	end if
	 	Set doc=doc.Page(1)
  end if
  If doc<>docText Then
     MyMsgBox "���� �� ���������"
     Exit Function
  End If

  set GetTextDocIfOpened = doc
End Function ' GetTextDocIfOpened

' -----------------------------------------------------------------------------
' �������� ������ �� ��������
' ���� ������ �����������, ������������ Nothing
'
' ������-�� �����������
'          set GetPlugin = Plugins(PluginName)
' �� ����� ��������,  ���� PluginName - ������� ��������� ����������
' ��� ������� ��� ����� ��������� �� ����������
'
Function GetPluginByName(PluginName)
  set GetPluginByName = Nothing

  err.Clear
  on error resume next
'  temp = Plugins(PluginName)
  temp = Plugins(CStr(PluginName))
  iErrNumber = Err.Number
  on error goto 0
'  if err.Number = 438 then ' ����� ������ ������������� ����������
  if iErrNumber = 438 then ' ����� ������ ������������� ����������
    err.Clear
    set GetPluginByName = Plugins(CStr(PluginName))
    Exit Function
  elseif iErrNumber = 0 then
    set GetPluginByName = Plugins(CStr(PluginName))
  end if
End Function ' GetPluginByName

' -----------------------------------------------------------------------------
' �������� ������ �� ��������
' ���� ������ �����������, ������������ Nothing
'
' ������-�� �����������
'          set GetScript = Scripts(ScriptName)
' �� ����� ��������,  ���� ScriptName - ������� ��������� ����������
' ��� ������� ��� ����� ��������� �� ����������
'
Function GetScriptByName(ScriptName)
  set GetScriptByName = Nothing

  on error resume next
  Err.Clear
  temp = Scripts(CStr(ScriptName))
  iErrNumber = err.Number
  on error goto 0
'  if err.Number = 438 then ' ����� ������ ������������� ����������
  if iErrNumber = 438 then ' ����� ������ ������������� ����������
    err.Clear
    set GetScriptByName = Scripts(CStr(ScriptName))
    Exit Function
  elseif iErrNumber = 0 then
    set GetScriptByName = Scripts(CStr(ScriptName))
  end if
End Function ' GetScriptByName

Function GetScriptIndexByName(ScriptName)
	set GetScriptIndexByName = nothing
  iScriptCount=Scripts.Count-1
  for i=0 to iScriptCount
    sScriptName = Scripts.Name(i)
	if Lcase(sScriptName) = LCase(trim(ScriptName)) then
		GetScriptIndexByName = i
		Exit Function
	end if
  Next
End Function ' GetScriptIndexByName

' ������������� �������. param - ������ ��������,
' ����� �� ������� � �������
'
' ��� �������� ������� �������������� ���
'Init SelfScript, "������ 1�++", "Wizard1CPP", Wizard1CPP
'
Sub InitScript(Script, ScriptName, PrefixForEvents, ByRef ScriptVariable)
  'set ScriptVariable1 = GetPluginByName(CStr(ScriptName))
  set ScriptVariable1 = GetScriptByName(CStr(ScriptName))
  if not ScriptVariable1 Is Nothing Then
      ' ����������� ������ � �������� �������
      set ScriptVariable = ScriptVariable1

      on error resume next
      Err.Clear
      Script.AddNamedItem PrefixForEvents, ScriptVariable, False

      if iErrNumber = -2147024809 then ' ��� ��� ����������������
        Echo "������ � ������� " & Script.Name & ": " & Err.Description & vbCRLF _
          & "���������� ������������ ������ ������� ��� �������!"
        ' �������� � ������
        Scripts.Unload Script.Name
        Err.Clear
      end if
  else
      ' ������ �� ��������. �������� � ������
      Scripts.Unload Script.Name
  End If
End Sub ' InitScript

' ��������� �� ������������ ������� (����������� ���������)
' ������� �������� �� �����
  Dim regExTest
Function RegExpTest(ByVal patrn, ByVal strng)
  if IsEmpty(regExTest) then
    Set regExTest = New RegExp         ' Create regular expression.
  end if
  regExTest.Pattern = patrn         ' Set pattern.
  regExTest.IgnoreCase = true      ' disable case sensitivity.
  RegExpTest = regExTest.Test(strng)      ' Execute the search test.
End Function

' ���������� �� � ��������� ������� ������ � ����� ������
Function MacrosExists(ScriptName, MacrosName)
  MacrosExists = false

  on error resume next
  Err.Clear
  Set e = CreateObject("Macrosenum.Enumerator")
  if Err.Number <> 0 then
  	Echo "�� ������� ������� ������ Macrosenum.Enumerator" & vbCRLF & _
  		"��������������� ��������������� dll(ocx) ����"
  	Exit Function
  end if
  on error goto 0

  Set script = GetScriptByName(ScriptName)
  if script is Nothing then
    Exit Function
  end if

  arr=e.EnumMacros(script)                        ' ��������� ������� �������� �������
  for j=0 to ubound(arr)
    if trim(LCase(arr(j))) = trim(LCase(MacrosName)) then
      MacrosExists = True
      Exit Function
    End if
  Next

End Function ' MacrosExists

' ��������� ��������� ������
Sub CallMacros(sScriptName, sMacrosName)
'    On Error Resume Next
    Cmd = "Scripts(""" & sScriptName & """)." & "" & sMacrosName & ""
'stop
    Eval "" & Cmd & ""
'	iErrNumber = Err.Number
'    On Error GoTo 0
End Sub ' CallMacros

' ��������� ���������
Sub RunCommand(ProgramPath, sArguments, bWait)
	set wshShell = createObject("WScript.Shell")
	CmdLine = ProgramPath & " " & sArguments

	on Error resume next
	wshShell.Run CmdLine, 1, bWait
	if Err.Number <> 0 then
		WshShell.Popup("������: �� ����o�� ��������� ���������" & _
			vbCRLF & vbCRLF & CmdLine & vbCRLF & vbCRLF & _
			"��������, ��������� ���� ��� �� ����������, ��� �� ��������� �� ���� ������ (���� ������� ������ ��� ���������)")
		Err.Clear
	end if
	on Error goto 0
End Sub ' RunCommand

' ��������� ��������� � ��������� �� ����������
Sub RunCommandAndWait(ProgramPath, sArguments)
	RunCommand ProgramPath, sArguments, true
End Sub ' RunCommandAndWait

' ������� ������� ��� �������������
' ����� ����, ��� OpenConf.SendCommand ������ PostMessage(WM_COMMAND,�����,0)
' ����� ������� �������������� ��������� ��������� Windows
' ���� ��� ������� ����� ����� ����� ��������� ����� (cscript.exe)
'
Sub SendCommand(iCommand)
	Configurator.SendCommand(iCommand)

	set wshShell = createObject("WScript.Shell")
	CmdLine = "cscript "
	wshShell.Run CmdLine, 0, true
End Sub ' SendCommand


' ������ �������������:
'----------------------
'		Set Pos = CommonScripts.GetDocumentPosition(Doc)
'		�����-�� ���, ������� ������ ������� �������
'		Pos.Restore
'----------------------

Class TDocumentPosition
	Private Doc
	Private Line1
	Private Col1
	Private Line2
	Private Col2

	Public Sub Save(aDoc)
		If aDoc Is Nothing then
			Exit Sub
		End if
		set Doc = aDoc
		Line1 = Doc.SelStartLine
		Col1 = Doc.SelStartCol
		Line2 = Doc.SelEndLine
		Col2 = Doc.SelEndCol
	End Sub

	Public Sub Restore()
		If not Doc Is Nothing Then
			Doc.MoveCaret Line1, Col1, Line2, Col2
		end if
	End Sub
End Class ' TDocumentPosition

Function GetDocumentPosition(Doc)
	Set GetDocumentPosition = new TDocumentPosition
	GetDocumentPosition.Save(Doc)
End Function ' GetDocumentPosition

Function SelectFileForRead(sFileName, sFilesFilter)
	SelectFileForRead = ""

    Set srv = CreateObject("Svcsvc.Service")
    'sFileName = srv.SelectFile(False, sFileName, "��������� �����|*.txt|��� �����|*", False)
    sDstFileName = srv.SelectFile(False, sFileName, sFilesFilter, False)
    If sDstFileName = "" Then
        Exit Function
    End If

    Set fs = CreateObject("Scripting.FileSystemObject")
    If Not fs.FileExists(sDstFileName) Then
        Error "��������� ���� �� ����������"
        Exit Function
    End If

	SelectFileForRead = sDstFileName
End Function ' SelectFileForRead

' ADirks
'������� ������ ��� ������ ������ �� ������ ���������. ��� ����������� ���� ������ ������������ ActiveX-������
' SelectValue.dll �� IAm ��� svcsvc.dll �� ���������� ��������. ���� �� ���� ������ �� ���������������, ��
' ������� ���������.
' - ValueList - ������, ���������� �������� ��� ������. �������� ��������� �������� vbCrLf ��� �������� ','
' - Caption   - ������, ���������� ��������� ���� ������
' ������������ ��������: ��������� ������. ���� ������ �� ������� (������������ ����� Esc), �� ������������ ������ ������
Function SelectValue(ValueList, Caption)
	SelectValue = ""
	Set SelObj = Nothing
	SelType = ""

	On Error Resume Next
	'��������� ������� SvcSvc.Service, ����� SelectValue.SelectVal, � ���� �� ����������, �� ������ ����������� ������������, ��� �� ������ �� �����
	Set SelObj = CreateObject("SvcSvc.Service")
	If Not SelObj Is Nothing Then
		SelType = "SvcSvc"
	Else
		Set SelObj = CreateObject("SelectValue.SelectVal")
		If Not SelObj Is Nothing Then SelType = "SelectValue"
	End If
	On Error Goto 0

	If SelObj Is Nothing Then
		Message "�� ���������� (��� �� ���������������) ActiveX ������ ��� ����������� ������ ������!", mExclamation
		Message "	������ SvcSvc.dll ����� ����� �� ������ http://openconf.itland.ru/beta/", mInformation
		Message "	������ SelectValue.dll ����� ����� � ������ 'Intellisence for OpenConf' �� IAm, �� ������ http://1c.proclub.ru/modules/mydownloads/personal.php?cid=1002&lid=4229", mInformation
		Exit Function
	End If

	If InStr(ValueList, vbCrLf) = 0 Then
		ValueList = Replace(ValueList, ",", vbCrLf)
	End If
	If SelType = "SelectValue" Then
		SelectValue = SelObj.SelectPopUp(ValueList, Windows.ActiveWnd.HWnd, vbCrLf)
	Else
		SelectValue = -1
		On Error Resume Next
		SelectValue = SelObj.FilterValue(ValueList, 1, Caption)
		If SelectValue = -1 Then
			SelectValue = SelObj.SelectValue(ValueList, Caption, false)
		End If
		On Error Goto 0
	End If
End Function

' ADirks
'������� ��������� � ����� N ���� ����� �� ���������� ������ width
Private Function MyFormatNumber(N, width)
	MyFormatNumber = Trim(CStr(N))
	do while len(MyFormatNumber) < width
		MyFormatNumber = "0" & MyFormatNumber
	loop
End Function

' ADirks
'������� �������������� ����
'� ��������� ������ ����� �������� ���� 'd', 'm', 'MMMM', 'y'
'	���������� �������� 'd', 'm' � 'y' ����� ���� ����� - �����. ����� ���� �����
'	��������� ����� ������ �� ������ �����
'	MMMM ����� �������� �� �������� ������
Function FormatDate(aDate, Format)
	Dim format_d, format_m, format_y
	Dim d, m, y

	Set RE = New RegExp
	RE.IgnoreCase = true

	format_d = ""
	format_m = ""
	format_y = ""


	RE.Pattern = "[^d]*(d*)"
	RE.IgnoreCase = true
	Set Matches = RE.Execute(Format)
	If Matches.Count > 0 Then format_d = Matches(0).SubMatches(0)

	RE.Pattern = "[^m]*(m*)"
	Set Matches = RE.Execute(Format)
	If Matches.Count > 0 Then format_m = Matches(0).SubMatches(0)

	RE.Pattern = "[^y]*(y*)"
	Set Matches = RE.Execute(Format)
	If Matches.Count > 0 Then format_y = Matches(0).SubMatches(0)

	d = MyFormatNumber(DatePart("d", aDate), len(format_d))
	If format_m = "MMMM" Then
		m = MonthName(DatePart("m", aDate))
	Else
		m = MyFormatNumber(DatePart("m", aDate), len(format_m))
	End If
	y = MyFormatNumber(DatePart("yyyy", aDate), len(format_y))

	FormatDate = Format
	FormatDate = Replace(FormatDate, format_d, d)
	FormatDate = Replace(FormatDate, format_m, m)
	FormatDate = Replace(FormatDate, format_y, y)
End Function

' Orefkov
' ������ ����������� ��������� ��������� �������. ��������,
' 	TogglePanel "�������-��������"
' 	TogglePanel "���� ���������"
' 	TogglePanel "������ ��������� ���������"
' 	TogglePanel "�����������"
Sub TogglePanel(PanelName)
	Windows.PanelVisible(PanelName)=Not Windows.PanelVisible(PanelName)
End Sub

' �������� ������� �� � ���������� �������� (�������� ��� Configurator.IBDir - ��� � ��������� ��������)
Function GetIBDir()

	set fso = CreateObject("Scripting.FileSystemObject")
 	set Folder = fso.GetFolder(IBDir)
 	GetIBDir = Folder.Path + "\"

End Function