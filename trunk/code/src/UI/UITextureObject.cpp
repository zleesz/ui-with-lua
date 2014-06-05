#include "StdAfx.h"
#include "UITextureObject.h"
#include "UITexture.h"
#include "UIResFactory.h"

CUITextureObject::CUITextureObject(void)
{
}

CUITextureObject::CUITextureObject(CUITreeContainer* pTree, LPXMLDOMNode pNode)
: CUIControlBase(pTree, pNode), m_bStretch(FALSE)
{
	RegisterClass(this);
	if(pNode == NULL || pNode->pMapAttr == NULL)
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	LPXMLChildNodes pChildNodes = pNode->pMapChildNode;
	if(pChildNodes == NULL)
		return;
	XMLChildNodes::const_iterator it = pChildNodes->find("attr");
	if(it == pChildNodes->end())
		return;
	LPXMLDOMNode pAttrNode = it->second;
	if(pAttrNode == NULL)
		return;
	LPXMLChildNodes pAttrChildNodes = pAttrNode->pMapChildNode;
	if(pAttrChildNodes == NULL)
		return;
	XMLChildNodes::const_iterator it2 = pAttrChildNodes->find("texture");
	if(it2 != pAttrChildNodes->end())
	{
		LPXMLDOMNode pImageNode = it2->second;
		m_strTextureID = pImageNode->strUData;
	}
	it2 = pAttrChildNodes->find("stretch");
	if(it2 != pAttrChildNodes->end())
	{
		LPXMLDOMNode pStretchNode = it2->second;
		if(pStretchNode->strUData == "1" || pStretchNode->strUData == "true")
		{
			m_bStretch = TRUE;
		}
	}
}

CUITextureObject::CUITextureObject(CUITreeContainer* pTree)
: CUIControlBase(pTree), m_bStretch(FALSE)
{
	RegisterClass(this);
}

CUITextureObject::~CUITextureObject(void)
{
	UnRegisterClass(this);
}

int CUITextureObject::GetID(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	std::string strID = pThis->GetID();
	lua_pushstring(L, strID.c_str());
	return 1;
}

int CUITextureObject::GetStretch(lua_State* L)
{
	CUITextureObject* pThis = (CUITextureObject*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushboolean(L, pThis->m_bStretch);
	return 1;
}

int CUITextureObject::SetStretch(lua_State* L)
{
	CUITextureObject* pThis = (CUITextureObject*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	pThis->m_bStretch = (BOOL)lua_toboolean(L, -2);
	return 0;
}

int CUITextureObject::GetVisible(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushboolean(L, pThis->GetVisible());
	return 1;
}

int CUITextureObject::SetVisible(lua_State* L)
{
	CUITextureObject* pThis = (CUITextureObject*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	BOOL bVisible = (BOOL)lua_toboolean(L, -2);
	pThis->SetAttr("visible", bVisible ? "true" : "false");
	return 0;
}

int CUITextureObject::GetEnable(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushboolean(L, pThis->GetEnable());
	return 1;
}

int CUITextureObject::GetTexture(lua_State* L)
{
	CUITextureObject* pThis = (CUITextureObject*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	CUITexture* pUITexture = UIResFactoryInstance->GetTexture(pThis->m_strTextureID.c_str());

	UILuaPushClassObj(L, pUITexture);
	return 1;
}

void CUITextureObject::SetAttr(const std::string& strName, const std::string& strValue)
{
	__super::SetAttr(strName, strValue);
}

void CUITextureObject::Render(CDCHandle dc)
{
	CUITexture* pUITexture = UIResFactoryInstance->GetTexture(m_strTextureID.c_str());
	if(NULL == pUITexture)
	{
		ATLASSERT(FALSE);
		return ;
	}
	const RECT rc = GetObjPos();
	SIZE sz = {rc.right - rc.left, rc.bottom - rc.top};
	BITMAP_HANDLE hBitmap = pUITexture->GetTextureBitmap(sz);
	UIGraphicInstance->AlphaPaintBitmap(dc.m_hDC, hBitmap, rc.left, rc.top, sz.cx, sz.cy);
	UIGraphicInstance->ReleaseBitmap(hBitmap);
}

int CUITextureObject::GetOwnerTree(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	const CUITreeContainer* pTree = pThis->GetOwnerTree();
	UILuaPushClassObj(L, pTree);
	return 1;
}

void CUITextureObject::OnInitControl()
{

}

void CUITextureObject::OnDetroy()
{

}

void CUITextureObject::OnRButtonUp(int x, int y)
{
	FireMouseEvent("OnRButtonUp", x, y);
}

void CUITextureObject::OnLButtonDown(int x, int y)
{
	FireMouseEvent("OnLButtonDown", x, y);
}
