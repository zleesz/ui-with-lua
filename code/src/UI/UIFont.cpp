#include "StdAfx.h"
#include ".\uifont.h"
#include "Util.h"
#include <math.h>

CUIFont::CUIFont(void)
{
	RegisterClass(this);
	Init();
}

CUIFont::~CUIFont(void)
{
	UnRegisterClass(this);
}

CUIFont::CUIFont(LPXMLDOMNode pNode) : CUIResBase(pNode)
{
	RegisterClass(this);
	Init();
	if(pNode == NULL)
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	if ((*pMapAttr)["name"].length() > 0)
	{
		Util::UTF8_to_Unicode((*pMapAttr)["name"].c_str(), m_strName);
	}
	if ((*pMapAttr)["bold"].length() > 0)
	{
		m_bBold = (BOOL)atoi((*pMapAttr)["bold"].c_str());
	}
	if ((*pMapAttr)["underline"].length() > 0)
	{
		m_bUnderline = (BOOL)atoi((*pMapAttr)["underline"].c_str());
	}
	if ((*pMapAttr)["height"].length() > 0)
	{
		m_lnHeight = atoi((*pMapAttr)["height"].c_str());
	}
}

void CUIFont::Init()
{
	m_font = NULL;
	m_oldFont = NULL;
	m_lnHeight = 0;
	m_bBold = FALSE;
	m_bUnderline = FALSE;
}

ResourceType CUIFont::GetType()
{
	return RT_UIFONT;
}

void CUIFont::SelectObject(CDCHandle dc)
{
	HFONT hSysFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	if (!hSysFont)
	{
		return;
	}
	LOGFONT lf;
	GetObject(hSysFont, sizeof(LOGFONT), &lf);	
	lf.lfUnderline = (BYTE)m_bUnderline;
	lf.lfWeight = m_bBold ? 700 : 400;
	lf.lfHeight = -(int)(fabs((double)m_lnHeight * 10 * GetDeviceCaps(dc.m_hDC, LOGPIXELSY)/72)/10.0+0.5);
	wcsncpy(lf.lfFaceName, m_strName.c_str(), LF_FACESIZE);
	m_font = ::CreateFontIndirect(&lf);

	m_oldFont = dc.SelectFont(m_font);
	::DeleteObject(hSysFont);
}

void CUIFont::RestoreObject(CDCHandle dc)
{
	if (m_oldFont)
	{
		dc.SelectFont(m_oldFont);
		m_oldFont = NULL;
	}
	if (m_font)
	{
		::DeleteObject(m_font);
		m_font = NULL;
	}
}