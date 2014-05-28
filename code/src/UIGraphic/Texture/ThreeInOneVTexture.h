
/********************************************************************
*		
*   Description :   ʵ����һ������3�������Texture
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
    //��������:  
    //0
    //--
    //1
    //--
    //2
    Bitmap* m_cellBitmaps[3];
    Bitmap* m_theOneBitmap;

    //�Ƿ�����1
    bool m_isCenterStretch;
};

}//namespace Graphic

#endif // ThreeInOneVTexture_h__