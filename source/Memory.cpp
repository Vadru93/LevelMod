#define NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "Pch.h"
#include "Memory.h"

void* __restrict mallocx(const DWORD buflen)
{
    void* __restrict allocated;
    //static const DWORD addr = 0x00471C61;
    static const DWORD mx = 0x004112F0;
    /*DWORD pMem = *(DWORD*)addr;
    while(pMem==0)
    pMem = *(DWORD*)addr;
    DWORD old = pMem;
    pMem = *(DWORD*)pMem;
    while(pMem==0)
    pMem = *(DWORD*)old;
    pMem+=4;
    old = pMem;
    pMem = *(DWORD*)pMem;
    while(pMem==0)
    pMem = *(DWORD*)old;
    */
    _asm push 0
    _asm push 1
    _asm push buflen
    _asm call mx
    /*_asm pop esi
    _asm mov esp, ebp
    _asm pop ebp
    _asm ret
    return NULL;*/
    _asm mov allocated, eax
    return allocated;
}