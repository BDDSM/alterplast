'
'����: Templates.vbs
'������: $Revision: 1.1 $
'�����: ��������� �������� <kuntashov@yandex.ru>
'
'���������� �������� ��������� �������� ������������� 1�:�����������.
'
'� ������ ���������� ������ �����:
'        
'        OpenConf_Scripts\��������������\Templates.wsc 
'            ����������, ����������� ������ ����������
'
'        OpenConf_Scripts\�������\��������������\Templates.vbs
'            ������ ��� OpenConf, ����������� ���������� � �������������� 
'            ������� OnTemplate ������� "������� 2" ���������� ��������.
'
'������ �������� ������� ��. �� http://npj.ru/kuntashov/open_conf/templates
'
'��� ��������� �������� ��������� ����������� ������������. �� ������
'�������������� � (���) �������������� �� �� �������� GNU Generic Public License.
'
'������ ��������� ���������������� � �������� ��������� ��������, ��
'��� �����-���� ��������, � ��� ����� ��� �������� ����������� ��� ������� ���
'�����-���� ������ ������������ �����.
'
'� ������ ������� �������� �� ���������� ����� ����� ������������ �� ������
'http://www.gnu.org/licenses/gpl.txt
'
'� ������� ��������� �������� ����� ������������ �� ������
'http://gnu.org.ru/gpl.html  
'

Sub Telepat_OnTemplate(Name, Text, Cancel)

    Set Template = TemplatesManager.CreateTemplate(Name, Text, true)        
    Cancel = Not TemplatesManager.ParseInteractively(Template)
    If Not Cancel Then
        Text = TemplatesManager.ParsingResult
    End If

End Sub

Private Sub CreateObjectOrDie(ProgId, Name)
    
    Set Object = Nothing    
    On Error Resume Next
    Set Object = CreateObject(ProgId)
    On Error GoTo 0
    
    If Object Is Nothing Then
        Message "�� ���� ������� ������ """ & ProgId & """", mRedErr
        Message "������ " & SelfScript.Name & " �� ��������", mInformation
        Scripts.UnLoad SelfScript.Name
		Exit Sub
    End If
    
    Object.SetConfig(Configurator)
    SelfScript.AddNamedItem Name, Object, False

End Sub

Private Sub Init()

    Set t = Plugins("�������")
    If t Is Nothing Then
        Message "������ �������� ������ ��� ������������� ������� �������!", mRedErr
        Scripts.Unload SelfScript.Name
    End If
    SelfScript.AddNamedItem "Telepat", t, False

    'CreateObjectOrDie "OpenConf.CommonServices", "CommonScripts" 
    CreateObjectOrDie "OpenConf.TemplatesManager", "TemplatesManager"

End Sub

Init
