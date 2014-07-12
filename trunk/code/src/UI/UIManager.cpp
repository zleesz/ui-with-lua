// UIManager.cpp : Implementation of CUIManager

#include "stdafx.h"
#include "UIManager.h"
extern "C"
{
#include "../UILua/lua/luaconf.h"
#include "../UILua/lua/lua.h"
#include "../UILua/lua/lauxlib.h"
};

#include "TimerManger.h"
#include "UIGraphic.h"
#include "UIWindowFactory.h"

// CUIManager
void CUIManager::InitLog4CPlus()
{
	LOG_AUTO();
	TCHAR szCfgName[MAX_PATH] = {0};
	::GetModuleFileName(_AtlBaseModule.m_hInst, szCfgName, MAX_PATH);
	::PathRemoveFileSpec(szCfgName);
	::PathAppend(szCfgName, _T("log4cplus.cfg"));

	::OutputDebugString(szCfgName);

	if (::PathFileExists(szCfgName))
	{
		::OutputDebugString(szCfgName);
		LOG_INIT_EX((LPCTSTR)szCfgName);
	}
	else
	{
		OutputDebugString(_T("log4cplus.cfg"));
		LOG_INIT();
	}
}

HRESULT CUIManager::FinalConstruct()
{
	LOG_AUTO();
	::OleInitialize(NULL);
	InitLog4CPlus();
	UILuaCreateLuaVM(NULL);
	// init timermanager
	CUITimerManger::GetInstance();
	UIGraphicInstance->InitGraphic();
	return S_OK;
}

void CUIManager::FinalRelease()
{
	LOG_AUTO();
	UILuaDestroyLuaVM(NULL);
	UIGraphicInstance->UnInitGraphic();
	::OleUninitialize();
}

STDMETHODIMP CUIManager::LoadSkin(BSTR bstrPath)
{
	LOG_AUTO();
	m_bstrPath = bstrPath;
	std::string strOnload;
	BOOL bValid = m_XmlParser.SetSkinPath(m_bstrPath, strOnload);
	if (!bValid)
	{
#ifdef _DEBUG
		return E_FAIL;
#else
		return S_FALSE;
#endif
	}

	if (strOnload.length() > 0 && ::PathFileExistsA(strOnload.c_str()))
	{
		//lua_State* luaState = UILuaGetLuaVM(NULL);
		int b = UILuaDoFile(strOnload.c_str(), NULL);
		ATLASSERT(b == 1);
	}
	LOG_DEBUG(_T("LoadSkin onload path=") << strOnload.c_str() << _T(", path=") << bstrPath);
	return S_OK;
}
