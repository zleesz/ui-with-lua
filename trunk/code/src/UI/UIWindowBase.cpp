#include "StdAfx.h"
#include "UIWindowBase.h"
#include "Util.h"

CUIWindowBase::CUIWindowBase(const std::string& strPath, LPXMLDOMNode pNode)
	: m_pUITreeContainer(NULL), 
	m_pUIEventWindow(NULL), 
	m_pUIWindowResizer(NULL)
{
	LOG_AUTO();
	if(pNode == NULL || pNode->pMapAttr == NULL)
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	assert((*pMapAttr)["id"].length() > 0);
	if((*pMapAttr)["id"].length() <= 0)
	{
		return;
	}
	m_pUIEventWindow = new CUIEventWndContainer(this);
	m_pUITreeContainer = new CUITreeContainer(this);
	m_pUIWindowResizer = new CUIWindowResizer(this);
	SetID((*pMapAttr)["id"]);
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

void CUIWindowBase::ParserCaptionValue(const std::string& strValue, RECT& rc)
{
	rc.left = atoi(strValue.c_str());
	int nIndex = strValue.find(',');
	if ((std::string::size_type)nIndex == std::string::npos)
	{
		return;
	}
	rc.top = atoi(strValue.c_str() + nIndex + 1);
	nIndex = strValue.find(',', nIndex + 1);
	if ((std::string::size_type)nIndex == std::string::npos)
	{
		return;
	}
	rc.right = atoi(strValue.c_str() + nIndex + 1);
	nIndex = strValue.find(',', nIndex + 1);
	if ((std::string::size_type)nIndex == std::string::npos)
	{
		return;
	}
	rc.bottom = atoi(strValue.c_str() + nIndex + 1);
}

void CUIWindowBase::SetAttr(const std::string& strName, const std::string& strValue)
{
	if(strName == "left" || strName == "top" || strName == "width" || strName == "height" ||
		strName == "alpha" ||
		strName == "minwidth" || strName == "minheight" || strName == "maxwidth" || strName == "maxheight")
	{
		CComVariant v(atoi(strValue.c_str()));
		v.Detach(&m_mapAttr[strName]);
	}
	else if(strName == "visible" ||
		strName == "layered" ||
		strName == "appwindow")
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
	else if (strName == "captionrect")
	{
		RECT rc = {0};
		ParserCaptionValue(strValue, rc);
		m_pUITreeContainer->AddCaptionRect(rc);
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
	GetAttr("left", &vTop);
	GetAttr("left", &vRight);
	GetAttr("left", &vBottom);

	rc->left = vLeft.vt == VT_I4 ? vLeft.intVal : 0;
	rc->top = vTop.vt == VT_I4 ? vTop.intVal : 0;
	rc->right = vRight.vt == VT_I4 ? vRight.intVal : 0;
	rc->bottom = vBottom.vt == VT_I4 ? vBottom.intVal : 0;
}

LRESULT CUIWindowBase::OnGetMinMaxInfo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	MINMAXINFO *mminfo = (PMINMAXINFO)lParam;
	CComVariant vMaxWidth, vMaxHeight, vMinWidth, vMinHeight;
	GetAttr("maxwidth", &vMaxWidth);
	GetAttr("maxheight", &vMaxHeight);
	GetAttr("minwidth", &vMinWidth);
	GetAttr("minheight", &vMinHeight);

	if (vMaxWidth.vt == VT_I4 && vMaxWidth.intVal > 0)
	{
		mminfo->ptMaxSize.x = vMaxWidth.intVal;
		mminfo->ptMaxTrackSize.x = vMaxWidth.intVal;
	}
	if (vMaxHeight.vt == VT_I4 && vMaxHeight.intVal > 0)
	{
		mminfo->ptMaxSize.y = vMaxHeight.intVal;
		mminfo->ptMaxTrackSize.y = vMaxHeight.intVal;
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

int CUIWindowBase::GetID(lua_State* luaState)
{
	CUIWindowBase* pThis = (CUIWindowBase*) lua_touserdata(luaState, -1);
	ATLASSERT(pThis);
	std::string strID = pThis->GetID();
	lua_pushstring(luaState, strID.c_str());
	return 1;
}

int CUIWindowBase::GetTitle(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	CComVariant vTitle = pThis->m_mapAttr["title"];
	if(vTitle.vt == VT_BSTR)
	{
		std::string strTitle;
		Util::BSTRToString(vTitle.bstrVal, strTitle);
		lua_pushstring(L, strTitle.c_str());
	}
	return 1;
}

int CUIWindowBase::SetTitle(lua_State* L)
{
	CUIWindowBase* pParent = (CUIWindowBase*) lua_touserdata(L, -1);
	ATLASSERT(pParent);
	const char* pszTitle = lua_tostring(L, -1);
	pParent->SetAttr("title", pszTitle);
	CUIWindowBase* pThis = (CUIWindowBase*)pParent;
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
	CUIWindowBase* pThis = (CUIWindowBase*) lua_touserdata(L, -1);
	lua_pushboolean(L, pThis->IsWindowVisible());
	return 1;
}

int CUIWindowBase::SetVisible(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*) lua_touserdata(L, -1);
	int bVisible = lua_toboolean(L, 2);
	pThis->ShowWindow(bVisible == 0 ? SW_HIDE : SW_SHOWNORMAL);
	return 0;
}

int CUIWindowBase::Show(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*) lua_touserdata(L, -1);
	int nCmd = (int)lua_tointeger(L, -2);
	pThis->ShowWindow(nCmd);
	return 0;
}

int CUIWindowBase::GetTreeContainer(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	UILuaPushClassObj(L, (void*)pThis->m_pUITreeContainer);
	return 1;
}

int CUIWindowBase::AddInputFilter(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*)lua_touserdata(L, -1);
	pThis->m_pUIEventWindow->AddInputFilter(L);
	return 0;
}

