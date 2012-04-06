#pragma once
#include <uiluax.h>

typedef std::map<std::string, int> FuncName2IndexMap, *LPFuncName2IndexMap;
typedef std::map<std::string, LPFuncName2IndexMap> Path2FuncMap, *LPPath2FuncMap;
typedef std::map<lua_State*, LPPath2FuncMap> LuaVM2Path2FuncMap, *LPLuaVM2Path2FuncMap;
class CUILuaManager
{
private:
	CUILuaManager();
	CUILuaManager(CUILuaManager&){};
	CUILuaManager& operator=(CUILuaManager&){};
	~CUILuaManager()
	{
		// delete all ...
		// luaL_unref(luaState, LUA_REGISTRYINDEX, nFunIndex);
	};
private:
	int UIDoLuaFile(lua_State* luaState);
	int UILuaCall(lua_State* luaState, int nArgs, int nRet);
	static void call_stack(lua_State* L, int n, std::string &strStack);
	static int on_error(lua_State *L);
private:
	LuaVM2Path2FuncMap m_mapLuaVM2Path2Func;
	std::string m_strStack;
public:
	int DoXmlLuaFile(const char* szFilePath, const char* szVMName);
	int GetLuaFuncIndex(const std::string& strPath, const std::string& strFuncName, const char* szVMName);
	BOOL CallLuaFuncByIndex(int nIndex, int nArgs, int nRet, const char* szVMName);
	BOOL CallLuaFunc(const std::string& strPath, const std::string& strFuncName, int nArgs, int nRet, const char* szVMName);
	void RegisterLuaFunc(const UILuaGlobalAPI& globalAPI);
	void RegisterLuaFuncs(const UILuaGlobalAPI* pGlobalAPI);
public:
	static CUILuaManager& GetInstance();
public:
	LOG_CLS_DEC();
};
#define UILuaManagerInstance CUILuaManager::GetInstance()