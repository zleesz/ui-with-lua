#include "StdAfx.h"
#include "TimerManger.h"

CUITimerManger::CUITimerManger(void) : m_lnTimerID(0)
{
	Create(HWND_MESSAGE, NULL, NULL, WS_POPUP, WS_EX_TOOLWINDOW);
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
	ATLASSERT(top >= 3);
	if(top < 3)
		return 0;
	CUITimerManger* pThis = (CUITimerManger*)lua_touserdata(L, -1);
	if(pThis == NULL)
		return 0;
	int t = lua_type(L, -4);
	if(!lua_isfunction(L, -3))
	{
		ATLASSERT(FALSE);
		return 0;
	}
	pThis->m_lnTimerID++;
	TimerData td;
	td.lnId = pThis->m_lnTimerID;
	td.lnElapse = lua_tointeger(L, -2);
	lua_pushvalue(L, -3);
	td.nFuncIndex = luaL_ref(L, LUA_REGISTRYINDEX);
	pThis->SetTimer(td.lnId, td.lnElapse, NULL);
	pThis->m_mapID2TimerData.insert(std::make_pair(td.lnId, td));
	return 1;
}

int CUITimerManger::_SetOnceTimer(lua_State* L)
{
	int top = lua_gettop(L);
	ATLASSERT(top >= 3);
	if(top < 3)
		return 0;
	CUITimerManger* pThis = (CUITimerManger*)lua_touserdata(L, -1);
	if(pThis == NULL)
		return 0;
	if(!lua_isfunction(L, -3))
	{
		ATLASSERT(FALSE);
		return 0;
	}
	pThis->m_lnTimerID++;
	TimerData td;
	td.bOnce = TRUE;
	td.lnId = pThis->m_lnTimerID;
	td.lnElapse = lua_tointeger(L, -2);
	lua_pushvalue(L, -3);
	td.nFuncIndex = luaL_ref(L, LUA_REGISTRYINDEX);
	pThis->SetTimer(td.lnId, td.lnElapse, NULL);
	pThis->m_mapID2TimerData.insert(std::make_pair(td.lnId, td));
	return 1;
}

int CUITimerManger::_KillTimer(lua_State* L)
{
	int top = lua_gettop(L);
	ATLASSERT(top >= 2);
	if(top < 2)
		return 0;
	CUITimerManger* pThis = (CUITimerManger*)lua_touserdata(L, -1);
	if(pThis == NULL)
		return 0;
	ULONG lnID = (ULONG)lua_tointeger(L, -2);
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

	CUILuaManager::GetInstance().CallLuaFuncByIndex(it->second.nFuncIndex, NULL);
	if(it->second.bOnce)
	{
		KillTimer(lnID);
		m_mapID2TimerData.erase(it);
	}
	return 0;
}
