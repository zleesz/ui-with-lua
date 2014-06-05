#include "StdAfx.h"
#include "UIWindowBase.h"
#include "Util.h"

CUIWindowBase::CUIWindowBase(void)
{
}

CUIWindowBase::CUIWindowBase(const std::string& strPath, LPXMLDOMNode pNode)
	: m_pUITreeContainer(NULL), m_pUIEventWindow(NULL)
{
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
}

void CUIWindowBase::SetAttr(const std::string& strName, const std::string& strValue)
{
	if(strName == "left" || strName == "top" || strName == "width" || strName == "height" ||
		strName == "alpha" ||
		strName == "minwidth" || strName == "minheight" || strName == "maxwidth" || strName == "maxheight" ||
		strName == "resizeleft" || strName == "resizetop" || strName == "resizeright" || strName == "resizebottom" ||
		strName == "resizelefttop" || strName == "resizeleftbottom" || strName == "resizerighttop" || strName == "resizerightbottom")
	{
		CComVariant v(atoi(strValue.c_str()));
		v.Detach(&m_mapAttr[strName]);
	}
	else if(strName == "visible" ||
		strName == "layered" ||
		strName == "appwindow" ||
		strName == "resize")
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
		Util::StringToWideString(strValue.c_str(), wstrValue);
		CComVariant v(wstrValue.c_str());
		v.Detach(&m_mapAttr[strName]);
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
		mminfo->ptMinTrackSize.x = vMinHeight.intVal;
	}
	return 0;
}

BOOL CUIWindowBase::IsInResizeLeftTopArea(const POINT& pt, const SIZE& sz)
{
	CComVariant vLeftTop;
	GetAttr("resizelefttop", &vLeftTop);
	if (vLeftTop.vt == VT_I4 && (pt.x <= vLeftTop.intVal) && pt.y <= vLeftTop.intVal)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowBase::IsInResizeLeftBottomArea(const POINT& pt, const SIZE& sz)
{
	CComVariant vLeftBottom;
	GetAttr("resizeleftbottom", &vLeftBottom);
	if (vLeftBottom.vt == VT_I4 && (pt.x <= vLeftBottom.intVal && sz.cy - pt.y <= vLeftBottom.intVal))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowBase::IsInResizeRightTopArea(const POINT& pt, const SIZE& sz)
{
	CComVariant vRightTop;
	GetAttr("resizerighttop", &vRightTop);
	if (vRightTop.vt == VT_I4 && (pt.x <= vRightTop.intVal && sz.cy - pt.y <= vRightTop.intVal))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowBase::IsInResizeRightBottomArea(const POINT& pt, const SIZE& sz)
{
	CComVariant vRightBottom;
	GetAttr("resizerightbottom", &vRightBottom);
	if (vRightBottom.vt == VT_I4 && (sz.cx - pt.x <= vRightBottom.intVal && sz.cy - pt.y <= vRightBottom.intVal))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowBase::IsInResizeLeftArea(const POINT& pt, const SIZE& /*sz*/)
{
	CComVariant vLeft;
	GetAttr("resizeleft", &vLeft);
	if (vLeft.vt == VT_I4 && pt.x <= vLeft.intVal)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowBase::IsInResizeTopArea(const POINT& pt, const SIZE& /*sz*/)
{
	CComVariant vTop;
	GetAttr("resizetop", &vTop);
	if (vTop.vt == VT_I4 && pt.y <= vTop.intVal)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowBase::IsInResizeRightArea(const POINT& pt, const SIZE& sz)
{
	CComVariant vRight;
	GetAttr("resizeright", &vRight);
	if (vRight.vt == VT_I4 && sz.cy - pt.x <= vRight.intVal)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowBase::IsInResizeBottomArea(const POINT& pt, const SIZE& sz)
{
	CComVariant vBottom;
	GetAttr("resizebottom", &vBottom);
	if (vBottom.vt == VT_I4 && sz.cy - pt.y <= vBottom.intVal)
	{
		return TRUE;
	}
	return FALSE;
}

LRESULT CUIWindowBase::OnNcHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (!GetResizable())
	{
		return 0;
	}
	POINT pt = {0};
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	ScreenToClient(&pt);

	RECT rc = {0};
	GetWindowRect(&rc);
	SIZE sz = {rc.right - rc.left, rc.bottom - rc.top};
	if (IsInResizeLeftArea(pt, sz))
	{
		bHandled = TRUE;
		return HTLEFT;
	}
	
	return 0;
}

LRESULT CUIWindowBase::OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (!GetResizable())
	{
		return 0;
	}
	int nHitTest = LOWORD(lParam);
	switch (nHitTest)
	{
	case HTLEFT:
		bHandled = TRUE;
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
		break;
	default:
		break;
	}
	return 0;
}

LRESULT CUIWindowBase::OnNcLButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (!GetResizable())
	{
		return 0;
	}
	int nHitTest = LOWORD(wParam);
	if(nHitTest == HTTOP)
	{
		bHandled = TRUE;
		SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, lParam);
	}
	else if(nHitTest == HTLEFT)
	{
		bHandled = TRUE;
		SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, lParam);
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
