#include "StdAfx.h"
#include ".\uibitmap.h"

CUIBitmap::CUIBitmap(void)
{
}

CUIBitmap::~CUIBitmap(void)
{
	UnRegisterClass(this);
	if(NULL != m_BitmapData.pImage)
	{
		delete m_BitmapData.pImage;
		m_BitmapData.pImage = NULL;
	}
	int nSize = m_vecImage.size();
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

CUIBitmap::CUIBitmap(LPXMLDOMNode pNode, const char* pszPath)
	: CUIResBase(pNode)
{
	RegisterClass(this);
	for(int i = 0; i < 4; i++)
	{
		m_arrPos[i] = -1;
	}
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
	m_BitmapData.strPath = szPath;
	std::string strType = (*pMapAttr)["type"];
	if(strType == "ThreeInOneH")
	{
		m_BitmapData.nType = BT_THREEINONEH;
	}
	else if(strType == "ThreeInOneV")
	{
		m_BitmapData.nType = BT_THREEINONEV;
	}
	else if(strType == "NineInOne")
	{
		m_BitmapData.nType = BT_NINEINONE;
	}
}

ResourceType CUIBitmap::GetType()
{
	return RT_UIBITMAP;
}

int CUIBitmap::GetID(lua_State* L)
{
	CUIResBase* pThis = (CUIResBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	lua_pushstring(L, pThis->GetID());
	return 1;
}

CxImage* CUIBitmap::GetImage()
{
	if(m_BitmapData.pImage != NULL)
		return m_BitmapData.pImage;
	ATLASSERT(::PathFileExistsA(m_BitmapData.strPath.c_str()));
	if(!::PathFileExistsA(m_BitmapData.strPath.c_str()))
		return NULL;
	CxImage* pImage = new CxImage;
	m_BitmapData.pImage = pImage;
	std::wstring wstrPath;
	Util::StringToWideString(m_BitmapData.strPath.c_str(), wstrPath);
	pImage->Load(wstrPath.c_str(), CXIMAGE_FORMAT_PNG);
	return pImage;
}

void CUIBitmap::GetVLeftPos(CxImage* pImage)
{
	BYTE	*lpSrc;
	uint8_t	*uiData = pImage->GetBits();
	uint8_t uWidth = pImage->GetWidth();
	uint8_t uHeight = pImage->GetHeight();
	uint8_t uWidthBytes = WIDTHBYTES(uWidth * 8);

	for(int j = 0; j < uWidth; j++)
	{
		for (int i = 0; i < uHeight; i++)
		{
			lpSrc = (BYTE*)uiData + uWidthBytes * i * 3 + j * 3;
			if(lpSrc[0] != 0xff || lpSrc[1] != 0 || lpSrc[2] != 0xff)
			{
				break;
			}
			if(i == uHeight - 1)
			{
				if(m_arrPos[0] < 0)
					m_arrPos[0] = j;
				else if(m_arrPos[1] < 0)
					m_arrPos[1] = j;
			}
		}
		if(m_arrPos[1] >= 0)
			break;
	}
}

void CUIBitmap::GetHTopPos(CxImage* pImage)
{
	BYTE	*lpSrc;
	uint8_t	*uiData = pImage->GetBits();
	uint8_t uWidth = pImage->GetWidth();
	uint8_t uHeight = pImage->GetHeight();
	uint8_t uWidthBytes = WIDTHBYTES(uWidth * 8);

	for (int i = 0; i < uHeight; i++)
	{
		for(int j = 0; j < uWidth; j++)
		{
			lpSrc = (BYTE*)uiData + uWidthBytes * i * 3 + j * 3;
			if(lpSrc[0] != 0xff || lpSrc[1] != 0 || lpSrc[2] != 0xff)
			{
				break;
			}
			if(j == uWidth - 1)
			{
				if(m_arrPos[2] < 0)
					m_arrPos[2] = i;
				else if(m_arrPos[3] < 0)
					m_arrPos[3] = i;
			}
		}
		if(m_arrPos[3] >= 0)
			break;
	}
	int top1 = uHeight - m_arrPos[3] - 1;
	m_arrPos[3] = uHeight - m_arrPos[2] - 1;
	m_arrPos[2] = top1;
}

void CUIBitmap::Render(HDC dc, const RECT& rc, BOOL bStretch)
{
	CxImage* pImage = GetImage();
	if(NULL == pImage)
		return;
	if(m_vecImage.size() <= 0)
	{
		if(BT_THREEINONEH == m_BitmapData.nType)
		{
			GetVLeftPos(pImage);
			CxImage *pDrawImage1 = new CxImage();
			CxImage *pDrawImage2 = new CxImage();
			CxImage *pDrawImage3 = new CxImage();
			RECT lrc = {0};
			lrc.left = 0;
			lrc.top = 0;
			lrc.right = m_arrPos[0];
			lrc.bottom = pImage->GetHeight();
			pImage->Crop(lrc, pDrawImage1);
			m_vecImage.push_back(pDrawImage1);
			lrc.left = m_arrPos[0] + 1;
			lrc.right = m_arrPos[1];
			pImage->Crop(lrc, pDrawImage2);
			m_vecImage.push_back(pDrawImage2);
			lrc.left = m_arrPos[1] + 1;
			lrc.right = pImage->GetWidth();
			pImage->Crop(lrc, pDrawImage3);
			m_vecImage.push_back(pDrawImage3);
		}
		else if(BT_THREEINONEV == m_BitmapData.nType)
		{
		}
		else if(BT_NINEINONE == m_BitmapData.nType)
		{
			GetVLeftPos(pImage);
			GetHTopPos(pImage);
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
			lrc.right = m_arrPos[0];
			lrc.bottom = m_arrPos[2];
			pImage->Crop(lrc, pDrawImage11);
			m_vecImage.push_back(pDrawImage11);

			lrc.left = m_arrPos[0] + 1;
			lrc.right = m_arrPos[1];
			pImage->Crop(lrc, pDrawImage12);
			m_vecImage.push_back(pDrawImage12);

			lrc.left = m_arrPos[1] + 1;
			lrc.right = pImage->GetWidth();
			pImage->Crop(lrc, pDrawImage13);
			m_vecImage.push_back(pDrawImage13);

			lrc.left = 0;
			lrc.top = m_arrPos[2] + 1;
			lrc.right = m_arrPos[0];
			lrc.bottom = m_arrPos[3];
			pImage->Crop(lrc, pDrawImage21);
			m_vecImage.push_back(pDrawImage21);

			lrc.left = m_arrPos[0] + 1;
			lrc.right = m_arrPos[1];
			pImage->Crop(lrc, pDrawImage22);
			m_vecImage.push_back(pDrawImage22);

			lrc.left = m_arrPos[1] + 1;
			lrc.right = pImage->GetWidth();
			pImage->Crop(lrc, pDrawImage23);
			m_vecImage.push_back(pDrawImage23);

			lrc.left = 0;
			lrc.top = m_arrPos[3] + 1;
			lrc.right = m_arrPos[0];
			lrc.bottom = pImage->GetHeight();
			pImage->Crop(lrc, pDrawImage31);
			m_vecImage.push_back(pDrawImage31);

			lrc.left = m_arrPos[0] + 1;
			lrc.right = m_arrPos[1];
			pImage->Crop(lrc, pDrawImage32);
			m_vecImage.push_back(pDrawImage32);

			lrc.left = m_arrPos[1] + 1;
			lrc.right = pImage->GetWidth();
			pImage->Crop(lrc, pDrawImage33);
			m_vecImage.push_back(pDrawImage33);
		}
		else
		{
			m_vecImage.push_back(pImage);
		}
	}
	if (m_vecImage.size() <= 0)
	{
		return;
	}
	if(BT_THREEINONEH == m_BitmapData.nType)
	{
		RECT lrc = {0};
		lrc.left = rc.left;
		lrc.top = rc.top;
		lrc.right = m_arrPos[0] + rc.left;
		if(bStretch)
		{
			lrc.bottom = rc.bottom;
		}
		else
		{
			lrc.bottom = pImage->GetHeight() + rc.top;
		}
		m_vecImage[0]->Draw(dc, lrc);
		lrc.left = lrc.right;
		lrc.right = rc.right - (pImage->GetWidth() - m_arrPos[1]) + 1;
		m_vecImage[1]->Draw(dc, lrc);
		lrc.left = lrc.right;
		lrc.right = lrc.left + (pImage->GetWidth() - m_arrPos[1]) - 1;
		m_vecImage[2]->Draw(dc, lrc);
	}
	else if(BT_THREEINONEV == m_BitmapData.nType)
	{
	}
	else if(BT_NINEINONE == m_BitmapData.nType)
	{
		RECT lrc = {0};
		lrc.left = rc.left;
		lrc.top = rc.top;
		lrc.right = m_arrPos[0] + rc.left;
		lrc.bottom = m_arrPos[2] + rc.top;
		m_vecImage[0]->Draw(dc, lrc);
		lrc.left = lrc.right;
		lrc.right = rc.right - (pImage->GetWidth() - m_arrPos[1]) + 1;
		m_vecImage[1]->Draw(dc, lrc);
		lrc.left = lrc.right;
		lrc.right = lrc.left + (pImage->GetWidth() - m_arrPos[1]) - 1;
		m_vecImage[2]->Draw(dc, lrc);

		lrc.left = rc.left;
		lrc.top = lrc.bottom;
		lrc.right = m_arrPos[0] + rc.left;
		lrc.bottom = rc.bottom - (pImage->GetHeight() - m_arrPos[3]) + 1;
		m_vecImage[3]->Draw(dc, lrc);
		lrc.left = lrc.right;
		lrc.right = rc.right - (pImage->GetWidth() - m_arrPos[1]) + 1;
		m_vecImage[4]->Draw(dc, lrc);
		lrc.left = lrc.right;
		lrc.right = lrc.left + (pImage->GetWidth() - m_arrPos[1]) - 1;
		m_vecImage[5]->Draw(dc, lrc);

		lrc.left = rc.left;
		lrc.top = lrc.bottom;
		lrc.right = m_arrPos[0] + rc.left;
		lrc.bottom = rc.bottom;
		m_vecImage[6]->Draw(dc, lrc);
		lrc.left = lrc.right;
		lrc.right = rc.right - (pImage->GetWidth() - m_arrPos[1]) + 1;
		m_vecImage[7]->Draw(dc, lrc);
		lrc.left = lrc.right;
		lrc.right = lrc.left + (pImage->GetWidth() - m_arrPos[1]) - 1;
		m_vecImage[8]->Draw(dc, lrc);
	}
	else
	{
		pImage->Draw(dc, rc);
	}
}