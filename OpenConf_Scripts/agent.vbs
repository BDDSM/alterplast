$NAME ���������

' (c) Orefkov
' �������� ������ �������������
' SelfScript.AddNamedItem,
' SelfScript.Name,
' Configurator_OnIdle,
' Documents.EnumAllTypedObj

' ��� ������ ������ ���� ���������� MS Agent
' �� ������ ����� ������� ������� ������ ��������� ������������ ����
' �� ������ ����� ������� c Ctrl ������� ������, ��� �������
' �� ������ ��������� "�������" ������

Dim helper          ' ��������
Dim AllObjects      ' ��� ����������� ������� ������������
Dim LastX, LastY    ' ���������� ��� ������ ����
Dim RecentForms()   ' ������ � ���������� �������
Dim rcFormsCount    ' ���������� ��������� � �������
Dim MaxRcForms      ' ������������ ���������� � ������ ��������� ����
Dim SelectMode      ' �����, � ������� ������ ��������� ����

' �������� ���������
Sub ShowAgent()
'    helper.Show
'    helper.MoveTo 100, 100
End Sub

Sub Configurator_AllPluginsInit()
    ShowAgent
End Sub

' ���������� OpenConf�� ��� "�������", �� � ��� ������ �����
' ��������� ���������� ��� ��������� �� ����� ������� ���������
Sub Configurator_OnIdle()
    ' ����� ������������ ��� ����� ��� ����, ����� ������� �������
    ' �������� ���� � ������, ��� ����,���� ��� ��� ���� � ������,
    ' ���� ����������� ��� ������ ������.
    ' ������� �������� ����
    Set wnd = Windows.ActiveWnd
    If wnd Is Nothing Then Exit Sub
    ' ������� �������� ����
    Set doc = wnd.Document
    ' ������ �������� ������ � ������� � ��������
    If doc = docUnknown Then Exit Sub
    ' ���������� ������, ����������� ��������
    docFull = CStr(doc.id) & "##"
    If doc.id < 0 Then
        ' ������� ����
        docFull = docFull & doc.Path
    Else
        ' ������ ������������
        Name = doc.Name
        If doc.Kind = "GlobalData" Then Name = "����� �������"
        docFull = docFull & Name & "##" & CStr(doc.Type) & "##" & CStr(doc.Kind) & "##" & doc.Path
    End If
    docFull = docFull & "##"
    ' ������ ������ �������� � ������ ���������
    For i = 0 To rcFormsCount - 1
        If InStr(RecentForms(i), docFull) = 1 Then
            For j = i + 1 To rcFormsCount - 1
                RecentForms(j - 1) = RecentForms(j)
            Next
            rcFormsCount = rcFormsCount - 1
            Exit For
        End If
    Next
    If rcFormsCount = MaxRcForms Then
        For i = 1 To rcFormCount - 1
            RecentForm(i - 1) = RecentForm(i)
        Next
        rcFormsCount = rcFormsCount - 1
    End If
    If doc = docWorkBook Then docFull = docFull & CStr(doc.ActivePage)
    RecentForms(rcFormsCount) = docFull
    rcFormsCount = rcFormsCount + 1
End Sub

' ������������� ������
Function InitAgent(DummyParam)   ' ������ ��������, ����� �� ������� � �������
    InitAgent = False
    On Error Resume Next
    Set a = CreateObject("Agent.Control.2")
    If Err <> 0 Then MsgBox "����� �� ����������": Exit Function
    a.Connected = True
    ' �������� ��������� �������� Merlin
    a.Characters.Load "Merlin", "merlin.acs"
    If Err <> 0 Then
        Err.Clear
        ' �������� ��������� �������� �� ���������
        a.Characters.Load "Merlin"
    End If
    If Err <> 0 Then MsgBox "��� �� ������ ���������": Exit Function

    ' ��������� ��������� ������ � ������������ ���� �������.
    ' ������ ����� ����� � ����� ����� ������� ��������� � ����� �������
    ' �� ���������� ����� � ����������� � ��� �������� (������ With Events � VB)
    ' ���� ������ False ������� True, �� ������ � �������� ������������ �������
    ' ����� �������� ��������, ��� �������� ��� �����,
    ' ��� ��������, � ������� Configurator
    SelfScript.AddNamedItem "Agent", a, False

    Set helper = Agent.Characters("Merlin")
    helper.AutoPopupMenu = False
    InitAgent = True
