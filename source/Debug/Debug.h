#pragma once
#ifndef DEBUG_H
#define DEBUG_H
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include "windows.h"
#include <stdio.h>
#include "Psapi.h"
#include <string.h>
#include <stdlib.h>
//Credit to Sirmabus
// Typical "trace" type function



void Tracer(LPCSTR format, ...);
void DebugPrint(const char* file, DWORD line, const char* date, const char* string, ...);


// ****************************************************************************
// Func: AddressToMappedName()
// Desc: Returns base name to memory address if it's mapped in the target
//       process.
//
// ****************************************************************************
char* AddressToMappedName(HANDLE hOwner, PVOID pAddress, char* szBuffer, int iSize);

bool ReportException(LPCTSTR pszFunction, LPEXCEPTION_POINTERS pExceptionInfo);
#ifdef _DEBUG
#define DEBUGSTART() debug_print("Enter" __FUNCTION__ "\n"); __try { 
#define DEBUGEND()   debug_print("Leave" __FUNCTION__ "\n"); }__except(ReportException(__FUNCTION__, GetExceptionInformation())){}
#else
#define DEBUGSTART()
#define DEBUGEND()
#endif
#endif