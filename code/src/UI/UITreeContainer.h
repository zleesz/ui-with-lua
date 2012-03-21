#pragma once
#include <uilog4cplus.h>
#include <uiluax.h>
#include <Util.h>
#include "UIControlBase.h"
#include <map>
#include <string>

class CUIWindowBase;
class CUITreeContainer
{
private:
	CUITreeContainer(void);
public:
	CUITreeContainer(CUIWindowBase* p);
	virtual ~CUITreeContainer(void);
private:
	typedef std::map<std::string, CUIControlBase*> ID2ControlMap;
	ID2ControlMap m_mapCtrl;
	CUIWindowBase* m_pBindWnd;
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
