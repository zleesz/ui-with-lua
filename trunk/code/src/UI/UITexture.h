#pragma once
#include "UIResBase.h"
#include <uiluax.h>
#include <UIParser.h>
#include "UIBitmap.h"

class CUITexture : public CUIResBase
{
private:
	CUITexture(void);
public:
	CUITexture(LPXMLDOMNode pNode, const char* pszPath);
	CUITexture(LPXMLDOMNode pNode, CUIImagelist* pImagelist);
	virtual ~CUITexture(void);
private:
	TEXTURE_HANDLE m_hTexture;
	CUIBitmap* m_pBitmap;
public:
	static int GetID(lua_State* L);
	static int GetBitmap(lua_State* L);
public:
	virtual ResourceType GetType() { return RT_UITEXTURE; };
	TEXTURE_HANDLE GetTexture();
	BITMAP_HANDLE GetTextureBitmap(SIZE& sz);
private:
	TEXTURE_HANDLE CreateTextureFromBitmap(LPXMLAttrMap pMapAttr);
public:
	BEGIN_LUA_CALL_MAP(CUITexture)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY(GetBitmap)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUITexture, UI.Texture);
public:
	LOG_CLS_DEC();
};
