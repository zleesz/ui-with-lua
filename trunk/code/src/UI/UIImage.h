#pragma once
#include "uicontrolbase.h"
#include <uiluax.h>
#include <Util.h>

class CUIImage :
	public CUIControlBase
{
private:
	CUIImage(void);
public:
	CUIImage(LPXMLDOMNode pNode);
	virtual ~CUIImage(void);
public:
	virtual ControlType GetType() { return CT_UIImage; };
	void Render(CDCHandle dc);
	static int GetID(lua_State* L);
	static int SetStretch(lua_State* L);
	static int GetImage(lua_State* L);
private:
	void SetAttr(std::string strName, std::string strValue);
private:
	std::string m_strImageID;
	BOOL m_bStretch;
public:
	BEGIN_LUA_CALL_MAP(CUIImage)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY(SetStretch)
		LUA_CALL_ENTRY(GetImage)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIImage, UI.Image);
};
