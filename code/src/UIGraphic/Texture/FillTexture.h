
/********************************************************************
*								
*   Description :   实现了一种单图拉伸填充满的Texture
*
********************************************************************/
#ifndef FillTexture_h__
#define FillTexture_h__

#include "./Texture.h"

namespace Graphic
{

class FillTexture : BaseTexture
{
public:
    FillTexture(Bitmap* pBitmap);
    virtual ~FillTexture();

public:
    //根据现在的Texture属性，生成Texture的结果位图
    virtual long SetBitmap(Bitmap* pBitmap);
    virtual Bitmap* UpdateTexture(SIZE newSize);
    virtual Bitmap* GetCellBitmap(DWORD index)
    {
        if(index == 0)
            return m_theBitmap;

        return NULL;
    }
protected:
    Bitmap* m_theBitmap;
};

}//namespace Graphic

#endif // FillTexture_h__