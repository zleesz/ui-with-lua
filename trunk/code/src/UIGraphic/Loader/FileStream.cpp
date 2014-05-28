//
// FileStream.cpp 
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "./FileStream.h"

#include <io.h>

using namespace Stream;

FileStream::FileStream(const wchar_t* fileName, const TCHAR* mode) :
    m_hFile(NULL)
{
    if ((m_hFile = _tfopen(fileName, mode)) == NULL)
    {
        Valid = false;
    }
    else
    {
        Valid = true;
    }
}

FileStream::~FileStream(void)
{
    if (m_hFile)
    {
        fclose(m_hFile);
    }
}

bool FileStream::Seek(size_t offset, int origin)
{
    if (!m_hFile) return false;
    return (bool)(fseek(m_hFile, (long)offset, origin) == 0);
}

size_t FileStream::Read(void* buffer, size_t size)
{
    if (!m_hFile) return -1;
    return fread(buffer, 1, size, m_hFile);
}

size_t FileStream::Write(const void* buffer, size_t size)
{
    if (!m_hFile) return -1;
    return fwrite(buffer, 1, size, m_hFile);
}

size_t FileStream::GetPosition() 
{
    if (!m_hFile) return -1;
    return ftell(m_hFile);
}

size_t FileStream::SetPosition(size_t pos)
{
    if (Seek(pos, SEEK_SET))
    {
        return pos;
    }
    else
    {
        return -1;
    }
}

size_t FileStream::GetSize()
{
    if (!m_hFile) return -1;
    return BaseStream::GetSize();
}

size_t FileStream::SetSize(const size_t size)
{
    if (!m_hFile) return -1;
    Seek(size, SEEK_SET);
    return _chsize(_fileno(m_hFile), (long)GetPosition());
}