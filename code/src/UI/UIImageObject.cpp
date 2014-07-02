#include "StdAfx.h"
#include "UIImageObject.h"
#include "UIBitmap.h"
#include "UIResFactory.h"

CUIImageObject::CUIImageObject(void)
{
	RegisterClass(this);
}

CUIImageObject::CUIImageObject(CUITreeContainer* pTree, LPXMLDOMNode pNode)
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

CUIImageObject::CUIImageObject(CUITreeContainer* pTree)
: CUIControlBase(pTree), m_bStretch(FALSE)
{
	RegisterClass(this);
}

CUIImageObject::~CUIImageObject(void)
{
	UnRegisterClass(this);
}

int CUIImageObject::GetStretch(lua_State* L)
{
	CUIImageObject** ppThis = (CUIImageObject**)luaL_checkudata(L, -1, GetRigisterClassName());
	CUIImageObject* pThis = *ppThis;
	if (!pThis)
	{
		ATLASSERT(pThis);
		return 0;
	}
	lua_pushboolean(L, pThis->m_bStretch);
	return 1;
}

int CUIImageObject::SetStretch(lua_State* L)
{
	CUIImageObject** ppThis = (CUIImageObject**)luaL_checkudata(L, -1, GetRigisterClassName());
	CUIImageObject* pThis = *ppThis;
	if (!pThis)
	{
		ATLASSERT(pThis);
		return 0;
	}
	pThis->m_bStretch = (BOOL)lua_toboolean(L, -2);
	return 0;
}

int CUIImageObject::GetVisible(lua_State* L)
{
	CUIImageObject** ppThis = (CUIImageObject**)luaL_checkudata(L, -1, GetRigisterClassName());
	CUIControlBase* pThis = static_cast<CUIControlBase*>(*ppThis);
	if (!pThis)
	{
		ATLASSERT(pThis);
		return 0;
	}
	lua_pushboolean(L, pThis->GetVisible());
	return 1;
}

int CUIImageObject::SetVisible(lua_State* L)
{
	CUIImageObject** ppThis = (CUIImageObject**)luaL_checkudata(L, -1, GetRigisterClassName());
	CUIImageObject* pThis = *ppThis;
	if (!pThis)
	{
		ATLASSERT(pThis);
		return 0;
	}
	BOOL bVisible = (BOOL)lua_toboolean(L, -2);
	pThis->SetAttr("visible", bVisible ? "true" : "false");
	return 0;
}

int CUIImageObject::GetEnable(lua_State* L)
{
	CUIImageObject** ppThis = (CUIImageObject**)luaL_checkudata(L, -1, GetRigisterClassName());
	CUIControlBase* pThis = static_cast<CUIControlBase*>(*ppThis);
	if (!pThis)
	{
		ATLASSERT(pThis);
		return 0;
	}
	lua_pushboolean(L, pThis->GetEnable());
	return 1;
}

int CUIImageObject::GetImage(lua_State* L)
{
	CUIImageObject** ppThis = (CUIImageObject**)luaL_checkudata(L, -1, GetRigisterClassName());
	CUIImageObject* pThis = *ppThis;
	if (!pThis)
	{
		ATLASSERT(pThis);
		return 0;
	}
	CUIBitmap* pUIBitmap = UIResFactoryInstance->GetBitmap(pThis->m_strImageID.c_str());

	UILuaPushClassObj(L, pUIBitmap);
	return 1;
}

void CUIImageObject::SetAttr(const std::string& strName, const std::string& strValue)
{
	if (_SetAttr(strName, strValue))
	{
		return;
	}
}

void CUIImageObject::Render(CDCHandle dc)
{
	CUIBitmap* pUIBitmap = UIResFactoryInstance->GetBitmap(m_strImageID.c_str());
	if(NULL == pUIBitmap)
	{
		ATLASSERT(FALSE);
		return ;
	}
	const RECT rc = GetObjPos();
	BITMAP_HANDLE hBitmap = pUIBitmap->GetBitmap();
	UIGraphicInstance->AlphaPaintBitmap(dc.m_hDC, hBitmap, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

void CUIImageObject::OnInitControl()
{

}

void CUIImageObject::OnDetroy()
{

}

void CUIImageObject::OnRButtonUp(int x, int y)
{
	FireMouseEvent("OnRButtonUp", x, y);
}

void CUIImageObject::OnLButtonDown(int x, int y)
{
	FireMouseEvent("OnLButtonDown", x, y);
}
