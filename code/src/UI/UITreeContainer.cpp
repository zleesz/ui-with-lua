#include "StdAfx.h"
#include "UITreeContainer.h"
#include "UIImageObject.h"
#include "UITextureObject.h"
#include "UIButton.h"
#include "UIText.h"
#include "UIWebBrowser.h"
#include "UIWindowBase.h"

CUITreeContainer::CUITreeContainer(void)
{
}

CUITreeContainer::CUITreeContainer(CUIWindowBase* p)
	: m_pBindWnd(p), 
	m_pMouseControl(NULL), 
	m_pCaptrueControl(NULL), 
	m_bTrackLeave(FALSE),
	m_pUIWindowCaption(NULL)
{
	LOG_AUTO();
	RegisterClass(this);
	m_pUIWindowCaption = new CUIWindowCaption;
}

CUITreeContainer::~CUITreeContainer(void)
{
	UnRegisterClass(this);
	if (m_pUIWindowCaption)
	{
		delete m_pUIWindowCaption;
	}
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
				if((*pAttrObj)["class"] == "UIImageObject")
				{
					pUICtrl = new CUIImageObject(this, pObjNode2);
				}
				else if((*pAttrObj)["class"] == "UITextureObject")
				{
					pUICtrl = new CUITextureObject(this, pObjNode2);
				}
				else if((*pAttrObj)["class"] == "UIButton")
				{
					pUICtrl = new CUIButton(this, pObjNode2);
				}
				else if((*pAttrObj)["class"] == "UIText")
				{
					pUICtrl = new CUIText(this, pObjNode2);
				}
				else if((*pAttrObj)["class"] == "UIWebBrowser")
				{
					pUICtrl = new CUIWebBrowser(this, pObjNode2);
				}
				else
				{
					assert(false && "unknown control class.");
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
	const char* pszID = lua_tostring(L, 2);
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
	else if(lua_isuserdata(L, 2))
	{
		pControl = (CUIControlBase*) lua_touserdata(L, 2);
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
	CUIControlBase* pControl = (CUIControlBase*) lua_touserdata(L, 2);
	if(NULL == pThis || NULL == pControl)
	{
		ATLASSERT(FALSE);
		lua_pushboolean(L, 0);
		return 1;
	}
	int nIndex = 0;
	if(lua_gettop(L) >= 3)
	{
		nIndex = (int)lua_tointeger(L, 3);
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
	CUITreeContainer* pThis = (CUITreeContainer*) lua_touserdata(L, -1);
	if(NULL == pThis)
	{
		ATLASSERT(FALSE);
		return 0;
	}
	if(lua_gettop(L) < 3)
	{
		ATLASSERT(FALSE);
		return 0;
	}
	const char* pszID = lua_tostring(L, 2);
	const char* pszClass = lua_tostring(L, 3);
	if(NULL == pszID || pszClass)
	{
		ATLASSERT(FALSE);
		return 0;
	}
	ID2ControlMap::const_iterator it = pThis->m_mapCtrl.find(pszID);
	if (it != pThis->m_mapCtrl.end())
	{
		ATLASSERT(FALSE);
		return 0;
	}
	CUIControlBase* pUICtrl = NULL;
	if(strcmp(pszClass, "UIImageObject") == 0)
	{
		pUICtrl = new CUIImageObject(pThis);
	}
	else if(strcmp(pszClass, "UITextureObject") == 0)
	{
		pUICtrl = new CUITextureObject(pThis);
	}
	else if(strcmp(pszClass, "UIButton") == 0)
	{
		pUICtrl = new CUIButton(pThis);
	}
	else if(strcmp(pszClass, "UIText") == 0)
	{
		pUICtrl = new CUIText(pThis);
	}
	else if(strcmp(pszClass, "UIWebBrowser") == 0)
	{
		pUICtrl = new CUIWebBrowser(pThis);
	}
	else
	{
		assert(false && "unknown control class.");
	}
	if(pUICtrl == NULL)
		return 0;
	pUICtrl->SetID(pszID);
	pThis->m_mapCtrl[pszID] = pUICtrl;
	UILuaPushClassObj(L, pUICtrl);
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
		pUICtrl->OnInitControl();
	}
	return 0;
}

LRESULT CUITreeContainer::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	// indexer zorder and hit test the object;
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	if (!m_bTrackLeave)
	{
		// 鼠标移入窗时，请求 WM_MOUSELEAVE 消息
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_pBindWnd->m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = HOVER_DEFAULT;
		m_bTrackLeave = TrackMouseEvent(&tme);
	}
	if(NULL != m_pCaptrueControl)
	{
		const RECT rc = m_pCaptrueControl->GetObjPos();
		m_pCaptrueControl->OnMouseMove(xPos - rc.left, yPos - rc.top);
		return 0;
	}
	/*
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
	}*/
	CUIControlBase* pControl = m_ZorderIndexer.HitTest(xPos, yPos);
	if(NULL != pControl)
	{
		if(m_pMouseControl && m_pMouseControl != pControl)
		{
			const RECT rc = m_pMouseControl->GetObjPos();
			m_pMouseControl->OnMouseLeave(xPos - rc.left, yPos - rc.top);
		}
		m_pMouseControl = pControl;
		const RECT rc = pControl->GetObjPos();
		pControl->OnMouseMove(xPos - rc.left, yPos - rc.top);
	}
	else
	{
		if(m_pMouseControl)
		{
			const RECT rc = m_pMouseControl->GetObjPos();
			m_pMouseControl->OnMouseLeave(xPos - rc.left, yPos - rc.top);
			m_pMouseControl = NULL;
		}
	}
	return 0;
}

LRESULT CUITreeContainer::OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	// indexer zorder and hit test the object;
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	if (m_pMouseControl)
	{
		const RECT rc = m_pMouseControl->GetObjPos();
		m_pMouseControl->OnMouseLeave(xPos - rc.left, yPos - rc.top);
		m_pMouseControl = NULL;
	}
	m_bTrackLeave = FALSE;
	return 0;
}

