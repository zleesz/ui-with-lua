#pragma once
#include "UILuaManager.h"
#include <Util.h>

class CUIWindowBase;
typedef struct tagEventNode
{
	std::string strPath;
	std::string strFuncName;
	int nFuncIndex;
	int nArgs;
	int nRet;
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
	virtual void AttachListener(const LPXMLAttrMap pAttrMap, LPEventNode& pEventNode);
	virtual void DetachListener();
	virtual void DispatchListener(const std::string& strEventName);
	virtual BOOL ParserEvent(LPXMLDOMNode pNode) = 0;
	virtual BOOL OnInitEvent(const std::string& strPath);
	void* GetBindWnd();
private:
	CUIWindowBase* m_pBindWnd;
	LuaEventMap m_mapEvent;
public:
	LOG_CLS_DEC();
};
