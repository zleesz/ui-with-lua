#include "StdAfx.h"
#include "UISimpleEdit.h"

CUISimpleEdit::CUISimpleEdit(void)
{
	m_bPassword = FALSE;
	RegisterClass(this);
}

CUISimpleEdit::~CUISimpleEdit(void)
{
	UnRegisterClass(this);
}

CUISimpleEdit::CUISimpleEdit(CUITreeContainer* pTree, LPXMLDOMNode pNode)
: CUIControlBase(pTree, pNode)
{
	m_bPassword = FALSE;
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
		std::string strText = it2->second->strUData;
		Util::UTF8_to_Unicode(strText.c_str(), m_strText);
	}
	it2 = pAttrChildNodes->find("color");
	if (it2 != pAttrChildNodes->end())
	{
		m_strColorID = it2->second->strUData;
	}
	it2 = pAttrChildNodes->find("font");
	if (it2 != pAttrChildNodes->end())
	{
		m_strFontID = it2->second->strUData;
	}
	it2 = pAttrChildNodes->find("password");
	if (it2 != pAttrChildNodes->end())
	{
		std::string strPwd = it2->second->strUData;
		m_bPassword = (strPwd == "1" || strPwd == "true");
	}
}

CUISimpleEdit::CUISimpleEdit(CUITreeContainer* pTree)
: CUIControlBase(pTree)
{
	RegisterClass(this);
}

