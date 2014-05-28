
/********************************************************************
*		
*   Description :   实现了一种9宫合一的Texture,九宫通过RGB(FF,0,FF)在一张图上划分
*
********************************************************************/  

#ifndef NineInOneTexture_h__
#define NineInOneTexture_h__

#include "./Texture.h"

namespace Graphic
{

class NineInOneTexture : BaseTexture
{
public:
    NineInOneTexture(Bitmap* theBitmap);
    virtual ~NineInOneTexture();

public:
    //设置九合一位图，如果pBitmap是一张有效的9宫图，则返回Ture
    bool SetTextureBitmap(Bitmap* pBitmap);
   
    virtual long SetBitmap(Bitmap* pBitmap);
    virtual Bitmap* UpdateTexture(SIZE newSize);    
    virtual Bitmap* GetCellBitmap(DWORD index)
    {
        if(index > 8)
            return NULL;

        return m_cellBitmaps[index];
    }

protected:
    //9宫 顺序为
    // 0  1  2
    // 3  4  5
    // 6  7  8
    Bitmap* m_cellBitmaps[9];

    bool m_isReady;
    Bitmap* m_theOneBitmap;
};

}


#endif // NineInOneTexture_h__
