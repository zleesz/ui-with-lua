#pragma once
#include "uicontrolbase.h"
#include "UIFont.h"
#include "UIColor.h"

class CUIText :
	public CUIControlBase
{
public:
	CUIText(void) {};
public:
	CUIText(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	CUIText(CUITreeContainer* pTree);
	virtual ~CUIText(void);
public:
	UICONTROL_GETTYPE_METHOD(CT_UIText);
	virtual void Render(CDCHandle dc);
	virtual void OnInitControl();
	virtual void OnDetroy();
	UICONTROL_DEFAULT_MOUSE_METHOD(OnLButtonDbClick);
	UICONTROL_DEFAULT_MOUSE_METHOD(OnMouseWheel);
	UICONTROL_DEFAULT_MOUSE_METHOD(OnMouseMove);
	UICONTROL_DEFAULT_MOUSE_METHOD(OnMouseLeave);
	UICONTROL_DEFAULT_MOUSE_METHOD(OnLButtonDown);
	UICONTROL_DEFAULT_MOUSE_METHOD(OnLButtonUp);
private:
	void SetAttr(const std::string& strName, const std::string& strValue);
private:
	std::string m_strText;
	std::string m_strFontID;
	std::string m_strColorID;
public:
	std::string SetText(const std::string& strText);
	void GetText(std::string& strText);

	static int SetText(lua_State* L);
	static int GetText(lua_State* L);
public:
	BEGIN_LUA_CALL_MAP(CUIText)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY(GetVisible)
		LUA_CALL_ENTRY(SetVisible)
		LUA_CALL_ENTRY(GetEnable)
		LUA_CALL_ENTRY(GetOwnerTree)
		LUA_CALL_ENTRY(SetText)
		LUA_CALL_ENTRY(GetText)
		LUA_CALL_ENTRY(SetObjPos)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIText, UI.Text);
public:
	LOG_CLS_DEC();
};
