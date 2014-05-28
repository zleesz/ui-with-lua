
/********************************************************************
*
*   Description :   默认的算法实现．
*
********************************************************************/  

#ifndef _DEFAULT_BLT_PROVIDER_H_
#define _DEFAULT_BLT_PROVIDER_H_

#include "./BltProvider.h"

namespace Graphic
{

class DefaultBltProvider : public IBltProvider
{
public:
    DefaultBltProvider();
    virtual ~DefaultBltProvider();

public:
    //拉伸
    virtual Bitmap* Stretch(Bitmap& src,long newWidth,long newHeight);
    virtual void Stretch(Bitmap& src,Bitmap& dest);

protected:
    void BuildXTable(long srcWidth,long destWidth);
    void BuildYTable(long srcHeight,long destHeight);
    long m_lastSrcWidth;
    long m_lastDestWidth;
    long m_lastsrcHeight;
    long m_lastDestHeight;

    long* m_posXTable;
    long* m_posYTable;

protected:
    DefaultBltProvider(const DefaultBltProvider& rhs) {return;}
    DefaultBltProvider& operator=(const DefaultBltProvider& rhs) {return *this;}

};


}//namespace Graphic

#endif //_DEFAULT_BLT_PROVIDER_H_