/*
������� ������� ���� WebBrowser'� � ������������� � �������� ����������,
��������� � ���� �������� � ���������� ������, � �������� ����������� �������
�������� ��������.
*/
function ShowHtml(url,title)
{
	h=OpenOleForm("Shell.Explorer", title)
	/*
	������-�� VBScript �� ������������� � �������� WebBrowser'�,
	������� ������������ JScript. ������� �������, ��� � JScript
	��������� ��������� ������� � ����������� ������������ �������,
	��������� ��������� ��������, �� ��� ��� �������.
	*/
	SelfScript.AddNamedItem("Html",h,false)
    eval('function Html::DocumentComplete(d,u){ return HtmlDC(0)}');
	Html.Navigate2(url)
}

/*
���������� ������� ��������� �������� ��������.
��������������, ��� � �������� �� ���� �������� ���� ����� SetConf,
����� ������� � �������� ���������� ������ Configurator.
*/
function HtmlDC(_)
{
	// ��������� � �������� Configurator
	try{
	Html.Document.Script.SetConf(Configurator)
	}catch(e)
	{
		Message("�� ������� �������� Configurator � ��������")
	}
	// ������������ ������, ����� �� ��� ����� � �������� ������ ��������
	Scripts.Reload(SelfScript.Name)
}

function ShowTestPage()
{
	ShowHtml(BinDir+"test.htm","Test page")
}

