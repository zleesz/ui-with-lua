#pragma once
#include <atlapp.h>
#include <atlgdi.h>
#include "UIResBase.h"
#include <uiluax.h>
#include <UIParser.h>

class CUIFont : public CUIResBase
{
public:
	CUIFont(void);
	CUIFont(LPXMLDOMNode pNode);
	virtual ~CUIFont(void);
private:
	void Init();
private:
	std::wstring	m_strName;
	LONG			m_lnHeight;
	BOOL			m_bBold;
	BOOL			m_bUnderline;
	HFONT			m_oldFont;
	HFONT			m_font;
public:
	virtual ResourceType GetType();
	void SelectObject(CDCHandle dc);
	void RestoreObject(CDCHandle dc);
public:
	BEGIN_LUA_CALL_MAP(CUIFont)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIFont, UI.Font);
};
