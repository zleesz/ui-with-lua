#include "StdAfx.h"
#include "UIButton.h"

CUIButton::CUIButton(void)
{
	RegisterClass(this);
}

CUIButton::CUIButton(CUITreeContainer* pTree, LPXMLDOMNode pNode)
	: CUIControlBase(pTree, pNode)
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
	XMLChildNodes::const_iterator it2 = pAttrChildNodes->find("normalbkg");
	if(it2 != pAttrChildNodes->end())
	{
		LPXMLDOMNode pImageNode = it2->second;
		// m_strImageID = pImageNode->strUData;
	}
}

CUIButton::~CUIButton(void)
{
	UnRegisterClass(this);
}

int CUIButton::GetID(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	std::string strID = pThis->GetID();
	lua_pushstring(L, strID.c_str());
	return 1;
}

void CUIButton::SetAttr(std::string strName, std::string strValue)
{
	__super::SetAttr(strName, strValue);
}

void CUIButton::Render(CDCHandle dc)
{
	
}