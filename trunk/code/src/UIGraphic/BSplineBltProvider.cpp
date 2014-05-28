//
// BSplineBltProvider.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./BSplineBltProvider.h"
#include <math.h>

using namespace Graphic;

BSplineBltProvider::BSplineBltProvider()
{

}

BSplineBltProvider::~BSplineBltProvider()
{

}

//À­Éì
Bitmap* BSplineBltProvider::Stretch(Bitmap& src,long newWidth,long newHeight)
{
    assert (src.m_info.ColorType == GRAPHIC_CT_ARGB32);
    if (newHeight == 0)
        newHeight = 2;
    if (newWidth == 0)
        newWidth = 2;

    Bitmap* pResult = new Bitmap(src.m_info.ColorType,newWidth,newHeight);
    float xScale, yScale;
    xScale = (float)src.m_info.Width  / (float)newWidth;
    yScale = (float)src.m_info.Height / (float)newHeight;

    const unsigned char* pSrc = src.GetBuffer(0,0);
    unsigned char* pDest = pResult->GetBuffer(0,0);
    long scanLineSrc = src.m_info.ScanLineLength;
    long scanLineDest = pResult->m_info.ScanLineLength;

    float f_x, f_y, a, b, rr, gg, bb, r1, r2;
    int   i_x, i_y, xx, yy;
    RGBQUAD rgb;

    const unsigned char* iSrc;
    unsigned char* iDst;

    for(long y=0; y<newHeight; y++)
    {
        f_y = (float) y * yScale - 0.5f;
        i_y = (int) floor(f_y);
        a   = f_y - (float)floor(f_y);
        for(long x=0; x<newWidth; x++)
        {
            f_x = (float) x * xScale - 0.5f;
            i_x = (int) floor(f_x);
            b   = f_x - (float)floor(f_x);

            rr = gg = bb = 0.0f;
            for(int m=-1; m<3; m++)
            {
                r1 = BSpline((float) m - a);
                yy = i_y+m;
                if (yy<0) yy=0;
                if (yy>=(long)src.m_info.Height) yy = (long)src.m_info.Height-1;
                for(int n=-1; n<3; n++)
                {
                    r2 = r1 * BSpline(b - (float)n);
                    xx = i_x+n;
                    if (xx<0) xx=0;
                    if (xx>=(long)src.m_info.Width) xx=(long)src.m_info.Width-1;

                    iSrc  = pSrc + yy*scanLineSrc + xx*4;
                    rgb.rgbBlue = *iSrc++;
                    rgb.rgbGreen= *iSrc++;
                    rgb.rgbRed  = *iSrc;
                    

                    rr += rgb.rgbRed * r2;
                    gg += rgb.rgbGreen * r2;
                    bb += rgb.rgbBlue * r2;
                }
            }


            iDst = pDest + y*scanLineDest + x*4;
            *iDst++ = (BYTE)bb;
            *iDst++ = (BYTE)gg;
            *iDst++   = (BYTE)rr;
            *iDst = 0;
            


        }
    }

    return pResult;
}

void BSplineBltProvider::Stretch(Bitmap& src,Bitmap& dest)
{
    assert (src.m_info.ColorType == GRAPHIC_CT_ARGB32);
    long newWidth = dest.m_info.Width;
    long newHeight = dest.m_info.Height;
    if (newHeight == 0)
        newHeight = 2;
    if (newWidth == 0)
        newWidth = 2;
    float xScale, yScale;
    xScale = (float)src.m_info.Width  / (float)newWidth;
    yScale = (float)src.m_info.Height / (float)newHeight;

    const unsigned char* pSrc = src.GetBuffer(0,0);
    unsigned char* pDest = dest.GetBuffer(0,0);
    long scanLineSrc = src.m_info.ScanLineLength;
    long scanLineDest = dest.m_info.ScanLineLength;

    float f_x, f_y, a, b, rr, gg, bb, r1, r2;
    int   i_x, i_y, xx, yy;
    RGBQUAD rgb;

    const unsigned char* iSrc;
    unsigned char* iDst;

    for(long y=0; y<newHeight; y++)
    {
        f_y = (float) y * yScale - 0.5f;
        i_y = (int) floor(f_y);
        a   = f_y - (float)floor(f_y);
        for(long x=0; x<newWidth; x++)
        {
            f_x = (float) x * xScale - 0.5f;
            i_x = (int) floor(f_x);
            b   = f_x - (float)floor(f_x);

            rr = gg = bb = 0.0f;
            for(int m=-1; m<3; m++)
            {
                r1 = BSpline((float) m - a);
                yy = i_y+m;
                if (yy<0) yy=0;
                if (yy>=(long)src.m_info.Height) yy = (long)src.m_info.Height-1;
                for(int n=-1; n<3; n++)
                {
                    r2 = r1 * BSpline(b - (float)n);
                    xx = i_x+n;
                    if (xx<0) xx=0;
                    if (xx>=(long)src.m_info.Width) xx=(long)src.m_info.Width-1;

                    iSrc  = pSrc + yy*scanLineSrc + xx*4;
                    rgb.rgbBlue = *iSrc++;
                    rgb.rgbGreen= *iSrc++;
                    rgb.rgbRed  = *iSrc;


                    rr += rgb.rgbRed * r2;
                    gg += rgb.rgbGreen * r2;
                    bb += rgb.rgbBlue * r2;
                }
            }


            iDst = pDest + y*scanLineDest + x*4;
            *iDst++ = (BYTE)bb;
            *iDst++ = (BYTE)gg;
            *iDst++   = (BYTE)rr;
            *iDst = 0;



        }
    }

}

float BSplineBltProvider::BSpline( const float x ) 
{
    if (x>2.0f) return 0.0f;
    float a, b, c, d;
    float xm1 = x - 1.0f;
    float xp1 = x + 1.0f;
    float xp2 = x + 2.0f;

    if ((xp2) <= 0.0f) a = 0.0f; else a = xp2*xp2*xp2;
    if ((xp1) <= 0.0f) b = 0.0f; else b = xp1*xp1*xp1;
    if (x <= 0) c = 0.0f; else c = x*x*x;
    if ((xm1) <= 0.0f) d = 0.0f; else d = xm1*xm1*xm1;

    return (0.16666666666666666667f * (a - (4.0f * b) + (6.0f * c) - (4.0f * d)));
}