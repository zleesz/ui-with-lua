#include "StdAfx.h"
#include "UIImageList.h"

CUIImageList::CUIImageList(void)
{
}

CUIImageList::~CUIImageList(void)
{
	UnRegisterClass(this);
}

CUIImageList::CUIImageList(LPXMLDOMNode pNode, const char* pszPath) : CUIResBase(pNode)
{
	RegisterClass(this);
	if(pNode == NULL)
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	std::string strPath = (*pMapAttr)["path"];
	char szPath[MAX_PATH] = {0};
	strcpy_s(szPath, MAX_PATH, pszPath);
	PathRemoveFileSpecA(szPath);
	PathCombineA(szPath, szPath, strPath.c_str());
	m_ImagelistData.strPath = szPath;

	std::string strSeperator = (*pMapAttr)["seperator"];
	std::string strBlockSize = (*pMapAttr)["blocksize"];
	std::string strVertical = (*pMapAttr)["vertical"];
	if(strSeperator == "1" || strSeperator == "true")
	{
		m_ImagelistData.bSeperator = TRUE;
	}
	if(strBlockSize.length() > 0)
	{
		m_ImagelistData.uBlockSize = atoi(strBlockSize.c_str());
	}
	assert(m_ImagelistData.uBlockSize > 0);
	if(strVertical == "1" || strVertical == "true")
	{
		m_ImagelistData.bVertical = TRUE;
	}
}

ResourceType CUIImageList::GetType()
{
	return RT_UIIMAGELIST;
}

int CUIImageList::GetID(lua_State* L)
{
	CUIResBase* pThis = (CUIResBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	std::string strID = pThis->GetID();
	lua_pushstring(L, strID.c_str());
	return 1;
}
