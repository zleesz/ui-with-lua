#pragma once
#include <stdio.h>
#include <Util.h>
#include <UIParser.h>

typedef struct _tagPackage
{
	std::string strName;
	std::string strAuthor;
	std::string strCopyRight;

	std::string strPackagePath;
	std::string strResPath;
	std::string strCodePath;
	std::string strOnload;
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
	const std::string& SetSkinPath(CComBSTR bstrPath);
private:
	BOOL PreCheckSkinValid();
	BOOL CheckSkinValid();
	void ParserSkinXml(LPXMLDOMNode pNode);
public:
	LOG_CLS_DEC();
};
