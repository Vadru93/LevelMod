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


//Menu Element
struct Element
{
	BYTE unk[256];//not yet disambled
	typedef Element* (__thiscall* const pGetElement)(Element* pThis, DWORD name, DWORD id);

	Element* GetElement(DWORD name, DWORD id = 1)
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
//Get's the slider value of a menu element, check optionsmenu.qb
bool GetSliderValue(CStruct* pStruct, CScript* pScript);

//LevelMod Example Script
//Change the values of a struct, check optionsmenu.qb
bool ChangeValues(CStruct* pStruct, CScript* pScript);

#endif