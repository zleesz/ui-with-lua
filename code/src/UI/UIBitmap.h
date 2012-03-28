#pragma once
#include "UIResBase.h"
#include "ImageTool.h"
#include <uiluax.h>

typedef enum enumBitmapType
{
	BT_DEFAULT = 1,
	BT_THREEINONEH,
	BT_THREEINONEV,
	BT_NINEINONE
} BitmapType;

class CUIBitmapBase
{
public:
	CUIBitmapBase() : m_pImage(NULL) {}
	~CUIBitmapBase();
public:
	CxImage* m_pImage;
	std::string m_strPath;
	typedef std::vector<CxImage*> ImageVec, LPImageVec;
	ImageVec m_vecImage;
public:
	virtual void SetPath(const std::string& strPath) { m_strPath = strPath; }
	virtual void LoadImage();
	virtual CxImage* GetImage();
public:
	virtual void CropBitmap() = 0;
	virtual void Render(HDC dc, const RECT& rc, BOOL bStretch) = 0;
	virtual BitmapType GetType() = 0;
};

class CUIBitmapDefault : public CUIBitmapBase
{
public:
	void CropBitmap();
	virtual void Render(HDC dc, const RECT& rc, BOOL bStretch);
	virtual BitmapType GetType() { return BT_DEFAULT; };
};

class CUIBitmapThreeInOneH : public CUIBitmapBase
{
public:
	CUIBitmapThreeInOneH() : m_nLeft1(-1), m_nLeft2(-1) {}
public:
	int m_nLeft1;
	int m_nLeft2;
public:
	void CropBitmap();
	virtual void Render(HDC dc, const RECT& rc, BOOL bStretch);
	virtual BitmapType GetType() { return BT_THREEINONEH; };
};

class CUIBitmapThreeInOneV : public CUIBitmapBase
{
public:
	CUIBitmapThreeInOneV() : m_nTop1(-1), m_nTop2(-1) {}
public:
	int m_nTop1;
	int m_nTop2;
public:
	void CropBitmap();
	virtual void Render(HDC dc, const RECT& rc, BOOL bStretch);
	virtual BitmapType GetType() { return BT_THREEINONEV; };
};

class CUIBitmapNineInOne : public CUIBitmapBase
{
public:
	CUIBitmapNineInOne() : m_nTop1(-1), m_nTop2(-1), m_nLeft1(-1), m_nLeft2(-1) {}
public:
	int m_nLeft1;
	int m_nLeft2;
	int m_nTop1;
	int m_nTop2;
public:
	void CropBitmap();
	virtual void Render(HDC dc, const RECT& rc, BOOL bStretch);
	virtual BitmapType GetType() { return BT_NINEINONE; };
};

class CUIBitmap : public CUIResBase
{
private:
	CUIBitmap(void);
public:
	CUIBitmap(LPXMLDOMNode pNode, const char* pszPath);
	virtual ~CUIBitmap(void);
private:
	CUIBitmapBase* m_pBitmap;
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
