#include "StdAfx.h"
#include "UIControlBase.h"

CUIControlBase::CUIControlBase(void)
{
}

CUIControlBase::CUIControlBase(CUITreeContainer* pTree, LPXMLDOMNode pNode)
	: m_pTree(pTree)
{
	if(pNode == NULL || pNode->pMapAttr == NULL)
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	SetID((*pMapAttr)["id"]);
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
	XMLChildNodes::const_iterator it2 = pAttrChildNodes->begin();
	for(; it2 != pAttrChildNodes->end(); it2++)
	{
		SetAttr(it2->first, it2->second->strUData);
	}
}

CUIControlBase::~CUIControlBase(void)
{
}

void CUIControlBase::SetAttr(std::string strName, std::string strValue)
{
	if(strName == "left" ||
		strName == "top" ||
		strName == "width" ||
		strName == "height" ||
		strName == "zorder")
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

void CUIControlBase::GetAttr(std::string strName, VARIANT* v)
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

const std::string& CUIControlBase::GetID()
{
	return m_strID;
}

void CUIControlBase::SetID(std::string strID)
{
	m_strID = strID;
}

CUITreeContainer* CUIControlBase::GetOwnerTree()
{
	return m_pTree;
}

ULONG CUIControlBase::GetZorder()
{
	CComVariant v;
	GetAttr("zorder", &v);
	if(v.vt != VT_I4)
		return 0;
	return v.ulVal;
}

const RECT& CUIControlBase::GetObjPos()
{
	RECT rc = {};
	CComVariant vLeft, vTop, vWidth, vHeight;
	GetAttr("left", &vLeft);
	GetAttr("top", &vTop);
	GetAttr("width", &vWidth);
	GetAttr("height", &vHeight);
	if(vLeft.vt != VT_I4)
		rc.left = 0;
	else
		rc.left = vLeft.intVal;
	if(vTop.vt != VT_I4)
		rc.top = 0;
	else
		rc.top = vTop.intVal;
	if(vWidth.vt != VT_I4)
		rc.right = 0;
	else
		rc.right = rc.left + vWidth.intVal;
	if(vHeight.vt != VT_I4)
		rc.bottom = 0;
	else
		rc.bottom = rc.top + vHeight.intVal;
	return rc;
}

BOOL CUIControlBase::OnHitTest(int x, int y)
{
	BOOL bVisible = GetVisible();
	if(!bVisible)
		return FALSE;
	const RECT rc = GetObjPos();
	if(x > rc.left && x < rc.bottom
		&& y > rc.top && y < rc.bottom)
		return TRUE;
	return FALSE;
}

BOOL CUIControlBase::GetVisible()
{
	BOOL bVisible = TRUE;
	CComVariant vVisible;
	GetAttr("visible", &vVisible);
	if(vVisible.vt == VT_I2)
	{
		bVisible = vVisible.boolVal == VARIANT_TRUE ? TRUE : FALSE;
	}
	return bVisible;
}

BOOL CUIControlBase::GetEnable()
{
	BOOL bEnable = TRUE;
	CComVariant vEnable;
	GetAttr("enable", &vEnable);
	if(vEnable.vt == VT_I2)
	{
		bEnable = vEnable.boolVal == VARIANT_TRUE ? TRUE : FALSE;
	}
	return bEnable;
}