#include "StdAfx.h"
#include "UIWindowResizer.h"
#include "UIWindowBase.h"

CUIWindowResizer::CUIWindowResizer(CUIWindowBase* p)
	: m_pBindWnd(p),
	m_bEnable(FALSE),
	m_nLeft(0),
	m_nTop(0),
	m_nRight(0),
	m_nBottom(0),
	m_nLeftTop(0),
	m_nLeftBottom(0),
	m_nRightTop(0),
	m_nRightBottom(0)
{
}

CUIWindowResizer::~CUIWindowResizer(void)
{
}

BOOL CUIWindowResizer::IsInResizeLeftTopArea(const POINT& pt, const SIZE& /*sz*/)
{
	if (pt.x <= m_nLeftTop && pt.y <= m_nLeftTop)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowResizer::IsInResizeLeftBottomArea(const POINT& pt, const SIZE& sz)
{
	if (pt.x <= m_nLeftBottom && sz.cy - pt.y <= m_nLeftBottom)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowResizer::IsInResizeRightTopArea(const POINT& pt, const SIZE& sz)
{
	if (pt.y <= m_nRightTop && sz.cx - pt.x <= m_nRightTop)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowResizer::IsInResizeRightBottomArea(const POINT& pt, const SIZE& sz)
{
	if (sz.cx - pt.x <= m_nRightBottom && sz.cy - pt.y <= m_nRightBottom)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowResizer::IsInResizeLeftArea(const POINT& pt, const SIZE& /*sz*/)
{
	if (pt.x <= m_nLeft)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowResizer::IsInResizeTopArea(const POINT& pt, const SIZE& /*sz*/)
{
	if (pt.y <= m_nTop)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowResizer::IsInResizeRightArea(const POINT& pt, const SIZE& sz)
{
	if (sz.cx - pt.x <= m_nRight)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIWindowResizer::IsInResizeBottomArea(const POINT& pt, const SIZE& sz)
{
	if (sz.cy - pt.y <= m_nBottom)
	{
		return TRUE;
	}
	return FALSE;
}

LRESULT CUIWindowResizer::OnNcHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (!m_bEnable)
	{
		return 0;
	}
	POINT pt = {0};
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(m_pBindWnd->m_hWnd, &pt);

	RECT rc = {0};
	::GetWindowRect(m_pBindWnd->m_hWnd, &rc);
	SIZE sz = {rc.right - rc.left, rc.bottom - rc.top};
	if (IsInResizeLeftTopArea(pt, sz))
	{
		bHandled = TRUE;
		return HTTOPLEFT;
	}
	else if (IsInResizeLeftBottomArea(pt, sz))
	{
		bHandled = TRUE;
		return HTBOTTOMLEFT;
	}
	else if (IsInResizeRightTopArea(pt, sz))
	{
		bHandled = TRUE;
		return HTTOPRIGHT;
	}
	else if (IsInResizeRightBottomArea(pt, sz))
	{
		bHandled = TRUE;
		return HTBOTTOMRIGHT;
	}
	else if (IsInResizeLeftArea(pt, sz))
	{
		bHandled = TRUE;
		return HTLEFT;
	}
	else if (IsInResizeTopArea(pt, sz))
	{
		bHandled = TRUE;
		return HTTOP;
	}
	else if (IsInResizeRightArea(pt, sz))
	{
		bHandled = TRUE;
		return HTRIGHT;
	}
	else if (IsInResizeBottomArea(pt, sz))
	{
		bHandled = TRUE;
		return HTBOTTOM;
	}

	return 0;
}

LRESULT CUIWindowResizer::OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (!m_bEnable)
	{
		return 0;
	}
	int nHitTest = LOWORD(lParam);
	switch (nHitTest)
	{
	case HTLEFT:
	case HTRIGHT:
		bHandled = TRUE;
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
		break;
	case HTTOP:
	case HTBOTTOM:
		bHandled = TRUE;
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
		break;
	case HTTOPLEFT:
	case HTBOTTOMRIGHT:
		bHandled = TRUE;
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
		break;
	case HTBOTTOMLEFT:
	case HTTOPRIGHT:
		bHandled = TRUE;
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
		break;
	default:
		break;
	}
	return 0;
}

LRESULT CUIWindowResizer::OnNcLButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (!m_bEnable)
	{
		return 0;
	}
	int nHitTest = LOWORD(wParam);
	switch (nHitTest)
	{
	case HTLEFT:
		bHandled = TRUE;
		::SendMessage(m_pBindWnd->m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, lParam);
		break;
	case HTTOP:
		bHandled = TRUE;
		::SendMessage(m_pBindWnd->m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, lParam);
		break;
	case HTRIGHT:
		bHandled = TRUE;
		::SendMessage(m_pBindWnd->m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, lParam);
		break;
	case HTBOTTOM:
		bHandled = TRUE;
		::SendMessage(m_pBindWnd->m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, lParam);
		break;
	case HTTOPLEFT:
		bHandled = TRUE;
		::SendMessage(m_pBindWnd->m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, lParam);
		break;
	case HTBOTTOMLEFT:
		bHandled = TRUE;
		::SendMessage(m_pBindWnd->m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, lParam);
		break;
	case HTTOPRIGHT:
		bHandled = TRUE;
		::SendMessage(m_pBindWnd->m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, lParam);
		break;
	case HTBOTTOMRIGHT:
		bHandled = TRUE;
		::SendMessage(m_pBindWnd->m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, lParam);
		break;
	default:
		break;
	}
	return 0;
}

void CUIWindowResizer::SetResizeEnable(BOOL bEnable)
{
	m_bEnable = bEnable;
}

void CUIWindowResizer::SetResizeLeft(LONG nLeft)
{
	m_nLeft = nLeft;
}

void CUIWindowResizer::SetResizeTop(LONG nTop)
{
	m_nTop = nTop;
}

void CUIWindowResizer::SetResizeRight(LONG nRight)
{
	m_nRight = nRight;
}

void CUIWindowResizer::SetResizeBottom(LONG nBottom)
{
	m_nBottom = nBottom;
}

void CUIWindowResizer::SetResizeLeftTop(LONG nLeftTop)
{
	m_nLeftTop = nLeftTop;
}

void CUIWindowResizer::SetResizeLeftBottom(LONG nLeftBottom)
{
	m_nLeftBottom = nLeftBottom;
}

void CUIWindowResizer::SetResizeRightTop(LONG nRightTop)
{
	m_nRightTop = nRightTop;
}

void CUIWindowResizer::SetResizeRightBottom(LONG nRightBottom)
{
	m_nRightBottom = nRightBottom;
}
