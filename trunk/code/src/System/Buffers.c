// Unicode support by Jim Park -- 08/23/2007

#include "stdafx.h"
#include "Plugin.h"
#include "System.h"
#include "Buffers.h"

typedef struct tagTempStack TempStack;
struct tagTempStack
{
    TempStack *Next;
    TCHAR Data[0];
};
TempStack *tempstack = NULL;

static void AllocWorker(unsigned int mult)
{
    size_t size;
    if ((size = popintptr()) == 0)
    {
        system_pushint(0);
        return;
    }
    system_pushintptr((INT_PTR) GlobalAlloc(GPTR, size * mult));
}

PLUGINFUNCTIONSHORT(Alloc)
{
    AllocWorker(sizeof(unsigned char));
}
PLUGINFUNCTIONEND

PLUGINFUNCTIONSHORT(StrAlloc)
{
    AllocWorker(sizeof(TCHAR));
}
PLUGINFUNCTIONEND

PLUGINFUNCTIONSHORT(Copy)
{
    SIZE_T size = 0;
    HANDLE source, dest;
    TCHAR *str;
    // Get the string
    if ((str = system_popstring()) == NULL) return;

    // Check for size option
    if (str[0] == _T('/'))
    {
        size = (SIZE_T) StrToIntPtr(str+1);
        dest = (HANDLE) popintptr();
    }
    else dest = (HANDLE) StrToIntPtr(str);
    source = (HANDLE) popintptr();

    // Ok, check the size
    if (size == 0) size = (SIZE_T) GlobalSize(source);
    // and the destinantion
    if (!dest) 
    {
        dest = GlobalAlloc((GPTR), size);
        system_pushintptr((INT_PTR) dest);
    }

    // COPY!
    copymem(dest, source, size);

    GlobalFree(str);
}
PLUGINFUNCTIONEND
