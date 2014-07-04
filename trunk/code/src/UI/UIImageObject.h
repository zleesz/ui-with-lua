#pragma once
#include "uicontrolbase.h"
#include <uiluax.h>
#include <Util.h>
#include "UITreeContainer.h"

class CUIImageObject :
	public CUIControlBase
{
public:
	CUIImageObject(void);
	CUIImageObject(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	CUIImageObject(CUITreeContainer* pTree);
	virtual ~CUIImageObject(void);
public:
	UICONTROL_GETTYPE_METHOD(CT_UIImage)
	void Render(CDCHandle dc);
	virtual void OnInitControl();
	virtual void OnDetroy();
	virtual void OnRButtonUp(int x, int y);
	virtual void OnLButtonDown(int x, int y);
	virtual void SetAttr(const std::string& strName, const std::string& strValue);
public:
	static int GetStretch(lua_State* L);
	static int SetStretch(lua_State* L);
	static int GetImage(lua_State* L);
	static int GetVisible(lua_State* L);
	static int SetVisible(lua_State* L);
	static int GetEnable(lua_State* L);
private:
	std::string m_strImageID;
	BOOL m_bStretch;
public:
	BEGIN_LUA_CALL_MAP(CUIImageObject)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY(SetStretch)
		LUA_CALL_ENTRY(GetStretch)
		LUA_CALL_ENTRY(GetImage)
		LUA_CALL_ENTRY(GetVisible)
		LUA_CALL_ENTRY(SetVisible)
		LUA_CALL_ENTRY(GetEnable)
		LUA_CALL_ENTRY(GetOwnerTree)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIImageObject, UI.ImageObject);
public:
	LOG_CLS_DEC();
};
