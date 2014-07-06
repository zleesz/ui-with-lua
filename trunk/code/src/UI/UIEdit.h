#pragma once
#include "uicontrolbase.h"
#include "UIWindowBase.h"
#include "UITreeContainer.h"
#include "UIResFactory.h"
#include "UIFont.h"
#include "UIColor.h"

typedef enum enumEditVAlign
{
	EVA_TOP,
	EVA_CENTER,
	EVA_BOTTOM
} EditVAlign;

typedef enum enumEditHAlign
{
	EHA_LEFT,
	EHA_CENTER,
	EHA_RIGHT
} EditHAlign;

class CUIEdit :
	public CUIControlBase
{
public:
	CUIEdit(void);
public:
	CUIEdit(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	CUIEdit(CUITreeContainer* pTree);
	virtual ~CUIEdit(void);
public:
	UICONTROL_GETTYPE_METHOD(CT_UIEdit);
	virtual void Render(CDCHandle dc);
	virtual void OnInitControl();
	virtual void OnDetroy();
	virtual void SetVisible(BOOL bVisible);
	virtual void OnSetFocus(BOOL bFocus);
	virtual void OnLButtonDown(int x, int y);
	virtual void OnLButtonUp(int x, int y);
	virtual void OnLButtonDbClick(int x, int y);
	virtual void OnMouseMove(int x, int y);
	virtual void OnKeyDown(int code, BOOL bAlt, BOOL bCtrl, BOOL bShift, int repeat);
	virtual void OnChar(const wchar_t& c, int repeat);
	virtual LRESULT OnSetCursor(int x, int y);
	virtual void SetAttr(const std::string& strName, const std::string& strValue);
private:
	void CaclTextRect(RECT& rc);
	LONG GetFormat();
	void UpdateCaretPos();
	size_t GetCaretPosFromPoint(int& x, int& y);

	void DealCodeKeyDown(int code);
	void DealCtrlKeyDown(int code);
	void DealShiftKeyDown(int code);
	void DealCtrlShiftKeyDown(int code);

	void GetWordPosByPos(size_t lnPos, size_t& lnStartPos, size_t& lnEndPos);
	void GetLeftWordPosByPos(size_t lnPos, size_t& lnStartPos);
	void GetRightWordPosByPos(size_t lnPos, size_t& lnEndPos);
private:
	std::wstring	m_strText;
	std::string		m_strFontID;
	std::string		m_strColorID;
	std::string		m_strNormalBkgID;
	std::string		m_strHoverBkgID;
	std::string		m_strFocusBkgID;
	std::string		m_strDisableBkgID;
	BOOL			m_bPassword;
	BOOL			m_bMultiLine;
	BOOL			m_bWordBreak;
	BOOL			m_bWordEllipsis;
	EditVAlign		m_evalign;
	EditHAlign		m_ehalign;
	BOOL			m_bInsert;
	size_t			m_nInsertPos;
	size_t			m_nSelStartPos;
	size_t			m_nSelEndPos;
	size_t			m_nLButtonDownPos;
	size_t			m_nMouseMoveLastPos;

public:
	std::string SetText(const std::string& strText);
	void GetText(std::wstring& strText);
	std::string SetTextColor(const std::string& strTextColor);
	void GetTextColor(std::string& strTextColor);
	void GetTextExtent(const wchar_t* psz, LONG lnSize, LONG& lnWidth, LONG& lnHeight);
	void GetTextExtent(const wchar_t* psz, LONG& lnWidth, LONG& lnHeight);
	void GetTextExtent(LONG& lnWidth, LONG& lnHeight);
	DWORD GetSel();
	void GetSel(int& nStartChar, int& nEndChar);

	static int GetText(lua_State* L);
	static int SetText(lua_State* L);
	static int GetTextColor(lua_State* L);
	static int SetTextColor(lua_State* L);
	static int GetTextExtent(lua_State* L);
public:
	BEGIN_LUA_CALL_MAP(CUIEdit)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY(GetVisible)
		LUA_CALL_ENTRY2(SetVisible, CUIControlBase::SetVisible)
		LUA_CALL_ENTRY(GetEnable)
		LUA_CALL_ENTRY(GetOwnerTree)
		LUA_CALL_ENTRY(GetText)
		LUA_CALL_ENTRY(SetText)
		LUA_CALL_ENTRY(GetTextColor)
		LUA_CALL_ENTRY(SetTextColor)
		LUA_CALL_ENTRY(SetObjPos)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIEdit, UI.Edit);
public:
	LOG_CLS_DEC();
};