LRESULT CUITreeContainer::OnNcHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	POINT pt = {0};
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(m_pBindWnd->m_hWnd, &pt);
	CUIControlBase* pControl = m_ZorderIndexer.HitMouseEventTest(pt.x, pt.y);
	if (pControl)
	{
		bHandled = TRUE;
		return HTCLIENT;
	}
	LONG nHitTest = HTNOWHERE;
	if (m_pUIWindowCaption->OnNcHitTest(pt, nHitTest))
	{
		bHandled = TRUE;
		return nHitTest;
	}
	RECT rc = {0};
	::GetWindowRect(m_pBindWnd->m_hWnd, &rc);
	if (::PtInRect(&rc, pt))
	{
		return HTCLIENT;
	}
	return HTNOWHERE;
}

LRESULT CUITreeContainer::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	if (NULL != m_pCaptrueControl)
	{
		const RECT rc = m_pCaptrueControl->GetObjPos();
		m_pCaptrueControl->OnLButtonDown(xPos - rc.left, yPos - rc.top);
		return 0;
	}
	if (NULL != m_pMouseControl)
	{
		BOOL bHit = m_pMouseControl->OnHitTest(xPos, yPos);
		if (bHit)
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
	if (NULL != m_pCaptrueControl)
	{
		const RECT rc = m_pCaptrueControl->GetObjPos();
		m_pCaptrueControl->OnLButtonUp(xPos - rc.left, yPos - rc.top);
		return 0;
	}
	if (NULL != m_pMouseControl)
	{
		BOOL bHit = m_pMouseControl->OnHitTest(xPos, yPos);
		if (bHit)
		{
			m_pMouseControl->OnLButtonUp(xPos, yPos);
		}
	}
	return 0;
}

LRESULT CUITreeContainer::OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	LOG_DEBUG("lParam : " << (LONG)lParam);
	m_pCaptrueControl = NULL;
	return 0;
}

LRESULT CUITreeContainer::OnSetCursor(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	POINT pt = {0};
	::GetCursorPos(&pt);
	::ScreenToClient(m_pBindWnd->m_hWnd, &pt);
	CUIControlBase* pControl = m_ZorderIndexer.HitTest(pt.x, pt.y);
	if (NULL != pControl)
	{
		LRESULT lres = pControl->OnSetCursor(pt.x, pt.y);
		if (lres)
		{
			bHandled = TRUE;
		}
	}
	return 0;
}

LRESULT CUITreeContainer::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	ID2ControlMap::const_iterator it = m_mapCtrl.begin();
	for(; it != m_mapCtrl.end(); it++)
	{
		CUIControlBase* pUICtrl = it->second;
		pUICtrl->AdjustItemPos();
	}
	return 0;
}

BOOL CUITreeContainer::SetCaptureMouse(CUIControlBase* pControl, BOOL bCapture)
{
	if(bCapture)
	{
		m_pCaptrueControl = pControl;
		::SetCapture(m_pBindWnd->m_hWnd);
		return TRUE;
	}
	else
	{
		m_pCaptrueControl = NULL;
		return ::ReleaseCapture();
	}
}

void CUITreeContainer::AddCaptionRect(const RECT& rc)
{
	m_pUIWindowCaption->AddCaptionRect(rc);
}
