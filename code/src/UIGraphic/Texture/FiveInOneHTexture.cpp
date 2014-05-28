//
// FiveInOneHTexture.cpp 
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./FiveInOneHTexture.h"
#include "../DefaultMixProvider.h"

using namespace Graphic;

FiveInOneHTexture::FiveInOneHTexture(Bitmap* pBitmap)
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

FiveInOneHTexture::~FiveInOneHTexture()
{
    for(int i=0;i<5;i++)
    {
        if(m_cellBitmaps[i])
            m_cellBitmaps[i]->Release();
    }

    if(m_theOneBitmap)
        m_theOneBitmap->Release();
}

long FiveInOneHTexture::SetBitmap(Bitmap* pBitmap)
{
    assert(pBitmap);

    BYTE* pNowRead = pBitmap->GetBuffer(0,0);
    size_t pixbyte = pBitmap->GetPixelBytes();
    long splitArray[6] = {-1,-1,-1,-1,-1,-1};
    long splitPos = 1;

    for(long i=0;i<(long)pBitmap->m_info.Width;++i)
    {
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
    splitArray[splitPos] = pBitmap->m_info.Width;
    
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
    clipRect.right = 0;
    clipRect.bottom = pBitmap->m_info.Height;

    for(int i=0;i<5;i++)
    {
        clipRect.left = splitArray[i]+1;
        clipRect.right = splitArray[i+1];
        m_cellBitmaps[i] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);
    }

    return 0;
}

Bitmap* FiveInOneHTexture::UpdateTexture(SIZE newSize)
{
    Bitmap* pTextureBitmap = NULL;
    pTextureBitmap = new Bitmap(m_theOneBitmap->m_info.ColorType, newSize.cx,newSize.cy);
    //IMixProvider* pMix = new DefaultMixProvider();
    IMixProvider* pMix = g_pMixProvider;
    pMix->SetAlphaMixSetting(GRAPHIC_BLEND_XXXX_SRCCOPY);
    //0,4优先显示，然后是2，最后是1，3
    pMix->Alpha(*pTextureBitmap,0,0,*m_cellBitmaps[0],0xff);
    if(newSize.cx - (long)m_cellBitmaps[4]->m_info.Width > 0)
    {
        pMix->Alpha(*pTextureBitmap,newSize.cx - (long)m_cellBitmaps[4]->m_info.Width,0,*m_cellBitmaps[4],0xff);
    }
    else
    {
        pMix->Alpha(*pTextureBitmap,0,0,*m_cellBitmaps[4],0xff);
        goto END;
    }
    
    long centerWidth = 0;
    if(newSize.cx - (long)m_cellBitmaps[0]->m_info.Width - (long)m_cellBitmaps[4]->m_info.Width - (long)m_cellBitmaps[2]->m_info.Width >= 0)
    {
        pMix->Alpha(*pTextureBitmap,(long)m_cellBitmaps[0]->m_info.Width + (newSize.cx - (long)m_cellBitmaps[0]->m_info.Width - (long)m_cellBitmaps[4]->m_info.Width - (long)m_cellBitmaps[2]->m_info.Width) / 2,0,
              *m_cellBitmaps[2],0xff);
        centerWidth = (long)m_cellBitmaps[2]->m_info.Width;
    }
    
    if(newSize.cx - (long)m_cellBitmaps[0]->m_info.Width - (long)m_cellBitmaps[4]->m_info.Width - centerWidth > 0)
    {
        long leftWidth = (newSize.cx - (long)m_cellBitmaps[0]->m_info.Width - (long)m_cellBitmaps[4]->m_info.Width - centerWidth) / 2;
        long rightWidth = newSize.cx - (long)m_cellBitmaps[0]->m_info.Width - (long)m_cellBitmaps[4]->m_info.Width - centerWidth - leftWidth;
        if(leftWidth > 0)
        {
            Bitmap* pLeft = new Bitmap(m_theOneBitmap->m_info.ColorType,leftWidth,newSize.cy);
            g_pBltProvider->Stretch(*m_cellBitmaps[1],*pLeft);
            pMix->Alpha(*pTextureBitmap,(long)m_cellBitmaps[0]->m_info.Width,0,*pLeft,0xff);
            pLeft->Release();
        }
        if(rightWidth > 0)
        {
            Bitmap* pRight = new Bitmap(m_theOneBitmap->m_info.ColorType,rightWidth,newSize.cy);
            g_pBltProvider->Stretch(*m_cellBitmaps[3],*pRight);
            pMix->Alpha(*pTextureBitmap,newSize.cx - (long)m_cellBitmaps[4]->m_info.Width - rightWidth,0,*pRight,0xff);
            pRight->Release();
        }
    }
END:
    //delete pMix;
    return pTextureBitmap;
}
