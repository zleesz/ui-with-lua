#pragma once
#include "UIResBase.h"
#include <uiluax.h>
#include <UIParser.h>

class CUIFont : public CUIResBase
{
public:
	CUIFont(void);
	CUIFont(LPXMLDOMNode pNode);
	virtual ~CUIFont(void);
public:
	virtual ResourceType GetType();
public:
	BEGIN_LUA_CALL_MAP(CUIFont)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIFont, UI.Font);
};
