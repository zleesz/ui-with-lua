#pragma once

//////////////////////////////LuaSupport//////////////////////////////////

extern "C"
{
#include <lua.h>
#include <luaconf.h>
#include <lauxlib.h>
};
// for class
#define BEGIN_LUA_CALL_MAP(x) \
public: \
	static UILuaGlobalAPI* _GetLuaCallEntries() throw() { \
	static UILuaGlobalAPI s_LuaCallMemberFunctions[] = {
#define LUA_CALL_ENTRY(func) {#func, func},
#define LUA_CALL_ENTRY2(funname, func) {#funname, func},
#define END_LUA_CALL_MAP \
	{NULL, NULL} \
	}; \
	return s_LuaCallMemberFunctions; \
};

#define LUA_CALL_GETOBJECT(x) \
	static x* __stdcall GetInstance(void* p) { \
		return (x*)p; \
	};
#define LUA_CALL_REGISTER_OBJECT(x, classname) \
	LUA_CALL_GETOBJECT(x); \
	static void RegisterClass(void* p) { \
		UILuaObject theObject; \
		theObject.MemberFunctions = _GetLuaCallEntries(); \
		theObject.ObjName = #classname; \
		theObject.userData = p; \
		theObject.pfnGetObject = (fnGetObject)x::GetInstance; \
		UILuaRegisterClass(theObject, NULL);  \
	}; \
	static void UnRegisterClass(void* p) { \
		UILuaUnRegisterClass(p, NULL); \
	};

#define LUA_CALL_GETSINGLETON(x) \
	static x* GetInstance() { \
		static x* pTheOne = NULL; \
		if(pTheOne == NULL) \
		{ \
			pTheOne = new x(); \
			x::RegisterClass(); \
		} \
		return pTheOne; \
	}; \
	static x* __stdcall _GetInstance(void*) { \
		return x::GetInstance(); \
	};
#define LUA_CALL_REGISTER_SINGLETON_OBJECT(x, classname) \
	LUA_CALL_GETSINGLETON(x); \
	static void RegisterClass() { \
		UILuaObject theObject; \
		theObject.MemberFunctions = _GetLuaCallEntries(); \
		theObject.ObjName = #classname; \
		theObject.userData = NULL; \
		theObject.pfnGetObject = (fnGetObject)x::_GetInstance; \
		UILuaRegisterGlobalObj(theObject, NULL);  \
};

//////////////////////////////////////////////////////////////////////////