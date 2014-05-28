
/********************************************************************
*		
*   Description :   实现了一种纵向3宫拉伸的Texture
*
********************************************************************/
#ifndef ThreeInOneVTexture_h__
#define ThreeInOneVTexture_h__

#include "./Texture.h"

namespace Graphic
{

class ThreeInOneVTexture : BaseTexture
{
public:
    ThreeInOneVTexture(Bitmap* pBitmap,bool isCenterStretch=true);
    virtual ~ThreeInOneVTexture();

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
    //纵向三宫:  
    //0
    //--
    //1
    //--
    //2
    Bitmap* m_cellBitmaps[3];
    Bitmap* m_theOneBitmap;

    //是否拉伸1
    bool m_isCenterStretch;
};

}//namespace Graphic

#endif // ThreeInOneVTexture_h__