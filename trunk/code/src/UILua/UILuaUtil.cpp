#pragma once
extern "C"
{
#define LUA_CORE
#include "luaconf.h"
#include "lua.h"
#include "lauxlib.h"
};
#include "UILuaUtil.h"
#include "UILuaGlobalFactory.h"
#include <Windows.h>
#include <assert.h>
#include <string>

static bool Unicode_to_Ansi( const wchar_t* src, std::string& dest)
{
	if (!src)
	{
		return false;
	}
	int  iLen = (int)wcslen(src);
	if (iLen <= 0)
	{
		return false;
	}
	char* szdest = new  char[iLen * 4];
	if(NULL == szdest)
	{
		return false;
	}
	ZeroMemory(szdest, iLen * 4);			
	int nLen = WideCharToMultiByte(936, NULL, src, iLen, szdest, iLen * 4, 0, 0);
	szdest[nLen] = '\0'; 
	dest = szdest;
	delete [] szdest ;
	return true;
}

static bool UTF8_to_Unicode(const char* src, std::wstring& wstr)
{
	if (src == NULL)
	{
		return false;
	}
	int iLen = (int)strlen(src);
	if (iLen == 0)
	{
		wstr = L"";
		return true;
	}
	wchar_t* szm = new wchar_t[iLen * 4];
	ZeroMemory(szm, iLen * 4);
	int nLen = MultiByteToWideChar(CP_UTF8, 0, src,iLen, szm, iLen*4); 
	szm [nLen] = '\0';
	wstr = szm;
	delete [] szm;
	return false;
}

std::string UILuaUtil::strStack;
int UILuaUtil::UIMessageBox(lua_State* luaState)
{
	int top = lua_gettop(luaState);
	assert(top > 0);
	if(!lua_isstring(luaState, 1))
	{
		return 0;
	}
	std::string strContent;
	std::string strTitle;
	if (top == 1)
	{
		strTitle = "信息";
		strContent = lua_tostring(luaState, 1);
	}
	else
	{
		strTitle = lua_tostring(luaState, 1);
		strContent = lua_tostring(luaState, 2);
	}
	MessageBoxA(NULL, strContent.c_str(), strTitle.c_str(), MB_OK);
	return 0;
}

void UILuaUtil::call_stack(lua_State* L, int n)
{
	lua_Debug ar;
	if(lua_getstack(L, n, &ar) == 1)
	{
		lua_getinfo(L, "nSlu", &ar);

		if(n == 0)
		{
			strStack += "\r\n<call stack>";
		}
		if(ar.name)
		{
			char szStack[_MAX_PATH] = {0};
			sprintf_s(szStack, _MAX_PATH, "\r\n%s() : line %d [%s : line %d]", ar.name, ar.currentline, ar.source, ar.linedefined);
			strStack += szStack;
		}
		else
		{
			char szStack[_MAX_PATH] = {0};
			sprintf_s(szStack, _MAX_PATH, "\r\nunknown : line %d [%s : line %d]", ar.currentline, ar.source, ar.linedefined);
			strStack += szStack;
		}

		call_stack(L, n+1);
	}
}

int UILuaUtil::on_error(lua_State *L)
{
	char szStack[_MAX_PATH] = {0};
	sprintf_s(szStack, _MAX_PATH, "%s", lua_tostring(L, -1));
	strStack += szStack;

	call_stack(L, 0);

	return 0;	
}

void UILuaUtil::UISetEnviroment(lua_State* /*luaState*/)
{
	/*lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	*/
	//lua_pushvalue(luaState, LUA_GLOBALSINDEX);
	//lua_setglobal(luaState, "_G");
	//lua_newtable(luaState);
	//c = lua_tocfunction(luaState, -1);
	//lua_getglobal(luaState, "_G");
	//int ret = lua_setfenv(luaState, -2);
}

