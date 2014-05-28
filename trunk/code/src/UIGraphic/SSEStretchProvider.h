/********************************************************************
*
*   Description :   SSE优化的放缩
*
********************************************************************/ 

#ifndef _SSE_STRETCH_PROVIDER_H_
#define _SSE_STRETCH_PROVIDER_H_

#include "./BltProvider.h"

namespace Graphic
{

class SSEStretchProvider : public IBltProvider
{
public:
    SSEStretchProvider(void);
    virtual ~SSEStretchProvider(void);
public:
    virtual Bitmap* Stretch(Bitmap& src, long newWidth, long newHeight);
    virtual void Stretch(Bitmap& src, Bitmap& dest);
protected:
    SSEStretchProvider(const SSEStretchProvider& rhs) {return;}
    SSEStretchProvider& operator=(const SSEStretchProvider& rhs) {return *this;}
};

}//Graphic

#endif