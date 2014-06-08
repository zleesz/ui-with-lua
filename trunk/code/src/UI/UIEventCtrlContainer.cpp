#include "StdAfx.h"
#include "UIEventCtrlContainer.h"

CUIEventCtrlContainer::CUIEventCtrlContainer(void)
{
}

CUIEventCtrlContainer::~CUIEventCtrlContainer(void)
{
}

BOOL CUIEventCtrlContainer::IsAttachedMouseEvent()
{
	LuaEventMap::const_iterator it = m_mapEvent.find("OnLButtonDown");
	if (it != m_mapEvent.end())
	{
		return TRUE;
	}
	return FALSE;
}