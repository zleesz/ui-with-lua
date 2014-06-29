#pragma once
#include "uicontrolbase.h"
#include <uiluax.h>
#include <Util.h>
#include "UITreeContainer.h"

class CUICaption :
	public CUIControlBase
{
public:
	CUICaption(void);
	CUICaption(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	CUICaption(CUITreeContainer* pTree);
	virtual ~CUICaption(void);
public:
	UICONTROL_GETTYPE_METHOD(CT_UICaption);
	void Render(CDCHandle dc);
	virtual void OnInitControl();
	virtual void OnDetroy();
	virtual BOOL OnHitTest(int x, int y, LONG& nHitTest);
	virtual void SetAttr(const std::string& /*strName*/, const std::string& /*strValue*/);
public:
	BEGIN_LUA_CALL_MAP(CUICaption)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY(GetOwnerTree)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUICaption, UI.Caption);
public:
	LOG_CLS_DEC();
};