#pragma once
#include <atlapp.h>
#include <atlwin.h>

class CUIWindowBase;

class CUIWindowResizer
{
private:
	CUIWindowResizer(void) {};
public:
	CUIWindowResizer(CUIWindowBase* p);
	virtual ~CUIWindowResizer(void);
public:
	BEGIN_MSG_MAP(CUIWindowResizer)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		MESSAGE_HANDLER(WM_NCLBUTTONDOWN, OnNcLButtonDown)
	END_MSG_MAP()
private:
	BOOL IsInResizeLeftTopArea(const POINT& pt, const SIZE& sz);
	BOOL IsInResizeLeftBottomArea(const POINT& pt, const SIZE& sz);
	BOOL IsInResizeRightTopArea(const POINT& pt, const SIZE& sz);
	BOOL IsInResizeRightBottomArea(const POINT& pt, const SIZE& sz);
	BOOL IsInResizeLeftArea(const POINT& pt, const SIZE& sz);
	BOOL IsInResizeTopArea(const POINT& pt, const SIZE& sz);
	BOOL IsInResizeRightArea(const POINT& pt, const SIZE& sz);
	BOOL IsInResizeBottomArea(const POINT& pt, const SIZE& sz);
public:
	LRESULT OnNcHitTest(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNcLButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	void SetResizeEnable(BOOL bEnable);
	void SetResizeLeft(LONG nLeft);
	void SetResizeTop(LONG nTop);
	void SetResizeRight(LONG nRight);
	void SetResizeBottom(LONG nBottom);
	void SetResizeLeftTop(LONG nLeftTop);
	void SetResizeLeftBottom(LONG nLeftBottom);
	void SetResizeRightTop(LONG nRightTop);
	void SetResizeRightBottom(LONG nRightBottom);
private:
	CUIWindowBase*	m_pBindWnd;
	BOOL			m_bEnable;
	LONG			m_nLeft;
	LONG			m_nTop;
	LONG			m_nRight;
	LONG			m_nBottom;
	LONG			m_nLeftTop;
	LONG			m_nLeftBottom;
	LONG			m_nRightTop;
	LONG			m_nRightBottom;
};
