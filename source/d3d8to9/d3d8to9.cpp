/**
 * Copyright (C) 2015 Patrick Mours. All rights reserved.
 * License: https://github.com/crosire/d3d8to9#license
 */

#define NO_DEFINES
#define PCH_H

#include "d3dx9.hpp"
#include "d3d8to9.hpp"
#include "..\Script\QBKey.h"
#include "..\Render\CustomShaders.h"


#define pRANDOM_SIZE 0x32000

static BYTE pRandom[pRANDOM_SIZE + 0x14] = { 0 };
BYTE hashTable[HASH_SIZE + 0x14] = { 0 };//the node hash names, each item is 12 bytes
//BYTE otherTable[0x50000];//unknown table, each item is 20 bytes

static DWORD pOld = 0x008A8B48;
static DWORD pNew = (DWORD)&hashTable;


struct SPrefixLookup
{
    // Checksum of the prefix string
    DWORD mChecksum;

    // Pointer to the list of node indices of the nodes that have this prefix.
    WORD* mpNodes;
};
// in thug1src 13000
#define MAX_PREFIX_LOOKUPS 21000
// in thug1src 121000, this needs to be so much bigger because of extralayer sharing same prefix
#define NODE_LIST_BUFFER_SIZE 0x65804
static SPrefixLookup sp_prefix_lookups[MAX_PREFIX_LOOKUPS];

//Node Index Lookup(For prefix)
static BYTE pIndexLookup[NODE_LIST_BUFFER_SIZE];


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
extern void ReadFirstOptions();

PFN_D3DXAssembleShader D3DXAssembleShader = nullptr;
PFN_D3DXDisassembleShader D3DXDisassembleShader = nullptr;
PFN_D3DXLoadSurfaceFromSurface D3DXLoadSurfaceFromSurface = nullptr;

#ifndef D3D8TO9NOLOG
// Very simple logging for the purpose of debugging only.
std::ofstream LOG;
#endif

extern bool bDebugMode;
#ifndef debug_print
#ifdef _DEBUG
#define debug_print(fmt, ...) \
            do { printf(fmt, __VA_ARGS__); } while (0)
#else
#define debug_print(fmt, ...)
#endif
#endif


extern unsigned long crc32f(const char* buf);
//To optimize CRC we put the checksum in eax instead of calling Checksum("string")
//string here means pointer in memory where string is pushed to stack
//eax means pointer to where function is called and should be replaced with eax
//esp means where stack is changed, since we remove the push string we need to decrease add esp with 4
struct OptimizedCRC
{
    DWORD string;
    DWORD eax;
    DWORD esp;

