#pragma once
extern "C"
{
#define LUA_CORE
#include "luaconf.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

#include <string>
#include <map>


class UILuaVMManager
{
private:
	UILuaVMManager() : m_strDefaultVMName("__default_lua_vm__") {};
	UILuaVMManager(UILuaVMManager&){};
	UILuaVMManager& operator=(UILuaVMManager&){};
	~UILuaVMManager()
	{
		// destroy all lua vm
	};
private:
	typedef std::map<std::string, lua_State*> ID2LuaVMMap;
	ID2LuaVMMap m_mapLuaVM;
	std::string m_strDefaultVMName;
public:
	static int UIMessageBox(lua_State* luaState);
private:
	void InitUtilFunc(lua_State* luaState);
public:
	lua_State* CreateLuaVM(const char* szVMName);
	lua_State* GetLuaVM(const char* szVMName);
	const std::string& GetLuaVMName(lua_State* L);
	void DestroyLuaVM(const char* szVMName);
public:
	static UILuaVMManager& GetInstance();
};

#define UILuaVMManagerInstance UILuaVMManager::GetInstance()