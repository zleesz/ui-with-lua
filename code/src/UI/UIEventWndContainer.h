#pragma once
#include "UIEventContainerBase.h"
#include <atlapp.h>
#include <atlwin.h>
#include <winuser.h>
#include <atltypes.h>
#include <uilog4cplus.h>

typedef struct tagFilterNode
{
	int nFunc;
	tagFilterNode(int n) : nFunc(n){}
}FilterNode, *LPFilterNode;

class CUIEventWndContainer :
	public CUIEventContainerBase
{
private:
	CUIEventWndContainer(void);
public:
	CUIEventWndContainer(CUIWindowBase* p);
	virtual ~CUIEventWndContainer(void);
public:
	BEGIN_MSG_MAP(CUIEventWndContainer)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_MOVE, OnMove)
		MESSAGE_HANDLER(WM_MOVING, OnMoving)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_ENABLE, OnEnableChange)
		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnInputMessage)
		MESSAGE_RANGE_HANDLER(WM_KEYFIRST,WM_KEYLAST, OnInputMessage)
	END_MSG_MAP()
private:
	std::vector<FilterNode> m_vecFilter;
	CUIWindowBase* m_pBindWnd;
	BOOL m_bTrackLeave;
public:
	virtual EventType GetType() { return ET_Window; };
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMove(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMoving(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEnableChange(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnInputMessage(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	void AddInputFilter(lua_State* L);
public:
	LOG_CLS_DEC();
};
