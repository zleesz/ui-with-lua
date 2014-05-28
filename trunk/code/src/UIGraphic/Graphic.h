
#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GRAPHIC_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GRAPHIC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef GRAPHIC_EXPORTS
#define GRAPHIC_API(x) extern "C" __declspec(dllexport) x __stdcall 
#else
#define GRAPHIC_API(x) extern "C" __declspec(dllimport) x __stdcall 
#endif

#include <wingdi.h>
//位图结构
typedef struct tagXLBitmapInfo
{
    unsigned long ColorType;
	unsigned long Width;
	unsigned long Height;
	long ScanLineLength; //如果scan line是负数，表示位图是倒序存储的

}XLBitmapInfo;


//alpha 混合的定义
#define GRAPHIC_BLEND_ARGB         0
#define GRAPHIC_BLEND_ARGB_ALPHA   1
#define GRAPHIC_BLEND_XRGB_ALPHA   2
#define GRAPHIC_BLEND_XXXX_SRCCOPY 3

//颜色类型定义
#define	GRAPHIC_CT_ARGB32 0
#define GRAPHIC_CT_RGB24  1

#define GRAPHIC_CT_UNKNOWN 0xCDCDCDCD

//Texture类型定义
#define XLTEXTURE_TYPE_FILL           0
#define XLTEXTURE_TYPE_NINE_IN_ONE    1
#define XLTEXTURE_TYPE_THREE_IN_ONE_V 2
#define XLTEXTURE_TYPE_THREE_IN_ONE_H 3
#define XLTEXTURE_TYPE_TILE           4

typedef DWORD* BITMAP_HANDLE;
typedef DWORD* BITMAPLIST_HANDLE;
typedef DWORD* TEXTURE_HANDLE;

//----------------初始化库-------------------------------
GRAPHIC_API(int) InitGraphicLib(void* initParam);
GRAPHIC_API(int) UnInitGraphicLib();

//-----------Bitmap数据结构定义---------------------------
GRAPHIC_API(BITMAP_HANDLE) CreateSizeBitmap(unsigned long BitmapColorType,unsigned long Width,unsigned long Height);
GRAPHIC_API(BITMAP_HANDLE) CreateBindBitmap(XLBitmapInfo* pBitmapInfo,BYTE* pBuffer);
GRAPHIC_API(unsigned long) AddRefBitmap(BITMAP_HANDLE hBitmap);
GRAPHIC_API(unsigned long) ReleaseBitmap(BITMAP_HANDLE hBitmap);

//得到一个BindBuffer的新位图,该位图的有效性取决于hSrcBitmap的有效性
GRAPHIC_API(BITMAP_HANDLE) ClipSubBindBitmap(BITMAP_HANDLE hSrcBitmap,RECT* pClibRect);
//得到一个OwnerBuffer的新位图
GRAPHIC_API(BITMAP_HANDLE) CloneBitmap(BITMAP_HANDLE hSrcBitmap);

//得到Bitmap的信息
GRAPHIC_API(int) GetBitmapInfo(BITMAP_HANDLE hBitmap,XLBitmapInfo* pResult);
GRAPHIC_API(BOOL) IsBitmapOwnerBuffer(BITMAP_HANDLE hBitmap);
GRAPHIC_API(BYTE*) GetBitmapBuffer(BITMAP_HANDLE hBitmap,unsigned long x,unsigned long y);

//----------Bitmap的一些Blend操作
GRAPHIC_API(int) BitmapAlphaBlend(BITMAP_HANDLE hDestBitmap,BITMAP_HANDLE hSrcBitmap,unsigned char alphaValue,unsigned long alphaType);
GRAPHIC_API(HBITMAP)CreateHBitmapFromHandle(BITMAP_HANDLE bitmapSrc);
//---------Bitmap的拉伸操作
GRAPHIC_API(BITMAP_HANDLE) StretchBitmap(BITMAP_HANDLE hBitmap,unsigned long newWidth,unsigned long newHeight);

//---------Bitmap的色彩调整操作
GRAPHIC_API(int) AdjustBitmapColor(BITMAP_HANDLE hBitmap); 
//针对一个ARGB点的颜色调整

//---------Bitmap的PNG32预处理
GRAPHIC_API(BOOL) PretreatmentPNG32(BITMAP_HANDLE pBitmap);

