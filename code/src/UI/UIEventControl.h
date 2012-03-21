#pragma once
#include "UIEventBase.h"

class CUIEventControl : public CUIEventBase
{
public:
	CUIEventControl(void);
	CUIEventControl(CUIWindowBase* p) : CUIEventBase(p){};
	virtual ~CUIEventControl(void);
public:
	virtual BOOL ParserEvent(LPXMLDOMNode pNode);
};
