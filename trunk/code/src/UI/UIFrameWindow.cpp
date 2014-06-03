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
	m_pUIEventWindow->OnBindEvent(GetXMLPath());
	m_pUITreeContainer->OnPreCreate();
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
			// ::UpdateLayeredWindow(m_hWnd, );
			TryUpdateLayeredWindow();
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

LRESULT CUIFrameWindow::OnCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	CComVariant vTitle = m_mapAttr["title"];
	if(vTitle.vt == VT_BSTR)
	{
		SetWindowText(vTitle.bstrVal);
	}
	bHandled = FALSE;
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

int CUIFrameWindow::GetTitle(lua_State* L)
{
	CUIFrameWindow* pThis = (CUIFrameWindow*) lua_touserdata(L, -1);
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

int CUIFrameWindow::SetTitle(lua_State* L)
{
	CUIWindowBase* pParent = (CUIWindowBase*) lua_touserdata(L, -1);
	ATLASSERT(pParent);
	const char* pszTitle = lua_tostring(L, -1);
	pParent->SetAttr("title", pszTitle);
	CUIFrameWindow* pThis = (CUIFrameWindow*)pParent;
	if(pThis->m_hWnd)
	{
		CComVariant vTitle = pThis->m_mapAttr["title"];
		if(vTitle.vt == VT_BSTR)
		{
			pThis->SetWindowText(vTitle.bstrVal);
		}
	}
	return 0;
}

int CUIFrameWindow::GetVisible(lua_State* L)
{
	CUIFrameWindow* pThis = (CUIFrameWindow*) lua_touserdata(L, -1);
	lua_pushboolean(L, pThis->IsWindowVisible());
	return 1;
}

int CUIFrameWindow::SetVisible(lua_State* L)
{
	CUIFrameWindow* pThis = (CUIFrameWindow*) lua_touserdata(L, -1);
	int bVisible = lua_toboolean(L, 2);
	pThis->ShowWindow(bVisible == 0 ? SW_HIDE : SW_SHOWNORMAL);
	return 0;
}

int CUIFrameWindow::Show(lua_State* L)
{
	CUIFrameWindow* pThis = (CUIFrameWindow*) lua_touserdata(L, -1);
	int nCmd = (int)lua_tointeger(L, -2);
	pThis->ShowWindow(nCmd);
	return 0;
}

int CUIFrameWindow::GetTreeContainer(lua_State* L)
{
	CUIWindowBase* pThis = (CUIWindowBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	UILuaPushClassObj(L, (void*)pThis->m_pUITreeContainer);
	return 1;
}

void CUIFrameWindow::TryUpdateLayeredWindow()
{
	CPaintDC dc(m_hWnd);
	CMemoryDC dcMem(dc.m_hDC, dc.m_ps.rcPaint);
	DoPaint(dcMem.m_hDC);
}

void CUIFrameWindow::DoPaint(CDCHandle dc)
{
	assert(m_pUITreeContainer);
	if(NULL != m_pUITreeContainer)
	{
		if (GetLayered())
		{
			BLENDFUNCTION bfunc;
			bfunc.AlphaFormat = AC_SRC_ALPHA;
			bfunc.BlendFlags = 0;
			bfunc.BlendOp = AC_SRC_OVER;
			bfunc.SourceConstantAlpha = 255;
			if (m_mapAttr["alpha"].vt == VT_I4 && m_mapAttr["alpha"].intVal >= 0)
			{
				bfunc.SourceConstantAlpha = (BYTE)m_mapAttr["alpha"].intVal;
			}
			RECT rc = {0};
			GetWindowRect(&rc);
			POINT pt = {rc.left, rc.top};
			SIZE sz = {rc.right - rc.left, rc.bottom - rc.top};
			HDC hMemDc = ::CreateCompatibleDC(dc.m_hDC);
			BITMAPINFO bmpinfo;
			bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmpinfo.bmiHeader.biWidth = sz.cx;
			bmpinfo.bmiHeader.biHeight = sz.cy;
			bmpinfo.bmiHeader.biPlanes = 1;
			bmpinfo.bmiHeader.biBitCount = 32;//32bpp
			bmpinfo.bmiHeader.biCompression = BI_RGB;
			void* pBits = NULL;
			HBITMAP hBitmap = CreateDIBSection(dc.m_hDC, &bmpinfo, DIB_RGB_COLORS, &pBits, NULL, 0);
			HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDc,hBitmap);
			m_pUITreeContainer->Render(hMemDc);

			POINT ptSrc = {0, 0};
			UIGraphicInstance->UpdateLayeredWindow(m_hWnd, dc.m_hDC, &pt, &sz, hMemDc, &ptSrc, 0, &bfunc, ULW_ALPHA);
			::SelectObject(hMemDc, hOldBitmap);
			::DeleteObject(hBitmap);
			::DeleteDC(hMemDc);
		}
		else
		{
			m_pUITreeContainer->Render(dc);
		}
	}
}