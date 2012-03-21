#pragma once
extern "C"
{
#define LUA_CORE
#include "luaconf.h"
#include "lua.h"
#include "lauxlib.h"
};
#include <uilog4cplus.h>
#include <string>
#include <map>

class UILuaGlobalFactory
{
private:
	UILuaGlobalFactory() {};
	UILuaGlobalFactory(UILuaGlobalFactory&){};
	UILuaGlobalFactory& operator=(UILuaGlobalFactory&){};
	~UILuaGlobalFactory()
	{
		// delete all ...
	};
private:
	typedef std::map<std::string, UILuaObject*> ID2LuaObjectMap;
	typedef std::map<lua_State*, ID2LuaObjectMap*> LuaVM2MapLuaObjectMap;
	LuaVM2MapLuaObjectMap m_mapGlobalObject;
public:
	bool IsGlobalRegistered(lua_State* L, const char* szName);
	void UnRegisterGlobal(const char* /*szName*/);
	void RegisterGlobal(lua_State* L, UILuaObject theObj);
	static int gc_obj(lua_State* L);
	static int proxy(lua_State* L);
	int GetGlobalObject(lua_State* L, const char* szObjName);
public:
	static UILuaGlobalFactory& GetInstance();
public:
	LOG_CLS_DEC();
};
