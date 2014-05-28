
#ifndef Texture_h__
#define Texture_h__

#include "../Bitmap.h"

namespace Graphic
{
    
class BaseTexture
{
public:
    BaseTexture()
    {
        m_ref = 1;
    }

    virtual ~BaseTexture()
    {

    }

public:
    unsigned long AddRef()
    {
        return InterlockedIncrement((LONG*)&m_ref);
    }

    unsigned long Release()
    {
        unsigned long l = InterlockedDecrement((LONG*)&m_ref);
        if (l == 0)
		{
            delete this;
		}
        return l;
    }


public:
    long GetTextureType()
    {
        return m_textureType;
    }


public:
    //根据现在的Texture属性，生成Texture的结果位图
    virtual long SetBitmap(Bitmap* pBitmap) = 0;
    virtual Bitmap* UpdateTexture(SIZE newSize) = 0;
    virtual Bitmap* GetCellBitmap(DWORD index) = 0;

protected:
    bool IsSplitPoint(long colorType,BYTE* pNowRead)
    {
        if(colorType == GRAPHIC_CT_ARGB32)
        {
            if(pNowRead[0] == 0xff && pNowRead[2] == 0xff && pNowRead[1] == 0)
            {
                return true;
            }
        }
        else if(colorType == GRAPHIC_CT_RGB24)
        {
            if(pNowRead[0] == 0xff && pNowRead[2] == 0xff && pNowRead[1] == 0)
            {
                return true;
            }
        }
        return false;
    }
protected:
    unsigned long m_ref;
    long m_textureType;
};

}

#endif // Texture_h__
