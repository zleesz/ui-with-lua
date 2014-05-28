//
// ThreeInOneVTexture.cpp 
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./ThreeInOneVTexture.h"
#include "../DefaultMixProvider.h"

using namespace Graphic;

ThreeInOneVTexture::ThreeInOneVTexture(Bitmap* pBitmap,bool isCenterStretch)
{
    m_theOneBitmap = NULL;
    m_cellBitmaps[0] = NULL;
    m_cellBitmaps[1] = NULL;
    m_cellBitmaps[2] = NULL;
    m_ref = 1;
    if(SetBitmap(pBitmap) !=0 )
        assert(0);
    m_isCenterStretch = isCenterStretch;
}

ThreeInOneVTexture::~ThreeInOneVTexture()
{
    if(m_theOneBitmap)
        m_theOneBitmap->Release();
    if(m_cellBitmaps[0])
        m_cellBitmaps[0]->Release();
    if(m_cellBitmaps[1])
        m_cellBitmaps[1]->Release();
    if(m_cellBitmaps[2])
        m_cellBitmaps[2]->Release();
}


//根据现在的Texture属性，生成Texture的结果位图
long ThreeInOneVTexture::SetBitmap(Bitmap* pBitmap)
{
    assert(pBitmap);
    long topSplitY = -1;
    long bottomSplitY = -1;

    BYTE* pNowRead = pBitmap->GetBuffer(0,0);
    size_t pixbyte = pBitmap->GetPixelBytes();

    for(long i=0;i<(long)pBitmap->m_info.Height;++i)
    {
        pNowRead = pBitmap->GetBuffer(0,i);
        if(IsSplitPoint(pBitmap->m_info.ColorType,pNowRead))
        {
            topSplitY = i;
            break;
        }
    }
    if(topSplitY == -1)
        return false;

    for(long i=pBitmap->m_info.Height-1;i>=0;--i)
    {
        pNowRead = pBitmap->GetBuffer(0,i);
        if(IsSplitPoint(pBitmap->m_info.ColorType,pNowRead))
        {
            bottomSplitY = i;
            break;
        }
    }
    if(bottomSplitY <= topSplitY)
        return false;

    for(int i=0;i<3;i++)
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
    clipRect.right = (long)m_theOneBitmap->m_info.Width;
    clipRect.bottom = topSplitY;
    m_cellBitmaps[0] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);

    clipRect.top = topSplitY+1;
    clipRect.bottom = bottomSplitY;
    m_cellBitmaps[1] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);

    clipRect.top = bottomSplitY+1;
    clipRect.bottom = (long)m_theOneBitmap->m_info.Height;
    m_cellBitmaps[2] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);

    return 0;
}

Bitmap* ThreeInOneVTexture::UpdateTexture(SIZE newSize)
{
    Bitmap* pTextureBitmap = NULL;
    pTextureBitmap = new Bitmap(m_theOneBitmap->m_info.ColorType, newSize.cx,newSize.cy);
    //IMixProvider* pMix = new DefaultMixProvider();
    IMixProvider* pMix = g_pMixProvider;
    pMix->SetAlphaMixSetting(GRAPHIC_BLEND_XXXX_SRCCOPY);

    if(m_isCenterStretch)
    {
        long centerHeight = newSize.cy - m_cellBitmaps[0]->m_info.Height - m_cellBitmaps[2]->m_info.Height;
        if(centerHeight > 0)
        {
            //画1
            Bitmap* pCenter = new Bitmap(m_theOneBitmap->m_info.ColorType,newSize.cx,centerHeight);
            g_pBltProvider->Stretch(*(m_cellBitmaps[1]),*pCenter);
            pMix->Alpha(*pTextureBitmap,0,m_cellBitmaps[0]->m_info.Height,*pCenter,0xff);
            pCenter->Release();
        }

        //画0
        Bitmap* pTop = new Bitmap(m_theOneBitmap->m_info.ColorType,newSize.cx,m_cellBitmaps[0]->m_info.Height);
        g_pBltProvider->Stretch(*m_cellBitmaps[0],*pTop);
        pMix->Alpha(*pTextureBitmap,0,0,*pTop,0xff);
        pTop->Release();

        //画2
        Bitmap* pBottom = new Bitmap(m_theOneBitmap->m_info.ColorType,newSize.cx,m_cellBitmaps[2]->m_info.Height);
        g_pBltProvider->Stretch(*m_cellBitmaps[2],*pBottom);
        pMix->Alpha(*pTextureBitmap,0,newSize.cy-m_cellBitmaps[2]->m_info.Height,*pBottom,0xff);
        pBottom->Release();
    }
    else
    {
        long topHeight = (newSize.cy - m_cellBitmaps[1]->m_info.Height) / 2;
        if(topHeight > 0)
        {
            Bitmap* pTop = new Bitmap(m_theOneBitmap->m_info.ColorType,newSize.cx,topHeight);
            g_pBltProvider->Stretch(*m_cellBitmaps[0],*pTop);
            pMix->Alpha(*pTextureBitmap,0,0,*pTop,0xff);
            pTop->Release();
        }

        pMix->Alpha(*pTextureBitmap,0,topHeight,*m_cellBitmaps[1],0xff);

        long bottomHeight = (newSize.cy - m_cellBitmaps[1]->m_info.Height - topHeight);
        if(bottomHeight > 0)
        {
            Bitmap* pBottom = new Bitmap(m_theOneBitmap->m_info.ColorType,newSize.cx,bottomHeight);
            g_pBltProvider->Stretch(*m_cellBitmaps[2],*pBottom);
            pMix->Alpha(*pTextureBitmap,0,newSize.cy - bottomHeight,*pBottom,0xff);
            pBottom->Release();
        }
    }
    //delete pMix;
    return pTextureBitmap;
}
