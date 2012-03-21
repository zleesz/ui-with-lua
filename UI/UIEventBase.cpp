#include "stdafx.h"
#include "UIEventBase.h"

CUIEventBase::CUIEventBase()
{
	
}

CUIEventBase::~CUIEventBase()
{
	// delete m_mapEvent
}

void CUIEventBase::AttachListener(const LPXMLAttrMap pAttrMap, LPEventNode& pEventNode)
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
	pEventNode = new EventNode;
	pEventNode->strPath = (*pAttrMap)["file"];
	pEventNode->strFuncName = (*pAttrMap)["func"];
	pVecEvent->push_back(pEventNode);
}

void CUIEventBase::DetachListener()
{
	
}

void CUIEventBase::DispatchListener(const std::string& strEventName)
{
	LOG_AUTO();
	LuaEventMap::const_iterator it = m_mapEvent.find(strEventName);
	if (it != m_mapEvent.end())
	{
		LPVecEvent pVecEvent = it->second;
		for(VecEvent::size_type st = 0; st < pVecEvent->size(); st ++)
		{
			LPEventNode pEventNode = (*pVecEvent)[st];
			CUILuaManager::GetInstance().CallLuaFunc(pEventNode->strPath, pEventNode->strFuncName, NULL);
		}
	}
}

BOOL CUIEventBase::OnInitEvent(const std::string& strPath)
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
			CUILuaManager::GetInstance().DoXmlLuaFile(szLuaPath, NULL);
			pEventNode->strPath = szLuaPath;
			pEventNode->nFuncIndex = CUILuaManager::GetInstance().GetLuaFuncIndex(szLuaPath, pEventNode->strFuncName, NULL);
		}
	}
	return TRUE;
}

void* CUIEventBase::GetUserData()
{
	return m_pUserData;
}