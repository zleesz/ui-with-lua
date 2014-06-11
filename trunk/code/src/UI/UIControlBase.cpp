#include "StdAfx.h"
#include "UIControlBase.h"
#include "UITreeContainer.h"
#include "UIWindowBase.h"
#include <MathExpParser.h>

CUIControlBase::CUIControlBase(void)
{
	ZeroMemory((void*)&m_rc, sizeof(m_rc));
}

CUIControlBase::CUIControlBase(CUITreeContainer* pTree, LPXMLDOMNode pNode)
	: m_pTree(pTree), m_pUIEventControl(NULL)
{
	ZeroMemory((void*)&m_rc, sizeof(m_rc));
	if(pNode == NULL || pNode->pMapAttr == NULL)
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	SetID((*pMapAttr)["id"]);
	m_pUIEventControl = new CUIEventCtrlContainer;
	LPXMLChildNodes pChildNodes = pNode->pMapChildNode;
	if(pChildNodes == NULL)
		return;
	XMLChildNodes::const_iterator it = pChildNodes->find("attr");
	if(it != pChildNodes->end())
	{
		LPXMLDOMNode pAttrNode = it->second;
		if(pAttrNode != NULL)
		{
			LPXMLChildNodes pAttrChildNodes = pAttrNode->pMapChildNode;
			if(pAttrChildNodes != NULL)
			{
				XMLChildNodes::const_iterator it2 = pAttrChildNodes->begin();
				for(; it2 != pAttrChildNodes->end(); it2++)
				{
					SetAttr(it2->first, it2->second->strUData);
				}
			}
		}
	}
	LPXMLDOMNode pEventNode = (*pChildNodes)["eventlist"];
	if(pEventNode != NULL && pEventNode->pMapChildNode != NULL)
	{
		m_pUIEventControl->ParserEvent(pEventNode);
	}
	m_pUIEventControl->OnBindEvent(m_pTree->GetBindWnd()->GetXMLPath());
}

CUIControlBase::CUIControlBase(CUITreeContainer* pTree)
: m_pTree(pTree), m_pUIEventControl(NULL)
{
	ZeroMemory((void*)&m_rc, sizeof(m_rc));
	m_pUIEventControl = new CUIEventCtrlContainer;
}

CUIControlBase::~CUIControlBase(void)
{
}

