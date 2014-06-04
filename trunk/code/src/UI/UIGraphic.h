#pragma once
#include "../../UIGraphic/Graphic.h"
#include <uiluax.h>
#include <objbase.h>
#include <GdiPlus.h>
#pragma comment(lib,"GdiPlus.lib")

class CUIGraphic
{
public:
	CUIGraphic(void);
	~CUIGraphic(void);

	typedef int (__stdcall *UIGraphic_InitGraphic_Func)(void*);
	typedef int (__stdcall *UIGraphic_UnInitGraphic_Func)();
	typedef BITMAP_HANDLE (__stdcall *UIGraphic_LoadBitmapFromFile_Func)(const wchar_t*, unsigned long);
	typedef BITMAP_HANDLE (__stdcall *UIGraphic_ClipSubBindBitmap_Func)(BITMAP_HANDLE, RECT*);
	typedef unsigned long (__stdcall *UIGraphic_AddRefBitmap_Func)(BITMAP_HANDLE);
	typedef unsigned long (__stdcall *UIGraphic_ReleaseBitmap_Func)(BITMAP_HANDLE);
	typedef int (__stdcall *UIGraphic_GetBitmapInfo_Func)(BITMAP_HANDLE, XLBitmapInfo*);
	typedef int (__stdcall *UIGraphic_BitmapAlphaBlend_Func)(BITMAP_HANDLE, BITMAP_HANDLE, unsigned char, unsigned long);
	typedef int (__stdcall *UIGraphic_PaintBitmap_Func)(HDC, BITMAP_HANDLE, RECT*, RECT*);
	typedef int (__stdcall *UIGraphic_AlphaPaintBitmap_Func)(HDC, int, int, int, int, BITMAP_HANDLE, int, int, int, int, BLENDFUNCTION);
	typedef BITMAPLIST_HANDLE (__stdcall *UIGraphic_CreateBitmapList_Func)(BITMAP_HANDLE);
	typedef unsigned long (__stdcall *UIGraphic_AddRefBitmapList_Func)(BITMAPLIST_HANDLE);
	typedef unsigned long (__stdcall *UIGraphic_ReleaseBitmapList_Func)(BITMAPLIST_HANDLE);
	typedef unsigned long (__stdcall *UIGraphic_GetBitmapListCount_Func)(BITMAPLIST_HANDLE);
	typedef BITMAP_HANDLE (__stdcall *UIGraphic_GetBitmapFromList_Func)(BITMAPLIST_HANDLE, unsigned long);
	typedef BYTE* (__stdcall *UIGraphic_GetBitmapBuffer_Func)(BITMAP_HANDLE,unsigned long,unsigned long);
	typedef HBITMAP (__stdcall *UIGraphic_CreateHBitmapFromHandle_Func)(BITMAP_HANDLE bitmapSrc);
	typedef TEXTURE_HANDLE (__stdcall *UIGraphic_CreateFillTexture_Func)(BITMAP_HANDLE hBitmap);
	typedef TEXTURE_HANDLE (__stdcall *UIGraphic_CreateTileTexture_Func)(BITMAP_HANDLE hBitmap);
	typedef TEXTURE_HANDLE(__stdcall *UIGraphic_CreateThreeInOneHTexture_Func)(BITMAP_HANDLE, bool);
	typedef TEXTURE_HANDLE(__stdcall *UIGraphic_CreateThreeInOneVTexture_Func)(BITMAP_HANDLE, bool);
	typedef TEXTURE_HANDLE(__stdcall *UIGraphic_CreateFiveInOneHTexture_Func)(BITMAP_HANDLE);
	typedef TEXTURE_HANDLE(__stdcall *UIGraphic_CreateFiveInOneVTexture_Func)(BITMAP_HANDLE);
	typedef TEXTURE_HANDLE(__stdcall *UIGraphic_CreateNineInOneTexture_Func)(BITMAP_HANDLE);
	typedef BITMAP_HANDLE(__stdcall *UIGraphic_GetCellBitmapFromTexture_Func)(TEXTURE_HANDLE, int);
	typedef BITMAP_HANDLE(__stdcall *UIGraphic_UpdateTexture_Func)(TEXTURE_HANDLE, SIZE);
	typedef unsigned long(__stdcall *UIGraphic_AddRefTexture_Func)(TEXTURE_HANDLE);
	typedef unsigned long(__stdcall *UIGraphic_ReleaseRefTexture_Func)(TEXTURE_HANDLE);

