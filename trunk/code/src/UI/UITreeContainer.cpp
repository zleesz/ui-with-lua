#include "StdAfx.h"
#include "UITreeContainer.h"
#include "UIImageObject.h"
#include "UITextureObject.h"
#include "UIButton.h"
#include "UIText.h"
#include "UIWebBrowser.h"
#include "UICaption.h"
#include "UIEdit.h"
#include "UIWindowBase.h"

CUITreeContainer::CUITreeContainer(void) : 
	m_pMouseControl(NULL), 
	m_pCaptrueControl(NULL), 
	m_pFocusControl(NULL), 
	m_bTrackLeave(FALSE)
{
	RegisterClass(this);
}

CUITreeContainer::CUITreeContainer(CUIWindowBase* p) :
	m_pBindWnd(p), 
	m_pMouseControl(NULL), 
	m_pCaptrueControl(NULL), 
	m_pFocusControl(NULL), 
	m_bTrackLeave(FALSE)
{
	LOG_AUTO();
	RegisterClass(this);
}

CUITreeContainer::~CUITreeContainer(void)
{
	UnRegisterClass(this);
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
				if ((*pAttrObj)["class"] == "UIImageObject")
				{
					pUICtrl = new CUIImageObject(this, pObjNode2);
				}
				else if ((*pAttrObj)["class"] == "UITextureObject")
				{
					pUICtrl = new CUITextureObject(this, pObjNode2);
				}
				else if ((*pAttrObj)["class"] == "UIButton")
				{
					pUICtrl = new CUIButton(this, pObjNode2);
				}
				else if ((*pAttrObj)["class"] == "UIText")
				{
					pUICtrl = new CUIText(this, pObjNode2);
				}
				else if ((*pAttrObj)["class"] == "UIWebBrowser")
				{
					pUICtrl = new CUIWebBrowser(this, pObjNode2);
				}
				else if ((*pAttrObj)["class"] == "UICaption")
				{
					pUICtrl = new CUICaption(this, pObjNode2);
				}
				else if ((*pAttrObj)["class"] == "UIEdit")
				{
					pUICtrl = new CUIEdit(this, pObjNode2);
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
	CUITreeContainer** ppThis = (CUITreeContainer**)luaL_checkudata(L, -1, GetRigisterClassName());
	CUITreeContainer* pThis = *ppThis;
	if (!pThis)
	{
		ATLASSERT(pThis);
		return 0;
	}
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
	CUITreeContainer** ppThis = (CUITreeContainer**)luaL_checkudata(L, -1, GetRigisterClassName());
	CUITreeContainer* pThis = *ppThis;
	if (!pThis)
	{
		ATLASSERT(pThis);
		return 0;
	}
	UILuaPushClassObj(L, (const void*)pThis->GetBindWnd());
	return 1;
}

int CUITreeContainer::RemoveUIObject(lua_State* L)
{
	CUITreeContainer** ppThis = (CUITreeContainer**)luaL_checkudata(L, -1, GetRigisterClassName());
	CUITreeContainer* pThis = *ppThis;
	if (!pThis)
	{
		ATLASSERT(pThis);
		return 0;
	}
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
		CUIControlBase** ppControl = (CUIControlBase**) lua_touserdata(L, 2);
		pControl = *ppControl;
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
	CUITreeContainer** ppThis = (CUITreeContainer**)luaL_checkudata(L, -1, GetRigisterClassName());
	CUITreeContainer* pThis = *ppThis;
	if (!pThis)
	{
		ATLASSERT(pThis);
		lua_pushboolean(L, 0);
		return 1;
	}
	CUIControlBase** ppControl = (CUIControlBase**) lua_touserdata(L, 2);
	CUIControlBase* pControl = *ppControl;
	if(NULL == pControl)
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
	CUITreeContainer** ppThis = (CUITreeContainer**)luaL_checkudata(L, -1, GetRigisterClassName());
	CUITreeContainer* pThis = *ppThis;
	if (!pThis)
	{
		ATLASSERT(pThis);
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
	else if (strcmp(pszClass, "UICaption") == 0)
	{
		pUICtrl = new CUICaption(pThis);
	}
	else if (strcmp(pszClass, "UIEdit") == 0)
	{
		pUICtrl = new CUIEdit(pThis);
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
	LONG nHitTest = 0;
	CUIControlBase* pControl = m_ZorderIndexer.HitTest(xPos, yPos, nHitTest);
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
	LONG nHitTest = 0;
	CUIControlBase* pControl = m_ZorderIndexer.HitTest(pt.x, pt.y, nHitTest);
	if (pControl)
	{
		bHandled = TRUE;
		return nHitTest;
	}
	RECT rc = {0};
	::GetWindowRect(m_pBindWnd->m_hWnd, &rc);
	if (::PtInRect(&rc, pt))
	{
		bHandled = TRUE;
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
	if (m_pFocusControl && m_pFocusControl != m_pMouseControl)
	{
		m_pFocusControl->OnSetFocus(FALSE);
		m_pFocusControl = NULL;
	}
	if (NULL != m_pMouseControl)
	{
		LONG nHitTest = 0;
		BOOL bHit = m_pMouseControl->OnHitTest(xPos, yPos, nHitTest);
		if (bHit)
		{
			if (m_pFocusControl != m_pMouseControl)
			{
				m_pFocusControl = m_pMouseControl;
				m_pFocusControl->OnSetFocus(TRUE);
			}
			const RECT rc = m_pMouseControl->GetObjPos();
			m_pMouseControl->OnLButtonDown(xPos - rc.left, yPos - rc.top);
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
		LONG nHitTest = 0;
		BOOL bHit = m_pMouseControl->OnHitTest(xPos, yPos, nHitTest);
		if (bHit)
		{
			const RECT rc = m_pMouseControl->GetObjPos();
			m_pMouseControl->OnLButtonUp(xPos - rc.left, yPos - rc.top);
		}
	}
	return 0;
}

LRESULT CUITreeContainer::OnLButtonDbClick(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	if (NULL != m_pCaptrueControl)
	{
		const RECT rc = m_pCaptrueControl->GetObjPos();
		m_pCaptrueControl->OnLButtonDbClick(xPos - rc.left, yPos - rc.top);
		return 0;
	}
	if (NULL != m_pMouseControl)
	{
		LONG nHitTest = 0;
		BOOL bHit = m_pMouseControl->OnHitTest(xPos, yPos, nHitTest);
		if (bHit)
		{
			const RECT rc = m_pMouseControl->GetObjPos();
			m_pMouseControl->OnLButtonDbClick(xPos - rc.left, yPos - rc.top);
		}
	}
	return 0;
}

LRESULT CUITreeContainer::OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if (m_pFocusControl)
	{
		BOOL bAlt = ::GetKeyState(VK_MENU) < 0;
		BOOL bCtrl = ::GetKeyState(VK_CONTROL) < 0;
		BOOL bShift = ::GetKeyState(VK_SHIFT) < 0;
		m_pFocusControl->OnKeyDown(wParam, bAlt, bCtrl, bShift, lParam & 0xFF);
	}
	return 0;
}

LRESULT CUITreeContainer::OnKeyUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if (m_pFocusControl)
	{
		BOOL bAlt = ::GetKeyState(VK_MENU) < 0;
		BOOL bCtrl = ::GetKeyState(VK_CONTROL) < 0;
		BOOL bShift = ::GetKeyState(VK_SHIFT) < 0;
		m_pFocusControl->OnKeyUp(wParam, bAlt, bCtrl, bShift, lParam & 0xFF);
	}
	return 0;
}

LRESULT CUITreeContainer::OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	wchar_t c = wchar_t(wParam);
	if (m_pFocusControl)
	{
		m_pFocusControl->OnChar(c, lParam & 0xFF);
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
	LONG nHitTest = 0;
	CUIControlBase* pControl = m_ZorderIndexer.HitTest(pt.x, pt.y, nHitTest);
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

LRESULT CUITreeContainer::OnSetFocus(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	if (m_pFocusControl)
	{
		m_pFocusControl->OnSetFocus(TRUE);
	}
	return 0;
}

LRESULT CUITreeContainer::OnKillFocus(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	if (m_pFocusControl)
	{
		m_pFocusControl->OnSetFocus(FALSE);
	}
	if (m_pCaptrueControl)
	{
		m_pCaptrueControl = NULL;
	}
	return 0;
}

LRESULT CUITreeContainer::OnImeRequest(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == IMR_QUERYCHARPOSITION)
	{
		IMECHARPOSITION* pImeChPos = (IMECHARPOSITION*)lParam;
		pImeChPos->pt.x = 100;
		pImeChPos->pt.y = 100;
		pImeChPos->cLineHeight = 30;
		pImeChPos->dwCharPos = 3;
		pImeChPos->dwSize = sizeof(IMECHARPOSITION);
		RECT rc = {0};
		m_pBindWnd->GetWindowRect(&rc);
		pImeChPos->rcDocument.left = rc.left;
		pImeChPos->rcDocument.top = rc.top;
		pImeChPos->rcDocument.right = rc.right;
		pImeChPos->rcDocument.bottom = rc.bottom;
		return sizeof(IMECHARPOSITION);
	}
	return 0;
}

BOOL CUITreeContainer::SetCaptureMouse(CUIControlBase* pControl, BOOL bCapture)
{
	if (bCapture)
	{
		m_pCaptrueControl = pControl;
		::SetCapture(m_pBindWnd->m_hWnd);
		return TRUE;
	}
	else if (m_pCaptrueControl)
	{
		m_pCaptrueControl = NULL;
		return ::ReleaseCapture();
	}
	return FALSE;
}

CUIControlBase* CUITreeContainer::GetCaptureMouse()
{
	return m_pCaptrueControl;
}

BOOL CUITreeContainer::SetFocusControl(CUIControlBase* pControl, BOOL bFocus)
{
	if (bFocus)
	{
		if (m_pFocusControl == pControl)
		{
			return TRUE;
		}
		if (!m_pBindWnd->IsWindow() || m_pBindWnd->m_hWnd != ::GetFocus())
		{
			// 当前窗口无焦点，不触发控件事件。
			m_pFocusControl = pControl;
			return FALSE;
		}
		if (m_pFocusControl)
		{
			m_pFocusControl->OnSetFocus(FALSE);
		}
		m_pFocusControl = pControl;
		m_pFocusControl->OnSetFocus(TRUE);
	}
	else if (m_pFocusControl)
	{
		if (m_pFocusControl != pControl)
		{
			return TRUE;
		}
		if (!m_pBindWnd->IsWindow() || m_pBindWnd->m_hWnd != ::GetFocus())
		{
			m_pFocusControl = NULL;
			return FALSE;
		}
		CUIControlBase* pFocusCtrl = m_pFocusControl;
		m_pFocusControl = NULL;
		pFocusCtrl->OnSetFocus(FALSE);
	}
	return FALSE;
}

CUIControlBase* CUITreeContainer::GetFocusControl()
{
	if (!m_pBindWnd->IsWindow() || m_pBindWnd->m_hWnd != ::GetFocus())
	{
		return NULL;
	}
	return m_pFocusControl;
}