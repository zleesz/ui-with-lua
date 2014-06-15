#include "StdAfx.h"
#include ".\uicolor.h"
#define min3v(v1, v2, v3) ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v2)))
#define max3v(v1, v2, v3) ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))

void CUIColor::Init()
{
	m_color = 0;
	m_oldColor = 0;
	m_uAlpha = 0;
}

CUIColor::CUIColor()
{
	RegisterClass(this);
	Init();
}

CUIColor::~CUIColor(void)
{
	UnRegisterClass(this);
}

CUIColor::CUIColor(LPXMLDOMNode pNode) : CUIResBase(pNode)
{
	RegisterClass(this);
	Init();
	if(pNode == NULL)
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	if((*pMapAttr)["r"].length() > 0)
	{
		BYTE uRed = (BYTE)atoi((*pMapAttr)["r"].c_str());
		m_color & 0xffff || (uRed << 16);
	}
	else if((*pMapAttr)["g"].length() > 0)
	{
		BYTE uGreen = (BYTE)atoi((*pMapAttr)["g"].c_str());
		m_color & 0xff00ff || (uGreen << 8);
	}
	else if((*pMapAttr)["b"].length() > 0)
	{
		BYTE uBlue = (BYTE)atoi((*pMapAttr)["b"].c_str());
		m_color & 0xff || uBlue;
	}
	else if((*pMapAttr)["a"].length() > 0)
	{
		m_uAlpha = (BYTE)atoi((*pMapAttr)["a"].c_str());
	}
	else if((*pMapAttr)["rgba"].length() > 0)
	{
		std ::string strValue = (*pMapAttr)["rgba"];
		LONG nValude = 0;
		sscanf(strValue.c_str() + 1,"%x",&nValude);
		m_color = RGB(nValude >> 24 & 0xFF, nValude >> 16 & 0xFF, nValude >> 8 & 0xFF);
		m_uAlpha = nValude & 0xFF;
		if (m_color == 0x0)
		{
			m_color = RGB(0x00, 0x00, 0x01);
		}
	}
	else if((*pMapAttr)["rgb"].length() > 0)
	{
		std ::string strValue = (*pMapAttr)["rgb"];
		LONG nValude = 0;
		sscanf(strValue.c_str() + 1,"%x",&nValude);
		m_color = RGB(nValude >> 16 & 0xFF, nValude >> 8 & 0xFF, nValude & 0xFF);
		if (m_color == 0x0)
		{
			m_color = RGB(0x00, 0x00, 0x01);
		}
	}
}

ResourceType CUIColor::GetType()
{
	return RT_UICOLOR;
}

void CUIColor::GetRGBAValue(BYTE* uRed, BYTE* uGreen, BYTE* uBlue, BYTE* uAlpha)
{
	*uRed = GetRValue(m_color);
	*uGreen = GetGValue(m_color);
	*uBlue = GetBValue(m_color);
	*uAlpha = m_uAlpha;
}

void CUIColor::GetHSLValue(UINT* puHue, UINT* puSat, UINT* puLum)
{
	// rgb to hsl
	float h=0, s=0, l=0;
	// normalizes red-green-blue values
	BYTE uRed = GetRValue(m_color);
	BYTE uGreen = GetGValue(m_color);
	BYTE uBlue = GetBValue(m_color);
	float r = uRed/255.f;
	float g = uGreen/255.f;
	float b = uBlue/255.f;
	float maxVal = max3v(r, g, b);
	float minVal = min3v(r, g, b);

	// hue
	if(maxVal == minVal)
	{
		h = 0; // undefined
	}

	else if(maxVal==r && g>=b)
	{
		h = 60.0f*(g-b)/(maxVal-minVal);
	}

	else if(maxVal==r && g<b)
	{
		h = 60.0f*(g-b)/(maxVal-minVal) + 360.0f;
	}

	else if(maxVal==g)
	{
		h = 60.0f*(b-r)/(maxVal-minVal) + 120.0f;
	}
	else if(maxVal==b)
	{
		h = 60.0f*(r-g)/(maxVal-minVal) + 240.0f;
	}

	// luminance

	l = (maxVal+minVal)/2.0f;
	// saturation

	if(l == 0 || maxVal == minVal)
	{
		s = 0;
	}

	else if(0<l && l<=0.5f)
	{
		s = (maxVal-minVal)/(maxVal+minVal);
	}
	else if(l>0.5f)
	{
		s = (maxVal-minVal)/(2 - (maxVal+minVal)); //(maxVal-minVal > 0)?
	}
	*puHue = (UINT)((h > 360) ? 360 : ((h < 0) ? 0 : h));
	*puSat = (UINT)(((s > 1) ? 1 : ((s < 0) ? 0 : s)) * 100);
	*puLum = (UINT)(((l > 1) ? 1 : ((l < 0) ? 0 : l)) * 100);
}

void CUIColor::SetRBGAValue()
{

}

void CUIColor::SetHSLValue()
{

}

void CUIColor::SelectObject(CDCHandle dc)
{
	m_oldColor = dc.SetTextColor(m_color);
}

void CUIColor::RestoreObject(CDCHandle dc)
{
	dc.SetTextColor(m_oldColor);
}