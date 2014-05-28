#include "StdAfx.h"
#include ".\uibitmap.h"
#include "UIImagelist.h"

CUIBitmap::CUIBitmap(void)
{
}

CUIBitmap::~CUIBitmap(void)
{
	UnRegisterClass(this);
	if(NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
}

CUIBitmap::CUIBitmap(LPXMLDOMNode pNode, const char* pszPath)
	: CUIResBase(pNode), m_pBitmap(NULL)
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
	std::string strType = (*pMapAttr)["type"];
	
	if(strType == "ThreeInOneH")
	{
		m_pBitmap = new CUIBitmapThreeInOneH;
	}
	else if(strType == "ThreeInOneV")
	{
		m_pBitmap = new CUIBitmapThreeInOneV;
	}
	else if(strType == "NineInOne")
	{
		m_pBitmap = new CUIBitmapNineInOne;
	}
	else
	{
		m_pBitmap = new CUIBitmapDefault;
	}
	m_pBitmap->m_pBitmapLoad = new CUIBitmapFile;
	CUIBitmapFile* pBitmapLoadFile = (CUIBitmapFile*)m_pBitmap->m_pBitmapLoad;
	pBitmapLoadFile->SetPath(szPath);
}

CUIBitmap::CUIBitmap(LPXMLDOMNode pNode, CUIImagelist* pImagelist)
	: CUIResBase(pNode), m_pBitmap(NULL)
{
	RegisterClass(this);
	if(pNode == NULL)
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	std::string strType = (*pMapAttr)["type"];

	if(strType == "ThreeInOneH")
	{
		m_pBitmap = new CUIBitmapThreeInOneH;
	}
	else if(strType == "ThreeInOneV")
	{
		m_pBitmap = new CUIBitmapThreeInOneV;
	}
	else if(strType == "NineInOne")
	{
		m_pBitmap = new CUIBitmapNineInOne;
	}
	else
	{
		m_pBitmap = new CUIBitmapDefault;
	}
	std::string strIndex = (*pMapAttr)["index"];
	int nIndex = 0;
	if(strIndex.length() > 0)
	{
		nIndex = atoi(strIndex.c_str());
	}
	m_pBitmap->m_pBitmapLoad = new CUIBitmapImagelist;
	CUIBitmapImagelist* pBitmapImagelist = (CUIBitmapImagelist*)m_pBitmap->m_pBitmapLoad;
	pBitmapImagelist->SetImagelist(pImagelist, nIndex);
}

int CUIBitmap::GetID(lua_State* L)
{
	CUIResBase* pThis = (CUIResBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushstring(L, pThis->GetID());
	return 1;
}

CUIBitmapBase::~CUIBitmapBase()
{
	if(NULL != m_pBitmapLoad)
	{
		delete m_pBitmapLoad;
		m_pBitmapLoad = NULL;
	}
	int nSize = (int)m_vecImage.size();
	if(nSize > 1)
	{
		for(int i = 0; i < nSize; i++)
		{
			CxImage* pImage = m_vecImage[i];
			delete pImage;
		}
	}
	m_vecImage.clear();
}

Bitmap* CUIBitmapFile::GetImage()
{
	if(m_pImage == NULL)
	{
		ATLASSERT(::PathFileExistsA(m_strPath.c_str()));
		if(!::PathFileExistsA(m_strPath.c_str()))
			return NULL;
		std::wstring wstrPath;
		Util::StringToWideString(m_strPath.c_str(), wstrPath);
		m_pImage = ImageLoaderFactory::Instance()->LoadImageFromFile(wstrPath.c_str(), 0);
	}
	return m_pImage;
}

Bitmap* CUIBitmapImagelist::GetImage()
{
	ATLASSERT(m_pImagelist);
	if(NULL == m_pImagelist)
		return NULL;
	Bitmap* pImage = m_pImagelist->GetBitmapByIndex(m_nIndex);
	return pImage;
}

void CUIBitmapImagelist::SetImagelist(CUIImagelist* pImagelist, int nIndex)
{
	 m_pImagelist = pImagelist;
	 m_nIndex = nIndex;
}

void CUIBitmap::Render(HDC dc, const RECT& rc, BOOL bStretch)
{
	ATLASSERT(NULL != m_pBitmap);
	if(NULL == m_pBitmap)
		return;
	m_pBitmap->Render(dc, rc, bStretch);
}