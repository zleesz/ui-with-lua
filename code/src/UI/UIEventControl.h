#pragma once
#include "UIEventBase.h"

class CUIEventControl : public CUIEventBase
{
public:
	CUIEventControl(void);
	virtual ~CUIEventControl(void);
public:
public:
	virtual EventType GetType() { return ET_Control; };
};
