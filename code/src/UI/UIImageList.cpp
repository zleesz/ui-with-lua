#include "StdAfx.h"
#include "UIImageList.h"
#include "UIResFactory.h"

CUIImagelist::~CUIImagelist(void)
{
	UnRegisterClass(this);
	if(m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
	for(int i = 0; i < (int)m_VecImage.size(); i++)
	{
		delete m_VecImage[i];
		m_VecImage[i] = NULL;
	}
	m_VecImage.clear();
	m_VecBitmap.clear();
}

CUIImagelist::CUIImagelist(LPXMLDOMNode pNode, const char* pszPath)
	: CUIResBase(pNode), m_uBlockSize(0), m_bSeparator(FALSE), m_bVertical(FALSE), m_VecBitmap(NULL), m_VecImage(NULL), m_pImage(NULL)
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

	std::string strSeperator = (*pMapAttr)["seperator"];
	std::string strBlockSize = (*pMapAttr)["blocksize"];
	std::string strVertical = (*pMapAttr)["vertical"];
	if(strSeperator == "1" || strSeperator == "true")
	{
		m_bSeparator = TRUE;
	}
	else if(strBlockSize.length() > 0)
	{
		m_uBlockSize = atoi(strBlockSize.c_str());
	}
	else
	{
		ATLASSERT(FALSE);
	}
	if(strVertical == "1" || strVertical == "true")
	{
		m_bVertical = TRUE;
	}
	LPXMLChildNodes pChildNodes = pNode->pMapChildNode;
	if(NULL == pChildNodes)
	{
		return;
	}
	XMLChildNodes::const_iterator it = pChildNodes->find("bitmap");
	LPXMLVecNodes pVecNodes = it->second->pVecNode;
	if(NULL == pVecNodes)
	{
		return;
	}
	XMLVecNodes::const_iterator it2 = pVecNodes->begin();
	for(; it2 != pVecNodes->end(); it2++)
	{
		LPXMLDOMNode pChildNode = *it2;
		CUIBitmap* pBitmap = new CUIBitmap(pChildNode, this);
		m_VecBitmap.push_back(pBitmap);
		UIResFactoryInstance->AddRes(pBitmap);
	}
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

CxImage* CUIImagelist::GetImage()
{
	if(m_pImage == NULL)
	{
		ATLASSERT(::PathFileExistsA(m_strPath.c_str()));
		if(!::PathFileExistsA(m_strPath.c_str()))
			return NULL;
		m_pImage = new CxImage;
		std::wstring wstrPath;
		Util::StringToWideString(m_strPath.c_str(), wstrPath);
		m_pImage->Load(wstrPath.c_str(), CXIMAGE_FORMAT_PNG);
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
		int nBlock = uWidth / m_uBlockSize + 0.5;
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
	CxImage* pImage = m_VecImage[nIndex];
	if(pImage)
	{
		return pImage;
	}
	else
	{
		
	}
}