
/********************************************************************
*			
*   Description :   实现了一种单图拉平铺充满的Texture
*
********************************************************************/
#ifndef TileTexture_h__
#define TileTexture_h__

#include "./Texture.h"

namespace Graphic
{

class TileTexture : BaseTexture
{
public:
    TileTexture(Bitmap* pBitmap);
    virtual ~TileTexture();

public:
    virtual long SetBitmap(Bitmap* pBitmap);
    virtual Bitmap* UpdateTexture(SIZE newSize);
    virtual Bitmap* GetCellBitmap(DWORD index)
    {
        if(index == 0)
            return m_cellBitmap;

        return NULL;
    }
protected:
    Bitmap* m_cellBitmap;
};

}//namespace Graphic

#endif // TileTexture_h__