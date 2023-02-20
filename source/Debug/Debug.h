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
#ifdef _DEBUG
#define assert(x, text) if(!(x)) { MessageBox(0, "Assertion in file: " __FILE__  "\nFunction: " __FUNCTION__ "\n" text, "Critical ERROR", 0); ExitProcess(0); }
#else
#define assert(x, text)
#endif


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

#define MAKE_RGB( r, g, b )		( ( ( 255 ) << 24 ) | ( ( b ) << 16 ) | ( ( g ) << 8 ) | ( r ) )
void AddDebugLine(Vertex v0, Vertex v1, int rgb0 = MAKE_RGB(127, 127, 127), int numDrawFrames = -1);
void DrawDebugLines();