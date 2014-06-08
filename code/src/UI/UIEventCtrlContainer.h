#pragma once
#include "UIEventContainerBase.h"

class CUIEventCtrlContainer : public CUIEventContainerBase
{
public:
	CUIEventCtrlContainer(void);
	virtual ~CUIEventCtrlContainer(void);
public:
	BOOL IsAttachedMouseEvent();
public:
	virtual EventType GetType() { return ET_Control; };
};
