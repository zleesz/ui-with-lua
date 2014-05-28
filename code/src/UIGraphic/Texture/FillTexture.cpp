//
// FillTexture.cpp 
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./FillTexture.h"

using namespace Graphic;

FillTexture::FillTexture(Bitmap* pBitmap)
{
    assert(pBitmap);
    m_ref = 1;
    m_textureType = XLTEXTURE_TYPE_FILL;
    m_theBitmap = NULL;
    m_theBitmap = pBitmap;
    m_theBitmap->AddRef();
}

FillTexture::~FillTexture()
{
    if(m_theBitmap)
        m_theBitmap->Release();
}


    //根据现在的Texture属性，生成Texture的结果位图
long FillTexture::SetBitmap(Bitmap* pBitmap) 
{
    if(m_theBitmap)
    {
        m_theBitmap->Release();
    }
    m_theBitmap = pBitmap;
    m_theBitmap->AddRef();
    return 0;
}

Bitmap* FillTexture::UpdateTexture(SIZE newSize) 
{
    return g_pBltProvider->Stretch(*m_theBitmap,newSize.cx,newSize.cy);
}