void CUISimpleEdit::Render(CDCHandle dc)
{
	RECT rc = GetObjPos();
	int oldBkMode = dc.SetBkMode(TRANSPARENT);
	CUIFont* pFont = UIResFactoryInstance->GetFont(m_strFontID.c_str());
	if (pFont)
	{
		pFont->SelectObject(dc);
	}
	CUIColor* pColor = UIResFactoryInstance->GetColor(m_strColorID.c_str());
	if (pColor)
	{
		pColor->SelectObject(dc);
	}
	dc.DrawText(m_strText.c_str(), -1, &rc, DT_CALCRECT | DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	dc.DrawText(m_strText.c_str(), -1, &rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	if (pFont)
	{
		pFont->RestoreObject(dc);
	}
	if (pColor)
	{
		pColor->RestoreObject(dc);
	}
	dc.SetBkMode(oldBkMode);
}

void CUISimpleEdit::OnInitControl()
{
}

void CUISimpleEdit::OnDetroy()
{
}

void CUISimpleEdit::SetAttr(const std::string& strName, const std::string& strValue)
{
	if (_SetAttr(strName, strValue))
	{
		return;
	}
	if (strName == "text")
	{
		Util::UTF8_to_Unicode(strValue.c_str(), m_strText);
	}
	else if (strName == "color")
	{
		m_strColorID = strValue;
	}
	else if (strName == "font")
	{
		m_strFontID = strValue;
	}
	else if (strName == "password")
	{
		m_bPassword = (strValue == "1" || strValue == "true");
	}
}

void CUISimpleEdit::CaclTextRect(RECT& rc)
{
	rc = GetObjPos();
	CUIWindowBase* pWindow = GetOwnerTree()->GetBindWnd();
	CDCHandle dc = ::GetDC(pWindow->m_hWnd);
	CUIFont* pFont = UIResFactoryInstance->GetFont(m_strFontID.c_str());
	if (pFont)
	{
		pFont->SelectObject(dc);
	}
	CUIColor* pColor = UIResFactoryInstance->GetColor(m_strColorID.c_str());
	if (pColor)
	{
		pColor->SelectObject(dc);
	}
	dc.DrawText(m_strText.c_str(), -1, &rc, DT_CALCRECT | DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	if (pFont)
	{
		pFont->RestoreObject(dc);
	}
	if (pColor)
	{
		pColor->RestoreObject(dc);
	}
}

void CUISimpleEdit::SetVisible(BOOL bVisible)
{
	SetAttr("visible", bVisible ? "true" : "false");
	Invalidate();
}

void CUISimpleEdit::OnSetFocus(BOOL bFocus)
{
	LOG_AUTO();
	if (bFocus)
	{
		CUIWindowBase* pWindow = GetOwnerTree()->GetBindWnd();
		LONG lnWidth = 0, lnHeight = 0;
		GetTextExtent(lnWidth, lnHeight);
		pWindow->CreateSolidCaret(1, lnHeight);
		if (GetEnable())
		{
			pWindow->ShowCaret();
		}
	}
	else
	{
		::DestroyCaret();
	}
	CUIControlBase::FireOnSetFocusEvent(bFocus);
}

void CUISimpleEdit::OnLButtonDown(int x, int y)
{
	RECT rc = {0};
	CaclTextRect(rc);
	::SetCaretPos(rc.left, rc.top);
}

LRESULT CUISimpleEdit::OnSetCursor(int x, int y)
{
	BOOL bEnable = CUIControlBase::GetEnable();
	if (bEnable)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
		return TRUE;
	}
	return FALSE;
}

void CUISimpleEdit::GetText(std::wstring& strText)
{
	strText = m_strText;
}

std::string CUISimpleEdit::SetText(const std::string& strText)
{
	std::wstring wstrTmp(m_strText);
	Util::UTF8_to_Unicode(strText.c_str(), m_strText);
	Invalidate();
	std::string strTmp;
	Util::Unicode_to_UTF8(wstrTmp.c_str(), strTmp);
	return strTmp;
}

void CUISimpleEdit::GetTextColor(std::string& strTextColor)
{
	strTextColor = m_strColorID;
}

std::string CUISimpleEdit::SetTextColor(const std::string& strTextColor)
{
	std::string strOldColor = m_strColorID;
	m_strColorID = strTextColor;
	Invalidate();
	return strOldColor;
}

void CUISimpleEdit::GetTextExtent(LONG& lnWidth, LONG& lnHeight)
{
	CUIWindowBase* pWindow = GetOwnerTree()->GetBindWnd();
	CDCHandle dc = ::GetDC(pWindow->m_hWnd);
	CUIFont* pFont = UIResFactoryInstance->GetFont(m_strFontID.c_str());
	if (pFont)
	{
		pFont->SelectObject(dc);
	}
	CUIColor* pColor = UIResFactoryInstance->GetColor(m_strColorID.c_str());
	if (pColor)
	{
		pColor->SelectObject(dc);
	}
	SIZE sz = {0};
	dc.GetTextExtent(m_strText.c_str(), -1, &sz);
	if (pFont)
	{
		pFont->RestoreObject(dc);
	}
	if (pColor)
	{
		pColor->RestoreObject(dc);
	}
	lnWidth = sz.cx;
	lnHeight = sz.cy;
}

int CUISimpleEdit::GetText(lua_State* L)
{
	CUISimpleEdit** ppThis = (CUISimpleEdit**) luaL_checkudata(L, -1, CUISimpleEdit::GetRigisterClassName());
	CUISimpleEdit* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	std::wstring wstrText;
	pThis->GetText(wstrText);
	std::string strText;
	Util::Unicode_to_UTF8(wstrText.c_str(), strText);
	lua_pushstring(L, strText.c_str());
	return 1;
}

int CUISimpleEdit::SetText(lua_State* L)
{
	CUISimpleEdit** ppThis = (CUISimpleEdit**) luaL_checkudata(L, -1, CUISimpleEdit::GetRigisterClassName());
	CUISimpleEdit* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	const char* pszText = lua_tostring(L, -2);
	if (pszText == NULL)
		pszText = "";
	std::string strOldText = pThis->SetText(pszText);
	lua_pushstring(L, strOldText.c_str());
	return 1;
}

int CUISimpleEdit::GetTextColor(lua_State* L)
{
	CUISimpleEdit** ppThis = (CUISimpleEdit**) luaL_checkudata(L, -1, CUISimpleEdit::GetRigisterClassName());
	CUISimpleEdit* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	std::string strTextColor;
	pThis->GetTextColor(strTextColor);
	lua_pushstring(L, strTextColor.c_str());
	return 1;
}

int CUISimpleEdit::SetTextColor(lua_State* L)
{
	CUISimpleEdit** ppThis = (CUISimpleEdit**) luaL_checkudata(L, -1, CUISimpleEdit::GetRigisterClassName());
	CUISimpleEdit* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	const char* pszTextColor = lua_tostring(L, -2);
	if (pszTextColor == NULL)
		pszTextColor = "";
	std::string strOldText = pThis->SetTextColor(pszTextColor);
	lua_pushstring(L, strOldText.c_str());
	return 1;
}

int CUISimpleEdit::GetTextExtent(lua_State* L)
{
	CUISimpleEdit** ppThis = (CUISimpleEdit**) luaL_checkudata(L, -1, CUISimpleEdit::GetRigisterClassName());
	CUISimpleEdit* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	LONG lnWidth = 0, lnHeight = 0;
	pThis->GetTextExtent(lnWidth, lnHeight);
	lua_pushinteger(L, (lua_Integer)lnWidth);
	lua_pushinteger(L, (lua_Integer)lnHeight);
	return 2;
}