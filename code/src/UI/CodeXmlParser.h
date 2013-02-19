#pragma once
#include <Util.h>
#include <string>
#include <UIParser.h>

class CCodeXmlParser
{
public:
	CCodeXmlParser(void);
	virtual ~CCodeXmlParser(void);
private:
	std::string m_strPath;
private:
	BOOL ParserDirectory(const std::string strPath);
	BOOL ParserCodeXml(const char* szPath);
public:
	BOOL SetCodePath(std::string strPath);
	BOOL ParserCodeDOM(const std::string& strPath, LPXMLDOMNode pNode);
	/*
	// IXmlParser
	void ElementStart(void *userData, const std::string& strName, XMLAttrMap& mAttr, const XMLNodeVector& vecNode);
	void ElementEnd(void *userData, const std::string& strName, const XMLNodeVector& vecNode);
	void CharacterData(void *userData, const std::string& strName, const std::string& strUData, const XMLNodeVector& vecNode);
	*/
public:
	LOG_CLS_DEC();
};
