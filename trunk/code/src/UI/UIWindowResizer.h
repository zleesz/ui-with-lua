#pragma once

class CUIWindowBase;

class CUIWindowResizer
{
private:
	CUIWindowResizer(void) {};
public:
	CUIWindowResizer(CUIWindowBase* p) : m_pBindWnd(p) {};
	virtual ~CUIWindowResizer(void);
private:
	CUIWindowBase* m_pBindWnd;
};
