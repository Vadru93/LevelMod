#pragma once
#include "GameMode.h"

//The Skate Module is the Skate/THPS specific part of the Neversoft Engine
struct SkateMod
{

    //004367E0 mdl::skate::GetPointer, increase VP count
    SkateMod* Instance()
    {
        return 0;
    }

    GameMode* GetGameMode()
    {
        typedef GameMode*(__thiscall* const pGetGameMode)(SkateMod* pThis);
        return pGetGameMode(0x0046FF6)(this);
    }

  

//
//00438B60 GetProfile
};