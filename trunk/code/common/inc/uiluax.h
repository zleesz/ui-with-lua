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
	}; \
	static const char* GetRigisterClassName() { \
		return #classname; \
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
	}; \
	const char* GetRigisterClassName() { \
		return #classname; \
	};

#include "Util.h"
namespace Util
{
namespace lua
{
	static void PushVariantToLuaStack(lua_State* L, VARIANT* pv)
	{
		switch(pv->vt)
		{
		case VT_I2:
			lua_pushboolean(L, (pv->iVal == 0) ? 0 : 1);
			break;
		case VT_I4:
			lua_pushinteger(L,pv->intVal);
			break;
		case VT_BSTR:
			{
				std::string str;
				Util::BSTRToString(pv->bstrVal,str);
				lua_pushlstring(L, str.c_str(), str.length());
			}				
			break;
		case VT_BOOL:
			lua_pushboolean(L, (pv->boolVal == VARIANT_FALSE) ? 0 : 1 );
			break;
		case VT_BYREF | VT_I4:
			UILuaPushClassObj(L, (void*)(LONG_PTR)pv->lVal);
			break;
		default:
			lua_pushnil(L);
			break;
		}
	}
}
}
//////////////////////////////////////////////////////////////////////////