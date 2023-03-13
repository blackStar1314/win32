#pragma once
#include <Windows.h>
#include <objbase.h>

class ComInit
{
public:

    ComInit() { CoInitialize(nullptr); }
    ~ComInit() { CoUninitialize(); }
};