    void Optimize()
    {
        DWORD old;
        DWORD temp_size = esp != 0xFF ? esp : 0;
        VirtualProtect((LPVOID)string, 5 + 5 + 3 + eax + temp_size, PAGE_EXECUTE_READWRITE, &old);

        DWORD ptr = string;
        char error[256];
        sprintf(error, "%X %d", string, esp);
        ptr++;
        char* c = *(char**)ptr;

        *(BYTE*)string = 0x90;
        string++;
        ptr += 4;

        //If trying to access esp we need to decrease by 4
        DWORD temp_ptr = ptr;
        if (esp != 0xFF)
        {
            while (*(BYTE*)temp_ptr == 0xC7 || *(BYTE*)temp_ptr == 0xC6 || *(BYTE*)temp_ptr == 0x88 || *(BYTE*)temp_ptr == 0x89)
            {
                if (temp_ptr > (ptr + eax))
                {
                    debug_print("temp_ptr %X ptr %X start %X\n", temp_ptr, ptr, string - 1);
                    MessageBox(0, 0, 0, 0);
                    break;
                }
                else if (*(BYTE*)temp_ptr == 0xC7 || *(BYTE*)temp_ptr == 0xC6)
                {
                    *(BYTE*)(temp_ptr + 3) -= 4;
                    temp_ptr += 8;
                }
                else
                {
                    if (*(BYTE*)(temp_ptr + 1) == 0x9E)
                    {
                        *(BYTE*)(temp_ptr + 2) -= 4;
                        temp_ptr += 6;
                    }
                    else
                    {
                        *(BYTE*)(temp_ptr + 3) -= 4;
                        temp_ptr += 4;
                    }
                }
            }
        }

        ptr += eax;
        if (*(BYTE*)ptr != 0xE8)
            MessageBox(0, "Wrong eax", "", 0);
        *(BYTE*)ptr = 0xB8;
        ptr++;
        debug_print("Optimizing checksum access: %s\n", c);
        *(DWORD*)ptr = crc32f(c);

        //Replace push string with nop
        *(DWORD*)string = 0x90909090;
        if (esp == 0xFF)
            return;
        ptr += 4;
        temp_ptr = ptr;

        //If found push skip 5 bytes
        if (*(BYTE*)temp_ptr == 0x68)
            temp_ptr += 5;
        else if (*(BYTE*)temp_ptr == 0x6A)
            temp_ptr += 2;

        //If trying to access esp we need to decrease by 4
        while ((*(BYTE*)temp_ptr == 0x8B && (*(BYTE*)(temp_ptr + 2) != 0xE8 && *(BYTE*)(temp_ptr +1) != 0xF0 && *(BYTE*)(temp_ptr + 1) != 0xCB && *(BYTE*)(temp_ptr + 1) != 0x8F && *(BYTE*)(temp_ptr + 1) != 0x8E && *(BYTE*)(temp_ptr + 1) != 0x0E && *(BYTE*)(temp_ptr + 1) != 0x4E)) || *(BYTE*)temp_ptr == 0x88 || *(BYTE*)temp_ptr == 0x89)
        {
            if (temp_ptr >= (ptr + esp))
            {
                MessageBox(0, 0, 0, 0);
                break;
            }
            else if (*(BYTE*)temp_ptr == 0x89  || *(BYTE*)temp_ptr == 0x88)
            {
                if (*(BYTE*)(temp_ptr + 1) == 0x9E)
                {
                    *(BYTE*)(temp_ptr + 2) -= 4;
                    temp_ptr += 6;
                }
                else
                {
                    *(BYTE*)(temp_ptr + 3) -= 4;
                    temp_ptr += 4;
                }
            }
            else
            {
                /*printf("temp %X\n", temp_ptr);
                MessageBox(0, "4", 0, 0);*/
                *(BYTE*)(temp_ptr + 3) -= 4;
                /*if (*(BYTE*)(temp_ptr + 3) == 0)
                    *(BYTE*)(temp_ptr + 3) = 0x90;*/
                    //advance to next
                if (*(BYTE*)(temp_ptr + 1) == 0x8C)
                    temp_ptr += 7;
                else
                    temp_ptr += 4;
            }
            //If found push skip 5 bytes
            if (*(BYTE*)temp_ptr == 0x68 || *(BYTE*)temp_ptr == 0xE8 || *(BYTE*)temp_ptr == 0xB8)
                temp_ptr += 5;
            else if (*(BYTE*)temp_ptr == 0x6A)
                temp_ptr += 2;
        }
        ptr += esp;
        if (*(BYTE*)ptr != 0x83)
        {
            MessageBox(0, "Wrong esp", error, 0);
        }

        //If add esp, 4 replace it with nop
        if (*(BYTE*)(ptr + 2) == 4)
        {
            *(BYTE*)ptr = 0x90;
            ptr++;
            *(BYTE*)ptr = 0x90;
            ptr++;
            *(BYTE*)ptr = 0x90;
        }
        //Else reduce by 4
        else
        {
            *(BYTE*)(ptr + 2) -= 4;
        }
    }
};

