/*===========================================================================
������:  DocPath.js
������:  $Revision: 1.4 $
�����:   ��������� ��������
E-mail:  kuntashov at [yandex.ru|gmail.com]
ICQ UIN: 338758861

��������: 
    ������� ��� ������/����������� � ����� ������ ������� ���� 
    ��������� ��������� (�������� ������, ������� ��� ���������� �����).

	����� ������ ������ ��������� ��������: ��� �������� �������� ������ 
	��� ������ ���� ��������� � ������ ��������� ����. ������, ����� ������� 
	���������� ���� ��������� �� ����� �����, ��� �� ������ ������. 
	������ ������ ������������� ������� ���������� �������� ������ �
	������� ������ ���� � ������ ������� ��� ��������� ���� (�������������, ��. 
	����, ����������� ���������).
	����� - Phoenix.

	�������:
	
    	ShowDocPath	- ������� ���� �������� ��������� � ���� ���������
    	CopyDocPath	- �������� ���� �������� ��������� � ����� ������ 
    	OpenParentDir - ��������� �������, � ������� ��������� ������� 
						�������� � ����������
    	CopyParentDir - �������� �������, � ������� ��������� �������
						�������� � ����� ������

===========================================================================*/

var mInStatusBar	= 0; // ���������� ���� � ������ �������
var mInCaption		= 1; // ���������� ���� � ��������� ����

// ��������� (����� �������� �� ���� �� �������� ����):
// ��� ���������� ���� �������� ������ - � ��������� ��� � ������ �������
// �� ��������� - � ���������
var mShowPathIn		= mInCaption; 

// ===========================================================================

var CoreObj = {
		
	version : "$Revision: 1.4 $".replace(/\s|\$/ig, "").replace(/^Revision:/, ""),

	ocs : null,

	init : function () 
	{
		try {
			this.ocs = new ActiveXObject("OpenConf.CommonServices");
			this.ocs.SetConfig(Configurator);
		}
		catch (e) {
			Message("�� ���� ������� ������ OpenConf.CommonServices", mRedErr);
			Message(e.description, mRedErr);
			Message("������ " + SelfScript.Name + " �� ��������", mInformation);
			Scripts.UnLoad(SelfScript.Name); 		
		}	
	},
	
	show : function ()
	{
		var a = null;
		if (a = Windows.ActiveWnd) {
			if (a = a.Document) {
				Message(a.Path, mInformation);
			}
		}
	},

	copy : function ()
	{
		var a = null;
		if (a = Windows.ActiveWnd) {
			if (a = a.Document) {
				this.ocs.CopyToClipboard(a.Path);
			}
		}
	},

	openDir : function ()
	{
		var a = null;
		if (a = Windows.ActiveWnd) {
			if (a = a.Document) {
				var p = this.ocs.FSO.GetParentFolderName(a.Path);				
				this.ocs.WSH.Run("%SYSTEMROOT%\\explorer.exe " + p);
			}
		}
	},

	copyDir : function ()
	{
		var a = null;
		if (a = Windows.ActiveWnd) {
			if (a = a.Document) {
				var p = this.ocs.FSO.GetParentFolderName(a.Path);
				this.ocs.CopyToClipboard(p);
			}
		}
	},

	getFileName : function ()
	{
    	if (!(Windows.ActiveWnd == null)) {
        	var WndName = Windows.ActiveWnd.Document.Name;
        	
    	    if ((WndName.substring(0, 14) == "CWorkBookDoc::") || (WndName.substring(0, 14) == "CBodyTextDoc::")) {
            	return Windows.ActiveWnd.Document.path;
        	}
    	    else {
        	    return WndName;
    	    }
	    }
	}

}

function ShowDocPath() { CoreObj.show() }
function CopyDocPath() { CoreObj.copy() }

function OpenParentDir() { CoreObj.openDir() }
function CopyParentDir() { CoreObj.copyDir() }

function Configurator::OnFileSaved(Doc)
{
    if (mShowPathIn == mInStatusBar)
        Status(CoreObj.getFileName());
    else
        Windows.ActiveWnd.Caption = CoreObj.getFileName();
}	

CoreObj.init()

