#pragma once
#include "uicontrolbase.h"
#include <uiluax.h>
#include <Util.h>
#include "UITreeContainer.h"

class CUIButton :
	public CUIControlBase
{
public:
	CUIButton(void);
public:
	CUIButton(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	virtual ~CUIButton(void);
public:
	virtual ControlType GetType() { return CT_UIButton; };
	void Render(CDCHandle dc);
	virtual void OnInitControl();
	virtual BOOL OnHitTest(int x, int y);
	static int GetID(lua_State* L);
private:
	void SetAttr(std::string strName, std::string strValue);
public:
	BEGIN_LUA_CALL_MAP(CUIButton)
		LUA_CALL_ENTRY(GetID)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIButton, UI.Button);
};
