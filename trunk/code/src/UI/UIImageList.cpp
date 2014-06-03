#include "StdAfx.h"
#include "UIImageList.h"
#include "UIResFactory.h"

CUIImagelist::~CUIImagelist(void)
{
	UnRegisterClass(this);
	if (m_hBitmapList)
	{
		UIGraphicInstance->ReleaseBitmapList(m_hBitmapList);
		m_hBitmapList = NULL;
	}
	if (m_hBitmap)
	{
		UIGraphicInstance->ReleaseBitmap(m_hBitmap);
		m_hBitmap = NULL;
	}
}

CUIImagelist::CUIImagelist(LPXMLDOMNode pNode, const char* pszPath)
	: CUIResBase(pNode), m_hBitmapList(NULL), m_hBitmap(NULL)
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
	std::wstring wstrPath;
	Util::StringToWideString(szPath, m_strPath);
	
	m_hBitmap = UIGraphicInstance->LoadBitmapFromFile(m_strPath.c_str());
	if (m_hBitmap == NULL)
	{
		assert(false);
		return;
	}
	LPXMLChildNodes pChildNode = pNode->pMapChildNode;
	if (pChildNode == NULL)
	{
		return;
	}
	XMLChildNodes::iterator it = pChildNode->begin();
	for (; it != pChildNode->end(); it++)
	{
		LPXMLVecNodes pVecNodes = it->second->pVecNode;
		for(int i = 0; i < (int)pVecNodes->size(); i++)
		{
			CUIResBase* pUIBase = NULL;
			if((*pVecNodes)[i]->strName == "bitmap")
			{
				pUIBase = new CUIBitmap((*pVecNodes)[i], this);
			}
			else if((*pVecNodes)[i]->strName == "texture")
			{
				pUIBase = new CUITexture((*pVecNodes)[i], this);
			}
			if(NULL == pUIBase)
			{
				continue;
			}
			if (!UIResFactoryInstance->AddRes(pUIBase))
			{
				delete pUIBase;
				pUIBase = NULL;
			}
		}
	}
}

int CUIImagelist::GetID(lua_State* L)
{
	CUIResBase* pThis = (CUIResBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	std::string strID = pThis->GetPrivateID();
	lua_pushstring(L, strID.c_str());
	return 1;
}

BITMAPLIST_HANDLE CUIImagelist::GetImageList()
{
	if (m_hBitmapList == NULL)
	{
		m_hBitmapList = UIGraphicInstance->CreateBitmapList(m_hBitmap);
		assert(m_hBitmapList);
	}
	return m_hBitmapList;
}

BITMAP_HANDLE CUIImagelist::GetBitmapByIndex(int nIndex)
{
	if (nIndex <= 0)
	{
		assert(false);
		return NULL;
	}
	BITMAPLIST_HANDLE hBitmapList = GetImageList();
	if (hBitmapList == NULL)
	{
		assert(false);
		return NULL;
	}
	return UIGraphicInstance->GetBitmapFromList(hBitmapList, nIndex);
}