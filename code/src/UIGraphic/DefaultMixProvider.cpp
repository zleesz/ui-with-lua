//
//  DefaultMixProvider.cpp
//
//  无优化的Bitmap Mix 算法实现  
//
/////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <stdlib.h>
#include <assert.h>

#include "./DefaultMixProvider.h"

using namespace Graphic;

DefaultMixProvider::DefaultMixProvider()
{
    m_setting = GRAPHIC_BLEND_ARGB;
}

DefaultMixProvider::~DefaultMixProvider()
{
}


//将src的srcTop,srcLeft,srcWidth,srcHeight表示的矩形位置的图混合捣dest的destX,destY位置
void DefaultMixProvider::DoARGB32Alpha(Bitmap& dest,long destX,long destY,
                                       Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight)
{
    return DoARGB32Alpha(dest,destX,destY,src,srcTop,srcLeft,srcWidth,srcHeight,255);
}
                                     

//将src的srcTop,srcLeft,srcWidth,srcHeight表示的矩形位置的图混合捣dest的destX,destY位置
void DefaultMixProvider::DoARGB32Alpha(Bitmap& dest,long destX,long destY,
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

    //直接套用公式  ARGB的混合,由于A通道的存在,所以

    /*
    //这三步已经在Bitmap的预处理里做了
    Src.Red = Src.Red   * Src.Alpha / 255;
    Src.Green = Src.Green * Src.Alpha / 255;
    Src.Blue  = Src.Blue  * Src.Alpha / 255;
    
    //真正的操作从这里开始
    Tmp.Red  = Src.Red * Alpha / 255
    Tmp.Green  = Src.Green * Alpha / 255
    Tmp.Blue  = Src.Blue * Alpha / 255
    Tmp.Alpha = Src.Alpha * Alpha / 255;

    Beta      = 255 – Tmp.Alpha;

    Dst.Red   = Tmp.Red   + Round((Beta * Dst.Red  )/255);
    Dst.Green = Tmp.Green + Round((Beta * Dst.Green)/255);
    Dst.Blue  = Tmp.Blue  + Round((Beta * Dst.Blue )/255);
    Dst.Alpha = Tmp.Alpha + Round((Beta * Dst.Alpha)/255);

    */
    int tmpRed,tmpGreen,tmpBlue,tmpAlpah;
    int beta;
    int destRed,destGreen,destBlue,destAlpha;

    const unsigned char* pSrcBuffer =  src.GetBuffer(srcStartX,srcStartY);
    unsigned char* pDestBuffer = NULL;
    long srcScanLine = src.m_info.ScanLineLength;
    long destScanLine = dest.m_info.ScanLineLength;

    pDestBuffer = dest.GetBuffer(destStartX,destStartY);
    
    for(int i=0;i<lines;i++)
    {
        const DWORD* pNowSrcBuffer = (const DWORD*)pSrcBuffer;
        DWORD* pNowDestBuffer = (DWORD*)pDestBuffer;
        for(int j=0;j<rowLen;++j)
        {
            unsigned char* pSrcByte = (unsigned char*)(pNowSrcBuffer +j);
            unsigned char* pDestByte = (unsigned char*)(pNowDestBuffer +j);

            tmpRed = pSrcByte[2] * alphaValue / 255;
            tmpBlue = pSrcByte[0] * alphaValue / 255;
            tmpGreen = pSrcByte[1] * alphaValue / 255;
            tmpAlpah = pSrcByte[3] * alphaValue / 255;
            beta = 255-tmpAlpah;
            destRed = tmpRed + (beta * pDestByte[2]) / 255;
            destGreen = tmpGreen + (beta * pDestByte[1]) / 255;
            destBlue = tmpBlue + (beta * pDestByte[0]) / 255;
            destAlpha = tmpAlpah + (beta * pDestByte[3]) / 255;
            destRed = destRed<255?destRed:255;
            destGreen = destGreen<255?destGreen:255;
            destBlue = destBlue<255?destBlue:255;
            destAlpha = destAlpha<255?destAlpha:255;

            unsigned char* pWrite = (unsigned char*) (pNowDestBuffer+j);
            pWrite[0] = destBlue;
            pWrite[1] = destGreen;
            pWrite[2] = destRed;
            pWrite[3] = destAlpha;
        }

        pSrcBuffer += srcScanLine;
        pDestBuffer += destScanLine;
    }

    return ;
}

