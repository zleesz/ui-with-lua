/********************************************************************
*
*   Description :   MemoryStream
*
********************************************************************/ 

#ifndef _MEMORY_STREAM_H_
#define _MEMORY_STREAM_H_

#include "./BaseMemoryStream.h"

namespace Stream
{

class MemoryStream : public BaseMemoryStream
{
public:
    MemoryStream(void);
    MemoryStream(void* ptr, size_t size);
    virtual ~MemoryStream(void);
public:
    virtual size_t Write(const void* buffer, size_t size);
    virtual size_t SetSize(const size_t size);
    void Clear();
protected:
    void* Realloc(const size_t size);
    size_t SetAllocSize(const size_t size);
protected:
    size_t m_allocSize;
};

}//Stream

#endif