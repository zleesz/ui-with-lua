// XLGraphic.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include <windows.h>
#include "./Graphic.h"

#include "./DefaultBltProvider.h"
#include "./SSEStretchProvider.h"
#include "./DefaultMixProvider.h"
#include "./MMXMixProvider.h"
#include "./BSplineBltProvider.h"

#include "./Loader/ImageLoaderFactory.h"
#include "./Loader/PNGLoader.h"

#include "./Texture/Texture.h"
#include "./Texture/NineInOneTexture.h"
#include "./Texture/FillTexture.h"
#include "./Texture/TileTexture.h"
#include "./Texture/ThreeInOneHTexture.h"
#include "./Texture/ThreeInOneVTexture.h"
#include "./Texture/FiveInOneVTexture.h"
#include "./Texture/FiveInOneHTexture.h"

#include "./BitmapList.h"

#include "./CPUInfo.h"

using namespace std;
using namespace Graphic;
using namespace System;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

IMixProvider* g_pMixProvider = NULL;
IBltProvider* g_pBltProvider = NULL;

static bool g_isInitLib = false;
//----------------初始化库-------------------------------
GRAPHIC_API(int) InitGraphicLib(void* initParam)
{
    if(g_isInitLib)
        return 0;

    g_isInitLib = true;

    if (CPUInfo::Instance().MMXSupport)
    {
        g_pMixProvider = new MMXMixProvider();
    }
    else
    {
        g_pMixProvider = new DefaultMixProvider();
    }
    if (CPUInfo::Instance().SSESupport)
    {
        g_pBltProvider = new SSEStretchProvider();
    }
    else
    {
        g_pBltProvider = new DefaultBltProvider();
    }
    ImageLoaderFactory::Instance().RegisterLoader(PNGLoader::GetLoaderNode());

	PNGLoader::SetMixProvider(g_pMixProvider);
    return 0;
}

GRAPHIC_API(int) UnInitGraphicLib()
{
    delete g_pBltProvider;
    g_pBltProvider = NULL;
    delete g_pMixProvider;
    g_pMixProvider = NULL;
    return 0;
}

//-----------Bitmap数据结构定义---------------------------
GRAPHIC_API(BITMAP_HANDLE) CreateSizeBitmap(unsigned long BitmapColorType,unsigned long Width,unsigned long Height)
{
    Bitmap* pResult = new Bitmap(BitmapColorType,Width,Height);
    return (BITMAP_HANDLE) pResult;
}

GRAPHIC_API(BITMAP_HANDLE) CreateBindBitmap(XLBitmapInfo* pBitmapInfo,BYTE* pBuffer)
{
    Bitmap* pResult = new Bitmap(pBitmapInfo->ColorType,pBitmapInfo->Width,pBitmapInfo->Height,pBitmapInfo->ScanLineLength,pBuffer);
    return (BITMAP_HANDLE) pResult;
}

GRAPHIC_API(unsigned long) AddRefBitmap(BITMAP_HANDLE hBitmap)
{
    Bitmap* pBitmap = (Bitmap*) hBitmap;
    return pBitmap->AddRef();
}

GRAPHIC_API(unsigned long) ReleaseBitmap(BITMAP_HANDLE hBitmap)
{
    Bitmap* pBitmap = (Bitmap*) hBitmap;  
    return pBitmap->Release();
}

//得到一个BindBuffer的新位图,该位图的有效性取决于hSrcBitmap的有效性
GRAPHIC_API(BITMAP_HANDLE) ClipSubBindBitmap(BITMAP_HANDLE hSrcBitmap,RECT* pClibRect)
{
    Bitmap* pBitmap = (Bitmap*) hSrcBitmap;  
    Bitmap* pResult = pBitmap->ClipSubBindBitmap(pClibRect);
    return (BITMAP_HANDLE) pResult;
}

//得到一个OwnerBuffer的新位图
GRAPHIC_API(BITMAP_HANDLE) CloneBitmap(BITMAP_HANDLE hSrcBitmap)
{
    Bitmap* pBitmap = (Bitmap*) hSrcBitmap;  
    Bitmap* pResult = pBitmap->CloneBitmap();
    return (BITMAP_HANDLE) pResult;
}

//得到Bitmap的信息
GRAPHIC_API(int) GetBitmapInfo(BITMAP_HANDLE hBitmap,XLBitmapInfo* pResult)
{   
    assert(pResult);
    if(pResult != NULL && hBitmap != NULL)
    {
        Bitmap* pBitmap = (Bitmap*) hBitmap;      
        *pResult = pBitmap->m_info;
    }
    return 0;
}