int UILuaUtil::UILuaDoFile(lua_State* luaState)
{
	LOG_AUTO();
	strStack.clear();
	int top = lua_gettop(luaState);
	assert(top > 0);
	char szError[_MAX_PATH] = {0};
	const char* pszFileName = lua_tostring(luaState, 1);
	int bret = 0;
	std::wstring wstrFileName;
	std::string strFileName;
	UTF8_to_Unicode(pszFileName, wstrFileName);

	TCHAR tszFileName[MAX_PATH] = {0};
	::PathCanonicalize(tszFileName, wstrFileName.c_str());

	Unicode_to_Ansi(tszFileName, strFileName);
	if (::PathFileExists(tszFileName))
	{
		lua_pushcclosure(luaState, on_error, 0);
		int errfunc = lua_gettop(luaState);
		if (luaL_loadfile(luaState, strFileName.c_str()) == 0)
		{
			int nFunIndex = luaL_ref(luaState, LUA_REGISTRYINDEX);
			lua_newtable(luaState);
			// t.__document = path
			lua_pushstring(luaState, "__document");
			lua_pushstring(luaState, pszFileName);
			lua_settable(luaState, -3);

			luaL_newmetatable(luaState, pszFileName);
			lua_pushstring(luaState, "__index");
			lua_pushvalue(luaState, LUA_GLOBALSINDEX);
			// t.__index = metatable
			lua_settable(luaState, -3);
			// fun.metatable = t
			lua_setmetatable(luaState, -2);
			lua_rawgeti(luaState, LUA_REGISTRYINDEX, nFunIndex);
			lua_pushvalue(luaState, -2);
			lua_setfenv(luaState, -2);
			int ret = lua_pcall(luaState, 0, 0, errfunc);
			if (ret == 0)
			{
				lua_pop(luaState, 1);
				bret = 1;

				lua_rawgeti(luaState, LUA_REGISTRYINDEX, nFunIndex);
				lua_getfenv(luaState, -1);
				return 1;
			}
			else
			{
				const char* szcError = lua_tostring(luaState, -1);
				sprintf_s(szError, _MAX_PATH, "path : %s\r\nerror:\r\n%s", pszFileName, szcError);
				luaL_unref(luaState, LUA_REGISTRYINDEX, nFunIndex);
			}
		}
		else
		{
			const char* szcError = lua_tostring(luaState, -1);
			sprintf_s(szError, _MAX_PATH, "path : %s\r\nerror:\r\n%s", pszFileName, szcError);
		}
		lua_pop(luaState, 1);
	}
	else
	{
		sprintf_s(szError, _MAX_PATH, "加载lua失败,lua文件不存在, path=%s", pszFileName);
	}
	if (bret == 0)
	{
		if (strStack.length() > 0)
		{
			int ret = MessageBoxA(NULL, szError, "加载lua文件失败", MB_OKCANCEL);
			if (ret != IDCANCEL)
			{
				MessageBoxA(NULL, strStack.c_str(), "加载lua文件失败", MB_OK | MB_ICONEXCLAMATION);
			}
		}
		else
		{
			MessageBoxA(NULL, szError, "加载lua文件失败", MB_OK);
		}
	}
	lua_settop(luaState, top);
	return 0;
}

int UILuaUtil::UILuaCall(lua_State* luaState, int args, int results)
{
	lua_pushcclosure(luaState, on_error, 0);
	int errfunc = lua_gettop(luaState);
	lua_insert(luaState, errfunc-1-args);
	int ret = lua_pcall(luaState, args, results, errfunc-1-args);
	if (ret == 0)
	{
		lua_pop(luaState, 1);
		return 0;
	}
	else
	{
		char szError[_MAX_PATH] = {0};
		const char* szcError = lua_tostring(luaState, -1);
		sprintf_s(szError, _MAX_PATH, "error:\r\n%s", szcError);
		if(strStack.length() > 0)
		{
			int ret = MessageBoxA(NULL, szError, "加载lua文件失败", MB_OKCANCEL);
			if(ret != IDCANCEL)
			{
				MessageBoxA(NULL, strStack.c_str(), "加载lua文件失败", MB_OK | MB_ICONEXCLAMATION);
			}
		}
		else
		{
			MessageBoxA(NULL, szError, "加载lua文件失败", MB_OK);
		}
		lua_pop(luaState, 1);
		return 1;
	}
}

int UILuaUtil::UILuaLog(lua_State* luaState)
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

int UILuaUtil::UILuaGetObject(lua_State* luaState)
{
	int top = lua_gettop(luaState);
	assert(top > 0);
	if(top <= 0)
	{
		return 0;
	}
	const char* szObjName = lua_tostring(luaState, -1);
	return UIluaGlobalFactoryInstance.GetGlobalObject(luaState, szObjName);
}

void UILuaUtil::InitUtilFunc(lua_State* luaState)
{
	LOG_AUTO();
	static UILuaGlobalAPI UILuaUtilFunc[] = 
	{
		{"UIMessageBox", UILuaUtil::UIMessageBox},
		{"UILuaDoFile", UILuaUtil::UILuaDoFile},
		{"UILuaGetObject", UILuaUtil::UILuaGetObject},
		{"UILog", UILuaUtil::UILuaLog},
		{NULL,NULL}
	};
	for (int i = 0; UILuaUtilFunc[i].name; i++)
	{
		lua_register(luaState, UILuaUtilFunc[i].name, UILuaUtilFunc[i].func);
	}
}

LUA_API int UILuaDoFile(const char* szFilePath, const char* szVMName)
{
	assert(szFilePath);
	if(NULL == szFilePath)
	{
		return 0;
	}
	lua_State* luaState = UILuaGetLuaVM(szVMName);
	assert(luaState);
	if (!luaState)
	{
		return 0;
	}
	int top = lua_gettop(luaState);
	lua_pushstring(luaState, szFilePath);
	lua_insert(luaState, 1);
	int ret = UILuaUtil::UILuaDoFile(luaState);
	lua_settop(luaState, top);
	return ret;
}

LUA_API int UILuaCall(int args, int results, const char* szVMName)
{
	lua_State* luaState = UILuaGetLuaVM(szVMName);
	assert(luaState);
	if(luaState)
	{
		return UILuaUtil::UILuaCall(luaState, args, results);
	}
	return 1;
}
