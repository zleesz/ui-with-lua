#include "StdAfx.h"
#include "UIResFactory.h"

CUIResFactory::CUIResFactory(void)
{
}

CUIResFactory::~CUIResFactory(void)
{
	PT2MapResMap::const_iterator itMap = m_mapRes.begin();
	for(; itMap != m_mapRes.end(); itMap++)
	{
		UIID2ResMap *map = itMap->second;
		delete map;
	}
}

void CUIResFactory::AddResElement(LPXMLDOMNode pNode, const char* pszPath)
{
	// 创建对应的UIRes;
	if(pNode == NULL)
		return;
	CUIResBase* pUIBase = NULL;
	if(pNode->strName == "color")
	{
		pUIBase = new CUIColor(pNode);
	}
	else if(pNode->strName == "bitmap")
	{
		pUIBase = new CUIBitmap(pNode, pszPath);
	}
	else if(pNode->strName == "font")
	{
		pUIBase = new CUIFont(pNode);
	}
	else if(pNode->strName == "icon")
	{
		pUIBase = new CUIIcon(pNode, pszPath);
	}
	if(NULL != pUIBase)
	{
		ResourceType pt = pUIBase->GetType();
		const char* pszID = pUIBase->GetID();
		LOG_DEBUG("id=" << pszID << ", type=" << pt);
		LPUIID2ResMap pUIID2ResMap = NULL;
		PT2MapResMap::const_iterator itMap = m_mapRes.find(pt);
		if(itMap == m_mapRes.end())
		{
			pUIID2ResMap = new UIID2ResMap;
			m_mapRes.insert(PT2MapResMap::value_type(pt, pUIID2ResMap));
		}
		else
		{
			pUIID2ResMap = itMap->second;
		}
		UIID2ResMap::const_iterator it = pUIID2ResMap->find(pszID);
		if(it == pUIID2ResMap->end())
		{
			pUIID2ResMap->insert(UIID2ResMap::value_type(pszID, pUIBase));
		}
		else
		{
			ATLASSERT(FALSE);
			delete pUIBase;
			pUIBase = NULL;
		}
	}
}

template <class T, ResourceType pt>
T* CUIResFactory::GetResObject(const char* id)
{
	T* pUIRes = NULL;
	PT2MapResMap::const_iterator itMap = m_mapRes.find(pt);
	if(itMap != m_mapRes.end())
	{
		UIID2ResMap* mapColor = itMap->second;
		UIID2ResMap::const_iterator itColorMap = mapColor->find(id);
		if(itColorMap != mapColor->end())
		{
			pUIRes = (T*)itColorMap->second;
		}
	}
	return pUIRes;
}

CUIColor* CUIResFactory::GetColor(const char* id)
{
	return GetResObject<CUIColor, RT_UICOLOR>(id);
}

CUIFont* CUIResFactory::GetFont(const char* id)
{
	return GetResObject<CUIFont, RT_UIFONT>(id);
}

CUIBitmap* CUIResFactory::GetBitmap(const char* id)
{
	return GetResObject<CUIBitmap, RT_UIBITMAP>(id);
}

CUIIcon* CUIResFactory::GetIcon(const char* id)
{
	return GetResObject<CUIIcon, RT_UIICON>(id);
}

template <class T, ResourceType pt>
int CUIResFactory::PushResObject(lua_State* L)
{
	const char* pszID = lua_tostring(L, -2);
	T* pObject = GetResObject<T, pt>(pszID);
	ATLASSERT(pObject);
	if(pObject)
	{
		UILuaPushClassObj(L, (void*)pObject);
		return 1;
	}
	return 0;
}

int CUIResFactory::GetColor(lua_State* L)
{
	int top = lua_gettop(L);
	if(top < 2)
	{
		ATLASSERT(FALSE);
		return 0;
	}
	CUIResFactory* pThis = (CUIResFactory*)lua_touserdata(L, -1);
	ATLASSERT(pThis);
	return pThis->PushResObject<CUIColor, RT_UICOLOR>(L);
}

int CUIResFactory::GetFont(lua_State* L)
{
	int top = lua_gettop(L);
	if(top < 2)
	{
		ATLASSERT(FALSE);
		return 0;
	}
	CUIResFactory* pThis = (CUIResFactory*)lua_touserdata(L, -1);
	ATLASSERT(pThis);
	return pThis->PushResObject<CUIFont, RT_UIFONT>(L);
}

int CUIResFactory::GetBitmap(lua_State* L)
{
	int top = lua_gettop(L);
	if(top < 2)
	{
		ATLASSERT(FALSE);
		return 0;
	}
	CUIResFactory* pThis = (CUIResFactory*)lua_touserdata(L, -1);
	ATLASSERT(pThis);
	return pThis->PushResObject<CUIBitmap, RT_UIBITMAP>(L);
}

int CUIResFactory::GetIcon(lua_State* L)
{
	int top = lua_gettop(L);
	if(top < 2)
	{
		ATLASSERT(FALSE);
		return 0;
	}
	CUIResFactory* pThis = (CUIResFactory*)lua_touserdata(L, -1);
	ATLASSERT(pThis);
	return pThis->PushResObject<CUIIcon, RT_UIICON>(L);
}