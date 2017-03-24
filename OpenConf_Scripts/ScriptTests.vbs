Sub TestScriptsMacros()
        Set e=CreateObject("Macrosenum.Enumerator")
        iScriptCount=Scripts.Count-1
        for i=0 to iScriptCount
                Message Scripts.Name(i),mMetaData
                Set script=Scripts(i)
                arr=e.EnumMacros(script)                        ' ��������� ������� �������� �������
                for j=0 to ubound(arr)
                        Message arr(j),mNone
                        'e.InvokeMacros Script, arr(j)  ' ����� �������: ������, ��� �������
                Next
        Next
End Sub

Sub TestPluginsMacros()
        Set e=CreateObject("Macrosenum.Enumerator")
        iScriptCount=Plugins.Count-1
        for i=0 to iScriptCount
                Message Plugins.Name(i),mMetaData
                Set script=Plugins.Item(i)
                arr=e.EnumMacros(script)                        ' ��������� ������� �������� �������
                for j=0 to ubound(arr)
                        Message arr(j),mNone
                        'e.InvokeMacros Script, arr(j)  ' ����� �������: ������, ��� �������
                Next
        Next
End Sub

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

	on error resume next
	temp = Plugins(PluginName)
	on error goto 0
	if err.Number = 438 then ' ����� ������ ������������� ����������
		set GetPluginByName = Plugins(PluginName)
		Exit Function
	end if
End Function ' GetPluginByName

Sub TestWizard1CPPMacros()
        Set e=CreateObject("Macrosenum.Enumerator")
'        iScriptCount = 0
'        for i=0 to iScriptCount
'                Message Plugins.Name(i),mMetaData
                Set script=GetPluginByName("������ 1�++")


                arr=e.EnumMacros(script)                        ' ��������� ������� �������� �������
                for j=0 to ubound(arr)
                        Message arr(j),mNone
                        'e.InvokeMacros Script, arr(j)  ' ����� �������: ������, ��� �������
                Next
'        Next
End Sub