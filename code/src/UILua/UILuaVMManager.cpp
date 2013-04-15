#include "UILuaVMManager.h"
#include "UILuaUtil.h"
#include <assert.h>

lua_State* UILuaVMManager::CreateLuaVM(const char* szVMName)
{
	std::string strVMName;
	if(szVMName == NULL) // default vm
	{
		strVMName = m_strDefaultVMName;
	}
	else
	{
		strVMName = szVMName;
	}
	ID2LuaVMMap::const_iterator it = m_mapLuaVM.find(strVMName);
	if(it != m_mapLuaVM.end())
	{
		assert(false);
		return it->second;
	}
	lua_State* luaState = luaL_newstate();
	luaopen_base(luaState);
	luaL_openlibs(luaState);
	assert(luaState);
	if(luaState)
	{
		m_mapLuaVM.insert(std::make_pair(strVMName, luaState));
	}
	UILuaUtil::InitUtilFunc(luaState);
	return luaState;
}

lua_State* UILuaVMManager::GetLuaVM(const char* szVMName)
{
	std::string strVMName;
	if(szVMName == NULL) // default vm
	{
		strVMName = m_strDefaultVMName;
	}
	else
	{
		strVMName = szVMName;
	}
	ID2LuaVMMap::const_iterator it = m_mapLuaVM.find(strVMName);
	if(it != m_mapLuaVM.end())
	{
		return it->second;
	}
	return NULL;
}

const std::string& UILuaVMManager::GetLuaVMName(lua_State* L)
{
	ID2LuaVMMap::const_iterator it = m_mapLuaVM.begin();
	for(; it != m_mapLuaVM.end(); it++)	
	{
		if(it->second == L)
			return it->first;
	}
	static std::string nullString;
	return nullString;
}

void UILuaVMManager::DestroyLuaVM(const char* szVMName)
{
	std::string strVMName;
	if(szVMName == NULL) // default vm
	{
		strVMName = m_strDefaultVMName;
	}
	else
	{
		strVMName = szVMName;
	}
	lua_State* luaState = GetLuaVM(strVMName.c_str());
	assert(luaState);
	if(luaState)
	{
		lua_close(luaState);
		m_mapLuaVM.erase(strVMName);
	}
}

UILuaVMManager& UILuaVMManager::GetInstance()
{
	static UILuaVMManager s_UILuaVMManager;
	return s_UILuaVMManager;
}

LUA_API lua_State* UILuaCreateLuaVM(const char* szVMName)
{
	return UILuaVMManagerInstance.CreateLuaVM(szVMName);
}
LUA_API lua_State* UILuaGetLuaVM(const char* szVMName)
{
	return UILuaVMManagerInstance.GetLuaVM(szVMName);
}
LUA_API void UILuaDestroyLuaVM(const char* szVMName)
{
	UILuaVMManagerInstance.DestroyLuaVM(szVMName);
}