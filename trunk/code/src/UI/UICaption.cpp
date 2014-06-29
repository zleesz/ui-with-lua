#include "StdAfx.h"
#include "UICaption.h"

CUICaption::CUICaption(void)
{
	RegisterClass(this);
}

CUICaption::~CUICaption(void)
{
	UnRegisterClass(this);
}

CUICaption::CUICaption(CUITreeContainer* pTree, LPXMLDOMNode pNode)
: CUIControlBase(pTree, pNode)
{
	RegisterClass(this);
}

CUICaption::CUICaption(CUITreeContainer* pTree)
: CUIControlBase(pTree)
{
	RegisterClass(this);
}

void CUICaption::Render(CDCHandle dc)
{

}

void CUICaption::OnInitControl()
{

}

void CUICaption::OnDetroy()
{

}

BOOL CUICaption::OnHitTest(int x, int y, LONG& nHitTest)
{
	const RECT rc = GetObjPos();
	if(x > rc.left && x < rc.right
		&& y > rc.top && y < rc.bottom)
	{
		nHitTest = HTCAPTION;
		return TRUE;
	}
	return FALSE;
}

void CUICaption::SetAttr(const std::string& strName, const std::string& strValue)
{
	if (_SetAttr(strName, strValue))
	{
		return;
	}
}