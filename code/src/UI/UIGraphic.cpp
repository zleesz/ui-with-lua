#include "StdAfx.h"
#include "UIGraphic.h"
#include <atltypes.h>

CUIGraphic::CUIGraphic(void)
{
	m_hGraphicDll = NULL;
	m_fnInitGraphicLib = NULL;
	m_fnUnInitGraphicLib = NULL;
	m_fnLoadBitmapFromFile = NULL;
	m_fnClipSubBindBitmap = NULL;
	m_fnAddRefBitmap = NULL;
	m_fnReleaseBitmap = NULL;
	m_fnGetBitmapInfo = NULL;
	m_fnBitmapAlphaBlend = NULL;
	m_fnPaintBitmap = NULL;
	m_fnAlphaPaintBitmap = NULL;
	m_fnCreateBitmapList = NULL;
	m_fnAddRefBitmapList = NULL;
	m_fnReleaseBitmapList = NULL;
	m_fnGetBitmapListCount = NULL;
	m_fnGetBitmapFromList = NULL;
	m_fnCreateHBitmapFromHandle = NULL;
	m_fnCreateFillTexture = NULL;
	m_fnCreateTileTexture = NULL;
	m_fnCreateThreeInOneHTexture = NULL;
	m_fnCreateThreeInOneVTexture = NULL;
	m_fnCreateFiveOneHTexture = NULL;
	m_fnCreateFiveOneVTexture = NULL;
	m_fnUpdateTexture = NULL;
	m_fnGetCellBitmapFromTexture = NULL;
	m_fnAddRefTexture = NULL;
	m_fnReleaseRefTexture = NULL;
	m_fnSetLayeredWindowAttribute = NULL;
	m_fnUpdateLayeredWindow = NULL;
	m_ulGdiToken = 0;
}

CUIGraphic::~CUIGraphic(void)
{
}

void CUIGraphic::InitGdiplus()
{
	if (m_ulGdiToken > 0)
	{
		return;
	}
	Gdiplus::GdiplusStartupInput GdiInput;
	GdiplusStartup(&m_ulGdiToken ,&GdiInput ,NULL);
}

