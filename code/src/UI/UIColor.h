#pragma once
#include <atlapp.h>
#include <atlgdi.h>
#include "UIResBase.h"
#include <Util.h>
#include <uiluax.h>
#include <UIParser.h>

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
	COLORREF m_oldColor;
private:
	void Init();
public:
	void GetRGBAValue(BYTE* uRed, BYTE* uGreen, BYTE* uBlue, BYTE* uAlpha);
	void GetHSLValue(UINT* puHue, UINT* puSat, UINT* puLum);
	void SetRBGAValue();
	void SetHSLValue();
	void SelectObject(CDCHandle dc);
	void RestoreObject(CDCHandle dc);
public:
	BEGIN_LUA_CALL_MAP(CUIColor)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIColor, UI.Color);
};
