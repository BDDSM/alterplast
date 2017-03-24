$NAME ������� �� ��������������

/* ===========================================================================
������: Synonym.js
������: $Revision: 1.2 $
�����: Phoenix, a13x

��������:
	��������� �������� �� ��������� ��������������.
	������������ �������� ���������� �� ���������� 1�v8.0
	� �������������� ��������� ����� ��������� �� ������ + ������� �����.

�������:
	�������������������(), GetSinonimByName()
		������� � ������������� ������� ��������� �� ������
		��� ��������������. ������ ����� ������������ � ��������
		"�������� ���������" � "�������� �������" (�����������, ��������� 
		� �.�.), � ��� ����� � � ���� ������� �������, ����������� � �������
		�������� ����� �� ������� ���������� � ������ �������������.
			
=========================================================================== */

var GW_HWNDNEXT	= 0x0002;
var GW_CHILD	= 0x0005;

var CoreObj = {

	api	: null, 
	svc	: null,
	
	hIdent		: null, // ������ ��������� ����� editbox'� ��������������
	hSynonym	: null, // ������ ��������� ����� editbox'� ��������

	// �������������
	init: function ()
	{
		this.svc = new ActiveXObject("Svcsvc.Service");		
		this.api = new ActiveXObject("DynamicWrapper");

		this.api.Register("USER32.DLL", "GetForegroundWindow", "f=s", "r=l");
		this.api.Register("USER32.DLL", "GetWindow",  "I=ll", "f=s", "r=l")
	},

	// ��������� �������� �� ������ ��������������
	genSynonym: function (id) 
	{
		if (!id) return "";

		var UpChars = "��������������������������������QWERTYUIOPASDFGHJKLZXCVBNM";
		var lResult = lCutLit = id.substring(0,1);

		for (var i = 1; i < id.length; i++) {
			lCutLit = id.substring(i, i+1);
			if (UpChars.indexOf(lCutLit) != -1) {
				lResult += " " + lCutLit.toLowerCase();
			}
			else {
				lResult += lCutLit;
			}
		}

		return lResult;
	},

	/* ����������� ����� ������� ����� � ��������������� � ���������
		����� �������� ��������� ���� � ������� hParent */
	findOurHandlers: function (hParent) 
	{
		// ���� ������ ������ ������� - ���������� �����
		if (this.hIdent && this.hSynonym) return;

		// ������� ��� �������� �������� ���� � ������� hParent
		for (var hChild = this.api.GetWindow(hParent, GW_CHILD); hChild; hChild = this.api.GetWindow(hChild, GW_HWNDNEXT))
		{
			if (this.hIdent && this.hSynonym) return;

			var title = this.svc.GetWindowText(hChild, false);
			if ("&�������������:" == title) { 
				/* ����� ����� � �������� � editbox'� � ���������������,
				   ������, ��������� ������� - ��� editbox, �������� ��� */
				this.hIdent = hChild = this.api.GetWindow(hChild, GW_HWNDNEXT);
				continue;
			}			
			if ("&�������:" == title) { 
				/* ����� ����� � �������� � editbox'� � c��������,
				   ������, ��������� ������� - ��� editbox, �������� ��� */
				this.hSynonym = hChild = this.api.GetWindow(hChild, GW_HWNDNEXT);
				continue;
			}

			// ���������� ������� �������� ��������
			this.findOurHandlers(hChild);
		}						 
	},

	// ���������� � ������������� ������� �� ������ ��������������
	getSynonymById: function ()
	{
		this.hIdent		= null;
		this.hSynonym	= null;

		// �������� �������� ����
		var hWnd = this.api.GetForegroundWindow();
		if (!hWnd) return;

		// ����� ������� ����-������ �������������� � �������� 
		// ����� ���� ��������� ��������� ����
		this.findOurHandlers(hWnd);

		// ���� ������ �������, ������� ������� �� ������ ��������������
		if (this.hIdent && this.hSynonym) {
			var synonym = this.genSynonym(this.svc.GetWindowText(this.hIdent, false));
			if (synonym) {				
				this.svc.SetWindowText(this.hSynonym, synonym);
			}
		}		
	}
}

// �������

function �������������������() { CoreObj.getSynonymById() }

// ��������, ����� �� ���� ���� ������������� �������, 
// ����������� ��� ������������� ���������� ������
function GetSinonimByName() { CoreObj.getSynonymById() }

CoreObj.init();