BOOL CUIGraphic::InitGraphic()
{
	InitGdiplus();
	if (m_hGraphicDll)
	{
		return FALSE;
	}

	TCHAR szDllPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL, szDllPath, MAX_PATH);
	::PathCombine(szDllPath, szDllPath, _T("..\\UIGraphic.dll"));
	m_hGraphicDll = ::LoadLibrary(szDllPath);
	if (m_hGraphicDll == NULL)
	{
		return FALSE;
	}

	m_fnInitGraphicLib = (UIGraphic_InitGraphic_Func)::GetProcAddress(m_hGraphicDll, "_InitGraphicLib@4");
	m_fnUnInitGraphicLib = (UIGraphic_UnInitGraphic_Func)::GetProcAddress(m_hGraphicDll, "_UnInitGraphicLib@0");
	m_fnLoadBitmapFromFile = (UIGraphic_LoadBitmapFromFile_Func)::GetProcAddress(m_hGraphicDll, "_LoadBitmapFromFile@8");
	m_fnClipSubBindBitmap = (UIGraphic_ClipSubBindBitmap_Func)::GetProcAddress(m_hGraphicDll, "_ClipSubBindBitmap@8");
	m_fnAddRefBitmap = (UIGraphic_AddRefBitmap_Func)::GetProcAddress(m_hGraphicDll, "_AddRefBitmap@4");
	m_fnReleaseBitmap = (UIGraphic_ReleaseBitmap_Func)::GetProcAddress(m_hGraphicDll, "_ReleaseBitmap@4");
	m_fnGetBitmapInfo = (UIGraphic_GetBitmapInfo_Func)::GetProcAddress(m_hGraphicDll, "_GetBitmapInfo@8");
	m_fnBitmapAlphaBlend = (UIGraphic_BitmapAlphaBlend_Func)::GetProcAddress(m_hGraphicDll, "_BitmapAlphaBlend@16");
	m_fnPaintBitmap = (UIGraphic_PaintBitmap_Func)::GetProcAddress(m_hGraphicDll, "_PaintBitmap@16");
	m_fnAlphaPaintBitmap = (UIGraphic_AlphaPaintBitmap_Func)::GetProcAddress(m_hGraphicDll, "_AlphaPaintBitmap@44");
	m_fnCreateBitmapList = (UIGraphic_CreateBitmapList_Func)::GetProcAddress(m_hGraphicDll, "_CreateBitmapList@4");
	m_fnAddRefBitmapList = (UIGraphic_AddRefBitmapList_Func)::GetProcAddress(m_hGraphicDll, "_AddRefBitmapList@4");
	m_fnReleaseBitmapList = (UIGraphic_ReleaseBitmapList_Func)::GetProcAddress(m_hGraphicDll, "_ReleaseBitmapList@4");
	m_fnGetBitmapListCount = (UIGraphic_GetBitmapListCount_Func)::GetProcAddress(m_hGraphicDll, "_GetBitmapListCount@4");
	m_fnGetBitmapBuffer  = (UIGraphic_GetBitmapBuffer_Func)::GetProcAddress(m_hGraphicDll, "_GetBitmapBuffer@12");
	m_fnGetBitmapFromList = (UIGraphic_GetBitmapFromList_Func)::GetProcAddress(m_hGraphicDll, "_GetBitmapFromList@8");
	m_fnCreateHBitmapFromHandle = (UIGraphic_CreateHBitmapFromHandle_Func)::GetProcAddress(m_hGraphicDll, "_CreateHBitmapFromHandle@4");
	m_fnCreateFillTexture = (UIGraphic_CreateFillTexture_Func)GetProcAddress(m_hGraphicDll,"_CreateFillTexture@4");
	m_fnCreateTileTexture = (UIGraphic_CreateTileTexture_Func)GetProcAddress(m_hGraphicDll,"_CreateTileTexture@4");
	m_fnCreateThreeInOneHTexture = (UIGraphic_CreateThreeInOneHTexture_Func)::GetProcAddress(m_hGraphicDll, "_CreateThreeInOneHTexture@8");
	m_fnCreateThreeInOneVTexture = (UIGraphic_CreateThreeInOneVTexture_Func)::GetProcAddress(m_hGraphicDll, "_CreateThreeInOneVTexture@8");
	m_fnCreateFiveOneHTexture = (UIGraphic_CreateFiveInOneHTexture_Func)::GetProcAddress(m_hGraphicDll, "_CreateFiveInOneHTexture@4");
	m_fnCreateFiveOneVTexture = (UIGraphic_CreateFiveInOneVTexture_Func)::GetProcAddress(m_hGraphicDll, "_CreateFiveInOneVTexture@4");
	m_fnCreateNineOneTexture = (UIGraphic_CreateNineInOneTexture_Func)::GetProcAddress(m_hGraphicDll, "_CreateNineInOneTexture@4");
	m_fnUpdateTexture = (UIGraphic_UpdateTexture_Func)::GetProcAddress(m_hGraphicDll, "_UpdateTexture@12");
	m_fnGetCellBitmapFromTexture = (UIGraphic_GetCellBitmapFromTexture_Func)::GetProcAddress(m_hGraphicDll, "_GetCellBitmapFromTexture@8");
	m_fnAddRefTexture = (UIGraphic_AddRefTexture_Func)::GetProcAddress(m_hGraphicDll, "_AddRefTexture@4");
	m_fnReleaseRefTexture = (UIGraphic_ReleaseRefTexture_Func)::GetProcAddress(m_hGraphicDll, "_ReleaseTexture@4");      

	//load USER32.dll
	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	if(hUser32)
	{
		m_fnSetLayeredWindowAttribute = (UIGraphic_lpfnSetLayeredWindowAttribute)::GetProcAddress(hUser32, "SetLayeredWindowAttributes");
		m_fnUpdateLayeredWindow = (UIGraphic_lpfnUpdateLayeredWindow)GetProcAddress(hUser32, "UpdateLayeredWindow");
	}

	if (m_fnInitGraphicLib == NULL || m_fnUnInitGraphicLib == NULL 
		|| m_fnLoadBitmapFromFile == NULL || m_fnClipSubBindBitmap == NULL
		|| m_fnAddRefBitmap == NULL || m_fnReleaseBitmap == NULL || m_fnGetBitmapInfo == NULL 
		|| m_fnBitmapAlphaBlend == NULL || m_fnPaintBitmap == NULL || m_fnAlphaPaintBitmap == NULL
		|| m_fnCreateBitmapList == NULL || m_fnAddRefBitmapList == NULL || m_fnReleaseBitmapList == NULL 
		|| m_fnGetBitmapListCount == NULL || m_fnGetBitmapFromList == NULL || m_fnGetBitmapBuffer == NULL|| m_fnCreateHBitmapFromHandle == NULL
		|| m_fnCreateTileTexture == NULL || m_fnCreateFillTexture == NULL
		|| m_fnCreateThreeInOneHTexture == NULL || m_fnCreateThreeInOneVTexture == NULL
		|| m_fnCreateFiveOneHTexture == NULL || m_fnCreateFiveOneVTexture == NULL
		|| m_fnCreateNineOneTexture == NULL
		|| m_fnUpdateTexture == NULL || m_fnGetCellBitmapFromTexture == NULL 
		|| m_fnAddRefTexture == NULL ||m_fnReleaseRefTexture == NULL
		|| m_fnSetLayeredWindowAttribute == NULL)
	{
		return FALSE;
	}

	m_fnInitGraphicLib(NULL);
	return TRUE;

}

