' ������ �������, ��������������� ����������� �������� PanelVisible
' ������ ������� ����������� ��������� ��������� �������.
' � �������� ����� ������ ������������ �� ���������.
' ����� ����� ������ ��������� ������� ������� ������������

Sub TogglePanel(PanelName)
	Windows.PanelVisible(PanelName)=Not Windows.PanelVisible(PanelName)
End Sub

Sub ToggleSyntaxHelper()
	TogglePanel "�������-��������"
End Sub

Sub ToggleOutPutWindow()
	TogglePanel "���� ���������"
End Sub

Sub TogleSearchWindow()
	TogglePanel "������ ��������� ���������"
End Sub

Sub TogleStdToolbar()
	TogglePanel "�����������"
End Sub
