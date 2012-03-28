#include "StdAfx.h"
#include "ImageTool.h"

void CImageTool::GetVLeftPos(CxImage* pImage, int& left1, int& left2)
{
	ATLASSERT(NULL != pImage);
	if(NULL == pImage)
		return;
	left1 = -1;
	left2 = -1;
	BYTE*		lpSrc;
	uint8_t*	lpData = pImage->GetBits();
	uint8_t		uWidth = pImage->GetWidth();
	uint8_t		uHeight = pImage->GetHeight();
	uint8_t		uWidthBytes = WIDTHBYTES(uWidth * 8);

	for(int j = 0; j < uWidth; j++)
	{
		for (int i = 0; i < uHeight; i++)
		{
			lpSrc = (BYTE*)lpData + uWidthBytes * i * 3 + j * 3;
			if(lpSrc[0] != 0xff || lpSrc[1] != 0 || lpSrc[2] != 0xff)
			{
				break;
			}
			if(i == uHeight - 1)
			{
				if(left1 < 0)
					left1 = j;
				else if(left2 < 0)
					left2 = j;
			}
		}
		if(left2 >= 0)
			break;
	}
}

void CImageTool::GetHTopPos(CxImage* pImage, int& top1, int& top2)
{
	ATLASSERT(NULL != pImage);
	if(NULL == pImage)
		return;
	top1 = -1;
	top2 = -1;
	BYTE*		lpSrc;
	uint8_t*	lpData = pImage->GetBits();
	uint8_t		uWidth = pImage->GetWidth();
	uint8_t		uHeight = pImage->GetHeight();
	uint8_t uWidthBytes = WIDTHBYTES(uWidth * 8);

	for (int i = 0; i < uHeight; i++)
	{
		for(int j = 0; j < uWidth; j++)
		{
			lpSrc = (BYTE*)lpData + uWidthBytes * i * 3 + j * 3;
			if(lpSrc[0] != 0xff || lpSrc[1] != 0 || lpSrc[2] != 0xff)
			{
				break;
			}
			if(j == uWidth - 1)
			{
				if(top1 < 0)
					top1 = i;
				else if(top2 < 0)
					top2 = i;
			}
		}
		if(top2 >= 0)
			break;
	}
	int toptmp = uHeight - top2 - 1;
	top2 = uHeight - top1 - 1;
	top1 = toptmp;
}