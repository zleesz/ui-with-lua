#include "StdAfx.h"
#include "UITexture.h"

CUITexture::CUITexture(void)
{
}

CUITexture::~CUITexture(void)
{
	UnRegisterClass(this);
	if(NULL != m_hTexture)
	{
		UIGraphicInstance->ReleaseTexture(m_hTexture);
		m_hTexture = NULL;
	}
	if(NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
}

CUITexture::CUITexture(LPXMLDOMNode pNode, const char* pszPath)
: CUIResBase(pNode), m_pBitmap(NULL), m_hTexture(NULL)
{
	RegisterClass(this);
	if(pNode == NULL)
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	m_pBitmap = new CUIBitmap(pNode, pszPath);
	CreateTextureFromBitmap(pMapAttr);
}

CUITexture::CUITexture(LPXMLDOMNode pNode, CUIImagelist* pImagelist)
: CUIResBase(pNode), m_pBitmap(NULL), m_hTexture(NULL)
{
	RegisterClass(this);
	if(pNode == NULL)
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	m_pBitmap = new CUIBitmap(pNode, pImagelist);
	CreateTextureFromBitmap(pMapAttr);
}

TEXTURE_HANDLE CUITexture::CreateTextureFromBitmap(LPXMLAttrMap pMapAttr)
{
	if (!m_pBitmap)
	{
		return NULL;
	}
	BITMAP_HANDLE hBitmap = m_pBitmap->GetBitmap();
	if (!hBitmap)
	{
		LOG_WARN("get bitmap failed!");
		return NULL;
	}
	std::string strType = (*pMapAttr)["type"];
	if (strType == "ThreeInOneH")
	{
		m_hTexture = UIGraphicInstance->CreateThreeInOneHTexture(hBitmap);
	}
	else if (strType == "ThreeInOneV")
	{
		m_hTexture = UIGraphicInstance->CreateThreeInOneVTexture(hBitmap);
	}
	else if (strType == "FiveInOneH")
	{
		m_hTexture = UIGraphicInstance->CreateFiveInOneHTexture(hBitmap);
	}
	else if (strType == "FiveInOneV")
	{
		m_hTexture = UIGraphicInstance->CreateFiveInOneVTexture(hBitmap);
	}
	else if (strType == "NineInOne")
	{
		m_hTexture = UIGraphicInstance->CreateNineInOneTexture(hBitmap);
	}
	else
	{
		m_hTexture = UIGraphicInstance->CreateFillTexture(hBitmap);
	}
	assert(m_hTexture);
	return m_hTexture;
}

int CUITexture::GetID(lua_State* L)
{
	CUIResBase* pThis = (CUIResBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushstring(L, pThis->GetPrivateID());
	return 1;
}

int CUITexture::GetBitmap(lua_State* L)
{
	CUITexture* pThis = (CUITexture*) lua_touserdata(L, -1);
	UILuaPushClassObj(L, pThis->m_pBitmap);
	return 1;
}

TEXTURE_HANDLE CUITexture::GetTexture()
{
	return m_hTexture;
}

BITMAP_HANDLE CUITexture::GetTextureBitmap(SIZE& sz)
{
	if (!m_hTexture)
	{
		return NULL;
	}
	return UIGraphicInstance->UpdateTexture(m_hTexture, sz);
}