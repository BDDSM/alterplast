'
' ������ ������ � �������� OpenConf.CommonServices
'

'
' ��������� ������������� �������
'
Sub Init(dummy) ' ��������� ��������, ����� ��������� �� �������� � �������
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
 
Init 0 ' ��� �������� ������� ��������� �������������

CommonScripts.Error "Error: �������"
CommonScripts.Echo "Echo: �������"
