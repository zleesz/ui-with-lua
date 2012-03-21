#pragma once
#include "UIResBase.h"
#include "UIBitmap.h"

class CUIImageList :
	public CUIResBase
{
private:
	CUIImageList(void);
public:
	CUIImageList(LPXMLDOMNode pNode, const char* pszPath);
	virtual ~CUIImageList(void);
public:
	virtual ResourceType GetType();
	static int GetID(lua_State* L);
private:
	typedef std::vector<CUIBitmap*> BitmapVec, *LPBitmapVec;
	typedef struct tagImageListData
	{
		std::string strPath;	// file path
		UINT		uBlockSize;		// bitmap block size
		BOOL		bSeperator;	// has seperator
		BOOL		bVertical;	// vertical(TRUE) or hirizon(FALSE)
		LPBitmapVec vecBitmap;	// vector of all bitmap
		tagImageListData() : uBlockSize(0), bSeperator(FALSE), bVertical(FALSE), vecBitmap(NULL) {}
	}ImageListData, *LPImageListData;
	ImageListData	m_ImagelistData;
public:
	BEGIN_LUA_CALL_MAP(CUIImageList)
		LUA_CALL_ENTRY(GetID)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIImageList, UI.ImageList);
};
