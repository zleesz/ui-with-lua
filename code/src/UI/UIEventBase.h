#pragma once
#include "UILuaManager.h"
#include <Util.h>

class CUIWindowBase;
typedef struct tagUIDISPPARAMS
{
	VARIANTARG *rgvarg;
	std::string strName;
	UINT nArgs;
	UINT nRet;
}UIDISPPARAMS;

typedef enum enumEventType {
	ET_Window = 1,
	ET_Control,
} EventType;

typedef struct tagEventNode
{
	std::string strPath;
	std::string strFuncName;
	int nFuncIndex;
}EventNode, *LPEventNode;
typedef std::vector<LPEventNode> VecEvent, *LPVecEvent; // event veclist
typedef std::map<std::string, LPVecEvent> LuaEventMap; // event name 2 veclist 

class CUIEventBase
{
public:
	CUIEventBase();
	CUIEventBase(CUIWindowBase* p) : m_pBindWnd(p) {};
	virtual ~CUIEventBase();
public:
	virtual void AttachListener(const LPXMLAttrMap pAttrMap);
	virtual void DetachListener();
	virtual int AttachListener(lua_State* L);
	virtual int DetachListener(lua_State* L);
	virtual void DispatchListener(UIDISPPARAMS& params);
	virtual BOOL ParserEvent(LPXMLDOMNode pNode) = 0;
	virtual BOOL OnBindEvent(const std::string& strPath);
	virtual EventType GetType() = 0;
	virtual void PushEventParams(UIDISPPARAMS& params);
	void* GetBindWnd();
public:
	CUIWindowBase* m_pBindWnd;
private:
	LuaEventMap m_mapEvent;
public:
	LOG_CLS_DEC();
};
