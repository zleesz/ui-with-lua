//
// MMXMixProvider.cpp
//
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "./MMXMixProvider.h"

using namespace Graphic;

unsigned long* pAlphaTable;
unsigned long* pBIAS;
unsigned long* pAlpha;

void GenAlphaTable()
{
    pAlphaTable = new unsigned long[256 * 2];
    pAlpha = (unsigned long*)((size_t)pAlphaTable & 0xFFFFFFF8);
    if (pAlpha < pAlphaTable)
    {
        pAlpha = pAlpha + 8;
    }
    unsigned long* p = pAlpha;
    for (int i = 0; i < 256; i++)
    {
        unsigned long l = i + (i << 16);
        *p = l;
        p++;
        *p = l;
        p++;
    }
    pBIAS = pAlpha + 0x80 * 2;
}

void FreeAlphaTable()
{
    delete[] pAlphaTable;
}

class AlphaTable
{
public:
    AlphaTable()
    {
        GenAlphaTable();
    }
    ~AlphaTable()
    {
        FreeAlphaTable();
    }
};

static AlphaTable alphaTable;


MMXMixProvider::MMXMixProvider()
{
    m_setting = GRAPHIC_BLEND_ARGB;
}

MMXMixProvider::~MMXMixProvider()
{
}

//将src的srcTop,srcLeft,srcWidth,srcHeight表示的矩形位置的图混合捣dest的destX,destY位置
void MMXMixProvider::DoARGB32Alpha(Bitmap& dest,long destX,long destY,
                                       Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight)
{
    return DoARGB32Alpha(dest,destX,destY,src,srcTop,srcLeft,srcWidth,srcHeight,255);
}

void MMXMixProvider::DoARGB32Alpha(Bitmap& dest,long destX,long destY,
                           Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight,
                           unsigned char alphaValue)  
{
    if(alphaValue == 0x00)
        return;
    long destStartX=0, destStartY=0;
    long srcStartX=0,srcStartY=0;
    long rowLen=0, lines=0;
    GetRealRect(dest,destX,destY,
        src,srcTop,srcLeft,srcWidth,srcHeight,
        destStartX,destStartY,
        srcStartX,srcStartY,
        rowLen,lines);

 //   int tmpRed,tmpGreen,tmpBlue,tmpAlpah;
 //   int beta;
//    int destRed,destGreen,destBlue,destAlpha;

    const unsigned char* pSrcBuffer =  src.GetBuffer(srcStartX,srcStartY);
    unsigned char* pDestBuffer = NULL;
    long srcScanLine = src.m_info.ScanLineLength;
    long destScanLine = dest.m_info.ScanLineLength;

    pDestBuffer = dest.GetBuffer(destStartX,destStartY);
    
    for(int i=0;i<lines;i++)
    {
        const DWORD* pNowSrcBuffer = (const DWORD*)pSrcBuffer;
        DWORD* pNowDestBuffer = (DWORD*)pDestBuffer;
        
        __asm
        {
            mov eax, rowLen
            test eax, eax
            js LABEL3
            mov esi, pNowSrcBuffer
            mov edi, pNowDestBuffer
            mov ecx, eax
            movzx edx, dword ptr [alphaValue]
LABEL1:
            movzx ebx, edx
            mov eax, [esi]
            pxor mm0, mm0
            movd mm1, eax
            shl ebx, 3
            movd mm2, [edi]
            punpcklbw mm1, mm0
            punpcklbw mm2, mm0
            add ebx, pAlpha
            psubw mm1, mm2
            pmullw mm1, [ebx]
            psllw mm2, 8
            mov ebx, pBIAS
            paddw mm2, [ebx]
            paddw mm1, mm2
            psrlw mm1, 8
            packuswb mm1, mm0
            movd eax, mm1
            mov [edi], eax
            add esi, 4
            add edi, 4
            dec ecx
            jnz LABEL1
            emms
LABEL3:
        }

        pSrcBuffer += srcScanLine;
        pDestBuffer += destScanLine;
    }

    return ;
}

//OK
void MMXMixProvider::DoXRGB32Alpha(Bitmap& dest,long destX,long destY,
                           Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight,
                           unsigned char alphaValue) 
{

    if(alphaValue == 0xff)
        return DoRGB32Overlap(dest,destX,destY,src,srcTop,srcLeft,srcWidth,srcHeight,alphaValue);
    if(alphaValue == 0x00)
        return;

    long destStartX=0, destStartY=0;
    long srcStartX=0,srcStartY=0;
    long rowLen=0, lines=0;
    GetRealRect(dest,destX,destY,
        src,srcTop,srcLeft,srcWidth,srcHeight,
        destStartX,destStartY,
        srcStartX,srcStartY,
        rowLen,lines);
    

//    int srcRed,srcGreen,srcBlue,srcAlpha;
//    int beta;
 //   int destRed,destGreen,destBlue,destAlpha;

    const unsigned char* pSrcBuffer =  src.GetBuffer(srcStartX,srcStartY);
    unsigned char* pDestBuffer = NULL;
    long srcScanLine = src.m_info.ScanLineLength;
    long destScanLine = dest.m_info.ScanLineLength;
    pDestBuffer = dest.GetBuffer(destStartX,destStartY);
   
    for(int i=0;i<lines;i++)
    {
        const DWORD* pNowSrcBuffer = (const DWORD*)pSrcBuffer;
        DWORD* pNowDestBuffer = (DWORD*)pDestBuffer;
        __asm
        {
            mov eax, rowLen
            test eax, eax
            js LABEL3
            mov esi, pNowSrcBuffer
            mov edi, pNowDestBuffer
            mov ecx, eax
            movzx edx, dword ptr [alphaValue]
LABEL1:
            movzx ebx, edx
            mov eax, [esi]
            pxor mm0, mm0
            movd mm1, eax
            shl ebx, 3
            movd mm2, [edi]
            punpcklbw mm1, mm0
            punpcklbw mm2, mm0
            add ebx, pAlpha
            psubw mm1, mm2
            pmullw mm1, [ebx]
            psllw mm2, 8
            mov ebx, pBIAS
            paddw mm2, [ebx]
            paddw mm1, mm2
            psrlw mm1, 8
            packuswb mm1, mm0
            movd eax, mm1
            or eax, 0xFF000000
            mov [edi], eax
            add esi, 4
            add edi, 4
            dec ecx
            jnz LABEL1
            emms
LABEL3:
        }

        pSrcBuffer += srcScanLine;
        pDestBuffer += destScanLine;
    }

    return ;
}

//OK
void MMXMixProvider::DoRGB32Overlap(Bitmap& dest,long destX,long destY,
                       Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight,
                       unsigned char alphaValue)
{
    long destStartX=0, destStartY=0;
    long srcStartX=0,srcStartY=0;
    long rowLen=0, lines=0;
    GetRealRect(dest,destX,destY,
        src,srcTop,srcLeft,srcWidth,srcHeight,
        destStartX,destStartY,
        srcStartX,srcStartY,
        rowLen,lines);

    const unsigned char* pSrcBuffer =  src.GetBuffer(srcStartX,srcStartY);
    unsigned char* pDestBuffer = NULL;
    long srcScanLine = src.m_info.ScanLineLength;
    long destScanLine = dest.m_info.ScanLineLength;
    pDestBuffer = dest.GetBuffer(destStartX,destStartY);

    for(int i=0;i<lines;i++)
    {
        memcpy((void*)pDestBuffer,(const void*)pSrcBuffer,rowLen * 4);
        pSrcBuffer += srcScanLine;
        pDestBuffer += destScanLine;
    }

    return ;
}
