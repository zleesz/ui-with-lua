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

class CUIBitmap : public CUIResBase
{
private:
	CUIBitmap(void);
public:
	CUIBitmap(LPXMLDOMNode pNode, const char* pszPath);
	virtual ~CUIBitmap(void);
private:
	typedef struct tagBitmapData
	{
		std::string strPath;
		int nType;
		CxImage* pImage;
		tagBitmapData(): nType(BT_DEFAULT), pImage(NULL) {}
	}BitmapData, *LPBitmapData;
	BitmapData m_BitmapData;
	typedef std::vector<CxImage*> ImageVec, LPImageVec;
	ImageVec m_vecImage;
	int m_arrPos[4];
public:
	virtual ResourceType GetType();
	static int GetID(lua_State* L);
	CxImage* GetImage();
	void Render(HDC dc, const RECT& rc, BOOL bStretch);
private:
	void GetVLeftPos(CxImage* pImage);
	void GetHTopPos(CxImage* pImage);
public:
	BEGIN_LUA_CALL_MAP(CUIBitmap)
		LUA_CALL_ENTRY(GetID)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIBitmap, UI.Bitmap);
};