void CUIGraphic::UnInitGraphic()
{
	if(m_hGraphicDll)
	{
		m_fnUnInitGraphicLib();
		FreeLibrary(m_hGraphicDll);
		m_hGraphicDll = NULL;
	}
	if (m_ulGdiToken > 0)
	{
		Gdiplus::GdiplusShutdown(m_ulGdiToken);
		m_ulGdiToken = 0;
	}
}

BITMAP_HANDLE CUIGraphic::LoadBitmapFromFile(const wchar_t* file, unsigned long type /* = 0 */)
{
	return m_fnLoadBitmapFromFile(file, type);
}

BITMAP_HANDLE CUIGraphic::ClipSubBindBitmap(BITMAP_HANDLE xlBitmap, RECT* pRect)
{
	return m_fnClipSubBindBitmap(xlBitmap, pRect);
}

BITMAP_HANDLE CUIGraphic::ClipSubBindBitmap(BITMAP_HANDLE xlBitmapSrc, int nBlockNum, int nSelectIndex)
{
	XLBitmapInfo bmpInfo;
	GetBitmapInfo(xlBitmapSrc, &bmpInfo);

	int nBlockSize = bmpInfo.Width / nBlockNum;

	RECT rcClip = {nSelectIndex*nBlockSize, 0, (nSelectIndex+1)*nBlockSize, bmpInfo.Height};
	return ClipSubBindBitmap(xlBitmapSrc, &rcClip);
}

unsigned long CUIGraphic::AddRefBitmap(BITMAP_HANDLE xlBitmap)
{
	return m_fnAddRefBitmap(xlBitmap);
}

unsigned long CUIGraphic::ReleaseBitmap(BITMAP_HANDLE xlBitmap)
{
	return m_fnReleaseBitmap(xlBitmap);
}

BOOL CUIGraphic::GetBitmapSize(BITMAP_HANDLE hndBitmap, int* nWidth, int* nHeight)
{
	if (hndBitmap == NULL)
	{
		return FALSE;
	}

	XLBitmapInfo BmInfo;
	m_fnGetBitmapInfo(hndBitmap, &BmInfo);

	if (nWidth != NULL)
	{
		*nWidth = BmInfo.Width;
	}
	if (nHeight != NULL)
	{
		*nHeight = BmInfo.Height;
	}
	return TRUE;
}

BYTE* CUIGraphic::GetBitmapBuffer(BITMAP_HANDLE hBitmap,unsigned long x,unsigned long y)
{
	return m_fnGetBitmapBuffer(hBitmap,x,y);
}

BOOL CUIGraphic::AlphaPaintBitmap(HDC hdc, BITMAP_HANDLE hBm,  LPRECT lprcDest, LPRECT lprcSrc)
{	
	if (hBm == NULL)
	{
		return FALSE;
	}

	CRect rc;
	if (lprcSrc == NULL)
	{
		int nWidth = 0;
		int nHeight = 0;
		GetBitmapSize(hBm, &nWidth, &nHeight);
		rc.SetRect(0, 0, nWidth, nHeight);
		lprcSrc = &rc;
	}

	BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	int nDestWidht = lprcDest->right - lprcDest->left;
	int nDestHeight = lprcDest->bottom - lprcDest->top;
	int nSrcWidth = lprcSrc->right - lprcSrc->left;
	int nSrcHeight = lprcSrc->bottom - lprcSrc->top;
	return m_fnAlphaPaintBitmap(hdc, lprcDest->left, lprcDest->top, nDestWidht, nDestHeight, hBm, lprcSrc->left, lprcSrc->top, nSrcWidth, nSrcHeight, blendPixelFunction) == 0?TRUE:FALSE;
}

BOOL CUIGraphic::AlphaPaintBitmap(HDC hDC, BITMAP_HANDLE hBm, int x, int y, long cx, long cy)
{
	if (hBm == NULL)
	{
		return FALSE;
	}

	int nWidth = 0;
	int nHeight = 0;
	GetBitmapSize(hBm, &nWidth, &nHeight);

	BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	return m_fnAlphaPaintBitmap(hDC, x, y, cx, cy, hBm, 0, 0, nWidth, nHeight, blendPixelFunction) == 0?TRUE:FALSE;
}

