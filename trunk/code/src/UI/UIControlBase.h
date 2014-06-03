#pragma once
#include <atlapp.h>
#include <atlgdi.h>
#include <string>
#include <map>
#include <atlapp.h>
#include <atlwin.h>
#include <winuser.h>
#include <atltypes.h>
#include "UIEventCtrlContainer.h"
#include "UIGraphic.h"

class CUITreeContainer;
typedef enum enumControlType {
	CT_UIImage = 1,
	CT_UITexture,
	CT_UIButton,
} ControlType;

class CUIControlBase
{
public:
	CUIControlBase(void);
	CUIControlBase(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	CUIControlBase(CUITreeContainer* pTree);
	virtual ~CUIControlBase(void);
private:
	std::string m_strID;
	CUITreeContainer* m_pTree;
public:
	typedef std::map<std::string, CComVariant> ID2AttrMap;
	ID2AttrMap m_mapAttr;
	CUIEventCtrlContainer* m_pUIEventControl;
protected:
	void FireUIEvent(std::string strName, int x, int y);
public:
	virtual ControlType GetType() = 0;
	virtual void Render(CDCHandle dc) = 0;
	virtual void OnInitControl();
	virtual void OnDetroy();
	virtual void OnLButtonDown(int x, int y);
	virtual void OnLButtonUp(int x, int y);
	virtual void OnLButtonDbClick(int x, int y);
	virtual void OnMouseMove(int x, int y);
	virtual void OnMouseLeave(int x, int y);
	virtual void OnMouseWheel(int x, int y);
public:
	const std::string& GetID();
	std::string SetID(const std::string& strID);
	void SetAttr(std::string strName, std::string strValue);
	void GetAttr(std::string strName, VARIANT* v);
	CUITreeContainer* GetOwnerTree();
	ULONG GetZorder();
	BOOL OnHitTest(int x, int y);
	BOOL GetVisible();
	BOOL GetEnable();
	const RECT& GetObjPos();
	void Invalidate();
	void SetCaptureMouse(BOOL bCapture);
public:
	LOG_CLS_DEC();
};

#define UICONTROL_DEFAULT_MOUSE_METHOD(method) \
	virtual void method(int x, int y) { \
		UNREFERENCED_PARAMETER(x); \
		UNREFERENCED_PARAMETER(y); \
	}
#define UICONTROL_GETTYPE_METHOD(x) \
	virtual ControlType GetType() {\
		return x; \
	}