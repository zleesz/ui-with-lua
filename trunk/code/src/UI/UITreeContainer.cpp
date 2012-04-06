#include "StdAfx.h"
#include "UITreeContainer.h"
#include "UIImage.h"
#include "UIButton.h"

CUITreeContainer::CUITreeContainer(void)
{
}

CUITreeContainer::CUITreeContainer(CUIWindowBase* p)
	: m_pBindWnd(p), m_pMouseControl(NULL)
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
	m_ZorderIndexer.Init(&m_mapCtrl);
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
	// render objects by z-orders
	m_ZorderIndexer.Render(dc);
}

CUIWindowBase* CUITreeContainer::GetBindWnd(void)
{
	return m_pBindWnd;
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
	UILuaPushClassObj(L, (const void*)pThis->GetBindWnd());
	return 1;
}

int CUITreeContainer::RemoveUIObject(lua_State* L)
{
	CUITreeContainer* pThis = (CUITreeContainer*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	CUIControlBase* pControl = NULL;
	if(lua_isstring(L, -2))
	{
		const char* pszID = lua_tostring(L, -2);
		ID2ControlMap::iterator it = pThis->m_mapCtrl.find(pszID);
		if(it == pThis->m_mapCtrl.end())
		{
			ATLASSERT(FALSE);
			lua_pushboolean(L, 0);
			return 1;
		}
		pControl = it->second;
		pThis->m_mapCtrl.erase(it);
	}
	else if(lua_isuserdata(L, -2))
	{
		pControl = (CUIControlBase*) lua_touserdata(L, -2);
		const std::string strID = pControl->GetID();
		ID2ControlMap::iterator it = pThis->m_mapCtrl.find(strID);
		if(it == pThis->m_mapCtrl.end())
		{
			ATLASSERT(FALSE);
			lua_pushboolean(L, 0);
			return 1;
		}
		pThis->m_mapCtrl.erase(it);
	}
	else
	{
		ATLASSERT(FALSE);
		lua_pushboolean(L, 0);
		return 1;
	}
	if(NULL == pControl)
	{
		ATLASSERT(FALSE);
		lua_pushboolean(L, 0);
		return 1;
	}
	TreeModifyData tmd;
	tmd.t = TMT_DELETE;
	tmd.pControl = pControl;
	pThis->OnTreeModify(&tmd);
	pControl->OnDetroy();
	delete pControl;
	pControl = NULL;
	lua_pushboolean(L, 1);
	return 1;
}

int CUITreeContainer::AddUIObject(lua_State* L)
{
	CUITreeContainer* pThis = (CUITreeContainer*) lua_touserdata(L, -1);
	CUIControlBase* pControl = (CUIControlBase*) lua_touserdata(L, -2);
	if(NULL == pThis || NULL == pControl)
	{
		ATLASSERT(FALSE);
		lua_pushboolean(L, 0);
		return 1;
	}
	int nIndex = 0;
	if(lua_gettop(L) >= 3)
	{
		nIndex = lua_tointeger(L, -3);
	}
	pThis->m_mapCtrl.insert(ID2ControlMap::value_type(pControl->GetID(), pControl));
	TreeModifyData tmd;
	tmd.t = TMT_ADD;
	tmd.pControl = pControl;
	pThis->OnTreeModify(&tmd);
	lua_pushboolean(L, 1);
	return 1;
}

int CUITreeContainer::CreateUIObject(lua_State* L)
{
	return 1;
}

void CUITreeContainer::OnTreeModify(const LPTreeModifyData ptmt)
{
	// 
	m_ZorderIndexer.OnTreeModify(ptmt);
}

LRESULT CUITreeContainer::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	ID2ControlMap::const_iterator it = m_mapCtrl.begin();
	for(; it != m_mapCtrl.end(); it++)
	{
		CUIControlBase* pUICtrl = it->second;
	}
	return 0;
}

LRESULT CUITreeContainer::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	// indexer zorder and hit test the object;
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	if(NULL != m_pMouseControl)
	{
		const RECT rc = m_pMouseControl->GetObjPos();
		BOOL bHit = m_pMouseControl->OnHitTest(xPos, yPos);
		if(bHit)
		{
			m_pMouseControl->OnMouseMove(xPos - rc.left, yPos - rc.top);
			return 0;
		}
		else
		{
			m_pMouseControl->OnMouseLeave(xPos - rc.left, yPos - rc.top);
			m_pMouseControl = NULL;
		}
	}
	CUIControlBase* pControl = m_ZorderIndexer.HitTest(xPos, yPos);
	if(NULL != pControl)
	{
		const RECT rc = pControl->GetObjPos();
		pControl->OnMouseMove(xPos - rc.left, yPos - rc.top);
		m_pMouseControl = pControl;
	}
	return 0;
}

LRESULT CUITreeContainer::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	if(NULL != m_pMouseControl)
	{
		BOOL bHit = m_pMouseControl->OnHitTest(xPos, yPos);
		if(bHit)
		{
			m_pMouseControl->OnLButtonDown(xPos, yPos);
		}
	}
	return 0;
}

LRESULT CUITreeContainer::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	if(NULL != m_pMouseControl)
	{
		BOOL bHit = m_pMouseControl->OnHitTest(xPos, yPos);
		if(bHit)
		{
			m_pMouseControl->OnLButtonUp(xPos, yPos);
		}
	}
	return 0;
}
