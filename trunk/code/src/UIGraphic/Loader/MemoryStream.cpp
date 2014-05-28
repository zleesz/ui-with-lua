//
// MemoryStream.cpp 
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "./MemoryStream.h"

#include <string>

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

using namespace Stream;

#define MEM_DELTA 0x2000

MemoryStream::MemoryStream(void)
{
}

MemoryStream::MemoryStream(void* ptr, size_t size)
{
    SetPointer(ptr, size);
}

MemoryStream::~MemoryStream(void)
{
    Clear();
}

size_t MemoryStream::Write(const void* buffer, size_t size)
{
    size_t ret = 0;
    if (m_pos >= 0 && size >= 0)
    {
        size_t pos = m_pos + size;
        if (pos > 0)
        {
            if (pos > m_size)
            {
                if (pos > m_allocSize)
                {
                    SetAllocSize(pos);
                }
                m_size = pos;
            }
            memcpy((char*)(m_ptr)+m_pos, buffer, size);
            m_pos += pos;
            ret = size;
            return ret;
        }
    }
    return ret;
}

size_t MemoryStream::SetSize(const size_t size)
{
    size_t posOld = m_pos;
    SetAllocSize(size);
    m_size = size;
    if (posOld > size) Seek(0, SEEK_END);
    return m_size;
}

void MemoryStream::Clear() 
{
    SetAllocSize(0);
    m_size = 0;
    m_pos = 0;
}

void* MemoryStream::Realloc(const size_t size)
{
    size_t allocSize = size;
    if (size > 0 && size != m_size)
    {
        allocSize = (size + (MEM_DELTA - 1)) & ~(MEM_DELTA - 1);
    }
    if (allocSize != m_allocSize)
    {
        if (allocSize == 0)
        {
      //      delete[] m_ptr;
            m_ptr = 0;
            return 0;
        }
        else
        {
            void* ret = 0;
            if (m_allocSize == 0)
            {
                ret = new char[allocSize];
            }
            else
            {
                ret = new char[allocSize];
                memcpy(ret, m_ptr, min(allocSize, m_allocSize));
                delete[] m_ptr;
            }
            if (ret)
            {
                return ret;
            }
        }
    }
    else
    {
        return m_ptr;
    }
    return 0;
}

size_t MemoryStream::SetAllocSize(const size_t size)
{
    SetPointer(Realloc(size), m_size);
    m_allocSize = size;
    return m_allocSize;
}