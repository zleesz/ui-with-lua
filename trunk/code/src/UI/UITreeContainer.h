#pragma once
#include <uilog4cplus.h>
#include <uiluax.h>
#include <Util.h>
#include "UIControlBase.h"
#include <map>
#include <string>
#include "ZorderIndexer.h"

class CUIWindowBase;

class CUITreeContainer
{
private:
	CUITreeContainer(void);
public:
	CUITreeContainer(CUIWindowBase* p);
	virtual ~CUITreeContainer(void);
private:
	ID2ControlMap m_mapCtrl;
	CUIWindowBase* m_pBindWnd;
	CZorderIndexer m_ZorderIndexer;
public:
	BEGIN_MSG_MAP(CUITreeContainer)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	END_MSG_MAP()
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	BOOL ParserUITree(LPXMLDOMNode pNode);
	BOOL OnPreCreate();
	BOOL OnCreate();
	void Render(CDCHandle dc);
public:
	static int GetUIObject(lua_State* L);
	static int GetOwnerWnd(lua_State* L);
public:
	BEGIN_LUA_CALL_MAP(CUITreeContainer)
		LUA_CALL_ENTRY(GetUIObject)
		LUA_CALL_ENTRY(GetOwnerWnd)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUITreeContainer, UI.TreeContainer);
public:
	LOG_CLS_DEC();
};
