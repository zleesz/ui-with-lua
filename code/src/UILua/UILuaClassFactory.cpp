
#include "UILuaClassFactory.h"
#include <assert.h>

bool UILuaClassFactory::IsClassObjectRegistered(lua_State* L, void* obj)
{
	assert(obj);
	if(NULL == obj)
		return false;
	LuaVM2MapLuaObjectMap::const_iterator it = m_mapClassObject.find(L);
	if (it == m_mapClassObject.end())
	{
		return false;
	}
	Obj2LuaObjectMap::const_iterator it2 = it->second->find(obj);
	if (it2 != it->second->end())
	{
		return true;
	}
	return false;
}

bool UILuaClassFactory::IsClassRegistered(lua_State* L, const char* pszName)
{
	assert(pszName);
	if(NULL == pszName)
		return false;
	LuaVM2MapLuaObjectMap::const_iterator it = m_mapClassObject.find(L);
	if (it == m_mapClassObject.end())
	{
		return false;
	}
	Obj2LuaObjectMap::const_iterator it2 = it->second->begin();
	for (; it2 != it->second->end(); it2++)
	{
		if (strcmp(it2->second->ObjName, pszName) == 0)
		{
			return true;
		}
	}
	return false;
}

void UILuaClassFactory::UnRegisterClass(lua_State* L, void* obj)
{
	assert(obj);
	if(NULL == obj)
		return ;
	LuaVM2MapLuaObjectMap::const_iterator it = m_mapClassObject.find(L);
	if(it != m_mapClassObject.end())
	{
		Obj2LuaObjectMap::const_iterator it2 = it->second->find(obj);
		if(it2 != it->second->end())
		{
			it->second->erase(it2);
		}
	}
}

int UILuaClassFactory::gc_obj(lua_State* L)
{
	LOG_AUTO();
	void* obj = (void*)lua_touserdata(L, -1);
	LOG_DEBUG(_T("gc_obj obj:") << obj);
	// delete (*obj);
	return 0;
}

void UILuaClassFactory::RegisterClass(lua_State* L, UILuaObject theObj)
{
	assert(theObj.ObjName); assert(theObj.pfnGetObject); assert(theObj.MemberFunctions);
	if(NULL == theObj.ObjName ||
		NULL == theObj.pfnGetObject||
		NULL == theObj.MemberFunctions)
		return;
	void* obj = theObj.pfnGetObject(theObj.userData);
	bool bRigister = IsClassObjectRegistered(L, obj);
	if (bRigister)
	{
		return;
	}
	Obj2LuaObjectMap* pmapObj2LuaObject = NULL;
	LuaVM2MapLuaObjectMap::const_iterator it = m_mapClassObject.find(L);
	if(it == m_mapClassObject.end())
	{
		pmapObj2LuaObject = new Obj2LuaObjectMap;
		m_mapClassObject.insert(std::make_pair(L, pmapObj2LuaObject));
	}
	else
	{
		pmapObj2LuaObject = it->second;
	}
	bRigister = IsClassRegistered(L, theObj.ObjName);
	UILuaObject* pTheObj = new UILuaObject;
	memcpy((void*)pTheObj, (const void*)&theObj, sizeof(UILuaObject));
	(*pmapObj2LuaObject)[obj] = pTheObj;

	if (bRigister)
	{
		return;
	}

	int top = lua_gettop(L);
	luaL_newmetatable(L, pTheObj->ObjName);

	// 把方法也注册进userdata的元表里
	for (int i = 0; pTheObj->MemberFunctions[i].name != NULL; ++i)
	{
		lua_pushstring(L, pTheObj->MemberFunctions[i].name);
		lua_pushlightuserdata(L, (void*)pTheObj);
		lua_pushnumber(L, i);
		lua_pushcclosure(L, &UILuaClassFactory::proxy, 2);
		lua_settable(L, -3);
	}
	// 注册__index方法
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	lua_settop(L, top);
}

int UILuaClassFactory::proxy(lua_State* L)
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

void UILuaClassFactory::PushClassObj(lua_State* L, const void* obj)
{
	assert(obj);
	LOG_TRACE(_T("PushClassObj obj:") << obj);
	if (NULL == obj)
	{
		assert(false);
		lua_pushnil(L);
		return;
	}
	LuaVM2MapLuaObjectMap::const_iterator it = m_mapClassObject.find(L);
	if (it == m_mapClassObject.end())
	{
		assert(false);
		lua_pushnil(L);
		return;
	}
	Obj2LuaObjectMap::const_iterator it2 = it->second->find(obj);
	if(it2 == it->second->end())
	{
		assert(false);
		lua_pushnil(L);
		return;
	}
	UILuaObject* pTheObj = it2->second;
	assert(pTheObj);
	LOG_TRACE(_T("PushClassObj pTheObj:") << pTheObj);
	if(NULL == pTheObj)
	{
		lua_pushnil(L);
		return;
	}
	userdataType *ud = static_cast<userdataType*>(lua_newuserdata(L, sizeof(userdataType)));
	ud->p = (void*)obj;
	luaL_getmetatable(L, pTheObj->ObjName);
	lua_setmetatable(L, -2);
}

UILuaClassFactory& UILuaClassFactory::GetInstance()
{
	static UILuaClassFactory s_UILuaClassFactory;
	return s_UILuaClassFactory;
}

LUA_API void UILuaRegisterClass(UILuaObject theObj, const char* szVMName)
{
	lua_State* luaState = UILuaGetLuaVM(szVMName);
	assert(luaState);
	if(luaState)
	{
		UIluaClassFactoryInstance.RegisterClass(luaState, theObj);
	}
}

LUA_API void UILuaUnRegisterClass(void* obj, const char* szVMName)
{
	lua_State* luaState = UILuaGetLuaVM(szVMName);
	assert(luaState);
	if(luaState)
	{
		UIluaClassFactoryInstance.UnRegisterClass(luaState, obj);
	}
}

LUA_API void UILuaPushClassObj(lua_State* luaState, const void* obj)
{
	assert(luaState);
	if(luaState)
	{
		UIluaClassFactoryInstance.PushClassObj(luaState, obj);
	}
}