GRAPHIC_API(BOOL) IsBitmapOwnerBuffer(BITMAP_HANDLE hBitmap)
{
    Bitmap* pBitmap = (Bitmap*) hBitmap;      
    return pBitmap->IsOwnerBuffer();
}

GRAPHIC_API(BYTE*) GetBitmapBuffer(BITMAP_HANDLE hBitmap,unsigned long x,unsigned long y)
{
    Bitmap* pBitmap = (Bitmap*) hBitmap;
    return pBitmap->GetBuffer(x,y);
}

//----------Bitmap的一些Blend操作
GRAPHIC_API(int) BitmapAlphaBlend(BITMAP_HANDLE hDestBitmap,BITMAP_HANDLE hSrcBitmap,unsigned char alphaValue,unsigned long alphaType)
{
    g_pMixProvider->SetAlphaMixSetting(alphaType);
    g_pMixProvider->Alpha(*((Bitmap*)hDestBitmap),*((Bitmap*)hSrcBitmap),alphaValue);
    return 0;
}

//---------Bitmap的拉伸操作
GRAPHIC_API(BITMAP_HANDLE) StretchBitmap(BITMAP_HANDLE hBitmap,unsigned long newWidth,unsigned long newHeight)
{
    Bitmap* pSrc = (Bitmap*) hBitmap;
    Bitmap* pResult = NULL;
    pResult = g_pBltProvider->Stretch(*pSrc,newWidth,newHeight);
    return (BITMAP_HANDLE) pResult;
}

//---------Bitmap的色彩调整操作
GRAPHIC_API(int) AdjustBitmapColor(BITMAP_HANDLE hBitmap)
{
    return -1;    
}
//针对一个ARGB点的颜色调整

//---------Bitmap的PNG32预处理
GRAPHIC_API(BOOL) PretreatmentPNG32(BITMAP_HANDLE pBitmap)
{
	//这个API存在问题，暂时不要使用
	//return FALSE;
	Bitmap* png32 = (Bitmap*) pBitmap;
	g_pMixProvider->PretreatmentPNG32(png32);
	//ImageLoaderFactory::Instance().GetLoader(
	return TRUE;
}

//---------Bitmap loader ??  如何支持读取，保存，处理大位图？ 暂时 不用考虑该问题，是否可以平滑升级到支持该特性
GRAPHIC_API(BITMAP_HANDLE) LoadBitmapFromMemory(const void *buffer, long size, unsigned long colorType)
{
	if (buffer == NULL)
		return NULL;

    Bitmap* pBitmap = ImageLoaderFactory::Instance().LoadImageFromMemory((void *)buffer,size,colorType);   
    return (BITMAP_HANDLE)pBitmap;
}

GRAPHIC_API(BITMAP_HANDLE) LoadBitmapFromFile(const wchar_t* filePath,unsigned long colorType)
{
	if (filePath == NULL || filePath[0] == L'\0')
		return NULL;

    Bitmap* pBitmap = ImageLoaderFactory::Instance().LoadImageFromFile(filePath,colorType);   
    return (BITMAP_HANDLE)pBitmap;
}

GRAPHIC_API(BITMAP_HANDLE) LoadBitmapFromFileReverse(const wchar_t*filePath,unsigned long colorType)
{
	if (filePath == NULL || filePath[0] == L'\0')
		return NULL;

	Bitmap* pBitmap = ImageLoaderFactory::Instance().LoadImageFromFileReverse(filePath,colorType);   
	return (BITMAP_HANDLE)pBitmap;
}


GRAPHIC_API(BITMAP_HANDLE) LoadBitmapFromDllRes(const wchar_t* dllPath,const wchar_t* resID,unsigned long colorType)
{
    return NULL;    
}

GRAPHIC_API(BITMAPLIST_HANDLE) CreateBitmapList(BITMAP_HANDLE hBitmap)
{
    BitmapList* pResult = new BitmapList((Bitmap*)hBitmap);
    return (BITMAPLIST_HANDLE) pResult;
}

GRAPHIC_API(unsigned long) AddRefBitmapList(BITMAPLIST_HANDLE hBitmapList)
{
    BitmapList* pList = (BitmapList*) hBitmapList;
    return pList->AddRef();
}

GRAPHIC_API(unsigned long) ReleaseBitmapList(BITMAPLIST_HANDLE hBitmapList)
{
    BitmapList* pList = (BitmapList*) hBitmapList;
    return pList->Release();
}

