//
// NineInOneTexture.cpp 
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./NineInOneTexture.h"
#include "../DefaultMixProvider.h"

using namespace Graphic;

NineInOneTexture::NineInOneTexture(Bitmap* theBitmap) : m_isReady(false)
{
    for(int i=0;i<9;i++)
    {
        m_cellBitmaps[i] = NULL;
    }
    m_theOneBitmap = NULL;
    m_isReady = SetTextureBitmap(theBitmap);
    m_textureType = XLTEXTURE_TYPE_NINE_IN_ONE;
    m_ref = 1;
}

NineInOneTexture::~NineInOneTexture()
{
    for(int i=0;i<9;i++)
    {
        if(m_cellBitmaps[i])
        {
            m_cellBitmaps[i]->Release();
            m_cellBitmaps[i] = NULL;
        }
    }

    if(m_theOneBitmap)
    {
        m_theOneBitmap->Release();
        m_theOneBitmap = NULL;
    }
}


long NineInOneTexture::SetBitmap(Bitmap* pBitmap)
{
    if(SetTextureBitmap(pBitmap))
    {
        return 0;
    }
    return 1;
}

//设置九合一位图，如果pBitmap是一张有效的9宫图，则返回Ture
bool NineInOneTexture::SetTextureBitmap(Bitmap* pBitmap)
{
    //TODO 可以优化成更好的9宫切分
    assert(pBitmap);
    long leftSplitX = -1;
    long rightSplitX = -1;
    long topSplitY = -1;
    long bottomSplitY = -1;

    long leftCenterSplitX = -1;
    long rightCenterSplitX = -1;
    long topCenterSplitY = -1;
    long bottomCenterSplitY = -1;

    size_t pixbyte = pBitmap->GetPixelBytes();
    BYTE* pNowRead = pBitmap->GetBuffer(0,0);

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
        return false;

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
    if(rightSplitX <= leftSplitX)
        return false;

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

    pNowRead = pBitmap->GetBuffer(0,topSplitY+1);
    for(long i=0;i<(long)pBitmap->m_info.Width;++i)
    {
        if(IsSplitPoint(pBitmap->m_info.ColorType,pNowRead))
        {
            leftCenterSplitX = i;
            break;
        }
        pNowRead += pixbyte;
    }
    if(leftCenterSplitX == -1)
        return false;

    pNowRead = pBitmap->GetBuffer(pBitmap->m_info.Width-1,topSplitY+1);
    for(long i=pBitmap->m_info.Width-1;i>=0;--i)
    {
        if(IsSplitPoint(pBitmap->m_info.ColorType,pNowRead))
        {
            rightCenterSplitX = i;
            break;
        }
        pNowRead -= pixbyte;
    }
    if(rightCenterSplitX <= leftCenterSplitX)
        return false;

    for(long i=0;i<(long)pBitmap->m_info.Height;++i)
    {
        pNowRead = pBitmap->GetBuffer(leftSplitX+1,i);
        if(IsSplitPoint(pBitmap->m_info.ColorType,pNowRead))
        {
            topCenterSplitY = i;
            break;
        }
    }
    if(topCenterSplitY == -1)
        return false;

    for(long i=pBitmap->m_info.Height-1;i>=0;--i)
    {
        pNowRead = pBitmap->GetBuffer(leftSplitX+1,i);
        if(IsSplitPoint(pBitmap->m_info.ColorType,pNowRead))
        {
            bottomCenterSplitY = i;
            break;
        }
    }
    if(bottomCenterSplitY <= topCenterSplitY)
        return false;

    for(int i=0;i<9;i++)
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
    clipRect.bottom = topSplitY;
    m_cellBitmaps[0] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);
    
    clipRect.left = leftSplitX+1;
    clipRect.right = rightSplitX;
    clipRect.bottom = topCenterSplitY;
    m_cellBitmaps[1] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);

    clipRect.left = rightSplitX+1;
    clipRect.right = m_theOneBitmap->m_info.Width;
    clipRect.bottom = topSplitY;
    m_cellBitmaps[2] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);

    clipRect.top = topSplitY+1;
    clipRect.bottom = bottomSplitY;
    clipRect.left = 0;
    clipRect.right = leftCenterSplitX;
    m_cellBitmaps[3] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);

    clipRect.left = leftSplitX + 1;
    clipRect.right = rightSplitX;
    m_cellBitmaps[4] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);

    clipRect.left = rightCenterSplitX + 1;
    clipRect.right = m_theOneBitmap->m_info.Width;
    m_cellBitmaps[5] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);

    clipRect.top = bottomSplitY + 1;
    clipRect.bottom = m_theOneBitmap->m_info.Height;
    clipRect.left = 0;
    clipRect.right = leftSplitX;
    m_cellBitmaps[6] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);

    clipRect.top = bottomCenterSplitY + 1;
    clipRect.left = leftSplitX + 1;
    clipRect.right = rightSplitX;
    m_cellBitmaps[7] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);

    clipRect.top = bottomSplitY + 1;
    clipRect.left = rightSplitX + 1;
    clipRect.right = m_theOneBitmap->m_info.Width;
    m_cellBitmaps[8] = m_theOneBitmap->ClipSubBindBitmap(&clipRect);

    return true;
}

