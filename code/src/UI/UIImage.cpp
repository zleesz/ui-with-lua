#include "StdAfx.h"
#include "UIImage.h"
#include "UIBitmap.h"
#include "UIResFactory.h"

CUIImage::CUIImage(void)
{
}

CUIImage::CUIImage(CUITreeContainer* pTree, LPXMLDOMNode pNode)
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
	XMLChildNodes::const_iterator it2 = pAttrChildNodes->find("image");
	if(it2 != pAttrChildNodes->end())
	{
		LPXMLDOMNode pImageNode = it2->second;
		m_strImageID = pImageNode->strUData;
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

CUIImage::~CUIImage(void)
{
	UnRegisterClass(this);
}

int CUIImage::GetID(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	std::string strID = pThis->GetID();
	lua_pushstring(L, strID.c_str());
	return 1;
}

int CUIImage::GetStretch(lua_State* L)
{
	CUIImage* pThis = (CUIImage*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushboolean(L, pThis->m_bStretch);
	return 1;
}

int CUIImage::SetStretch(lua_State* L)
{
	CUIImage* pThis = (CUIImage*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	pThis->m_bStretch = (BOOL)lua_toboolean(L, -2);
	return 0;
}

int CUIImage::GetVisible(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushboolean(L, pThis->GetVisible());
	return 1;
}

int CUIImage::SetVisible(lua_State* L)
{
	CUIImage* pThis = (CUIImage*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	BOOL bVisible = (BOOL)lua_toboolean(L, -2);
	pThis->SetAttr("visible", bVisible ? "true" : "false");
	return 0;
}

int CUIImage::GetEnable(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushboolean(L, pThis->GetEnable());
	return 1;
}

int CUIImage::GetImage(lua_State* L)
{
	CUIImage* pThis = (CUIImage*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	CUIBitmap* pUIBitmap = UIResFactoryInstance->GetBitmap(pThis->m_strImageID.c_str());

	UILuaPushClassObj(L, pUIBitmap);
	return 1;
}

void CUIImage::SetAttr(std::string strName, std::string strValue)
{
	__super::SetAttr(strName, strValue);
}

void CUIImage::Render(CDCHandle dc)
{
	CUIBitmap* pUIBitmap = UIResFactoryInstance->GetBitmap(m_strImageID.c_str());
	if(NULL == pUIBitmap)
	{
		ATLASSERT(FALSE);
		return ;
	}
	const RECT rc = GetObjPos();
	pUIBitmap->Render(dc.m_hDC, rc, m_bStretch);
}

int CUIImage::GetOwnerTree(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	const CUITreeContainer* pTree = pThis->GetOwnerTree();
	UILuaPushClassObj(L, pTree);
	return 1;
}

void CUIImage::OnInitControl()
{

}

void CUIImage::OnDetroy()
{

}

void CUIImage::OnRButtonUp(int x, int y)
{
	FireUIEvent("OnRButtonUp", x, y);
}

void CUIImage::OnLButtonDown(int x, int y)
{
	FireUIEvent("OnLButtonDown", x, y);
}
