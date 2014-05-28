//
// TileTexture.cpp 
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./TileTexture.h"
#include "../DefaultMixProvider.h"

using namespace Graphic;

TileTexture::TileTexture(Bitmap* pBitmap)
{
    assert(pBitmap);
    m_ref = 1;
    m_textureType = XLTEXTURE_TYPE_FILL;
    m_cellBitmap = NULL;
    m_cellBitmap = pBitmap;
    m_cellBitmap->AddRef();
}

TileTexture::~TileTexture()
{
    if(m_cellBitmap)
        m_cellBitmap->Release();
}


//根据现在的Texture属性，生成Texture的结果位图
long TileTexture::SetBitmap(Bitmap* pBitmap) 
{
    if(m_cellBitmap)
    {
        m_cellBitmap->Release();
    }
    m_cellBitmap = pBitmap;
    m_cellBitmap->AddRef();
    return 0;
}

Bitmap* TileTexture::UpdateTexture(SIZE newSize) 
{
    if(newSize.cx < (long)m_cellBitmap->m_info.Width &&
       newSize.cy < (long)m_cellBitmap->m_info.Height)
    {
        RECT clipRect;
        clipRect.left = 0;
        clipRect.top = 0;
        clipRect.right = newSize.cx;
        clipRect.bottom = newSize.cy;
        return m_cellBitmap->ClipSubBindBitmap(&clipRect);
    }
    
    Bitmap* pTextureBitmap = new Bitmap(m_cellBitmap->m_info.ColorType, newSize.cx,newSize.cy);
    //IMixProvider* pMix = new DefaultMixProvider();
    IMixProvider* pMix = g_pMixProvider;
    pMix->SetAlphaMixSetting(GRAPHIC_BLEND_XXXX_SRCCOPY);

    int YStep = newSize.cy / m_cellBitmap->m_info.Height;
    if(newSize.cy % (long)m_cellBitmap->m_info.Height != 0)
        YStep ++;

    int XStep = newSize.cx / m_cellBitmap->m_info.Width;
    if(newSize.cx % (long)m_cellBitmap->m_info.Width != 0)
        XStep ++;
    
    long startX = 0;
    long startY = 0;
    for(int j=0;j<YStep;++j)
    {
        startX = 0;
        for(int i=0;i<XStep;++i)
        {
            pMix->Alpha(*pTextureBitmap,startX,startY,*m_cellBitmap,0xff);
            startX += (long)m_cellBitmap->m_info.Width;
        }
        startY += (long)m_cellBitmap->m_info.Height;
    }
    //delete pMix;
    return pTextureBitmap;
}