OptimizedCRC optimized[] = { {0x00401B3F, 5, 9},  {0x00401E0C, 4, 9}, {0x004021DA, 4, 9}, {0x00404C50, 8, 0}, {0x00404C71, 0, 0},
    {0x00404C89, 0, 0},  {0x00404CA1, 0, 0}, {0x00405240, 16, 0}, {0x00405640, 8, 0}, {0x0041525F, 0, 0}, {0x0041527A, 0, 0},
    {0x00405660, 0, 0}, {0x00405678, 0, 0},  {0x0040BBA7, 0, 4},  {0x0040BBBD, 0, 4},  {0x0040BBD3, 0, 4},  {0x0040BBE9,0, 4},
 {0x0040BBFF, 0, 4},  {0x00413C97, 0, 0},  {0x00413FB4, 0, 0},  {0x00413FD6, 0, 0},  {0x0041417E, 0, 0},   {0x0041419D, 0, 0},
 {0x004141E7, 0, 0},  {0x00414206, 0, 0},  {0x004143BD, 0, 0},  {0x004146A4,8, 0 },  {0x004151A8, 0, 0},   {0x00415295, 0, 0},
 {0x004152B2, 0, 0},  {0x004152D0, 0, 0},  {0x004152EC, 0, 0},  {0x00415374, 0, 0},  {0x00415397, 0, 0},   {0x004153BA, 0, 0},
{0x004153E0, 0, 0}, {0x0041545B, 0, 0}, {0x00415476, 0, 0}, {0x00415491, 0, 0}, {0x004154A9, 0, 4}, {0x00416291, 8, 0},
{0x004162DA, 0, 0}, {0x0041631B, 0, 0}, {0x004163A4, 0, 0}, {0x00416823, 8, 0}, {0x00416A52, 0, 9}, {0x00416A89, 0, 9},
{0x00416AC0, 0, 9}, {0x00416AF3, 0, 9}, {0x00416B45, 0, 9}, {0x00416B75, 0, 9}, {0x00416BA5, 0, 9}, {0x00416C11, 0, 9},
{0x00416C58, 0, 9}, {0x00416C7D, 0, 9}, {0x00416CA9, 0, 0}, {0x0041AA22, 0, 0}, {0x0041AC0C, 0, 0}, {0x0041AC3F, 0, 0},
{0x0041B04A, 0, 0}, {0x0041D258, 0, 22}, {0x0041D268, 0, 6}, {0x0041F7B3, 8, 0}, {0x0041FF8C, 0, 0}, {0x00420051, 0, 0},
{0x00421C41, 0, 0}, {0x00421C5D, 0, 0}, {0x0042216F, 8, 0}, {0x00422D31, 2, 0}, {0x00422D70, 0, 0}, {0x00422D7E, 0, 0},
    {0x00422DB2, 0, 0}, {0x00422DC0, 0, 0}, {0x00422DD5, 2, 0}, {0x00422DE5, 0, 0}, {0x00422EF4, 0, 0}, {0x00422F0D, 0, 0},
    {0x00422F23, 0, 0}, {0x00422FD9, 4, 0}, {0x004242CE, 8, 0}, {0x00424A05, 5, 9}, {0x00424BD8, 2, 0}, {0x00424BF9, 2, 0},
    {0x00424EA5, 0, 0}, {0x0042556C, 0, 4}, {0x0042560F, 2, 0}, {0x00425639, 0, 4}, {0x004256BF, 2, 0}, {0x004256E9, 0, 4},
    {0x0042576F, 2, 0}, {0x00425799, 0, 4}, {0x0042590F, 2, 0}, {0x00425939, 0, 4}, {0x00425A6F, 0, 21}, {0x00425A79, 4, 7},
    {0x004265AB, 0, 0}, {0x0042C8B5, 0, 6}, {0x0042D59E, 11, 0}, {0x00431ED1, 11, 0}, {0x00433FF5, 0, 13}, {0x004341E8, 2, 0},
    {0x00434212, 0, 4}, {0x00434236, 0, 4}, {0x00434508, 0, 0}, {0x00434531, 0, 0}, {0x0043484D, 0, 0}, {0x00434FCA, 0, 0},
{0x004375B3, 0, 0}, {0x0043766C, 0, 0}, {0x004378E5, 2, 0}, /*{0x004380A5, 30, 0},*/ {0x004380ED, 6, 0}, {0x0043883C, 0, 0},
    {0x00438BBA, 2, 4}, {0x00438E2D, 4, 4}, {0x00438E82, 0, 0}, {0x00438E9A, 0, 0}, {0x00438F9C, 0, 0}, {0x00438FC2, 0, 0},
    {0x00438FDF, 0, 0}, {0x00438FFD, 0, 0}, {0x0043901B, 0, 0}, {0x00439528, 0, 0}, {0x00439585, 0, 0}, {0x004395F3, 0, 0},
    {0x0043969B, 0, 0}, {0x0043972A, 0, 6}, {0x004399A9, 0, 0}, {0x004399BE, 0, 6}, {0x00439AE7, 0, 6}, {0x00439B6B, 2, 0},
    {0x00439B8D, 0, 6}, {0x0043A342, 0, 4}, {0x0043A596, 0, 4}, {0x0043B229, 0, 4}, {0x0043B28C, 0, 0}, {0x0043CFF0, 5, 0},
    {0x0043D00A, 0, 0}, {0x0043D01A, 0, 0}, {0x0043D039, 0, 0}, {0x0043D25C, 0, 0}, {0x0043D271, 0, 0}, {0x0043D281, 0, 0},
    {0x0043D2C3, 4, 0}, {0x0043DA4A, 0, 0}, {0x0043DD79, 0, 3}, {0x0043DE11, 0, 0}, {0x0043DEF4, 5, 0}, {0x0043DF0E, 0, 0},
    {0x0043769F, 0, 0}, {0x0043DF1E, 0, 0}, {0x0043DF3A, 0, 0}, {0x0043DFDA, 0, 0}, {0x0043E0E2, 0, 0}, {0x0043E0FB, 0, 0},
    {0x0043E111, 0, 0}, {0x0043E144, 0, 2}, {0x0043E15D, 0, 0}, {0x0043FDAE, 0, 0}, {0x0043FF67, 0, 0}, {0x0043FFB9, 0, 0},
    {0x004404EA, 5, 0}, {0x004409C0, 0, 9}, {0x00440A34, 0, 9}, {0x00440AB9, 4, 9}, {0x00440B1F, 8, 0}, {0x00440B4D, 0, 9},
    {0x00440BC0, 0, 9}, {0x00440CF4, 0, 0}, {0x00440E25, 8, 9}, {0x0044114D, 3, 0}, {0x0044125E, 3, 0}, {0x00441412, 3, 0},
    {0x00441570, 2, 0}, {0x00441592, 0, 0}, {0x004415B2, 0, 0}, {0x004415D2, 0, 0}, {0x004415F2, 0, 0}, {0x00441612, 0, 0},
    {0x00441632, 0, 0}, {0x00441652, 0, 0}, {0x00441672, 0, 0}, {0x00441692, 0, 0}, {0x004416B2, 0, 0}, {0x004416D2, 0, 0},
    {0x004416F2, 0, 0}, {0x00441712, 0, 0}, {0x00441732, 0, 0}, {0x00441752, 0, 0}, {0x00441772, 0, 0}, {0x00441792, 0, 0},
    {0x004417B2, 0, 0}, {0x0044301E, 5 , 0}, {0x0044306C, 5, 0}, {0x004430BD, 5, 0}, {0x004431FD, 0, 0}, {0x004432A8, 8, 0},
    {0x004432C0, 0, 0}, {0x00443377, 8, 0}, {0x0044338F, 0, 0}, {0x00443464, 4, 0}, {0x00443501, 4, 0}, {0x00443626, 0, 0},
    {0x00443644, 5, 9}, {0x004438DE, 5, 0}, {0x0044393B, 0, 34}, {0x00443945, 6, 18}, {0x00443FE4, 8, 0}, {0x004440FE, 16, 0}
};

