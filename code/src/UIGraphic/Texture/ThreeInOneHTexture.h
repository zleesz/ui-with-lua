
/********************************************************************
*
*   Description :   实现了一种横向3宫拉伸的Texture
*
********************************************************************/
#ifndef ThreeInOneHTexture_h__
#define ThreeInOneHTexture_h__

#include "./Texture.h"

namespace Graphic
{

class ThreeInOneHTexture : BaseTexture
{
public:
    ThreeInOneHTexture(Bitmap* pBitmap,bool isCenterStretch=true);
    virtual ~ThreeInOneHTexture();

public:
    //根据现在的Texture属性，生成Texture的结果位图
    virtual long SetBitmap(Bitmap* pBitmap);
    virtual Bitmap* UpdateTexture(SIZE newSize);
    virtual Bitmap* GetCellBitmap(DWORD index)
    {
        if(index > 2)
            return NULL;
        return m_cellBitmaps[index];
    }
    
protected:
    //横向三宫:  0 | 1 | 2
    Bitmap* m_cellBitmaps[3];
    Bitmap* m_theOneBitmap;

    //是否拉伸 1
    bool m_isCenterStretch;
};

}//namespace Graphic

#endif // ThreeInOneHTexture_h__