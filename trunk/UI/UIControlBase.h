#pragma once
#include <atlapp.h>
#include <atlgdi.h>
#include <string>
#include <map>
#include <atlapp.h>
#include <atlwin.h>
#include <winuser.h>
#include <atltypes.h>
#include "UIEventControl.h"

typedef enum enumControlType {
	CT_UIImage = 1,
	CT_UIButton,
} ControlType;

class CUIControlBase
{
public:
	CUIControlBase(void);
	CUIControlBase(LPXMLDOMNode pNode);
	virtual ~CUIControlBase(void);
private:
	std::string m_strID;
public:
	typedef std::map<std::string, CComVariant> ID2AttrMap;
	ID2AttrMap m_mapAttr;
	CUIEventControl m_UIEventControl;
private:
	void SetAttr(std::string strName, std::string strValue);
	void SetID(std::string strID);
public:
	virtual ControlType GetType() = 0;
	virtual void Render(CDCHandle dc) = 0;
	const std::string& GetID();
	void GetAttr(std::string strName, VARIANT* v);
};
