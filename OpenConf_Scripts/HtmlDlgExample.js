// ========================================================================
// 2005-11-07
// a13x <kuntashov at yandex dot ru>
// ������ ������� � html-��������
// ========================================================================

// ����������� ������� ��������� html-������� ��� ��������
// ��������� � ���� ������ js-�������
var HtmlDlgEventProxy =
{
	Button1OnClick : function (ThisForm) {
		var name = "���";
		if (!ThisForm.CheckBox1.checked) {
//			name = CommonScripts.InputBox("������ ���", "Html Dialog Example", "");
			name = CommonScripts.InputBox("������� ���");
		}
		Message("������, " + name + "!");
	},

	Button2OnClick : function (ThisForm) {
		Message(ThisForm.Edit1.value);
	},

	Button3OnClick : function (ThisForm)
	{
		// ����� ����� �������� �������� ������� ���������

		// ������� ������
		ThisForm.ComboBox1.options.length=0;

		// ��������� 10 ����� ���������
		for (var i=0; i<10; i++) {
			var opt = HtmlWindow.Document.createElement("OPTION");
			opt.value = i;
			opt.text = "�������� " + i;
			ThisForm.ComboBox1.options.add(opt);
		}

	},

	OnOpen : function ()
	{
		Message("��� ��������");
	},

	OnClose : function () {
		// ���������������, ����� �������� ������������ ���� ������� �� ���������� HtmlWindow
		// � �������� ������������ ����������� ������� ������ ��� ������ ��� ��� :-)
		Scripts.Reload(SelfScript.Name);
	},

	ListBox1OnChange : function (ThisForm) {
		Message("������ �������: " + ThisForm.ListBox1.options[ThisForm.ListBox1.selectedIndex].value
				+ " (" + ThisForm.ListBox1.options[ThisForm.ListBox1.selectedIndex].innerText + ")");
	},

	ComboBox1OnChange : function (ThisForm) {
		Message("������ �������: " + ThisForm.ComboBox1.options[ThisForm.ComboBox1.selectedIndex].value);
	},

	RadioButton1OnClick: function (ThisForm, SelectedIndex) {
		Message("������ ���������� � " + ThisForm.RadioButton1[SelectedIndex].value);
	}
}

// ���������� ��� �������� ����, ����� �� ������ �������� � html-������
// ��� ������, � ������� ����������� ����������� ������� html-���������
function onDocumentComplete(d, u)
{
	try {
		// ������������� � �������� ��������� html-���������
		HtmlWindow.Document.Script.SetEventProxy(HtmlDlgEventProxy);
		Windows.ActiveWnd.Maximized = true;
	}
	catch (e) {
		Message("������ ������������� ���� HTML-�������", mRedErr);
		Message(e.description, mRedErr);
	}
}

// ������ ��� �������� html-������� ������ �������
function OpenHTMLDialog()
{
	var wnd = OpenOleForm("Shell.Explorer", "HTML Dialog Example");
	SelfScript.AddNamedItem("HtmlWindow", wnd, false);
    eval('function HtmlWindow::DocumentComplete(d,u){ return onDocumentComplete(d, u) }');
	HtmlWindow.Navigate2(BinDir+"config\\data\\HtmlDlgExample.htm");
}

function CreateObjectOrDie(progid, id)
{
	try {
		var obj = new ActiveXObject(progid);
		if (id) {
			SelfScript.AddNamedItem(id, obj, false);
		}
		return obj;
	}
	catch (e) {
		Message("�� ���� ������� ������ " + progid, mRedErr);
		Message(e.description, mRedErr);
		Message("������ " + SelfScript.Name + " �� ��������", mInformation);
		Scripts.UnLoad(SelfScript.Name);
	}
}

function Init(_)
{
	CreateObjectOrDie("OpenConf.CommonServices", "CommonScripts");
	//OpenHTMLDialog();
}

Init();
