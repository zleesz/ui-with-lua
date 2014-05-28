
#ifndef BitmapList_h__
#define BitmapList_h__

#include <vector>

namespace Graphic
{

class BitmapList
{
public:
    BitmapList(Bitmap* pOrgBitmap);
    ~BitmapList();

public:
    unsigned long AddRef();
    unsigned long Release();

public:
    unsigned long GetBitmapCount();
    Bitmap* GetBitmapByIndex(unsigned long index);

protected:
    void ParseBitmapList();
    bool IsSplitPoint(long colorType,BYTE* pNowRead);

protected:
    unsigned long m_ref;
    Bitmap* m_pOrgBitmap;
    std::vector<Bitmap*> m_allSubBitmaps;
};

}


#endif // BitmapList_h__