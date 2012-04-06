#include "StdAfx.h"
#include "UILuaManager.h"

CUILuaManager::CUILuaManager()
{
}

int CUILuaManager::UIDoLuaFile(lua_State* luaState)
{
	int top = lua_gettop(luaState);
	assert(top > 0);
	char szError[_MAX_PATH] = {0};
	const char* filename = lua_tostring(luaState, -1);
	top = lua_gettop(luaState);
	int bret = 0;
	char szPath[MAX_PATH] = {0};
	strcpy_s(szPath, MAX_PATH, filename);
	strlwr(szPath);
	if(::PathFileExistsA(szPath))
	{
		lua_pushlightuserdata(luaState, (void*)this);
		lua_pushcclosure(luaState, on_error, 1);
		int errfunc = lua_gettop(luaState);
		if(luaL_loadfile(luaState, szPath) == 0)
		{
			int nFunIndex = luaL_ref(luaState, LUA_REGISTRYINDEX);
			lua_newtable(luaState);
			luaL_newmetatable(luaState, szPath);
			lua_pushstring(luaState, "__index");
			lua_pushvalue(luaState, LUA_GLOBALSINDEX);
			lua_settable(luaState, -3);
			lua_setmetatable(luaState, -2);
			lua_rawgeti(luaState, LUA_REGISTRYINDEX, nFunIndex);
			lua_pushvalue(luaState, -2);
			lua_setfenv(luaState, -2);
			int ret = lua_pcall(luaState, 0, 0, errfunc);
			if(ret == 0)
			{
				lua_pop(luaState, 1);
				bret = 1;
				LuaVM2Path2FuncMap::const_iterator it = m_mapLuaVM2Path2Func.find(luaState);
				if(it == m_mapLuaVM2Path2Func.end())
				{
					ATLASSERT(FALSE);
					luaL_unref(luaState, LUA_REGISTRYINDEX, nFunIndex);
					return 0;
				}
				LPFuncName2IndexMap pMapFuncName2Index = NULL;
				LPPath2FuncMap pMapPath2Func = it->second;
				Path2FuncMap::const_iterator it2 = pMapPath2Func->find(std::string(szPath));
				if(it2 == pMapPath2Func->end())
				{
					pMapFuncName2Index = new FuncName2IndexMap;
					pMapPath2Func->insert(std::make_pair(szPath, pMapFuncName2Index));
				}
				else
				{
					pMapFuncName2Index = it2->second;
				}
				lua_rawgeti(luaState, LUA_REGISTRYINDEX, nFunIndex);
				lua_getfenv(luaState, -1);
				lua_pushnil(luaState);
				while(lua_next(luaState, -2) != 0)
				{
					const char* pszFuncName = lua_tostring(luaState, -2);
					int nEnvFunIndex = luaL_ref(luaState, LUA_REGISTRYINDEX);
					pMapFuncName2Index->insert(std::make_pair(pszFuncName, nEnvFunIndex));
				}
			}
			else
			{
				const char* szcError = lua_tostring(luaState, -1);
				sprintf_s(szError, _MAX_PATH, "path : %s\r\nerror:\r\n%s", szPath, szcError);
				luaL_unref(luaState, LUA_REGISTRYINDEX, nFunIndex);
			}
		}
		else
		{
			const char* szcError = lua_tostring(luaState, -1);
			sprintf_s(szError, _MAX_PATH, "path : %s\r\nerror:\r\n%s", szPath, szcError);
		}
		lua_pop(luaState, 1);
	}
	else
	{
		sprintf_s(szError, _MAX_PATH, "加载lua失败,lua文件不存在, path=%s", filename);
	}
	if(bret == 0)
	{
		if(m_strStack.length() > 0)
		{
			int ret = MessageBoxA(NULL, szError, "加载lua文件失败", MB_OKCANCEL);
			if(ret != IDCANCEL)
			{
				MessageBoxA(NULL, m_strStack.c_str(), "加载lua文件失败", MB_OK | MB_ICONEXCLAMATION);
			}
		}
		else
		{
			MessageBoxA(NULL, szError, "加载lua文件失败", MB_OK);
		}
	}
	lua_pushboolean(luaState, bret);
	return 1;
}

void CUILuaManager::call_stack(lua_State* L, int n, std::string &strStack)
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

		call_stack(L, n+1, strStack);
	}
}

int CUILuaManager::on_error(lua_State *L)
{
	CUILuaManager* pUILuaManager = (CUILuaManager*)lua_touserdata(L, lua_upvalueindex(1));
	char szStack[_MAX_PATH] = {0};
	sprintf_s(szStack, _MAX_PATH, "%s", lua_tostring(L, -1));
	pUILuaManager->m_strStack += szStack;

	call_stack(L, 0, pUILuaManager->m_strStack);

	return 0;	
}