End Function

' ����������� ������� �� ������
' ���������� �� ������ �� ������
Sub Agent_Click(id, btn, sh, x, y)
    Set cmd = helper.Commands
    cmd.RemoveAll
    helper.StopAll
    LastX = x
    LastY = y
    If btn = 1 Then
        If sh = 0 Then
            FillCommandsLeftClick cmd
        ElseIf sh = 2 Then
            FillCommandsLeftClickCtrl cmd
        End If
    ElseIf btn = 2 Then
        FillCommandsRightClick cmd
    End If
    cmd.Add "ShowHelp", "��� ���...", , True, True
    helper.ShowPopupMenu x, y
End Sub

' ���������� ��� ������ ������� ���� ���������
Sub Agent_Command(UserInput)
    PlayMagic = False
    If UserInput.Name = "ShowHelp" Then
        helper.Speak "������. � ���� ��������. ������ �� ��� ����� ������� ����, " & _
            "� � ������ ������� ���� ��������� ������������� ����� ����� � ������. " & _
            "���� ��� ���� �� ��������� Ctrl, � ������ ������� ���� ����� ����� ��� ������. " & _
            "�� ������ ������ ������� ���� � ����� ��������� ��������� �������� ������"
        Exit Sub
    End If
    Select Case SelectMode
        Case 1
            PlayMagic = OnSelectCmd1(UserInput)
        Case 2
            PlayMagic = OnSelectCmd2(UserInput)
        Case 3
            PlayMagic = OnSelectCmd3(UserInput)
    End Select
    If PlayMagic = True Then helper.Play "DoMagic1"
End Sub

Sub Agent_DragComplete(CharacterID, Button, Shift, x, y)
    helper.Speak "����, ����, �������!!!"
End Sub

' ���������� ������ ������ ��� ������ ������ �������
Sub FillCommandsRightClick(Commands)
    SelectMode = 1
    Names = Array("Conf", "GM", "CommonTables", "Synt", "Assist")
    captions = Array("������� ������������", "���������� ������", "����� �������", _
        "������ �������-��������", "����� � �������-���������")
    For i = 0 To 4
        Commands.Add Names(i), captions(i), , True, True
    Next
End Sub

' ��������� ������ ������� ��� ������ ������ �������
Function OnSelectCmd1(UserInput)
    OnSelectCmd1 = True
    Select Case UserInput.Name
        Case "Conf"
            SendCommand cmdOpenConfigWnd
        Case "Synt"
            SendCommand 33884   'FullSynt
        Case "Assist"
            SendCommand 33879   'Assist
        Case "GM"
            Documents("����������������").Open
        Case "CommonTables"
            Documents("�������").Open
        Case Else
            OnSelectCmd1 = False
    End Select
End Function


Sub FillCommandsLeftClickCtrl(Commands)
    SelectMode = 2
    helper.Think "��� �� ������ �������?.."
    ' �������� ��� ����������� ������� �����
    ' ���������� ������, � ������� ����������� ��� ����� � ������ ������������
    ' ����� �� ��������� ��������, ����� Count
    ' For i=0 to AllObjects.Count-1
    '   Set doc=AllObjects(i)
    ' �� � ���� ������ ������� ����� ���� � ��� �������,
    ' � ������� ��� ������������ ����� 1�
    ' OpenConf ��������� ��������� ������� ������������ ��
    ' ����� �������� � ��������
    ' ��� ����� ������ ��������
    ' ChildCount(Parent)        - ���������� ���������� ����� ���������� ��������
    ' ChildsName(Parent,index)  - ���������� ��� �������� ������ � �������� �������
    ' ObjectCount(Parent)       - ���������� ���������� �������� � ����� ��������
    ' �������� - ������, � ������� ����� ����� ����������� ����� �����
    ' ����� �� ��������� ����� ������� � ���� TurboMD �� ������� "����������",
    ' �� ����������� ����, ��� �� ����������� �������� ������ � ������ �����
    Set AllObjects = Documents.EnumAllTypedObj()
    ' ���������� ������ �������� ������� ������
    For i = 0 To AllObjects.ChildsCount("") - 1
        Name = AllObjects.ChildName("", i)
        Commands.Add Name, Name, , True, True
    Next
