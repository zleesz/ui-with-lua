/********************************************************************
*
*   Description :   CPUID
*
********************************************************************/ 

#ifndef _CPUID_H_
#define _CPUID_H_

namespace System
{

inline bool CPUIDAvailable()
{
    bool result;
    __asm
    {
        mov edx, 0
        pushfd
        pop eax
        mov ecx, eax
        xor eax, 0x00200000
        push eax
        popfd
        pushfd
        pop eax
        xor ecx, eax
        jz END
        mov edx, 1
END:
        push eax
        popfd
        mov eax, edx
        mov result, al
    }
    return result;
}

inline unsigned int CPUSignature()
{
    unsigned int result;
    __asm
    {
        push ebx
        mov eax, 1
        cpuid
        pop ebx
        mov result, eax
    }
    return result;
}

inline unsigned int CPUFeatures()
{
    unsigned int result;
    __asm
    {
        push ebx
        mov eax, 1
        cpuid
        pop ebx
        mov result, edx
    }
    return result;
}

}//System

#endif