#include "StdAfx.h"
#include "UITreeContainer.h"
#include "UIImage.h"
#include "UIButton.h"

CUITreeContainer::CUITreeContainer(void)
{
}

CUITreeContainer::CUITreeContainer(CUIWindowBase* p)
	: m_pBindWnd(p)
{
	LOG_AUTO();
	RegisterClass(this);
}

CUITreeContainer::~CUITreeContainer(void)
{
}

BOOL CUITreeContainer::ParserUITree(LPXMLDOMNode pNode)
{
	if(NULL == pNode || pNode->strName != "uitree")
	{
		return FALSE;
	}
	LPXMLChildNodes pChildNodes = pNode->pMapChildNode;
	if(pChildNodes == NULL)
		return FALSE;
	XMLChildNodes::const_iterator it = pChildNodes->find("obj");
	if(it != pChildNodes->end())
	{
		LPXMLDOMNode pObjNode = it->second;
		if(pObjNode && pObjNode->pVecNode)
		{
			LPXMLVecNodes pVecNodes = pObjNode->pVecNode;
			XMLVecNodes::const_iterator itVec = pVecNodes->begin();
			for(; itVec != pVecNodes->end(); itVec++)
			{
				LPXMLDOMNode pObjNode2 = *itVec;
				LPXMLAttrMap pAttrObj = pObjNode2->pMapAttr;
				if(pAttrObj == NULL)
					continue;
				CUIControlBase* pUICtrl = NULL;
				if((*pAttrObj)["class"] == "UIImage")
				{
					pUICtrl = new CUIImage(this, pObjNode2);
				}
				else if((*pAttrObj)["class"] == "UIButton")
				{
					pUICtrl = new CUIButton(this, pObjNode2);
				}
				if(pUICtrl == NULL)
					continue;
				std::string strID = pUICtrl->GetID();
				if(strID.length() <= 0)
				{
					ATLASSERT(false && "object must has id!");
					delete pUICtrl;
					pUICtrl = NULL;
					continue;
				}
				m_mapCtrl[strID] = pUICtrl;
			}
		}
	}
	return TRUE;
}

BOOL CUITreeContainer::OnPreCreate()
{
	return TRUE;
}

BOOL CUITreeContainer::OnCreate()
{
	return TRUE;
}

void CUITreeContainer::Render(CDCHandle dc)
{
	ID2ControlMap::const_iterator it = m_mapCtrl.begin();
	for(; it != m_mapCtrl.end(); it++)
	{
		CUIControlBase* pUICtrl = it->second;
		pUICtrl->Render(dc);
	}
}

int CUITreeContainer::GetUIObject(lua_State* L)
{
	CUITreeContainer* pThis = (CUITreeContainer*) lua_touserdata(L, -1);
	const char* pszID = lua_tostring(L, -2);
	if(pszID == NULL)
	{
		assert(false);
		return 0;
	}
	ID2ControlMap::const_iterator it = pThis->m_mapCtrl.find(pszID);
	if(it == pThis->m_mapCtrl.end())
	{
		lua_pushnil(L);
	}
	else
	{
		UILuaPushClassObj(L, it->second);
	}
	return 1;
}

int CUITreeContainer::GetOwnerWnd(lua_State* L)
{
	CUITreeContainer* pThis = (CUITreeContainer*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	UILuaPushClassObj(L, (const void*)pThis->m_pBindWnd);
	return 1;
}

LRESULT CUITreeContainer::OnCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	ID2ControlMap::const_iterator it = m_mapCtrl.begin();
	for(; it != m_mapCtrl.end(); it++)
	{
		CUIControlBase* pUICtrl = it->second;
	}
	return 0;
}

LRESULT CUITreeContainer::OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	int xPos = GET_X_LPARAM(lParam); 
	int yPos = GET_Y_LPARAM(lParam); 
	
	return 0;
}