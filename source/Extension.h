#pragma once
#ifndef EXTENSION_H
#define EXTENSION_H
#undef NO_DEFINES
#ifndef NO_EXTRA_INCLUDE
#include "Observe.h"
#include "Memory.h"
#include "Globals.h"
#include "SuperSectors.h"
#include "Settings.h"
#endif




DWORD EXTERN GetTagCount();

void EXTERN SendChatMsg(char* text);

//Use this to be able draw your own things in present hook.
/*typedef HRESULT(__stdcall* Hooked_Present)(LPDIRECT3DDEVICE8 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
EXTERN Hooked_Present pHookedPresent;*/



//Set Element Text
inline void SetElementText(int id, char* text);

struct PairedText
{
    char* text;
    DWORD value;
};

//Menu Element
struct Element
{

    BYTE unk[0x1CC];
    BYTE* pUnk;//This seems to be unused
    float xy[2];
    BOOL unk2;
    PairedText* pair;//This is actually supported array of char, but dunno how to use, probably there is a bool that should be used when create sliderelement
    DWORD value;


    PairedText* AllocatePairedText(DWORD num)
    {
        /*typedef PairedText* (__thiscall* const pAllocatePairedText)(void* pMemRegion, DWORD size);
        return pAllocatePairedText(0x00411310)((void*)0x00859364, num * sizeof(PairedText));*/
        return (PairedText*)mallocx(num * sizeof(PairedText));
    }

    void SetSliderArrayText(const CArray* pArray);

    typedef Element* (__thiscall* const pGetElement)(Element* pThis, DWORD name, DWORD id);

    Element* GetElement(DWORD name, DWORD id = 0)
    {
        return pGetElement(0x004D23B0)(this, name, id);
    }
    DWORD GetValue()
    {
        typedef DWORD(__thiscall* const GetElementValue)(Element* pThis);
        return GetElementValue(0x004D54B0)(this);
    }
};

DWORD GetElementSliderValue(DWORD name);


//LevelMod Example Script
//Gets the slider value of a menu element, check optionsmenu.qb
bool GetSliderValue(CStruct* pStruct, CScript* pScript);

//Sets the sider array text
bool SetSliderArrayText(CStruct* pStruct, CScript* pScript);

//LevelMod Example Script
//Change the values of a struct, check optionsmenu.qb
bool ChangeValues(CStruct* pStruct, CScript* pScript);

#endif