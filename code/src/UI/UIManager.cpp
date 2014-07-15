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

static int UILuaLog(lua_State* luaState)
{
	// LOG_METHOD();
	int top = lua_gettop(luaState);
	std::string strInfo("<UILOG> ");
	for(int i = 1; i <= top; i++)
	{
		int t = lua_type(luaState, i);
		if(lua_isnumber(luaState, i) && t==LUA_TNUMBER)
		{
			int n = (int)lua_tointeger(luaState, i);
			char szN[30] = {0};
			_itoa_s(n, szN, 10);
			strInfo += szN;
		}
		else if(lua_isstring(luaState, i))
		{
			size_t nLen = 0;
			const char* sz = (const char*)lua_tolstring(luaState, i, &nLen);
			strInfo += sz;
		}
		else if(lua_isboolean(luaState, i))
		{
			int b = lua_toboolean(luaState, i);
			if(b == 0)
			{
				strInfo += "false";
			}
			else
			{
				strInfo += "true";
			}
		}
		else if(lua_isnoneornil(luaState, i))
		{
			strInfo += "nil";
		}
		else if(lua_islightuserdata(luaState, i))
		{
			LONG ln = (LONG)(LONG_PTR)lua_touserdata(luaState, i);
			char szLn[30] = {0};
			sprintf_s(szLn, 30, "lightuserdata:0x%08X", ln);
			strInfo += szLn;
		}
		else if(lua_isuserdata(luaState, i))
		{
			LONG ln = (LONG)(LONG_PTR)lua_touserdata(luaState, i);
			char szLn[30] = {0};
			sprintf_s(szLn, 30, "userdata:0x%08X", ln);
			strInfo += szLn;
		}
		else if(lua_istable(luaState, i))
		{
			strInfo += "?table?";
		}
		else if(lua_isfunction(luaState, i))
		{
			LONG ln = (LONG)(LONG_PTR)lua_topointer(luaState, i);
			char szLn[30] = {0};
			sprintf_s(szLn, 30, "function:0x%08X", ln);
			strInfo += szLn;
		}
		else
		{
			char szLn[30] = {0};
			sprintf_s(szLn, 30, "unknown:t=%d", t);
			strInfo += szLn;
		}
		strInfo += " ";
	}
	LOG_DEBUG(strInfo.c_str());
	OutputDebugStringA(strInfo.c_str());
	return 0;
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
		lua_State* luaState = UILuaGetLuaVM(NULL);
		int b = UILuaDoFile(strOnload.c_str(), NULL);
		ATLASSERT(b == 1);
	}
	LOG_DEBUG(_T("LoadSkin onload path=") << strOnload.c_str() << _T(", path=") << bstrPath);
	return S_OK;
}
