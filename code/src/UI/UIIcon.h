#pragma once
#include "UIResBase.h"
#include <Util.h>
#include <uiluax.h>
#include <UIParser.h>

class CUIIcon : public CUIResBase
{
public:
	CUIIcon(void);
	CUIIcon(LPXMLDOMNode pNode, const char* pszPath);
	virtual ~CUIIcon(void);
public:
	virtual ResourceType GetType();
public:
	BEGIN_LUA_CALL_MAP(CUIIcon)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIIcon, UI.Icon);
};
