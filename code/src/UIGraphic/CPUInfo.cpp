#include "stdafx.h"
#include "./CPUInfo.h"
#include "./CPUID.h"

using namespace System;

CPUInfo::CPUInfo(void) :
    MMXSupport(false),
    SSESupport(false),
    SSE2Support(false)
{
    CPUInfoInit();
}

CPUInfo& CPUInfo::Instance()
{
    static CPUInfo instance;
    return instance;
}

void CPUInfo::CPUInfoInit()
{
    if (!CPUIDAvailable()) return; // no CPUID available
    unsigned int features = CPUFeatures();
    if (features & 0x800000) MMXSupport = true; // support MMX
    if (features & 0x2000000) SSESupport = true; // support SSE
    if (features & 0x4000000) SSE2Support = true; // support SSE2
}