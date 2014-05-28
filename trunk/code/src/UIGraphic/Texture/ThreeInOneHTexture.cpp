//
// ThreeInOneHTexture.cpp 
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./ThreeInOneHTexture.h"
#include "../DefaultMixProvider.h"
using namespace Graphic;

ThreeInOneHTexture::ThreeInOneHTexture(Bitmap* pBitmap,bool isCenterStretch)
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

ThreeInOneHTexture::~ThreeInOneHTexture()
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
long ThreeInOneHTexture::SetBitmap(Bitmap* pBitmap)
{
    assert(pBitmap);
    long leftSplitX = -1;
    long rightSplitX = -1;

    BYTE* pNowRead = pBitmap->GetBuffer(0,0);
    size_t pixbyte = pBitmap->GetPixelBytes();

    for(long i=0;i<(long)pBitmap->m_info.Width;++i)
    {
        if(IsSplitPoint(pBitmap->m_info.ColorType,pNowRead))
        {
            leftSplitX = i;
            break;
        }
        pNowRead += pixbyte;
    }
    if(leftSplitX == -1)
        return 1;

    pNowRead = pBitmap->GetBuffer(pBitmap->m_info.Width-1,0);
    for(long i=pBitmap->m_info.Width-1;i>=0;--i)
    {
        if(IsSplitPoint(pBitmap->m_info.ColorType,pNowRead))
        {
            rightSplitX = i;
            break;
        }
        pNowRead -= pixbyte;
    }
    if(rightSplitX == -1)
        return 1;

    if(rightSplitX <= leftSplitX)
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
    clipRect.right = leftSplitX;
    clipRect.bottom = (long)pBitmap->m_info.Height;
    m_cellBitmaps[0] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);

    clipRect.left = leftSplitX+1;
    clipRect.right = rightSplitX;
    m_cellBitmaps[1] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);

    clipRect.left = rightSplitX+1;
    clipRect.right = (long)m_theOneBitmap->m_info.Width;
    m_cellBitmaps[2] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);

    return 0;
}

Bitmap* ThreeInOneHTexture::UpdateTexture(SIZE newSize)
{
    Bitmap* pTextureBitmap = NULL;
    pTextureBitmap = new Bitmap(m_theOneBitmap->m_info.ColorType, newSize.cx,newSize.cy);
    //IMixProvider* pMix = new DefaultMixProvider();
    IMixProvider* pMix = g_pMixProvider;
    pMix->SetAlphaMixSetting(GRAPHIC_BLEND_XXXX_SRCCOPY);
    
    if(m_isCenterStretch)
    {
        long centerWidth = newSize.cx - (long)m_cellBitmaps[0]->m_info.Width - (long)m_cellBitmaps[2]->m_info.Width;

        if(centerWidth > 0)
        {
            Bitmap* pCenter = new Bitmap(m_theOneBitmap->m_info.ColorType,centerWidth,newSize.cy);
            g_pBltProvider->Stretch(*(m_cellBitmaps[1]),*pCenter);
            pMix->Alpha(*pTextureBitmap,(long)m_cellBitmaps[0]->m_info.Width,0,*pCenter,0xff);
            pCenter->Release();
        }

        Bitmap* pLeft = new Bitmap(m_theOneBitmap->m_info.ColorType,(long)m_cellBitmaps[0]->m_info.Width,newSize.cy);
        g_pBltProvider->Stretch(*m_cellBitmaps[0],*pLeft);
        pMix->Alpha(*pTextureBitmap,0,0,*pLeft,0xff);
        pLeft->Release();

        Bitmap* pRight = new Bitmap(m_theOneBitmap->m_info.ColorType,(long)m_cellBitmaps[2]->m_info.Width,newSize.cy);
        g_pBltProvider->Stretch(*m_cellBitmaps[2],*pRight);
        pMix->Alpha(*pTextureBitmap,newSize.cx - (long)m_cellBitmaps[2]->m_info.Width,0,*pRight,0xff);
        pRight->Release();
    }
    else
    {
        long leftWidth = (newSize.cx - (long)m_cellBitmaps[1]->m_info.Width) / 2;
        if(leftWidth > 0)
        {
            Bitmap* pLeft = new Bitmap(m_theOneBitmap->m_info.ColorType,leftWidth,newSize.cy);
            g_pBltProvider->Stretch(*m_cellBitmaps[0],*pLeft);
            pMix->Alpha(*pTextureBitmap,0,0,*pLeft,0xff);
            pLeft->Release();
        }

        pMix->Alpha(*pTextureBitmap,leftWidth,0,*m_cellBitmaps[1],0xff);
        
        long rightWidth = (newSize.cx - (long)m_cellBitmaps[1]->m_info.Width - leftWidth);
        if(rightWidth > 0)
        {
            Bitmap* pRight = new Bitmap(m_theOneBitmap->m_info.ColorType,rightWidth,newSize.cy);
            g_pBltProvider->Stretch(*m_cellBitmaps[2],*pRight);
            pMix->Alpha(*pTextureBitmap,newSize.cx - rightWidth,0,*pRight,0xff);
            pRight->Release();
        }
    }


    //delete pMix;
    return pTextureBitmap;
}
