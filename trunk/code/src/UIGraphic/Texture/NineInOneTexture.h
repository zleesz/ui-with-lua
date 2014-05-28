
/********************************************************************
*		
*   Description :   ʵ����һ��9����һ��Texture,�Ź�ͨ��RGB(FF,0,FF)��һ��ͼ�ϻ���
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
    //���þź�һλͼ�����pBitmap��һ����Ч��9��ͼ���򷵻�Ture
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
    //9�� ˳��Ϊ
    // 0  1  2
    // 3  4  5
    // 6  7  8
    Bitmap* m_cellBitmaps[9];

    bool m_isReady;
    Bitmap* m_theOneBitmap;
};

}


#endif // NineInOneTexture_h__
