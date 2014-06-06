#include "StdAfx.h"
#include ".\uibitmap.h"

CUIBitmap::CUIBitmap(void)
{
}

CUIBitmap::~CUIBitmap(void)
{
	UnRegisterClass(this);
	if(NULL != m_hBitmap)
	{
		UIGraphicInstance->ReleaseBitmap(m_hBitmap);
		m_hBitmap = NULL;
	}
}

CUIBitmap::CUIBitmap(LPXMLDOMNode pNode, const char* pszPath)
	: CUIResBase(pNode), m_hBitmap(NULL)
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
	Util::UTF8_to_Unicode(szPath, wstrPath);
	m_hBitmap = UIGraphicInstance->LoadBitmapFromFile(wstrPath.c_str());
	assert(m_hBitmap);
}

CUIBitmap::CUIBitmap(LPXMLDOMNode pNode, CUIImagelist* pImagelist)
	: CUIResBase(pNode), m_hBitmap(NULL)
{
	RegisterClass(this);
	if(pNode == NULL)
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	std::string strIndex = (*pMapAttr)["index"];
	int nIndex = 0;
	if(strIndex.length() > 0)
	{
		nIndex = atoi(strIndex.c_str());
	}
	assert(nIndex > 0);
	m_hBitmap = pImagelist->GetBitmapByIndex(nIndex);
	assert(m_hBitmap);
}

int CUIBitmap::GetID(lua_State* L)
{
	CUIResBase* pThis = (CUIResBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushstring(L, pThis->GetPrivateID());
	return 1;
}

BITMAP_HANDLE CUIBitmap::GetBitmap()
{
	return m_hBitmap;
}