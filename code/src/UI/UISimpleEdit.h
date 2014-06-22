#pragma once
#include "uicontrolbase.h"
#include "UIWindowBase.h"
#include "UITreeContainer.h"

class CUISimpleEdit :
	public CUIControlBase
{
public:
	CUISimpleEdit(void);
public:
	CUISimpleEdit(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	CUISimpleEdit(CUITreeContainer* pTree);
	virtual ~CUISimpleEdit(void);
public:
	UICONTROL_GETTYPE_METHOD(CT_UISimpleEdit);
	virtual void Render(CDCHandle dc);
	virtual void OnInitControl();
	virtual void OnDetroy();
private:
	void SetAttr(const std::string& strName, const std::string& strValue);
private:
	std::string m_strText;
public:
	BEGIN_LUA_CALL_MAP(CUISimpleEdit)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY(GetVisible)
		LUA_CALL_ENTRY(SetVisible)
		LUA_CALL_ENTRY(GetEnable)
		LUA_CALL_ENTRY(GetOwnerTree)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUISimpleEdit, UI.SimpleEdit);
public:
	LOG_CLS_DEC();
};
