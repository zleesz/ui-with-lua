#pragma once
#include "UIEventContainerBase.h"

class CUIControlEventBase
{
	
};

class CUIEventCtrlContainer : public CUIEventContainerBase
{
public:
	CUIEventCtrlContainer(void);
	virtual ~CUIEventCtrlContainer(void);
public:
public:
	virtual EventType GetType() { return ET_Control; };
};
