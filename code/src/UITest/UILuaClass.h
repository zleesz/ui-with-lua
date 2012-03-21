#pragma once
#include <uiluax.h>
#include <uilog4cplus.h>

class CUILuaClass
{
public:
	CUILuaClass(void);
	virtual ~CUILuaClass(void);
public:
	static int Exit(lua_State* L);
public:
	BEGIN_LUA_CALL_MAP(CUILuaClass)
		LUA_CALL_ENTRY(Exit)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_SINGLETON_OBJECT(CUILuaClass, UITest.LuaClass);
public:
	LOG_CLS_DEC();
};
