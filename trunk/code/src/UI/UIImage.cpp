#include "StdAfx.h"
#include "UIImage.h"
#include "UIBitmap.h"
#include "UIResFactory.h"

CUIImage::CUIImage(void)
{
	RegisterClass(this);
}

CUIImage::CUIImage(LPXMLDOMNode pNode)
	: CUIControlBase(pNode), m_bStretch(FALSE)
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

int CUIImage::SetStretch(lua_State* L)
{
	CUIImage* pThis = (CUIImage*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushboolean(L, pThis->m_bStretch);
	return 1;
}

int CUIImage::GetImage(lua_State* L)
{
	CUIImage* pThis = (CUIImage*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	CUIBitmap* pUIBitmap = CUIResFactory::GetInstance()->GetBitmap(pThis->m_strImageID.c_str());

	UILuaPushClassObj(L, pUIBitmap);
	return 1;
}
void CUIImage::SetAttr(std::string strName, std::string strValue)
{

}

void CUIImage::Render(CDCHandle dc)
{
	CUIBitmap* pUIBitmap = CUIResFactory::GetInstance()->GetBitmap(m_strImageID.c_str());
	RECT rc = {0};
	CComVariant vLeft, vTop, vWidth, vHeight;
	GetAttr("left", &vLeft);
	GetAttr("top", &vTop);
	GetAttr("width", &vWidth);
	GetAttr("height", &vHeight);
	rc.left = vLeft.intVal;
	rc.top = vTop.intVal;
	rc.right = vLeft.intVal + vWidth.intVal;
	rc.bottom = vTop.intVal + vHeight.intVal;
	pUIBitmap->Render(dc.m_hDC, rc, m_bStretch);
}