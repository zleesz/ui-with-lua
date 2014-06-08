#pragma once
#include <atlapp.h>
#include <atlwin.h>

class CUIWindowCaption
{
public:
	typedef std::vector<RECT> VecRECT;
public:
	void ClearCaptionArea();
	void AddCaptionRect(const RECT& rc);
	void SetCaptionRect(const RECT& rc);
public:
	BOOL OnNcHitTest(const POINT& pt, LONG& lnHitTest);
private:
	VecRECT				m_vecRECT;
};