int CUIWindowBase::AttachListener(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*)lua_touserdata(L, -1);
	return pThis->m_pUIEventWindow->AttachListener(L);
}

int CUIWindowBase::DetachListener(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*)lua_touserdata(L, -1);
	return pThis->m_pUIEventWindow->DetachListener(L);
}

int CUIWindowBase::Min(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*)lua_touserdata(L, -1);
	if (!pThis->m_hWnd)
	{
		return 0;
	}
	::SendMessage(pThis->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	return 0;
}

int CUIWindowBase::Max(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*)lua_touserdata(L, -1);
	if (!pThis->m_hWnd)
	{
		return 0;
	}
	::SendMessage(pThis->m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	return 0;
}

int CUIWindowBase::Restore(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*)lua_touserdata(L, -1);
	if (!pThis->m_hWnd)
	{
		return 0;
	}
	::SendMessage(pThis->m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	return 0;
}

int CUIWindowBase::GetLayered(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*)lua_touserdata(L, -1);
	lua_pushboolean(L, pThis->GetLayered());
	return 1;
}

int CUIWindowBase::SetMaxTrackSize(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*)lua_touserdata(L, -1);
	LONG lnWidth = (LONG)lua_tointeger(L, -2);
	LONG lnHeight = (LONG)lua_tointeger(L, -3);
	char szWidth[10] = {0}, szHeight[10] = {0};
	_itoa(lnWidth, szWidth, 10);
	_itoa(lnHeight, szHeight, 10);
	pThis->SetAttr("maxwidth", szWidth);
	pThis->SetAttr("maxheight", szHeight);
	return 0;
}

int CUIWindowBase::SetMinTrackSize(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*)lua_touserdata(L, -1);
	LONG lnWidth = (LONG)lua_tointeger(L, -2);
	LONG lnHeight = (LONG)lua_tointeger(L, -3);
	char szWidth[10] = {0}, szHeight[10] = {0};
	_itoa(lnWidth, szWidth, 10);
	_itoa(lnHeight, szHeight, 10);
	pThis->SetAttr("minwidth", szWidth);
	pThis->SetAttr("minheight", szHeight);
	return 0;
}

int CUIWindowBase::SetResizable(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*)lua_touserdata(L, -1);
	BOOL bResizable = (BOOL)lua_toboolean(L, -2);
	pThis->SetResizable(bResizable);
	return 0;
}

int CUIWindowBase::GetWindowRect(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*)lua_touserdata(L, -1);
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
	CWindowImpl<CUIWindowBase>* pThis = (CWindowImpl<CUIWindowBase>*)lua_touserdata(L, -1);
	HWND hWnd = pThis->GetParent();
	lua_pushlightuserdata(L, (void*)hWnd);
	return 1;
}

int CUIWindowBase::SetParent(lua_State* L)
{
	CWindowImpl<CUIWindowBase>* pThis = (CWindowImpl<CUIWindowBase>*)lua_touserdata(L, -1);
	HWND hWnd = NULL;
	if (lua_isuserdata(L, -2))
	{
		hWnd = (HWND)lua_touserdata(L, -2);
	}
	else if (lua_isnumber(L, -2))
	{
		hWnd = (HWND)(LONG)lua_tonumber(L, -2);
	}
	if (hWnd == NULL)
	{
		ATLASSERT(FALSE);
	}
	pThis->SetParent(hWnd);
	return 0;
}

int CUIWindowBase::GetHWND(lua_State* L)
{
	CWindowImpl<CUIWindowBase>* pThis = (CWindowImpl<CUIWindowBase>*)lua_touserdata(L, -1);
	lua_pushlightuserdata(L, (void*)pThis->m_hWnd);
	return 1;
}