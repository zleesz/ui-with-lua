#include "StdAfx.h"
#include "UIWindowBase.h"
#include "Util.h"

CUIWindowBase::CUIWindowBase(const std::string& strPath, LPXMLDOMNode pNode)
	: m_pUITreeContainer(NULL), 
	m_pUIEventWindow(NULL), 
	m_pUIWindowResizer(NULL),
	m_nUserData(0)
{
	LOG_AUTO();
	if(pNode == NULL || pNode->pMapAttr == NULL)
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	m_pUIEventWindow = new CUIEventWndContainer(this);
	m_pUITreeContainer = new CUITreeContainer(this);
	m_pUIWindowResizer = new CUIWindowResizer(this);
	m_strXMLPath = strPath;
	std::transform(strPath.begin(),strPath.end(),m_strXMLPath.begin(),tolower);
	//PathCanonicalizeA(m_strXMLPath, m_strXMLPath);
	LPXMLDOMNode pUITreeNode = (*pNode->pMapChildNode)["uitree"];
	if(pUITreeNode != NULL && pUITreeNode->pMapChildNode != NULL)
	{
		ParserUITree(pUITreeNode);
	}
	LPXMLDOMNode pAttrNode = (*pNode->pMapChildNode)["attr"];
	if(pAttrNode != NULL && pAttrNode->pMapChildNode != NULL)
	{
		ParserAttr(pAttrNode);
	}
	LPXMLDOMNode pEventNode = (*pNode->pMapChildNode)["eventlist"];
	if(pEventNode != NULL && pEventNode->pMapChildNode != NULL)
	{
		ParserEvent(pEventNode);
	}
}

CUIWindowBase::~CUIWindowBase(void)
{
	if (m_nUserData > 0)
	{
		lua_State* L = UILuaGetLuaVM(NULL);
		luaL_unref(L, LUA_REGISTRYINDEX, m_nUserData);
	}
	if (m_hWnd)
	{
		DestroyWindow();
	}
	if(NULL != m_pUIEventWindow)
	{
		delete m_pUIEventWindow;
	}
	if(NULL != m_pUITreeContainer)
	{
		delete m_pUITreeContainer;
	}
	if(NULL != m_pUIWindowResizer)
	{
		delete m_pUIWindowResizer;
	}
}

void CUIWindowBase::ParserResizeValue(const std::string& strValue)
{
	m_pUIWindowResizer->SetResizeEnable(TRUE);
	m_pUIWindowResizer->SetResizeLeft(atoi(strValue.c_str()));
	int nIndex = strValue.find(',');
	if ((std::string::size_type)nIndex == std::string::npos)
	{
		return;
	}
	m_pUIWindowResizer->SetResizeTop(atoi(strValue.c_str()));
	nIndex = strValue.find(',', nIndex + 1);
	if ((std::string::size_type)nIndex == std::string::npos)
	{
		return;
	}
	m_pUIWindowResizer->SetResizeRight(atoi(strValue.c_str()));
	nIndex = strValue.find(',', nIndex + 1);
	if ((std::string::size_type)nIndex == std::string::npos)
	{
		return;
	}
	m_pUIWindowResizer->SetResizeBottom(atoi(strValue.c_str()));
	nIndex = strValue.find(',', nIndex + 1);
	if ((std::string::size_type)nIndex == std::string::npos)
	{
		return;
	}
	m_pUIWindowResizer->SetResizeLeftTop(atoi(strValue.c_str()));
	nIndex = strValue.find(',', nIndex + 1);
	if ((std::string::size_type)nIndex == std::string::npos)
	{
		return;
	}
	m_pUIWindowResizer->SetResizeRightTop(atoi(strValue.c_str()));
	nIndex = strValue.find(',', nIndex + 1);
	if ((std::string::size_type)nIndex == std::string::npos)
	{
		return;
	}
	m_pUIWindowResizer->SetResizeRightBottom(atoi(strValue.c_str()));
	nIndex = strValue.find(',', nIndex + 1);
	if ((std::string::size_type)nIndex == std::string::npos)
	{
		return;
	}
	m_pUIWindowResizer->SetResizeLeftBottom(atoi(strValue.c_str()));
}

