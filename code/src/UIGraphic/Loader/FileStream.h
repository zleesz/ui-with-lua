/********************************************************************
*
*   Description :   FileStream
*
********************************************************************/ 

#ifndef _FILE_STREAM_H_
#define _FILE_STREAM_H_

#include "./BaseStream.h"
#include <string>
#include <tchar.h>

using namespace std;

namespace Stream
{

class FileStream : public BaseStream
{
public:
    FileStream(const wchar_t* fileName, const TCHAR* mode);
    virtual ~FileStream(void);
public:
    virtual bool Seek(size_t offset, int origin);
    virtual size_t Read(void* buffer, size_t size);
    virtual size_t Write(const void* buffer, size_t size);
    virtual size_t GetPosition();
    virtual size_t SetPosition(size_t pos);
    virtual size_t GetSize();
    virtual size_t SetSize(const size_t size);
protected:
    FILE* m_hFile;
};

}//Stream

#endif
