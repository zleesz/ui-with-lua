#pragma once
#include "UIEventContainerBase.h"

class CUIEventCtrlContainer : public CUIEventContainerBase
{
public:
	CUIEventCtrlContainer(void);
	virtual ~CUIEventCtrlContainer(void);
public:
	virtual EventType GetType() { return ET_Control; };
};
