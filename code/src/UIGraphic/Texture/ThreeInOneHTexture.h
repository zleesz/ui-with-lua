
/********************************************************************
*
*   Description :   ʵ����һ�ֺ���3�������Texture
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
    //�������ڵ�Texture���ԣ�����Texture�Ľ��λͼ
    virtual long SetBitmap(Bitmap* pBitmap);
    virtual Bitmap* UpdateTexture(SIZE newSize);
    virtual Bitmap* GetCellBitmap(DWORD index)
    {
        if(index > 2)
            return NULL;
        return m_cellBitmaps[index];
    }
    
protected:
    //��������:  0 | 1 | 2
    Bitmap* m_cellBitmaps[3];
    Bitmap* m_theOneBitmap;

    //�Ƿ����� 1
    bool m_isCenterStretch;
};

}//namespace Graphic

#endif // ThreeInOneHTexture_h__