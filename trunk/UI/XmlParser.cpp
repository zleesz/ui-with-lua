#include "StdAfx.h"
#include ".\xmlparser.h"
#include "ResXmlParser.h"
#include "CodeXmlParser.h"

#define XML_FMT_INT_MOD "l"

BOOL CUIXmlParser::PreCheckSkinValid()
{
	LOG_AUTO();
	if(!::PathFileExistsA(m_strPath.c_str()))
	{
		return FALSE;
	}

	char szXmlPath[MAX_PATH] = {0};
	strncpy_s(szXmlPath, m_strPath.c_str(), MAX_PATH);
	::PathAppendA(szXmlPath, "\\package.xml");
	ATLASSERT(::PathFileExistsA(szXmlPath));
	if(::PathFileExistsA(szXmlPath))
	{
		m_package.strPackagePath = szXmlPath;
		return TRUE;
	}
	return FALSE;
}

BOOL CUIXmlParser::CheckSkinValid()
{
	LOG_AUTO();
	if(m_package.strResPath.length() <= 0 ||
		m_package.strCodePath.length() <= 0 ||
		m_package.strOnload.length() <= 0)
	{
		return FALSE;
	}
	if(!::PathFileExistsA(m_package.strResPath.c_str()) ||
		!::PathFileExistsA(m_package.strCodePath.c_str()) ||
		!::PathFileExistsA(m_package.strOnload.c_str()))
	{
		return FALSE;
	}
	return TRUE;
}

const std::string& CUIXmlParser::SetSkinPath(CComBSTR bstrPath)
{
	LOG_AUTO();
	BOOL bRet = FALSE;
	Util::BSTRToString(bstrPath, m_strPath);
	BOOL bValid = PreCheckSkinValid();
	ATLASSERT(bValid);
	if(!bValid)
	{
		return FALSE;
	}
	CXmlParser xmlParser;
	LPXMLDOMNode pNode = NULL;
	bRet = xmlParser.SetXmlPath(m_package.strPackagePath, pNode);
	if(bRet)
	{
		ParserSkinXml(pNode);
		bRet = CheckSkinValid();
		ATLASSERT(bRet && "invalid skin package!");
		if(bRet)
		{
			// ½âÎö×ÊÔ´xml...
			CResXmlParser resParser;
			bRet = resParser.SetResPath(m_package.strResPath);
			if(bRet)
			{
				CCodeXmlParser codeParser;
				bRet = codeParser.SetCodePath(m_package.strCodePath);
			}
		}
	}
	return m_package.strOnload;
}

CUIXmlParser::~CUIXmlParser(void)
{
}

void CUIXmlParser::ParserSkinXml(LPXMLDOMNode pNode)
{
	if(pNode == NULL || pNode->pMapChildNode == NULL)
		return;
	LPXMLDOMNode pConfigNode = (*pNode->pMapChildNode)["config"];
	if(pConfigNode)
	{
		LPXMLAttrMap pMapAttr = pConfigNode->pMapAttr;
		if(pMapAttr)
		{
			if((*pMapAttr)["name"].length() > 0)
			{
				m_package.strName = (*pMapAttr)["name"];
			}
			if((*pMapAttr)["author"].length() > 0)
			{
				m_package.strAuthor = (*pMapAttr)["author"];
			}
			if((*pMapAttr)["copyright"].length() > 0)
			{
				m_package.strCopyRight = (*pMapAttr)["copyright"];
			}
		}
		if(pConfigNode->pMapChildNode)
		{
			LPXMLDOMNode pResNode = (*pConfigNode->pMapChildNode)["res"];
			if(pResNode && pResNode->pMapAttr)
			{
				if((*pResNode->pMapAttr)["package"].length() > 0)
				{
					char szPath[MAX_PATH] = {0};
					strncpy_s(szPath, m_strPath.c_str(), MAX_PATH);
					::PathAppendA(szPath, (*pResNode->pMapAttr)["package"].c_str());
					::PathAddBackslashA(szPath);
					m_package.strResPath = szPath;
				}
			}
			LPXMLDOMNode pScriptNode = (*pConfigNode->pMapChildNode)["script"];
			if(pScriptNode && pScriptNode->pMapAttr)
			{
				if((*pScriptNode->pMapAttr)["package"].length() > 0)
				{
					char szPath[MAX_PATH] = {0};
					strncpy_s(szPath, m_strPath.c_str(), MAX_PATH);
					::PathAppendA(szPath, (*pScriptNode->pMapAttr)["package"].c_str());
					::PathAddBackslashA(szPath);
					m_package.strCodePath = szPath;
				}
				if((*pScriptNode->pMapAttr)["onload"].length() > 0)
				{
					char szPath[MAX_PATH] = {0};
					if(m_package.strCodePath.length() > 0)
					{
						strncpy_s(szPath, m_package.strCodePath.c_str(), MAX_PATH);
					}
					else
					{
						strncpy_s(szPath, m_strPath.c_str(), MAX_PATH);
					}
					::PathAppendA(szPath, (*pScriptNode->pMapAttr)["onload"].c_str());
					m_package.strOnload = szPath;
				}
			}
		}
	}
}