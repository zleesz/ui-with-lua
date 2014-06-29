#include "StdAfx.h"
#include "UIMenuItem.h"

CUIMenuItem::CUIMenuItem(void)
{
}

CUIMenuItem::CUIMenuItem(CUITreeContainer* pTree, LPXMLDOMNode pNode)
{

}

CUIMenuItem::CUIMenuItem(CUITreeContainer* pTree)
{

}

CUIMenuItem::~CUIMenuItem(void)
{
}

void CUIMenuItem::Render(CDCHandle dc)
{

}

void CUIMenuItem::OnInitControl()
{

}

void CUIMenuItem::OnDetroy()
{

}

void CUIMenuItem::SetAttr(const std::string& strName, const std::string& strValue)
{
	if (_SetAttr(strName, strValue))
	{
		return;
	}
}