GRAPHIC_API(unsigned long) GetBitmapListCount(BITMAPLIST_HANDLE hBitmapList)
{
    BitmapList* pList = (BitmapList*) hBitmapList;
    return pList->GetBitmapCount();
}

GRAPHIC_API(BITMAP_HANDLE) GetBitmapFromList(BITMAPLIST_HANDLE hBitmapList,unsigned long index)
{
    BitmapList* pList = (BitmapList*) hBitmapList;
    return (BITMAP_HANDLE)pList->GetBitmapByIndex(index);
}

//----- 直接绘制函数
//GRAPHIC_API void DrawText();
//GRAPHIC_API void DrawLine();
//GRAPHIC_API void DrawRect();

//-------------- 一些和Windows有关的OS函数，方便使用，可能废除--------------
GRAPHIC_API(int) PaintBitmap(HDC hDC,BITMAP_HANDLE hBitmap,RECT* pDestRect,RECT* pSrcRect)
{
    if (!hBitmap) return 0;
    Bitmap* pSrcBitmap = (Bitmap*)hBitmap;
    if (pSrcBitmap->m_info.Width <= 0) return 0;
    if (pSrcBitmap->m_info.Height <= 0) return 0;
    if (pSrcBitmap->m_info.ColorType != GRAPHIC_CT_ARGB32) return 0;

    assert(pSrcBitmap->m_info.ColorType == GRAPHIC_CT_ARGB32);

    int srcX = 0;
    int srcY = 0;
    long srcWidth = 0;
    long srcHeight = 0;

    if(pSrcRect)
    {
        srcX = pSrcRect->left;
        srcY = pSrcRect->top;
        srcWidth = pSrcRect->right - pSrcRect->left;
        srcHeight= pSrcRect->bottom - pSrcRect->top;
        if(srcHeight > (long)pSrcBitmap->m_info.Height)
            srcHeight = pSrcBitmap->m_info.Height;
        if(srcWidth > (long)pSrcBitmap->m_info.Width)
            srcWidth = pSrcBitmap->m_info.Width;
    }
    else
    {
        srcWidth = pSrcBitmap->m_info.Width;
        srcHeight = pSrcBitmap->m_info.Height;

        if(srcWidth > (pDestRect->right - pDestRect->left))
            srcWidth = pDestRect->right - pDestRect->left;
        if(srcHeight > (pDestRect->bottom - pDestRect->top))
            srcHeight = pDestRect->bottom - pDestRect->top;
    }

    //构建DIB头
    BITMAPINFO bmpInfo;
    if(pSrcBitmap->IsOwnerBuffer() || 
        pSrcBitmap->GetPixelBytes()*pSrcBitmap->m_info.Width == pSrcBitmap->m_info.ScanLineLength)
    {
        bmpInfo.bmiHeader.biHeight = -(long)pSrcBitmap->m_info.Height;
        bmpInfo.bmiHeader.biWidth = pSrcBitmap->m_info.Width;
        bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpInfo.bmiHeader.biPlanes = 1;
        bmpInfo.bmiHeader.biBitCount = (unsigned short)pSrcBitmap->GetPixelBytes()<<3;
        bmpInfo.bmiHeader.biCompression = BI_RGB;
        bmpInfo.bmiHeader.biSizeImage = 0;
        bmpInfo.bmiHeader.biXPelsPerMeter = 0;
        bmpInfo.bmiHeader.biYPelsPerMeter = 0;
        bmpInfo.bmiHeader.biClrUsed = 0;
        bmpInfo.bmiHeader.biClrImportant = 0;

        return ::StretchDIBits(hDC,pDestRect->left,pDestRect->top,(pDestRect->right-pDestRect->left),(pDestRect->bottom-pDestRect->top)
            ,srcX,-srcY,srcWidth,srcHeight,pSrcBitmap->GetBuffer(0,0),&bmpInfo,DIB_RGB_COLORS,SRCCOPY);
    }
    else
    {
        Bitmap* pCloneBmp = pSrcBitmap->CloneBitmap();
        bmpInfo.bmiHeader.biHeight = -(long)pCloneBmp->m_info.Height;
        bmpInfo.bmiHeader.biWidth = pCloneBmp->m_info.Width;
        bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpInfo.bmiHeader.biPlanes = 1;
        bmpInfo.bmiHeader.biBitCount = (unsigned short)pCloneBmp->GetPixelBytes()<<3;
        bmpInfo.bmiHeader.biCompression = BI_RGB;
        bmpInfo.bmiHeader.biSizeImage = 0;
        bmpInfo.bmiHeader.biXPelsPerMeter = 0;
        bmpInfo.bmiHeader.biYPelsPerMeter = 0;
        bmpInfo.bmiHeader.biClrUsed = 0;
        bmpInfo.bmiHeader.biClrImportant = 0;

        int result = ::StretchDIBits(hDC,pDestRect->left,pDestRect->top,(pDestRect->right-pDestRect->left),(pDestRect->bottom-pDestRect->top)
            ,srcX,-srcY,srcWidth,srcHeight,pCloneBmp->GetBuffer(0,0),&bmpInfo,DIB_RGB_COLORS,SRCCOPY);
        pCloneBmp->Release();
        return result;
    }
}

