#pragma once
#include <string>
#include <Util.h>
#include <UIParser.h>
#include "UIGraphic.h"

typedef enum enumResourceType {
	RT_UICOLOR = 1,
	RT_UIFONT,
	RT_UIBITMAP,
	RT_UIIMAGELIST,
	RT_UIICON,
	RT_UITEXTURE
} ResourceType;

class CUIResBase
{
public:
	CUIResBase() : m_strID("") {};
	CUIResBase(LPXMLDOMNode pNode) : m_strID("")
	{
		if(pNode == NULL)
			return;
		LPXMLAttrMap pMapAttr = pNode->pMapAttr;
		if(pMapAttr == NULL)
			return;
		m_strID = (*pMapAttr)["id"];
	};
private:
	std::string m_strID;
public:
	virtual ResourceType GetType() = 0;	// return parser type
	virtual const char* GetPrivateID()
	{
		return m_strID.c_str();
	};
};
