#include "StdAfx.h"
#include ".\uiicon.h"

CUIIcon::CUIIcon(void)
{
}

CUIIcon::~CUIIcon(void)
{
	UnRegisterClass(this);
}

CUIIcon::CUIIcon(LPXMLDOMNode pNode, const char* pszPath) : CUIResBase(pNode)
{
	RegisterClass(this);
}

ResourceType CUIIcon::GetType()
{
	return RT_UIICON;
}
