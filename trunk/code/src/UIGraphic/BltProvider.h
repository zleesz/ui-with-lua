
/********************************************************************
*				
*   Description :  λͼ�����㷨��ע��"Blt"ȡ��������BitBlt����
*
********************************************************************/  

#ifndef _BLT_PROVIDER_H_
#define _BLT_PROVIDER_H_

#include "./Bitmap.h"

namespace Graphic
{

class IBltProvider
{
public:
    //����,����һ����λͼ
    virtual Bitmap* Stretch(Bitmap& src,long newWidth,long newHeight) = 0;
    //���쵽destλͼ��
    virtual void Stretch(Bitmap& src,Bitmap& dest) = 0;
};

}//namespace Graphic

#endif //_BLT_PROVIDER_H_