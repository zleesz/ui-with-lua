#include "StdAfx.h"
#include "UIEventWindow.h"
#include "UIWindowBase.h"

CUIEventWindow::CUIEventWindow(void)
{
}

CUIEventWindow::CUIEventWindow(CUIWindowBase* p) : CUIEventBase(p)
{
}

CUIEventWindow::~CUIEventWindow(void)
{
}

void CUIEventWindow::ParserOneEvent(LPXMLDOMNode pNode)
{
	if(pNode == NULL || pNode->strName != "event")
		return;
	LPXMLAttrMap pAttrMap = pNode->pMapAttr;
	if(pAttrMap == NULL)
		return;
	LPEventNode pEventNode = NULL;
	AttachListener(pAttrMap, pEventNode);
	ATLASSERT(pEventNode);
	if(NULL != pEventNode)
	{
		//pEventNode->
	}
}

BOOL CUIEventWindow::ParserEvent(LPXMLDOMNode pNode)
{
	if (NULL == pNode || pNode->strName != "eventlist")
		return FALSE;
	LPXMLChildNodes pChildNode = pNode->pMapChildNode;
	if(pChildNode == NULL)
		return FALSE;
	XMLChildNodes::const_iterator it = pChildNode->find("event");
	if(it == pChildNode->end())
	{
		return FALSE;
	}
	LPXMLVecNodes pVecNodes = it->second->pVecNode;
	for(XMLVecNodes::size_type st = 0; st != pVecNodes->size(); st++)
	{
		ParserOneEvent((*pVecNodes)[st]);
	}
	return TRUE;
}

LRESULT CUIEventWindow::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	LOG_AUTO();
	CComVariant avarParams[1];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;

	UIDISPPARAMS params = { avarParams, "OnCreate", 1, 0 };
	DispatchListener(params);
	return 0;
}

LRESULT CUIEventWindow::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	LOG_AUTO();
	CComVariant avarParams[1];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;

	UIDISPPARAMS params = { avarParams, "OnClose", 1, 0 };
	DispatchListener(params);
	return 0;
}

LRESULT CUIEventWindow::OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	LOG_AUTO();
	CComVariant avarParams[2];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;
	avarParams[1].vt = VT_I2;
	avarParams[1].boolVal = (VARIANT_BOOL)wParam;

	UIDISPPARAMS params = { avarParams, "OnShowWindow", 2, 0 };
	DispatchListener(params);
	return 0;
}

LRESULT CUIEventWindow::OnMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	LOG_AUTO();
	CComVariant avarParams[3];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;
	avarParams[1] = (int)(short) LOWORD(lParam);
	avarParams[2] = (int)(short) HIWORD(lParam);

	UIDISPPARAMS params = { avarParams, "OnMove", 3, 0 };
	DispatchListener(params);
	return 0;
}

LRESULT CUIEventWindow::OnMoving(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	LOG_AUTO();
	CComVariant avarParams[5];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;
	RECT* pRc = (RECT*)(LONG_PTR)lParam;
	avarParams[1] = pRc->left;
	avarParams[2] = pRc->top;
	avarParams[3] = pRc->right;
	avarParams[4] = pRc->bottom;

	UIDISPPARAMS params = { avarParams, "OnMoving", 5, 0 };
	DispatchListener(params);
	return 0;
}

LRESULT CUIEventWindow::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	LOG_AUTO();
	CComVariant avarParams[4];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;
	avarParams[1] = (LONG)wParam;
	avarParams[2] = (int)(short) LOWORD(lParam);
	avarParams[3] = (int)(short) HIWORD(lParam);

	UIDISPPARAMS params = { avarParams, "OnSize", 4, 0 };
	DispatchListener(params);
	return 0;
}

LRESULT CUIEventWindow::OnEnableChange(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	LOG_AUTO();
	CComVariant avarParams[2];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;
	avarParams[1].vt = VT_I2;
	avarParams[1].boolVal = (VARIANT_BOOL)wParam;

	UIDISPPARAMS params = { avarParams, "OnEnableChange", 2, 0 };
	DispatchListener(params);
	return 0;
}

LRESULT CUIEventWindow::OnSetFocus(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	LOG_AUTO();
	CComVariant avarParams[3];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;
	avarParams[1].vt = VT_I2;
	avarParams[1].boolVal = VARIANT_TRUE;
	if(wParam != NULL)
	{
		avarParams[2] = (LONG)(LONG_PTR)wParam;
	}

	UIDISPPARAMS params = { avarParams, "OnFocusChange", 3, 0 };
	DispatchListener(params);
	return 0;
}

LRESULT CUIEventWindow::OnKillFocus(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	LOG_AUTO();
	CComVariant avarParams[3];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)m_pBindWnd;
	avarParams[1].vt = VT_I2;
	avarParams[1].boolVal = VARIANT_FALSE;
	if(wParam != NULL)
	{
		avarParams[2] = (LONG)(LONG_PTR)wParam;
	}

	UIDISPPARAMS params = { avarParams, "OnFocusChange", 3, 0 };
	DispatchListener(params);
	return 0;
}

LRESULT CUIEventWindow::OnInputMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	LOG_AUTO();
	lua_State* L = UILuaGetLuaVM(NULL);
	for(int i = 0; i < (int)m_vecFilter.size(); i++)
	{
		UILuaPushClassObj(L, (void*)m_pBindWnd);
		lua_pushinteger(L, uMsg);
		lua_pushinteger(L, wParam);
		lua_pushinteger(L, lParam);
		CUILuaManager::GetInstance().CallLuaFuncByIndex(m_vecFilter[i].nFunc, 4, 0, NULL);
	}
	return 0;
}

void CUIEventWindow::AddInputFilter(lua_State* L)
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