//将src的srcTop,srcLeft,srcWidth,srcHeight表示的矩形位置的图混合捣dest的destX,destY位置
void DefaultMixProvider::DoXRGB32Alpha(Bitmap& dest,long destX,long destY,
                                       Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight,
                                       unsigned char alphaValue)
{
    if(alphaValue == 0)
        return;

    if(alphaValue == 0xff)
    {
        return DoRGB32Overlap(dest,destX,destY,src,srcTop,srcLeft,srcWidth,srcHeight,alphaValue);
    }
    //计算src,dest  Bitmap的相交区域,进行裁减
    long destStartX=0, destStartY=0;
    long srcStartX=0,srcStartY=0;
    long rowLen=0, lines=0;
    GetRealRect(dest,destX,destY,
        src,srcTop,srcLeft,srcWidth,srcHeight,
        destStartX,destStartY,
        srcStartX,srcStartY,
        rowLen,lines);

    /*
    公式:
    Dst.Red   = Src.Red * Alpha   + Round((Beta * Dst.Red  )/255);
    Dst.Green = Src.Green * Alpha + Round((Beta * Dst.Green)/255);
    Dst.Blue  = Src.Blue * Alpha + Round((Beta * Dst.Blue )/255);
    Dst.Alpha = 0xFF
    
    */

    int srcRed,srcGreen,srcBlue,srcAlpha;
    int beta;
    int destRed,destGreen,destBlue,destAlpha;

    const unsigned char* pSrcBuffer =  src.GetBuffer(srcStartX,srcStartY);
    unsigned char* pDestBuffer = NULL;
    long srcScanLine = src.m_info.ScanLineLength;
    long destScanLine = dest.m_info.ScanLineLength;
    pDestBuffer = dest.GetBuffer(destStartX,destStartY);
   
    for(int i=0;i<lines;i++)
    {
        const DWORD* pNowSrcBuffer = (const DWORD*)pSrcBuffer;
        DWORD* pNowDestBuffer = (DWORD*)pDestBuffer;
        for(int j=0;j<rowLen;++j)
        {
            srcBlue = pNowSrcBuffer[j] & 0xff;
            srcGreen = (pNowSrcBuffer[j]>>8) & 0xff;
            srcRed = (pNowSrcBuffer[j]>>16) & 0xff;
            srcAlpha = (pNowSrcBuffer[j]>>24) & 0xff;
         
            beta = 255-alphaValue;
            destBlue = pNowDestBuffer[j] & 0xff;
            destGreen = (pNowDestBuffer[j]>>8) & 0xff;
            destRed = (pNowDestBuffer[j]>>16) & 0xff;
            destAlpha = (pNowDestBuffer[j]>>25) & 0xff;

            destRed = (srcRed * alphaValue + (beta * destRed)) / 255;
            destGreen = (srcGreen * alphaValue + (beta * destGreen)) / 255;
            destBlue = (srcBlue * alphaValue  + (beta * destBlue)) / 255;
            

            destRed = destRed<255?destRed:255;
            destGreen = destGreen<255?destGreen:255;
            destBlue = destBlue<255?destBlue:255;
            destAlpha = 0xFF;
            unsigned char* pWrite = (unsigned char*) (pNowDestBuffer+j);
            pWrite[0] = destBlue;
            pWrite[1] = destGreen;
            pWrite[2] = destRed;
            pWrite[3] = destAlpha;
        }

        pSrcBuffer += srcScanLine;
        pDestBuffer += destScanLine;
    }

    return ;
}

void DefaultMixProvider::DoRGB32Overlap(Bitmap& dest,long destX,long destY,
                       Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight,
                       unsigned char alphaValue)
{
    //计算src,dest  Bitmap的相交区域,进行裁减
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
