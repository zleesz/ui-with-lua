#include "StdAfx.h"
#include "UIModalWindow.h"

CUIModalWindow::CUIModalWindow(void)
{
}

CUIModalWindow::~CUIModalWindow(void)
{
	UnRegisterClass(this);
}

CUIModalWindow::CUIModalWindow(const std::string& strPath, LPXMLDOMNode pNode)
	: CUIWindowBase(strPath, pNode)
{
	RegisterClass(this);
	LPXMLDOMNode pAttrNode = (*pNode->pMapChildNode)["attr"];
	if(pAttrNode != NULL && pAttrNode->pMapChildNode != NULL)
	{
		ParserAttr(pAttrNode);
	}
}

BOOL CUIModalWindow::Render(CDCHandle dc)
{
	return TRUE;
}

DWORD CUIModalWindow::GetStyle()
{
	DWORD dwStyle = 0;
	return dwStyle;
}

DWORD CUIModalWindow::GetStyleEx()
{
	DWORD dwStyle = 0;
	return dwStyle;
}

BOOL CUIModalWindow::CreateWnd(HWND hParent)
{
	return TRUE;
}

void CUIModalWindow::SetAttr(std::string strName, std::string strValue)
{
}

void CUIModalWindow::GetAttr(std::string strName, VARIANT* v)
{
}

BOOL CUIModalWindow::ParserAttr(LPXMLDOMNode pAttrNode)
{
	return TRUE;
}