//---------Bitmap loader ??  如何支持读取，保存，处理大位图？ 暂时 不用考虑该问题，是否可以平滑升级到支持该特性
GRAPHIC_API(BITMAP_HANDLE) LoadBitmapFromMemory(const void *buffer, long size, unsigned long colorType);
GRAPHIC_API(BITMAP_HANDLE) LoadBitmapFromFile(const wchar_t* filePath,unsigned long colorType);
GRAPHIC_API(BITMAP_HANDLE) LoadBitmapFromFileReverse(const wchar_t*filePath,unsigned long colorType);
GRAPHIC_API(BITMAP_HANDLE) LoadBitmapFromDllRes(const wchar_t* dllPath,const wchar_t* resID,unsigned long colorType);

//---------Bitmap List----------------------------
GRAPHIC_API(BITMAPLIST_HANDLE) CreateBitmapList(BITMAP_HANDLE hBitmap);
GRAPHIC_API(unsigned long) AddRefBitmapList(BITMAPLIST_HANDLE hBitmapList);
GRAPHIC_API(unsigned long) ReleaseBitmapList(BITMAPLIST_HANDLE hBitmapList);
GRAPHIC_API(unsigned long) GetBitmapListCount(BITMAPLIST_HANDLE hBitmapList);
GRAPHIC_API(BITMAP_HANDLE) GetBitmapFromList(BITMAPLIST_HANDLE hBitmapList,unsigned long index);
//----- 直接绘制函数
//GRAPHIC_API void DrawText();
//GRAPHIC_API void DrawLine();
//GRAPHIC_API void DrawRect();

//----------Texture---------------
GRAPHIC_API(unsigned long) AddRefTexture(TEXTURE_HANDLE hTexture);
GRAPHIC_API(unsigned long) ReleaseTexture(TEXTURE_HANDLE hTexture);

GRAPHIC_API(long) SetTextureBitmap(TEXTURE_HANDLE hTexture,BITMAP_HANDLE hBitmap);
GRAPHIC_API(BITMAP_HANDLE) UpdateTexture(TEXTURE_HANDLE hTexture,SIZE newSize);
GRAPHIC_API(BITMAP_HANDLE) GetCellBitmapFromTexture(TEXTURE_HANDLE hTexture,DWORD index);

GRAPHIC_API(TEXTURE_HANDLE) CreateFillTexture(BITMAP_HANDLE hBitmap);
GRAPHIC_API(TEXTURE_HANDLE) CreateTileTexture(BITMAP_HANDLE hBitmap);
GRAPHIC_API(TEXTURE_HANDLE) CreateNineInOneTexture(BITMAP_HANDLE hBitmap);
GRAPHIC_API(TEXTURE_HANDLE) CreateThreeInOneHTexture(BITMAP_HANDLE hBitmap,bool isCenterStretch);
GRAPHIC_API(TEXTURE_HANDLE) CreateThreeInOneVTexture(BITMAP_HANDLE hBitmap,bool isCenterStretch);
GRAPHIC_API(TEXTURE_HANDLE) CreateFiveInOneHTexture(BITMAP_HANDLE hBitmap);
GRAPHIC_API(TEXTURE_HANDLE) CreateFiveInOneVTexture(BITMAP_HANDLE hBitmap);

//-------------- 一些和Windows有关的OS函数，方便使用，可能废除--------------
GRAPHIC_API(int) PaintBitmap(HDC hDC,BITMAP_HANDLE hBitmap,RECT* pDestRect,RECT* pSrcRect);
GRAPHIC_API(int) AlphaPaintBitmap(HDC hdcDest, int nXOriginDest, int nYOriginDest,
                                       int nWidthDest, int nHeightDest,
                                       BITMAP_HANDLE hSrcBitmap, int nXOriginSrc, int nYOriginSrc,
                                       int nWidthSrc, int nHeightSrc,
                                       BLENDFUNCTION blendFunction);

//从一个已有的DC转换出一个XLBitmap,如果DC里的Bitmap没有合适的alpha通道，可以指定一个alpha通道
//GRAPHIC_API(BITMAP_HANDLE) GetXLBitmapFromDDB(HBITMAP hSrcBitmap,);

#endif

