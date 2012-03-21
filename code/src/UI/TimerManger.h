#pragma once
#include "UILuaManager.h"
#include <uilog4cplus.h>
#include <atlapp.h>
#include <atlwin.h>
#include <winuser.h>
#include <atltypes.h>

class CUITimerManger
	: public CWindowImpl<CUITimerManger>
{
public:
	CUITimerManger(void);
	~CUITimerManger(void);
public:
	static int _SetTimer(lua_State* L);
	static int _SetOnceTimer(lua_State* L);
	static int _KillTimer(lua_State* L);
private:
	typedef struct tagTimerData
	{
		ULONG lnId;
		ULONG lnElapse;
		int nFuncIndex;
		BOOL bOnce;
		tagTimerData() : lnId(0), lnElapse(0), nFuncIndex(0), bOnce(FALSE) {}
	}TimerData, *LPTimerData;
	typedef std::map<ULONG, TimerData> ID2TimerDataMap;

	ID2TimerDataMap m_mapID2TimerData;
	ULONG m_lnTimerID;
private:
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	BEGIN_MSG_MAP(CUITimerManger)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
	END_MSG_MAP()
public:
	BEGIN_LUA_CALL_MAP(CUITimerManger)
		LUA_CALL_ENTRY2(SetTimer, _SetTimer)
		LUA_CALL_ENTRY2(SetOnceTimer, _SetOnceTimer)
		LUA_CALL_ENTRY2(KillTimer, _KillTimer)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_SINGLETON_OBJECT(CUITimerManger, UI.TimerManager)
public:
	LOG_CLS_DEC();
};
