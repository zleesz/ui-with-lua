#include "StdAfx.h"
#include "UIWindowCaption.h"

void CUIWindowCaption::ClearCaptionArea()
{
	m_vecRECT.clear();
}

void CUIWindowCaption::AddCaptionRect(const RECT& rc)
{
	m_vecRECT.push_back(rc);
}

void CUIWindowCaption::SetCaptionRect(const RECT& rc)
{
	ClearCaptionArea();
	AddCaptionRect(rc);
}

BOOL CUIWindowCaption::OnNcHitTest(const POINT& pt, LONG& lnHitTest)
{
	lnHitTest = HTNOWHERE;
	VecRECT::size_type nSize = m_vecRECT.size();
	if (nSize <= 0)
	{
		return 0;
	}
	for (VecRECT::size_type i = 0; i < nSize; i++)
	{
		if (::PtInRect(&m_vecRECT[i], pt))
		{
			lnHitTest = HTCAPTION;
			return TRUE;
		}
	}
	return FALSE;
}
