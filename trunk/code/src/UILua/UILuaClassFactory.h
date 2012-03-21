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

class UILuaClassFactory
{
private:
	UILuaClassFactory() {};
	UILuaClassFactory(UILuaClassFactory&){};
	UILuaClassFactory& operator=(UILuaClassFactory&){};
	~UILuaClassFactory()
	{
		// delete all ...
	};
private:
	typedef std::map<const void*, UILuaObject*> Obj2LuaObjectMap;
	typedef std::map<lua_State*, Obj2LuaObjectMap*> LuaVM2MapLuaObjectMap;
	LuaVM2MapLuaObjectMap m_mapClassObject;
public:
	bool IsClassRegistered(lua_State* L, void* obj);
	void UnRegisterClass(lua_State* L, void* obj);
	void RegisterClass(lua_State* L, UILuaObject theObj);
	void PushClassObj(lua_State* luaState, const void* obj);
	static int gc_obj(lua_State* L);
	static int proxy(lua_State* L);
public:
	static UILuaClassFactory& GetInstance();
public:
	LOG_CLS_DEC();
};
