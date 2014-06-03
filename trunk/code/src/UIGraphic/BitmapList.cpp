//
// BitmapList.cpp
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./BitmapList.h"

using namespace std;
using namespace Graphic;

BitmapList::BitmapList(Bitmap* pOrgBitmap)
{
    assert(pOrgBitmap);
    m_pOrgBitmap = pOrgBitmap;
    ParseBitmapList();
    m_ref = 1;
}

BitmapList::~BitmapList()
{
    if(m_pOrgBitmap)
    {
        m_pOrgBitmap->Release();
        m_pOrgBitmap = NULL;

        for(size_t i=0;i<m_allSubBitmaps.size();++i)
        {
            m_allSubBitmaps[i]->Release();
        }
    }
}

unsigned long BitmapList::AddRef()
{
	return InterlockedIncrement((LONG*)&m_ref);
}

unsigned long BitmapList::Release()
{
    unsigned long l = InterlockedDecrement((LONG*)&m_ref);
    if (l == 0)
        delete this;
    return l;
}

unsigned long BitmapList::GetBitmapCount()
{
    return (unsigned long)m_allSubBitmaps.size();            
}

Bitmap* BitmapList::GetBitmapByIndex(unsigned long index)
{
	//加个保护，NULL指针访问崩溃比越界容易解...
	if (index > m_allSubBitmaps.size())
	{
		assert(false);
		return NULL;
	}
    m_allSubBitmaps[index-1]->AddRef();
    return m_allSubBitmaps[index-1];
}

bool BitmapList::IsSplitPoint(long colorType,BYTE* pNowRead)
{
    if(colorType == GRAPHIC_CT_ARGB32)
    {
        if(pNowRead[0] == 0xff && pNowRead[1] == 0x00 && pNowRead[2] == 0xff)
        {
            return true;
        }
    }
    else if(colorType == GRAPHIC_CT_RGB24)
    {
        if(pNowRead[0] == 0xff && pNowRead[1] == 0x00 && pNowRead[2] == 0xff)
        {
            return true;
        }
    }
    return false;
}


void BitmapList::ParseBitmapList()
{

    BYTE* pNowRead = m_pOrgBitmap->GetBuffer(0,0);
    size_t pixbyte = m_pOrgBitmap->GetPixelBytes();

    vector<long> splitPoints;
	long i = 0;
    for(i=0;i<(long)m_pOrgBitmap->m_info.Width;++i)
    {
        if(IsSplitPoint(m_pOrgBitmap->m_info.ColorType,pNowRead))
        {
            splitPoints.push_back(i);
        }
        pNowRead += pixbyte;
    }
    splitPoints.push_back(i);
    long stratX = 0;
    for(int i=0;i<(long)splitPoints.size();++i)
    {
        BYTE* pStart = m_pOrgBitmap->GetBuffer(stratX,0);        
        long newWidth = splitPoints[i] - stratX;
        Bitmap* pSubBitmap = new Bitmap(m_pOrgBitmap->m_info.ColorType,newWidth,m_pOrgBitmap->m_info.Height,m_pOrgBitmap->m_info.ScanLineLength,pStart);
        m_allSubBitmaps.push_back(pSubBitmap);
        stratX = splitPoints[i]+1;
    }
    return;
}