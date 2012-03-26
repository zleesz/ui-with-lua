#pragma once
#include "UIEventBase.h"

class CUIEventControl : public CUIEventBase
{
private:
	CUIEventControl(void);
public:
	CUIEventControl(CUIWindowBase* p);
	virtual ~CUIEventControl(void);
public:
public:
	virtual BOOL ParserEvent(LPXMLDOMNode pNode);
	virtual EventType GetType() { return ET_Control; };
};
