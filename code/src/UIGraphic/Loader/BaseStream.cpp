//
// BaseStream.cpp 
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "./BaseStream.h"

using namespace Stream;

BaseStream::BaseStream(void) :
    Valid(false)
{

}

BaseStream::~BaseStream(void)
{
}

size_t BaseStream::GetSize()
{
    size_t pos, size;
    pos = GetPosition();
    Seek(0, SEEK_END);
    size = GetPosition();
    Seek(pos, SEEK_SET);
    return size;
}

size_t BaseStream::SetSize(const size_t size)
{
    //do nothing
    return -1;
}