#pragma once
#include "UIResBase.h"
#include <Util.h>
#include <uiluax.h>

class CUIColor : public CUIResBase
{
public:
	CUIColor(void);
	CUIColor(LPXMLDOMNode pNode);
	virtual ~CUIColor(void);
public:
	virtual ResourceType GetType();
private:
	COLORREF m_color;
	BYTE m_uAlpha;
private:
	void Init();
public:
	void GetRGBAValue(BYTE* uRed, BYTE* uGreen, BYTE* uBlue, BYTE* uAlpha);
	void GetHSLValue(UINT* puHue, UINT* puSat, UINT* puLum);
	void SetRBGAValue();
	void SetHSLValue();
public:
	BEGIN_LUA_CALL_MAP(CUIColor)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIColor, UI.Color);
};
