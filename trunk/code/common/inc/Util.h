#pragma once
#include <uilog4cplus.h>
#include <string>
#include <expat.h>
#include <memory>
#include <map>
#include <vector>

namespace Util
{
	static bool BSTRToString( BSTR src, std::string& dest)
	{
		if(!src)
			return false;
		int  iLen = (int)wcslen(src);
		if(iLen > 0)
		{
			char* szdest = new  char[iLen * 4];
			if(NULL == szdest)
				return false;
			ZeroMemory(szdest, iLen * 4);			
			int nLen = WideCharToMultiByte(CP_UTF8, NULL, src, iLen, szdest, iLen * 4, 0, 0);
			szdest[nLen] = '\0'; 
			dest = szdest;
			delete [] szdest ;
			return true;
		}
		return false;
	}
	static bool StringToWideString(const char* src, std::wstring& wstr)
	{
		if(!src)
			return false;
		int iLen = (int)strlen(src);
		wchar_t* szm = new wchar_t[iLen * 4];
		ZeroMemory(szm, iLen * 4);
		int nLen = MultiByteToWideChar(CP_UTF8, 0, src,iLen, szm, iLen*4); 
		szm [nLen] = '\0';
		wstr = szm;
		delete [] szm;
		return false;
	}
	static HRESULT CreateInstanceFromHandle(HMODULE hModuleHandle, REFCLSID rclsid, REFIID riid, void** ppv)
	{
		if(hModuleHandle==NULL) return E_FAIL;

		HRESULT hr = E_FAIL;

		typedef HRESULT (STDAPICALLTYPE *_pfnDllGetClassObject)(REFCLSID , REFIID , LPVOID*);
		_pfnDllGetClassObject pfnDllGetClassObject = (_pfnDllGetClassObject)
			GetProcAddress(hModuleHandle, "DllGetClassObject");
		if(pfnDllGetClassObject == NULL)
		{
			return  hr;
		}

		CComPtr<IClassFactory> spCF;
		hr = (*pfnDllGetClassObject)(rclsid, IID_IClassFactory, (LPVOID*) &spCF);
		if (hr != S_OK)
		{
			return hr;
		}

		// Create instance
		hr = spCF->CreateInstance(NULL, riid, ppv);	
		return hr;
	}
};
typedef std::map<std::string, std::string> XMLAttrMap;
typedef XMLAttrMap* LPXMLAttrMap;
typedef struct tagXMLDOMNode {
	std::string strName;
	std::string strUData;
	LPXMLAttrMap pMapAttr;
	std::map<std::string, tagXMLDOMNode*>* pMapChildNode;
	tagXMLDOMNode* pXMLParentNode;
	std::vector<tagXMLDOMNode*>* pVecNode;
	tagXMLDOMNode() : pMapAttr(NULL), pMapChildNode(NULL), pXMLParentNode(NULL), pVecNode(NULL) {};
}XMLDOMNode, *LPXMLDOMNode;

typedef std::map<std::string, LPXMLDOMNode> XMLChildNodes, *LPXMLChildNodes;
typedef std::vector<tagXMLDOMNode*> XMLVecNodes, *LPXMLVecNodes;