BOOL CUILuaManager::UILuaCall(lua_State* luaState, int nArgs, int nRet)
{
	int top = lua_gettop(luaState);
	assert(top > 0);
	if(top <= 0)
		return FALSE;
	bool bIsFunc = lua_isfunction(luaState, top-nArgs);
	assert(bIsFunc);
	if(!bIsFunc)
		return FALSE;
	char szError[_MAX_PATH] = {0};
	lua_pushlightuserdata(luaState, (void*)this);
	lua_pushcclosure(luaState, on_error, 1);
	int errfunc = lua_gettop(luaState);
	lua_insert(luaState, errfunc-1-nArgs);
	int ret = lua_pcall(luaState, nArgs, nRet, errfunc-1-nArgs);
	if(ret == 0)
	{
		lua_pop(luaState, 1);
		return TRUE;
	}
	const char* szcError = lua_tostring(luaState, -1);
	sprintf_s(szError, _MAX_PATH, "call function failed:\r\n%s", szcError);
	if(m_strStack.length() > 0)
	{
		int ret = MessageBoxA(NULL, szError, "调用lua函数失败", MB_OKCANCEL);
		if(ret != IDCANCEL)
		{
			MessageBoxA(NULL, m_strStack.c_str(), "调用lua函数失败", MB_OK | MB_ICONEXCLAMATION);
		}
	}
	else
	{
		MessageBoxA(NULL, szError, "调用lua函数失败", MB_OK);
	}
	return FALSE;
}
int CUILuaManager::DoXmlLuaFile(const char* szFilePath, const char* szVMName)
{
	assert(szFilePath);
	if(NULL == szFilePath)
	{
		return 0;
	}
	lua_State* L = UILuaGetLuaVM(szVMName);
	LPPath2FuncMap pMapPath2Func = NULL;
	LuaVM2Path2FuncMap::const_iterator it = m_mapLuaVM2Path2Func.find(L);
	if(it == m_mapLuaVM2Path2Func.end())
	{
		pMapPath2Func = new Path2FuncMap;
		m_mapLuaVM2Path2Func.insert(std::make_pair(L, pMapPath2Func));
	}
	else
	{
		pMapPath2Func = it->second;
	}
	std::string strRealPath = szFilePath;
	std::transform(strRealPath.begin(), strRealPath.end(), strRealPath.begin(), tolower);
	Path2FuncMap::const_iterator it2 = pMapPath2Func->find(strRealPath);
	if(it2 != pMapPath2Func->end())
	{
		return 0;
	}
	lua_State* luaState = UILuaGetLuaVM(szVMName);
	assert(luaState);
	if(luaState)
	{
		lua_pushstring(luaState, szFilePath);
		UILuaManagerInstance.UIDoLuaFile(luaState);
		return lua_toboolean(luaState, -1);
	}
	return 0;
}

int CUILuaManager::GetLuaFuncIndex(const std::string& strPath, const std::string& strFuncName, const char* szVMName)
{
	LOG_AUTO();
	std::string strRealPath = strPath;
	std::transform(strPath.begin(),strPath.end(),strRealPath.begin(),tolower);
	lua_State* L = UILuaGetLuaVM(szVMName);
	LuaVM2Path2FuncMap::const_iterator it = m_mapLuaVM2Path2Func.find(L);
	if(it == m_mapLuaVM2Path2Func.end())
	{
		ATLASSERT(FALSE);
		return 0;
	}
	Path2FuncMap::const_iterator it2 = it->second->find(strRealPath);
	if(it2 == it->second->end())
	{
		ATLASSERT(FALSE);
		return 0;
	}
	LPFuncName2IndexMap pMapFuncName2Index = it2->second;
	ATLASSERT(pMapFuncName2Index);
	if(pMapFuncName2Index == NULL)
	{
		return 0;
	}
	FuncName2IndexMap::const_iterator it3 = pMapFuncName2Index->find(strFuncName);
	if(it3 == pMapFuncName2Index->end())
	{
		return 0;
	}
	return it3->second;
}

BOOL CUILuaManager::CallLuaFuncByIndex(int nIndex, int nArgs, int nRet, const char* szVMName)
{
	lua_State* L = UILuaGetLuaVM(szVMName);
	lua_rawgeti(L, LUA_REGISTRYINDEX, nIndex);
	int top = lua_gettop(L);
	lua_insert(L, top-nArgs);
	return UILuaCall(L, nArgs, nRet);
}

BOOL CUILuaManager::CallLuaFunc(const std::string& strPath, const std::string& strFuncName, int nArgs, int nRet, const char* szVMName)
{
	int nIndex = GetLuaFuncIndex(strPath, strFuncName, szVMName);
	if(nIndex <= 0)
	{
		return FALSE;
	}
	return CallLuaFuncByIndex(nIndex, nArgs, nRet, szVMName);
}

CUILuaManager& CUILuaManager::GetInstance()
{
	static CUILuaManager s_UILuaManager;
	return s_UILuaManager;
}

void CUILuaManager::RegisterLuaFunc(const UILuaGlobalAPI& globalAPI)
{
	lua_State* luaState = UILuaGetLuaVM(NULL);
	lua_register(luaState, globalAPI.name, globalAPI.func);
}

void CUILuaManager::RegisterLuaFuncs(const UILuaGlobalAPI* pGlobalAPI)
{
	lua_State* luaState = UILuaGetLuaVM(NULL);
	for(int i = 0; pGlobalAPI[i].name; i++)
	{
		lua_register(luaState, pGlobalAPI[i].name, pGlobalAPI[i].func);
	}
}