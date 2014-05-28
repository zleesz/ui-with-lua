//
//  Bitmap.cpp
//
////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include "./Bitmap.h"
#include <stdio.h>
#include <assert.h>

using namespace Graphic;

Bitmap::Bitmap(long colorType,unsigned long width,unsigned long height ) : m_ref(1),
                                                       m_isOwnerBuffer(true),
                                                       m_buffer(NULL)
{
    assert(width> 0);
    assert(height > 0);

    m_info.Height = height;
    m_info.Width = width;
    m_info.ColorType = colorType;

	m_bPretreatment = false;

    switch(colorType)
    {
    case GRAPHIC_CT_ARGB32:
        m_pixelBytes = 4;
        m_buffer = new unsigned char[width*height*m_pixelBytes];
        m_info.ScanLineLength = width * m_pixelBytes ;
        break;
    case GRAPHIC_CT_RGB24:
        m_pixelBytes = 3;
        m_buffer = new unsigned char[width*height*m_pixelBytes];
        m_info.ScanLineLength = width * m_pixelBytes ;
        break;
    default:
        assert(0);
    }

}

Bitmap::Bitmap(long colorType,unsigned long width,unsigned long height,long scanLineSize,unsigned char* pBuffer ) : m_ref(1),
                                                                                                m_buffer(pBuffer),
                                                                                                m_isOwnerBuffer(false)
{
    assert(width > 0);
    assert(height > 0);
    assert(pBuffer);

    m_info.Height = height;
    m_info.Width = width;
    m_info.ColorType = colorType;
    m_info.ScanLineLength = scanLineSize;

    switch(colorType)
    {
    case GRAPHIC_CT_ARGB32:
        m_pixelBytes = 4;
        break;
    case GRAPHIC_CT_RGB24:
        m_pixelBytes = 3;
    default:
        assert(0);
    }
}

Bitmap::~Bitmap(void)
{
    if(m_isOwnerBuffer)
    {
        if(NULL != m_buffer)
        {
            delete[] m_buffer;
            m_buffer = NULL;
        }
    }
}

unsigned long Bitmap::AddRef()
{
	return InterlockedIncrement((LONG*)&m_ref);
}

unsigned long Bitmap::Release()
{
	long l = InterlockedDecrement((LONG*)&m_ref);
	if (l == 0)
    {
		delete this;
    }
    if(l < 0)
    {
        //引用计数错误！
        assert(0);

    }
	return l;
}

Bitmap* Bitmap::ChangeColorType(long colorType)
{
    assert(m_info.ColorType == GRAPHIC_CT_RGB24);
    assert(colorType == GRAPHIC_CT_ARGB32);

    Bitmap* pResult = new Bitmap(GRAPHIC_CT_ARGB32,m_info.Width,m_info.Height);
    const unsigned char* pRead = GetBuffer(0,0);
    unsigned char* pWrite = pResult->GetBuffer(0,0);
    for(int i=0;i<(long)m_info.Height;++i)
    {
        const unsigned char* pNowRead = pRead;
        unsigned char* pNowWrite = pWrite;
        for(int j=0;j<(long)m_info.Width;++j)
        {
            pNowWrite[0] = pNowRead[0];
            pNowWrite[1] = pNowRead[1];
            pNowWrite[2] = pNowRead[2];
            pNowWrite[3] = 0xFF;
            pNowWrite += 4;
            pNowRead += 3;
        }
        pRead += m_info.ScanLineLength;
        pWrite += pResult->m_info.ScanLineLength;
    }

    return pResult;
}


//得到每个Pixel有多少字节
unsigned long Bitmap::GetPixelBytes() const
{
    return m_pixelBytes;
}

unsigned char* Bitmap::GetBuffer( unsigned long x,unsigned long y ) const
{
    assert(x < m_info.Width);
    assert(y < m_info.Height);
    assert(m_buffer);
    
    return m_buffer + (y * m_info.ScanLineLength  + x * m_pixelBytes);
}

bool Bitmap::IsOwnerBuffer()
{
    return m_isOwnerBuffer;
}


Bitmap* Bitmap::ClipSubBindBitmap(RECT* pClibRect)
{
    if(pClibRect->bottom > (long)m_info.Height || pClibRect->right > (long)m_info.Width)
    {
        assert(0);
        return NULL;
    }

    unsigned char* pBuffer = GetBuffer(pClibRect->left,pClibRect->top);
    Bitmap* pResultBitmap = new Bitmap(m_info.ColorType,pClibRect->right - pClibRect->left,
                                       pClibRect->bottom - pClibRect->top,m_info.ScanLineLength,pBuffer);
    return pResultBitmap;
}

Bitmap* Bitmap::CloneBitmap()
{
    Bitmap* pResult = new Bitmap(m_info.ColorType,m_info.Width,m_info.Height);
    size_t copyBytePerLine = pResult->GetPixelBytes() * pResult->m_info.Width;

    unsigned char* pRead = GetBuffer(0,0);
    unsigned char* pWrite = pResult->GetBuffer(0,0);
    for(unsigned long i = 0;i<pResult->m_info.Height;++i)
    {
        memcpy(pWrite,pRead,copyBytePerLine);
        pRead += m_info.ScanLineLength;
        pWrite += pResult->m_info.ScanLineLength;
    }
    
    return pResult;
}

bool Bitmap::PretreatmentPNG32()
{
    assert(m_info.ColorType == GRAPHIC_CT_ARGB32);
	if (m_bPretreatment == true)
	{
		return false;
	}

	long nScanLine = m_info.ScanLineLength;

	unsigned char* pBuffer = m_buffer;
	for (int i=0; i<(long)m_info.Width; i++)
	{
		DWORD * pNowBuffer = (DWORD*)pBuffer;
		for (int j=0; j<(long)m_info.Height; j++)
		{
			unsigned char * pSrcByte = (unsigned char*)(pNowBuffer + j);
			//unsigned char * pDestByte = (unsigned char*)(pNowBuffer + j);
			
			//ALPHA混合预处理
			pSrcByte[0] = pSrcByte[0] * pSrcByte[3] / 255;
			pSrcByte[1] = pSrcByte[1] * pSrcByte[3] / 255;
			pSrcByte[2] = pSrcByte[2] * pSrcByte[3] / 255;

			//pDestByte[0] = pSrcByte[0];
			//pDestByte[1] = pSrcByte[1];
			//pDestByte[2] = pSrcByte[2];
			//pDestByte[3] = pSrcByte[3];
		}
		pBuffer += nScanLine;
	}

	m_bPretreatment = true;
	return true;
}

void Bitmap::SetPretreatment(bool bPretreatment)
{
	m_bPretreatment = bPretreatment;
}

bool Bitmap::GetPretreatment()
{
	return m_bPretreatment;
}







