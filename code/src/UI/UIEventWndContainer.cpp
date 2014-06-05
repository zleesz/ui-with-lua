#include "StdAfx.h"
#include "UIEventWndContainer.h"
#include "UIWindowBase.h"

CUIEventWndContainer::CUIEventWndContainer(void)
{
}

CUIEventWndContainer::CUIEventWndContainer(CUIWindowBase* p) : m_pBindWnd(p)
{
}

CUIEventWndContainer::~CUIEventWndContainer(void)
{
}


LRESULT CUIEventWndContainer::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	LOG_AUTO();
	bHandled = FALSE;
	CComVariant avarParams[1];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;

	CComVariant avarRets[1];
	UIDISPPARAMS params = { avarParams, "OnCreate", 1, 1, avarRets };
	DispatchListener(params);
	if (avarRets[0].vt == VT_BOOL)
	{
		bHandled = avarRets[0].boolVal ? TRUE : FALSE;
	}
	return 0;
}

LRESULT CUIEventWndContainer::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	LOG_AUTO();
	bHandled = FALSE;
	CComVariant avarParams[1];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;

	CComVariant avarRets[1];
	UIDISPPARAMS params = { avarParams, "OnClose", 1, 1, avarRets };
	DispatchListener(params);
	if (avarRets[0].vt == VT_BOOL)
	{
		bHandled = avarRets[0].boolVal ? TRUE : FALSE;
	}
	return 0;
}

LRESULT CUIEventWndContainer::OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	LOG_AUTO();
	bHandled = FALSE;
	CComVariant avarParams[2];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;
	avarParams[1].vt = VT_I2;
	avarParams[1].boolVal = (VARIANT_BOOL)wParam;

	CComVariant avarRets[1];
	UIDISPPARAMS params = { avarParams, "OnShowWindow", 2, 1, avarRets };
	DispatchListener(params);
	if (avarRets[0].vt == VT_BOOL)
	{
		bHandled = avarRets[0].boolVal ? TRUE : FALSE;
	}
	return 0;
}

LRESULT CUIEventWndContainer::OnMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	LOG_AUTO();
	bHandled = FALSE;
	CComVariant avarParams[3];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;
	avarParams[1] = (int)(short) LOWORD(lParam);
	avarParams[2] = (int)(short) HIWORD(lParam);

	CComVariant avarRets[1];
	UIDISPPARAMS params = { avarParams, "OnMove", 3, 1 };
	DispatchListener(params);
	if (avarRets[0].vt == VT_BOOL)
	{
		bHandled = avarRets[0].boolVal ? TRUE : FALSE;
	}
	return 0;
}

LRESULT CUIEventWndContainer::OnMoving(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	LOG_AUTO();
	bHandled = FALSE;
	CComVariant avarParams[5];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;
	RECT* pRc = (RECT*)(LONG_PTR)lParam;
	avarParams[1] = pRc->left;
	avarParams[2] = pRc->top;
	avarParams[3] = pRc->right;
	avarParams[4] = pRc->bottom;

	CComVariant avarRets[1];
	UIDISPPARAMS params = { avarParams, "OnMoving", 5, 1, avarRets };
	DispatchListener(params);
	if (avarRets[0].vt == VT_BOOL)
	{
		bHandled = avarRets[0].boolVal ? TRUE : FALSE;
	}
	return 0;
}

LRESULT CUIEventWndContainer::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LOG_AUTO();
	bHandled = FALSE;
	CComVariant avarParams[4];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;
	avarParams[1] = (LONG)wParam;
	avarParams[2] = (int)(short) LOWORD(lParam);
	avarParams[3] = (int)(short) HIWORD(lParam);

	CComVariant avarRets[1];
	UIDISPPARAMS params = { avarParams, "OnSize", 4, 1, avarRets };
	DispatchListener(params);
	if (avarRets[0].vt == VT_BOOL)
	{
		bHandled = avarRets[0].boolVal ? TRUE : FALSE;
	}
	return 0;
}

LRESULT CUIEventWndContainer::OnEnableChange(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	LOG_AUTO();
	bHandled = FALSE;
	CComVariant avarParams[2];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;
	avarParams[1].vt = VT_I2;
	avarParams[1].boolVal = (VARIANT_BOOL)wParam;

	CComVariant avarRets[1];
	UIDISPPARAMS params = { avarParams, "OnEnableChange", 2, 1, avarRets };
	DispatchListener(params);
	if (avarRets[0].vt == VT_BOOL)
	{
		bHandled = avarRets[0].boolVal ? TRUE : FALSE;
	}
	return 0;
}

LRESULT CUIEventWndContainer::OnSetFocus(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	LOG_AUTO();
	bHandled = FALSE;
	CComVariant avarParams[3];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;
	avarParams[1].vt = VT_I2;
	avarParams[1].boolVal = VARIANT_TRUE;
	if(wParam != NULL)
	{
		avarParams[2] = (LONG)(LONG_PTR)wParam;
	}

	CComVariant avarRets[1];
	UIDISPPARAMS params = { avarParams, "OnFocusChange", 3, 1, avarRets };
	DispatchListener(params);
	if (avarRets[0].vt == VT_BOOL)
	{
		bHandled = avarRets[0].boolVal ? TRUE : FALSE;
	}
	return 0;
}

LRESULT CUIEventWndContainer::OnKillFocus(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	LOG_AUTO();
	bHandled = FALSE;
	CComVariant avarParams[3];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;
	avarParams[1].vt = VT_I2;
	avarParams[1].boolVal = VARIANT_FALSE;
	if(wParam != NULL)
	{
		avarParams[2] = (LONG)(LONG_PTR)wParam;
	}

	CComVariant avarRets[1];
	UIDISPPARAMS params = { avarParams, "OnFocusChange", 3, 1, avarRets };
	DispatchListener(params);
	if (avarRets[0].vt == VT_BOOL)
	{
		bHandled = avarRets[0].boolVal ? TRUE : FALSE;
	}
	return 0;
}

LRESULT CUIEventWndContainer::OnInputMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//LOG_AUTO();
	bHandled = FALSE;
	lua_State* L = UILuaGetLuaVM(NULL);
	for(int i = 0; i < (int)m_vecFilter.size(); i++)
	{
		UILuaPushClassObj(L, (void*)m_pBindWnd);
		lua_pushinteger(L, uMsg);
		lua_pushinteger(L, wParam);
		lua_pushinteger(L, lParam);
		UILuaManagerInstance.CallLuaFuncByIndex(m_vecFilter[i].nFunc, 4, 1, NULL);
		if (lua_isboolean(L, -1))
		{
			bHandled = (BOOL)lua_toboolean(L, -1);
			if (bHandled)
			{
				break;
			}
		}
	}
	return 0;
}

void CUIEventWndContainer::AddInputFilter(lua_State* L)
{
	LOG_AUTO();
	bool b = lua_isfunction(L, 2);
	ATLASSERT(b);
	if(!b)
		return;
	lua_pushvalue(L, 2);
	int nFunIndex = luaL_ref(L, LUA_REGISTRYINDEX);
	m_vecFilter.push_back(FilterNode(nFunIndex));
	return;
}