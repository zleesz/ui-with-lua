
#ifndef FiveInOneVTexture_h__
#define FiveInOneVTexture_h__

#include "./Texture.h"

namespace Graphic
{

class FiveInOneVTexture : BaseTexture
{
public:
    FiveInOneVTexture(Bitmap* pBitmap);
    virtual ~FiveInOneVTexture();

public:
    //�������ڵ�Texture���ԣ�����Texture�Ľ��λͼ
    virtual long SetBitmap(Bitmap* pBitmap);
    virtual Bitmap* UpdateTexture(SIZE newSize);
    virtual Bitmap* GetCellBitmap(DWORD index)
    {
        if(index > 4)
            return NULL;
        return m_cellBitmaps[index];
    }

protected:
    //�����幬: 
    // 0 
    //--- 
    // 1
    //---
    // 2
    //---
    // 3
    //---
    // 4
    //1,3����
    
    Bitmap* m_cellBitmaps[5];
    Bitmap* m_theOneBitmap;
};

}//namespace Graphic

#endif // FiveInOneVTexture_h__