/**
 * Copyright (C) 2015 Patrick Mours. All rights reserved.
 * License: https://github.com/crosire/d3d8to9#license
 */

#define NO_DEFINES
#define PCH_H

#include "d3dx9.hpp"
#include "d3d8to9.hpp"
#include "..\QBKey.h"

#define pRANDOM_SIZE 0x32000

static BYTE pRandom[pRANDOM_SIZE + 0x14] = { 0 };
BYTE hashTable[HASH_SIZE + 0x14] = { 0 };//the node hash names, each item is 12 bytes
//BYTE otherTable[0x50000];//unknown table, each item is 20 bytes

static DWORD pOld = 0x008A8B48;
static DWORD pNew = (DWORD)&hashTable;

//faces??
static BYTE pFaces[0x60804];

//#pragma pack(1)
/*struct TriggerScript
{
    DWORD qbKey;
    DWORD unk1;
    DWORD test;//should be 2
    //BYTE padding[3];
    void* script;
    BYTE* link;
};*/
//struct QBKeyHeader;
//#pragma pop(pack)

QBKeyHeader triggers[MAX_TRIGGERS]{ 0 };
QBKeyHeader triggers2[MAX_TRIGGERS2]{ 0 };
static DWORD pOld2 = 0x0087D8F8;
static DWORD pNew2 = (DWORD)&triggers;
QBKeyHeader* oldTriggers = (QBKeyHeader*)pOld2;

//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)FixMessage, 0, 0, 0);
extern void InitSkater();
extern void FixChat();
extern void FixRenderBugs();
extern void FixVertBugs();
extern void FixClampBugs();
extern void FixQBParsing();
extern void FixTagLimitProtections();
extern void ReadChatHook();
extern VOID HookFunc(DWORD destAddress, VOID(*func)(VOID), BYTE nopCount);
extern void AddMessageHook();
extern void AddFunctions();
extern void FixSpineFlag();
extern void FixMemoryProtection();

PFN_D3DXAssembleShader D3DXAssembleShader = nullptr;
PFN_D3DXDisassembleShader D3DXDisassembleShader = nullptr;
PFN_D3DXLoadSurfaceFromSurface D3DXLoadSurfaceFromSurface = nullptr;

#ifndef D3D8TO9NOLOG
// Very simple logging for the purpose of debugging only.
std::ofstream LOG;
#endif

