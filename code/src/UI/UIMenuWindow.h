#pragma once
#include "UIWindowBase.h"
#include "UITreeContainer.h"
#include "UILuaManager.h"
#include <Util.h>
#include <UIParser.h>

class CUIMenuWindow : 
	public CUIWindowBase
{
private:
	CUIMenuWindow(void) {};
public:
	CUIMenuWindow(const std::string& strPath, LPXMLDOMNode pNode);
	virtual ~CUIMenuWindow(void);
public:
	DECLARE_WND_CLASS(_T("UIMenuWindow"))
	BEGIN_MSG_MAP(CUIMenuWindow)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_NCPAINT, OnNcPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		//MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		//MESSAGE_HANDLER(WM_NCCALCSIZE, OnNcCalcSize)
		//MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
		CHAIN_MSG_MAP_MEMBER((*m_pUIEventWindow))
		CHAIN_MSG_MAP_MEMBER((*m_pUIWindowResizer))
		CHAIN_MSG_MAP_MEMBER((*m_pUITreeContainer))
	END_MSG_MAP()
public:
	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) { return 1;}
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNcPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	virtual WindowType GetType() { return WT_FrameHostWnd; };
	virtual BOOL Render(CDCHandle dc);
	virtual BOOL CreateWnd(HWND hParent);
	void SetAttr(const std::string& strName, const std::string& strValue);
	void GetAttr(std::string strName, VARIANT* v);
	virtual BOOL ParserAttr(LPXMLDOMNode pAttrNode);
	void TryUpdateWindow();	
private:
	virtual DWORD GetStyle();
	virtual DWORD GetStyleEx();
	void DoPaint(CDCHandle dc);
public:
	BEGIN_LUA_CALL_MAP(CUIMenuWindow)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY(GetTitle)
		LUA_CALL_ENTRY(SetTitle)
		LUA_CALL_ENTRY(GetVisible)
		LUA_CALL_ENTRY(SetVisible)
		LUA_CALL_ENTRY(Show)
		LUA_CALL_ENTRY(GetTreeContainer)
		LUA_CALL_ENTRY(AddInputFilter)
		LUA_CALL_ENTRY(AttachListener)
		LUA_CALL_ENTRY(DetachListener)
		LUA_CALL_ENTRY(Min)
		LUA_CALL_ENTRY(Max)
		LUA_CALL_ENTRY(Restore)
		LUA_CALL_ENTRY(GetLayered)
		LUA_CALL_ENTRY(SetMaxTrackSize)
		LUA_CALL_ENTRY(SetMinTrackSize)
		LUA_CALL_ENTRY(SetParent)
		LUA_CALL_ENTRY(GetParent)
		LUA_CALL_ENTRY(GetHWND)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIMenuWindow, UI.MenuWindow);
public:
	LOG_CLS_DEC();
};
