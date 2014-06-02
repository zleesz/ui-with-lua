#pragma once
#include "UIResBase.h"
#include <uiluax.h>
#include <UIParser.h>
#include "UIImagelist.h"

class CUIBitmap : public CUIResBase
{
private:
	CUIBitmap(void);
public:
	CUIBitmap(LPXMLDOMNode pNode, const char* pszPath);
	CUIBitmap(LPXMLDOMNode pNode, CUIImagelist* pImagelist);
	virtual ~CUIBitmap(void);
private:
	BITMAP_HANDLE* m_pBitmap;
public:
	virtual ResourceType GetType() { return RT_UIBITMAP; };
	static int GetID(lua_State* L);
	void Render(HDC dc, const RECT& rc, BOOL bStretch);
public:
	BEGIN_LUA_CALL_MAP(CUIBitmap)
		LUA_CALL_ENTRY(GetID)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIBitmap, UI.Bitmap);
};
