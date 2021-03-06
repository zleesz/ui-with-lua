
#include "UILuaGlobalFactory.h"
#include <assert.h>
#include "UILuaUtil.h"

bool UILuaGlobalFactory::IsGlobalRegistered(lua_State* L, const char* szName)
{
	assert(szName);
	if(NULL == szName)
		return false;
	LuaVM2MapLuaObjectMap::const_iterator it = m_mapGlobalObject.find(L);
	if(it != m_mapGlobalObject.end())
	{
		ID2LuaObjectMap::const_iterator it2 = it->second->find(szName);
		if(it2 != it->second->end())
		{
			return true;
		}
	}
	return false;
}

void UILuaGlobalFactory::RegisterGlobal(lua_State* L, UILuaObject theObj)
{
	assert(theObj.ObjName); assert(theObj.pfnGetObject); assert(theObj.MemberFunctions);
	if(NULL == theObj.ObjName ||
		NULL == theObj.pfnGetObject||
		NULL == theObj.MemberFunctions)
		return;
	bool bRigister = IsGlobalRegistered(L, theObj.ObjName);
	if(bRigister)
	{
		assert(false && "重复注册 global");
		return;
	}
	ID2LuaObjectMap* pmapID2LuaObject = NULL;
	LuaVM2MapLuaObjectMap::const_iterator it = m_mapGlobalObject.find(L);
	if(it == m_mapGlobalObject.end())
	{
		pmapID2LuaObject = new ID2LuaObjectMap;
		m_mapGlobalObject.insert(std::make_pair(L, pmapID2LuaObject));
	}
	else
	{
		pmapID2LuaObject = it->second;
	}
	UILuaObject* pTheObj = new UILuaObject;
	memcpy((void*)pTheObj, (const void*)&theObj, sizeof(UILuaObject));
	(*pmapID2LuaObject)[theObj.ObjName] = pTheObj;

	int top = lua_gettop(L);
	int n = luaL_newmetatable(L, pTheObj->ObjName);
	LOG_TRACE("n : " << n);
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, &UILuaGlobalFactory::gc_obj);
	lua_settable(L, -3);

	// 把方法也注册进userdata的元表里
	for (int i = 0; pTheObj->MemberFunctions[i].name != NULL; ++i)
	{
		lua_pushstring(L, pTheObj->MemberFunctions[i].name);
		lua_pushlightuserdata(L, (void*)pTheObj);
		lua_pushnumber(L, i);
		lua_pushcclosure(L, &UILuaGlobalFactory::proxy, 2);
		lua_settable(L, -3);
	}
	// 注册__index方法
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	lua_settop(L, top);
}

void UILuaGlobalFactory::UnRegisterGlobal(lua_State* L, const char* szName)
{
}

int UILuaGlobalFactory::GetGlobalObject(lua_State* L, const char* szObjName)
{
	LuaVM2MapLuaObjectMap::const_iterator it = m_mapGlobalObject.find(L);
	if(it == m_mapGlobalObject.end())
	{
		return 0;
	}
	ID2LuaObjectMap::const_iterator it2 = it->second->find(szObjName);
	if(it2 == it->second->end())
	{
		return 0;
	}
	UILuaObject* pTheObj = it2->second;
	void* obj = pTheObj->pfnGetObject(pTheObj->userData);

	userdataType *ud = static_cast<userdataType*>(lua_newuserdata(L, sizeof(userdataType)));
	ud->p = (void*)obj;
	luaL_getmetatable(L, pTheObj->ObjName);
	lua_setmetatable(L, -2);
	return 1;
}

int UILuaGlobalFactory::gc_obj(lua_State* L)
{
	LOG_AUTO();
	void* obj = (void*)lua_touserdata(L, -1);
	LOG_DEBUG(_T("gc_obj obj:") << obj);
	// delete (*obj);
	return 0;
}

int UILuaGlobalFactory::proxy(lua_State* L)
{
	// 取出要调用的函数编号
	UILuaObject* pTheObj = (UILuaObject*)lua_touserdata(L, lua_upvalueindex(1));
	userdataType *ud = static_cast<userdataType*>(luaL_checkudata(L, 1, pTheObj->ObjName));
	if (!ud)
	{
		luaL_typerror(L, 1, pTheObj->ObjName);
		assert(false);
		return 0;
	}
	lua_remove(L, 1);
	int i = (int)lua_tonumber(L, lua_upvalueindex(2));

	assert(NULL != pTheObj);
	if(NULL == pTheObj)
	{
		return 0;
	}
	if(!pTheObj->MemberFunctions || !pTheObj->MemberFunctions[i].name)
	{
		return 0;
	}
	// 实际的调用函数
	void* p = lua_newuserdata(L, sizeof(void*));
	*((void**)p) = ud->p;
	luaL_getmetatable(L, pTheObj->ObjName);
	lua_setmetatable(L, -2);
	lua_insert(L, 1);
	return pTheObj->MemberFunctions[i].func(L);
}

void UILuaGlobalFactory::PushGlobalObj(lua_State* L,const char* szName)
{
	assert(szName);
	LOG_TRACE(_T("PushGlobalObj name:") << szName);
	if (NULL == szName)
	{
		lua_pushnil(L);
		return;
	}
	LuaVM2MapLuaObjectMap::const_iterator it = m_mapGlobalObject.find(L);
	if (it == m_mapGlobalObject.end())
	{
		lua_pushnil(L);
		return;
	}
	ID2LuaObjectMap::const_iterator it2 = it->second->find(szName);
	if (it2 == it->second->end())
	{
		lua_pushnil(L);
		return;
	}
	UILuaObject* pTheObj = it2->second;
	assert(pTheObj);
	LOG_TRACE(_T("PushGlobalObj pTheObj:") << pTheObj);
	if (NULL == pTheObj)
	{
		lua_pushnil(L);
		return;
	}
	userdataType *ud = static_cast<userdataType*>(lua_newuserdata(L, sizeof(userdataType)));
	ud->p = (void*)pTheObj->pfnGetObject(NULL);
	luaL_getmetatable(L, pTheObj->ObjName);
	lua_setmetatable(L, -2);
}

UILuaGlobalFactory& UILuaGlobalFactory::GetInstance()
{
	static UILuaGlobalFactory s_UILuaGlobalFactory;
	return s_UILuaGlobalFactory;
}

LUA_API void UILuaRegisterGlobalObj(UILuaObject theObj, const char* szVMName)
{
	lua_State* luaState = UILuaGetLuaVM(szVMName);
	assert(luaState);
	if(luaState)
	{
		UIluaGlobalFactoryInstance.RegisterGlobal(luaState, theObj);
	}
}

LUA_API void UILuaUnRegisterGlobalObj(const char* szName, const char* szVMName)
{
	lua_State* luaState = UILuaGetLuaVM(szVMName);
	assert(luaState);
	if(luaState)
	{
		UIluaGlobalFactoryInstance.UnRegisterGlobal(luaState, szName);
	}
}

LUA_API void UILuaPushGlobalObj(lua_State* luaState, const char* szName)
{
	assert(luaState);
	if(luaState)
	{
		UIluaGlobalFactoryInstance.PushGlobalObj(luaState, szName);
	}
}
