#pragma once
#include "UIWindowBase.h"
#include "UITreeContainer.h"
#include "UILuaManager.h"
#include <Util.h>

class CUIFrameWindow : 
	public CUIWindowBase,
	public CWindowImpl<CUIFrameWindow>
{
private:
	CUIFrameWindow(void);
public:
	CUIFrameWindow(const std::string& strPath, LPXMLDOMNode pNode);
	virtual ~CUIFrameWindow(void);
public:
	BEGIN_MSG_MAP(CUIFrameWindow)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		CHAIN_MSG_MAP_MEMBER((*m_pUIEventWindow))
	END_MSG_MAP()
public:
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	virtual WindowType GetType() { return WT_FrameHostWnd; };
	virtual BOOL Render(CDCHandle dc);
	virtual BOOL CreateWnd(HWND hParent);
	void SetAttr(std::string strName, std::string strValue);
	void GetAttr(std::string strName, VARIANT* v);
	virtual BOOL ParserAttr(LPXMLDOMNode pAttrNode);
private:
	virtual DWORD GetStyle();
	virtual DWORD GetStyleEx();
	void DoPaint(CDCHandle dc);
public:
	static int GetID(lua_State* luaState);
	static int GetTreeContainer(lua_State* luaState);
public:
	BEGIN_LUA_CALL_MAP(CUIFrameWindow)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY(GetTreeContainer)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIFrameWindow, UI.FrameWindow);
public:
	LOG_CLS_DEC();
};