End Sub

Function OnSelectCmd2(UserInput)
    OnSelectCmd2 = False
    helper.Think helper.Commands(UserInput.Name).Caption & "..."
    Set cmd = helper.Commands
    Parent = UserInput.Name
    Do
        cmd.RemoveAll
        Count = AllObjects.ChildsCount(Parent) - 1
        If Count < 0 Then
            LastAdded = Parent
        Else
            For i = 0 To Count
                Name = AllObjects.ChildName(Parent, i)
                FullName = Parent & "." & Name
                cmd.Add FullName, Name, , True, True
                LastAdded = FullName
            Next
        End If
        If cmd.Count = 1 Then Parent = LastAdded Else Exit Do
    Loop While 1 = 1
    If helper.Commands.Count < 1 Then
        Set doc = AllObjects(LastAdded)
        doc.Open
        OnSelectCmd2 = True
        Set AllObjects = Nothing
    End If
    If Not OnSelectCmd2 Then helper.ShowPopupMenu LastX, LastY
End Function


Sub FillCommandsLeftClick(Commands)
    SelectMode = 3
    For i = rcFormsCount - 1 To 0 Step -1
        Name = RecentForms(i)
        delimFirst = InStr(Name, "##") + 2
        delimNext = InStr(delimFirst, Name, "##")
        ShortName = Mid(Name, delimFirst, delimNext - delimFirst)
        Commands.Add Name, ShortName, , True, True
    Next
End Sub

Function OnSelectCmd3(UserInput)
    OnSelectCmd3 = True
    docFull = UserInput.Name
    del = InStr(docFull, "##")
    id = CLng(Left(docFull, del - 1))
    docFull = Mid(docFull, del + 2)
    Set doc = Nothing
    On Error Resume Next
    If id > 0 Then
        del = InStr(docFull, "##")
        Name = CLng(Left(docFull, del - 1))
        docFull = Mid(docFull, del + 2)

        del = InStr(docFull, "##")
        typedeoc = CLng(Left(docFull, del - 1))
        docFull = Mid(docFull, del + 2)

        del = InStr(docFull, "##")
        Kind = Left(docFull, del - 1)
        docFull = Mid(docFull, del + 2)

        del = InStr(docFull, "##")
        Path = Left(docFull, del - 1)
        docFull = Mid(docFull, del + 2)
        Set doc = Documents.DocFromID(id, typedoc, Path, Kind)
    Else
        del = InStr(docFull, "##")
        Path = Left(docFull, del - 1)
        docFull = Mid(docFull, del + 2)
        Set doc = Documents.Open(Path)
    End If
    If doc Is Nothing Then
        Message "�������� " & UserInput.Caption & " �� ������.", mRedErr
        OnSelectCmd3 = False
        For i = 0 To rcFormsCount - 1
            If RecentForms(i) = UserInput.Name Then
                For j = i + 1 To rcFormsCount - 1
                    RecentForms(j - 1) = RecentForms(j)
                Next
                rcFormsCount = rcFormsCount - 1
                Exit For
            End If
        Next
    Else
        doc.Open
        If doc = docWorkBook Then doc.ActivePage = CLng(docFull)
    End If
End Function


If InitAgent(0) = False Then Scripts.Unload SelfScript.Name
rcFormsCount = 0
MaxRcForms = 10
ReDim RecentForms(MaxRcForms)
