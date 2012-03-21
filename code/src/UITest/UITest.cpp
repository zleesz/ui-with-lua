// UITest.cpp : main source file for UITest.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <Util.h>
#include "..\UI\UI.h"
#include "..\UI\UI_i.c"

#include "resource.h"
#include "UILuaClass.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
	int nRet = 0;
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

void InitUIModule()
{
	TCHAR tszPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL, tszPath, MAX_PATH);
	::PathRemoveFileSpec(tszPath);
	TCHAR tszSkinPath[MAX_PATH] = {0};
	_tcsncpy_s(tszSkinPath, tszPath, MAX_PATH);
	::PathAppend(tszPath, _T("UI.dll"));
	HMODULE hModele = ::LoadLibrary(tszPath);
	ATLASSERT(hModele);
	CComPtr<IUIManager> spManager;
	Util::CreateInstanceFromHandle(hModele, CLSID_UIManager, IID_IUIManager, (void**)&spManager);
	ATLASSERT(spManager);
	// initial instance
	CUILuaClass* pUILuaClass = CUILuaClass::GetInstance();
#ifdef _DEBUG
	::PathAppend(tszSkinPath, _T("..\\..\\code\\skin\\main"));
#else
	::PathAppend(tszSkinPath, _T("skin"));
#endif
	ATLASSERT(::PathFileExists(tszSkinPath));
	CComBSTR bstrPath(tszSkinPath);
	spManager->LoadSkin(bstrPath);
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	InitUIModule();

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
