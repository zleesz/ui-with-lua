#include "StdAfx.h"
#include "ResXmlParser.h"

CResXmlParser::CResXmlParser(void)
{
}

CResXmlParser::~CResXmlParser(void)
{
}

BOOL CResXmlParser::ParserResXml(const char* szPath)
{
	LOG_AUTO();
	CXmlParser xmlParser;
	LPXMLDOMNode pNode = NULL;
	BOOL bRet = xmlParser.SetXmlPath(szPath, pNode);
	ParserResDOM(pNode, szPath);
	ATLASSERT(bRet && "res xml parser failed!");
	LOG_DEBUG(_T("bRet=") << bRet << _T(", szpath") << szPath);
	return bRet;
}

BOOL CResXmlParser::ParserResDOM(LPXMLDOMNode pNode, const char* pszPath)
{
	if(pNode == NULL || pNode->pMapChildNode == NULL)
		return FALSE;
	LPXMLDOMNode pUINode = (*pNode->pMapChildNode)["ui"];
	if(NULL == pUINode)
	{
		ATLASSERT(false);
		LOG_WARN(_T("parser res failed! first node must be ui!"));
		return FALSE;
	}
	LPXMLChildNodes pChildNodes = pUINode->pMapChildNode;
	if(NULL == pChildNodes)
	{
		return TRUE;
	}
	XMLChildNodes::const_iterator it = pChildNodes->begin();
	for(; it != pChildNodes->end(); it++)
	{
		LPXMLVecNodes pVecNodes = it->second->pVecNode;
		for(int i = 0; i < (int)pVecNodes->size(); i++)
		{
			CUIResFactory::GetInstance()->AddResElement((*pVecNodes)[i], pszPath);
		}
	}
	return TRUE;
}

BOOL CResXmlParser::SetResPath(std::string strPath)
{
	LOG_AUTO();
	BOOL bRet = FALSE;
	m_strPath = strPath;
	ATLASSERT(::PathFileExistsA(m_strPath.c_str()));
	ATLASSERT(::PathIsDirectoryA(m_strPath.c_str()));
	if(::PathFileExistsA(m_strPath.c_str()) && ::PathIsDirectoryA(m_strPath.c_str()))
	{
		//遍历目录下的xml 
		char szFindPath[MAX_PATH] = {0}; 
		strncpy_s(szFindPath, m_strPath.c_str(), MAX_PATH); 
		::PathAppendA(szFindPath, "*.xml"); // 找所有.xml文件 
		WIN32_FIND_DATAA wfd; 
		HANDLE hFind = ::FindFirstFileA(szFindPath, &wfd); 
		if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败 
			return bRet;
		do
		{ 
			if (wfd.cFileName[0] == '.')
				continue; // 过滤这两个目录 
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			}
			else
			{
				char szFile[MAX_PATH] = {0};
				strncpy_s(szFile, m_strPath.c_str(), MAX_PATH); 
				::PathAppendA(szFile, wfd.cFileName);
				bRet = ParserResXml(szFile);
				if(!bRet)
					break;
			}
		}while(::FindNextFileA(hFind, &wfd));
		::FindClose(hFind);
	}
	return bRet;
}