Bitmap* NineInOneTexture::UpdateTexture(SIZE newSize)
{
    Bitmap* pTextureBitmap = NULL;
    

    long centerWidth = newSize.cx - (long)m_cellBitmaps[0]->m_info.Width - (long)m_cellBitmaps[2]->m_info.Width;
    long centerHeight = newSize.cy - (long)m_cellBitmaps[0]->m_info.Height - (long)m_cellBitmaps[6]->m_info.Height;
    if(centerWidth < 0 || centerHeight < 0)
        return NULL;

    pTextureBitmap = new Bitmap(m_theOneBitmap->m_info.ColorType, newSize.cx,newSize.cy);
    //IMixProvider* pMix = new DefaultMixProvider();
    IMixProvider* pMix = g_pMixProvider;
    pMix->SetAlphaMixSetting(GRAPHIC_BLEND_XXXX_SRCCOPY);
    //先画4
    centerWidth = newSize.cx - (long)m_cellBitmaps[3]->m_info.Width - (long)m_cellBitmaps[5]->m_info.Width;
    centerHeight = newSize.cy - (long)m_cellBitmaps[1]->m_info.Height - (long)m_cellBitmaps[7]->m_info.Height;
    Bitmap* pCenter = new Bitmap(m_theOneBitmap->m_info.ColorType,centerWidth,centerHeight);
    g_pBltProvider->Stretch(*m_cellBitmaps[4],*pCenter);
    pMix->Alpha(*pTextureBitmap,(long)m_cellBitmaps[3]->m_info.Width,(long)m_cellBitmaps[1]->m_info.Height,*pCenter,0xff);
    pCenter->Release();
    pCenter = NULL;
    //画4个不用拉伸的 0，2，6，8
    pMix->Alpha(*pTextureBitmap,0,0,*m_cellBitmaps[0],0xff);
    pMix->Alpha(*pTextureBitmap,(long)newSize.cx-m_cellBitmaps[2]->m_info.Width,0,*m_cellBitmaps[2],0xff);
    pMix->Alpha(*pTextureBitmap,0,newSize.cy-(long)m_cellBitmaps[6]->m_info.Height,*m_cellBitmaps[6],0xff);
    pMix->Alpha(*pTextureBitmap,newSize.cx-(long)m_cellBitmaps[8]->m_info.Width,newSize.cy-(long)m_cellBitmaps[6]->m_info.Height,*m_cellBitmaps[8],0xff);
    //画拉伸的1，7
    centerWidth = newSize.cx - (long)m_cellBitmaps[0]->m_info.Width - (long)m_cellBitmaps[2]->m_info.Width;
    Bitmap* pWidthBitmap = new Bitmap(m_theOneBitmap->m_info.ColorType,centerWidth,(long)m_cellBitmaps[1]->m_info.Height);
    g_pBltProvider->Stretch(*m_cellBitmaps[1],*pWidthBitmap);
    pMix->Alpha(*pTextureBitmap,(long)m_cellBitmaps[0]->m_info.Width,0,*pWidthBitmap,0xff);
    pWidthBitmap->Release();
    pWidthBitmap = NULL;
    pWidthBitmap = new Bitmap(m_theOneBitmap->m_info.ColorType,centerWidth,(long)m_cellBitmaps[7]->m_info.Height);
    g_pBltProvider->Stretch(*m_cellBitmaps[7],*pWidthBitmap);
    pMix->Alpha(*pTextureBitmap,(long)m_cellBitmaps[6]->m_info.Width,newSize.cy - (long)m_cellBitmaps[7]->m_info.Height,*pWidthBitmap,0xff);
    pWidthBitmap->Release();
    pWidthBitmap = NULL;
    //画拉伸的3,5
    centerHeight = newSize.cy - (long)m_cellBitmaps[0]->m_info.Height - (long)m_cellBitmaps[6]->m_info.Height;
    Bitmap* pHeightBitmap = new Bitmap(m_theOneBitmap->m_info.ColorType,(long)m_cellBitmaps[3]->m_info.Width,centerHeight);
    g_pBltProvider->Stretch(*m_cellBitmaps[3],*pHeightBitmap);
    pMix->Alpha(*pTextureBitmap,0,(long)m_cellBitmaps[0]->m_info.Height,*pHeightBitmap,0xff);
    pHeightBitmap->Release();
    pHeightBitmap = NULL;
    pHeightBitmap = new Bitmap(m_theOneBitmap->m_info.ColorType,(long)m_cellBitmaps[5]->m_info.Width,centerHeight);
    g_pBltProvider->Stretch(*m_cellBitmaps[5],*pHeightBitmap);
    pMix->Alpha(*pTextureBitmap,newSize.cx - (long)m_cellBitmaps[5]->m_info.Width,(long)m_cellBitmaps[2]->m_info.Height,*pHeightBitmap,0xff);
    pHeightBitmap->Release();
    pHeightBitmap = NULL;

	//Add by zx
	//delete pMix;
	pMix = NULL;

    return pTextureBitmap;
}