void CUIControlBase::SetAttr(const std::string& strName, const std::string& strValue)
{
	if (strName == "left" ||
		strName == "top" ||
		strName == "width" ||
		strName == "height")
	{
		m_mapAttr[strName] = CComVariant(strValue.c_str());
		AdjustItemPos(FALSE);
	}
	else if (strName == "zorder")
	{
		m_mapAttr[strName] = CComVariant(atoi(strValue.c_str()));
	}
	else if (strName == "visible")
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
	else if (strName == "cursor")
	{
		m_mapAttr[strName] = CComVariant(strValue.c_str());
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

std::string CUIControlBase::SetID(const std::string& strID)
{
	std::string strOldID = m_strID;
	m_strID = strID;
	return strOldID;
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

void CUIControlBase::TranslateFatherPos(std::wstring& strPos, const RECT& fatherRc)
{
	wchar_t szFatherLeft[10] = {0};
	wchar_t szFatherTop[10] = {0};
	wchar_t szFatherWidth[10] = {0};
	wchar_t szFatherHeight[10] = {0};

	_itow(fatherRc.left, szFatherLeft, 10);
	_itow(fatherRc.left, szFatherTop, 10);
	_itow(fatherRc.right - fatherRc.left, szFatherWidth, 10);
	_itow(fatherRc.bottom - fatherRc.top, szFatherHeight, 10);

	Util::ReplaceAll(strPos, L"father.left", szFatherLeft);
	Util::ReplaceAll(strPos, L"father.top", szFatherTop);
	Util::ReplaceAll(strPos, L"father.width", szFatherWidth);
	Util::ReplaceAll(strPos, L"father.height", szFatherHeight);
}

void CUIControlBase::AdjustItemPos(BOOL bFire)
{
	CComVariant vLeft, vTop, vWidth, vHeight;
	GetAttr("left", &vLeft);
	GetAttr("top", &vTop);
	GetAttr("width", &vWidth);
	GetAttr("height", &vHeight);

	RECT oldRc = m_rc;
	RECT wndRc = {0};
	m_pTree->GetBindWnd()->GetWindowRect(&wndRc);
	if (vLeft.vt == VT_BSTR)
	{
		std::wstring strLeft(vLeft.bstrVal);
		TranslateFatherPos(strLeft, wndRc);
		CMathExpParser mathExpParser;
		m_rc.left = mathExpParser.Calc(strLeft.c_str());
	}
	if (vTop.vt == VT_BSTR)
	{
		std::wstring strTop(vTop.bstrVal);
		TranslateFatherPos(strTop, wndRc);
		CMathExpParser mathExpParser;
		m_rc.top = mathExpParser.Calc(strTop.c_str());
	}
	if (vWidth.vt == VT_BSTR)
	{
		std::wstring strWidth(vWidth.bstrVal);
		TranslateFatherPos(strWidth, wndRc);
		CMathExpParser mathExpParser;
		m_rc.right = m_rc.left + mathExpParser.Calc(strWidth.c_str());
	}
	if (vHeight.vt == VT_BSTR)
	{
		std::wstring strHeight(vHeight.bstrVal);
		TranslateFatherPos(strHeight, wndRc);
		CMathExpParser mathExpParser;
		m_rc.bottom = m_rc.top + mathExpParser.Calc(strHeight.c_str());
	}
	if (bFire && !::EqualRect(&oldRc, &m_rc))
	{
		OnPosChange(m_rc.left, m_rc.top, m_rc.right, m_rc.bottom, oldRc.left, oldRc.top, oldRc.right, oldRc.bottom);
		FirePosChange(m_rc.left, m_rc.top, m_rc.right, m_rc.bottom, oldRc.left, oldRc.top, oldRc.right, oldRc.bottom);
	}
}

const RECT& CUIControlBase::GetObjPos()
{
	return m_rc;
}

void CUIControlBase::SetObjPos(const RECT& rc)
{
	char szLeft[10] = {0};
	char szTop[10] = {0};
	char szWidth[10] = {0};
	char szHeight[10] = {0};

	_itoa(rc.left, szLeft, 10);
	_itoa(rc.top, szTop, 10);
	_itoa(rc.right - rc.left, szWidth, 10);
	_itoa(rc.bottom - rc.top, szHeight, 10);

	const RECT oldRc = GetObjPos();

	SetAttr("left", szLeft);
	SetAttr("top", szTop);
	SetAttr("width", szWidth);
	SetAttr("height", szHeight);

	AdjustItemPos();
	RECT unionRc = {0};
	::UnionRect(&unionRc, &rc, &oldRc);
	InvalidateRect(unionRc);
}

BOOL CUIControlBase::OnHitTest(int x, int y)
{
	const RECT rc = GetObjPos();
	if(x > rc.left && x < rc.right
		&& y > rc.top && y < rc.bottom)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CUIControlBase::OnHitMouseEventTest(int x, int y)
{
	const RECT rc = GetObjPos();
	if(x > rc.left && x < rc.right
		&& y > rc.top && y < rc.bottom)
	{
		if (m_pUIEventControl->IsAttachedMouseEvent())
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CUIControlBase::OnPosChange(int left, int top, int right, int bottom, int oldLeft, int oldTop, int oldRight, int oldBottom)
{

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

void CUIControlBase::SetVisible(BOOL bVisible)
{
	SetAttr("visible", bVisible ? "true" : "false");
	Invalidate();
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

void CUIControlBase::Invalidate()
{
	if(NULL == m_pTree)
		return;
	CUIWindowBase* pWindow = m_pTree->GetBindWnd();
	if(NULL == pWindow)
		return;
	const RECT rc = GetObjPos();
	pWindow->InvalidateRect(&rc, FALSE);
}

void CUIControlBase::InvalidateRect(const RECT& rc)
{
	if(NULL == m_pTree)
		return;
	CUIWindowBase* pWindow = m_pTree->GetBindWnd();
	if(NULL == pWindow)
		return;
	pWindow->InvalidateRect(&rc, FALSE);
}

void CUIControlBase::SetCaptureMouse(BOOL bCapture)
{
	if(NULL == m_pTree)
		return;
	m_pTree->SetCaptureMouse(this, bCapture);
}

void CUIControlBase::OnInitControl()
{
	
}

void CUIControlBase::OnDetroy()
{
	
}

void CUIControlBase::OnLButtonDown(int x, int y)
{
	FireMouseEvent("OnLButtonDown", x, y);
}

void CUIControlBase::OnLButtonUp(int x, int y)
{
	FireMouseEvent("OnLButtonUp", x, y);
}

void CUIControlBase::OnLButtonDbClick(int x, int y)
{
	FireMouseEvent("OnLButtonDbClick", x, y);
}

void CUIControlBase::OnMouseMove(int x, int y)
{
	FireMouseEvent("OnMouseMove", x, y);
}

void CUIControlBase::OnMouseLeave(int x, int y)
{
	FireMouseEvent("OnMouseLeave", x, y);
}

void CUIControlBase::OnMouseWheel(int x, int y)
{
	FireMouseEvent("OnMouseWheel", x, y);
}

LRESULT CUIControlBase::OnSetCursor(int /*x*/, int /*y*/)
{
	CComVariant vCursor;
	GetAttr("cursor", &vCursor);
	if (vCursor.vt != VT_BSTR)
	{
		return FALSE;
	}
	if (wcscmp(vCursor.bstrVal, L"IDC_HAND") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		return TRUE;
	}
	else if (wcscmp(vCursor.bstrVal, L"IDC_ARROW") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		return TRUE;
	}
	else if (wcscmp(vCursor.bstrVal, L"IDC_IBEAM") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
		return TRUE;
	}
	else if (wcscmp(vCursor.bstrVal, L"IDC_WAIT") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_WAIT)));
		return TRUE;
	}
	else if (wcscmp(vCursor.bstrVal, L"IDC_CROSS") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS)));
		return TRUE;
	}
	else if (wcscmp(vCursor.bstrVal, L"IDC_UPARROW") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_UPARROW)));
		return TRUE;
	}
	else if (wcscmp(vCursor.bstrVal, L"IDC_SIZE") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZE)));
		return TRUE;
	}
	else if (wcscmp(vCursor.bstrVal, L"IDC_ICON") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ICON)));
		return TRUE;
	}
	else if (wcscmp(vCursor.bstrVal, L"IDC_SIZENWSE") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
		return TRUE;
	}
	else if (wcscmp(vCursor.bstrVal, L"IDC_SIZENESW") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
		return TRUE;
	}
	else if (wcscmp(vCursor.bstrVal, L"IDC_SIZEWE") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
		return TRUE;
	}
	else if (wcscmp(vCursor.bstrVal, L"IDC_SIZENS") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
		return TRUE;
	}
	else if (wcscmp(vCursor.bstrVal, L"IDC_SIZEALL") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL)));
		return TRUE;
	}
	else if (wcscmp(vCursor.bstrVal, L"IDC_NO") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_NO)));
		return TRUE;
	}
	else if (wcscmp(vCursor.bstrVal, L"IDC_APPSTARTING") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_APPSTARTING)));
		return TRUE;
	}
	else if (wcscmp(vCursor.bstrVal, L"IDC_HELP") == 0)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HELP)));
		return TRUE;
	}
	return FALSE;
}