void CUIWindowBase::SetAttr(const std::string& strName, const std::string& strValue)
{
	if(strName == "left" || strName == "top" || strName == "width" || strName == "height" ||
		strName == "alpha" ||
		strName == "minwidth" || strName == "minheight" || strName == "maxwidth" || strName == "maxheight" ||
		strName == "cornereclipse")
	{
		CComVariant v(atoi(strValue.c_str()));
		v.Detach(&m_mapAttr[strName]);
	}
	else if(strName == "visible" ||
		strName == "layered" ||
		strName == "appwindow" ||
		strName == "toolwindow" ||
		strName == "min" || strName == "max")
	{
		CComVariant v;
		if(strValue == "true" || strValue == "1")
		{
			v = VARIANT_TRUE;
		}
		else
		{
			v = VARIANT_FALSE;
		}
		v.Detach(&m_mapAttr[strName]);
	}
	else if(strName == "title")
	{
		std::wstring wstrValue;
		Util::UTF8_to_Unicode(strValue.c_str(), wstrValue);
		CComVariant v(wstrValue.c_str());
		v.Detach(&m_mapAttr[strName]);
	}
	else if (strName == "resizerect")
	{
		ParserResizeValue(strValue);
	}
}

void CUIWindowBase::GetAttr(std::string strName, VARIANT* v)
{
	VariantInit(v);
	ID2AttrMap::const_iterator it = m_mapAttr.find(strName);
	if(it != m_mapAttr.end())
	{
		CComVariant vAttr;
		vAttr = it->second;
		vAttr.Detach(v);
	}
}

BOOL CUIWindowBase::ParserAttr(LPXMLDOMNode pAttrNode)
{
	if (NULL == pAttrNode || pAttrNode->strName != "attr")
		return FALSE;
	LPXMLChildNodes pChildNodes = pAttrNode->pMapChildNode;
	if(NULL == pChildNodes)
		return FALSE;
	XMLChildNodes::const_iterator it = pChildNodes->begin();
	for(; it != pChildNodes->end(); it++)
	{
		SetAttr(it->first, it->second->strUData);
	}
	return TRUE;
}

BOOL CUIWindowBase::ParserUITree(LPXMLDOMNode pNode)
{
	if (NULL == pNode || pNode->strName != "uitree")
		return FALSE;
	return m_pUITreeContainer->ParserUITree(pNode);
}

BOOL CUIWindowBase::ParserEvent(LPXMLDOMNode pNode)
{
	if (NULL == pNode || pNode->strName != "eventlist")
		return FALSE;
	return m_pUIEventWindow->ParserEvent(pNode);
}

DWORD CUIWindowBase::GetStyle()
{
	DWORD dwStyle = 0; 
	if (m_mapAttr["enable"].vt == VT_I2 && m_mapAttr["enable"].boolVal != VARIANT_TRUE)
	{
		dwStyle |= WS_DISABLED;
	}

	if (m_mapAttr["max"].vt == VT_I2 && m_mapAttr["max"].boolVal == VARIANT_TRUE)
	{
		dwStyle |= WS_MAXIMIZEBOX;
	}

	if (m_mapAttr["min"].vt == VT_I2 && m_mapAttr["min"].boolVal == VARIANT_TRUE)
	{
		dwStyle |= WS_MINIMIZEBOX;
	}
	return dwStyle;
}

DWORD CUIWindowBase::GetStyleEx()
{
	DWORD dwExStyle = 0; 
	if (m_mapAttr["layered"].vt == VT_I2 && m_mapAttr["layered"].boolVal == VARIANT_TRUE)
	{
		dwExStyle |= WS_EX_LAYERED;
	}
	if (m_mapAttr["topmost"].vt == VT_I2 && m_mapAttr["topmost"].boolVal == VARIANT_TRUE)
	{
		dwExStyle |= WS_EX_TOPMOST;
	}
	if (m_mapAttr["appwindow"].vt == VT_I2 && m_mapAttr["appwindow"].boolVal == VARIANT_TRUE)
	{
		dwExStyle |= WS_EX_APPWINDOW;
	}
	if (m_mapAttr["toolwindow"].vt == VT_I2 && m_mapAttr["toolwindow"].boolVal == VARIANT_TRUE)
	{
		dwExStyle |= WS_EX_TOOLWINDOW;
	}
	if (m_mapAttr["topmost"].vt == VT_I2 && m_mapAttr["topmost"].boolVal == VARIANT_TRUE)
	{
		dwExStyle |= WS_EX_TOPMOST;
	}
	return dwExStyle;
}

