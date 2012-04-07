#include "StdAfx.h"
#include "UIButton.h"
#include "UIWindowBase.h"
#include "UIResFactory.h"
#include "UIBitmap.h"

void CUIButtonBitmap::Init(const LPXMLChildNodes pXMLChildNodes)
{
	XMLChildNodes::const_iterator it2 = pXMLChildNodes->find("normalbkg");
	if(it2 != pXMLChildNodes->end())
	{
		LPXMLDOMNode pImageNode = it2->second;
		m_strNormalBkg = pImageNode->strUData;
	}
	it2 = pXMLChildNodes->find("hoverbkg");
	if(it2 != pXMLChildNodes->end())
	{
		LPXMLDOMNode pImageNode = it2->second;
		m_strHoverBkg = pImageNode->strUData;
	}
	it2 = pXMLChildNodes->find("downbkg");
	if(it2 != pXMLChildNodes->end())
	{
		LPXMLDOMNode pImageNode = it2->second;
		m_strDownBkg = pImageNode->strUData;
	}
	it2 = pXMLChildNodes->find("disablebkg");
	if(it2 != pXMLChildNodes->end())
	{
		LPXMLDOMNode pImageNode = it2->second;
		m_strDisableBkg = pImageNode->strUData;
	}
}

void CUIButtonBitmap::Render(CDCHandle dc, const RECT& rc, ButtonState brt)
{
	CUIBitmap* pUIBitmap = NULL;
	switch(brt)
	{
	case BS_NORMAL:
		pUIBitmap = UIResFactoryInstance->GetBitmap(m_strNormalBkg.c_str());
		break;
	case BS_HOVER:
		pUIBitmap = UIResFactoryInstance->GetBitmap(m_strHoverBkg.c_str());
		break;
	case BS_DOWN:
		pUIBitmap = UIResFactoryInstance->GetBitmap(m_strDownBkg.c_str());
		break;
	case BS_DISABLE:
		pUIBitmap = UIResFactoryInstance->GetBitmap(m_strDisableBkg.c_str());
		break;
	default:
		break;
	}
	if(NULL == pUIBitmap)
	{
		return ;
	}
	pUIBitmap->Render(dc, rc, TRUE);
}

void CUIButtonImagelist::Init(const LPXMLChildNodes pXMLChildNodes)
{
	XMLChildNodes::const_iterator it2 = pXMLChildNodes->find("imagelist");
	if(it2 != pXMLChildNodes->end())
	{
		LPXMLDOMNode pImageNode = it2->second;
		m_strImagelist= pImageNode->strUData;
	}
}

void CUIButtonImagelist::Render(CDCHandle dc, const RECT& rc, ButtonState brt)
{
	 //UIResFactoryInstance->
}

CUIButton::CUIButton(CUITreeContainer* pTree, LPXMLDOMNode pNode)
	: CUIControlBase(pTree, pNode), m_state(BS_NORMAL), m_pButton(NULL)
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
	XMLChildNodes::const_iterator it2 = pAttrChildNodes->find("imagelist");
	if(it2 != pAttrChildNodes->end())
	{
		m_pButton = new CUIButtonImagelist;
	}
	else
	{
		m_pButton = new CUIButtonBitmap;
	}
	if(NULL == m_pButton)
	{
		ATLASSERT(FALSE);
		return ;
	}
	m_pButton->Init(pAttrChildNodes);
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

int CUIButton::GetVisible(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushboolean(L, pThis->GetVisible());
	return 1;
}

int CUIButton::GetEnable(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushboolean(L, pThis->GetEnable());
	return 1;
}

void CUIButton::SetAttr(std::string strName, std::string strValue)
{
	__super::SetAttr(strName, strValue);
}

void CUIButton::Render(CDCHandle dc)
{
	if(NULL == m_pButton)
	{
		ATLASSERT(FALSE);
		return ;
	}
	const RECT rc = GetObjPos();
	m_pButton->Render(dc, rc, m_state);
}

void CUIButton::OnInitControl()
{

}

void CUIButton::OnDetroy()
{

}

void CUIButton::OnMouseMove(int x, int y)
{
	LOG_AUTO();
	BOOL bEnable = CUIControlBase::GetEnable();
	if(m_state == BS_NORMAL)
	{
		m_state = BS_HOVER;
		Invalidate();
	}
}

void CUIButton::OnMouseLeave(int x, int y)
{
	LOG_AUTO();
	BOOL bEnable = CUIControlBase::GetEnable();
	if(m_state == BS_HOVER)
	{
		m_state = BS_NORMAL;
		Invalidate();
	}
}

void CUIButton::OnLButtonDown(int x, int y)
{
	LOG_AUTO();
	BOOL bEnable = CUIControlBase::GetEnable();
	if(m_state == BS_HOVER)
	{
		m_state = BS_DOWN;
		Invalidate();
	}
}

void CUIButton::OnLButtonUp(int x, int y)
{
	LOG_AUTO();
	BOOL bEnable = CUIControlBase::GetEnable();
	if(m_state == BS_DOWN)
	{
		if(OnHitTest(x, y))
		{
			m_state = BS_HOVER;
		}
		else
		{
			m_state = BS_NORMAL;
		}
		Invalidate();
	}
}
