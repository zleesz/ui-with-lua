#pragma once
#include "UIResBase.h"
#include <uiluax.h>
#include <UIParser.h>

class CUIImagelist :
	public CUIResBase
{
private:
	CUIImagelist(void) {};
public:
	CUIImagelist(LPXMLDOMNode pNode, const char* pszPath);
	virtual ~CUIImagelist(void);
private:
	BITMAPLIST_HANDLE GetImageList();
public:
	virtual ResourceType GetType() { return RT_UIIMAGELIST; }
	static int GetID(lua_State* L);
	BITMAP_HANDLE GetBitmapByIndex(int nIndex);
private:
	std::wstring m_strPath;		// file path
	BITMAPLIST_HANDLE m_hBitmapList;
	BITMAP_HANDLE m_hBitmap;
public:
	BEGIN_LUA_CALL_MAP(CUIImagelist)
		LUA_CALL_ENTRY(GetID)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIImagelist, UI.ImageList);
};
