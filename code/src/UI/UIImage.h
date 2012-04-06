#pragma once
#include "uicontrolbase.h"
#include <uiluax.h>
#include <Util.h>
#include "UITreeContainer.h"

class CUIImage :
	public CUIControlBase
{
public:
	CUIImage(void);
	CUIImage(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	virtual ~CUIImage(void);
public:
	UICONTROL_GETTYPE_METHOD(CT_UIImage)
	void Render(CDCHandle dc);
	virtual void OnInitControl();
	virtual void OnDetroy();
	UICONTROL_DEFAULT_MOUSE_METHOD(OnLButtonDown);
	UICONTROL_DEFAULT_MOUSE_METHOD(OnLButtonUp);
	UICONTROL_DEFAULT_MOUSE_METHOD(OnLButtonDbClick);
	UICONTROL_DEFAULT_MOUSE_METHOD(OnMouseMove);
	UICONTROL_DEFAULT_MOUSE_METHOD(OnMouseLeave);
	UICONTROL_DEFAULT_MOUSE_METHOD(OnMouseWheel);
public:
	static int GetID(lua_State* L);
	static int GetStretch(lua_State* L);
	static int SetStretch(lua_State* L);
	static int GetImage(lua_State* L);
	static int GetVisible(lua_State* L);
	static int SetVisible(lua_State* L);
	static int GetEnable(lua_State* L);
	static int GetOwnerTree(lua_State* L);
private:
	void SetAttr(std::string strName, std::string strValue);
private:
	std::string m_strImageID;
	BOOL m_bStretch;
public:
	BEGIN_LUA_CALL_MAP(CUIImage)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY(SetStretch)
		LUA_CALL_ENTRY(GetStretch)
		LUA_CALL_ENTRY(GetImage)
		LUA_CALL_ENTRY(GetVisible)
		LUA_CALL_ENTRY(SetVisible)
		LUA_CALL_ENTRY(GetEnable)
		LUA_CALL_ENTRY(GetOwnerTree)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIImage, UI.Image);
};
