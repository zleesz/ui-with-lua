#include "StdAfx.h"
#include "TimerManger.h"

CUITimerManger::CUITimerManger(void) : m_lnTimerID(0)
{
	Create(HWND_MESSAGE, NULL, NULL, WS_POPUP, WS_EX_TOOLWINDOW);
	static UILuaGlobalAPI UILuaUtilFunc[] = 
	{
		{"SetTimer", CUITimerManger::_SetTimer},
		{"SetOnceTimer", CUITimerManger::_SetOnceTimer},
		{"KillTimer", CUITimerManger::_KillTimer},
		{"AsynCall", CUITimerManger::AsynCall},
		{NULL,NULL}
	};
	for (int i = 0; UILuaUtilFunc[i].name; i++)
	{
		UILuaManagerInstance.RegisterLuaFunc(UILuaUtilFunc[i]);
	}
}

CUITimerManger::~CUITimerManger(void)
{
	if(m_hWnd)
	{
		DestroyWindow();
	}
}

int CUITimerManger::_SetTimer(lua_State* L)
{
	int top = lua_gettop(L);
	ATLASSERT(top >= 2);
	if(top < 2)
		return 0;
	CUITimerManger* pThis = UITimerManagerInstance;
	if(pThis == NULL)
		return 0;
	if(!lua_isfunction(L, top >= 3 ? 2 : 1))
	{
		ATLASSERT(FALSE);
		return 0;
	}
	pThis->m_lnTimerID++;
	TimerData td;
	td.lnId = pThis->m_lnTimerID;
	td.lnElapse = (ULONG)lua_tointeger(L, top >= 3 ? 3 : 2);
	lua_pushvalue(L, top >= 3 ? 2 : 1);
	td.nFuncIndex = luaL_ref(L, LUA_REGISTRYINDEX);
	pThis->SetTimer(td.lnId, td.lnElapse, NULL);
	pThis->m_mapID2TimerData.insert(std::make_pair(td.lnId, td));
	return 1;
}

int CUITimerManger::_SetOnceTimer(lua_State* L)
{
	int top = lua_gettop(L);
	ATLASSERT(top >= 2);
	if(top < 2)
		return 0;
	CUITimerManger* pThis = UITimerManagerInstance;
	if(pThis == NULL)
		return 0;
	if(!lua_isfunction(L, top >= 3 ? 2 : 1))
	{
		ATLASSERT(FALSE);
		return 0;
	}
	pThis->m_lnTimerID++;
	TimerData td;
	td.bOnce = TRUE;
	td.lnId = pThis->m_lnTimerID;
	td.lnElapse = (ULONG)lua_tointeger(L, top >= 3 ? 3 : 2);
	lua_pushvalue(L, top >= 3 ? 2 : 1);
	td.nFuncIndex = luaL_ref(L, LUA_REGISTRYINDEX);
	pThis->SetTimer(td.lnId, td.lnElapse, NULL);
	pThis->m_mapID2TimerData.insert(std::make_pair(td.lnId, td));
	return 1;
}

int CUITimerManger::AsynCall(lua_State* L)
{
	int top = lua_gettop(L);
	ATLASSERT(top >= 1);
	if(top < 1)
		return 0;
	CUITimerManger* pThis = UITimerManagerInstance;
	if(pThis == NULL)
		return 0;
	if(!lua_isfunction(L, -1))
	{
		ATLASSERT(FALSE);
		return 0;
	}
	pThis->m_lnTimerID++;
	TimerData td;
	td.bOnce = TRUE;
	td.lnId = pThis->m_lnTimerID;
	td.lnElapse = 0;
	lua_pushvalue(L, -1);
	td.nFuncIndex = luaL_ref(L, LUA_REGISTRYINDEX);
	pThis->SetTimer(td.lnId, td.lnElapse, NULL);
	pThis->m_mapID2TimerData.insert(std::make_pair(td.lnId, td));
	return 1;
}

int CUITimerManger::_KillTimer(lua_State* L)
{
	int top = lua_gettop(L);
	ATLASSERT(top >= 1);
	if(top < 1)
		return 0;
	CUITimerManger* pThis = UITimerManagerInstance;
	if(pThis == NULL)
		return 0;
	ULONG lnID = (ULONG)lua_tointeger(L, top >= 2 ? 2 : 1);
	if(lnID <= 0)
		return 0;
	ID2TimerDataMap::iterator it = pThis->m_mapID2TimerData.find(lnID);
	if(it == pThis->m_mapID2TimerData.end())
	{
		return 0;
	}
	pThis->KillTimer(lnID);
	pThis->m_mapID2TimerData.erase(it);
	return 0;
}

LRESULT CUITimerManger::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	ULONG lnID = (ULONG)wParam;
	ID2TimerDataMap::iterator it = m_mapID2TimerData.find(lnID);
	if(it == m_mapID2TimerData.end())
	{
		return 0;
	}
	int nFuncIndex = it->second.nFuncIndex;
	if(it->second.bOnce)
	{
		KillTimer(lnID);
		m_mapID2TimerData.erase(it);
	}

	//const char* szName = GetRigisterClassName();
	lua_State* luaState = UILuaGetLuaVM(NULL);
	UILuaPushGlobalObj(luaState, GetRigisterClassName());
	lua_pushinteger(luaState, lnID);
	UILuaManagerInstance.CallLuaFuncByIndex(nFuncIndex, 2, 0, NULL);
	return 0;
}
