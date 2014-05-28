/********************************************************************
*
*   Description :   BaseStream
*
********************************************************************/ 

#ifndef _BASE_STREAM_H_
#define _BASE_STREAM_H_

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

namespace Stream
{

class BaseStream
{
public:
    BaseStream(void);
    virtual ~BaseStream(void);
public:
    virtual bool Seek(size_t offset, int origin) = 0;
    virtual size_t Read(void* buffer, size_t size) = 0;
    virtual size_t Write(const void* buffer, size_t size) = 0;
    virtual size_t GetPosition() = 0;
    virtual size_t SetPosition(size_t pos) = 0;
    virtual size_t GetSize();
    virtual size_t SetSize(const size_t size);
public:
    bool  Valid;
};

}//Stream

#endif