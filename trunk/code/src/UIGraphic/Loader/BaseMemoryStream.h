/********************************************************************
*
*   Description :   BaseMemoryStream
*
********************************************************************/ 

#ifndef _BASE_MEMORY_STREAM_H_
#define _BASE_MEMORY_STREAM_H_

#include "./BaseStream.h"

namespace Stream
{

class BaseMemoryStream : public BaseStream
{
public:
    BaseMemoryStream(void);
    virtual ~BaseMemoryStream(void);
public:
    virtual bool Seek(size_t offset, int origin);
    virtual size_t Read(void* buffer, size_t size);
    virtual size_t GetPosition();
    virtual size_t SetPosition(size_t pos);
    virtual size_t GetSize();
protected:
    void SetPointer(void* ptr, size_t size);
protected:
    void* m_ptr;
    size_t m_size;
    size_t m_pos;
};

}//Stream

#endif