GRAPHIC_API(int) AlphaPaintBitmap(HDC hdcDest, int nXOriginDest, int nYOriginDest,
                                       int nWidthDest, int nHeightDest,
                                       BITMAP_HANDLE hSrcBitmap, int nXOriginSrc, int nYOriginSrc,
                                       int nWidthSrc, int nHeightSrc,
                                       BLENDFUNCTION blendFunction)
{
    if (!hSrcBitmap) return 0;
    //TODO: 这里考虑用DIB Section优化,否则在非32bit色深环境下，该函数失效
	HDC dcMemory;
	dcMemory = ::CreateCompatibleDC(hdcDest);
	HBITMAP bitmapMemory;
    BITMAPINFO bmpInfo;
    bmpInfo.bmiHeader.biHeight = -nHeightSrc;
    bmpInfo.bmiHeader.biWidth = nWidthSrc;
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;
    bmpInfo.bmiHeader.biSizeImage = 0;
    bmpInfo.bmiHeader.biXPelsPerMeter = 0;
    bmpInfo.bmiHeader.biYPelsPerMeter = 0;
    bmpInfo.bmiHeader.biClrUsed = 0;
    bmpInfo.bmiHeader.biClrImportant = 0;
    BYTE* pbase = NULL;
    bitmapMemory=CreateDIBSection(dcMemory,&bmpInfo,DIB_RGB_COLORS,(void**)&pbase,0,0);
    //bitmapMemory = ::CreateBitmap(nWidthSrc,nHeightSrc,4,8,NULL);
    //bitmapMemory = ::CreateCompatibleBitmap(hdcDest,nWidthSrc,nHeightSrc);
    ::SelectObject(dcMemory,bitmapMemory);

	RECT rcDest;
    RECT rcSrc;
	::SetRect(&rcDest, 0, 0, nWidthSrc, nHeightSrc);
    ::SetRect(&rcSrc, nXOriginSrc, nYOriginSrc, nXOriginSrc+nWidthSrc, nYOriginSrc+nHeightSrc);
	PaintBitmap(dcMemory,hSrcBitmap,&rcDest,&rcSrc);
    
    //::BitBlt(hdcDest,nXOriginSrc,nYOriginDest,nWidthDest,nHeightDest,dcMemory,0,0,SRCCOPY);
	::AlphaBlend(hdcDest,nXOriginDest,nYOriginDest,nWidthDest,nHeightDest,
		dcMemory,0,0,nWidthSrc,nHeightSrc,blendFunction);
    
	
    ::DeleteDC(dcMemory);
    ::DeleteObject(bitmapMemory);
    return 0;
}

GRAPHIC_API(HBITMAP) CreateHBitmapFromHandle(BITMAP_HANDLE bitmapSrc)
{
	HDC dcMemory;
	dcMemory = ::CreateCompatibleDC(::GetWindowDC(GetDesktopWindow()));
	HBITMAP bitmapMemory;

	XLBitmapInfo info;
	GetBitmapInfo(bitmapSrc,&info);
	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biHeight = -(long)info.Height;
	bmpInfo.bmiHeader.biWidth = info.Width;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biSizeImage = 0;
	bmpInfo.bmiHeader.biXPelsPerMeter = 0;
	bmpInfo.bmiHeader.biYPelsPerMeter = 0;
	bmpInfo.bmiHeader.biClrUsed = 0;
	bmpInfo.bmiHeader.biClrImportant = 0;
	BYTE* pbase = NULL;
	bitmapMemory=CreateDIBSection(dcMemory,&bmpInfo,DIB_RGB_COLORS,(void**)&pbase,0,0);
	//bitmapMemory = ::CreateBitmap(nWidthSrc,nHeightSrc,4,8,NULL);
	//bitmapMemory = ::CreateCompatibleBitmap(hdcDest,nWidthSrc,nHeightSrc);
	::SelectObject(dcMemory,bitmapMemory);

	RECT rcDest;
	//RECT rcSrc;
	::SetRect(&rcDest, 0, 0, info.Width, info.Height);
	PaintBitmap(dcMemory,bitmapSrc,&rcDest,&rcDest);

	return bitmapMemory;
}

