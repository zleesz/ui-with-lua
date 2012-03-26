#pragma once
#include "uiwindowbase.h"

class CUIToolWindow :
	public CUIWindowBase
{
public:
	CUIToolWindow(void);
	virtual ~CUIToolWindow(void);
public:
	BEGIN_MSG_MAP(CUIToolWindow)
	END_MSG_MAP()
};