#ifdef _DEBUG
namespace LevelModSettings
{
    extern bool bLogging;
}
#endif


void PatchOpenSpy(DWORD address)
{
    DWORD old;
    VirtualProtect((void*)address, 11, PAGE_EXECUTE_READWRITE, &old);
    if (!strcmp((char*)address, "gamespy.com") && !strcmp((char*)address, "openspy.org"))
    {
        MessageBox(0, "Unknown exe detected", "Please install official 1.01 patch", 0);
    }
    else
    {
        char OpenSpy[] = "openspy.org";
        memcpy((void*)address, OpenSpy, 11);
    }
}
CRITICAL_SECTION critical;
extern FILE* logFile;
extern "C" Direct3D8 * WINAPI Direct3DCreate8(UINT SDKVersion)
{
#ifdef _DEBUG
    /*static char** ppArgs;
    static DWORD num_args;
    _asm mov eax, [esp+0x890+0xDC];
    _asm mov ppArgs, eax;
    _asm mov eax, [esp+0x8A0+0xDC];
    _asm mov num_args, eax;
    */
    InitializeCriticalSection(&critical);
    LPTSTR cmd = GetCommandLine();
    cmd++;
    while (*cmd != '"')
        ++cmd;
    cmd++;
    while (isspace(*cmd))
        cmd++;

    while (*cmd == '-')
    {
        if (!_stricmp(cmd, "-log"))
        {
            LevelModSettings::bLogging = true;
            logFile = fopen("LM_Log.txt", "w");
            break;
        }
        else
            cmd += strlen(cmd) + 1;
    }
#endif
    //MessageBox(0, 0, 0, 0);
    memcpy((char*)0x5BBAF8, "Scripts\\qdir_lm.txt", 20);

    ReadFirstOptions();
    DWORD old;

    PatchOpenSpy(0x5CAEFD);
    PatchOpenSpy(0x5CB46F);
    PatchOpenSpy(0x5CB6DB);

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

    VirtualProtect((void*)0x004DC7C9, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004DC635, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x000F4241, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    //VirtualProtect((void*)0x005CEC78, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x005CEC78 *= 6;
    /*VirtualProtect((void*)0x00535BB9, 6, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x00535BB9 = 0x0000F2E9;
    *(WORD*)0x00535BBD = 0x9000;*/

    //VirtualProtect((void*)0x0049D936, sizeof(DWORD)*3, PAGE_EXECUTE_READWRITE, &old);

   /* *(DWORD*)0x0049D936 = 0x909090C3;
    *(DWORD*)0x0049D93A = 0x90909090;
    *(DWORD*)0x0049D93E = 0x90909090;*/

    /*FixMemoryProtection();

    *(DWORD*)0x0042C18D = 0x20AA00*2;
    *(DWORD*)0x0042C1C7 = *(DWORD*)0x0042C18D - 0x10;//0x20A9F0;
    *(DWORD*)0x0042C1D5 = *(DWORD*)0x0042C18D - 0x10;//0x20A9F0;
    *(DWORD*)0x0042C1E1 = *(DWORD*)0x0042C18D - 0x10+8;//0x20A9F8;
    *(DWORD*)0x0042C1EC = *(DWORD*)0x0042C18D - 0x10+0xC;*///0x20A9FC;

    //*(DWORD*)0x0041134F *= 2;

    //MessageHandler
    * (DWORD*)0x004DC7C9 *= 4;
    //NetHandler
    *(DWORD*)0x004DC635 *= 4;
    //Unk
    //*(DWORD*)0x000F4241 *= 2;


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

    VirtualProtect((void*)0x00438031, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x004c050c, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    //VirtualProtect((void*)0x004c067b, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x004C067A, sizeof(DWORD) * 2, PAGE_EXECUTE_READWRITE, &old);


    VirtualProtect((void*)0x54EA45, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x54EA64, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    //*(DWORD*)0x54EA45 = 1; //fulscreen draw mode
    //*(DWORD*)0x54EA64 = 1; //windowed draw mode

    //moved to here because code was executing in another thread and using half fixed stuff
    //Optimize static checksum access
    for (DWORD i = 0; i < sizeof(optimized) / sizeof(OptimizedCRC); i++)
    {
        /*for (DWORD j = 0; j < sizeof(optimized) / sizeof(OptimizedCRC); j++)
        {
            if (optimized[i].esp != 0xFF && optimized[i].string > optimized[j].string && optimized[i].string < (optimized[j].string + optimized[j].eax + optimized[j].esp + 10))
            {
                printf("%X\n", optimized[i].string);
                MessageBox(0, 0, 0, 0);
            }
        }*/
        optimized[i].Optimize();
    }
    /**(BYTE*)(0x00425A7E + 3) += 4;
    *(BYTE*)(0x00425A87 + 3) += 4;
    *(DWORD*)0x00425A8E = 0x8904C483;


    *(BYTE*)(0x0044394A + 2) += 4;
    *(BYTE*)(0x00443957 + 2) += 4;
    *(DWORD*)0x00443967 = 0x8908C483;*/



    if (Gfx::fps_fix)
    {
        /**(DWORD*)0x004C067A = 0x90909090;
        *(DWORD*)(0x004C067A + 4) = 0x90909090;*/
        /**(DWORD*)0x004c050c = 0x3E80;// 16000;
        *(DWORD*)0x004c067a = 0x90909090;
        *(DWORD*)0x004c067e = 0x90909090;*/
    }

    static BYTE CodeCave_AddAnimation[] = { 0x8D, 0x54, 0x29, 0x30, 0x8B, 0x03, 0x52, 0x85, 0xC0, 0x0F, 0x84, 0xD8, 0x49, 0x0F, 0x00, 0x05,
         0x20, 0x01, 0x00, 0x00, 0x89, 0x15, 0x21, 0x00, 0x40, 0x00, 0x8B, 0x10, 0x81, 0xFA, 0x30, 0x30, 0x30, 0x30, 0x75, 0x09,
         0xC6, 0x05, 0x20, 0x00, 0x40, 0x00, 0x01, 0xEB, 0x07, 0xC6, 0x05, 0x20, 0x00, 0x40, 0x00, 0x01, 0x83, 0xFA, 0x02, 0x0F,
         0x85, 0xAA, 0x49, 0x0F, 0x00, 0x83, 0xC0, 0x10, 0x8B, 0x15, 0x21, 0x00, 0x40, 0x00, 0x89, 0x10, 0xE9, 0x9A, 0x49, 0x0F, 0x00 };
    //DWORD AddAnimation_Addr = 0x004094FB;
    VirtualProtect((void*)CodeCave_AddAnimation, sizeof(CodeCave_AddAnimation), PAGE_EXECUTE_READWRITE, &old);

    // memcpy((void*)AddAnimation_Addr, CodeCave_AddAnimation, sizeof(CodeCave_AddAnimation));
    VirtualProtect((void*)0x004FDEDD, 5, PAGE_EXECUTE_READWRITE, &old);

    /**(DWORD*)0x004FDEDD = 0xF0B619E9;
    *(BYTE*)0x004FDEE1 = 0xFF;*/
    *(DWORD*)&(CodeCave_AddAnimation[11]) = (0x004FDEE2) - ((DWORD)CodeCave_AddAnimation + 11 + 4);
    *(DWORD*)&(CodeCave_AddAnimation[57]) = (0x004FDEE2) - ((DWORD)CodeCave_AddAnimation + 57 + 4);
    *(DWORD*)&(CodeCave_AddAnimation[73]) = (0x004FDEE2) - ((DWORD)CodeCave_AddAnimation + 73 + 4);
    *(BYTE*)0x004FDEDD = 0xE9;
    *(DWORD*)0x004FDEDE = (DWORD)CodeCave_AddAnimation - (0x004FDEDD + 5);

    //MessageBox(0, 0, 0, 0);

    *(DWORD*)0x00438031 *= 2;//testing compressed strings

    //VirtualProtect((void*)0x0042BFE2, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);


    /*VirtualProtect((void*)0x004FEE77, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FEEA4, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FEEAF, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FEEC6, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FF050, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);*/

    * (DWORD**)0x0042B0EF = &sp_prefix_lookups[0].mChecksum;
    *(DWORD**)0x0042B137 = &sp_prefix_lookups[0].mChecksum;
    *(DWORD**)0x0042B153 = &sp_prefix_lookups[0].mChecksum;
    *(DWORD**)0x0042BC7B = &sp_prefix_lookups[0].mChecksum;
    *(DWORD**)0x0042BCAF = &sp_prefix_lookups[0].mChecksum;
    *(DWORD**)0x0042BCE7 = &sp_prefix_lookups[0].mChecksum;
    *(DWORD**)0x0042BC5E = &sp_prefix_lookups[0].mChecksum;

    *(WORD***)0x0042B0FE = &sp_prefix_lookups[0].mpNodes;
    *(WORD***)0x0042BBDE = &sp_prefix_lookups[0].mpNodes;
    *(WORD***)0x0042BBF5 = &sp_prefix_lookups[0].mpNodes;
    *(WORD***)0x0042BC50 = &sp_prefix_lookups[0].mpNodes;
    *(WORD***)0x0042BC92 = &sp_prefix_lookups[0].mpNodes;
    *(WORD***)0x0042BC99 = &sp_prefix_lookups[0].mpNodes;
    *(WORD***)0x0042BCDB = &sp_prefix_lookups[0].mpNodes;

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

    *(DWORD*)0x0042BBCD = (DWORD)&pIndexLookup;

    //*(BYTE*)0x008E1DF8 = 1;


    VirtualProtect((void*)0x004492C5, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);


    VirtualProtect((void*)0x00427A99, 16, PAGE_EXECUTE_READWRITE, &old);


    //Actually old data should be 0 at this point, but copy to be sure
    /*for (DWORD i = 0; i < OLD_HASH_SIZE; i += 4)
    {

        *(DWORD*)(pNew + i) = *(DWORD*)(pOld + i);

    }
    //Actually old data should be 0 at this point, but copy to be sure
    for (DWORD i = 0; i < OLD_OTHER_SIZE; i += 4)//Triggers
    {
        *(DWORD*)(pNew2 + i) = *(DWORD*)(pOld2 + i);
    }*/




    * (DWORD*)0x0042B247 = (DWORD)&hashTable;
    *(DWORD*)0x0042B250 = (DWORD)&hashTable;
    *(DWORD*)0x0042B2C1 = (DWORD)&hashTable;
    *(DWORD*)0x0042B1A4 = (DWORD)&hashTable + 8;
    *(DWORD*)0x0042B1CF = (DWORD)&hashTable + HASH_SIZE + 8;

    *(DWORD*)0x008A8B4D = (DWORD)&hashTable + 4;
    *(DWORD*)0x0042B171 = (DWORD)&hashTable + 4;
    *(DWORD*)0x0042B183 = (DWORD)&hashTable + HASH_SIZE + 4;
    //Dunno why it crashes when changing these, maybe should try 0xFF?
    //*(BYTE*)0x0042B2B8 = 0xFF;
    //*(BYTE*)0x0042B23E = 0xFF;

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



    *(DWORD*)0x0042B9C8 = 0xC4B0;
    *(DWORD*)0x0042B9E9 = 0x1064;
    *(DWORD*)0x0042BB31 = 0x1064;



    //The stuff previusly made in exe


    //this is the SuperSectors list size
    //if change the 0xFF here need to also change it in LevelMod, else it can't find the SuperSectors
    *(DWORD*)0x41211F = 0xC4B00;
    *(DWORD*)0x412134 = 0x10064;
    *(BYTE*)0x41217F = 0xFF;
    *(DWORD*)0x4121D1 = 0x10064;
    *(BYTE*)0x412245 = 0xFF;

    *(BYTE*)0x42B93F = 0xFF;
    *(BYTE*)0x42b98A = 0xFF;

    //*(WORD*)0x42B9C9 = 0x3000;
    //*(BYTE*)0x42B9EA = 0x40;
    //*(BYTE*)0x42BA1E = 0xFF;
    //*(BYTE*)0x42BAC2 = 0xFF;
    //*(BYTE*)0x42BB32 = 0x40;

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
    //MessageBox(0, "Please debug from here", 0, 0);


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
        const HMODULE mod = LoadLibrary(TEXT("d3dx9_43.dll"));

        if (mod != nullptr)
        {
            D3DXAssembleShader = reinterpret_cast<PFN_D3DXAssembleShader>(GetProcAddress(mod, "D3DXAssembleShader"));
            D3DXDisassembleShader = reinterpret_cast<PFN_D3DXDisassembleShader>(GetProcAddress(mod, "D3DXDisassembleShader"));
            D3DXLoadSurfaceFromSurface = reinterpret_cast<PFN_D3DXLoadSurfaceFromSurface>(GetProcAddress(mod, "D3DXLoadSurfaceFromSurface"));
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

    //MessageBox(0, "Am I called correctly?", "", 0);
    return new Direct3D8(d3d);
}

//void UpdateShatterObjects();
struct SuperSector;
bool ShouldBlend(SuperSector*);
void SetupBlendedMode(WORD flags);
void RestoreBlendedMode();
void RenderBlendedSectors();
void DrawDebugLines();
void __stdcall RenderShatterObjects();
//Code taken from PARTYMANX partymod 
void __cdecl fixedDrawWorldAgain(int16_t param_1, int param_2) {
    int* lastVertexBuffer = (int*)0x00906760;
    int* rwObj = (int*)0x00970a8c;
    int* currentTexture = (int*)0x0090675c;
    int* numMeshes = (int*)0x005c94c8;
    int* meshList = (int*)pRandom;
    typedef void(__cdecl* RwGetRenderState)(int, void*);
    typedef void(__cdecl* RwSetRenderState)(int, int);
#define RwSetRenderState RwSetRenderState(0x0055ce10)
    typedef void(__cdecl* SetTextureStage)(int*);
#define SetTextureStage SetTextureStage(0x004f4320)
    typedef void(__cdecl* SomeLightingStuff)(int, int);
#define SomeLightingStuff SomeLightingStuff(0x0052fae0)
    typedef void(__cdecl* DrawWorldMesh)(int, int);
#define DrawWorldMesh DrawWorldMesh(0x004f4210)

    int32_t uVar1;
    int texture;
    int i;
    int* mesh;	// unknown struct

    uVar1 = *rwObj;
    SomeLightingStuff(0, 0);
    *lastVertexBuffer = 0xffffffff;
    *currentTexture = -1;
    RwSetRenderState(0x14, 2);
    i = 0;
    if (0 < *numMeshes) {
        mesh = meshList;
        texture = *currentTexture;
        while (i < *numMeshes) {
            if ((mesh[2] != 0) && ((*(uint16_t*)(*mesh + 0x1a) & param_1) == 0)) {
                SuperSector* sector = NULL;
#ifdef _DEBUG
                if (mesh[3])
                {
                    BYTE* p = (BYTE*)(mesh[3] + 8);
                    if (p)
                    {
                        sector = *(SuperSector**)p;
                    }
                    //MessageBox(0, ok, ok, 0);
                }
#endif

                if (texture != *mesh) {
                    SetTextureStage(mesh);
                    if ((*(uint8_t*)(*mesh + 0x1a) & 0x40)) {
                        RwSetRenderState(0x14, 2);
                    }
                    else {
                        RwSetRenderState(0x14, 1);
                    }
                }
                if (ShouldBlend(sector))
                {
                    DWORD old_blend, old_alpha;
                    Gfx::pDevice->GetRenderState(D3DRS_SRCBLEND, &old_blend);
                    Gfx::pDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &old_alpha);
                    Gfx::pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                    Gfx::pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
                    DrawWorldMesh(*rwObj, mesh[3]);
                    RestoreBlendedMode();
                    Gfx::pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, old_alpha);
                    Gfx::pDevice->SetRenderState(D3DRS_SRCBLEND, old_blend);
                }
                else
                    DrawWorldMesh(*rwObj, mesh[3]);
                texture = mesh[0];
                *currentTexture = texture;
            }
            i = i + 1;
            mesh = mesh + 5;
        }
    }

    DrawDebugLines();
    RenderShatterObjects();

    RwSetRenderState(0x14, 2);
    RwSetRenderState(8, 1);

    return;
}