#include "StdAfx.h"
#include "UIWindowFactory.h"
#include "UIModalWindow.h"
#include "UIFrameWindow.h"

CUIWindowFactory::CUIWindowFactory(void)
{
}

CUIWindowFactory::~CUIWindowFactory(void)
{
}

int CUIWindowFactory::GetWindow(lua_State* luaState)
{
	CUIWindowFactory* pThis = (CUIWindowFactory*) lua_touserdata(luaState, -1);
	std::string strName = lua_tostring(luaState, -2);
	UIWindowMap::const_iterator it = pThis->m_mapID2Window.find(strName);
	if(it == pThis->m_mapID2Window.end())
	{
		return 0;
	}
	UILuaPushClassObj(luaState, (void*)it->second);
	return 1;
}
int CUIWindowFactory::Create(lua_State* luaState)
{
	CUIWindowFactory* pThis = (CUIWindowFactory*) lua_touserdata(luaState, -1);
	ATLASSERT(pThis);
	std::string strID = lua_tostring(luaState, -2);
	ATLASSERT(strID.length() > 0);
	HWND hParent = NULL;
	int top = lua_gettop(luaState);
	if(top >= 3)
	{
		hParent = (HWND)(LONG_PTR)(LONG)lua_tonumber(luaState, -3);
	}

	CUIWindowBase* pUIWindow = pThis->m_mapID2Window[strID];
	ATLASSERT(pUIWindow);
	if(pUIWindow)
	{
		// µ÷ÓÃCreate
		pUIWindow->CreateWnd(hParent);
	}
	UILuaPushClassObj(luaState, (void*)pUIWindow);
	return 1;
}

void CUIWindowFactory::ParserWindowDOM(const std::string& strPath, LPXMLDOMNode pNode)
{
	if(pNode == NULL || pNode->strName != "hostwnd")
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	// add hostwnd
	CUIWindowBase* pUIWindow = NULL;
	if((*pMapAttr)["type"] == "FrameHostWnd")
	{
		pUIWindow = new CUIFrameWindow(strPath, pNode);
	}
	else if((*pMapAttr)["type"] == "ModalHostWnd")
	{
		pUIWindow = new CUIModalWindow(strPath, pNode);
	}
	std::string strID = pUIWindow->GetID();
	if(strID.length() > 0)
	{
		m_mapID2Window[strID] = pUIWindow;
	}
	else
	{
		delete pUIWindow;
		return;
	}
}
