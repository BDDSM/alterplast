$NAME ����������� �������� ��������

Sub CodeBeautifier()
	Set Doc = CommonScripts.GetTextDocIfOpened(0)
	If Doc Is Nothing Then Exit Sub
	Set Pos = CommonScripts.GetDocumentPosition(Doc)
	If Doc.SelStartLine <> Doc.SelEndLine or Doc.SelStartCol <> Doc.SelEndCol Then
		Text = Doc.Range(Doc.SelStartLine, Doc.SelStartCol, Doc.SelEndLine, Doc.SelEndCol)
		Text = Scripts("CodeBeautifier").Beautify_OpenConf(Text)
		Doc.Range(Doc.SelStartLine, Doc.SelStartCol, Doc.SelEndLine, Doc.SelEndCol) = Text
	Else
		Doc.Text = Scripts("CodeBeautifier").Beautify_OpenConf(Doc.Text)
	End If
	Pos.Restore
End Sub

'������� ���� � ������� ������
Sub CodeIns()
	Set Doc = CommonScripts.GetTextDocIfOpened(0)
	If Doc Is Nothing Then Exit Sub

	DoCodeIns(Doc)
End Sub

Sub DoCodeIns(Doc)
	mod_type = GetModuleType(Doc)
	If mod_type = "" Then Exit Sub

	names = split(Doc.Name, ".")
	Text = Scripts("CodeIns").CodeIns(Doc.Text, names(1), IBDir & mod_type & ".pl")

	If Text <> Doc.Text Then
		If Doc.IsOpen Then Set Pos = CommonScripts.GetDocumentPosition(Doc)
		Doc.Text = Text
		If Doc.IsOpen Then Pos.Restore
	End If
End Sub

'������� ���� �� ���� ������������
Sub CodeInsReferences()
	Set Refs = MetaData.TaskDef.Childs("����������")
	For i = 0 To Refs.Count - 1
		Message Refs(i).Name, mMetaData
		
		Set Doc = Documents("����������." & Refs(i).Name & ".�����.������")
		If not Doc Is Nothing Then DoCodeIns(Doc)
		
		Set Doc = Documents("����������." & Refs(i).Name & ".����� ������.������")
		If not Doc Is Nothing Then DoCodeIns(Doc)

		Set ListForms = Refs(i).Childs("�����������")
		For j = 0 To ListForms.Count - 1
			Set Doc = Documents("����������." & Refs(i).Name & ".�����������." & ListForms(j).Name & ".������")
			If not Doc Is Nothing Then DoCodeIns(Doc)
		Next
	Next
End Sub

'������� ���� �� ���� ����������
Sub CodeInsDocuments()
	Set Docs = MetaData.TaskDef.Childs("��������")
	For i = 0 To Docs.Count - 1
		Message Docs(i).Name, mMetaData

		Set Doc = Documents("��������." & Docs(i).Name & ".�����.������")
		If not Doc Is Nothing Then DoCodeIns(Doc)
		Set Doc = Documents("��������." & Docs(i).Name & ".������ ���������")
		If not Doc Is Nothing Then DoCodeIns(Doc)
	Next

	Set Journals = MetaData.TaskDef.Childs("������")
	For i = 0 To Journals.Count - 1
		Message Journals(i).Name, mMetaData

		Set ListForms = Journals(i).Childs("�����������")
		For j = 0 To ListForms.Count - 1
			Set Doc = Documents("������." & Journals(i).Name & ".�����." & ListForms(j).Name & ".������")
			If not Doc Is Nothing Then DoCodeIns(Doc)
		Next
	Next
End Sub

'������� ���� �� ���� ������������ � ����������
Sub CodeInsToAll()
	CodeInsReferences()
	CodeInsDocuments()
End Sub

Function GetModuleType(Doc)
	GetModuleType = ""
	names = split(lcase(Doc.Name), ".")
	
	last = Ubound(names)
	If names(0) = "��������" Then
		If names(last) = "������ ���������" Then
			GetModuleType = "���������������"
		Else
			GetModuleType = "��������������"
		End If
	ElseIf names(0) = "����������" Then
		If names(last-1) = "�����" Then
			GetModuleType = "�������������"
		ElseIf names(last-1) = "����� ������" Then
			GetModuleType = "�����������"
		ElseIf names(last-2) = "�����������" Then
			GetModuleType = "�����������"
		End If
	ElseIf names(0) = "������" Then
		GetModuleType = "������������"
	ElseIf names(0) = "����" Then
		GetModuleType = "�����"
	ElseIf names(0) = "���������" Then
		GetModuleType = "���������"
	End If		
End Function
	
' ��������� ������������� �������
Private Sub Init()
    Set c = Nothing
    On Error Resume Next
    Set c = CreateObject("OpenConf.CommonServices")
    On Error GoTo 0
    If c Is Nothing Then
        Message "�� ���� ������� ������ OpenConf.CommonServices", mRedErr
        Message "������ " & SelfScript.Name & " �� ��������", mInformation
        Scripts.UnLoad SelfScript.Name
		Exit Sub
    End If
    c.SetConfig(Configurator)
	SelfScript.AddNamedItem "CommonScripts", c, False
End Sub
 
Init ' ��� �������� ������� ��������� �������������
