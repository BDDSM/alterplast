Sub ManagePRM()
	
	Set FSO = CreateObject("Scripting.FileSystemObject")
	
	If FSO.FileExists(IBDir & "turbomd.prm") Then

		
		'��������� ����������� ��������� dd
		FSO.CopyFile IBDir & "turbomd.prm", IBDir & "turbomd.prm.bak", True
		FSO.DeleteFile(IBDir & "turbomd.prm")
		'MsgBox "turbomd.prm ������������ � turbomd.prm.bak"
	End If 
	
End Sub
                                                                   
'����� ���������� - ��������� prm
'------------------------------------------------------------------
Sub Configurator_MetaDataSaved(FileNAme)
	ManagePRM()
End Sub


