$NAME ����� ����������
'
' ������ ������� aka GrayT mail: tswsoft@tamb.ru
'
'������ ������ � ��������� ������������� ��� ������������ ������ ���������
'(���������� � ���������). �.�. ������ ����� ������������ �����������
'��������� ���������� ������� - ���� ����������, ������� � ���������, ����
'������� ��������� � �.�. � ������ ���������� ��������� ������������
'�����������, ���������, ������ � �.�.
'��� ������ � ����� ���������� ������� ������ � ������� �� ������������
'���������
'
'��� ������ ��������� � ������� ������������
'ActivX �� ���������� �������� - svcsvc.dll.
'
'������� ������:
'1.������������ svcsvc.dll (regsvr32 svcsvc.dll)
'2.��������� ��������� DescriptSubSystem.ert. � �� ������� ������������
'������ ��������� � �������� ������������, ����������� � ������ �����������.
'������ ����������� � �������� ������������ � ����� DescriptSubSystem.xml
'3.� ������������� ��������� ChoiceSubSystem.vbs:.. ������ ����������� :)

Dim tree

Sub ReadFromXML(Pusto)
set Objc= CreateObject("MSXML2.DOMDocument")
If not Objc.Load(IbDir & "DescriptSubSystem.xml") then
	MsgBox ("��� ����� �������� ��������� DescriptSubSystem.xml" & vbCrLf &_
                "��� ������������ ����� �����������" & vbCrLf &_
                "��������� DescriptSubSystem.ert ")
	Exit sub
End if

set root=Objc.ChildNodes.item(1)
set ListSubRoot = root.ChildNodes
maxCount = ListSubRoot.length-1
for i=0 to maxCount
 set SubRoot = ListSubRoot.item(i)
 tree = tree & SubRoot.GetAttribute("Description") & vbCrLf

 set ListSubRoot2 = SubRoot.ChildNodes
 maxCount2 = ListSubRoot2.length-1
 for j=0 to maxCount2
   set SubRoot2 = ListSubRoot2.item(j)
   tree = tree & vbTab & SubRoot2.GetAttribute("Tip") & vbCrLf
   set ListSubRoot3 = SubRoot2.ChildNodes
   maxCount3 = ListSubRoot3.length-1
   for k=0 to maxCount3
    set SubRoot3 = ListSubRoot3.item(k)
    tree = tree & vbTab & vbTab & SubRoot3.GetAttribute("Object") & vbCrLf
   next
 next
next
End Sub

Sub ShoiceSubSystem ()
	tree = ""
	ReadFromXML(0)
	Set srv=CreateObject("Svcsvc.Service")
	Cmd = srv.SelectInTree(tree,"�������....",false)
	ln = Len(Cmd)
	If Ln = 0 Then
		Exit sub
	End if
	fs = InStr(1, Cmd, "\")
	Cmd = Right(Cmd, Ln-fs)

	ln = Len(Cmd)
	fs = InStr(1, Cmd, "\")
	Cmd1 = Left(Cmd, fs-1)
	Cmd  = Right(Cmd, Ln-fs)

	Select case Cmd1
		Case "�����������"
			Cmd = "����������." & Cmd
		Case "���������"
			Cmd = "��������." & Cmd
		Case "������"
			Cmd = "�����." & Cmd
		Case "���������"
			Cmd = "���������." & Cmd
		Case "�������"
			Cmd = "������." & Cmd
		Case "���������������"
			Cmd = "��������������." & Cmd
		Case "������������"
			Cmd = "�����������." & Cmd
		Case else
			MsgBox "���� �� �������� ��� � ���� ������ :("
			Exit sub
	End select

	ListDoc = ""
	Set AllObjects = Documents.EnumAllTypedObj()
	Count = AllObjects.ChildsCount(Cmd) - 1

	'If Count >= 0 Then
	If Count > 0 Then ' artbear
		For i = 0 To Count
	    	Name = AllObjects.ChildName(Cmd, i)
	        FullName = Cmd & "." & Name

			Count = AllObjects.ChildsCount(FullName) - 1

			If Count >= 0 Then
				For j = 0 To Count
			    	Name2 = AllObjects.ChildName(FullName, j)
			        FullName2 = FullName & "." & Name2
					ListDoc = ListDoc & Name & "." & Name2 & "| |" & FullName2 & vbCrLf
			    Next
			Else
					ListDoc = ListDoc & Name & "| |" & FullName & vbCrLf
			End If
	    Next
		CurentDoc = srv.PopupMenu(ListDoc,0) ' artbear
		
	ElseIf Count = 0 Then ' ����� ������� ����� -- artbear
    	Name = AllObjects.ChildName(Cmd, i)
        FullName = Cmd & "." & Name 
		CurentDoc = FullName
	Else
		  MsgBox "������� �� �� ���� ���"
		  Exit sub
	End If

	Set Doc = AllObjects(CurentDoc)
	Doc.Open
End sub
 
' ���������� �� ���������� ��������
' ������ �� ��� ��������� ������� ���� � ������ ����������� � ��������� ��������� ��������
Sub SetupSubSystem()
	Set v7=CreateObject("V77.Application")
	v7.Initialize v7.rmtrade,"/d""" & ibdir & """","NO_SPLASH_SHOW"
	'v7.EvalExpr "��������������������(""�����"",,""" & BinDir & "config\DescriptSubSystem.ert"")"
	v7.EvalExpr "��������������������(""�����"",,""" & BinDir & "config\Scripts\�����\DescriptSubSystem.ert"")"
End Sub