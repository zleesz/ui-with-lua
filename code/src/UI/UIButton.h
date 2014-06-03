#pragma once
#include "uicontrolbase.h"
#include <uiluax.h>
#include <Util.h>
#include "UITreeContainer.h"
typedef enum enumButtonState
{
	BS_NORMAL = 1,
	BS_HOVER,
	BS_DOWN,
	BS_DISABLE,
}ButtonState;

typedef enum enumButtonResType
{
	BRT_BITMAP = 1,
	BRT_IMAGELIST,
}ButtonResType;

class CUIButton;
class CUIButtonBase
{
public:
	virtual void Init(const LPXMLChildNodes pXMLChildNodes) = 0;
	virtual void Render(CDCHandle dc, const RECT& rc, ButtonState brt) = 0;
	virtual ButtonResType GetType() = 0;
};

class CUIButtonBitmap
	: public CUIButtonBase
{
private:
	std::string m_strNormalBkg;
	std::string m_strHoverBkg;
	std::string m_strDownBkg;
	std::string m_strDisableBkg;
public:
	virtual void Init(const LPXMLChildNodes pXMLChildNodes);
	virtual void Render(CDCHandle dc, const RECT& rc, ButtonState brt);
	virtual ButtonResType GetType() { return BRT_BITMAP; };
};

class CUIButtonImagelist
	: public CUIButtonBase
{
private:
	std::string m_strImagelist;
public:
	virtual void Init(const LPXMLChildNodes pXMLChildNodes);
	virtual void Render(CDCHandle dc, const RECT& rc, ButtonState brt);
	virtual ButtonResType GetType() { return BRT_IMAGELIST; };
};

class CUIButton :
	public CUIControlBase
{
public:
	CUIButton(void) {};
public:
	CUIButton(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	CUIButton(CUITreeContainer* pTree);
	virtual ~CUIButton(void);
public:
	static int GetID(lua_State* L);
	static int GetVisible(lua_State* L);
	static int GetEnable(lua_State* L);
	static int GetOwnerTree(lua_State* L);
public:
	UICONTROL_GETTYPE_METHOD(CT_UIButton)
	virtual void Render(CDCHandle dc);
	virtual void OnInitControl();
	virtual void OnDetroy();
	UICONTROL_DEFAULT_MOUSE_METHOD(OnLButtonDbClick);
	UICONTROL_DEFAULT_MOUSE_METHOD(OnMouseWheel);
	virtual void OnMouseMove(int x, int y);
	virtual void OnMouseLeave(int x, int y);
	virtual void OnLButtonDown(int x, int y);
	virtual void OnLButtonUp(int x, int y);
private:
	void SetAttr(std::string strName, std::string strValue);
	void FireOnClick(int x, int y);
	ButtonState m_state;
	CUIButtonBase* m_pButton;
public:
	BEGIN_LUA_CALL_MAP(CUIButton)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY(GetVisible)
		LUA_CALL_ENTRY(GetEnable)
		LUA_CALL_ENTRY(GetOwnerTree)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIButton, UI.Button);
public:
	LOG_CLS_DEC();
};
