#include "StdAfx.h"
#include ".\uifont.h"

CUIFont::CUIFont(void)
{
}

CUIFont::~CUIFont(void)
{
	UnRegisterClass(this);
}

CUIFont::CUIFont(LPXMLDOMNode pNode) : CUIResBase(pNode)
{
	RegisterClass(this);
}

ResourceType CUIFont::GetType()
{
	return RT_UIFONT;
}
