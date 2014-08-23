#include "StdAfx.h"
#include "UIWindowFactory.h"
#include "UIModalWindow.h"
#include "UIFrameWindow.h"

CUIWindowFactory::CUIWindowFactory(void)
{
	RegisterGlobalObj();
}

CUIWindowFactory::~CUIWindowFactory(void)
{
	UnRegisterGlobalObj();
}

int CUIWindowFactory::GetWindow(lua_State* luaState)
{
	CUIWindowFactory** ppThis = (CUIWindowFactory**) luaL_checkudata(luaState, 1, CUIWindowFactory::GetRigisterClassName());
	CUIWindowFactory* pThis = *ppThis;
	if (!pThis)
	{
		ATLASSERT(FALSE);
		return 0;
	}
	std::string strName = lua_tostring(luaState, 2);
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
	CUIWindowFactory** ppThis = (CUIWindowFactory**) luaL_checkudata(luaState, 1, CUIWindowFactory::GetRigisterClassName());
	CUIWindowFactory* pThis = *ppThis;
	if (!pThis)
	{
		ATLASSERT(FALSE);
		return 0;
	}
	int top = lua_gettop(luaState);
	if (top < 3)
	{
		ATLASSERT(FALSE);
		return 0;
	}
	std::string strWindowTypeID = lua_tostring(luaState, 2);
	std::string strID = lua_tostring(luaState, 3);
	HWND hParent = NULL;
	if (top >= 4)
	{
		if (lua_isnumber(luaState, 4))
		{
			hParent = (HWND)(LONG_PTR)(LONG)lua_tonumber(luaState, 4);
		}
		else if (lua_isuserdata(luaState, 4))
		{
			hParent = (HWND)lua_touserdata(luaState, 4);
		}
	}
	UIWindowMap::iterator itWindow = pThis->m_mapID2Window.find(strID);
	if (itWindow != pThis->m_mapID2Window.end())
	{
		itWindow->second->CreateWnd(::IsWindow(hParent) ? hParent : NULL);
		UILuaPushClassObj(luaState, (void*)itWindow->second);
		return 1;
	}

	ATLASSERT(strWindowTypeID.length() > 0 && strID.length() > 0);
	UIWindowXMLMap::iterator it = pThis->m_mapWindowXML.find(strWindowTypeID);
	if (it == pThis->m_mapWindowXML.end())
	{
		ATLASSERT(FALSE);
		return 0;
	}
	LPXMLDOMNode pNode = it->second.pXMLDOMNode;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
	{
		return 0;
	}
	// add hostwnd
	CUIWindowBase* pUIWindow = NULL;
	if((*pMapAttr)["type"] == "FrameHostWnd")
	{
		pUIWindow = new CUIFrameWindow(it->second.strPath, pNode);
	}
	else if((*pMapAttr)["type"] == "ModalHostWnd")
	{
		pUIWindow = new CUIModalWindow(it->second.strPath, pNode);
	}
	else
	{
		ATLASSERT(FALSE);
		return 0;
	}
	pThis->m_mapID2Window[strID] = pUIWindow;
	pUIWindow->SetID(strID);
	// µ÷ÓÃCreate
	pUIWindow->CreateWnd(::IsWindow(hParent) ? hParent : NULL);
	UILuaPushClassObj(luaState, (void*)pUIWindow);
	return 1;
}

int CUIWindowFactory::DestroyWindow(lua_State* luaState)
{
	CUIWindowFactory** ppThis = (CUIWindowFactory**) luaL_checkudata(luaState, 1, CUIWindowFactory::GetRigisterClassName());
	CUIWindowFactory* pThis = *ppThis;
	if (!pThis)
	{
		ATLASSERT(FALSE);
		return 0;
	}
	int top = lua_gettop(luaState);
	if (top < 2)
	{
		ATLASSERT(FALSE);
		return 0;
	}
	std::string strID;
	if (lua_isuserdata(luaState, 2))
	{
		CUIWindowBase** ppWindow = (CUIWindowBase**)lua_touserdata(luaState, 2);
		CUIWindowBase* pWindow = *ppWindow;
		if (!pWindow)
		{
			ATLASSERT(FALSE);
			return 0;
		}
		strID = pWindow->GetID();
	}
	else if (lua_isstring(luaState, 2))
	{
		strID = lua_tostring(luaState, 2);
	}
	else
	{
		ATLASSERT(FALSE);
		return 0;
	}
	if (strID.length() <= 0)
	{
		return 0;
	}
	UIWindowMap::iterator itWindow = pThis->m_mapID2Window.find(strID);
	if (itWindow == pThis->m_mapID2Window.end())
	{
		ATLASSERT(FALSE);
		return 0;
	}
	itWindow->second->Destroy();
	pThis->m_mapID2Window.erase(itWindow);
	return 0;
}

void CUIWindowFactory::ParserWindowDOM(const std::string& strPath, LPXMLDOMNode pNode)
{
	if(pNode == NULL || pNode->strName != "hostwnd")
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	std::string strID = (*pMapAttr)["id"];
	if (strID.length() <= 0)
	{
		return;
	}
	WindowXMLNode xmlNode;
	xmlNode.pXMLDOMNode = pNode;
	xmlNode.strPath = strPath;
	m_mapWindowXML[strID] = xmlNode;
}
