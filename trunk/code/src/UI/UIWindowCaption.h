#pragma once

class CUIWindowBase;

class CUIWindowCaption
{
private:
	CUIWindowCaption(void) {};
public:
	CUIWindowCaption(CUIWindowBase* p) : m_pBindWnd(p) {};
	virtual ~CUIWindowCaption(void);
private:
	CUIWindowBase* m_pBindWnd;
};
