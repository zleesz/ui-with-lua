#pragma once
#include <Util.h>
#include "UIResFactory.h"

class CResXmlParser
{
public:
	CResXmlParser(void);
	virtual ~CResXmlParser(void);
private:
	std::string m_strPath;
public:
	BOOL SetResPath(std::string strPath);
private:
	BOOL ParserResXml(const char* szPath);
	BOOL ParserResDOM(LPXMLDOMNode pNode, const char* pszPath);
public:
	LOG_CLS_DEC();
};
