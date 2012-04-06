#include "stdafx.h"
#include "UIEventBase.h"
#include "UIWindowBase.h"

CUIEventBase::CUIEventBase()
	: m_pBindWnd(NULL)
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

void CUIEventBase::DispatchListener(UIDISPPARAMS& params)
{
	LOG_AUTO();
	PushEventParams(params);
	LuaEventMap::const_iterator itLuaEvent = m_mapEvent.find(params.strName);
	if (itLuaEvent != m_mapEvent.end())
	{
		LPVecEvent pVecEvent = itLuaEvent->second;
		for(VecEvent::size_type st = 0; st < pVecEvent->size(); st ++)
		{
			LPEventNode pEventNode = (*pVecEvent)[st];
			UILuaManagerInstance.CallLuaFunc(pEventNode->strPath, pEventNode->strFuncName, params.nArgs, params.nRet, NULL);
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

void* CUIEventBase::GetBindWnd()
{
	return m_pBindWnd;
}

void CUIEventBase::PushEventParams(UIDISPPARAMS& params)
{
	lua_State* L = UILuaGetLuaVM(NULL);
	for(int i = 0; i < (int)params.nArgs; i++)
	{
		Util::lua::PushVariantToLuaStack(L, &params.rgvarg[i]);
	}
}
