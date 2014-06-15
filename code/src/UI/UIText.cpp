#include "StdAfx.h"
#include "UIText.h"

CUIText::CUIText(CUITreeContainer* pTree, LPXMLDOMNode pNode)
: CUIControlBase(pTree, pNode)
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
	XMLChildNodes::const_iterator it2 = pAttrChildNodes->find("text");
	if (it2 != pAttrChildNodes->end())
	{
		m_strText = it2->second->strUData;
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
}

void CUIText::SetAttr(const std::string& /*strName*/, const std::string& /*strValue*/)
{
	
}

CUIText::CUIText(CUITreeContainer* pTree)
: CUIControlBase(pTree)
{
	RegisterClass(this);
}

CUIText::~CUIText(void)
{
	UnRegisterClass(this);
}

void CUIText::Render(CDCHandle dc)
{
	RECT rc = GetObjPos();
	std::wstring wstrText;
	Util::UTF8_to_Unicode(m_strText.c_str(), wstrText);
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
	dc.DrawText(wstrText.c_str(), -1, &rc, DT_LEFT | DT_VCENTER);
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

void CUIText::OnInitControl()
{

}

void CUIText::OnDetroy()
{

}

void CUIText::GetText(std::string& strText)
{
	strText = m_strText;
}

std::string CUIText::SetText(const std::string& strText)
{
	std::string strTmp(m_strText);
	m_strText = strText;
	return strTmp;
}

void CUIText::GetTextColor(std::string& strTextColor)
{
	strTextColor = m_strColorID;
}

std::string CUIText::SetTextColor(const std::string& strTextColor)
{
	std::string strOldColor = m_strColorID;
	m_strColorID = strTextColor;
	return strOldColor;
}

int CUIText::GetText(lua_State* L)
{
	CUIText* pThis = (CUIText*) luaL_checkudata(L, -1, CUIText::GetRigisterClassName());
	std::string strOldText;
	pThis->GetText(strOldText);
	lua_pushstring(L, strOldText.c_str());
	return 1;
}

int CUIText::SetText(lua_State* L)
{
	CUIText* pThis = (CUIText*) luaL_checkudata(L, -1, CUIText::GetRigisterClassName());
	const char* pszText = lua_tostring(L, -2);
	if (pszText == NULL)
		pszText = "";
	std::string strOldText = pThis->SetText(pszText);
	lua_pushstring(L, strOldText.c_str());
	return 1;
}

int CUIText::GetTextColor(lua_State* L)
{
	CUIText* pThis = (CUIText*) luaL_checkudata(L, -1, CUIText::GetRigisterClassName());
	std::string strOldText;
	pThis->GetText(strOldText);
	lua_pushstring(L, strOldText.c_str());
	return 1;
}

int CUIText::SetTextColor(lua_State* L)
{
	CUIText* pThis = (CUIText*) luaL_checkudata(L, -1, CUIText::GetRigisterClassName());
	const char* pszText = lua_tostring(L, -2);
	if (pszText == NULL)
		pszText = "";
	std::string strOldText = pThis->SetText(pszText);
	lua_pushstring(L, strOldText.c_str());
	return 1;
}
