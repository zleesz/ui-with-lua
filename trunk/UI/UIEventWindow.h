#pragma once
#include "UIEventBase.h"
#include <atlapp.h>
#include <atlapp.h>
#include <atlwin.h>
#include <winuser.h>
#include <atltypes.h>
#include <uilog4cplus.h>

class CUIEventWindow :
	public CUIEventBase
{
public:
	CUIEventWindow(void);
	CUIEventWindow(void* p) : CUIEventBase(p){};
	virtual ~CUIEventWindow(void);
public:
	BEGIN_MSG_MAP(CUIEventWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
	END_MSG_MAP()
private:
	void ParserOneEvent(LPXMLDOMNode pNode);
public:
	virtual BOOL ParserEvent(LPXMLDOMNode pNode);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	LOG_CLS_DEC();
};
