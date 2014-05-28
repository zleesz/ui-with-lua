//
// DefaultBltProvider.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "./DefaultBltProvider.h"

using namespace Graphic;

DefaultBltProvider::DefaultBltProvider()
{
    m_lastSrcWidth = -1;
    m_lastDestWidth = -1;
    m_lastDestHeight = -1;
    m_lastsrcHeight = -1;
    m_posXTable = NULL;
    m_posYTable = NULL;
}

DefaultBltProvider::~DefaultBltProvider()
{
    if(m_posXTable != NULL)
    {
        delete[] m_posXTable;
    }

    if(m_posYTable != NULL)
    {
        delete[] m_posYTable;
    }
}

void DefaultBltProvider::BuildXTable(long srcWidth,long destWidth)
{
    if(m_posXTable != NULL)
    {
        if(srcWidth == m_lastSrcWidth && destWidth == m_lastDestWidth)
            return;
        else
        {
            delete [] m_posXTable;
            m_posXTable = NULL;
        }
    }
    
    m_posXTable = new long[destWidth];
    if(srcWidth == destWidth)
    {
        for(int i=0;i<destWidth;++i)
        {
            m_posXTable[i] = i;
        }       
    }
    else
    {
        for(int i=0;i<destWidth;++i)
        {
            m_posXTable[i] = (long)((i * srcWidth + 0.5) / destWidth);
        }
    }


    m_lastSrcWidth = srcWidth;
    m_lastDestWidth = destWidth;
    
}

void DefaultBltProvider::BuildYTable(long srcHeight,long destHeight)
{
    if(m_posYTable != NULL)
    {
        if(srcHeight == m_lastsrcHeight && destHeight == m_lastDestHeight)
            return;
        else
        {
            delete [] m_posYTable;
            m_posYTable = NULL;
        }
    }

    m_posYTable = new long[destHeight];
    if(srcHeight == destHeight)
    {
        for(int i=0;i<destHeight;++i)
        {
            m_posYTable[i] = i;
        }
    }
    else
    {
        for(int i=0;i<destHeight;++i)
        {
            m_posYTable[i] = (long)((i * srcHeight + 0.5) / destHeight);
        }
    }

    m_lastsrcHeight = srcHeight;
    m_lastDestHeight = destHeight;
}


//À­Éì
Bitmap* DefaultBltProvider::Stretch(Bitmap& src,long newWidth,long newHeight)
{
    assert (src.m_info.ColorType == GRAPHIC_CT_ARGB32);
    BuildXTable(src.m_info.Width,newWidth);
    BuildYTable(src.m_info.Height,newHeight);

    Bitmap* pResult = new Bitmap(src.m_info.ColorType,newWidth,newHeight);
    const unsigned char* pSrc = src.GetBuffer(0,0);
    unsigned char* pDest = pResult->GetBuffer(0,0);
    long scanLineSrc = src.m_info.ScanLineLength;
    long scanLineDest = pResult->m_info.ScanLineLength;
    DWORD* pNowWrite = NULL;
    for(int i=0;i<newHeight;++i)
    {
        const DWORD* pNowReadLine = (const DWORD*)(pSrc + m_posYTable[i]*scanLineSrc);
        DWORD* pNowWrite = (DWORD*) pDest;
        for(int j=0;j<newWidth;++j)
        {
            pNowWrite[j] = pNowReadLine[ m_posXTable[j] ];      
        }
        pDest+=scanLineDest;
    }

    return pResult;
}

void DefaultBltProvider::Stretch(Bitmap& src,Bitmap& dest)
{
    assert (src.m_info.ColorType == GRAPHIC_CT_ARGB32);
    assert (dest.m_info.ColorType == GRAPHIC_CT_ARGB32);

    long newWidth = dest.m_info.Width;
    long newHeight = dest.m_info.Height;

    BuildXTable(src.m_info.Width,newWidth);
    BuildYTable(src.m_info.Height,newHeight);

    Bitmap* pResult = &dest;
    const unsigned char* pSrc = src.GetBuffer(0,0);
    unsigned char* pDest = pResult->GetBuffer(0,0);
    long scanLineSrc = src.m_info.ScanLineLength;
    long scanLineDest = pResult->m_info.ScanLineLength;
    DWORD* pNowWrite = NULL;
    for(int i=0;i<newHeight;++i)
    {
        const DWORD* pNowReadLine = (const DWORD*)(pSrc + m_posYTable[i]*scanLineSrc);
        DWORD* pNowWrite = (DWORD*) pDest;
        for(int j=0;j<newWidth;++j)
        {
            pNowWrite[j] = pNowReadLine[ m_posXTable[j] ];      
        }
        pDest+=scanLineDest;
    }
}

