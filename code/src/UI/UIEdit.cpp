#include "StdAfx.h"
#include "UIEdit.h"
#include <Util.h>

CUIEdit::CUIEdit(void)
{
	m_bPassword = FALSE;
	m_bMultiLine = FALSE;
	m_bWordBreak = FALSE;
	m_bWordEllipsis = FALSE;
	m_evalign = EVA_TOP;
	m_ehalign = EHA_LEFT;
	m_bInsert = FALSE;
	m_nInsertPos = 0;
	m_nSelStartPos = 0;
	m_nSelEndPos = 0;
	m_nLButtonDownPos = 0;
	m_nMouseMoveLastPos = 0;
	RegisterClass(this);
}

CUIEdit::~CUIEdit(void)
{
	UnRegisterClass(this);
}

CUIEdit::CUIEdit(CUITreeContainer* pTree, LPXMLDOMNode pNode)
: CUIControlBase(pTree, pNode)
{
	m_bPassword = FALSE;
	m_bMultiLine = FALSE;
	m_bWordBreak = FALSE;
	m_bWordEllipsis = FALSE;
	m_evalign = EVA_TOP;
	m_ehalign = EHA_LEFT;
	m_bInsert = FALSE;
	m_nInsertPos = 0;
	m_nSelStartPos = 0;
	m_nSelEndPos = 0;
	m_nLButtonDownPos = 0;
	m_nMouseMoveLastPos = 0;
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

CUIEdit::CUIEdit(CUITreeContainer* pTree)
: CUIControlBase(pTree)
{
	m_bPassword = FALSE;
	m_bMultiLine = FALSE;
	m_bWordBreak = FALSE;
	m_bWordEllipsis = FALSE;
	m_evalign = EVA_TOP;
	m_ehalign = EHA_LEFT;
	m_bInsert = FALSE;
	m_nInsertPos = 0;
	m_nSelStartPos = 0;
	m_nSelEndPos = 0;
	m_nLButtonDownPos = 0;
	m_nMouseMoveLastPos = 0;
	RegisterClass(this);
}

void CUIEdit::Render(CDCHandle dc)
{
	RECT rc = {0};
	CaclTextRect(rc);
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
	int oldBkMode = dc.SetBkMode(TRANSPARENT);
	RECT rcObj = GetObjPos();
	if (m_nSelEndPos > 0)
	{
		LONG lnFormat = DT_TOP;
		if (!m_bMultiLine)
		{
			lnFormat |= DT_SINGLELINE;
		}
		// 有选择的
		// 先画选择左边的字，再画选择的背景，再画选择的字，再画选择右边的字
		LONG lnLeft = rc.left;
		if (m_nSelStartPos > 0)
		{
			// 画左边的字
			LONG lnWidth, lnHeight = 0;
			GetTextExtent(m_strText.c_str(), m_nSelStartPos, lnWidth, lnHeight);
			if (lnLeft + lnWidth > rcObj.left)
			{
				// 只有在范围内的才显示
				if (lnLeft <= rcObj.left && lnLeft + lnWidth >= rcObj.right)
				{
					RECT rcLeft = rcObj;
					dc.DrawText(m_strText.c_str(), -1, &rcLeft, GetFormat());
				}
				else if (lnLeft <= rcObj.left)
				{
					RECT rcLeft = rc;
					rcLeft.left = rcObj.left;
					rcLeft.right = rc.left + lnWidth;
					dc.DrawText(m_strText.c_str(), m_nSelStartPos, &rcLeft, lnFormat | DT_RIGHT);
				}
				else
				{
					RECT rcLeft = rc;
					rcLeft.right = rc.left + lnWidth;
					if (rcLeft.right > rcObj.right)
					{
						rcLeft.right = rcObj.right;
					}
					dc.DrawText(m_strText.c_str(), m_nSelStartPos, &rcLeft, lnFormat | DT_LEFT);
				}
			}
			lnLeft += lnWidth;
		}
		LONG lnWidth, lnHeight = 0;
		GetTextExtent(m_strText.c_str() + m_nSelStartPos, m_nSelEndPos - m_nSelStartPos, lnWidth, lnHeight);
		if (lnLeft + lnWidth > rcObj.left)
		{
			// 只有在范围内的才显示
			if (lnLeft <= rcObj.left && lnLeft + lnWidth >= rcObj.right)
			{
				// 第一段没显示且第二段也不显示的。直接全显示出来
				RECT rcBkg = rcObj;
				rcBkg.top = rc.top;
				rcBkg.bottom = rc.top + lnHeight;
				dc.FillRect(&rcBkg, COLOR_HIGHLIGHT);
				RECT rcCenter = rcObj;
				COLORREF oldColor = dc.SetTextColor(RGB(0xFF, 0xFF, 0xFF));
				dc.DrawText(m_strText.c_str(), -1, &rcCenter, GetFormat());
				dc.SetTextColor(oldColor);
			}
			else if (lnLeft <= rcObj.left)
			{
				RECT rcCenter = {0};
				rcCenter.top = rc.top;
				rcCenter.bottom = rc.top + lnHeight;
				rcCenter.left = rcObj.left;
				rcCenter.right = lnLeft + lnWidth;
				// 画选择的背景
				RECT rcBkg = rcCenter;
				dc.FillRect(&rcBkg, COLOR_HIGHLIGHT);
				// 画选择的字
				COLORREF oldColor = dc.SetTextColor(RGB(0xFF, 0xFF, 0xFF));
				dc.DrawText(m_strText.c_str() + m_nSelStartPos, m_nSelEndPos - m_nSelStartPos, &rcCenter, lnFormat | DT_RIGHT);
				dc.SetTextColor(oldColor);
			}
			else if (lnLeft < rcObj.right)
			{
				RECT rcCenter = {0};
				rcCenter.top = rc.top;
				rcCenter.bottom = rc.top + lnHeight;
				rcCenter.left = lnLeft;
				rcCenter.right = lnLeft + lnWidth;
				if (rcCenter.right > rcObj.right)
				{
					rcCenter.right = rcObj.right;
				}
				// 画选择的背景
				RECT rcBkg = rcCenter;
				dc.FillRect(&rcBkg, COLOR_HIGHLIGHT);
				// 画选择的字
				COLORREF oldColor = dc.SetTextColor(RGB(0xFF, 0xFF, 0xFF));
				dc.DrawText(m_strText.c_str() + m_nSelStartPos, m_nSelEndPos - m_nSelStartPos, &rcCenter, lnFormat | DT_LEFT);
				dc.SetTextColor(oldColor);
			}
		}

		lnLeft += lnWidth;
		if (lnLeft < rcObj.right)
		{
			GetTextExtent(m_strText.c_str() + m_nSelEndPos, m_strText.length() - m_nSelEndPos, lnWidth, lnHeight);
			if (lnLeft <= rcObj.left && lnLeft + lnWidth >= rcObj.right)
			{
				RECT rcRight = rcObj;
				dc.DrawText(m_strText.c_str(), -1, &rcRight, GetFormat());
			}
			else if (lnLeft <= rcObj.left)
			{
				RECT rcRight = rc;
				rcRight.left = rcObj.left;
				rcRight.right = rcRight.left + lnWidth;
				// 画右边的字
				dc.DrawText(m_strText.c_str() + m_nSelEndPos, m_strText.length() - m_nSelEndPos, &rcRight, lnFormat | DT_RIGHT);
			}
			else
			{
				RECT rcRight = rc;
				rcRight.left = lnLeft;
				if (rcRight.right > rcObj.right)
				{
					rcRight.right = rcObj.right;
				}
				// 画右边的字
				dc.DrawText(m_strText.c_str() + m_nSelEndPos, m_strText.length() - m_nSelEndPos, &rcRight, lnFormat | DT_LEFT);
			}
		}
	}
	else
	{
		dc.DrawText(m_strText.c_str(), -1, &rcObj, GetFormat());
	}
	dc.SetBkMode(oldBkMode);
	if (pFont)
	{
		pFont->RestoreObject(dc);
	}
	if (pColor)
	{
		pColor->RestoreObject(dc);
	}

	if (GetFocus())
	{
		// 如果当前有焦点，则得更新一下光标
		UpdateCaretPos();
	}
}

void CUIEdit::OnInitControl()
{
}

void CUIEdit::OnDetroy()
{
}

void CUIEdit::SetAttr(const std::string& strName, const std::string& strValue)
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

void CUIEdit::CaclTextRect(RECT& rc)
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

LONG CUIEdit::GetFormat()
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

void CUIEdit::UpdateCaretPos()
{
	RECT rc = {0};
	CaclTextRect(rc);
	RECT rcObj = GetObjPos();
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
	dc.GetTextExtent(m_strText.c_str(), m_nInsertPos, &sz);
	if (pFont)
	{
		pFont->RestoreObject(dc);
	}
	if (pColor)
	{
		pColor->RestoreObject(dc);
	}
	if (rc.left + sz.cx < rcObj.left || rc.left + sz.cx > rcObj.right)
	{
		// 超出范围，不显示
		::SetCaretPos(-1, rc.top);
		return;
	}
	::SetCaretPos(rc.left + sz.cx, rc.top);
}

size_t CUIEdit::GetCaretPosFromPoint(int& x, int& y)
{
	size_t lnCaretPos = 0;
	RECT rc = {0};
	CaclTextRect(rc);
	y = rc.top;
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
	LONG lnXPos = x;
	for (size_t i = 0; i < m_strText.length(); i++)
	{
		SIZE sz = {0};
		dc.GetTextExtent(&m_strText[i], 1, &sz);
		if (lnLeft + sz.cx < lnXPos)
		{
			lnLeft += sz.cx;
			lnCaretPos = i + 1;
			x = lnLeft;
			continue;
		}
		if ((lnXPos - lnLeft) <= sz.cx / 2)
		{
			x = lnLeft;
			lnCaretPos = i;
		}
		else
		{
			x = lnLeft + sz.cx;
			lnCaretPos = i + 1;
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
	return lnCaretPos;
}

void CUIEdit::GetWordPosByPos(size_t lnPos, size_t& lnStartPos, size_t& lnEndPos)
{
	bool bIsWord = true;
	if (lnPos > 0)
	{
		// 至少选中一个
		lnStartPos = lnPos - 1;
		bIsWord = Util::IsWordChar(m_strText[lnPos - 1]);
	}
	// 往左找
	for (size_t i = lnPos - 1; i != std::string::npos; i--)
	{
		bool bLeftIsWord = Util::IsWordChar(m_strText[i]);
		if (bIsWord != bLeftIsWord)
		{
			break;
		}
		lnStartPos = i;
	}
	// 往右找
	for (size_t i = lnPos; i < m_strText.length(); i++)
	{
		bool bRightIsWord = Util::IsWordChar(m_strText[i]);
		if (bIsWord != bRightIsWord)
		{
			break;
		}
		lnEndPos = i + 1;
	}
}

void CUIEdit::GetLeftWordPosByPos(size_t lnPos, size_t& lnStartPos)
{
	if (lnPos == 0)
	{
		lnStartPos = lnPos;
		return;
	}
	// 至少选中一个
	lnStartPos = lnPos - 1;
	bool bIsWord = Util::IsWordChar(m_strText[lnPos - 1]);
	// 往左找
	for (size_t i = lnPos - 1; i != std::string::npos; i--)
	{
		bool bLeftIsWord = Util::IsWordChar(m_strText[i]);
		if (bIsWord != bLeftIsWord)
		{
			break;
		}
		lnStartPos = i;
	}
}

void CUIEdit::GetRightWordPosByPos(size_t lnPos, size_t& lnEndPos)
{
	if (lnPos == m_strText.length())
	{
		lnEndPos = lnPos;
		return;
	}
	// 至少选中一个
	lnEndPos = lnPos + 1;
	bool bIsWord = Util::IsWordChar(m_strText[lnPos]);
	// 往右找
	for (size_t i = lnPos; i < m_strText.length(); i++)
	{
		bool bRightIsWord = Util::IsWordChar(m_strText[i]);
		if (bIsWord != bRightIsWord)
		{
			break;
		}
		lnEndPos = i + 1;
	}
}

void CUIEdit::SetVisible(BOOL bVisible)
{
	SetAttr("visible", bVisible ? "true" : "false");
	Invalidate();
}

void CUIEdit::OnSetFocus(BOOL bFocus)
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

void CUIEdit::OnLButtonDown(int x, int y)
{
	RECT rc = GetObjPos();
	x += rc.left;
	y += rc.top;
	m_nInsertPos = GetCaretPosFromPoint(x, y);

	if (m_nSelEndPos > 0)
	{
		Invalidate();
	}
	if (x >= rc.left && x <= rc.right)
	{
		// 超出范围，不显示
		::SetCaretPos(x, y);
	}
	else
	{
		::SetCaretPos(-1, y);
	}

	m_nLButtonDownPos = m_nInsertPos;
	m_nSelStartPos = 0;
	m_nSelEndPos = 0;
	m_nMouseMoveLastPos = m_nLButtonDownPos;
	SetCaptureMouse(TRUE);
}

void CUIEdit::OnLButtonUp(int /*x*/, int /*y*/)
{
	SetCaptureMouse(FALSE);
}

void CUIEdit::OnLButtonDbClick(int x, int y)
{
	RECT rc = GetObjPos();
	x += rc.left;
	y += rc.top;
	size_t lnCaretPos = GetCaretPosFromPoint(x, y);
	size_t lnStartPos = lnCaretPos, lnEndPos = lnCaretPos;
	GetWordPosByPos(lnCaretPos, lnStartPos, lnEndPos);
	if (lnStartPos == lnEndPos)
	{
		return;
	}
	m_nSelStartPos = lnStartPos;
	m_nSelEndPos = lnEndPos;
	m_nLButtonDownPos = m_nSelStartPos;
	Invalidate();
	if (m_nInsertPos != lnEndPos)
	{
		m_nInsertPos = lnEndPos;
		UpdateCaretPos();
	}
}

void CUIEdit::OnMouseMove(int x, int y)
{
	if (!GetCaptureMouse())
	{
		return;
	}
	RECT rc = GetObjPos();
	x += rc.left;
	y += rc.top;
	size_t lnNewCaretPos = GetCaretPosFromPoint(x, y);
	if (lnNewCaretPos == m_nMouseMoveLastPos)
	{
		return;
	}
	m_nMouseMoveLastPos = lnNewCaretPos;
	if (lnNewCaretPos > m_nLButtonDownPos)
	{
		m_nSelEndPos= lnNewCaretPos;
		m_nSelStartPos = m_nLButtonDownPos;
	}
	else if (lnNewCaretPos < m_nLButtonDownPos)
	{
		m_nSelEndPos= m_nLButtonDownPos;
		m_nSelStartPos = lnNewCaretPos;
	}
	else
	{
		m_nSelEndPos = m_nSelStartPos = 0;
	}
	Invalidate();
	if (m_nInsertPos != m_nMouseMoveLastPos)
	{
		m_nInsertPos = m_nMouseMoveLastPos;
		UpdateCaretPos();
	}
}

void CUIEdit::DealCodeKeyDown(int code)
{
	BOOL bUpdateCaretPos = FALSE;
	BOOL bInvalid = FALSE;
	switch (code)
	{
	case VK_RIGHT:
		if (m_nSelEndPos > 0)
		{
			// 有选择内容
			m_nInsertPos = m_nSelEndPos;
			m_nSelStartPos = m_nSelEndPos = 0;
			bUpdateCaretPos = TRUE;
			bInvalid = TRUE;
		}
		else if (m_nInsertPos < m_strText.length())
		{
			m_nInsertPos++;
			bUpdateCaretPos = TRUE;
		}
		break;
	case VK_LEFT:
		if (m_nSelEndPos > 0)
		{
			// 有选择内容
			m_nInsertPos = m_nSelStartPos;
			m_nSelStartPos = m_nSelEndPos = 0;
			bUpdateCaretPos = TRUE;
			bInvalid = TRUE;
		}
		else if (m_nInsertPos > 0)
		{
			m_nInsertPos--;
			bUpdateCaretPos = TRUE;
		}
		m_nLButtonDownPos = m_nInsertPos;
		break;
	case VK_UP:
		break;
	case VK_DOWN:
		break;
	case VK_BACK:
		if (m_nSelEndPos > 0)
		{
			m_strText.erase(m_nSelStartPos, m_nSelEndPos - m_nSelStartPos);
			m_nInsertPos = m_nSelStartPos;
			m_nSelStartPos = m_nSelEndPos = 0;
			bUpdateCaretPos = TRUE;
			bInvalid = TRUE;
		}
		else if (m_nInsertPos > 0)
		{
			m_strText.erase(m_nInsertPos-1, 1);
			m_nInsertPos--;
			bUpdateCaretPos = TRUE;
			bInvalid = TRUE;
		}
		m_nLButtonDownPos = m_nInsertPos;
		break;
	case VK_DELETE:
		if (m_nSelEndPos > 0)
		{
			m_strText.erase(m_nSelStartPos, m_nSelEndPos - m_nSelStartPos);
			m_nInsertPos = m_nSelStartPos;
			m_nSelStartPos = m_nSelEndPos = 0;
			m_nLButtonDownPos = m_nInsertPos;
			bUpdateCaretPos = TRUE;
			bInvalid = TRUE;
		}
		else if (m_nInsertPos < m_strText.length())
		{
			m_strText.erase(m_nInsertPos, 1);
			bUpdateCaretPos = TRUE;
			bInvalid = TRUE;
		}
		break;
	case VK_HOME:
		if (m_nSelEndPos > 0)
		{
			bInvalid = TRUE;
			m_nSelStartPos = m_nSelEndPos = 0;
		}
		bUpdateCaretPos = TRUE;
		m_nLButtonDownPos = m_nInsertPos = 0;
		break;
	case VK_END:
		if (m_nSelEndPos > 0)
		{
			bInvalid = TRUE;
			m_nSelStartPos = m_nSelEndPos = 0;
		}
		bUpdateCaretPos = TRUE;
		m_nLButtonDownPos = m_nInsertPos = m_strText.length();
		break;
	default:
		break;
	}
	if (bInvalid)
	{
		Invalidate();
	}
	if (bUpdateCaretPos)
	{
		UpdateCaretPos();
	}
}

void CUIEdit::DealCtrlKeyDown(int code)
{
	BOOL bUpdateCaretPos = FALSE;
	BOOL bInvalid = FALSE;
	switch (code)
	{
	case 'A':
		// Ctrl + A : select all
		m_nInsertPos =m_nLButtonDownPos = 0;
		m_nSelStartPos = 0;
		m_nSelEndPos = m_strText.length();
		bUpdateCaretPos = TRUE;
		break;
	case 'Z':
		// Ctrl + Z : cancel
		break;
	case 'Y':
		// Ctrl + Y : recover
		break;
	default:
		break;
	}
	if (bUpdateCaretPos)
	{
		UpdateCaretPos();
	}
	if (bInvalid)
	{
		Invalidate();
	}
}

void CUIEdit::DealShiftKeyDown(int code)
{
	BOOL bUpdateCaretPos = FALSE;
	BOOL bInvalid = FALSE;
	switch (code)
	{
	case VK_LEFT:
		if (m_nSelEndPos == 0)
		{
			// 无选择文字时
			if (m_nLButtonDownPos > 0)
			{
				m_nSelEndPos = m_nLButtonDownPos;
				m_nSelStartPos = m_nSelEndPos - 1;
				m_nInsertPos = m_nSelStartPos;
				bUpdateCaretPos = TRUE;
				bInvalid = TRUE;
			}
		}
		else
		{
			// 已经选择了文字
			if (m_nInsertPos > m_nLButtonDownPos)
			{
				// 光标在右边
				if (m_nInsertPos == m_nLButtonDownPos + 1)
				{
					m_nInsertPos = m_nLButtonDownPos;
					m_nSelStartPos = m_nSelEndPos = 0;
					bUpdateCaretPos = TRUE;
					bInvalid = TRUE;
				}
				else
				{
					m_nSelEndPos--;
					m_nInsertPos = m_nSelEndPos;
					bUpdateCaretPos = TRUE;
					bInvalid = TRUE;
				}
			}
			else
			{
				if (m_nInsertPos > 0)
				{
					m_nSelStartPos--;
					m_nInsertPos = m_nSelStartPos;
					bUpdateCaretPos = TRUE;
					bInvalid = TRUE;
				}
			}
		}
		break;
	case VK_RIGHT:
		if (m_nSelEndPos == 0)
		{
			// 无选择文字时
			if (m_nLButtonDownPos < m_strText.length())
			{
				m_nSelStartPos = m_nLButtonDownPos;
				m_nSelEndPos = m_nSelStartPos + 1;
				m_nInsertPos = m_nSelEndPos;
				bUpdateCaretPos = TRUE;
				bInvalid = TRUE;
			}
		}
		else
		{
			// 已经选择了文字
			if (m_nInsertPos > m_nLButtonDownPos)
			{
				// 光标在右边
				if (m_nInsertPos < m_strText.length())
				{
					m_nSelEndPos++;
					m_nInsertPos = m_nSelEndPos;
					bUpdateCaretPos = TRUE;
					bInvalid = TRUE;
				}
			}
			else
			{
				if (m_nInsertPos == m_nLButtonDownPos - 1)
				{
					m_nInsertPos = m_nLButtonDownPos;
					m_nSelStartPos = m_nSelEndPos = 0;
					bUpdateCaretPos = TRUE;
					bInvalid = TRUE;
				}
				else
				{
					m_nSelStartPos++;
					m_nInsertPos = m_nSelStartPos;
					bUpdateCaretPos = TRUE;
					bInvalid = TRUE;
				}
			}
		}
		break;
	case VK_UP:
		break;
	case VK_DOWN:
		break;
	case VK_HOME:
		if (m_nSelEndPos == 0)
		{
			if (m_nLButtonDownPos > 0)
			{
				m_nInsertPos = 0;
				m_nSelStartPos = 0;
				m_nSelEndPos = m_nLButtonDownPos;
				bUpdateCaretPos = TRUE;
				bInvalid = TRUE;
			}
		}
		else
		{
			if (m_nSelEndPos > m_nLButtonDownPos)
			{
				// 目前选中的是右边，回退就行
				m_nInsertPos = m_nLButtonDownPos;
				m_nSelStartPos = m_nSelEndPos = 0;
				bUpdateCaretPos = TRUE;
				bInvalid = TRUE;
			}
			else
			{
				m_nInsertPos = 0;
				m_nSelStartPos = 0;
				m_nSelEndPos = m_nLButtonDownPos;
				bUpdateCaretPos = TRUE;
				bInvalid = TRUE;
			}
		}
		break;
	case VK_END:
		if (m_nSelEndPos == 0)
		{
			if (m_nLButtonDownPos < m_strText.length())
			{
				m_nInsertPos = m_strText.length();
				m_nSelStartPos = m_nLButtonDownPos;
				m_nSelEndPos = m_nInsertPos;
				bUpdateCaretPos = TRUE;
				bInvalid = TRUE;
			}
		}
		else
		{
			if (m_nSelStartPos < m_nLButtonDownPos)
			{
				// 目前选中的是左边，回退就行
				m_nInsertPos = m_nLButtonDownPos;
				m_nSelStartPos = m_nSelEndPos = 0;
				bUpdateCaretPos = TRUE;
				bInvalid = TRUE;
			}
			else
			{
				m_nInsertPos = m_strText.length();
				m_nSelStartPos = m_nLButtonDownPos;
				m_nSelEndPos = m_nInsertPos;
				bUpdateCaretPos = TRUE;
				bInvalid = TRUE;
			}
		}
		break;
	default:
		break;
	}
	if (bInvalid)
	{
		Invalidate();
	}
	if (bUpdateCaretPos)
	{
		UpdateCaretPos();
	}
}

void CUIEdit::DealCtrlShiftKeyDown(int code)
{
	BOOL bUpdateCaretPos = FALSE;
	BOOL bInvalid = FALSE;
	switch (code)
	{
	case VK_LEFT:
		if (m_nSelEndPos == 0)
		{
			// 无选择文字时
			if (m_nLButtonDownPos > 0)
			{
				m_nSelEndPos = m_nLButtonDownPos;
				GetLeftWordPosByPos(m_nInsertPos, m_nSelStartPos);
				m_nInsertPos = m_nSelStartPos;
				bUpdateCaretPos = TRUE;
				bInvalid = TRUE;
			}
		}
		else
		{
			// 已经选择了文字
			if (m_nInsertPos > m_nLButtonDownPos)
			{
				// 光标在右边
				size_t lnStartPos = m_nInsertPos;
				GetLeftWordPosByPos(m_nInsertPos, lnStartPos);
				if (lnStartPos <= m_nLButtonDownPos)
				{
					m_nInsertPos = m_nLButtonDownPos;
					m_nSelStartPos = m_nSelEndPos = 0;
					bUpdateCaretPos = TRUE;
					bInvalid = TRUE;
				}
				else
				{
					m_nSelEndPos = lnStartPos;
					m_nInsertPos = m_nSelEndPos;
					bUpdateCaretPos = TRUE;
					bInvalid = TRUE;
				}
			}
			else
			{
				if (m_nInsertPos > 0)
				{
					GetLeftWordPosByPos(m_nInsertPos, m_nSelStartPos);
					m_nInsertPos = m_nSelStartPos;
					bUpdateCaretPos = TRUE;
					bInvalid = TRUE;
				}
			}
		}
		break;
	case VK_RIGHT:
		if (m_nSelEndPos == 0)
		{
			// 无选择文字时
			if (m_nLButtonDownPos < m_strText.length())
			{
				size_t lnEndPos = m_nSelEndPos;
				GetRightWordPosByPos(m_nInsertPos, lnEndPos);
				m_nSelStartPos = m_nLButtonDownPos;
				m_nSelEndPos = lnEndPos;
				m_nInsertPos = m_nSelEndPos;
				bUpdateCaretPos = TRUE;
				bInvalid = TRUE;
			}
		}
		else
		{
			// 已经选择了文字
			if (m_nInsertPos > m_nLButtonDownPos)
			{
				// 光标在右边
				if (m_nInsertPos < m_strText.length())
				{
					GetRightWordPosByPos(m_nInsertPos, m_nSelEndPos);
					m_nInsertPos = m_nSelEndPos;
					bUpdateCaretPos = TRUE;
					bInvalid = TRUE;
				}
			}
			else
			{
				size_t lnEndPos = m_nInsertPos;
				GetRightWordPosByPos(m_nInsertPos, lnEndPos);
				if (lnEndPos >= m_nLButtonDownPos)
				{
					m_nInsertPos = m_nLButtonDownPos;
					m_nSelStartPos = m_nSelEndPos = 0;
					bUpdateCaretPos = TRUE;
					bInvalid = TRUE;
				}
				else
				{
					m_nSelStartPos = lnEndPos;
					m_nInsertPos = m_nSelStartPos;
					bUpdateCaretPos = TRUE;
					bInvalid = TRUE;
				}
			}
		}
		break;
	case VK_UP:
		break;
	case VK_DOWN:
		break;
	case VK_HOME:
		if (!m_bMultiLine)
		{
			DealShiftKeyDown(code);
			return;
		}
		break;
	case VK_END:
		if (!m_bMultiLine)
		{
			DealShiftKeyDown(code);
			return;
		}
		break;
	default:
		break;
	}
	if (bInvalid)
	{
		Invalidate();
	}
	if (bUpdateCaretPos)
	{
		UpdateCaretPos();
	}
}

void CUIEdit::OnKeyDown(int code, BOOL bAlt, BOOL bCtrl, BOOL bShift, int /*repeat*/)
{
	if (!bCtrl && !bShift)
	{
		DealCodeKeyDown(code);
	}
	else if (bCtrl && !bShift)
	{
		DealCtrlKeyDown(code);
	}
	else if (!bCtrl && bShift)
	{
		DealShiftKeyDown(code);
	}
	else if (bCtrl && bShift)
	{
		DealCtrlShiftKeyDown(code);
	}
}

void CUIEdit::OnChar(const wchar_t& c, int /*repeat*/)
{
	if (::GetKeyState(VK_CONTROL) < 0 || c == VK_BACK || c == VK_ESCAPE)
	{
		return;
	}
	if (!m_bMultiLine && c == VK_RETURN)
	{
		return;
	}
	if (m_nSelEndPos > 0)
	{
		m_strText.erase(m_nSelStartPos, m_nSelEndPos - m_nSelStartPos);
		m_nInsertPos = m_nSelStartPos;
		m_nSelStartPos = m_nSelEndPos = 0;
	}
	m_strText.insert(m_nInsertPos, 1, c);
	m_nInsertPos++;
	m_nLButtonDownPos = m_nInsertPos;
	Invalidate();
	UpdateCaretPos();
}

LRESULT CUIEdit::OnSetCursor(int /*x*/, int /*y*/)
{
	BOOL bEnable = CUIControlBase::GetEnable();
	if (bEnable)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
		return TRUE;
	}
	return FALSE;
}

void CUIEdit::GetText(std::wstring& strText)
{
	strText = m_strText;
}

std::string CUIEdit::SetText(const std::string& strText)
{
	std::wstring wstrTmp(m_strText);
	Util::UTF8_to_Unicode(strText.c_str(), m_strText);
	Invalidate();
	std::string strTmp;
	Util::Unicode_to_UTF8(wstrTmp.c_str(), strTmp);
	return strTmp;
}

void CUIEdit::GetTextColor(std::string& strTextColor)
{
	strTextColor = m_strColorID;
}

std::string CUIEdit::SetTextColor(const std::string& strTextColor)
{
	std::string strOldColor = m_strColorID;
	m_strColorID = strTextColor;
	Invalidate();
	return strOldColor;
}

void CUIEdit::GetTextExtent(LONG& lnWidth, LONG& lnHeight)
{
	GetTextExtent(m_strText.c_str(), lnWidth, lnHeight);
}

void CUIEdit::GetTextExtent(const wchar_t* psz, LONG& lnWidth, LONG& lnHeight)
{
	GetTextExtent(psz, -1, lnWidth, lnHeight);
}

void CUIEdit::GetTextExtent(const wchar_t* psz, LONG lnSize, LONG& lnWidth, LONG& lnHeight)
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
	const wchar_t* pszCacl = psz;
	if (pszCacl == NULL || wcslen(pszCacl) == 0)
	{
		pszCacl = L"A";
		lnSize = 1;
	}
	dc.GetTextExtent(pszCacl, lnSize, &sz);
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

int CUIEdit::GetText(lua_State* L)
{
	CUIEdit** ppThis = (CUIEdit**) luaL_checkudata(L, 1, CUIEdit::GetRigisterClassName());
	CUIEdit* pThis = *ppThis;
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

int CUIEdit::SetText(lua_State* L)
{
	CUIEdit** ppThis = (CUIEdit**) luaL_checkudata(L, 1, CUIEdit::GetRigisterClassName());
	CUIEdit* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	const char* pszText = lua_tostring(L, 2);
	if (pszText == NULL)
		pszText = "";
	std::string strOldText = pThis->SetText(pszText);
	lua_pushstring(L, strOldText.c_str());
	return 1;
}

int CUIEdit::GetTextColor(lua_State* L)
{
	CUIEdit** ppThis = (CUIEdit**) luaL_checkudata(L, 1, CUIEdit::GetRigisterClassName());
	CUIEdit* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	std::string strTextColor;
	pThis->GetTextColor(strTextColor);
	lua_pushstring(L, strTextColor.c_str());
	return 1;
}

int CUIEdit::SetTextColor(lua_State* L)
{
	CUIEdit** ppThis = (CUIEdit**) luaL_checkudata(L, 1, CUIEdit::GetRigisterClassName());
	CUIEdit* pThis = *ppThis;
	if (!pThis)
	{
		return 0;
	}
	const char* pszTextColor = lua_tostring(L, 2);
	if (pszTextColor == NULL)
		pszTextColor = "";
	std::string strOldText = pThis->SetTextColor(pszTextColor);
	lua_pushstring(L, strOldText.c_str());
	return 1;
}

int CUIEdit::GetTextExtent(lua_State* L)
{
	CUIEdit** ppThis = (CUIEdit**) luaL_checkudata(L, 1, CUIEdit::GetRigisterClassName());
	CUIEdit* pThis = *ppThis;
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