BOOL CUIWindowBase::GetLayered()
{
	if (m_hWnd)
	{
		return GetStyleEx() & WS_EX_LAYERED;
	}
	if (m_mapAttr["layered"].vt == VT_I2 && m_mapAttr["layered"].boolVal == VARIANT_TRUE)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowBase::SetLayered(BOOL bLayered)
{
	SetAttr("layered", bLayered ? "true" : "false");
	if (m_hWnd)
	{
		return bLayered ? ModifyStyleEx(0, WS_EX_LAYERED) : ModifyStyleEx(WS_EX_LAYERED, 0);
	}
	return TRUE;
}

BOOL CUIWindowBase::GetResizable()
{
	if (m_mapAttr["resize"].vt == VT_I2 && m_mapAttr["resize"].boolVal == VARIANT_TRUE)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowBase::SetResizable(BOOL bResizable)
{
	SetAttr("resize", bResizable ? "true" : "false");
	return TRUE;
}

void CUIWindowBase::GetWindowRect(LPRECT rc)
{
	if (m_hWnd)
	{
		CWindowImpl<CUIWindowBase>::GetWindowRect(rc);
		return;
	}
	CComVariant vLeft, vTop, vRight, vBottom;
	GetAttr("left", &vLeft);
	GetAttr("top", &vTop);
	GetAttr("right", &vRight);
	GetAttr("bottom", &vBottom);

	rc->left = vLeft.vt == VT_I4 ? vLeft.intVal : 0;
	rc->top = vTop.vt == VT_I4 ? vTop.intVal : 0;
	rc->right = vRight.vt == VT_I4 ? vRight.intVal : 0;
	rc->bottom = vBottom.vt == VT_I4 ? vBottom.intVal : 0;
}

void CUIWindowBase::Min()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void CUIWindowBase::Max()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void CUIWindowBase::Restore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
}

void CUIWindowBase::Destroy()
{
	DestroyWindow();
}

void CUIWindowBase::Move(LONG lnLeft, LONG lnTop, LONG lnWidth, LONG lnHeight)
{
	if (m_hWnd)
	{
		MoveWindow(lnLeft, lnTop, lnWidth, lnHeight);
		return;
	}
	char szLeft[10] = {0};
	char szTop[10] = {0};
	char szWidth[10] = {0};
	char szHeight[10] = {0};

	_itoa_s(lnLeft, szLeft, _countof(szLeft), 10);
	_itoa_s(lnTop, szTop, _countof(szLeft), 10);
	_itoa_s(lnWidth, szWidth, _countof(szLeft), 10);
	_itoa_s(lnHeight, szHeight, _countof(szLeft), 10);

	SetAttr("left", szLeft);
	SetAttr("top", szTop);
	SetAttr("width", szWidth);
	SetAttr("height", szHeight);
}

LRESULT CUIWindowBase::OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	if (IsIconic())
		bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CUIWindowBase::OnGetMinMaxInfo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	MINMAXINFO *mminfo = (PMINMAXINFO)lParam;
	CComVariant vMaxWidth, vMaxHeight, vMinWidth, vMinHeight;
	GetAttr("maxwidth", &vMaxWidth);
	GetAttr("maxheight", &vMaxHeight);
	GetAttr("minwidth", &vMinWidth);
	GetAttr("minheight", &vMinHeight);
	HMONITOR hMonitor = ::MonitorFromWindow(m_hWnd , MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo = {0};
	monitorInfo.cbSize = sizeof(MONITORINFO);
	if (::GetMonitorInfo(hMonitor, &monitorInfo))
	{
		mminfo->ptMaxPosition.x = monitorInfo.rcWork.left;
		mminfo->ptMaxPosition.y = monitorInfo.rcWork.top;
	}
	if (vMaxWidth.vt == VT_I4 && vMaxWidth.intVal > 0)
	{
		mminfo->ptMaxSize.x = vMaxWidth.intVal;
		mminfo->ptMaxTrackSize.x = vMaxWidth.intVal;
	}
	else
	{
		mminfo->ptMaxSize.x = monitorInfo.rcWork.right - monitorInfo.rcWork.left;
		mminfo->ptMaxTrackSize.x = monitorInfo.rcWork.right - monitorInfo.rcWork.left;
	}
	if (vMaxHeight.vt == VT_I4 && vMaxHeight.intVal > 0)
	{
		mminfo->ptMaxSize.y = vMaxHeight.intVal;
		mminfo->ptMaxTrackSize.y = vMaxHeight.intVal;
	}
	else
	{
		mminfo->ptMaxSize.y = monitorInfo.rcWork.bottom - monitorInfo.rcWork.top;
		mminfo->ptMaxTrackSize.y = monitorInfo.rcWork.bottom - monitorInfo.rcWork.top;
	}
	if (vMinWidth.vt == VT_I4 && vMinWidth.intVal > 0)
	{
		mminfo->ptMinTrackSize.x = vMinWidth.intVal;
	}
	if (vMinHeight.vt == VT_I4 && vMinHeight.intVal > 0)
	{
		mminfo->ptMinTrackSize.y = vMinHeight.intVal;
	}
	return 0;
}

LRESULT CUIWindowBase::OnNcCalcSize(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	return 0;
}

int CUIWindowBase::GetID(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	ATLASSERT(pThis);
	std::string strID = pThis->GetID();
	lua_pushstring(L, strID.c_str());
	return 1;
}

int CUIWindowBase::GetTitle(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	ATLASSERT(pThis);
	CComVariant vTitle = pThis->m_mapAttr["title"];
	if(vTitle.vt == VT_BSTR)
	{
		std::string strTitle;
		Util::Unicode_to_UTF8(vTitle.bstrVal, strTitle);
		lua_pushstring(L, strTitle.c_str());
	}
	return 1;
}

int CUIWindowBase::SetTitle(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	ATLASSERT(pThis);
	const char* pszTitle = lua_tostring(L, 2);
	pThis->SetAttr("title", pszTitle);
	if (!pThis->m_hWnd)
	{
		return 0;
	}
	CComVariant vTitle = pThis->m_mapAttr["title"];
	if(vTitle.vt == VT_BSTR)
	{
		pThis->SetWindowText(vTitle.bstrVal);
	}
	return 0;
}

int CUIWindowBase::GetVisible(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	lua_pushboolean(L, pThis->IsWindowVisible());
	return 1;
}

int CUIWindowBase::SetVisible(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	int bVisible = lua_toboolean(L, 2);
	pThis->ShowWindow(bVisible == 0 ? SW_HIDE : SW_SHOWNORMAL);
	return 0;
}

int CUIWindowBase::Show(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}

	int nCmd = (int)lua_tointeger(L, 2);
	pThis->ShowWindow(nCmd);
	return 0;
}

