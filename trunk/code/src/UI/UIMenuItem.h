#pragma once
#include "UIControlBase.h"
#include <uiluax.h>
#include <Util.h>
#include "UITreeContainer.h"

class CUIMenuItem : 
	public CUIControlBase
{
public:
	CUIMenuItem(void);
	CUIMenuItem(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	CUIMenuItem(CUITreeContainer* pTree);
	virtual ~CUIMenuItem(void);
public:
	UICONTROL_GETTYPE_METHOD(CT_UIMenuItem)
	void Render(CDCHandle dc);
	virtual void OnInitControl();
	virtual void OnDetroy();
	virtual void SetAttr(const std::string& strName, const std::string& strValue);
public:
	static int GetOwnerTree(lua_State* L);
public:
private:
	std::string m_strImageID;
	BOOL m_bStretch;
public:
	BEGIN_LUA_CALL_MAP(CUIMenuItem)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY(GetVisible)
		LUA_CALL_ENTRY(SetVisible)
		LUA_CALL_ENTRY(GetEnable)
		LUA_CALL_ENTRY(GetOwnerTree)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIMenuItem, UI.MenuItem);
public:
	LOG_CLS_DEC();
};
