#include "StdAfx.h"
#include "UIEventControl.h"

CUIEventControl::CUIEventControl(void)
{
}

CUIEventControl::CUIEventControl(CUIWindowBase* p) : CUIEventBase(p)
{
}

CUIEventControl::~CUIEventControl(void)
{
}

BOOL CUIEventControl::ParserEvent(LPXMLDOMNode pNode)
{
	return TRUE;
}
