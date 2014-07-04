#include "StdAfx.h"
#include "UISimpleEdit.h"

CUISimpleEdit::CUISimpleEdit(void)
{
	m_bPassword = FALSE;
	m_bMultiLine = FALSE;
	m_bWordBreak = FALSE;
	m_bWordEllipsis = FALSE;
	m_evalign = EVA_TOP;
	m_ehalign = EHA_LEFT;
	m_bInsert = FALSE;
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
	m_bMultiLine = FALSE;
	m_bWordBreak = FALSE;
	m_bWordEllipsis = FALSE;
	m_evalign = EVA_TOP;
	m_ehalign = EHA_LEFT;
	m_bInsert = FALSE;
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
	it2 = pAttrChildNodes->find("valign");
	if (it2 != pAttrChildNodes->end())
	{
		std::string strVAlign = it2->second->strUData;
		if (strVAlign == "top")
		{
			m_evalign = EVA_TOP;
		}
		else if (strVAlign == "center")
		{
			m_evalign = EVA_CENTER;
		}
		else if (strVAlign == "bottom")
		{
			m_evalign = EVA_BOTTOM;
		}
	}
	it2 = pAttrChildNodes->find("halign");
	if (it2 != pAttrChildNodes->end())
	{
		std::string strHAlign = it2->second->strUData;
		if (strHAlign == "left")
		{
			m_ehalign = EHA_LEFT;
		}
		else if (strHAlign == "center")
		{
			m_ehalign = EHA_CENTER;
		}
		else if (strHAlign == "right")
		{
			m_ehalign = EHA_RIGHT;
		}
	}
	it2 = pAttrChildNodes->find("wordbreak");
	if (it2 != pAttrChildNodes->end())
	{
		std::string strWordBreak = it2->second->strUData;
		m_bWordBreak = (strWordBreak == "1" || strWordBreak == "true");
	}
	it2 = pAttrChildNodes->find("wordellipsis");
	if (it2 != pAttrChildNodes->end())
	{
		std::string strWordEllipsis = it2->second->strUData;
		m_bWordEllipsis = (strWordEllipsis == "1" || strWordEllipsis == "true");
	}
	it2 = pAttrChildNodes->find("multiline");
	if (it2 != pAttrChildNodes->end())
	{
		std::string strMultiLine = it2->second->strUData;
		m_bMultiLine = (strMultiLine == "1" || strMultiLine == "true");
	}
}

CUISimpleEdit::CUISimpleEdit(CUITreeContainer* pTree)
: CUIControlBase(pTree)
{
	m_bPassword = FALSE;
	m_bMultiLine = FALSE;
	m_bWordBreak = FALSE;
	m_bWordEllipsis = FALSE;
	m_evalign = EVA_TOP;
	m_ehalign = EHA_LEFT;
	m_bInsert = FALSE;
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
	dc.DrawText(m_strText.c_str(), -1, &rc, GetFormat());
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
	LONG lnFormat = GetFormat();
	if (lnFormat & DT_SINGLELINE)
	{
		LONG lnWidth = 0, lnHeight = 0;
		GetTextExtent(lnWidth, lnHeight);
		if (lnFormat & DT_LEFT)
		{
			rc.right = rc.left + lnWidth;
		}
		else if (lnFormat & DT_RIGHT)
		{
			rc.left = rc.right - lnWidth;
		}
		else if (lnFormat & DT_CENTER)
		{
			rc.left = rc.left + (rc.right - rc.left - lnWidth) / 2;
			rc.right = rc.left + lnWidth;
		}

		if (lnFormat & DT_TOP)
		{
			rc.bottom = rc.top + lnHeight;
		}
		else if (lnFormat & DT_BOTTOM)
		{
			rc.top = rc.bottom - lnHeight;
		}
		else if (lnFormat & DT_VCENTER)
		{
			rc.top = rc.top + (rc.bottom - rc.top- lnHeight) / 2;
			rc.bottom = rc.top + lnHeight;
		}
	}
	else
	{

	}
}

LONG CUISimpleEdit::GetFormat()
{
	LONG lnFormat = 0;
	if (!m_bMultiLine)
	{
		lnFormat |= DT_SINGLELINE;
	}
	if (m_bWordBreak)
	{
		lnFormat |= DT_WORDBREAK;
	}
	if (m_bWordEllipsis)
	{
		lnFormat |= DT_WORD_ELLIPSIS;
	}
	switch (m_ehalign)
	{
	case EHA_CENTER:
		lnFormat |= DT_CENTER;
		break;
	case EHA_LEFT:
		lnFormat |= DT_LEFT;
		break;
	case EHA_RIGHT:
		lnFormat |= DT_RIGHT;
		break;
	default:
		break;
	}
	switch (m_evalign)
	{
	case EVA_CENTER:
		lnFormat |= DT_VCENTER;
		break;
	case EVA_TOP:
		lnFormat |= DT_TOP;
		break;
	case EVA_BOTTOM:
		lnFormat |= DT_BOTTOM;
		break;
	default:
		break;
	}
	return lnFormat;
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
	LONG lnLeft = rc.left;
	for (size_t i = 0; i < m_strText.length(); i++)
	{
		SIZE sz = {0};
		dc.GetTextExtent(&m_strText[i], 1, &sz);
		if (lnLeft + sz.cx < x)
		{
			lnLeft += sz.cx;
			continue;
		}
		if ((x - lnLeft) <= sz.cx / 2)
		{
			rc.left = lnLeft;
		}
		else
		{
			rc.left = lnLeft + sz.cx;
		}
		break;
	}
	if (pFont)
	{
		pFont->RestoreObject(dc);
	}
	if (pColor)
	{
		pColor->RestoreObject(dc);
	}
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
	GetTextExtent(m_strText.c_str(), lnWidth, lnHeight);
}

void CUISimpleEdit::GetTextExtent(const wchar_t* psz, LONG& lnWidth, LONG& lnHeight)
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
	dc.GetTextExtent(psz, -1, &sz);
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