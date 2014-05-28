
#ifndef FiveInOneHTexture_h__
#define FiveInOneHTexture_h__

#include "./Texture.h"

namespace Graphic
{

class FiveInOneHTexture : BaseTexture
{
public:
    FiveInOneHTexture(Bitmap* pBitmap);
    virtual ~FiveInOneHTexture();

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
    //�����幬:  0 | 1 | 2 | 3 | 4
    //1,3����
    Bitmap* m_cellBitmaps[5];
    Bitmap* m_theOneBitmap;
};

}//namespace Graphic

#endif // FiveInOneHTexture_h__