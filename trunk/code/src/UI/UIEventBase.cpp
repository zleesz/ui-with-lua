#include "stdafx.h"
#include "UIEventBase.h"
#include "UIWindowBase.h"

CUIEventBase::CUIEventBase()
{
	
}

CUIEventBase::~CUIEventBase()
{
	// delete m_mapEvent
	LuaEventMap::iterator it = m_mapEvent.begin();
	for(; it != m_mapEvent.end(); it++)
	{
		LPVecEvent pVecEvent = it->second;
		VecEvent::iterator it2 = pVecEvent->begin();
		for(; it2 != pVecEvent->end(); it2++)
		{
			delete *it2;
		}
		pVecEvent->clear();
		delete pVecEvent;
	}
	m_mapEvent.clear();
}

void CUIEventBase::ParserOneEvent(LPXMLDOMNode pNode)
{
	if(pNode == NULL || pNode->strName != "event")
		return;
	LPXMLAttrMap pAttrMap = pNode->pMapAttr;
	if(pAttrMap == NULL)
		return;
	AttachListener(pAttrMap);
}

BOOL CUIEventBase::ParserEvent(LPXMLDOMNode pNode)
{
	if (NULL == pNode || pNode->strName != "eventlist")
		return FALSE;
	LPXMLChildNodes pChildNode = pNode->pMapChildNode;
	if(pChildNode == NULL)
		return FALSE;
	XMLChildNodes::const_iterator it = pChildNode->find("event");
	if(it == pChildNode->end())
	{
		return FALSE;
	}
	LPXMLVecNodes pVecNodes = it->second->pVecNode;
	for(XMLVecNodes::size_type st = 0; st != pVecNodes->size(); st++)
	{
		ParserOneEvent((*pVecNodes)[st]);
	}
	return TRUE;
}

void CUIEventBase::AttachListener(const LPXMLAttrMap pAttrMap)
{
	ATLASSERT(pAttrMap);
	if(pAttrMap == NULL)
		return;
	std::string strName = (*pAttrMap)["name"];
	LPVecEvent pVecEvent = NULL;
	LuaEventMap::iterator it = m_mapEvent.find(strName);
	if(it == m_mapEvent.end())
	{
		pVecEvent = new VecEvent;
		m_mapEvent[strName] = pVecEvent;
	}
	else
	{
		pVecEvent = it->second;
	}
	LPEventNode pEventNode = new EventNode;
	pEventNode->strPath = (*pAttrMap)["file"];
	pEventNode->strFuncName = (*pAttrMap)["func"];
	pVecEvent->push_back(pEventNode);
}

void CUIEventBase::DetachListener()
{
	
}

int CUIEventBase::AttachListener(lua_State* L)
{
	const char* pszName = lua_tostring(L, 2);
	if(NULL == pszName || strlen(pszName) <= 0)
	{
		ATLASSERT(FALSE);
		return 0;
	}
	std::string strName(pszName);
	if(!lua_isfunction(L, 3))
	{
		ATLASSERT(FALSE);
		return 0;
	}
	LPVecEvent pVecEvent = NULL;
	LuaEventMap::iterator it = m_mapEvent.find(strName);
	if(it == m_mapEvent.end())
	{
		pVecEvent = new VecEvent;
		m_mapEvent[strName] = pVecEvent;
	}
	else
	{
		pVecEvent = it->second;
	}
	lua_pushvalue(L, 3);
	int nFunIndex = luaL_ref(L, LUA_REGISTRYINDEX);
	LPEventNode pEventNode = new EventNode;
	pEventNode->nFuncIndex = nFunIndex;

	if(lua_gettop(L) > 4)
	{
		if(lua_isboolean(L, 4))
		{
			bool bPush = (bool)lua_toboolean(L, 4);
			if(bPush)
			{
				pVecEvent->push_back(pEventNode);
			}
			else
			{
				pVecEvent->insert(pVecEvent->begin(), pEventNode);
			}
		}
		else if(lua_isnumber(L, 4))
		{
			int nIndex = lua_tonumber(L, 4);
			if(nIndex >= pVecEvent->size())
			{
				pVecEvent->push_back(pEventNode);
			}
			else if(nIndex <= 0)
			{
				pVecEvent->insert(pVecEvent->begin(), pEventNode);
			}
			else
			{
				VecEvent::iterator it = pVecEvent->begin();
				it += (nIndex - 1);
				pVecEvent->insert(it, pEventNode);
			}
		}
	}
	else
	{
		pVecEvent->push_back(pEventNode);
	}
	lua_pushboolean(L, 1);
	return 1;
}

int CUIEventBase::DetachListener(lua_State* L)
{
	return 0;
}

void CUIEventBase::DispatchListener(UIDISPPARAMS& params)
{
	LOG_AUTO();
	lua_State* L = UILuaGetLuaVM(NULL);
	LuaEventMap::const_iterator itLuaEvent = m_mapEvent.find(params.strName);
	if (itLuaEvent != m_mapEvent.end())
	{
		LPVecEvent pVecEvent = itLuaEvent->second;
		for(VecEvent::size_type st = 0; st < pVecEvent->size(); st ++)
		{
			LPEventNode pEventNode = (*pVecEvent)[st];
			PushEventParams(params);
			if(pEventNode->strPath.length() > 0)
			{
				UILuaManagerInstance.CallLuaFunc(pEventNode->strPath, pEventNode->strFuncName, params.nArgs, params.nRet, NULL);
			}
			else
			{
				UILuaManagerInstance.CallLuaFuncByIndex(pEventNode->nFuncIndex, params.nArgs, params.nRet, NULL);
			}
			lua_pop(L, params.nRet);
		}
	}
}

BOOL CUIEventBase::OnBindEvent(const std::string& strPath)
{
	char szPath[MAX_PATH] = {0};
	strcpy_s(szPath, MAX_PATH, strPath.c_str());
	::PathAppendA(szPath, "..");
	lua_State* L = UILuaGetLuaVM(NULL);
	LuaEventMap::const_iterator it = m_mapEvent.begin();
	for (; it != m_mapEvent.end(); it++)
	{
		LPVecEvent pVecEvent = it->second;
		// CUILuaManager::GetLuaFuncIndex(strPath, pVecEvent-
		for(VecEvent::size_type st = 0; st != (*pVecEvent).size(); st ++)
		{
			char szLuaPath[MAX_PATH] = {0};
			LPEventNode pEventNode = (*pVecEvent)[st];
			::PathCombineA(szLuaPath, szPath, pEventNode->strPath.c_str());
			UILuaManagerInstance.DoXmlLuaFile(szLuaPath, NULL);
			pEventNode->strPath = szLuaPath;
			pEventNode->nFuncIndex = UILuaManagerInstance.GetLuaFuncIndex(szLuaPath, pEventNode->strFuncName, NULL);
		}
	}
	return TRUE;
}

void CUIEventBase::PushEventParams(UIDISPPARAMS& params)
{
	lua_State* L = UILuaGetLuaVM(NULL);
	for(int i = 0; i < (int)params.nArgs; i++)
	{
		Util::lua::PushVariantToLuaStack(L, &params.rgvarg[i]);
	}
}
