#pragma once
#include "UIResBase.h"
#include "UIBitmap.h"

class CUIImagelist :
	public CUIResBase
{
private:
	CUIImagelist(void) {};
public:
	CUIImagelist(LPXMLDOMNode pNode, const char* pszPath);
	virtual ~CUIImagelist(void);
private:
	CxImage* GetImage();
	void CropBitmap();
	void CropSeperatorBitmap();
	void CropBlockBitmap();
public:
	virtual ResourceType GetType();
	static int GetID(lua_State* L);
	CxImage* GetImageByIndex(int nIndex);
private:
	typedef std::vector<CUIBitmap*> BitmapVec, *LPBitmapVec;
	typedef std::vector<CxImage*> ImageVec, *LPImageVec;
	std::string m_strPath;		// file path
	UINT		m_uBlockSize;	// bitmap block size
	BOOL		m_bSeparator;	// has separator
	BOOL		m_bVertical;	// vertical(TRUE) or horizon(FALSE)
	BitmapVec	m_VecBitmap;	// vector of all bitmap
	ImageVec	m_VecImage;	// vector of all image
	CxImage*	m_pImage;		// raw image
public:
	BEGIN_LUA_CALL_MAP(CUIImagelist)
		LUA_CALL_ENTRY(GetID)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIImagelist, UI.ImageList);
};