#define BUFFSIZE        8192
class CXmlParser
{
public:
	CXmlParser() : m_pXMLDOMNode(NULL) {};
	~CXmlParser()
	{
		UnInitData(m_pXMLDOMNode);
	};
private:
	XML_Parser			m_xmlParser;
	std::string			m_strPath;
	std::string			m_strCData;
	LPXMLDOMNode		m_pXMLDOMNode;
	LPXMLDOMNode		m_pNode;
private:
	static inline void ConvertMapAttr(const char** attr, XMLAttrMap& mAttr)
	{
		for(int i = 0; attr[i]; i += 2)
		{
			mAttr[ attr[ i ] ] = attr[ i + 1 ];
		}
	}
	void UnInitData(LPXMLDOMNode& pNode)
	{
		if(pNode == NULL)
			return;
		if(pNode->pMapAttr)
		{
			pNode->pMapAttr->clear();
			delete pNode->pMapAttr;
			pNode->pMapAttr = NULL;
		}
		if(pNode->pVecNode)
		{
			for(XMLVecNodes::size_type st = 1; st != pNode->pVecNode->size(); st++)
			{
				LPXMLDOMNode pNode2 = (*pNode->pVecNode)[st];
				UnInitData(pNode2);
			}
			pNode->pVecNode->clear();
		}
		if(pNode->pMapChildNode)
		{
			XMLChildNodes::iterator it = pNode->pMapChildNode->begin();
			while (it != pNode->pMapChildNode->end())
			{
				UnInitData(it->second);
				pNode->pMapChildNode->erase(it);
				it = pNode->pMapChildNode->begin();
			}
			delete pNode->pMapChildNode;
			pNode->pMapChildNode = NULL;
		}
		delete pNode;
		pNode = NULL;
	}
public:
	BOOL SetXmlPath(std::string strPath, LPXMLDOMNode& pNode)
	{
		LOG_AUTO();
		BOOL bRet = FALSE;
		m_strPath = strPath;
		m_pXMLDOMNode = new XMLDOMNode;
		m_pNode = m_pXMLDOMNode;
		m_xmlParser = XML_ParserCreate(NULL);
		ATLASSERT(m_xmlParser);
		XML_SetElementHandler(m_xmlParser, ElementStart, ElementEnd);
		XML_SetUserData(m_xmlParser, this);
		XML_SetCharacterDataHandler(m_xmlParser, CharacterData);
		// ½âÎö xml
		char szBuf[BUFFSIZE] = {0};
		FILE* file = NULL;
		fopen_s(&file, m_strPath.c_str(), "rb");
		while(file)
		{
			int len = (int)fread(szBuf, 1, BUFFSIZE, file);
			int done = feof(file);

			if (XML_Parse(m_xmlParser, szBuf, len, done) == XML_STATUS_ERROR) {
				LOG_ERROR( _T("Parse error at line : ") << 
					XML_GetCurrentLineNumber(m_xmlParser) << 
					", error string : " <<
					XML_ErrorString(XML_GetErrorCode(m_xmlParser)));
				break;
			}
			if (done)
			{
				bRet = TRUE;
				break;
			}
		}
		if(file != NULL)
		{
			fclose(file);
		}
		XML_ParserFree(m_xmlParser);
		m_xmlParser = NULL;
		if(!bRet)
		{
			UnInitData(m_pXMLDOMNode);
		}
		pNode = m_pXMLDOMNode;
		return bRet;
	}
	static void XMLCALL ElementStart(void *userData, const char *el, const char **attr)
	{
		LOG_DEBUG(_T("ElementStart el=") << el);
		CXmlParser* pThis = (CXmlParser*) userData;
		pThis->m_strCData.clear();
		if(pThis->m_pNode->pMapChildNode == NULL)
		{
			pThis->m_pNode->pMapChildNode = new XMLChildNodes;
		}
		LPXMLDOMNode pNode = new XMLDOMNode;
		pNode->pXMLParentNode = pThis->m_pNode;
		LPXMLChildNodes pMapChildNode = pThis->m_pNode->pMapChildNode;
		XMLChildNodes::const_iterator it = pMapChildNode->find(el);
		if(it != pMapChildNode->end())
		{
			LPXMLDOMNode pFirstNode = it->second;
			pFirstNode->pVecNode->push_back(pNode);
		}
		else
		{
			pThis->m_pNode->pMapChildNode->insert(std::make_pair(el, pNode));
			if (pNode->pVecNode == NULL)
			{
				pNode->pVecNode = new XMLVecNodes;
				pNode->pVecNode->push_back(pNode);
			}
		}
		pThis->m_pNode = pNode;
		pThis->m_pNode->strName = el;
		pThis->m_pNode->pMapAttr = new XMLAttrMap;
		ConvertMapAttr(attr, *(pThis->m_pNode->pMapAttr));
	}
	static void XMLCALL ElementEnd(void *userData, const char *el)
	{
		CXmlParser* pThis = (CXmlParser*) userData;
		std::string strName(el);
		if(pThis->m_strCData.length() > 0)
		{
			pThis->m_pNode->strUData = pThis->m_strCData;
		}
		pThis->m_pNode = pThis->m_pNode->pXMLParentNode;
		pThis->m_strCData.clear();
	}
	static void XMLCALL CharacterData(void *userData, const XML_Char *s, int len)
	{
		CXmlParser* pThis = (CXmlParser*) userData;
		pThis->m_strCData.append(s, len);
	}
public:
	LOG_CLS_DEC();
};
