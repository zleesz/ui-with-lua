#pragma once
#include "UIResBase.h"
#include <uiluax.h>
#include <UIParser.h>
#include "../../UIGraphic/Bitmap.h"

typedef enum enumBitmapLoadType
{
	BLT_FILE = 1,
	BLT_IMAGELIST,
} BitmapLoadType;

class CUIImagelist;
class CUIBitmapLoadBase
{
public:
	virtual BITMAP_HANDLE* GetImage() = 0;
	virtual BitmapLoadType GetType() = 0;
};

class CUIBitmapFile
	: public CUIBitmapLoadBase
{
public:
	CUIBitmapFile() : m_pImage(NULL) {}
public:
	std::string m_strPath;
	Bitmap* m_pImage;
public:
	void SetPath(const std::string& strPath) { m_strPath = strPath; }
	virtual BITMAP_HANDLE* GetImage();
	virtual BitmapLoadType GetType() { return BLT_FILE; }
};

class CUIBitmapImagelist
	: public CUIBitmapLoadBase
{
public:
	CUIBitmapImagelist() : m_nIndex(0), m_pImagelist(NULL) {}
public:
	CUIImagelist* m_pImagelist;
	int m_nIndex;
public:
	void SetImagelist(CUIImagelist* pImagelist, int nIndex);
	virtual BITMAP_HANDLE* GetImage();
	virtual BitmapLoadType GetType() { return BLT_IMAGELIST; }
};

class CUIBitmap : public CUIResBase
{
private:
	CUIBitmap(void);
public:
	CUIBitmap(LPXMLDOMNode pNode, const char* pszPath);
	CUIBitmap(LPXMLDOMNode pNode, CUIImagelist* pImagelist);
	virtual ~CUIBitmap(void);
private:
	Bitmap* m_pBitmap;
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
