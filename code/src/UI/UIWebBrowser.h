#pragma once
#include "uicontrolbase.h"
#include <uiluax.h>
#include <Util.h>
#include "UITreeContainer.h"

class CUIWebBrowser : 
	public CUIControlBase
{
public:
	CUIWebBrowser(void) {};
public:
	CUIWebBrowser(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	CUIWebBrowser(CUITreeContainer* pTree);
	virtual ~CUIWebBrowser(void);
public:
	UICONTROL_GETTYPE_METHOD(CT_UIWebBrowser);
	virtual void Render(CDCHandle dc) {};
	virtual void OnInitControl() {};
	virtual void OnDetroy() {};

	static int Navigate(lua_State* L);
public:
	BEGIN_LUA_CALL_MAP(CUIWebBrowser)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY(GetVisible)
		LUA_CALL_ENTRY(SetVisible)
		LUA_CALL_ENTRY(GetEnable)
		LUA_CALL_ENTRY(GetOwnerTree)
		LUA_CALL_ENTRY(Navigate)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIWebBrowser, UI.WebBrowser);
public:
	LOG_CLS_DEC();
};
