#pragma once
extern "C"
{
#define LUA_CORE
#include "luaconf.h"
#include "lua.h"
#include "lauxlib.h"
};
#include <uilog4cplus.h>

class UILuaUtil
{
private:
	static std::string strStack;
private:
	static void call_stack(lua_State* L, int n);
	static int on_error(lua_State *L);
	static void UISetEnviroment(lua_State* luaState);
public:
	static void InitUtilFunc(lua_State* luaState);
public:
	static int getfenv(lua_State* luaState);
	static int UIMessageBox(lua_State* luaState);
	static int UILuaDoFile(lua_State* luaState);
	static int UILuaGetObject(lua_State* luaState);
	static int UILuaCall(lua_State* luaState, int args, int results);
public:
	LOG_CLS_DEC();
};
