#include "StdAfx.h"
#include "UIEventWindow.h"

CUIEventWindow::CUIEventWindow(void)
{
}

CUIEventWindow::~CUIEventWindow(void)
{
}

void CUIEventWindow::ParserOneEvent(LPXMLDOMNode pNode)
{
	if(pNode == NULL || pNode->strName != "event")
		return;
	LPXMLAttrMap pAttrMap = pNode->pMapAttr;
	if(pAttrMap == NULL)
		return;
	LPEventNode pEventNode = NULL;
	AttachListener(pAttrMap, pEventNode);
	ATLASSERT(pEventNode);
	if(NULL != pEventNode)
	{
		//pEventNode->
	}
}

BOOL CUIEventWindow::ParserEvent(LPXMLDOMNode pNode)
{
	if (NULL == pNode || pNode->strName != "eventlist")
		return FALSE;
	LPXMLChildNodes pChildNode = pNode->pMapChildNode;
	if(pChildNode == NULL)
		return FALSE;
	XMLChildNodes::const_iterator it = pChildNode->find("event");
	if(it == pChildNode->end())
	{
		return FALSE;
	}
	LPXMLVecNodes pVecNodes = it->second->pVecNode;
	for(XMLVecNodes::size_type st = 0; st != pVecNodes->size(); st++)
	{
		ParserOneEvent((*pVecNodes)[st]);
	}
	return TRUE;
}

LRESULT CUIEventWindow::OnCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	LOG_AUTO();
	DispatchListener("OnCreate");
	return 0;
}

LRESULT CUIEventWindow::OnClose(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	LOG_AUTO();
	DispatchListener("OnClose");
	return 0;
}