#pragma once
#include "uiwindowbase.h"
#include <uiluax.h>
#include <Util.h>
#include "UITreeContainer.h"

class CUIModalWindow :
	public CUIWindowBase
{
private:
	CUIModalWindow(void);
public:
	CUIModalWindow(const std::string& strPath, LPXMLDOMNode pNode);
	virtual ~CUIModalWindow(void);
public:
	virtual WindowType GetType() { return WT_ModalHostWnd; };
	virtual BOOL Render(CDCHandle dc);
	virtual BOOL CreateWnd(HWND hParent);
	void SetAttr(std::string strName, std::string strValue);
	void GetAttr(std::string strName, VARIANT* v);
	virtual BOOL ParserAttr(LPXMLDOMNode pAttrNode);
private:
	virtual DWORD GetStyle();
	virtual DWORD GetStyleEx();
public:
	static int GetID(lua_State* luaState);
public:
	BEGIN_LUA_CALL_MAP(CUIModalWindow)
		LUA_CALL_ENTRY(GetID)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIModalWindow, UI.ModalWindow);
};
