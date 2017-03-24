$NAME svn + gcomp
//$Id: svngcomp.js,v 1.2 2007/04/14 20:02:39 alest Exp $
//
//����� ������� ������� aka alest (j_alesant@mail.ru)
//
//���������������� �� �������� GPL

// ������������� ��� �����, ���� ��� ��������� ����, ��� ������ �� ���� ������������
//	��� ������� ������
// ���� � gcomp ������� ����
// ���������, ���� ����� �����, � ������� ��������� ��
function decompile() {
	var gcompPath = 'cmd /U /C D:\\work\\1c\\gcomp.exe';
	var srcMDDir = 'SRC\\md';
	var _gcomp_ini  = ' --no-profiles --no-empty-mxl --no-empty-folders --no-broken-links '
	var params = ' -d -F "' + CommonScripts.FSO.buildPath(CommonScripts.IBDir(), '1cv7.md')
		+ '" -D "' + CommonScripts.FSO.buildPath(CommonScripts.IBDir(), srcMDDir) + '"'
		+ _gcomp_ini;

	cmdPath = gcompPath;
	
	var a = Windows.ActiveWnd;
/*	var doc = a.Document;
	CommonScripts.Echo('ID=' + doc.ID);
	CommonScripts.Echo('Kind=' + doc.Kind);
	CommonScripts.Echo('Path=' + doc.Path);
	CommonScripts.Echo('Name=' + doc.Name);
	CommonScripts.Echo('Type=' + doc.Type);
	CommonScripts.Echo('***************************************************');
*/
	if (!a || (a && a.Caption.indexOf('������������') != -1)) {
		CommonScripts.RunCommand(gcompPath, params, 0);
	}
	else {
		var doc = a.Document;
		if (doc) {
		
			var fStr = '';
			if (doc.Type == 0) {
				//Name=CMDDocDoc::�������� �����������
				fStr = doc.Name;
				if (fStr.indexOf('CMDDocDoc') != -1) {
					var re = /(.+)(��������)\s(.+)/;
					fStr = fStr.replace(re, "$2.$3");
				}
				//Name=CMDSubDoc::���������� �����������������
				else if (fStr.indexOf('CMDSubDoc') != -1) {
					var re = /(.+)(����������)\s(.+)/;
					fStr = fStr.replace(re, "$2.$3");
				}
				//Name=CMDJournalDoc::������ ����������
				else if (fStr.indexOf('CMDJournalDoc') != -1) {
					var re = /(.+)(������)\s(.+)/;
					fStr = fStr.replace(re, "$2.$3");
				}
				
			}
			else if (doc.Kind == 'ModuleText') {
				fStr = '����������������';
			}
			else if (doc.Kind == 'Transact') {
				//Name=��������.���������������.������ ���������
				//��������\���������������\����������������
				var re = /(��������)\.([^\.]+)\.(.+)/;
				fStr = doc.Name;
				fStr = fStr.replace(re, "$1\\$2\\����������������");
			}
			else if ((doc.Kind == 'Document') && (doc == docWorkBook)) {
				//Name=��������.���������������.�����
				//��������\���������������
				var re = /(��������)\.([^\.]+)\.(.+)/;
				fStr = doc.Name;
				fStr = fStr.replace(re, "$1\\$2");
				CommonScripts.Echo(fStr);
			}
			else if (((doc.Kind == 'Subconto') || (doc.Kind == 'SubFolder') || (doc.Kind == 'SubList'))
			&& (doc == docWorkBook)) {
				//Name=����������.����������������������������.�����
				//��������\���������������
				var re = /(����������)\.([^\.]+)\.(.+)/;
				fStr = doc.Name;
				fStr = fStr.replace(re, "$1\\$2");
			}
			else if (doc.Kind == 'Journal') {
				//Name=������.�����������.�����.�����������
				//������\������������
				var re = /(������)\.([^\.]+)\.(.+)/;
				fStr = doc.Name;
				fStr = fStr.replace(re, "$1\\$2");
			}
			else if ((doc.ID == -1) && (doc.Type == 1)) {
				//Name=CWorkBookDoc::������� �����(���������) - D:\work\1c\Clients\Pinskdrev\����\dev\ExtForms\���2.ert
				//Path=D:\work\1c\Clients\Pinskdrev\����\dev\ExtForms\���2.ert
				//� ������ ���������� \" �������� �������- ������ ��������� �������, ��� ��� ������ ������������������
				cmdPath = 'cmd /U /C decompile_ert.py';
				params = '-b "' + CommonScripts.FSO.GetAbsolutePathName(CommonScripts.IBDir()) + '" -f "' + doc.Path + '"';
			}
			
			CommonScripts.RunCommand(cmdPath, params + (fStr ? (' --filter ' + fStr) : ''), 0);
		}
	}
}


function init(_) // ��������� ��������, ����� ��������� �� �������� � �������
{
	try {
		var ocs = new ActiveXObject('OpenConf.CommonServices');
		ocs.SetConfig(Configurator);
		SelfScript.AddNamedItem('CommonScripts', ocs, false);
	}
	catch (e) {
		Message('�� ���� ������� ������ OpenConf.CommonServices', mRedErr);
		Message(e.description, mRedErr);
		Message('������ ' + SelfScript.Name + ' �� ��������', mInformation);
		Scripts.UnLoad(SelfScript.Name);
	}
}

init(0);