GRAPHIC_API(unsigned long) AddRefTexture(TEXTURE_HANDLE hTexture)
{
    BaseTexture* pTexture = (BaseTexture*) hTexture;
    return pTexture->AddRef();
}

GRAPHIC_API(unsigned long) ReleaseTexture(TEXTURE_HANDLE hTexture)
{
    BaseTexture* pTexture = (BaseTexture*) hTexture;
    return pTexture->Release();
}

GRAPHIC_API(long) SetTextureBitmap(TEXTURE_HANDLE hTexture,BITMAP_HANDLE hBitmap)
{
    BaseTexture* pTexture = (BaseTexture*) hTexture;
    return pTexture->SetBitmap((Bitmap*)hBitmap);
}

GRAPHIC_API(BITMAP_HANDLE) UpdateTexture(TEXTURE_HANDLE hTexture,SIZE newSize)
{
    if (!hTexture) return NULL;
    if (newSize.cx <= 0 || newSize.cy <= 0 || newSize.cx > 0xFFFF || newSize.cy > 0xFFFF) return NULL;
    BaseTexture* pTexture = (BaseTexture*) hTexture;
    return (BITMAP_HANDLE)pTexture->UpdateTexture(newSize);
}

GRAPHIC_API(TEXTURE_HANDLE) CreateFillTexture(BITMAP_HANDLE hBitmap)
{
    if (!hBitmap) return NULL;
    FillTexture* pNewTexture = new FillTexture((Bitmap*) hBitmap);
    return (TEXTURE_HANDLE)((BaseTexture*)pNewTexture);
}

GRAPHIC_API(TEXTURE_HANDLE) CreateTileTexture(BITMAP_HANDLE hBitmap)
{
    if (!hBitmap) return NULL;
    TileTexture* pNewTexture = new TileTexture((Bitmap*) hBitmap);
    return (TEXTURE_HANDLE)((BaseTexture*)pNewTexture);
}

GRAPHIC_API(TEXTURE_HANDLE) CreateThreeInOneHTexture(BITMAP_HANDLE hBitmap,bool isCenterStretch)
{
    if (!hBitmap) return NULL;
    ThreeInOneHTexture* pNewTexture = new ThreeInOneHTexture((Bitmap*) hBitmap,isCenterStretch);
    return (TEXTURE_HANDLE)((BaseTexture*)pNewTexture);
}

GRAPHIC_API(TEXTURE_HANDLE) CreateThreeInOneVTexture(BITMAP_HANDLE hBitmap,bool isCenterStretch)
{
    if (!hBitmap) return NULL;
    ThreeInOneVTexture* pNewTexture = new ThreeInOneVTexture((Bitmap*) hBitmap,isCenterStretch);
    return (TEXTURE_HANDLE)((BaseTexture*)pNewTexture);
}

GRAPHIC_API(TEXTURE_HANDLE) CreateFiveInOneHTexture(BITMAP_HANDLE hBitmap)
{
    if (!hBitmap) return NULL;
    FiveInOneHTexture* pNewTexture = new FiveInOneHTexture((Bitmap*) hBitmap);
    return (TEXTURE_HANDLE)((BaseTexture*)pNewTexture);
}

GRAPHIC_API(TEXTURE_HANDLE) CreateFiveInOneVTexture(BITMAP_HANDLE hBitmap)
{
    if (!hBitmap) return NULL;
    FiveInOneVTexture* pNewTexture = new FiveInOneVTexture((Bitmap*) hBitmap);
    return (TEXTURE_HANDLE)((BaseTexture*)pNewTexture);
}

GRAPHIC_API(TEXTURE_HANDLE) CreateNineInOneTexture(BITMAP_HANDLE hBitmap)
{
    if (!hBitmap) return NULL;
    NineInOneTexture* pNewTexture = new NineInOneTexture((Bitmap*) hBitmap);
    return (TEXTURE_HANDLE)((BaseTexture*)pNewTexture);
}

GRAPHIC_API(BITMAP_HANDLE) GetCellBitmapFromTexture(TEXTURE_HANDLE hTexture,DWORD index)
{
    if (!hTexture) return NULL;
    BaseTexture* pTexture = (BaseTexture*) hTexture;
    return (BITMAP_HANDLE)pTexture->GetCellBitmap(index);
}
