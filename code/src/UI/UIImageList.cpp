#include "StdAfx.h"
#include "UIImageList.h"
#include "UIResFactory.h"

CUIImagelist::~CUIImagelist(void)
{
	UnRegisterClass(this);
	if (m_pBitmapList)
	{
		delete m_pBitmapList;
		m_pBitmapList = NULL;
	}
}

CUIImagelist::CUIImagelist(LPXMLDOMNode pNode, const char* pszPath)
	: CUIResBase(pNode), m_pBitmapList(NULL)
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
	m_strPath = szPath;

	Bitmap* pBitmap = ImageLoaderFactory::Instance()->LoadImageFromFile(m_strPath.c_str(), 0);
	m_pBitmapList = new BitmapList(pBitmap);
}

ResourceType CUIImagelist::GetType()
{
	return RT_UIIMAGELIST;
}

int CUIImagelist::GetID(lua_State* L)
{
	CUIResBase* pThis = (CUIResBase*) lua_touserdata(L, -1);
	ATLASSERT(pThis);
	std::string strID = pThis->GetID();
	lua_pushstring(L, strID.c_str());
	return 1;
}

BitmapList* CUIImagelist::GetImageList()
{
	if(m_pBitmapList == NULL)
	{
		ATLASSERT(::PathFileExistsA(m_strPath.c_str()));
		if(!::PathFileExistsA(m_strPath.c_str()))
			return NULL;
		m_pBitmapList = new BitmapList;
		std::wstring wstrPath;
		Util::StringToWideString(m_strPath.c_str(), wstrPath);
	}
	return m_pImage;
}

void CUIImagelist::CropSeperatorBitmap()
{
	CxImage* pImage = GetImage();
	if(NULL == pImage)
	{
		return;
	}
	if(!m_bVertical)
	{
		// horizon
		VecLinePos vecLinePos;
		CImageTool::GetHLeftPos(pImage, vecLinePos);
		RECT lrc = {0};
		lrc.left = 0;
		lrc.top = 0;
		lrc.right = 0;
		lrc.bottom = pImage->GetHeight();
		for(int i = 0; i < (int)vecLinePos.size(); i++)
		{
			CxImage* pSubImage = new CxImage;
			lrc.right = vecLinePos[i];
			pImage->Crop(lrc, pSubImage);
			lrc.left = lrc.right + 1;
			m_VecImage.push_back(pSubImage);
		}
		lrc.right = pImage->GetWidth();
		CxImage* pSubImage = new CxImage;
		pImage->Crop(lrc, pSubImage);
		m_VecImage.push_back(pSubImage);
	}
	else
	{
	}
}

void CUIImagelist::CropBlockBitmap()
{
	if(m_uBlockSize <= 0)
		return;
	CxImage* pImage = GetImage();
	if(NULL == pImage)
	{
		return;
	}
	if(!m_bVertical)
	{
		uint32_t uWidth = pImage->GetWidth();
		int nBlock = (int)(uWidth / m_uBlockSize + 0.5);
		RECT lrc = {0};
		lrc.left = 0;
		lrc.top = 0;
		lrc.right = 0;
		lrc.bottom = pImage->GetHeight();
		for(int i = 0; i < nBlock - 1; i++)
		{
			CxImage* pSubImage = new CxImage;
			lrc.right = (i + 1) * m_uBlockSize;
			pImage->Crop(lrc, pSubImage);
			lrc.left = lrc.right;
			m_VecImage.push_back(pSubImage);
		}
		lrc.right = uWidth;
		CxImage* pSubImage = new CxImage;
		pImage->Crop(lrc, pSubImage);
		m_VecImage.push_back(pSubImage);
		/*CxImage* pNew = new CxImage;
		pNew->Copy((*m_VecImage[0]));
		pNew->Save(_T("C:\\1.png"), CXIMAGE_FORMAT_PNG);*/
	}
	else
	{
	}
}

void CUIImagelist::CropBitmap()
{
	if(m_VecImage.size() > 0)
		return;
	// crop bitmap by attr
	if(m_bSeparator)
	{
		CropSeperatorBitmap();
	}
	else if(m_uBlockSize > 0)
	{
		CropBlockBitmap();
	}
}

CxImage* CUIImagelist::GetImageByIndex(int nIndex)
{
	CropBitmap();
	if(nIndex <= 0 || (int)m_VecImage.size() < nIndex)
		return NULL;
	return m_VecImage[nIndex-1];
}