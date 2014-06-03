#pragma once
#include "uicontrolbase.h"
#include <uiluax.h>
#include <Util.h>
#include "UITreeContainer.h"

class CUITextureObject :
	public CUIControlBase
{
public:
	CUITextureObject(void);
	CUITextureObject(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	CUITextureObject(CUITreeContainer* pTree);
	virtual ~CUITextureObject(void);
public:
	UICONTROL_GETTYPE_METHOD(CT_UITexture)
	void Render(CDCHandle dc);
	virtual void OnInitControl();
	virtual void OnDetroy();
	UICONTROL_DEFAULT_MOUSE_METHOD(OnLButtonUp);
	UICONTROL_DEFAULT_MOUSE_METHOD(OnLButtonDbClick);
	UICONTROL_DEFAULT_MOUSE_METHOD(OnMouseMove);
	UICONTROL_DEFAULT_MOUSE_METHOD(OnMouseLeave);
	UICONTROL_DEFAULT_MOUSE_METHOD(OnMouseWheel);
	virtual void OnRButtonUp(int x, int y);
	virtual void OnLButtonDown(int x, int y);
public:
	static int GetID(lua_State* L);
	static int GetStretch(lua_State* L);
	static int SetStretch(lua_State* L);
	static int GetTexture(lua_State* L);
	static int GetVisible(lua_State* L);
	static int SetVisible(lua_State* L);
	static int GetEnable(lua_State* L);
	static int GetOwnerTree(lua_State* L);
private:
	void SetAttr(std::string strName, std::string strValue);
private:
	std::string m_strTextureID;
	BOOL m_bStretch;
public:
	BEGIN_LUA_CALL_MAP(CUITextureObject)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY(SetStretch)
		LUA_CALL_ENTRY(GetStretch)
		LUA_CALL_ENTRY(GetTexture)
		LUA_CALL_ENTRY(GetVisible)
		LUA_CALL_ENTRY(SetVisible)
		LUA_CALL_ENTRY(GetEnable)
		LUA_CALL_ENTRY(GetOwnerTree)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUITextureObject, UI.TextureObject);
public:
	LOG_CLS_DEC();
};
