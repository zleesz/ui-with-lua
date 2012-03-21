#include "StdAfx.h"
#include "UIFrameWindow.h"

CUIFrameWindow::CUIFrameWindow(void)
{
}

CUIFrameWindow::~CUIFrameWindow(void)
{
	UnRegisterClass(this);
}

CUIFrameWindow::CUIFrameWindow(const std::string& strPath, LPXMLDOMNode pNode)
	: CUIWindowBase(strPath, pNode)
{
	RegisterClass(this);
	LPXMLDOMNode pAttrNode = (*pNode->pMapChildNode)["attr"];
	if(pAttrNode != NULL && pAttrNode->pMapChildNode != NULL)
	{
		ParserAttr(pAttrNode);
	}
}

BOOL CUIFrameWindow::Render(CDCHandle dc)
{
	return TRUE;
}

DWORD CUIFrameWindow::GetStyle()
{
	DWORD dwStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |WS_SYSMENU|WS_TABSTOP;
	if (m_mapAttr["enable"].vt == VT_I2 && m_mapAttr["enable"].boolVal != VARIANT_TRUE)
	{
		dwStyle |= WS_DISABLED;
	}

	if (m_mapAttr["resize"].vt == VT_I2 && m_mapAttr["resize"].boolVal == VARIANT_TRUE)
	{
		dwStyle |= WS_THICKFRAME;
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

DWORD CUIFrameWindow::GetStyleEx()
{
	DWORD dwExStyle = 0; 
	if (m_mapAttr["appwindow"].vt == VT_I2 && m_mapAttr["appwindow"].boolVal == VARIANT_TRUE)
	{
		dwExStyle |= WS_EX_APPWINDOW;
	}

	if (m_mapAttr["topmost"].vt == VT_I2 && m_mapAttr["topmost"].boolVal == VARIANT_TRUE)
	{
		dwExStyle |= WS_EX_TOPMOST;
	}	

	if (m_mapAttr["layered"].vt == VT_I2 && m_mapAttr["layered"].boolVal == VARIANT_TRUE)
	{
		dwExStyle |= WS_EX_LAYERED;
	}

	return dwExStyle;
}

BOOL CUIFrameWindow::CreateWnd(HWND hParent)
{
	LOG_AUTO();
	m_pUIEventWindow->OnInitEvent(GetXMLPath());
	m_UITreeContainer.OnPreCreate();
	if (hParent != NULL)
	{
		ATLASSERT(::IsWindow(hParent));
		if (!::IsWindow(hParent))
		{
			hParent = NULL;
		}
	}
	CRect rcBound;
	rcBound.left = m_mapAttr["left"].intVal;
	rcBound.top = m_mapAttr["top"].intVal;
	rcBound.right = m_mapAttr["width"].intVal + rcBound.left;
	rcBound.bottom = m_mapAttr["height"].intVal + rcBound.top;
	Create(hParent, rcBound, NULL, GetStyle(), GetStyleEx());
	ATLASSERT(IsWindow());
	if(!IsWindow())
	{
		return FALSE;
	}
	if (m_mapAttr["visible"].vt == VT_I2 && m_mapAttr["visible"].boolVal == VARIANT_TRUE)
	{
		ShowWindow(SW_SHOW);
		if (m_mapAttr["layered"].vt == VT_I2 && m_mapAttr["layered"].boolVal == VARIANT_TRUE)
		{
			// TryInvalidateRgn(NULL);
		}
		UpdateWindow();
	}
	return TRUE;
}

void CUIFrameWindow::SetAttr(std::string strName, std::string strValue)
{
}

void CUIFrameWindow::GetAttr(std::string strName, VARIANT* v)
{
}

BOOL CUIFrameWindow::ParserAttr(LPXMLDOMNode pAttrNode)
{
	return TRUE;
}

LRESULT CUIFrameWindow::OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(wParam != NULL)
	{
		RECT rect = { 0 };
		GetClientRect(&rect);
		CMemoryDC dcMem((HDC)wParam, rect);
		DoPaint(dcMem.m_hDC);
	}
	else
	{
		CPaintDC dc(m_hWnd);
		CMemoryDC dcMem(dc.m_hDC, dc.m_ps.rcPaint);        
		DoPaint(dcMem.m_hDC);
	}

	return 0;
}

int CUIFrameWindow::GetID(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	std::string strID = pThis->GetID();
	lua_pushstring(L, strID.c_str());
	return 1;
}

int CUIFrameWindow::GetTreeContainer(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	UILuaPushClassObj(L, (void*)&pThis->m_UITreeContainer);
	return 1;
}

void CUIFrameWindow::DoPaint(CDCHandle dc)
{
	m_UITreeContainer.Render(dc);
}