int CUIWindowBase::GetTreeContainer(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	ATLASSERT(pThis);
	UILuaPushClassObj(L, (void*)pThis->m_pUITreeContainer);
	return 1;
}

int CUIWindowBase::AddInputFilter(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	pThis->m_pUIEventWindow->AddInputFilter(L);
	return 0;
}

int CUIWindowBase::AttachListener(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	return pThis->m_pUIEventWindow->AttachListener(L);
}

int CUIWindowBase::DetachListener(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	return pThis->m_pUIEventWindow->DetachListener(L);
}

int CUIWindowBase::Min(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	pThis->Min();
	return 0;
}

int CUIWindowBase::Max(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	pThis->Max();
	return 0;
}

int CUIWindowBase::Restore(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	pThis->Restore();
	return 0;
}

int CUIWindowBase::Destroy(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	pThis->Destroy();
	return 0;
}

int CUIWindowBase::Move(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	LONG lnLeft = lua_tointeger(L, 2);
	LONG lnTop = lua_tointeger(L, 3);
	LONG lnWidth = lua_tointeger(L, 4);
	LONG lnHeight = lua_tointeger(L, 5);
	pThis->Move(lnLeft, lnTop, lnWidth, lnHeight);
	return 0;
}

int CUIWindowBase::SetUserData(lua_State* luaState)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(luaState, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	if (lua_gettop(luaState) < 2 || lua_isnil(luaState, 2))
	{
		pThis->m_nUserData = 0;
		return 0;
	}
	lua_pushvalue(luaState, 2);
	pThis->m_nUserData = luaL_ref(luaState, LUA_REGISTRYINDEX);
	return 0;
}

