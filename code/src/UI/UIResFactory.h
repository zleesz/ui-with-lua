#pragma once
#include <uiluax.h>
#include <Util.h>
#include "UIResBase.h"
#include "UIColor.h"
#include "UIFont.h"
#include "UIBitmap.h"
#include "UIIcon.h"
#include "UIImageList.h"

class CUIResFactory
{
private:
	CUIResFactory(void);
	virtual ~CUIResFactory(void);
public:
	void AddResElement(LPXMLDOMNode pNode, const char* pszPath);
	void AddRes(CUIResBase* pRes);
	CUIColor* GetColor(const char* id);
	CUIFont* GetFont(const char* id);
	CUIBitmap* GetBitmap(const char* id);
	CUIIcon* GetIcon(const char* id);
	CUIImagelist* GetImagelist(const char* id);

private:
	typedef std::map<const std::string, CUIResBase*> UIID2ResMap, *LPUIID2ResMap;
	typedef std::map<ResourceType, UIID2ResMap*> PT2MapResMap, *LPPT2MapResMap;
	PT2MapResMap m_mapRes;
private:
	template <class T, ResourceType pt>
	T* GetResObject(const char* id);
	template <class T, ResourceType pt>
	int PushResObject(lua_State* L);
public:
	static int GetColor(lua_State* luaState);
	static int GetFont(lua_State* luaState);
	static int GetBitmap(lua_State* luaState);
	static int GetIcon(lua_State* luaState);
	static int GetImagelist(lua_State* luaState);
public:
	BEGIN_LUA_CALL_MAP(CUIResFactory)
		LUA_CALL_ENTRY(GetColor)
		LUA_CALL_ENTRY(GetFont)
		LUA_CALL_ENTRY(GetBitmap)
		LUA_CALL_ENTRY(GetIcon)
		LUA_CALL_ENTRY(GetImagelist)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_SINGLETON_OBJECT(CUIResFactory, UI.ResFactory);
public:
	LOG_CLS_DEC();
};

#define UIResFactoryInstance CUIResFactory::GetInstance()