#pragma once
//#ifndef NO_DEFINES
#ifndef DEFINES_H
#define DEFINES_H
#include "pch.h"

#ifdef EXPORTING
#define EXTERN __declspec(dllexport)
#else
#define EXTERN __declspec(dllimport)
#pragma comment(lib, "LevelMod.lib")
#endif

bool EXTERN InvalidReadPtr(const void* const __restrict ptr, const DWORD size);
bool EXTERN InvalidReadPtr(const void* const __restrict ptr);
void DebugPrint(const char* file, DWORD line, const char* date, const char* string, ...);

enum enum_matrix
{
    RIGHT = 0,
    UP,
    AT,
    POS,
};

enum enum_vertex
{
    X = 0,
    Y,
    Z,
    W
};

#define _T(x) x
extern bool bAddedOptions, bDebugMode, bHooked;
//where p = pointer to data
#define VALIDATE_PTR(p) \
{ \
if (bDebugMode && InvalidReadPtr(p)) \
{ \
  debug_print( _T("Parameter ") _T(#p) \
  _T(" is not a valid read pointer\r\n") \
  _T("CFUNC ")  __FUNCTION__ _T("\r\n")); return NULL; \
} \
}

//where p = pointer to data s = size of data
#define VALIDATE_DATA(p, s) \
{ \
if (bDebugMode && InvalidReadPtr(p, s)) \
{ \
  debug_print( _T("Parameter ") _T(#p) \
  _T(" is not a valid read area\r\n") \
  _T("CFUNC ") __FUNCTION__ _T("\r\n")); return NULL; \
} \
}

#ifndef debug_print
#ifdef _DEBUG
#ifndef LevelModSettings
namespace LevelModSettings
{
    extern bool bLogging;
}
#endif
#define debug_print(fmt, ...) \
              do { if(LevelModSettings::bLogging) DebugPrint(__FILE__, __LINE__, __DATE__, fmt, __VA_ARGS__); \
              else printf(fmt, __VA_ARGS__); } while (0)
#else
#define debug_print(fmt, ...)
#endif
#endif

void _printf(const char* str, ...);
#endif
//#endif