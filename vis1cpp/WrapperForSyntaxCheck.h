#ifndef WRAPPER_FOR_SYNTAXCHECK_H
#define WRAPPER_FOR_SYNTAXCHECK_H

//extern HANDLE hProcess;

typedef unsigned __int64 ui64;

// ����� ��� ��������� ������� ����� ��������� ����������� � ������ �������.
// ��� ������ ���������
// ����� - orefkov
//
// artbear - �������� ����� ��������������� ��������� � ������ ������
//
class CTrapSwap
{
public:
	CTrapSwap():m_pFunc(NULL), m_SwapBuf(0){}

	template<typename T>
	void SetTrapByName(HINSTANCE hDll, LPCSTR strFuncName, T newFunc)
	{
		if(!hDll)
		{
			MessageBox("�������� ��������� �� DLL - %s ", strFuncName);
			return;
		}
		DWORD* pFunc=reinterpret_cast<DWORD*>(GetProcAddress(hDll, strFuncName));
		if(!pFunc)
		{
			MessageBox("�� ������� ��������� �� ������� - %s ", strFuncName);
			return;
		}
		DWORD pNewFunc=*reinterpret_cast<DWORD*>(&newFunc);

		_SetTrap(pFunc, pNewFunc);
	}
	
	template<typename T>
	void SetTrapByName(LPCSTR strDll, LPCSTR strFuncName, T newFunc)
	{
		SetTrapByName(GetModuleHandle(strDll), strFuncName, newFunc);
	}

	template<typename T, typename M>
	void SetTrapOnImportFunc(T oldFunc, M newFunc)
	{
		BYTE* ptr=*reinterpret_cast<BYTE**>(&oldFunc);
		ptr+=2;
		DWORD* pFunc=**reinterpret_cast<DWORD***>(ptr);
		DWORD pNewFunc=*reinterpret_cast<DWORD*>(&newFunc);

		_SetTrap(pFunc, pNewFunc);
	}

	template<typename T, typename M>
	void SetTrapOnFunc(T oldFunc, M newFunc)
	{
		DWORD* pFunc=*reinterpret_cast<DWORD**>(&oldFunc);
		DWORD pNewFunc=*reinterpret_cast<DWORD*>(&newFunc);

		_SetTrap(pFunc, pNewFunc);
	}

	void Swap()
	{
		ui64 t=*m_pFunc;
		*m_pFunc=m_SwapBuf;
		m_SwapBuf=t;
	}
	ui64* m_pFunc; // TODO ����� �� ����� public-������ ?
protected:
	ui64 m_SwapBuf;
	void _SetTrap(DWORD* pFunc, DWORD pNewFunc)
	{
		if(!pFunc)
		{
			MessageBox("CTrapSwap::_SetTrap - �� ������� ��������� �� ������� pFunc - %s ");
			return;
		}
		if(!pNewFunc)
		{
			MessageBox("CTrapSwap::_SetTrap - �� ������� ��������� �� ������� pNewFunc - %s ");
			return;
		}
		m_pFunc=reinterpret_cast<ui64*>(pFunc);
		DWORD old;
		VirtualProtect(pFunc, 8, PAGE_EXECUTE_READWRITE, &old);
		// ��������� ����� �� ����� ����� �������
		m_SwapBuf = (static_cast<ui64>(pNewFunc - reinterpret_cast<DWORD>(pFunc)-5)<<8) | 0xE9;
		// � ��������� � ������ ������� �������������� �����
		Swap();
		FlushInstructionCache(GetCurrentProcess(), m_pFunc, 8);
	}
};

struct trapswaper
{
	CTrapSwap& trap;
	trapswaper(CTrapSwap& t):trap(t){trap.Swap();}
	~trapswaper(){trap.Swap();}
};

// ����� ��� ��������� ������� ����� ��������� ����������� � ������ �������
// c ������ ����������
// ����� - artbear (c) 2006
//
// ������� �� ������ CTrapSwap (�) orefkov
//	���������� ��� ��������� ���������� � ���������� ��� ������� CTrapSwap
//
template<typename TypeFunc_Wrap>
class CTrap
{
private:
	CTrapSwap trap;
public:
	
	void SetTrapByName(HINSTANCE hDll, LPCSTR strFuncName, TypeFunc_Wrap newFunc)
	{
		trap.SetTrapByName(hDll, strFuncName, newFunc);
	}
	
	void SetTrapByName(LPCSTR strDll, LPCSTR strFuncName, TypeFunc_Wrap newFunc)
	{
		trap.SetTrapByName(GetModuleHandle(strDll), strFuncName, newFunc);
	}
	
	template<typename M>
	void SetTrapOnImportFunc(TypeFunc_Wrap oldFunc, M newFunc)
	{
		trap.SetTrapOnImportFunc(oldFunc, newFunc);
	}
	
	template<typename M>
	void SetTrapOnFunc(TypeFunc_Wrap oldFunc, M newFunc)
	{
		trap.SetTrapOnFunc(oldFunc, newFunc);
	}
	
	void Swap()
	{
		trap.Swap();
	}
};

// ������ ��������� - ������������ � �������� � �������� ��������� �������
// ��� ����� ��������� ��� ����������� �������� ����� ������������
//
template<typename TypeFunc_Wrap>
class CTrapGuard
{
private:
	CTrap<TypeFunc_Wrap>& trap;
public:
	CTrapGuard(CTrap<TypeFunc_Wrap>& t): trap(t) {trap.Swap();}
	~CTrapGuard() {trap.Swap();}
};

class CWrapperForSyntaxCheck
{
public:
	typedef BOOL(CWnd::*PPRETRANSLATE)(MSG*);
	typedef int	(CBLModule7::*TFunc_CBLModule7_Compile)();
	typedef void (CMainMessageHandler::*TFunc_CMainMessageHandler_WriteMessage)(char const *,enum MessageMarker,int);

	typedef void (CMainMessageHandler::*TFunc_CMainMessageHandler_ClearAll)(void);
	typedef void (CMainMessageHandler::*TFunc_CMainMessageHandler_Hide)(void);
	
	static void SetHooks();
	static void DestroyHooks();
	
	static int CallOrigCBLModuleCompile(CBLModule7* pMod);

private:
	static CTrap<TFunc_CBLModule7_Compile> trCompile;
	int	Wrap_CBLModule_Compile();
 	static bool bInsideCompileHandler;

	// �������� ������ � ���� ���������
	static CTrap<TFunc_CMainMessageHandler_WriteMessage> TrWrap_CMainMessageHandler_WriteMessage;
	void Wrap_CMainMessageHandler_WriteMessage(char const * szString, enum MessageMarker marker, int i1);

	// ��� ��������� ������� ������ "�������-��������" �� ������ "��������� ��������"
	static HHOOK WH_CallWndProc_HookHandle;
	static LRESULT __stdcall WH_CallWndProc_Hook(int code, WPARAM wParam, LPARAM lParam);

	// ��� ��������� ������� F4 - ������� � ��������� ������
	//		TODO ���� ������ ���, ��������� ������ � ����
	//
	// ��� ��������� ������ "�������-��������" ����� ������ ���������
	//
	BOOL PreTranslateMessage(MSG* pMsg);
	static PPRETRANSLATE m_pPreTranslate;
};

#endif