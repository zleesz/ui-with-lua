#pragma once
#include "UIWindowBase.h"
#include "UITreeContainer.h"
#include "UILuaManager.h"
#include <Util.h>
#include <UIParser.h>

#define ADD_MSG_MAP_MEMBER(theChainClass) \
	{ \
		BOOL bHandled = theChainClass.ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult); \
		if (bHandled) \
			return TRUE; \
	}

class CUIFrameWindow : 
	public CUIWindowBase
{
private:
	CUIFrameWindow(void);
public:
	CUIFrameWindow(const std::string& strPath, LPXMLDOMNode pNode);
	virtual ~CUIFrameWindow(void);
public:
	BEGIN_MSG_MAP(CUIFrameWindow)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		ADD_MSG_MAP_MEMBER((*m_pUIEventWindow))
		MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		MESSAGE_HANDLER(WM_NCLBUTTONDOWN, OnNcLButtonDown)
		CHAIN_MSG_MAP_MEMBER((*m_pUITreeContainer))
	END_MSG_MAP()
public:
	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) { return 1;}
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	virtual WindowType GetType() { return WT_FrameHostWnd; };
	virtual BOOL Render(CDCHandle dc);
	virtual BOOL CreateWnd(HWND hParent);
	void SetAttr(const std::string& strName, const std::string& strValue);
	void GetAttr(std::string strName, VARIANT* v);
	virtual BOOL ParserAttr(LPXMLDOMNode pAttrNode);
	void TryUpdateLayeredWindow();	
private:
	virtual DWORD GetStyle();
	virtual DWORD GetStyleEx();
	void DoPaint(CDCHandle dc);
public:
	static int GetID(lua_State* luaState);
	static int GetTitle(lua_State* luaState);
	static int SetTitle(lua_State* luaState);
	static int GetVisible(lua_State* luaState);
	static int SetVisible(lua_State* luaState);
	static int Show(lua_State* luaState);
	static int GetTreeContainer(lua_State* luaState);
public:
	BEGIN_LUA_CALL_MAP(CUIFrameWindow)
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
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIFrameWindow, UI.FrameWindow);
public:
	LOG_CLS_DEC();
};
