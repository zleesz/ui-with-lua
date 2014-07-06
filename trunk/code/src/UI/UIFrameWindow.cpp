#include "StdAfx.h"
#include "UIFrameWindow.h"

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
	DWORD dwStyle = WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CUIWindowBase::GetStyle();
	return dwStyle;
}

DWORD CUIFrameWindow::GetStyleEx()
{
	DWORD dwExStyle = CUIWindowBase::GetStyleEx(); 
	if (m_mapAttr["appwindow"].vt == VT_I2 && m_mapAttr["appwindow"].boolVal == VARIANT_TRUE)
	{
		dwExStyle |= WS_EX_APPWINDOW;
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
			TryUpdateWindow();
		}
		UpdateWindow();
	}
	return TRUE;
}

void CUIFrameWindow::SetAttr(const std::string& strName, const std::string& strValue)
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


LRESULT CUIFrameWindow::OnNcPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	LOG_AUTO();
	return 0;
}

void CUIFrameWindow::UpdateCornerRect()
{
	CComVariant vCornerEclipse = m_mapAttr["cornereclipse"];
	if (vCornerEclipse.vt != VT_I4 || vCornerEclipse.intVal <= 0)
	{
		return;
	}
	CRgn rgn;
	RECT r;
	GetWindowRect(&r);
	OffsetRect(&r, -r.left, -r.top);
	rgn.CreateRoundRectRgn(r.left, r.top, r.right+1, r.bottom+1, vCornerEclipse.intVal, vCornerEclipse.intVal);
	SetWindowRgn(rgn, TRUE);
}

LRESULT CUIFrameWindow::OnCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	SetWindowLong(GWL_STYLE, GetWindowLong(GWL_STYLE) & ~WS_CAPTION);
	CComVariant vTitle = m_mapAttr["title"];
	if(vTitle.vt == VT_BSTR)
	{
		SetWindowText(vTitle.bstrVal);
	}
	bHandled = FALSE;

	UpdateCornerRect();
	return 0;
}

LRESULT CUIFrameWindow::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	if (wParam == SIZE_RESTORED)
	{
		//SetWindowLong(GWL_STYLE, GetWindowLong(GWL_STYLE) & ~WS_CAPTION);
	}
	UpdateCornerRect();
	return 0;
}

LRESULT CUIFrameWindow::OnSysCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	if (wParam == SC_MINIMIZE)
	{
		//SetWindowLong(GWL_STYLE, GetWindowLong(GWL_STYLE) | WS_CAPTION);
	}
	else if (wParam == SC_MAXIMIZE)
	{
		//SetWindowLong(GWL_STYLE, GetWindowLong(GWL_STYLE) & ~WS_CAPTION);
	}
	return 0;
}

void CUIFrameWindow::TryUpdateWindow()
{
	CPaintDC dc(m_hWnd);
	CMemoryDC dcMem(dc.m_hDC, dc.m_ps.rcPaint);
	DoPaint(dcMem.m_hDC);
}

void CUIFrameWindow::SaveBitmapBits(HBITMAP hBitmap, BYTE** ppBits, DWORD* pdwByteSize)
{
	if (!ppBits || !pdwByteSize)
	{
		return;
	}
	ATLASSERT(*ppBits == NULL);
	delete[] *ppBits;

	BITMAP bmp;
	if (GetObject(hBitmap, sizeof(BITMAP), &bmp))
	{
		DWORD dwPxByteSize = bmp.bmWidth*bmp.bmHeight*bmp.bmBitsPixel/8;
		BYTE* pBits = new BYTE[dwPxByteSize];
		if (pBits)
		{
			::memcpy(pBits, (BYTE*)bmp.bmBits, dwPxByteSize);
			*ppBits = pBits;
			*pdwByteSize = dwPxByteSize;
		}
	}
}

void CUIFrameWindow::MixAlpha(HBITMAP hBitmap, BYTE* pBitsSrc, DWORD dwSize)
{
	BITMAP bmp;
	::GetObject(hBitmap, sizeof(BITMAP), (void*)&bmp);
	BYTE* pDest = (BYTE*)bmp.bmBits;
	BYTE* pSrc = pBitsSrc;
	if (pDest == NULL)
	{
		return;
	}		

	WORD wBitSet = bmp.bmBitsPixel/8;
	DWORD dwPxByteSize = bmp.bmWidth*bmp.bmHeight*wBitSet;

	if (dwPxByteSize != dwSize)
	{
		return;
	}

	for(DWORD i = 0; i < dwPxByteSize; i+=DWORD(wBitSet))
	{
		if (::memcmp(pSrc+i, pDest+i, 3) != 0)
		{			
			pDest[i+3] = 0xFF;
		}
	}
}

void CUIFrameWindow::DoPaint(CDCHandle dc)
{
	if(NULL == m_pUITreeContainer)
	{
		assert(false);
		return;
	}
	if (GetLayered())
	{
		HDC hDC = CreateCompatibleDC(NULL);
		RECT rc;
		GetWindowRect(&rc);

		BITMAPINFO bmi;
		::memset(&bmi, 0, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = rc.right-rc.left;
		bmi.bmiHeader.biHeight = rc.bottom-rc.top;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = 0;

		BYTE* pBits = NULL;
		HBITMAP hDIBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
		HBITMAP hOldDIBitmap = (HBITMAP)::SelectObject(hDC, hDIBitmap);

		::FillRect(hDC, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

		DWORD dwSize = 0;
		BYTE* pBitmapBits = NULL;
		SaveBitmapBits(hDIBitmap, &pBitmapBits, &dwSize);

		BLENDFUNCTION bfunc;
		bfunc.AlphaFormat = AC_SRC_ALPHA;
		bfunc.BlendFlags = 0;
		bfunc.BlendOp = AC_SRC_OVER;
		bfunc.SourceConstantAlpha = 255;
		if (m_mapAttr["alpha"].vt == VT_I4 && m_mapAttr["alpha"].intVal >= 0)
		{
			bfunc.SourceConstantAlpha = (BYTE)m_mapAttr["alpha"].intVal;
		}
		m_pUITreeContainer->Render(hDC);

		MixAlpha(hDIBitmap, pBitmapBits, dwSize);
		POINT pt = {rc.left, rc.top};
		SIZE sz = {rc.right - rc.left, rc.bottom - rc.top};
		POINT ptSrc = {0, 0};
		UIGraphicInstance->UpdateLayeredWindow(m_hWnd, NULL, &pt, &sz, hDC, &ptSrc, 0, &bfunc, ULW_ALPHA);

		SelectObject(hDC, hOldDIBitmap);
		::DeleteObject(hDIBitmap);
		::DeleteDC(hDC);
	}
	else
	{
		m_pUITreeContainer->Render(dc);
	}
}