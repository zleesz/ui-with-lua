#pragma once
#include <uiluax.h>
#include <Util.h>
#include "UIWindowBase.h"

class CUIWindowFactory
{
private:
	CUIWindowFactory(void);
	virtual ~CUIWindowFactory(void);
private:
	typedef std::map<std::string, CUIWindowBase*> UIWindowMap;
	UIWindowMap m_mapID2Window;
public:
	void ParserWindowDOM(const std::string& strPath, LPXMLDOMNode pNode);
public:
	static int GetWindow(lua_State* luaState);
	static int Create(lua_State* luaState);
public:
	
	BEGIN_LUA_CALL_MAP(CUIWindowFactory)
		LUA_CALL_ENTRY(GetWindow)
		LUA_CALL_ENTRY(Create)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_SINGLETON_OBJECT(CUIWindowFactory, UI.WindowFactory);
};