int CUIGraphic::GetBitmapInfo(BITMAP_HANDLE xlBitmap, XLBitmapInfo* pInfo)
{
	return m_fnGetBitmapInfo(xlBitmap, pInfo);
}

int CUIGraphic::AlphaBlend(BITMAP_HANDLE xlBitmapSrc, BITMAP_HANDLE xlBitmapDest, unsigned char alphaValue, unsigned long alphaType)
{
	return m_fnBitmapAlphaBlend(xlBitmapSrc, xlBitmapDest, alphaValue, alphaType);
}

int CUIGraphic::PaintBitmap(HDC hdc, RECT& rcDest, BITMAP_HANDLE xlBitmapSrc, RECT& rcSrc)
{
	return m_fnPaintBitmap(hdc, xlBitmapSrc, &rcDest, &rcSrc);
}

BOOL CUIGraphic::PaintBitmap(HDC hDC, BITMAP_HANDLE hBm, int x, int y, long cx, long cy)
{
	if (hBm == NULL)
	{
		_ASSERT(FALSE);
		return FALSE;
	}

	CRect rcDest(x, y, x + cx, y + cy);
	return PaintBitmap(hDC, hBm, rcDest, NULL) == 0?TRUE:FALSE;
}

BOOL CUIGraphic::PaintBitmap(HDC hDC, BITMAP_HANDLE hBm, LPRECT lprcDest, LPRECT lprcSrc)
{
	if (hBm == NULL)
	{
		_ASSERT(FALSE);
		return FALSE;
	}

	CRect rc;
	if (lprcSrc == NULL)
	{
		XLBitmapInfo info;
		m_fnGetBitmapInfo(hBm, &info);
		int nWidth = info.Width;
		int nHeight = info.Height;
		rc.SetRect(0, 0, nWidth, nHeight);
		lprcSrc = &rc;
	}

	return m_fnPaintBitmap(hDC, hBm, lprcDest, lprcSrc) == 0?TRUE:FALSE;	
}

BITMAPLIST_HANDLE CUIGraphic::CreateBitmapList(BITMAP_HANDLE xlBitmap)
{
	return m_fnCreateBitmapList(xlBitmap);
}

unsigned long CUIGraphic::AddRefBitmapList(BITMAPLIST_HANDLE xlBitmapList)
{
	return m_fnAddRefBitmapList(xlBitmapList);
}

unsigned long CUIGraphic::ReleaseBitmapList(BITMAPLIST_HANDLE xlBitmapList)
{
	return m_fnReleaseBitmapList(xlBitmapList);
}

unsigned long CUIGraphic::GetBitmapListCount(BITMAPLIST_HANDLE xlBitmapList)
{
	return m_fnGetBitmapListCount(xlBitmapList);
}

BITMAP_HANDLE CUIGraphic::GetBitmapFromList(BITMAPLIST_HANDLE xlBitmapList, unsigned long index)
{
	return m_fnGetBitmapFromList(xlBitmapList, index);
}

HBITMAP CUIGraphic::CreateHBitmapFromHandle(BITMAP_HANDLE bitmapSrc)
{
	return m_fnCreateHBitmapFromHandle(bitmapSrc);
}

TEXTURE_HANDLE CUIGraphic::CreateFillTexture(BITMAP_HANDLE hBitmap)
{
	return m_fnCreateFillTexture(hBitmap);
}

TEXTURE_HANDLE CUIGraphic::CreateTileTexture(BITMAP_HANDLE hBitmap)
{
	return m_fnCreateTileTexture(hBitmap);
}

TEXTURE_HANDLE CUIGraphic::CreateThreeInOneHTexture(BITMAP_HANDLE xlhbmpSrc, bool bStretchCenter /*= true*/)
{
	return m_fnCreateThreeInOneHTexture(xlhbmpSrc, bStretchCenter);
}

TEXTURE_HANDLE CUIGraphic::CreateThreeInOneVTexture(BITMAP_HANDLE xlhbmpSrc, bool bStretchCenter /*= true*/)
{
	return m_fnCreateThreeInOneVTexture(xlhbmpSrc, bStretchCenter);
}

TEXTURE_HANDLE CUIGraphic::CreateFiveInOneHTexture(BITMAP_HANDLE xlhbmpSrc)
{
	return m_fnCreateFiveOneHTexture(xlhbmpSrc);
}

