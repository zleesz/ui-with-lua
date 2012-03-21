#include "StdAfx.h"
#include "UIWindowBase.h"

CUIWindowBase::CUIWindowBase(void)
{
}

CUIWindowBase::CUIWindowBase(const std::string& strPath, LPXMLDOMNode pNode)
{
	if(pNode == NULL || pNode->pMapAttr == NULL)
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	assert((*pMapAttr)["id"].length() > 0);
	if((*pMapAttr)["id"].length() <= 0)
	{
		return;
	}
	m_pUIEventWindow = new CUIEventWindow((void*)this);
	SetID((*pMapAttr)["id"]);
	m_strXMLPath = strPath;
	std::transform(strPath.begin(),strPath.end(),m_strXMLPath.begin(),tolower);
	//PathCanonicalizeA(m_strXMLPath, m_strXMLPath);
	LPXMLDOMNode pUITreeNode = (*pNode->pMapChildNode)["uitree"];
	if(pUITreeNode != NULL && pUITreeNode->pMapChildNode != NULL)
	{
		ParserUITree(pUITreeNode);
	}
	LPXMLDOMNode pAttrNode = (*pNode->pMapChildNode)["attr"];
	if(pAttrNode != NULL && pAttrNode->pMapChildNode != NULL)
	{
		ParserAttr(pAttrNode);
	}
	LPXMLDOMNode pEventNode = (*pNode->pMapChildNode)["eventlist"];
	if(pEventNode != NULL && pEventNode->pMapChildNode != NULL)
	{
		ParserEvent(pEventNode);
	}
}

CUIWindowBase::~CUIWindowBase(void)
{
}

void CUIWindowBase::SetAttr(std::string strName, std::string strValue)
{
	if(strName == "left" ||
		strName == "top" ||
		strName == "width" ||
		strName == "height")
	{
		CComVariant v(atoi(strValue.c_str()));
		v.Detach(&m_mapAttr[strName]);
	}
	else if(strName == "visible")
	{
		CComVariant v;
		if(strValue == "true" || strValue == "1")
		{
			v = VARIANT_TRUE;
		}
		else
		{
			v = VARIANT_FALSE;
		}
		v.Detach(&m_mapAttr[strName]);
	}
}

void CUIWindowBase::GetAttr(std::string strName, VARIANT* v)
{
	VariantInit(v);
	ID2AttrMap::const_iterator it = m_mapAttr.find(strName);
	if(it != m_mapAttr.end())
	{
		CComVariant vAttr;
		vAttr = it->second;
		vAttr.Detach(v);
	}
}

BOOL CUIWindowBase::ParserAttr(LPXMLDOMNode pAttrNode)
{
	if (NULL == pAttrNode || pAttrNode->strName != "attr")
		return FALSE;
	LPXMLChildNodes pChildNodes = pAttrNode->pMapChildNode;
	if(NULL == pChildNodes)
		return FALSE;
	XMLChildNodes::const_iterator it = pChildNodes->begin();
	for(; it != pChildNodes->end(); it++)
	{
		SetAttr(it->first, it->second->strUData);
	}
	return TRUE;
}

BOOL CUIWindowBase::ParserUITree(LPXMLDOMNode pNode)
{
	if (NULL == pNode || pNode->strName != "uitree")
		return FALSE;
	return m_UITreeContainer.ParserUITree(pNode);
}

BOOL CUIWindowBase::ParserEvent(LPXMLDOMNode pNode)
{
	if (NULL == pNode || pNode->strName != "eventlist")
		return FALSE;
	return m_pUIEventWindow->ParserEvent(pNode);
}
