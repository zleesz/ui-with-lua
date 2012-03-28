#include "StdAfx.h"
#include ".\uibitmap.h"

CUIBitmap::CUIBitmap(void)
{
}

CUIBitmap::~CUIBitmap(void)
{
	UnRegisterClass(this);
	if(NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
}

CUIBitmap::CUIBitmap(LPXMLDOMNode pNode, const char* pszPath)
	: CUIResBase(pNode), m_pBitmap(NULL)
{
	RegisterClass(this);
	if(pNode == NULL)
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	std::string strPath = (*pMapAttr)["path"];
	char szPath[MAX_PATH] = {0};
	strcpy_s(szPath, MAX_PATH, pszPath);
	PathRemoveFileSpecA(szPath);
	PathCombineA(szPath, szPath, strPath.c_str());
	std::string strType = (*pMapAttr)["type"];
	
	if(strType == "ThreeInOneH")
	{
		m_pBitmap = new CUIBitmapThreeInOneH;
	}
	else if(strType == "ThreeInOneV")
	{
		m_pBitmap = new CUIBitmapThreeInOneV;
	}
	else if(strType == "NineInOne")
	{
		m_pBitmap = new CUIBitmapNineInOne;
	}
	m_pBitmap->SetPath(szPath);
}

int CUIBitmap::GetID(lua_State* L)
{
	CUIResBase* pThis = (CUIResBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushstring(L, pThis->GetID());
	return 1;
}

CUIBitmapBase::~CUIBitmapBase()
{
	if(NULL != m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
	int nSize = (int)m_vecImage.size();
	if(nSize > 1)
	{
		for(int i = 0; i < nSize; i++)
		{
			CxImage* pImage = m_vecImage[i];
			delete pImage;
		}
	}
	m_vecImage.clear();
}

CxImage* CUIBitmapBase::GetImage()
{
	LoadImage();
	return m_pImage;
}

void CUIBitmapBase::LoadImage()
{
	if(m_pImage != NULL)
		return;
	ATLASSERT(::PathFileExistsA(m_strPath.c_str()));
	if(!::PathFileExistsA(m_strPath.c_str()))
		return ;
	CxImage* pImage = new CxImage;
	m_pImage = pImage;
	std::wstring wstrPath;
	Util::StringToWideString(m_strPath.c_str(), wstrPath);
	m_pImage->Load(wstrPath.c_str(), CXIMAGE_FORMAT_PNG);
}

void CUIBitmapDefault::CropBitmap()
{
	if(m_vecImage.size() > 0)
		return;

	CxImage* pImage = GetImage();
	m_vecImage.push_back(pImage);
}

void CUIBitmapDefault::Render(HDC dc, const RECT& rc, BOOL /*bStretch*/)
{
	CropBitmap();
	m_vecImage[0]->Draw(dc, rc);
}

void CUIBitmapThreeInOneH::CropBitmap()
{
	if(m_vecImage.size() > 0)
		return;

	CxImage* pImage = GetImage();
	if(NULL == pImage)
		return;
	CImageTool::GetVLeftPos(pImage, m_nLeft1, m_nLeft2);
	CxImage *pDrawImage1 = new CxImage();
	CxImage *pDrawImage2 = new CxImage();
	CxImage *pDrawImage3 = new CxImage();
	RECT lrc = {0};
	lrc.left = 0;
	lrc.top = 0;
	lrc.right = m_nLeft1;
	lrc.bottom = pImage->GetHeight();
	pImage->Crop(lrc, pDrawImage1);
	m_vecImage.push_back(pDrawImage1);
	lrc.left = m_nLeft1 + 1;
	lrc.right = m_nLeft2;
	pImage->Crop(lrc, pDrawImage2);
	m_vecImage.push_back(pDrawImage2);
	lrc.left = m_nLeft2 + 1;
	lrc.right = pImage->GetWidth();
	pImage->Crop(lrc, pDrawImage3);
	m_vecImage.push_back(pDrawImage3);
}

void CUIBitmapThreeInOneH::Render(HDC dc, const RECT& rc, BOOL bStretch)
{
	CropBitmap();
	RECT lrc = {0};
	lrc.left = rc.left;
	lrc.top = rc.top;
	lrc.right = m_nLeft1 + rc.left;
	if(bStretch)
	{
		lrc.bottom = rc.bottom;
	}
	else
	{
		lrc.bottom = m_pImage->GetHeight() + rc.top;
	}
	m_vecImage[0]->Draw(dc, lrc);
	lrc.left = lrc.right;
	lrc.right = rc.right - (m_pImage->GetWidth() - m_nLeft2) + 1;
	m_vecImage[1]->Draw(dc, lrc);
	lrc.left = lrc.right;
	lrc.right = lrc.left + (m_pImage->GetWidth() - m_nLeft2) - 1;
	m_vecImage[2]->Draw(dc, lrc);
}

void CUIBitmapThreeInOneV::CropBitmap()
{
	if(m_vecImage.size() > 0)
		return;

}

void CUIBitmapThreeInOneV::Render(HDC /*dc*/, const RECT& /*rc*/, BOOL /*bStretch*/)
{
	CropBitmap();
}

void CUIBitmapNineInOne::CropBitmap()
{
	if(m_vecImage.size() > 0)
		return;

	CxImage* pImage = GetImage();
	if(NULL == pImage)
		return;
	CImageTool::GetVLeftPos(pImage, m_nLeft1, m_nLeft2);
	CImageTool::GetHTopPos(pImage, m_nTop1, m_nTop2);
	CxImage *pDrawImage11 = new CxImage();
	CxImage *pDrawImage12 = new CxImage();
	CxImage *pDrawImage13 = new CxImage();
	CxImage *pDrawImage21 = new CxImage();
	CxImage *pDrawImage22 = new CxImage();
	CxImage *pDrawImage23 = new CxImage();
	CxImage *pDrawImage31 = new CxImage();
	CxImage *pDrawImage32 = new CxImage();
	CxImage *pDrawImage33 = new CxImage();
	RECT lrc = {0};
	lrc.left = 0;
	lrc.top = 0;
	lrc.right = m_nLeft1;
	lrc.bottom = m_nTop1;
	pImage->Crop(lrc, pDrawImage11);
	m_vecImage.push_back(pDrawImage11);

	lrc.left = m_nLeft1 + 1;
	lrc.right = m_nLeft2;
	pImage->Crop(lrc, pDrawImage12);
	m_vecImage.push_back(pDrawImage12);

	lrc.left = m_nLeft2 + 1;
	lrc.right = pImage->GetWidth();
	pImage->Crop(lrc, pDrawImage13);
	m_vecImage.push_back(pDrawImage13);

	lrc.left = 0;
	lrc.top = m_nTop1 + 1;
	lrc.right = m_nLeft1;
	lrc.bottom = m_nTop2;
	pImage->Crop(lrc, pDrawImage21);
	m_vecImage.push_back(pDrawImage21);

	lrc.left = m_nLeft1 + 1;
	lrc.right = m_nLeft2;
	pImage->Crop(lrc, pDrawImage22);
	m_vecImage.push_back(pDrawImage22);

	lrc.left = m_nLeft2 + 1;
	lrc.right = pImage->GetWidth();
	pImage->Crop(lrc, pDrawImage23);
	m_vecImage.push_back(pDrawImage23);

	lrc.left = 0;
	lrc.top = m_nTop2 + 1;
	lrc.right = m_nLeft1;
	lrc.bottom = pImage->GetHeight();
	pImage->Crop(lrc, pDrawImage31);
	m_vecImage.push_back(pDrawImage31);

	lrc.left = m_nLeft1 + 1;
	lrc.right = m_nLeft2;
	pImage->Crop(lrc, pDrawImage32);
	m_vecImage.push_back(pDrawImage32);

	lrc.left = m_nLeft2 + 1;
	lrc.right = pImage->GetWidth();
	pImage->Crop(lrc, pDrawImage33);
	m_vecImage.push_back(pDrawImage33);
}

void CUIBitmapNineInOne::Render(HDC dc, const RECT& rc, BOOL /*bStretch*/)
{
	CropBitmap();
	RECT lrc = {0};
	lrc.left = rc.left;
	lrc.top = rc.top;
	lrc.right = m_nLeft1 + rc.left;
	lrc.bottom = m_nTop1 + rc.top;
	m_vecImage[0]->Draw(dc, lrc);
	lrc.left = lrc.right;
	lrc.right = rc.right - (m_pImage->GetWidth() - m_nLeft2) + 1;
	m_vecImage[1]->Draw(dc, lrc);
	lrc.left = lrc.right;
	lrc.right = lrc.left + (m_pImage->GetWidth() - m_nLeft2) - 1;
	m_vecImage[2]->Draw(dc, lrc);

	lrc.left = rc.left;
	lrc.top = lrc.bottom;
	lrc.right = m_nLeft1 + rc.left;
	lrc.bottom = rc.bottom - (m_pImage->GetHeight() - m_nTop2) + 1;
	m_vecImage[3]->Draw(dc, lrc);
	lrc.left = lrc.right;
	lrc.right = rc.right - (m_pImage->GetWidth() - m_nLeft2) + 1;
	m_vecImage[4]->Draw(dc, lrc);
	lrc.left = lrc.right;
	lrc.right = lrc.left + (m_pImage->GetWidth() - m_nLeft2) - 1;
	m_vecImage[5]->Draw(dc, lrc);

	lrc.left = rc.left;
	lrc.top = lrc.bottom;
	lrc.right = m_nLeft1 + rc.left;
	lrc.bottom = rc.bottom;
	m_vecImage[6]->Draw(dc, lrc);
	lrc.left = lrc.right;
	lrc.right = rc.right - (m_pImage->GetWidth() - m_nLeft2) + 1;
	m_vecImage[7]->Draw(dc, lrc);
	lrc.left = lrc.right;
	lrc.right = lrc.left + (m_pImage->GetWidth() - m_nLeft2) - 1;
	m_vecImage[8]->Draw(dc, lrc);
}

void CUIBitmap::Render(HDC dc, const RECT& rc, BOOL bStretch)
{
	ATLASSERT(NULL != m_pBitmap);
	if(NULL == m_pBitmap)
		return;
	m_pBitmap->Render(dc, rc, bStretch);
}