//
// FiveInOneVTexture.cpp 
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./FiveInOneVTexture.h"
#include "../DefaultMixProvider.h"

using namespace Graphic;

FiveInOneVTexture::FiveInOneVTexture(Bitmap* pBitmap)
{
    m_theOneBitmap = NULL;

    m_cellBitmaps[0] = NULL;
    m_cellBitmaps[1] = NULL;
    m_cellBitmaps[2] = NULL;
    m_cellBitmaps[3] = NULL;
    m_cellBitmaps[4] = NULL;

    m_ref = 1;
    if(SetBitmap(pBitmap) !=0 )
        assert(0);
}

FiveInOneVTexture::~FiveInOneVTexture()
{
    for(int i=0;i<5;i++)
    {
        if(m_cellBitmaps[i])
            m_cellBitmaps[i]->Release();
    }

    if(m_theOneBitmap)
        m_theOneBitmap->Release();
}

long FiveInOneVTexture::SetBitmap(Bitmap* pBitmap)
{
    assert(pBitmap);

    BYTE* pNowRead = pBitmap->GetBuffer(0,0);
    size_t pixbyte = pBitmap->GetPixelBytes();
    long splitArray[6] = {-1,-1,-1,-1,-1,-1};
    long splitPos = 1;

    for(long i=0;i<(long)pBitmap->m_info.Height;++i)
    {
        pNowRead = pBitmap->GetBuffer(0,i);
        if(IsSplitPoint(pBitmap->m_info.ColorType,pNowRead))
        {
            splitArray[splitPos] = i;
            if(splitPos != 0)
            {
                if(splitArray[splitPos-1] == i-1)
                {
                    return -1;
                }
            }
            splitPos++;
        }
        pNowRead += pixbyte;
    }

    if(splitPos != 5)
        return -1;
    splitArray[splitPos] = pBitmap->m_info.Height;

    for(int i=0;i<5;i++)
    {
        if(m_cellBitmaps[i])
        {
            m_cellBitmaps[i]->Release();
            m_cellBitmaps[i] = NULL;
        }
    }

    m_theOneBitmap = pBitmap;
    m_theOneBitmap->AddRef();

    RECT clipRect;
    clipRect.left = 0;
    clipRect.top = 0;
    clipRect.right = pBitmap->m_info.Width;
    clipRect.bottom = 0;

    for(int i=0;i<5;i++)
    {
        clipRect.top = splitArray[i]+1;
        clipRect.bottom = splitArray[i+1];
        m_cellBitmaps[i] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);
    }

    return 0;
}

Bitmap* FiveInOneVTexture::UpdateTexture(SIZE newSize)
{
    Bitmap* pTextureBitmap = NULL;
    pTextureBitmap = new Bitmap(m_theOneBitmap->m_info.ColorType, newSize.cx,newSize.cy);
    //IMixProvider* pMix = new DefaultMixProvider();
    IMixProvider* pMix = g_pMixProvider;
    pMix->SetAlphaMixSetting(GRAPHIC_BLEND_XXXX_SRCCOPY);
    //0,4优先显示，然后是1，3 最后是2
    long centerDrawHeight = 0;
    pMix->Alpha(*pTextureBitmap,0,0,*m_cellBitmaps[0],0xff);
    if(newSize.cy - (long)m_cellBitmaps[4]->m_info.Height > 0)
    {
        pMix->Alpha(*pTextureBitmap,0,newSize.cy - (long)m_cellBitmaps[4]->m_info.Height,*m_cellBitmaps[4],0xff);
    }
    else
    {
        pMix->Alpha(*pTextureBitmap,0,0,*m_cellBitmaps[4],0xff);
        goto END;
    }

    if(newSize.cy - (long)m_cellBitmaps[0]->m_info.Height - (long)m_cellBitmaps[4]->m_info.Height - (long)m_cellBitmaps[2]->m_info.Height >= 0)
    {
        pMix->Alpha(*pTextureBitmap,0,(long)m_cellBitmaps[0]->m_info.Height + (newSize.cy - (long)m_cellBitmaps[0]->m_info.Height - (long)m_cellBitmaps[4]->m_info.Height - (long)m_cellBitmaps[2]->m_info.Height) / 2,
            *m_cellBitmaps[2],0xff);
        centerDrawHeight = (long)m_cellBitmaps[2]->m_info.Height;
    }

    if(newSize.cy - (long)m_cellBitmaps[0]->m_info.Height - (long)m_cellBitmaps[4]->m_info.Height - centerDrawHeight > 0)
    {
        long topHeight = (newSize.cy - (long)m_cellBitmaps[0]->m_info.Height - (long)m_cellBitmaps[4]->m_info.Height - centerDrawHeight) / 2;
        long bottomHeight = newSize.cy - (long)m_cellBitmaps[0]->m_info.Height - (long)m_cellBitmaps[4]->m_info.Height - centerDrawHeight - topHeight;
        if(topHeight > 0)
        {
            Bitmap* pTop = new Bitmap(m_theOneBitmap->m_info.ColorType,newSize.cx,topHeight);
            g_pBltProvider->Stretch(*m_cellBitmaps[1],*pTop);
            pMix->Alpha(*pTextureBitmap,0,(long)m_cellBitmaps[0]->m_info.Height,*pTop,0xff);
            pTop->Release();
        }
        if(bottomHeight > 0)
        {
            Bitmap* pBottom = new Bitmap(m_theOneBitmap->m_info.ColorType,newSize.cx,bottomHeight);
            g_pBltProvider->Stretch(*m_cellBitmaps[3],*pBottom);
            pMix->Alpha(*pTextureBitmap,0,newSize.cy - (long)m_cellBitmaps[4]->m_info.Height - bottomHeight,*pBottom,0xff);
            pBottom->Release();
        }
    }
END:
    //delete pMix;
    return pTextureBitmap;
}