void CUIControlBase::FireMouseEvent(std::string strName, int x, int y)
{
	LOG_AUTO();
	CComVariant avarParams[3];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)this;
	avarParams[1] = x;
	avarParams[2] = y;

	UIDISPPARAMS params = { avarParams, strName.c_str(), 3, 0 };
	m_pUIEventControl->DispatchListener(params);
}

void CUIControlBase::FirePosChange(int left, int top, int right, int bottom, int oldLeft, int oldTop, int oldRight, int oldBottom)
{
	CComVariant avarParams[9];
	avarParams[0].vt = VT_BYREF | VT_I4;
	avarParams[0].lVal = (LONG)(LONG_PTR)this;
	avarParams[1] = left;
	avarParams[2] = top;
	avarParams[3] = right;
	avarParams[4] = bottom;
	avarParams[5] = oldLeft;
	avarParams[6] = oldTop;
	avarParams[7] = oldRight;
	avarParams[8] = oldBottom;

	UIDISPPARAMS params = { avarParams, "OnPosChange", 9, 0 };
	m_pUIEventControl->DispatchListener(params);
}

int CUIControlBase::GetID(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	std::string strID = pThis->GetID();
	lua_pushstring(L, strID.c_str());
	return 1;
}

int CUIControlBase::GetVisible(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushboolean(L, pThis->GetVisible());
	return 1;
}

int CUIControlBase::SetVisible(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	BOOL bVisible = (BOOL)lua_toboolean(L, -2);
	pThis->SetVisible(bVisible);
	return 0;
}

int CUIControlBase::GetEnable(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushboolean(L, pThis->GetEnable());
	return 1;
}

int CUIControlBase::GetOwnerTree(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	UILuaPushClassObj(L, pThis->GetOwnerTree());
	return 1;
}

int CUIControlBase::SetObjPos(lua_State* L)
{
	CUIControlBase* pThis = (CUIControlBase*) lua_touserdata(L, -1);
	LONG lnLeft = (LONG)lua_tonumber(L, 2);
	LONG lnTop = (LONG)lua_tonumber(L, 3);
	LONG lnWidth = (LONG)lua_tonumber(L, 4);
	LONG lnHeight = (LONG)lua_tonumber(L, 5);

	RECT rc = {lnLeft, lnTop, lnLeft + lnWidth, lnTop + lnHeight};
	pThis->SetObjPos(rc);
	return 0;
}
