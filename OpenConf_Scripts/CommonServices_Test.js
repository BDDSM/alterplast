/*
 * ������ ������ � �������� OpenConf.CommonServices
 * ��� ������������� � �������� ������� �������� ��� OpenConf
 *
 * $Date: 2005/03/11 05:22:05 $
 * $Revision: 1.6 $
 */

function HelloMacro()
{
	CommonScripts.Echo("Hello, World!");
}

/*
 * ���������� ������� ������ �������
 */
function Version(_)
{
	return "$Revision: 1.6 $".replace(/\s|\$/ig, "").replace(/^Revision:/, "");
}

/*
 * �������� ������ OpenConf.CommonServices
 */ 
function TestCommonServicesVersion()
{
	CommonScripts.Echo(CommonScripts.Version);

	if (CommonScripts.Version < "1.26") {
		CommonScripts.Echo("< 1.26");
	}
	if (CommonScripts.Version > "1.23") {
		CommonScripts.Echo(">1.23");
	}
	if (CommonScripts.Version == "1.24") {
		CommonScripts.Echo("== 1.24");
	}
}

/*
 * ��������� ������������� �������
 */
function Init(_) // ��������� ��������, ����� ��������� �� �������� � �������
{
	try {
		var ocs = new ActiveXObject("OpenConf.CommonServices");
		ocs.SetConfig(Configurator);
		SelfScript.AddNamedItem("CommonScripts", ocs, false);
	}
	catch (e) {
		Message("�� ���� ������� ������ OpenConf.CommonServices", mRedErr);
		Message(e.description, mRedErr);
		Message("������ " + SelfScript.Name + " �� ��������", mInformation);
		Scripts.UnLoad(SelfScript.Name); 		
	}	
}

Init();
