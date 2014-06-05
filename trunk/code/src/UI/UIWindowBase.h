#pragma once
#include <atlapp.h>
#include <atlgdi.h>
#include <string>
#include <map>
#include <atlapp.h>
#include <atlwin.h>
#include <winuser.h>
#include <atltypes.h>

#include <Util.h>
#include "UITreeContainer.h"
#include "UIEventWndContainer.h"

typedef enum enumWindowType {
	WT_FrameHostWnd = 1,
	WT_ModalHostWnd,
	WT_MenuHostWnd,
	WT_TipHostWnd,
} WindowType;

class CUIWindowBase :
	public CWindowImpl<CUIWindowBase>
{
public:
	CUIWindowBase(void);
	CUIWindowBase(const std::string& strPath, LPXMLDOMNode pNode);
	virtual ~CUIWindowBase(void);
private:
	std::string m_strID;
	std::string m_strXMLPath;
private:
	virtual BOOL ParserAttr(LPXMLDOMNode pAttrNode);
	virtual BOOL ParserUITree(LPXMLDOMNode pNode);
	virtual BOOL ParserEvent(LPXMLDOMNode pNode);
private:
	BOOL IsInResizeLeftTopArea(const POINT& pt, const SIZE& sz);
	BOOL IsInResizeLeftBottomArea(const POINT& pt, const SIZE& sz);
	BOOL IsInResizeRightTopArea(const POINT& pt, const SIZE& sz);
	BOOL IsInResizeRightBottomArea(const POINT& pt, const SIZE& sz);
	BOOL IsInResizeLeftArea(const POINT& pt, const SIZE& sz);
	BOOL IsInResizeTopArea(const POINT& pt, const SIZE& sz);
	BOOL IsInResizeRightArea(const POINT& pt, const SIZE& sz);
	BOOL IsInResizeBottomArea(const POINT& pt, const SIZE& sz);
public:
	typedef std::map<std::string, CComVariant> ID2AttrMap;
	ID2AttrMap m_mapAttr;
	virtual WindowType GetType() = 0;
	virtual BOOL Render(CDCHandle dc) = 0;
	virtual BOOL CreateWnd(HWND hParent) = 0;
	void SetAttr(const std::string& strName, const std::string& strValue);
	void GetAttr(std::string strName, VARIANT* v);
	const std::string& GetID() { return m_strID; };
	void SetID(std::string strID) { m_strID = strID; };
	const std::string& GetXMLPath() { return m_strXMLPath; };

	BOOL GetLayered();
	BOOL SetLayered(BOOL bLayered);
	BOOL GetResizable();
	BOOL SetResizable(BOOL bResizable);
public:
	LRESULT OnGetMinMaxInfo(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNcHitTest(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNcLButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	static int GetID(lua_State* luaState);
	static int AddInputFilter(lua_State* L);
	static int AttachListener(lua_State* L);
	static int DetachListener(lua_State* L);
	static int Min(lua_State* luaState);
	static int Max(lua_State* luaState);
	static int Restore(lua_State* luaState);
	static int GetLayered(lua_State* luaState);
	static int SetMaxTrackSize(lua_State* luaState);
	static int SetMinTrackSize(lua_State* luaState);
	static int SetResizable(lua_State* luaState);
public:
	CUITreeContainer* m_pUITreeContainer;
	CUIEventWndContainer* m_pUIEventWindow;
protected:
	virtual DWORD GetStyle();
	virtual DWORD GetStyleEx();
};
