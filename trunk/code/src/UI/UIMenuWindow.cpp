#include "StdAfx.h"
#include ".\UIMenuWindow.h"

CUIMenuWindow::~CUIMenuWindow(void)
{
	UnRegisterClass(this);
}

CUIMenuWindow::CUIMenuWindow(const std::string& strPath, LPXMLDOMNode pNode)
: CUIWindowBase(strPath, pNode)
{
	RegisterClass(this);
	LPXMLDOMNode pAttrNode = (*pNode->pMapChildNode)["attr"];
	if(pAttrNode != NULL && pAttrNode->pMapChildNode != NULL)
	{
		ParserAttr(pAttrNode);
	}
}

BOOL CUIMenuWindow::Render(CDCHandle dc)
{
	return TRUE;
}

DWORD CUIMenuWindow::GetStyle()
{
	DWORD dwStyle = WS_POPUP | WS_CLIPCHILDREN | /*WS_THICKFRAME | */WS_CLIPSIBLINGS | WS_SYSMENU | WS_TABSTOP | CUIWindowBase::GetStyle();
	return dwStyle;
}

DWORD CUIMenuWindow::GetStyleEx()
{
	DWORD dwExStyle = CUIWindowBase::GetStyleEx(); 
	if (m_mapAttr["appwindow"].vt == VT_I2 && m_mapAttr["appwindow"].boolVal == VARIANT_TRUE)
	{
		dwExStyle |= WS_EX_APPWINDOW;
	}
	return dwExStyle;
}

BOOL CUIMenuWindow::CreateWnd(HWND hParent)
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
			TryUpdateWindow();
		}
		UpdateWindow();
	}
	return TRUE;
}

void CUIMenuWindow::SetAttr(const std::string& strName, const std::string& strValue)
{
	if (strName == "sysmenu")
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
}

void CUIMenuWindow::GetAttr(std::string strName, VARIANT* v)
{
}

BOOL CUIMenuWindow::ParserAttr(LPXMLDOMNode pAttrNode)
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

LRESULT CUIMenuWindow::OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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


LRESULT CUIMenuWindow::OnNcPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	LOG_AUTO();
	HDC WindowDC = ::GetWindowDC(m_hWnd);
	::ReleaseDC(m_hWnd, WindowDC);
	return 0;
}

LRESULT CUIMenuWindow::OnCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	CComVariant vTitle = m_mapAttr["title"];
	if(vTitle.vt == VT_BSTR)
	{
		SetWindowText(vTitle.bstrVal);
	}
	bHandled = FALSE;
	// 	int iRet; 
	// 	BOOL bRet; 
	// 	CRgn m_rgn1,m_rgn2,m_rgn3; 
	// 	RECT r; 
	// 
	// 	GetWindowRect(&r); 
	// 	OffsetRect(&r,-r.left,-r.top); 
	// 	m_rgn1.CreateRoundRectRgn(r.left,r.top,r.right+1,r.top+45,12,12); 
	// 	m_rgn2.CreateRoundRectRgn(r.left,r.top+18,r.right+1,r.bottom+2,12,12); 
	// 	m_rgn2.CombineRgn(m_rgn1,m_rgn2,RGN_OR); 
	// 	iRet = SetWindowRgn(m_rgn2,TRUE); 
	return 0;
}

void CUIMenuWindow::TryUpdateWindow()
{
	CPaintDC dc(m_hWnd);
	CMemoryDC dcMem(dc.m_hDC, dc.m_ps.rcPaint);
	DoPaint(dcMem.m_hDC);
}

void CUIMenuWindow::DoPaint(CDCHandle dc)
{
	if(NULL == m_pUITreeContainer)
	{
		assert(false);
		return;
	}
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
		RECT wndrc = {0};
		GetWindowRect(&wndrc);
		POINT pt = {wndrc.left, wndrc.top};
		SIZE sz = {wndrc.right - wndrc.left, wndrc.bottom - wndrc.top};
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