int CUIWindowBase::GetUserData(lua_State* luaState)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(luaState, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	if (pThis->m_nUserData <= 0)
	{
		return 0;
	}
	lua_rawgeti(luaState, LUA_REGISTRYINDEX, pThis->m_nUserData);
	return 1;
}

int CUIWindowBase::GetLayered(lua_State* luaState)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(luaState, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	lua_pushboolean(luaState, pThis->GetLayered());
	return 1;
}

int CUIWindowBase::SetMaxTrackSize(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	LONG lnWidth = (LONG)lua_tointeger(L, 2);
	LONG lnHeight = (LONG)lua_tointeger(L, 3);
	char szWidth[10] = {0}, szHeight[10] = {0};
	_itoa(lnWidth, szWidth, 10);
	_itoa(lnHeight, szHeight, 10);
	pThis->SetAttr("maxwidth", szWidth);
	pThis->SetAttr("maxheight", szHeight);
	return 0;
}

int CUIWindowBase::SetMinTrackSize(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	LONG lnWidth = (LONG)lua_tointeger(L, 2);
	LONG lnHeight = (LONG)lua_tointeger(L, 3);
	char szWidth[10] = {0}, szHeight[10] = {0};
	_itoa(lnWidth, szWidth, 10);
	_itoa(lnHeight, szHeight, 10);
	pThis->SetAttr("minwidth", szWidth);
	pThis->SetAttr("minheight", szHeight);
	return 0;
}

int CUIWindowBase::SetResizable(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	BOOL bResizable = (BOOL)lua_toboolean(L, 2);
	pThis->SetResizable(bResizable);
	return 0;
}

int CUIWindowBase::GetWindowRect(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	RECT rc = {0};
	pThis->GetWindowRect(&rc);
	lua_pushinteger(L, (lua_Integer)rc.left);
	lua_pushinteger(L, (lua_Integer)rc.top);
	lua_pushinteger(L, (lua_Integer)rc.right);
	lua_pushinteger(L, (lua_Integer)rc.bottom);
	return 4;
}

int CUIWindowBase::GetParent(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	CWindowImpl<CUIWindowBase>* pWindow = static_cast<CWindowImpl<CUIWindowBase>*>(pThis);
	HWND hWnd = pWindow->GetParent();
	lua_pushlightuserdata(L, (void*)hWnd);
	return 1;
}

int CUIWindowBase::SetParent(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	CWindowImpl<CUIWindowBase>* pWindow = static_cast<CWindowImpl<CUIWindowBase>*>(pThis);
	HWND hWnd = NULL;
	if (lua_isuserdata(L, 2))
	{
		hWnd = (HWND)lua_touserdata(L, 2);
	}
	else if (lua_isnumber(L, 2))
	{
		hWnd = (HWND)(LONG)lua_tonumber(L, 2);
	}
	if (hWnd == NULL)
	{
		ATLASSERT(FALSE);
	}
	pWindow->SetParent(hWnd);
	return 0;
}

int CUIWindowBase::GetHWND(lua_State* L)
{
	CUIWindowBase** ppThis = (CUIWindowBase**) lua_touserdata(L, 1);
	CUIWindowBase* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	lua_pushlightuserdata(L, (void*)pThis->m_hWnd);
	return 1;
}