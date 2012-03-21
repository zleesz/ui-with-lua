#include "StdAfx.h"
#include "CodeXmlParser.h"
#include "UIWindowFactory.h"

CCodeXmlParser::CCodeXmlParser(void)
{
}

CCodeXmlParser::~CCodeXmlParser(void)
{
}

BOOL CCodeXmlParser::ParserCodeXml(const char* szPath)
{
	LOG_AUTO();
	CXmlParser xmlParser;
	LPXMLDOMNode pNode = NULL;
	BOOL bRet = xmlParser.SetXmlPath(szPath, pNode);
	if(bRet)
	{
		bRet = ParserCodeDOM(szPath, pNode);
	}
	ATLASSERT(bRet && "code xml parser failed!");
	LOG_DEBUG(_T("bRet=") << bRet << _T(", szpath") << szPath);
	return bRet;
}

BOOL CCodeXmlParser::ParserCodeDOM(const std::string& strPath, LPXMLDOMNode pNode)
{
	if(pNode == NULL || pNode->pMapChildNode == NULL)
		return FALSE;
	LPXMLDOMNode pFirstNode = (*pNode->pMapChildNode)["ui"];
	if(pFirstNode == NULL)
		return FALSE;
	LPXMLChildNodes pChildNodes = pFirstNode->pMapChildNode;
	XMLChildNodes::const_iterator it = pChildNodes->begin();
	for(; it != pChildNodes->end(); it ++)
	{
		if(it->first == "hostwnd")
		{
			CUIWindowFactory::GetInstance()->ParserWindowDOM(strPath, it->second);
		}
	}
	return TRUE;
}

BOOL CCodeXmlParser::ParserDirectory(const std::string strPath)
{
	LOG_AUTO();
	BOOL bRet = FALSE;
	ATLASSERT(::PathFileExistsA(strPath.c_str()));
	ATLASSERT(::PathIsDirectoryA(strPath.c_str()));
	if(::PathFileExistsA(strPath.c_str()) && ::PathIsDirectoryA(strPath.c_str()))
	{
		//遍历目录下的xml 
		char szFindPath[MAX_PATH] = {0}; 
		strncpy_s(szFindPath, strPath.c_str(), MAX_PATH); 
		::PathAppendA(szFindPath, "*.xml"); // 找所有.xml文件 
		WIN32_FIND_DATAA wfd; 
		HANDLE hFind = ::FindFirstFileA(szFindPath, &wfd); 
		if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败 
			return bRet;
		do
		{ 
			if (wfd.cFileName[0] == '.')
				continue; // 过滤这两个目录 
			char szFile[MAX_PATH] = {0};
			strncpy_s(szFile, strPath.c_str(), MAX_PATH); 
			::PathAppendA(szFile, wfd.cFileName);
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// 递归 遍历目录
				bRet = ParserDirectory(szFile);
			}
			else
			{
				bRet = ParserCodeXml(szFile);
			}
			if(!bRet)
				break;
		}while(::FindNextFileA(hFind, &wfd));
		::FindClose(hFind);
	}
	return bRet;
}

BOOL CCodeXmlParser::SetCodePath(std::string strPath)
{
	LOG_AUTO();
	BOOL bRet = FALSE;
	m_strPath = strPath;
	bRet = ParserDirectory(m_strPath);
	return bRet;
}
/*
void CCodeXmlParser::ElementStart(void *userData, const std::string& strName, XMLAttrMap& mAttr, const XMLNodeVector& vecNode)
{
	CUIWindowFactory::GetInstance()->ElementStart(strName, mAttr, vecNode);
}

void CCodeXmlParser::ElementEnd(void *userData, const std::string& strName, const XMLNodeVector& vecNode)
{
	UNREFERENCED_PARAMETER(userData);
	UNREFERENCED_PARAMETER(strName);
	UNREFERENCED_PARAMETER(vecNode);
}

void CCodeXmlParser::CharacterData(void *userData, const std::string& strName, const std::string& strUData, const XMLNodeVector& vecNode)
{
	UNREFERENCED_PARAMETER(userData);
	CUIWindowFactory::GetInstance()->CharacterData(strName, strUData, vecNode);
}
*/