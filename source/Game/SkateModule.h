#pragma once
#include "GameMode.h"

//The Skate Module is the Skate/THPS specific part of the Neversoft Engine
struct SkateMod
{

    //004367E0 mdl::skate::GetPointer, increase VP count
    SkateMod* Instance()
    {
        return *(SkateMod**)0x008E1E90;
    }

    GameMode* GetGameMode()
    {
        typedef GameMode* (__thiscall* const pGetGameMode)(SkateMod* pThis);
        return pGetGameMode(0x0046FF6)(this);
    }


    static bool ShouldBeAbsentNode(CStruct* pNode)
    {
        Network::NetHandler* net = Network::NetHandler::Instance();

        return (net->InNetGame() && pNode->ContainsFlag(Checksum("AbsentInNetGames")));
    }



    //
    //00438B60 GetProfile
};