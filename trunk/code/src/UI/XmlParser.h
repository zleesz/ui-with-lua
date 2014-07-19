#pragma once
#include <stdio.h>
#include <list>
#include <Util.h>
#include <UIParser.h>
#include "ResXmlParser.h"
#include "CodeXmlParser.h"

typedef struct _tagPackage
{
	std::string strName;
	std::string strAuthor;
	std::string strCopyRight;

	std::string strPackagePath;
	std::string strResPath;
	std::string strCodePath;
	std::string strOnload;

	CResXmlParser resParser;
	CCodeXmlParser codeParser;
} Package;

class CUIXmlParser
{
public:
	CUIXmlParser(){};
	virtual ~CUIXmlParser(void);
private:
	std::string	m_strPath;
	Package m_package;
public:
	BOOL SetSkinPath(CComBSTR bstrPath, std::string& strOnload);
private:
	BOOL PreCheckSkinValid();
	BOOL CheckSkinValid();
	void ParserSkinXml(LPXMLDOMNode pNode);
public:
	LOG_CLS_DEC();
};
