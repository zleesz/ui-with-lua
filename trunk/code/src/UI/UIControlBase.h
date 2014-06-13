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
	CT_UIText,
	CT_UIWebBrowser,
	CT_UICaption,
} ControlType;

class CUIControlBase
{
public:
	CUIControlBase(void);
	CUIControlBase(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	CUIControlBase(CUITreeContainer* pTree);
	virtual ~CUIControlBase(void);
private:
	std::string			m_strID;
	CUITreeContainer*	m_pTree;
	RECT				m_rc;
public:
	typedef std::map<std::string, CComVariant> ID2AttrMap;
	ID2AttrMap m_mapAttr;
	CUIEventCtrlContainer* m_pUIEventControl;
protected:
	void TranslateFatherPos(std::wstring& strPos, const RECT& fatherRc);
	void FireMouseEvent(std::string strName, int x, int y);
	void FirePosChange(int left, int top, int right, int bottom, int oldLeft, int oldTop, int oldRight, int oldBottom);
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
	virtual LRESULT OnSetCursor(int x, int y);
	virtual BOOL OnHitTest(int x, int y, LONG& nHitTest);
	virtual void OnPosChange(int left, int top, int right, int bottom, int oldLeft, int oldTop, int oldRight, int oldBottom);

	virtual void AdjustItemPos(BOOL bFire = TRUE);
	virtual BOOL GetVisible();
	virtual void SetVisible(BOOL bVisible);
	virtual const RECT& GetObjPos();
	virtual void SetObjPos(const RECT& rc);
public:
	const std::string& GetID();
	std::string SetID(const std::string& strID);
	void SetAttr(const std::string& strName, const std::string& strValue);
	void GetAttr(std::string strName, VARIANT* v);
	CUITreeContainer* GetOwnerTree();
	ULONG GetZorder();
	BOOL GetEnable();
	void Invalidate();
	void InvalidateRect(const RECT& rc);
	void SetCaptureMouse(BOOL bCapture);
public:
	static int GetID(lua_State* L);
	static int GetVisible(lua_State* L);
	static int SetVisible(lua_State* L);
	static int GetEnable(lua_State* L);
	static int GetOwnerTree(lua_State* L);
	static int SetObjPos(lua_State* L);

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