//
// BaseMemoryStream.cpp 
//
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "./BaseMemoryStream.h"

#include <string.h>

using namespace Stream;

BaseMemoryStream::BaseMemoryStream(void) :
    m_ptr(0),
    m_size(0),
    m_pos(0)  
{
}

BaseMemoryStream::~BaseMemoryStream(void)
{
}

bool BaseMemoryStream::Seek(size_t offset, int origin)
{
    switch(origin)
    {
    case SEEK_CUR:
        m_pos += offset;
    	break;
    case SEEK_END:
        m_size += offset;
        break;
    default:
        m_pos = offset;
        break;
    }
    return true;
}

size_t BaseMemoryStream::Read(void* buffer, size_t size)
{
    size_t ret = 0;
    if (m_pos >= 0 && size >= 0)
    {
        ret = m_size - m_pos;
        if (ret > 0)
        {
            if (ret > size) ret = size;
            memcpy(buffer, (char*)(m_ptr)+m_pos, ret);
            m_pos += ret;
            return ret;
        }
    }
    return ret;
}

size_t BaseMemoryStream::GetPosition()
{
    return m_pos;
}

size_t BaseMemoryStream::SetPosition(size_t pos)
{
    m_pos = pos;
    return m_pos;
}

size_t BaseMemoryStream::GetSize()
{
    return m_size;
}

void BaseMemoryStream::SetPointer(void* ptr, size_t size)
{
    if (ptr)
    {
        m_ptr = ptr;
        m_size = size;
    }
    else
    {
        m_ptr = NULL;
        m_size = 0;
    }
}