TEXTURE_HANDLE CUIGraphic::CreateFiveInOneVTexture(BITMAP_HANDLE xlhbmpSrc)
{
	return m_fnCreateFiveOneVTexture(xlhbmpSrc);
}

TEXTURE_HANDLE CUIGraphic::CreateNineInOneTexture(BITMAP_HANDLE xlhbmpSrc)
{
	return m_fnCreateNineOneTexture(xlhbmpSrc);
}

BITMAP_HANDLE CUIGraphic::GetCellBitmapFromTexture(TEXTURE_HANDLE xlTexture, int nIndex)
{
	return m_fnGetCellBitmapFromTexture(xlTexture, nIndex);
}

BITMAP_HANDLE CUIGraphic::UpdateTexture(TEXTURE_HANDLE xlTexture, SIZE newSize)
{
	return m_fnUpdateTexture(xlTexture, newSize);
}

unsigned long CUIGraphic::AddRefTexture(TEXTURE_HANDLE xlTexture)
{
	return m_fnAddRefTexture(xlTexture);
}

unsigned long CUIGraphic::ReleaseTexture(TEXTURE_HANDLE xlTexture)
{
	return m_fnReleaseRefTexture(xlTexture);
}

BOOL CUIGraphic::SetLayeredWindowAttributes(HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags)
{
	return m_fnSetLayeredWindowAttribute(hWnd, cr, bAlpha, dwFlags);
}

BOOL CUIGraphic::UpdateLayeredWindow(HWND hWnd, HDC hdcDst, POINT *pptDst, SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags)
{
	return m_fnUpdateLayeredWindow(hWnd, hdcDst, pptDst, psize, hdcSrc, pptSrc, crKey, pblend, dwFlags);
}

#define ALLOC_UNIT 100
HRGN CUIGraphic::GetRgnFromUIBitmap(BITMAP_HANDLE xlBmp,DWORD dwColor)
{
	HRGN hRgn = NULL;
	DWORD maxRects = ALLOC_UNIT;
	HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
	if(hData == NULL)
	{
		return hRgn;
	}

	RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
	pData->rdh.dwSize = sizeof(RGNDATAHEADER);
	pData->rdh.iType = RDH_RECTANGLES;
	pData->rdh.nCount = pData->rdh.nRgnSize = 0;
	SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

	XLBitmapInfo bmpInfo;
	UIGraphicInstance->GetBitmapInfo(xlBmp,&bmpInfo);
	assert(bmpInfo.ColorType == GRAPHIC_CT_ARGB32);
	if (bmpInfo.ColorType != GRAPHIC_CT_ARGB32) return hRgn;

	BYTE* p32 = UIGraphicInstance->GetBitmapBuffer(xlBmp,0,0);

	for(long y=0;y<(long)bmpInfo.Height;++y)
	{
		for(long x=0;x<(long)bmpInfo.Width;++x)
		{
			int x0 = x;
			LONG *p = (LONG *)p32 + x;

			while (x < (long)bmpInfo.Width)
			{
				if ((*p == (LONG)dwColor))
				{
					break;
				}
				p++;
				x++;
			}

			if (x > x0)
			{
				if (pData->rdh.nCount >= maxRects)
				{
					GlobalUnlock(hData);
					maxRects += ALLOC_UNIT;
					hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
					pData = (RGNDATA *)GlobalLock(hData);
				}

				RECT *pr = (RECT *)&pData->Buffer;
				SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
				if (x0 < pData->rdh.rcBound.left)
					pData->rdh.rcBound.left = x0;
				if (y < pData->rdh.rcBound.top)
					pData->rdh.rcBound.top = y;
				if (x > pData->rdh.rcBound.right)
					pData->rdh.rcBound.right = x;
				if (y+1 > pData->rdh.rcBound.bottom)
					pData->rdh.rcBound.bottom = y+1;
				pData->rdh.nCount++;

				if (pData->rdh.nCount == 2000)
				{
					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
					if (hRgn)
					{
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h);
					}
					else
					{
						hRgn = h;
					}

					pData->rdh.nCount = 0;
					SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
				}
			}
		}

		p32 += bmpInfo.ScanLineLength;
	}

	HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
	if (hRgn)
	{
		CombineRgn(hRgn, hRgn, h, RGN_OR);
		DeleteObject(h);
	}
	else
	{
		hRgn = h;
	}
	GlobalUnlock(hData); 
	GlobalFree(hData);
	return hRgn;
}

void CUIGraphic::GdiDrawString()
{
	if (m_ulGdiToken <= 0)
	{
		return;
	}
}