	typedef BOOL (WINAPI *UIGraphic_lpfnSetLayeredWindowAttribute)(HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);
	typedef BOOL (WINAPI *UIGraphic_lpfnUpdateLayeredWindow)(HWND hWnd, HDC hdcDst, POINT *pptDst, SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);
protected:
	UIGraphic_InitGraphic_Func               m_fnInitGraphicLib;
	UIGraphic_UnInitGraphic_Func             m_fnUnInitGraphicLib;
	UIGraphic_LoadBitmapFromFile_Func        m_fnLoadBitmapFromFile;
	UIGraphic_ClipSubBindBitmap_Func         m_fnClipSubBindBitmap;
	UIGraphic_AddRefBitmap_Func              m_fnAddRefBitmap;
	UIGraphic_ReleaseBitmap_Func             m_fnReleaseBitmap;
	UIGraphic_GetBitmapInfo_Func             m_fnGetBitmapInfo;
	UIGraphic_BitmapAlphaBlend_Func          m_fnBitmapAlphaBlend;
	UIGraphic_PaintBitmap_Func	             m_fnPaintBitmap;
	UIGraphic_AlphaPaintBitmap_Func          m_fnAlphaPaintBitmap;
	UIGraphic_CreateBitmapList_Func          m_fnCreateBitmapList;
	UIGraphic_AddRefBitmapList_Func          m_fnAddRefBitmapList;
	UIGraphic_ReleaseBitmapList_Func         m_fnReleaseBitmapList;
	UIGraphic_GetBitmapListCount_Func        m_fnGetBitmapListCount;
	UIGraphic_GetBitmapFromList_Func         m_fnGetBitmapFromList;
	UIGraphic_GetBitmapBuffer_Func           m_fnGetBitmapBuffer;
	UIGraphic_CreateHBitmapFromHandle_Func   m_fnCreateHBitmapFromHandle;
	UIGraphic_CreateFillTexture_Func		 m_fnCreateFillTexture;
	UIGraphic_CreateTileTexture_Func		 m_fnCreateTileTexture;
	UIGraphic_CreateThreeInOneHTexture_Func	 m_fnCreateThreeInOneHTexture;
	UIGraphic_CreateThreeInOneVTexture_Func  m_fnCreateThreeInOneVTexture;
	UIGraphic_CreateFiveInOneHTexture_Func   m_fnCreateFiveOneHTexture;
	UIGraphic_CreateFiveInOneVTexture_Func   m_fnCreateFiveOneVTexture;
	UIGraphic_CreateNineInOneTexture_Func    m_fnCreateNineOneTexture;
	UIGraphic_UpdateTexture_Func			 m_fnUpdateTexture;
	UIGraphic_GetCellBitmapFromTexture_Func  m_fnGetCellBitmapFromTexture;
	UIGraphic_AddRefTexture_Func             m_fnAddRefTexture;
	UIGraphic_ReleaseRefTexture_Func         m_fnReleaseRefTexture;
	//USER32.DLLµÄAPI
	UIGraphic_lpfnSetLayeredWindowAttribute  m_fnSetLayeredWindowAttribute;
	UIGraphic_lpfnUpdateLayeredWindow		 m_fnUpdateLayeredWindow;
public:
	BOOL InitGraphic();
	void UnInitGraphic();
	BOOL GetBitmapSize(BITMAP_HANDLE hndBitmap, int* nWidth, int* nHeight);
	BOOL AlphaPaintBitmap(HDC hdc, BITMAP_HANDLE hBm,  LPRECT lprcDest, LPRECT lprcSrc);
	BOOL AlphaPaintBitmap(HDC hDC, BITMAP_HANDLE hBm, int x, int y, long cx, long cy);
	BITMAP_HANDLE LoadBitmapFromFile(const wchar_t* file, unsigned long type  = 0 );
	BITMAP_HANDLE ClipSubBindBitmap(BITMAP_HANDLE xlBitmap, RECT* pRect);
	BITMAP_HANDLE ClipSubBindBitmap(BITMAP_HANDLE xlBitmapSrc, int nBlockNum, int nSelectIndex);
	unsigned long AddRefBitmap(BITMAP_HANDLE xlBitmap);
	unsigned long ReleaseBitmap(BITMAP_HANDLE xlBitmap);
	int GetBitmapInfo(BITMAP_HANDLE xlBitmap, XLBitmapInfo* pInfo);
	int AlphaBlend(BITMAP_HANDLE xlBitmapSrc, BITMAP_HANDLE xlBitmapDest, unsigned char alphaValue, unsigned long alphaType);
	int  PaintBitmap(HDC hdc, RECT& rcDest, BITMAP_HANDLE xlBitmapSrc, RECT& rcSrc);
	BOOL PaintBitmap(HDC hDC, BITMAP_HANDLE hBm, int x, int y, long cx, long cy);
	BOOL PaintBitmap(HDC hDC, BITMAP_HANDLE hBm, LPRECT lprcDest, LPRECT lprcSrc);
	BITMAPLIST_HANDLE CreateBitmapList(BITMAP_HANDLE xlBitmap);
	unsigned long AddRefBitmapList(BITMAPLIST_HANDLE xlBitmapList);
	unsigned long ReleaseBitmapList(BITMAPLIST_HANDLE xlBitmapList);
	unsigned long GetBitmapListCount(BITMAPLIST_HANDLE xlBitmapList);
	BYTE* GetBitmapBuffer(BITMAP_HANDLE hBitmap,unsigned long x,unsigned long y);
	BITMAP_HANDLE GetBitmapFromList(BITMAPLIST_HANDLE xlBitmapList, unsigned long index);
	HBITMAP CreateHBitmapFromHandle(BITMAP_HANDLE bitmapSrc);
	TEXTURE_HANDLE CreateFillTexture(BITMAP_HANDLE hBitmap);
	TEXTURE_HANDLE CreateTileTexture(BITMAP_HANDLE hBitmap);
	TEXTURE_HANDLE CreateThreeInOneHTexture(BITMAP_HANDLE xlhbmpSrc, bool bStretchCenter = true);
	TEXTURE_HANDLE CreateThreeInOneVTexture(BITMAP_HANDLE xlhbmpSrc, bool bStretchCenter = true);
	TEXTURE_HANDLE CreateFiveInOneHTexture(BITMAP_HANDLE xlhbmpSrc);
	TEXTURE_HANDLE CreateFiveInOneVTexture(BITMAP_HANDLE xlhbmpSrc);
	TEXTURE_HANDLE CreateNineInOneTexture(BITMAP_HANDLE xlhbmpSrc);
	BITMAP_HANDLE GetCellBitmapFromTexture(TEXTURE_HANDLE xlTexture, int nIndex);
	BITMAP_HANDLE UpdateTexture(TEXTURE_HANDLE xlTexture, SIZE newSize);
	unsigned long AddRefTexture(TEXTURE_HANDLE xlTexture);
	unsigned long ReleaseTexture(TEXTURE_HANDLE xlTexture);
	BOOL SetLayeredWindowAttributes(HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);
	BOOL UpdateLayeredWindow(HWND hWnd, HDC hdcDst, POINT *pptDst, SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);
	HRGN GetRgnFromUIBitmap(BITMAP_HANDLE xlBmp,DWORD dwColor);

	void GdiDrawString();
private:
	HMODULE m_hGraphicDll;
	ULONG_PTR m_ulGdiToken;
private:
	void InitGdiplus();
public:
	BEGIN_LUA_CALL_MAP(CUIGraphic)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_SINGLETON_OBJECT(CUIGraphic, UI.Graphic);
public:
	LOG_CLS_DEC();
};

#define UIGraphicInstance CUIGraphic::GetInstance()