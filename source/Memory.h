#pragma once
#ifndef MEMORY_H
#define MEMORY_H
#undef NO_DEFINES
#include "Defines.h"
#include "Script.h"

typedef QBKeyHeader* (__cdecl* const pFreex)(void const* const __restrict addr);
static const pFreex freex = (pFreex)(0x00411400);

void* __restrict mallocx(const DWORD buflen);
#endif