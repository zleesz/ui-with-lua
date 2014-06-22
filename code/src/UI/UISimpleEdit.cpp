#include "StdAfx.h"
#include "UISimpleEdit.h"

CUISimpleEdit::CUISimpleEdit(void)
{
	RegisterClass(this);
}

CUISimpleEdit::~CUISimpleEdit(void)
{
	UnRegisterClass(this);
}

CUISimpleEdit::CUISimpleEdit(CUITreeContainer* pTree, LPXMLDOMNode pNode)
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
	XMLChildNodes::const_iterator it2 = pAttrChildNodes->find("text");
	if (it2 != pAttrChildNodes->end())
	{
		m_strText = it2->second->strUData;
	}
}

CUISimpleEdit::CUISimpleEdit(CUITreeContainer* pTree)
: CUIControlBase(pTree)
{
	RegisterClass(this);
}

void CUISimpleEdit::Render(CDCHandle dc)
{
}

void CUISimpleEdit::OnInitControl()
{
}

void CUISimpleEdit::OnDetroy()
{
}

void CUISimpleEdit::SetAttr(const std::string& strName, const std::string& strValue)
{
}