extern "C" Direct3D8 * WINAPI Direct3DCreate8(UINT SDKVersion)
{
    //MessageBox(0, 0, 0, 0);
    memcpy((char*)0x5BBAF8, "Scripts\\qdir_lm.txt", 20);
    DWORD old;
    VirtualProtect((void*)0x0042B247, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042B250, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042B2C1, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042B1A4, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042B1CF, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x008A8B4D, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042B183, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x0042B2B8, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042B23E, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);



    VirtualProtect((void*)0x00426123, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
    VirtualProtect((void*)0x0042635A, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
    VirtualProtect((void*)0x0042B91C, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
    VirtualProtect((void*)0x0042C231, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
    VirtualProtect((void*)0x00426353, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
    VirtualProtect((void*)0x0042B923, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
    VirtualProtect((void*)0x0042C0F2, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
    VirtualProtect((void*)0x0042C10D, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
    VirtualProtect((void*)0x0042C24D, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
    /*VirtualProtect((void*)0x004282B1, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
    VirtualProtect((void*)0x004282BD, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable*/

    VirtualProtect((void*)0x0042611A, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042634A, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042B913, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);

    //something about nodearray
    VirtualProtect((void*)0x0042B9C8, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042B9E9, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BB31, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);



    //previus exe stuff
    VirtualProtect((void*)0x41211F, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x412134, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x41217F, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x4121D1, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x412245, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x42B93F, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x42b98A, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x42B9C9, sizeof(WORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x42B9EA, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x42BA1E, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x42BAC2, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x42BB32, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x42BB73, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x42BB98, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x42BBA0, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x42C09F, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x42C0C6, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x42C0CE, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x42C18D, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x42C1C7, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x42C1D5, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x42C1E1, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x42C1EC, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    //pRandom, something about drawing original max was only 1024 now it's 10 times bigger
    VirtualProtect((void*)0x004F41ED, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004F48A9, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004F4975, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004F4B49, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004F4B50, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x005C94C8, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042B171, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    /*FixMemoryProtection();

    *(DWORD*)0x0042C18D = 0x20AA00;
    *(DWORD*)0x0042C1C7 = 0x20A9F0;
    *(DWORD*)0x0042C1D5 = 0x20A9F0;
    *(DWORD*)0x0042C1E1 = 0x20A9F8;
    *(DWORD*)0x0042C1EC = 0x20A9FC;*/


    VirtualProtect((void*)0x0042BFE2, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BFE7, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x0042C114, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042C126, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042C151, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042C254, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042C271, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BFE2, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BFE7, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BE4B, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x004266D3, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x0042898E, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x00428992, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004266D7, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);


    VirtualProtect((void*)(0x0042BF3E + 6), sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BF46, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BF47, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x0042BF4E, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BF52, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BF56, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BF5A, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BF5B, sizeof(WORD), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x0042BF4E, 6, PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x0042C09D, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x0042C0C3, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042C0CB, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x0042BBCD, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x008E1DF8, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);



    VirtualProtect((void*)0x0042B0EF, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042B137, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042B153, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BC7B, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BCAF, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BCE7, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BC5E, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x0042B0FE, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BBDE, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BBF5, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BC50, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BC92, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BC99, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042BCDB, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    //VirtualProtect((void*)0x0042BFE2, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);


    /*VirtualProtect((void*)0x004FEE77, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FEEA4, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FEEAF, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FEEC6, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FF050, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);*/

    * (DWORD*)0x0042B0EF = 0x04A1F60F;
    *(DWORD*)0x0042B137 = 0x04A1F60F;
    *(DWORD*)0x0042B153 = 0x04A1F60F;
    *(DWORD*)0x0042BC7B = 0x04A1F60F;
    *(DWORD*)0x0042BCAF = 0x04A1F60F;
    *(DWORD*)0x0042BCE7 = 0x04A1F60F;
    *(DWORD*)0x0042BC5E = 0x04A1F60F;

    *(DWORD*)0x0042B0FE = 0x04A1F613;
    *(DWORD*)0x0042BBDE = 0x04A1F613;
    *(DWORD*)0x0042BBF5 = 0x04A1F613;
    *(DWORD*)0x0042BC50 = 0x04A1F613;
    *(DWORD*)0x0042BC92 = 0x04A1F613;
    *(DWORD*)0x0042BC99 = 0x04A1F613;
    *(DWORD*)0x0042BCDB = 0x04A1F613;

    *(DWORD*)0x0042BFE2 = 0x008935CC;


    *(DWORD*)0x0042C114 = (DWORD)&triggers2;
    *(DWORD*)0x0042C126 = (DWORD)&triggers2 + OTHER2_SIZE;

    *(DWORD*)0x0042C151 = (DWORD)&triggers2;

    *(DWORD*)0x0042C254 = (DWORD)&triggers2;
    *(DWORD*)0x0042C271 = (DWORD)&triggers2 + OTHER2_SIZE;


    *(DWORD*)0x0042C12D = (DWORD)&triggers2 + 0x10;
    *(DWORD*)0x0042C13B = (DWORD)&triggers2[MAX_TRIGGERS2 - 1].NextHeader;
    *(DWORD*)0x0042C143 = (DWORD)&triggers2[MAX_TRIGGERS2 - 1].NextHeader;

    *(DWORD*)0x004266D3 = (DWORD)&triggers2;
    *(BYTE*)0x004266D7 = 0x77;

    *(DWORD*)0x0042898E = (DWORD)&triggers2;
    *(BYTE*)0x00428992 = 0x77;

    //*(DWORD*)0x0042BFE2 = 0x008E1DF0 - 0x008920CC;//0x008B4B48-EXTRA_STRINGS;
    *(DWORD*)0x0042BFE7 = /*0x1D4C0*/ EXTRA_STRINGS + *(DWORD*)0x0042BFE7;

    *(DWORD*)0x0042BE4B = MAX_NUM_STRINGS * 8;


    //For debug...
    //*(BYTE*)(0x0042BF3E + 6) = 1;
    *(BYTE*)0x0042BF46 = 0x90;
    *(DWORD*)0x0042BF47 = 0x90909090;

    *(DWORD*)0x0042BF4E = 0x90909090;
    *(DWORD*)0x0042BF52 = 0x90909090;
    *(DWORD*)0x0042BF56 = 0x90909090;
    *(BYTE*)0x0042BF5A = 0x90;
    *(WORD*)0x0042BF5B = 0x9090;

    *(WORD*)0x0042BF4E = 0x1D89;
    *(DWORD*)0x0042BF50 = pOld;


    //Total Number of Level CScripts?
    *(DWORD*)0x0042C09D = *(DWORD*)0x0042C09D * 125;

    *(DWORD*)0x0042C0C4 = *(DWORD*)0x0042C09D - 8;
    *(DWORD*)0x0042C0CC = *(DWORD*)0x0042C09D - 4;

    *(DWORD*)0x0042BBCD = (DWORD)&pFaces;

    //*(BYTE*)0x008E1DF8 = 1;


    VirtualProtect((void*)0x004492C5, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);


    VirtualProtect((void*)0x00427A99, 16, PAGE_EXECUTE_READWRITE, &old);


    //Actually old data should be 0 at this point, but copy to be sure
    for (DWORD i = 0; i < OLD_HASH_SIZE; i += 4)
    {

        *(DWORD*)(pNew + i) = *(DWORD*)(pOld + i);

    }
    //Actually old data should be 0 at this point, but copy to be sure
    for (DWORD i = 0; i < OLD_OTHER_SIZE; i += 4)//Triggers
    {
        *(DWORD*)(pNew2 + i) = *(DWORD*)(pOld2 + i);
    }




    *(DWORD*)0x0042B247 = (DWORD)&hashTable;
    *(DWORD*)0x0042B250 = (DWORD)&hashTable;
    *(DWORD*)0x0042B2C1 = (DWORD)&hashTable;
    *(DWORD*)0x0042B1A4 = (DWORD)&hashTable + 8;
    *(DWORD*)0x0042B1CF = (DWORD)&hashTable + HASH_SIZE + 8;

    *(DWORD*)0x008A8B4D = (DWORD)&hashTable + 4;
    *(DWORD*)0x0042B171 = (DWORD)&hashTable + 4;
    *(DWORD*)0x0042B183 = (DWORD)&hashTable + HASH_SIZE + 4;
    //Dunno why it crashes when changing these, maybe should try 0xFF?
    //*(BYTE*)0x0042B2B8 = 0x3F;
    //*(BYTE*)0x0042B23E = 0x3F;

    //fix crash when calling function that don't exists
    *(DWORD*)0x00427A99 = 0xFF85;
    *(DWORD*)0x00427A9B = 0x90909090;
    *(DWORD*)0x00427A9F = 0x90909090;

    //Blood? old size was 8200 new 8200*4
    //*(DWORD*)0x004492C5 = 0x8020;


    *(DWORD*)0x00426123 = (DWORD)&triggers;
    *(DWORD*)0x0042635A = (DWORD)&triggers;
    *(DWORD*)0x0042B91C = (DWORD)&triggers;
    *(DWORD*)0x0042C231 = (DWORD)&triggers;
    *(DWORD*)0x0042612A = (DWORD)&triggers + 8;
    *(DWORD*)0x00426353 = (DWORD)&triggers + 8;
    *(DWORD*)0x0042B923 = (DWORD)&triggers + 8;
    *(DWORD*)0x0042C0F2 = (DWORD)&triggers + 16;
    *(DWORD*)0x0042C10D = (DWORD)&triggers + OTHER_SIZE + 0x10;

    *(DWORD*)0x0042C24D = (DWORD)&triggers + OTHER_SIZE;


    /**(DWORD*)0x004282B1 = (DWORD)&otherTable + OTHER_SIZE;
    *(DWORD*)0x004282BD = (DWORD)&otherTable + OTHER_SIZE;*/

    //008918F8 maybe this one needs to get increased aswelll?
    //Dunno why it crash when try to change this, maybe should try 0xFF?
    *(BYTE*)0x0042611A = 0xFF;
    *(BYTE*)0x0042634A = 0xFF;
    *(BYTE*)0x0042B913 = 0xFF;


    //pRandom, something about drawing original max was only 1024 now it's 10 times
    //if it randomly crashes when looking around in a huge map maybe this need to be increased again
    *(DWORD*)0x004F41ED = (DWORD)&pRandom;
    *(DWORD*)0x004F48A9 = (DWORD)&pRandom;
    *(DWORD*)0x004F4975 = (DWORD)&pRandom;
    *(DWORD*)0x004F4B49 = (DWORD)&pRandom;
    *(DWORD*)0x004F4B50 = (DWORD)&pRandom;
    *(DWORD*)0x005C94C8 = pRANDOM_SIZE / 0x14;



    *(DWORD*)0x0042B9C8 = 0xC0000;
    *(DWORD*)0x0042B9E9 = 0x10000;
    *(DWORD*)0x0042BB31 = 0x10000;



    //The stuff previusly made in exe


    //this is the SuperSectors list size
    //if change the 0xFF here need to also change it in LevelMod, else it can't find the SuperSectors
    *(DWORD*)0x41211F = 0xC0000;
    *(DWORD*)0x412134 = 0x10000;
    *(BYTE*)0x41217F = 0xFF;
    *(DWORD*)0x4121D1 = 0x10000;
    *(BYTE*)0x412245 = 0xFF;

    *(BYTE*)0x42B93F = 0xFF;
    *(BYTE*)0x42b98A = 0xFF;

    *(WORD*)0x42B9C9 = 0x3000;
    *(BYTE*)0x42B9EA = 0x40;
    //*(BYTE*)0x42BA1E = 0x3F;
    //*(BYTE*)0x42BAC2 = 0x3F;
    *(BYTE*)0x42BB32 = 0x40;

    *(BYTE*)0x42BB73 = 0x10;
    *(BYTE*)0x42BB98 = 0x10;
    *(BYTE*)0x42BBA0 = 0x10;

    *(BYTE*)0x42C09F = 0x02;
    *(BYTE*)0x42C0C6 = 0x02;
    *(BYTE*)0x42C0CE = 0x02;

    //this is the NodeArray size maybe need to increase this...
    //each item in list is 0x10 bytes so the total size need to be able to be divided by that
    *(DWORD*)0x42C18D = 0x20AA00;
    *(DWORD*)0x42C1C7 = *(DWORD*)0x42C18D - 0x10;//last item
    *(DWORD*)0x42C1D5 = *(DWORD*)0x42C18D - 0x10;//last item
    *(DWORD*)0x42C1E1 = *(DWORD*)0x42C18D - 0x10 + 8;//last item+8
    *(DWORD*)0x42C1EC = *(DWORD*)0x42C18D - 0x10 + 0xC;//last item+0xC


    //this is old code, keep it here just in case
/*for (DWORD i = 0; i < OLD_MAX_TRIGGERS; i++)
{

    if (oldTriggers[i].test)
    {
    /**(TriggerScript*)((BYTE*)&triggers[i] + 0x3000) = oldTriggers[i];
    (*(TriggerScript*)(((BYTE*)&triggers[i] + 0x3000))).link -= (DWORD)oldTriggers;
    (*(TriggerScript*)(((BYTE*)&triggers[i] + 0x3000))).link += (DWORD)triggers + 0x3000;*/
    /*triggers[i] = oldTriggers[i];
    if (triggers[i].link)
    {
        triggers[i].link -= (DWORD)oldTriggers;
        triggers[i].link += (DWORD)triggers;
        printf("oldLink %p new link %p triggers %p\n", oldTriggers[i].link, triggers[i].link, triggers);
    }*/
    //}
    /*DWORD q = oldTriggers[i].qbKey;
    q &= 0x0000FFFF;
    q = q * 4 + q;

    TriggerScript* t = (TriggerScript*)(q*4 + pNew2);
    if((BYTE)t->test!=2)
    {
        t->qbKey = oldTriggers[i].qbKey;
        t->unk1 = oldTriggers[i].unk1;
        t->test = oldTriggers[i].test;
        //t->padding = oldTriggers[i].padding;
        t->script = oldTriggers[i].script;
        t->link = oldTriggers[i].link;

    }
    else
    {
        while (t->qbKey)
        {
            t++;
        }
        t->qbKey = oldTriggers[i].qbKey;
        t->unk1 = oldTriggers[i].unk1;
        t->test = oldTriggers[i].test;
        //t->padding = oldTriggers[i].padding;
        t->script = oldTriggers[i].script;
        t->link = oldTriggers[i].link;
    }
}*/





    HookFunc(0x004788C5, ReadChatHook, 1);
    HookFunc(0x004E0DF2, AddMessageHook, 2);

    InitSkater();
    FixChat();
    FixRenderBugs();
    FixVertBugs();
    FixClampBugs();
    FixQBParsing();

    FixTagLimitProtections();

    VirtualProtect((void*)0x004A7CAC, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00478983, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);


    FixSpineFlag();
    //MessageBox(0, 0, 0, 0);


#ifndef D3D8TO9NOLOG
    static bool LogMessageFlag = true;

    if (!LOG.is_open())
    {
        LOG.open("d3d8_2.log", std::ios::trunc);
    }

    if (!LOG.is_open() && LogMessageFlag)
    {
        LogMessageFlag = false;
        MessageBox(nullptr, TEXT("Failed to open debug log file \"d3d8.log\"!"), nullptr, MB_ICONWARNING);
    }

    LOG << "Redirecting '" << "Direct3DCreate8" << "(" << SDKVersion << ")' ..." << std::endl;
    LOG << "> Passing on to 'Direct3DCreate9':" << std::endl;
#endif

    IDirect3D9* const d3d = Direct3DCreate9(D3D_SDK_VERSION);

    if (d3d == nullptr)
    {
        return nullptr;
    }

    // Load D3DX
    if (!D3DXAssembleShader || !D3DXDisassembleShader || !D3DXLoadSurfaceFromSurface)
    {
        const HMODULE module = LoadLibrary(TEXT("d3dx9_43.dll"));

        if (module != nullptr)
        {
            D3DXAssembleShader = reinterpret_cast<PFN_D3DXAssembleShader>(GetProcAddress(module, "D3DXAssembleShader"));
            D3DXDisassembleShader = reinterpret_cast<PFN_D3DXDisassembleShader>(GetProcAddress(module, "D3DXDisassembleShader"));
            D3DXLoadSurfaceFromSurface = reinterpret_cast<PFN_D3DXLoadSurfaceFromSurface>(GetProcAddress(module, "D3DXLoadSurfaceFromSurface"));
        }
        else
        {
#ifndef D3D8TO9NOLOG
            LOG << "Failed to load d3dx9_43.dll! Some features will not work correctly." << std::endl;
#endif
            if (MessageBox(nullptr, TEXT(
                "Failed to load d3dx9_43.dll! Some features will not work correctly.\n\n"
                "It's required to install the \"Microsoft DirectX End-User Runtime\" in order to use d3d8to9.\n\n"
                "Please click \"OK\" to open the official download page or \"CANCEL\" to continue anyway."), nullptr, MB_ICONWARNING | MB_TOPMOST | MB_SETFOREGROUND | MB_OKCANCEL | MB_DEFBUTTON1) == IDOK)
            {
                //ShellExecute(nullptr, TEXT("open"), TEXT("https://www.microsoft.com/download/details.aspx?id=35"), nullptr, nullptr, SW_SHOW);

                return nullptr;
            }
        }
    }

    return new Direct3D8(d3d);
}

