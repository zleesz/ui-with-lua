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

class CUITreeContainer;
typedef enum enumControlType {
	CT_UIImage = 1,
	CT_UIButton,
} ControlType;

class CUIControlBase
{
public:
	CUIControlBase(void);
	CUIControlBase(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	virtual ~CUIControlBase(void);
private:
	std::string m_strID;
	CUITreeContainer* m_pTree;
public:
	typedef std::map<std::string, CComVariant> ID2AttrMap;
	ID2AttrMap m_mapAttr;
	CUIEventControl* m_pUIEventControl;
private:
	void SetID(std::string strID);
public:
	virtual ControlType GetType() = 0;
	virtual void Render(CDCHandle dc) = 0;
	virtual void OnInitControl() = 0;
	virtual BOOL OnHitTest(int x, int y) = 0;
	/*virtual void OnLButtonDown(int x, int y) = 0;
	virtual void OnLButtonUp(int x, int y) = 0;
	virtual void OnLButtonDbClick(int x, int y) = 0;
	virtual void OnMouseMove(int x, int y) = 0;
	virtual void OnMouseLeave(int x, int y) = 0;
	virtual void OnMouseWheel(int x, int y) = 0;*/
	const std::string& GetID();
	void SetAttr(std::string strName, std::string strValue);
	void GetAttr(std::string strName, VARIANT* v);
	const CUITreeContainer* GetOwnerTree();
};