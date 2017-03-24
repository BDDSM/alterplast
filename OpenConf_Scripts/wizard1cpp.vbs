$NAME ������ 1�++
'      ������ ��� ���������� �������� "������ 1�++" (c) artbear,  2004
'
'         ��� e-mail: artbear@bashnet.ru
'         ��� ICQ: 265666057

Dim Wizard1CPP

Sub TestCreateExampleClass
  Wizard1CPP.ClassName = "�����_�����"

 Wizard1CPP.FolderPath = "C:\" ' ����� �������� � ���������� ����� � �����
'  Wizard1CPP.FolderPath = "ExtForms"  ' ����� �������� � ������������� ����� � �����
'  Wizard1CPP.FolderPath = "#������\�����" ' ����� �������� � ������� �� oxy.ini

  Wizard1CPP.ClassFileName = "����_������_������.ert" ' ��� �������� ������� �����
'  Wizard1CPP.ClassFileName = "����_������_ ������.ert" ' ������ ������ ��������� �� ������

  Wizard1CPP.EnableConstructor = 1
  Wizard1CPP.EnableDestructor = 0
  Wizard1CPP.EnableDebug = 0
  Wizard1CPP.UsedAsVisualForm = 0
  Wizard1CPP.BaseClasses = ""

  Wizard1CPP.CreateClass

End Sub ' TestCreateExampleClass

Sub Wizard1CPP_OnCreateClass(ModuleText)
' ����� �������� ��� ������ ������,  ��������:
  ModuleText = "// ������ ������ Wizard1C++ "+vbCRLF +ModuleText + vbCRLF +" // ���������� ������ Wizard1C++ !!"

'  Echo ModuleText
End Sub

Sub Wizard1CPP_OnCreateDefinitionOfClass(DefinitionText)
' ����� �������� ��� ����������� ������,  ��������:
  DefinitionText = "// ������ ����������� ������ "+vbCRLF +DefinitionText + vbCRLF +" // ���������� ����������� ������ !!"

'  Echo "����������� ������ = "+vbCrLF+DefinitionText
End Sub

Sub TestCreateExampleClassWithDebugMessages

  Wizard1CPP.EnableDebugMessages = -1 ' �������� ��������� ���������� ���������

  TestCreateExampleClass

  'Wizard1CPP.EnableDebugMessages = 0 ' ��������� ��������� ���������� ���������

End Sub ' TestCreateExampleClassWithDebugMessages

Sub TestHistory
  Echo "������� ��������� ������� - " + Wizard1CPP.ClassNameHistory

  Echo "������� �������������� ��������� ��� �������� ������� - " + Wizard1CPP.FolderPathHistory
  Echo "������� �������������� ������� ������� - " + Wizard1CPP.BaseClassesHistory

End Sub ' TestHistory

Sub ClearAll
  Wizard1CPP.ClassName = ""

  Wizard1CPP.FolderPath = ""
  Wizard1CPP.ClassFileName = ""
  Wizard1CPP.EnableConstructor = 0
  Wizard1CPP.EnableDestructor = 0
  Wizard1CPP.EnableDebug = 0
  Wizard1CPP.UsedAsVisualForm = 0
  Wizard1CPP.BaseClasses = ""
  Wizard1CPP.EnableBaseClassesVariable = 0

End Sub ' ClearAll

Sub Min
  Wizard1CPP.EnableConstructor = 0
  Wizard1CPP.EnableDestructor = 0
  Wizard1CPP.EnableDebug = 0
  Wizard1CPP.UsedAsVisualForm = 0
End Sub 'Min

Sub Max
  Wizard1CPP.EnableConstructor = 1
  Wizard1CPP.EnableDestructor = 1
  Wizard1CPP.EnableDebug = 1
  Wizard1CPP.UsedAsVisualForm = 1
End Sub 'Max

Sub Echo(text)
  Message CStr(text), mNone
End Sub'Echo

Function GetPluginByName(PluginName)
  set GetPluginByName = Nothing
'echo "PluginName = " & PluginName
  err.Clear
'Echo CStr(err.Number)

  on error resume next
  Wizard1CPP = Plugins(CStr(PluginName))
  Err_My = err.Number
'Echo "1-" & CStr(Err_My)
'  on error goto 0
'Echo "2-" &  CStr(Err_My)
'Echo CStr(GetPluginByName)
  if Err_My = 438 then ' ����� ������ ������������� ����������
    set GetPluginByName = Plugins(CStr(PluginName))
'Echo "3-" & CStr(err.Number)
'Echo "4-" & CStr(GetPluginByName)
    Exit Function
  end if
End Function

'' ������������� �������. param - ������ ��������,
'' ����� �� ������� � �������
''
'Sub Init(param)
'' set Wizard1CPP = GetPluginByName("������ 1�++")
' set Wizard1CPP = Plugins("������ 1�++")
' if not Wizard1CPP Is Nothing Then
'     ' ����������� ������ � �������� �������
'		SelfScript.AddNamedItem "Wizard1CPP", Wizard1CPP, False
'		Wizard1CPP.EnableDebugMessages = 0 ' ��������� ��������� ���������� ���������
'  else
'      ' ������ �� ��������. �������� � ������
'      Scripts.Unload SelfScript.Name
'  End If
'End Sub


' ������������� �������. param - ������ ��������,
' ����� �� ������� � �������
'
'Sub Init(Script, ScriptName, PrefixForEvents, ByRef ScriptVariable)
Sub Init(Script, ScriptName, PrefixForEvents, ScriptVariable)
  set ScriptVariable = GetPluginByName(CStr(ScriptName))
  if not ScriptVariable Is Nothing Then
      ' ����������� ������ � �������� �������
      Script.AddNamedItem PrefixForEvents, ScriptVariable, False
  else
      ' ������ �� ��������. �������� � ������
      Scripts.Unload Script.Name
  End If
End Sub

' ��� �������� ������� �������������� ���
Init SelfScript, "������ 1�++", "Wizard1CPP", Wizard1CPP
on error resume next
Wizard1CPP.EnableDebugMessages = 0 ' ��������� ��������� ���������� ���������

'' ��� �������� ������� �������������� ���
'Init 0