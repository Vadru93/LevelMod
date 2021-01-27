#undef NO_DEFINES
#undef NO_EXTRA_INCLUDE
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Commands.h"
#include "Extension\Extension.h"
#include "zip\unzip.h"
#include <conio.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
//#include "detours.h"
#include "Psapi.h"
#include <iostream>
#include "Shellapi.h"
#include <sstream>
#include "d3d9.h"
#include "Settings\IniReader.h"
#include "Settings\IniWriter.h"
#include "Hook\Bugfixes.h"
#include "Memory\String.h"
#undef ONLY_SHADER
#include "Render\CustomShaders.h"
#include "Objects\ObjParser.h"
#include <string.h>
#include <memory.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Render\shadow.h"
#include "dinput.h"
#include "Objects\rail.h"
/*0
004F9B9E < -non semi
    8
    004F9BFB < --semi transparent






    1 = models
    2 = skater
    4 = particles ?
    call 004F9C80*/

    /*//Game states
    extern bool GotSuperSectors;

    //XINPUT
    extern bool vibrating;
    extern XINPUT_VIBRATION vibration;
    extern DWORD vibrationFrames;
    extern CXBOXController* Player1;

    //Script stuff
    extern const char* QBTypes[];*/

    //Not curently used?
ID3DXLine* line = NULL;

DWORD numLineVertices = 0;
D3DCOLOR lineColor = 0;
D3DXMATRIX lineWorld;

vector<SuperSector*> EnvironmentObjects;
extern vector<SuperSector*> PointyObjects;
extern vector<ColouredVertex> bbox_rails;

LPD3DXSPRITE eye_sprite;
DWORD wheel_timer = 0;
LPDIRECT3DTEXTURE9 wheel_texture[12];

BYTE eye_state = 0;
DWORD actual_timer = 0;
DWORD last_state;
bool rotating = false;
double framelength;

void SetTagLimit(DWORD limit);


bool init = true;
bool init2 = false;
bool bAddedOptions = false;

CIniWriter* OptionWriter = NULL;// ini("LevelMod.ini");
CIniReader* OptionReader = NULL;
char IniPath[MAX_PATH + 1] = "";

const DWORD line_fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

struct line_vertex {
    float x, y, z, rhw;  // The transformed(screen space) position for the vertex.
    DWORD colour;        // The vertex colour.
};
line_vertex lineVertices[2];

#pragma comment(lib, "d3dx9.lib")


#define PAGE_READABLE (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)






using namespace std;
//Used for qb loading
char qbPath[256] = "";
//Used to store LevelName
char Level[256] = "";
//True when downloading new update
bool bDownloading = false;
//True when DebugMode is enabled
bool bDebugMode = false;
//Old bugfix
static const char* clamp = "Clamp";
BYTE clampfunc[] = { 0x2E, 0x3E, 0x2B, 0xFF, 0xFE, 0xCE, 0xAE, 0xBE, 0x90, 0x90, 0x23 };
//Game function
void* __restrict mallocx(const DWORD buflen);
//Last loading qb script
static const char* lastQB = NULL;
//Reverse endian
#define REV_END(x)  ((x << 24) | ((x << 8) & 0xFF0000) | (x >> 24) | ((x >> 8) & 0xFF00))
//Maximum recv length
#define MAXRECV 20576
#undef SendMessage

//Not sure if these declarations are needed anymore?
struct CStruct;
struct CStructHeader;
struct CArray;
struct KeyState;
struct Matrix;
void SetStructValues(CStructHeader* pStruct, CStructHeader* values);
void SetArrayValues(CArray* pArray, CStructHeader* values);
bool InvalidReadPtr(const void* const __restrict ptr, const DWORD size);
bool InvalidReadPtr(const void* const __restrict ptr);

//Current version
#define VERSION 4.8f
static float version = VERSION;

//Used for logging
FILE* debugFile = NULL;

//Used for custom ObserveMode
extern ObserveMode* pObserve;

void MaybeFixStutter();

//Used for graf tag counter
char tags[256] = "Tags: 0";
char stat_cheat_message[] = "Stat cheat detected...";
LPD3DXFONT m_font = NULL;
RECT rct;

void InjectHook(DWORD addr, BYTE* hook, DWORD size)
{
    DWORD old;
    VirtualProtect((void*)addr, size, PAGE_EXECUTE_READWRITE, &old);
    memcpy((void*)addr, hook, size);
}

__inline void HookFunction(DWORD addr, void(SfxManager::* function)(float* lvol, float* rvol, Vector* soundSource, float dropoffDist), BYTE byteCode = 0, DWORD nopCount=0)
{
    DWORD old;
    VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, PAGE_EXECUTE_READWRITE, &old);
    if (byteCode)
        *(DWORD*)(addr - 1) = byteCode;
    *(DWORD*)addr = (PtrToUlong((void*&)function) - addr) - 4;
    for (DWORD i = 0; i < nopCount; i++)
        *(BYTE*)addr++ = 0x90;
    //
    //VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, old, &old);
}

__inline void HookFunction(DWORD addr, bool (RailNode::* function )(int SearchNode), BYTE byteCode = 0, DWORD nopCount = 0)
{
    DWORD old;
    VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, PAGE_EXECUTE_READWRITE, &old);
    if (byteCode)
        *(DWORD*)(addr - 1) = byteCode;
    *(DWORD*)addr = (PtrToUlong((void*&)function) - addr) - 4;
    for (DWORD i = 0; i < nopCount; i++)
        *(BYTE*)addr++ = 0x90;
    //
    //VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, old, &old);
}

__inline void HookFunction(DWORD addr, void (Skater::* function)(), BYTE byteCode = 0, DWORD nopCount = 0)
{
    DWORD old;
    VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, PAGE_EXECUTE_READWRITE, &old);
    if (byteCode)
        *(DWORD*)(addr - 1) = byteCode;
    *(DWORD*)addr = (PtrToUlong((void*&)function) - addr) - 4;
    for (DWORD i = 0; i < nopCount; i++)
        *(BYTE*)addr++ = 0x90;
    //
    //VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, old, &old);
}

__inline void HookFunction(DWORD addr, void (Skater::* function)(DWORD type), BYTE byteCode = 0, DWORD nopCount = 0)
{
    DWORD old;
    VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, PAGE_EXECUTE_READWRITE, &old);
    if (byteCode)
        *(DWORD*)(addr - 1) = byteCode;
    *(DWORD*)addr = (PtrToUlong((void*&)function) - addr) - 4;
    for (DWORD i = 0; i < nopCount; i++)
        *(BYTE*)addr++ = 0x90;
    //
    //VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, old, &old);
}

__inline void HookFunction(DWORD addr, void (Skater::* function)(D3DXVECTOR3 *off_point), BYTE byteCode = 0, DWORD nopCount = 0)
{
    DWORD old;
    VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, PAGE_EXECUTE_READWRITE, &old);
    if (byteCode)
        *(DWORD*)(addr - 1) = byteCode;
    *(DWORD*)addr = (PtrToUlong((void*&)function) - addr) - 4;
    for (DWORD i = 0; i < nopCount; i++)
        *(BYTE*)addr++ = 0x90;
    //
    //VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, old, &old);
}

__inline void HookFunction(DWORD addr, void (Skater::* function)(Skater::EStateType state, DWORD terrain), BYTE byteCode = 0, DWORD nopCount = 0)
{
    DWORD old;
    VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, PAGE_EXECUTE_READWRITE, &old);
    if (byteCode)
        *(DWORD*)(addr - 1) = byteCode;
    *(DWORD*)addr = (PtrToUlong((void*&)function) - addr) - 4;
    for (DWORD i = 0; i < nopCount; i++)
        *(BYTE*)addr++ = 0x90;
    //
    //VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, old, &old);
}

__inline void HookFunction(DWORD addr, void (RailManager::* function)(DWORD index, CStruct* pNodeStruct), BYTE byteCode = 0, DWORD nopCount = 0)
{
    DWORD old;
    VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, PAGE_EXECUTE_READWRITE, &old);
    if (byteCode)
        *(DWORD*)(addr - 1) = byteCode;
    *(DWORD*)addr = (PtrToUlong((void*&)function) - addr) - 4;
    for (DWORD i = 0; i < nopCount; i++)
        *(BYTE*)addr++ = 0x90;
    //
    //VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, old, &old);
}


__inline void HookFunction(DWORD addr, void (Network::MessageHandler::* function)(unsigned char msg_id, void* pCallback, int flags, Network::NetHandler* net_handler, int prio), BYTE byteCode = 0, DWORD nopCount = 0)
{
    DWORD old;
    VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, PAGE_EXECUTE_READWRITE, &old);
    if (byteCode)
        *(DWORD*)(addr - 1) = byteCode;
    *(DWORD*)addr = (PtrToUlong((void*&)function) - addr) - 4;
    for (DWORD i = 0; i < nopCount; i++)
        *(BYTE*)addr++ = 0x90;
    //
    //VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, old, &old);
}

__inline void HookFunction(DWORD addr, DWORD function, BYTE byteCode = 0, DWORD nopCount = 0)
{
    DWORD old;
    VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, PAGE_EXECUTE_READWRITE, &old);
    if (byteCode)
        *(DWORD*)(addr - 1) = byteCode;
    *(DWORD*)addr = ((function) - addr) - 4;
    addr += 4;
    for (DWORD i = 0; i < nopCount; i++)
        *(BYTE*)addr++ = 0x90;
    //
    //VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, old, &old);
}

__inline void HookFunction(DWORD addr, void* function, BYTE byteCode = 0, DWORD nopCount = 0)
{
    DWORD old;
    VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, PAGE_EXECUTE_READWRITE, &old);
    if (byteCode)
        *(DWORD*)(addr - 1) = byteCode;
    *(DWORD*)addr = (PtrToUlong(function) - addr) - 4;
    addr += 4;
    for (DWORD i = 0; i < nopCount; i++)
        *(BYTE*)addr++ = 0x90;
    //
    //VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, old, &old);
}

struct Hook
{
    void* func;
    BYTE backup[10];

    Hook()
    {
        func = NULL;
    }


    static DWORD_PTR* GetD3D8VTable(DWORD Base, DWORD Len)
    {
        /*BYTE* pBase = (BYTE*)Base;
        for (DWORD i = 0; i < Len; i++)
        {
          if (*pBase == 0xC7 && *(pBase + 1) == 0x06 && *(pBase + 0x06) == 0x89 && *(pBase + 0x07) == 0x86 && *(pBase + 0x0C) == 0x89 && *(pBase + 0x0D) == 0x86)
            return (DWORD*)*(DWORD*)(pBase + 2);
          pBase++;
        }
        return (0);*/
        BYTE* pBase = (BYTE*)Base;
        DWORD ptr = (DWORD)(pBase + 0x0001A4F3);
        return (DWORD*)*(DWORD*)(&ptr);//0x000012DC);
    }

    Hook(DWORD vTableIndex, void* Function, DWORD nopCount = 0)
    {
        func = NULL;
        DWORD addr = NULL;
        do {
            addr = (DWORD)GetModuleHandle("d3d8.dll");
            Sleep(20);
        } while (!addr);
        if (!addr)
            MessageBox(0, "no d3d8 found", "", 0);
        DWORD ptr = *(DWORD*)0x00970E48;
        DWORD_PTR* vTable = (DWORD*)*(DWORD*)(ptr);
        _printf("vTable %X\n", vTable);
        //DWORD_PTR* vTable = GetD3D8VTable(addr, 0x0001A4F4);//0x128000);
        if (vTable == NULL)
        {
            MessageBox(0, "no vtabl found", "", 0);
            ExitProcess(0);
        }

        addr = (DWORD)vTable[vTableIndex];
        _printf("addr %X", addr);
        /*BYTE jmp[6] = { 0xe9,			//jmp
        0x00, 0x00, 0x00, 0x00,		//address
        0xc3 };						//retn

        ReadProcessMemory((HANDLE)-1, (void*)addr, backup, 6, 0);
        *(DWORD*)&jmp[1] = ((DWORD)Function - addr - 5);
        WriteProcessMemory((HANDLE)-1, (void*)addr, jmp, 6, 0);*/

        ReadProcessMemory((HANDLE)-1, (void*)addr, backup, 10, 0);

        static BYTE jmpTable[11] = { 0 };
        BYTE* jmp = jmpTable;
        DWORD dwback;
        VirtualProtect(jmp, 11, PAGE_EXECUTE_READWRITE, &dwback);
        VirtualProtect((BYTE*)addr, (5 + nopCount), PAGE_READWRITE, &dwback);
        memcpy(jmp, (BYTE*)addr, (5 + nopCount)); jmp += (5 + nopCount);
        jmp[0] = 0xE9;
        *(DWORD*)(jmp + 1) = (DWORD)((BYTE*)addr + (5 + nopCount) - jmp) - (5 + nopCount);
        ((BYTE*)addr)[0] = 0xE9;
        *(DWORD*)((BYTE*)addr + 1) = (DWORD)((BYTE*)Function - (BYTE*)addr) - 5;
        //VirtualProtect((BYTE*)addr, 5, dwback, &dwback);
        func = jmp - (5 + nopCount);
        DWORD idx = 0;
        while (idx < nopCount)
        {
            *(BYTE*)((BYTE*)addr + 5 + idx) = 0x90;
            idx++;
        }
        /*char addrs[25] = "";
        sprintf(addrs, "%X %X", addr, func);
        MessageBox(0, addrs, addrs, 0);*/
    }

    Hook(const char* Dll, const char* FuncName, void* Function)
    {
        func = NULL;
        DWORD addr = (DWORD)GetProcAddress(GetModuleHandle(Dll), FuncName);
        if (addr == 0)
        {
            addr = (DWORD)GetModuleHandle("d3d8.dll");
            if (addr == 0)
            {
                MessageBox(0, "fail", "fail", MB_OK);
                return;
            }
            DWORD_PTR* vTable = GetD3D8VTable(addr, 0x0001A4F4);//0x128000);
            if (vTable == NULL)
            {
                MessageBox(0, "fail", "fail", MB_OK);
                return;
            }
            addr = (DWORD)vTable[15];
        }
        BYTE jmp[6] = { 0xe9,			//jmp
          0x00, 0x00, 0x00, 0x00,		//address
          0xc3 };						//retn

        ReadProcessMemory((HANDLE)-1, (void*)addr, backup, 6, 0);
        *(DWORD*)&jmp[1] = ((DWORD)Function - addr - 5);
        WriteProcessMemory((HANDLE)-1, (void*)addr, jmp, 6, 0);
    }

    void Restore()
    {
        if (func)
            WriteProcessMemory((HANDLE)-1, (void*)func, backup, 6, 0);
    }

    void Rehook(void* Function)
    {
        if (func)
        {
            BYTE jmp[6] = { 0xe9,			//jmp
              0x00, 0x00, 0x00, 0x00,		//address
              0xc3 };						//retn

            ReadProcessMemory((HANDLE)-1, (void*)func, backup, 6, 0);
            *(DWORD*)&jmp[1] = ((DWORD)Function - (DWORD)func - 5);
            WriteProcessMemory((HANDLE)-1, (void*)func, jmp, 6, 0);
        }
    }
};

//Used for custom mod to be able to add render callback
Hook CreateDeviceHook;
Hook PresentHook;
//HRESULT (*CreateDevice)(UINT Adapter,DWORD DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3D_PARAMS* pPresentationParameters,void** ppReturnedDeviceInterface);
//HRESULT (*Present)(LPDIRECT3DDEVICE8 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)=NULL;

/*HRESULT __stdcall Present(LPDIRECT3DDEVICE8 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
typedef HRESULT(__stdcall* Present_t)(LPDIRECT3DDEVICE8 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
Present_t pPresent;*/
struct D3D_PARAMS
{
};

void NotGood(DWORD checksum, CScript* pScript)
{
    _printf("Called a function that doesn't exsits..\nName %s(%X) in function %s(%X) location %p\n", FindChecksumName(checksum), checksum, FindChecksumName(pScript->scriptChecksum), pScript->scriptChecksum, pScript->address);
    MessageBox(0, "This means your Scripts are trashed or old", "Called a function that don't exists", 0);
}

__declspec(naked)  void __cdecl NotGood_naked()
{
    static const DWORD jmpBack = 0x00427EF8;
    static DWORD checksum;
    static CScript* pScript;
    _asm mov checksum, edx;
    _asm mov pScript, esi;
    _asm pushad;
    _asm pushfd;
    NotGood(checksum, pScript);
    _asm popfd;
    _asm popad;
    _asm jmp[jmpBack];
}

bool CheatIsOn(DWORD cheat)
{
    typedef bool(__cdecl* const pCheatIsOn)(DWORD, DWORD);
    return pCheatIsOn(0x004B5310)(cheat, 1);
}

DWORD GetCheat(DWORD checksum)
{
    typedef DWORD(__cdecl* const pGetCheat)(DWORD, DWORD);
    return pGetCheat(0x004263E0)(checksum, 1);
}


Vertex oldSpeed;
float oldAngle1, oldAngle2;

//depricated
bool PreWallplantScript(CStruct* pParams, CScript* pScript)
{
    static const DWORD ptr = 0x005D06C0;
    VALIDATE_PTR((void*)ptr);
    DWORD pSpeed = *(DWORD*)ptr + 0x580;
    VALIDATE_PTR((void*)pSpeed);
    pSpeed = *(DWORD*)pSpeed + 0x4;
    VALIDATE_PTR((void*)pSpeed);
    pSpeed = *(DWORD*)pSpeed + 0x2C;
    VALIDATE_PTR((void*)pSpeed);
    pSpeed = *(DWORD*)pSpeed + 0x48;
    VALIDATE_PTR((void*)pSpeed);
    pSpeed = *(DWORD*)pSpeed + 0x334;
    VALIDATE_DATA((Vertex*)pSpeed, sizeof(Vertex));

    oldSpeed = (*(const Vertex* const __restrict)pSpeed);

    static const DWORD ptr2 = 0x008E2498;
    VALIDATE_PTR((void*)ptr2);
    DWORD pPos = *(DWORD*)ptr2 + 0x13C;
    VALIDATE_PTR((void*)pPos);
    pPos = *(DWORD*)pPos + 0x14;
    VALIDATE_PTR((void*)pPos);
    const DWORD old = pPos;
    pPos = *(DWORD*)pPos + 0x4BC;
    if (InvalidReadPtr((void*)pPos))
    {
        pPos = old;
        pPos = *(DWORD*)pPos + 0x3B4;
        VALIDATE_PTR((void*)pPos);
    }
    pPos = *(DWORD*)pPos + 0x18;
    VALIDATE_DATA((Vertex*)pPos, sizeof(Vertex));
    pPos += 0x35C;
    oldAngle1 = *((float*)pPos);
    pPos += 8;
    oldAngle2 = *((float*)pPos);
    return true;
}

bool WallplantScript(CStruct* pParams, CScript* pScript)
{
    DEBUGSTART()
    {
        //ExecuteQBScript("DefaultGapScript", pParams, pScript);
        //ExecuteQBScript("DefaultGapScript", pParams, pScript);
        static const DWORD ptr = 0x005D06C0;
        VALIDATE_PTR((void*)ptr);
        DWORD pSpeed = *(DWORD*)ptr + 0x580;
        VALIDATE_PTR((void*)pSpeed);
        pSpeed = *(DWORD*)pSpeed + 0x4;
        VALIDATE_PTR((void*)pSpeed);
        pSpeed = *(DWORD*)pSpeed + 0x2C;
        VALIDATE_PTR((void*)pSpeed);
        pSpeed = *(DWORD*)pSpeed + 0x48;
        VALIDATE_PTR((void*)pSpeed);
        pSpeed = *(DWORD*)pSpeed + 0x334;
        VALIDATE_DATA((Vertex*)pSpeed, sizeof(Vertex));

        Vertex speed = (*(const Vertex* const __restrict)pSpeed);
        speed = oldSpeed;
        speed.x *= -1.0f;
        //speed.y = 0.0f;
        speed.z *= -1.0f;
        (*(Vertex*)pSpeed) = speed;

        /*static const DWORD ptr2 = 0x008E1E90;
        VALIDATE_PTR((void*)ptr2);
        DWORD pAngle = *(DWORD*)ptr2 + 0xC0;
        VALIDATE_PTR((void*)pAngle);
        pAngle = *(DWORD*)pAngle + 0x5C4;
        VALIDATE_PTR((void*)pAngle);
        pAngle = *(DWORD*)pAngle + 0x0C;
        VALIDATE_PTR((void*)pAngle);
        pAngle = *(DWORD*)pAngle + 0x10;
        VALIDATE_PTR((void*)pAngle);
        pAngle = *(DWORD*)pAngle + 0x40;
        VALIDATE_DATA((float*)pAngle, sizeof(float));
        (*(float*)pAngle) *= -1.0f;//float angle = *(float*)pAngle*(180.0f / 3.14159f);
        _printf("speedx %f speedz %f anglez %f\n", speed.x, speed.z, *(float*)pAngle);*/

        void* first = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x13C);
        void* last = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x140);
        void* current = first;
        Vertex* camera = ObserveMode::GetCamera();
        Vertex* pos = NULL;

        if (current && !InvalidReadPtr(current) && *(DWORD*)current == 0x0058E504 && *(DWORD*)((DWORD)current + 4) != 0xFFFFFFFF)
        {
            pos = (Vertex*)(*(DWORD*)((DWORD)current + 20) + 0x18);
            speed = *((Vertex*)(*(DWORD*)((DWORD)current + 20) + 0x18 + 16));
            _printf("pos %f oldPos %f", pos->x, speed.x);

            /*speed.x -= pos->x;
            speed.z -= pos->z;*/
            /*char* c = (char*)((DWORD)current + 32);
            DWORD i = 0;
            while (*c != 0)
            {
            name[i] = *c;
            c++;
            i++;
            }
            name[i] = 0;*/
        }
        else
            return false;


        float cameraAngle = atan2f((pos->x - camera->x), (pos->z - camera->z));
        cameraAngle *= -1.0f;
        if (camera->x >= pos->x)
            pos->x += 10.0f;
        else
            pos->x -= 10.0f;
        if (camera->z >= pos->z)
            pos->z += 10.0f;
        else
            pos->z -= 10.0f;

        static const DWORD ptr2 = 0x008E2498;
        VALIDATE_PTR((void*)ptr2);
        DWORD pPos = *(DWORD*)ptr2 + 0x13C;
        VALIDATE_PTR((void*)pPos);
        pPos = *(DWORD*)pPos + 0x14;
        VALIDATE_PTR((void*)pPos);
        const DWORD old = pPos;
        pPos = *(DWORD*)pPos + 0x4BC;
        if (InvalidReadPtr((void*)pPos))
        {
            pPos = old;
            pPos = *(DWORD*)pPos + 0x3B4;
            VALIDATE_PTR((void*)pPos);
        }
        pPos = *(DWORD*)pPos + 0x18;
        VALIDATE_DATA((Vertex*)pPos, sizeof(Vertex));
        pPos += 0x35C;
        //*((float*)pPos) *= -1.0f;
        *((float*)pPos) = oldAngle1;
        pPos += 8;
        //float oldf = *(float*)pPos;
        //*((float*)pPos) = (cameraAngle / 0.5f / 3.14159f) - 1.0f;
        //_printf("new %f camAngle%f old %f ", *((float*)pPos), cameraAngle, oldf);
        //*((float*)pPos) *= -1.0f;*/
        *((float*)pPos) = oldAngle2;
        //_printf("newnew %f posX %f speedX %f\n", *((float*)pPos), pos->x, speed.x);
    }
    DEBUGEND()
        return true;
}

BYTE oldLimit = 32;

bool CreatePair(CStruct* pStruct, CScript* pScript)
{
    const char* name = "";
    pStruct->GetString("name", &name);
    float x = 0;
    float y = 0;
    CStructHeader* value = pStruct->head;
    while (value)
    {
        if (value->QBkey == Checksum("x"))
        {
            x = value->value.f;
        }
        else if (value->QBkey == Checksum("y"))
            y = value->value.f;
        value = value->NextHeader;
    }

    CStructHeader* param = pScript->params->AddParam((char*)name, QBKeyHeader::PAIR);
    param->pVec = (D3DXVECTOR3*)mallocx(8);
    param->pVec->x = x;
    param->pVec->y = y;
    return true;
}

void DestroySuperSectors()
{
    EnvironmentObjects.clear();
    PointyObjects.clear();
    String::RemoveLevelStrings();
    QScript::Scripts->ClearLevelTable();
    _printf("Going to remove MovingObjects\n");
    GameState::GotSuperSectors = false;
    extern void UnloadShatterObjects();
    UnloadShatterObjects();
    *(bool*)0x00400020 = false;
    if (movingObjects.size())
        movingObjects.clear();

    if (!Gfx::loadingShaders)
        Gfx::UnloadShaders();
}
void CreateSuperSectors()
{
    _printf("Going to create MovingObjects\n");
    GameState::GotSuperSectors = true;
    KeyMap::UpdateKeyMap();
    *(bool*)0x00400020 = true;
    //Game::skater = Skater::UpdateSkater();
}
FILE* logFile;
void __cdecl add_log(const char* string, ...)
{
    if (string == (const char*)0x005B6120) [[unlikely]]
        DestroySuperSectors();
    else if (string == (const char*)0x005B6104) [[unlikely]]
        CreateSuperSectors();
    if (!bDebugMode) [[likely]]
        return;

    static char buf[256] = "";
    static CScript* pScript = NULL;
    _asm mov pScript, esi
    /*if ((DWORD)string == 0 || (DWORD)string < 0x00420000 || (DWORD)string > 0x02000000)
      return -1;
      //MessageBox(0, string, string, 0);
      va_list args;
      va_start(args, string);
      int k = -1;

      if (logFile)
      {
      fseek(logFile, 0, SEEK_END);
      k = vfprintf(logFile, string, args);
      fputc('\n', logFile);
      fflush(logFile);
      }
      va_end(args);
      return k;*/

    if ((DWORD)string == 0 || (DWORD)string < 0x00420000 || (DWORD)string > 0x02000000 || (strstr(string, "Tried to"))) [[unlikely]]//string[0] == '\n' && string[1] == 'T' && string[2] == 'r'))
        return;

    va_list args;
    va_start(args, string);
    int k = -1;
    k = vsprintf(buf, string, args);
    va_end(args);
    char* p = buf;
    if (pScript && (DWORD)pScript > 0x00400000 && (DWORD)pScript < 0x10000000)
    {
        while (*p != 0x0)
        {
            if (*p == '?' && *(p + 1) == '?' && *(p + 2) == '?' && *(p + 3) == '?' && *(p + 4) == '?') [[unlikely]]
            {
                sprintf(buf, "%s:%s", buf, lastQB != NULL ? lastQB : FindChecksumName(pScript->scriptChecksum));
                break;
            }
            p++;
        }
    }
    _printf(buf);
    _printf("\n");
    return;
}

/*void HookedPrintf(const char* string, ...)
{
va_list args;
va_start(args, string);
add_log(string, args);
}*/

void FixMemoryProtection()
{
    DWORD old;
    VirtualProtect((VOID*)0x0042C18D, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((VOID*)0x0042C1C7, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((VOID*)0x0042C1D5, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((VOID*)0x0042C1E1, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((VOID*)0x0042C1EC, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((VOID*)0x00426088, 1, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((VOID*)0x004260E7, 1, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((VOID*)0x004288F7, 1, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((VOID*)0x00428B97, 1, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((VOID*)0x004288F7, 1, PAGE_EXECUTE_READWRITE, &old);
}


bool bHooked = false;
BYTE oldCustomPrint[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
BYTE oldPrint[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static bool cr = false;

#undef CreateConsole
BOOL CreateConsole()
{
    if (cr)
        return FALSE;
    cr = true;
    int hConHandle = 0;
    HANDLE lStdHandle = 0;
    FILE* fp = 0;

    BOOL ret = AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    freopen("CONERR$", "w", stderr);
    /*lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
    fp = _fdopen(hConHandle, "w");
    *stdout = *fp;
    setvbuf(stdout, NULL, _IONBF, 0);

    lStdHandle = GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
    fp = _fdopen(hConHandle, "r");
    *stdin = *fp;
    setvbuf(stdin, NULL, _IONBF, 0);

    lStdHandle = GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
    fp = _fdopen(hConHandle, "w");
    *stderr = *fp;
    setvbuf(stderr, NULL, _IONBF, 0);*/
    return ret;

}

bool hookedFopen = false;

DWORD HookGeneralFunction(const char* Dll, const char* FuncName, void* Function, unsigned char* backup, int pa)
{
    static BYTE caveFix[6];
    DWORD addr = (DWORD)GetProcAddress(GetModuleHandle(Dll), FuncName);
    if (addr == 0)
    {
        MessageBox(0, "fail", "fail", MB_OK);
        addr = pa - 3;
    }
    BYTE jmp[6] = { 0xe9,			//jmp
        0x00, 0x00, 0x00, 0x00,		//address
        0xc3 };						//retn

    //memcpy(backup, (void*)addr, 6);
    ReadProcessMemory((HANDLE)-1, (void*)addr, backup, 6, 0);
    DWORD calc = ((DWORD)Function - addr - 5);
    memcpy(&jmp[1], &calc, 4);
    //memcpy((void*)addr, &jmp, 6);
    WriteProcessMemory((HANDLE)-1, (void*)addr, jmp, 6, 0);

    return addr;
}



char* strduplow(char* str) {
    char* p;

    str = _strdup(str);
    for (p = str; *p; p++) {
        *p = tolower(*p);
    }
    return(str);
}
DWORD fopenAddr;
BYTE backupFOP[6];
FILE* __cdecl hookfopen(const char* filename, const char* mode)
{
    /*MessageBox(0, filename, mode, MB_OK);
    //char* filename2 = strduplow((char*)filename);
    if (strstr(filename, ".qb"))
        MessageBox(0, "QB FOUND", filename, MB_OK);
    //add_log("fopen(%s,%s)", filename, mode);*/
    WriteProcessMemory((HANDLE)-1, (void*)fopenAddr, backupFOP, 6, 0);
    //MessageBox(0, "REdone", "", MB_OK);
    FILE* ret = fopen(filename, mode);
    fopenAddr = HookGeneralFunction("msvcrt.dll", "fopen", hookfopen, backupFOP, 0);
    return ret;
}
bool HookFopen()
{
    if (!hookedFopen)
    {
        hookedFopen = true;
        fopenAddr = HookGeneralFunction("msvcrt.dll", "fopen", hookfopen, backupFOP, 0);
    }
    char msg[256] = "";
    sprintf(msg, "fopenaddr %x", fopenAddr);
    //MessageBox(0, msg, msg, MB_OK);
    return fopenAddr != 0;
}

#define TIME_TIMED 5000
#define TIME_STEP 5
bool(*IsHosting)(CStruct*, CScript*) = NULL;

struct Message
{
    SOCKET socket;
    const char* data;
    DWORD length;
    IN_ADDR ip;

    Message(SOCKET _socket, const char* _data, DWORD _lenght, IN_ADDR _ip)
    {
        this->socket = _socket;
        this->data = _data;
        this->length = _lenght;
        this->ip = _ip;
    }
};

void SendMessage(Message* msg)
{
    _printf("sending msg!!\n");

    DWORD timeSpent = 0;
    while (timeSpent <= TIME_TIMED)
    {
        if (send(msg->socket, msg->data, msg->length, 0) >= 0)
            break;
        timeSpent += TIME_STEP;
        Sleep(TIME_STEP);
    }
    _printf("disconnecting client: %d.%d.%d.%d!!\n", ((BYTE*)&msg->ip)[0], ((BYTE*)&msg->ip)[1], ((BYTE*)&msg->ip)[2], ((BYTE*)&msg->ip)[3]);

    closesocket(msg->socket);
}


struct StructScript
{
    CStruct* pStruct;
    CScript* pScript;
};

DWORD GetServerAddress()
{
    _printf("getting server address!!\n");

    /*DWORD addr=0x05D0968;
    addr = *(DWORD*)addr+0xB8;
    addr = *(DWORD*)addr+0x10;
    addr = *(DWORD*)addr+0x4;
    addr = *(DWORD*)addr+0x1C;
    return *(DWORD*)addr+0x1F04;*/
    DWORD addr = 0x008E2498;
    addr = *(DWORD*)addr + 0x178;
    addr = *(DWORD*)addr + 0x44;

    return *(DWORD*)addr;
}

void StartedGraf(StructScript* pStructScript)
{
    if (!IsHosting)//Initialize game function pointer
    {
        //MessageBox(0,"IsHosting==NULL","",0);
        QBKeyHeader* header = GetQBKeyHeader(Checksums::OnServer);
        if (header)
            IsHosting = header->pFunction;
    }
    if (IsHosting)//has pointer to game fucntion?
    {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1)//error couldn't create valid socket
        {
            _printf("socket failure\n");
            if (!IsHosting(pStructScript->pStruct, pStructScript->pScript))//not hosting, so limit graf for now
            {
                if (oldLimit != 32)
                {
                    _printf("temporarly limiting graf\n");//MessageBox(0,"limiting graf","",0);

                    CStruct pStruct;//= new CStruct;
                    CStructHeader head;
                    pStruct.head = &head;//new CStructHeader;
                    pStruct.tail = pStruct.head;
                    pStruct.head->Data = 32;
                    pStruct.head->Type = QBKeyHeader::QBKeyType::INT;
                    pStruct.head->QBkey = 0;
                    SetTagLimit(&pStruct, NULL);
                    //delete pStruct.head;
                }
            }
            WSACleanup();
            return;
        }
        /*DWORD addr=0x05D0968;
        addr = *(DWORD*)addr+0xB8;
        addr = *(DWORD*)addr+0x10;
        addr = *(DWORD*)addr+0x4;
        addr = *(DWORD*)addr+0x1C;

        sockaddr_in server;
        server.sin_addr.s_addr =  *(DWORD*)addr+0x1F04;
        server.sin_family = AF_INET;
        server.sin_port = htons( 6500 );*/
        if (IsHosting(pStructScript->pStruct, pStructScript->pScript))//host trying to send info that graf is unlimited
        {
            _printf("hosting!!\n");

            SOCKADDR_IN sin;
            sin.sin_family = AF_INET;
            sin.sin_port = htons(6500);
            sin.sin_addr.s_addr = INADDR_ANY;
            if (bind(serverSocket, (LPSOCKADDR)&sin, sizeof(sin)) < 0)
            {
                _printf("bind error: ");

                char error[256];
                sprintf(error, "%d\n", WSAGetLastError());
                _printf(error);

                closesocket(serverSocket);
                WSACleanup();
                return;
            }

            if (listen(serverSocket, 8) < 0)
            {
                _printf("listen error: ");

                char error[256];
                sprintf(error, "%d\n", WSAGetLastError());
                _printf(error);

                closesocket(serverSocket);
                WSACleanup();
                return;
            }
            _printf("opening connection!!\n");

            FD_SET fdSet;
            //shutdown(serverSocket, SD_RECEIVE);
            if (oldLimit == 32)
            {
                _printf("setting taglimit to 200");

                CStruct pStruct;//= new CStruct;
                CStructHeader head;
                pStruct.head = &head;//new CStructHeader;
                pStruct.tail = pStruct.head;
                pStruct.head->Data = 200;
                pStruct.head->Type = QBKeyHeader::QBKeyType::INT;
                pStruct.head->QBkey = 0;
                SetTagLimit(&pStruct, NULL);
            }
            //MessageBox(0,"your hosting","",0);
            DWORD timeSpent = 0;
            timeval timer;
            timer.tv_sec = 5;
            timer.tv_usec = 0;
            for (DWORD i = 0; i < 8; i++)//while(timeSpent<=TIME_TIMED)
            {
                _printf("checking for connections %u/8\n", i + 1);

                FD_ZERO(&fdSet);
                FD_SET(serverSocket, &fdSet);
                if (select(0, &fdSet, NULL, NULL, &timer) == SOCKET_ERROR)
                {
                    char error[256];
                    sprintf(error, "%d\n", WSAGetLastError());
                    _printf("SelectError: ");
                    _printf(error);

                }
                if (FD_ISSET(serverSocket, &fdSet))
                {
                    _printf("trying to accept connection\n");

                    sockaddr_in clientInfo;
                    SOCKET connectionSocket = ::accept(serverSocket, (sockaddr*)&clientInfo, NULL);
                    if (connectionSocket != INVALID_SOCKET)
                    {
                        _printf("connected to client: %d.%d.%d.%d!!\n", ((BYTE*)&clientInfo.sin_addr)[0], ((BYTE*)&clientInfo.sin_addr)[1], ((BYTE*)&clientInfo.sin_addr)[2], ((BYTE*)&clientInfo.sin_addr)[3]);
                        Message msg(connectionSocket, (const char*)&LevelModSettings::bUnlimitedGraf, 1, clientInfo.sin_addr);
                        HANDLE hRequestThread = ::CreateThread(NULL, 4, (LPTHREAD_START_ROUTINE)SendMessage, (LPVOID)&msg, 0/*CREATE_SUSPENDED*/, NULL);
                    }
                    else
                    {
                        char error[256];
                        sprintf(error, "%d\n", WSAGetLastError());
                        _printf("Error: ");
                        _printf(error);

                    }
                }
                /*timeSpent+=TIME_STEP;
                Sleep(TIME_STEP);*/
            }
            Sleep(TIME_TIMED);
        }
        else//client trying to get info from host, if no info received graf will remain limited
        {
            _printf("client!!\n");

            //shutdown(serverSocket, SD_SEND);
            //MessageBox(0,"your client","",0);
            sockaddr_in server;
            DWORD address = GetServerAddress();
            _printf("server address: %d.%d.%d.%d\n", ((BYTE*)&address)[0], ((BYTE*)&address)[1], ((BYTE*)&address)[2], ((BYTE*)&address)[3]);

            server.sin_addr.s_addr = address;
            server.sin_family = AF_INET;
            server.sin_port = htons(6500);
            bool unlimitedGraf = false;
            DWORD timeSpent = 0;
            for (DWORD i = 0; i < 2; i++)
            {
                int err = 0;
                _printf("trying to connect\n");

                if (err = connect(serverSocket, (sockaddr*)&server, sizeof(sockaddr)) >= 0)
                {
                    _printf("connected!!\n");

                    timeSpent = 0;
                    while (timeSpent <= TIME_TIMED)
                    {
                        if (recv(serverSocket, (char*)&unlimitedGraf, 1, 0) >= 0)
                        {
                            _printf("recieved!!\n");

                            //shutdown(serverSocket, SD_RECEIVE);
                            break;
                        }
                        timeSpent += TIME_STEP;
                        Sleep(TIME_STEP);
                    }
                    break;
                }
                else
                {
                    char error[256];
                    sprintf(error, "%d\n", WSAGetLastError());
                    _printf("Error: ");
                    _printf(error);

                }
                /*timeSpent+=TIME_STEP;
                Sleep(TIME_STEP);*/
            }
            if (unlimitedGraf)
            {
                _printf("unlimiting graf!!\n");

                if (oldLimit == 32)
                {
                    CStruct pStruct;//= new CStruct;
                    CStructHeader head;
                    pStruct.head = &head;//new CStructHeader;
                    pStruct.tail = pStruct.head;
                    pStruct.head->Data = 200;
                    pStruct.head->Type = QBKeyHeader::QBKeyType::INT;
                    pStruct.head->QBkey = 0;
                    SetTagLimit(&pStruct, NULL);
                }
            }
            else if (oldLimit != 32)
            {
                _printf("limiting graf!!\n");

                //MessageBox(0,"limiting graf","",0);
                CStruct pStruct;//= new CStruct;
                CStructHeader head;
                pStruct.head = &head;//new CStructHeader;
                pStruct.tail = pStruct.head;
                pStruct.head->Data = 32;
                pStruct.head->Type = QBKeyHeader::QBKeyType::INT;
                pStruct.head->QBkey = 0;
                SetTagLimit(&pStruct, NULL);
                //delete pStruct.head;
            }
            else
            {
                _printf("graf already limited!!\n");
            }
        }
        _printf("closing connection!!\n");

        closesocket(serverSocket);
        WSACleanup();
    }
    else if (oldLimit != 32)//couldn't initialize game function pointer, so will need to limit graf....
    {
        _printf("NO PTR TO OnServer!!\n");
        CStruct pStruct;//= new CStruct;
        CStructHeader head;
        pStruct.head = &head;//new CStructHeader;
        pStruct.tail = pStruct.head;
        pStruct.head->Data = 32;
        pStruct.head->Type = QBKeyHeader::QBKeyType::INT;
        pStruct.head->QBkey = 0;
        SetTagLimit(&pStruct, NULL);
        //delete pStruct.head;
    }
}

bool GrafStarted(CStruct* pStruct, CScript* pScript)
{
    //MessageBox(0, "GrafStarted", "", 0);
    StructScript structScript;
    structScript.pStruct = pStruct;
    structScript.pScript = pScript;
    if (LevelModSettings::bUnlimitedGraf)
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartedGraf, &structScript, 0, 0);
    //MessageBox(0,"done","done",0);
    return true;
}

DWORD debug = 0;

void ReadFirstOptions()
{
    char temp[MAX_PATH] = "";
    GetCurrentDirectory(MAX_PATH, temp);
    sprintf(IniPath, "%s\\LevelMod.ini", temp);
    //MessageBox(0, IniPath, IniPath, 0);
    OptionWriter = new CIniWriter(IniPath);
    OptionReader = new CIniReader(IniPath);


    //_printf("Reading from ini file %s, default %d ", "LM_GFX_eBuffering", Gfx::numBackBuffers);
    DWORD new_value = OptionReader->ReadInt("Script_Settings", "LM_GFX_eBuffering", Gfx::numBackBuffers);
    if (new_value < 3)
        Gfx::numBackBuffers = new_value;
    //_printf("value %d\n", Gfx::numBackBuffers);

    //_printf("Reading from ini file %s, default %d ", "LM_GFX_eAntiAlias", Gfx::AntiAliasing);
    new_value = OptionReader->ReadInt("Script_Settings", "LM_GFX_eAntiAlias", Gfx::AntiAliasing);
    if (new_value < 5)
        Gfx::AntiAliasing = new_value;
    //_printf("value %d\n", Gfx::AntiAliasing);

    //_printf("Reading from ini file %s, default %d ", "LM_GFX_bFiltering", Gfx::filtering);
    new_value = OptionReader->ReadInt("Script_Settings", "LM_GFX_bFiltering", Gfx::filtering);
    if (new_value < 2)
        Gfx::filtering= new_value;
    //_printf("value %d\n", Gfx::filtering);

    //_printf("Reading from ini file %s, default %d ", "LM_GFX_bFixStutter", Gfx::fps_fix);
    new_value = OptionReader->ReadInt("Script_Settings", "LM_GFX_eFixStutter", Gfx::fps_fix);
    if (new_value < 4)
        Gfx::fps_fix = new_value;

    new_value = OptionReader->ReadInt("Script_Settings", "LM_GFX_eFixStutter", Gfx::bVSync);
    if (new_value < 2)
        Gfx::bVSync = new_value;
    //_printf("value %d\n", Gfx::fps_fix);
    //CreateConsole();

    //_printf("Reading from ini file %s, default %d ", "LM_DebugOption_bDebugMode", debug);
    new_value = OptionReader->ReadInt("Script_Settings", "LM_DebugOption_bDebugMode", debug);
    if (new_value < 2)
       debug = new_value;
    //_printf("value %d\n", debug);
    if (debug)
    {
        CreateConsole();
        _printf("Welcome to DebugMode\n");
    }
}

void FixTagLimitProtections()
{
    DWORD old;
    VirtualProtect((void*)0x004359B7, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004359E1, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435A07, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435A3F, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435A51, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435A85, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435A9D, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435AB2, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435AC2, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435AD2, 4, PAGE_EXECUTE_READWRITE, &old);


    VirtualProtect((void*)0x00435AF7, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435B21, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435B4A, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435B51, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435B5A, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435B7E, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435B8C, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435B9B, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435BBA, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435BD3, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435BE8, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435C29, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435C6B, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435C90, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435C97, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435CBA, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435CFF, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435D2D, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435D50, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435DAC, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435E07, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435E1C, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435E2D, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435E38, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435E67, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435E7C, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435E8C, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00435E9D, 4, PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x0043BA97, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0043BABC, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0043BB07, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0043BB27, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0043BBED, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0043BC0A, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0043BC1A, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0043BC7F, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0043BC94, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0043BCA4, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0043BCB4, 4, PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((void*)0x004BFCD3, 6, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004BFD94, 6, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004BFCEF, 12, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0058C300, 12, PAGE_EXECUTE_READWRITE, &old);

    //251patch
    VirtualProtect((void*)0x004355AD, 1, PAGE_EXECUTE_READWRITE, &old);

    //BW Manual Fix
    VirtualProtect((void*)0x0049D7BE, 11, PAGE_EXECUTE_READWRITE, &old);


    *(DWORD*)0x004BFCD3 = 0x0F39A2E9;
    *(WORD*)0x004BFCD7 = 0x9000;
    *(DWORD*)0x004BFCEF = 0xE9909090;
    *(DWORD*)0x004BFCF3 = 0x000CC615;
    *(DWORD*)0x004BFCF7 = 0x90909090;
    *(DWORD*)0x004BFD94 = 0x0F38F3E9;
    *(WORD*)0x004BFD98 = 0x9000;

    BYTE GrafFix1[] = { 0x85, 0xF6, 0x0F, 0x86, 0x63, 0xC6, 0xF0, 0xFF, 0xBA, 0x3C, 0x03, 0x40, 0x00, 0xE9, 0x4D, 0xC6, 0xF0, 0xFF, 0x89, 0x44, 0x24, 0x10, 0x0F, 0x86, 0x47, 0xC7, 0xF0, 0xFF, 0xBE, 0x3C, 0x03, 0x40, 0x00, 0xE9, 0xFA, 0xC6, 0xF0, 0xFF };

    VirtualProtect((void*)0x005B367A, sizeof(GrafFix1), PAGE_EXECUTE_READWRITE, &old);
    memcpy((void*)0x005B367A, GrafFix1, sizeof(GrafFix1));

    BYTE GrafFix2[] = { 0x89, 0x3D, 0x00, 0xC3, 0x58, 0x00, 0xBF, 0x3C, 0x03, 0x40, 0x00, 0x89, 0x04, 0x97, 0x8B, 0x3D, 0x00, 0xC3, 0x58, 0x00, 0x83, 0xBF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x75, 0x0A, 0xC7, 0x05, 0x06, 0xC3, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x05, 0x06, 0xC3, 0x58, 0x00, 0x83, 0xE2, 0x1F, 0x89, 0x04, 0x97, 0x8B, 0x87, 0x80, 0x00, 0x00, 0x00, 0xE9, 0xB1, 0x39, 0xF3, 0xFF };

    VirtualProtect((void*)0x0058C30C, sizeof(GrafFix2), PAGE_EXECUTE_READWRITE, &old);
    memcpy((void*)0x0058C30C, GrafFix2, sizeof(GrafFix2));

    VirtualProtect((void*)0x004BFD66, 1, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004BFD6A, 1, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004BFC85, 1, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004BFD89, 1, PAGE_EXECUTE_READWRITE, &old);

    *(BYTE*)0x004BFC85 = 200;
    *(BYTE*)0x004BFC89 = 200;
}

void SetTagLimit(DWORD _limit)
{
    DWORD tagLimit = _limit;

    if (tagLimit > 0xFF)
        MessageBox(0, "value should be between 1-255", "Too High tagLimit", 0);

    oldLimit = (BYTE)tagLimit;

    BYTE limit = oldLimit;//Limit the counters
    *(BYTE*)0x004BFD66 = limit;
    *(BYTE*)0x004BFD6A = limit;


    int numExtra = (tagLimit - 32) * 4;//Increase Stack Size by  4*(extra number of tags limited)
    *(DWORD*)0x004359B7 = 0xA0 + numExtra;
    *(DWORD*)0x004359E1 = 0xB8 + numExtra;
    *(DWORD*)0x00435A07 = 0xBC + numExtra;
    *(DWORD*)0x00435A3F = 0xB4 + numExtra;
    *(DWORD*)0x00435A51 = 0x80 + numExtra;
    *(DWORD*)0x00435A85 = 0x80 + numExtra;
    *(DWORD*)0x00435A9D = 0xB4 + numExtra;
    *(DWORD*)0x00435AB2 = 0xB4 + numExtra;
    *(DWORD*)0x00435AC2 = 0xAC + numExtra;
    *(DWORD*)0x00435AD2 = 0xAC + numExtra;

    *(DWORD*)0x00435AF7 = 0x158 + numExtra;
    *(DWORD*)0x00435B21 = 0x178 + numExtra;
    *(DWORD*)0x00435B4A = 0x188 + numExtra;
    *(DWORD*)0x00435B51 = 0x178 + numExtra;
    *(DWORD*)0x00435B5A = 0x170 + numExtra;
    *(DWORD*)0x00435B7E = 0x17C + numExtra;
    *(DWORD*)0x00435B8C = 0x184 + numExtra;
    *(DWORD*)0x00435B9B = 0x188 + numExtra;
    *(DWORD*)0x00435BBA = 0x180 + numExtra;
    *(DWORD*)0x00435BD3 = 0x170 + numExtra;
    *(DWORD*)0x00435BE8 = 0x170 + numExtra;
    //*(DWORD*)0x00435C29 = 0x80 + numExtra;
    //*(DWORD*)0x00435C6B = 0x80 + numExtra;
    *(DWORD*)0x00435C90 = 0x17C + numExtra;
    *(DWORD*)0x00435C97 = 0x180 + numExtra;
    *(DWORD*)0x00435CBA = 0x184 + numExtra;
    //*(DWORD*)0x00435CFF = 0x80 + numExtra;
    *(DWORD*)0x00435D2D = 0x180 + numExtra;
    *(DWORD*)0x00435D50 = 0x184 + numExtra;
    //*(DWORD*)0x00435DAC = 0x80 + numExtra;
    *(DWORD*)0x00435E07 = 0x170 + numExtra;
    *(DWORD*)0x00435E1C = 0x170 + numExtra;
    *(DWORD*)0x00435E2D = 0x180 + numExtra;
    *(DWORD*)0x00435E38 = 0x184 + numExtra;
    *(DWORD*)0x00435E67 = 0x170 + numExtra;
    *(DWORD*)0x00435E7C = 0x170 + numExtra;
    *(DWORD*)0x00435E8C = 0x168 + numExtra;
    *(DWORD*)0x00435E9D = 0x164 + numExtra;

    *(DWORD*)0x0043BA97 = 0x134 + numExtra;
    *(DWORD*)0x0043BABC = 0x150 + numExtra;
    *(DWORD*)0x0043BB07 = 0x150 + numExtra;
    *(DWORD*)0x0043BB27 = 0x154 + numExtra;
    *(DWORD*)0x0043BBED = 0x154 + numExtra;
    *(DWORD*)0x0043BC0A = 0x80 + numExtra;
    *(DWORD*)0x0043BC1A = 0x80 + numExtra;
    *(DWORD*)0x0043BC7F = 0x148 + numExtra;
    *(DWORD*)0x0043BC94 = 0x148 + numExtra;
    *(DWORD*)0x0043BCA4 = 0x140 + numExtra;
    *(DWORD*)0x0043BCB4 = 0x140 + numExtra;
}

//old code
bool SetTagLimit(CStruct* pStruct, CScript* pScript)
{
    DEBUGSTART()
    {
        //MessageBox(0,"inside limit", "", 0);
        int tagLimit = 0;
        /*if(pStruct==NULL)
        tagLimit=32;
        else
        {*/
        if (!pStruct->GetInt(&tagLimit) || tagLimit == 0 || tagLimit > 0xFF)
            return oldLimit == 32;
        //}
        oldLimit = (BYTE)tagLimit;
        BYTE limit = oldLimit;//Limit the counters
        *(BYTE*)0x004BFD66 = limit;
        *(BYTE*)0x004BFD6A = limit;
        /**(BYTE*)0x004BFC85 = limit;
        *(BYTE*)0x004BFC89 = limit;*/
        LevelModSettings::bUnlimitedGraf = (limit != 32);//Set the new settings




        int numExtra = (tagLimit - 32) * 4;//Increase Stack Size by  4*(extra number of tags limited)
        *(DWORD*)0x004359B7 = 0xA0 + numExtra;
        *(DWORD*)0x004359E1 = 0xB8 + numExtra;
        *(DWORD*)0x00435A07 = 0xBC + numExtra;
        *(DWORD*)0x00435A3F = 0xB4 + numExtra;
        *(DWORD*)0x00435A51 = 0x80 + numExtra;
        *(DWORD*)0x00435A85 = 0x80 + numExtra;
        *(DWORD*)0x00435A9D = 0xB4 + numExtra;
        *(DWORD*)0x00435AB2 = 0xB4 + numExtra;
        *(DWORD*)0x00435AC2 = 0xAC + numExtra;
        *(DWORD*)0x00435AD2 = 0xAC + numExtra;


        *(DWORD*)0x00435AF7 = 0x158 + numExtra;
        *(DWORD*)0x00435B21 = 0x178 + numExtra;
        *(DWORD*)0x00435B4A = 0x188 + numExtra;
        *(DWORD*)0x00435B51 = 0x178 + numExtra;
        *(DWORD*)0x00435B5A = 0x170 + numExtra;
        *(DWORD*)0x00435B7E = 0x17C + numExtra;
        *(DWORD*)0x00435B8C = 0x184 + numExtra;
        *(DWORD*)0x00435B9B = 0x188 + numExtra;
        *(DWORD*)0x00435BBA = 0x180 + numExtra;
        *(DWORD*)0x00435BD3 = 0x170 + numExtra;
        *(DWORD*)0x00435BE8 = 0x170 + numExtra;
        *(DWORD*)0x00435C29 = 0x80 + numExtra;
        *(DWORD*)0x00435C6B = 0x80 + numExtra;
        *(DWORD*)0x00435C90 = 0x17C + numExtra;
        *(DWORD*)0x00435C97 = 0x180 + numExtra;
        *(DWORD*)0x00435CBA = 0x184 + numExtra;
        *(DWORD*)0x00435CFF = 0x80 + numExtra;
        *(DWORD*)0x00435D2D = 0x180 + numExtra;
        *(DWORD*)0x00435D50 = 0x184 + numExtra;
        *(DWORD*)0x00435DAC = 0x80 + numExtra;
        *(DWORD*)0x00435E07 = 0x170 + numExtra;
        *(DWORD*)0x00435E1C = 0x170 + numExtra;
        *(DWORD*)0x00435E2D = 0x180 + numExtra;
        *(DWORD*)0x00435E38 = 0x184 + numExtra;
        *(DWORD*)0x00435E67 = 0x170 + numExtra;
        *(DWORD*)0x00435E7C = 0x170 + numExtra;
        *(DWORD*)0x00435E8C = 0x168 + numExtra;
        *(DWORD*)0x00435E9D = 0x164 + numExtra;

        *(DWORD*)0x0043BA97 = 0x134 + numExtra;
        *(DWORD*)0x0043BABC = 0x150 + numExtra;
        *(DWORD*)0x0043BB07 = 0x150 + numExtra;
        *(DWORD*)0x0043BB27 = 0x154 + numExtra;
        *(DWORD*)0x0043BBED = 0x154 + numExtra;
        *(DWORD*)0x0043BC0A = 0x80 + numExtra;
        *(DWORD*)0x0043BC1A = 0x80 + numExtra;
        *(DWORD*)0x0043BC7F = 0x148 + numExtra;
        *(DWORD*)0x0043BC94 = 0x148 + numExtra;
        *(DWORD*)0x0043BCA4 = 0x140 + numExtra;
        *(DWORD*)0x0043BCB4 = 0x140 + numExtra;
        /*FILE*f = fopen("settings","w+b");
        if(f)
        {
        fwrite(&LevelModSettings::UseNewMenu, 1, 1, f);
        fwrite(&LevelModSettings::FixSound, 1, 1, f);
        fwrite(&LevelModSettings::TeleFix, 1, 1, f);
        fwrite(&oldLimit, 1, 1, f);
        bool disableGrass = false;
        QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"GrassLayersDisabled"));
        if(header)
        disableGrass = header->value.i;
        fwrite(&disableGrass,1,1,f);
        fclose(f);
        }*/
    }
    DEBUGEND()
        return true;
}

bool AddToGlobal(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* header = pStruct->head;
    int qbKey = 0;
    int value = 0;
    while (header)
    {
        if (header->QBkey == 0xA1DC81F9)
        {
            qbKey = header->GetInt();
            if (value)
                break;
        }
        if (header->QBkey == 0xE288A7CB)
        {
            value = header->GetInt();
            if (qbKey)
                break;
        }
        header = header->NextHeader;
    }
    int oldValue;
    header = pScript->params->head;
    while (header)
    {
        if (header->QBkey == qbKey)
        {
            oldValue = header->GetInt();
            break;
        }
        header = header->NextHeader;
    }
    header = pScript->params->head;
    while (header)
    {
        if (header->QBkey == 0x34406F14)
        {
            header->value.i = oldValue + value;
            return true;
        }
        header = header->NextHeader;
    }
    CStruct* const __restrict params = pScript->params;
    CStructHeader* const __restrict pHeader = params->AllocateCStruct();
    if (!pHeader)
        return false;
    params->tail->NextHeader = pHeader;
    params->tail = pHeader;
    pHeader->Type = QBKeyHeader::LOCAL;
    pHeader->QBkey = 0x34406F14;
    pHeader->value.i = oldValue + value;
    pHeader->NextHeader = NULL;
    return true;
}

bool SubToGlobal(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* header = pStruct->head;
    int qbKey = 0;
    int value = 0;
    while (header)
    {
        if (header->QBkey == 0xA1DC81F9)
        {
            qbKey = header->GetInt();
            if (value)
                break;
        }
        if (header->QBkey == 0xE288A7CB)
        {
            value = header->GetInt();
            if (qbKey)
                break;
        }
        header = header->NextHeader;
    }
    int oldValue;
    header = pScript->params->head;
    while (header)
    {
        if (header->QBkey == qbKey)
        {
            oldValue = header->GetInt();
            break;
        }
        header = header->NextHeader;
    }
    header = pScript->params->head;
    while (header)
    {
        if (header->QBkey == 0x34406F14)
        {
            header->value.i = oldValue - value;
            return true;
        }
        header = header->NextHeader;
    }
    CStruct* const __restrict params = pScript->params;
    CStructHeader* const __restrict pHeader = params->AllocateCStruct();
    if (!pHeader)
        return false;
    params->tail->NextHeader = pHeader;
    params->tail = pHeader;
    pHeader->Type = QBKeyHeader::LOCAL;
    pHeader->QBkey = 0x34406F14;
    pHeader->value.i = oldValue + value;
    pHeader->NextHeader = NULL;
    return true;
}

bool FreezeCamera(CStruct* pStruct, CScript* pScript)
{
    const static DWORD pFreeze = 0x004A7CAC;
    *(BYTE*)pFreeze = 0xEB;
    return true;
}

bool UnfreezeCamera(CStruct* pStruct, CScript* pScript)
{
    const static DWORD pFreeze = 0x004A7CAC;
    *(BYTE*)pFreeze = 0x74;
    return true;
}

bool FileExistsScript(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* header = pStruct->head;

    if (header)
    {
        static char dir[256] = "data\\";
        DWORD len = strlen(header->pStr) + 1;
        memcpy(&dir[5], header->pStr, len);

        FILE* f = fopen(dir, "rb");
        if (f)
        {
            fclose(f);
            return true;
        }
        return false;
    }
    return true;
}

bool AddParamScript(CStruct* pStruct, CScript* pScript)
{
    return pScript->AddCStruct(pStruct->head);
}

bool NotScript(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* header = pStruct->head;
    while (header)
    {
        if (header->Type == QBKeyHeader::LOCAL)
        {
            QBKeyHeader* Header = GetQBKeyHeader(header->Data);
            if (Header && Header->type == QBKeyHeader::COMPILED_FUNCTION)
            {
                CStruct params;
                params.head = header->NextHeader;
                params.tail = pStruct->tail;
                if (params.head == NULL)
                    params.tail = NULL;
                /*pStruct->head = header->NextHeader;
                if(pStruct->head==NULL)
                pStruct->tail=NULL;*/
                return !Header->CallScript(&params, pScript);
            }
            break;
        }
        header = header->NextHeader;
    }
    return true;
}

bool UpdateSpineText(CStruct* pStruct, CScript* pScript)
{
    int id = 0;

    typedef Element* (__cdecl* const pAllocateElement)(DWORD unk);
    static const pAllocateElement AllocateElement = (pAllocateElement)(0x004D12A0);
    typedef void(__cdecl* const pFreeElement)();
    static const pFreeElement FreeElement = (pFreeElement)(0x004D12F0);
    typedef void* (__cdecl* const pCastPointer)(void* pointer, LONG VfDelta, DWORD SrcType, DWORD TargetType, BOOL isReference);
    static const pCastPointer CastPointer = (pCastPointer)(*(DWORD*)0x0058D150);// 0x00577E58);

    if (pStruct->GetScript("menu_id", &id))
    {
        Element* container = AllocateElement(0);
        Element* element = container->GetElement(id);
        //element = (Element*)CastPointer((void*)element, 0, 0x005B6344, 0x005B666C, FALSE);

        FreeElement();
    }
    return true;
}

bool ChangeString(CStruct* pStruct, CScript* pScript)
{
    //MessageBox(0, "ChangeStr", "", 0);
    int s1 = 0;
    pStruct->GetScript("string", &s1);
    QBKeyHeader* header = GetQBKeyHeader(s1);
    const char* s2 = NULL;
    CStructHeader* pParam = NULL;
    if (pStruct->GetStruct(Checksums::param, &pParam))
    {
        if (pScript->params->GetStruct(pParam->Data, &pParam))
        {
            freex(header->pStr);
            header->pStr = (char*)mallocx(strlen(pParam->pStr) + 1);
            memcpy(header->pStr, s2, strlen(s2) + 1);
        }
    }
    else
    {
        pStruct->GetString("value", &s2);
        freex(header->pStr);
        header->pStr = (char*)mallocx(strlen(s2) + 1);
        memcpy(header->pStr, s2, strlen(s2) + 1);
    }
    return true;
}

bool strstr(CStruct* pStruct, CScript* pScript)
{

    const char* s1;
    pStruct->GetString("s1", &s1);

    const char* s2;
    pStruct->GetString("s2", &s2);

    return strstr(s1, s2);
}


void RemoveMovingObject(SuperSector* sector)
{
    for (DWORD i = 0; i < movingObjects.size(); i++)
    {
        if (movingObjects[i].sector == sector)
        {
            movingObjects.erase(movingObjects.begin() + i);
            return;
        }
    }
}


bool doneIt = false;

/*bool ChangeParamToUnnamedScript(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* pParam = NULL;
    if (pStruct->GetStruct(Checksums::param, &pParam))
    {
        if (pScript->params->GetStruct(pParam->Data, &pParam))
        {
            pParam->QBkey = 0;
            return true;
        }
    }
    return false;
}*/

char funcName[100] = "";
CStruct* funcParam;

void ExecuteQBThread()
{
    ExecuteQBScript(funcName, funcParam);
    funcParam->DellocateCStruct();
    delete[] funcParam;
}


EXTERN bool QBKeyHeader::SetFloat(DWORD checksum, float value)
{
    _printf("Seting Name %s\n", FindChecksumName(checksum));
    if (this->type == QBKeyHeader::STRUCT || this->type == QBKeyHeader::LOCAL_STRUCT)
    {
        CStructHeader* pStruct = *(CStructHeader**)this->pStruct;

        while (pStruct)
        {
            if (pStruct->Type == QBKeyHeader::STRUCT || pStruct->Type == QBKeyHeader::LOCAL_STRUCT)
            {
                if ((*(CStructHeader**)pStruct->pStruct)->SetFloat(checksum, value))
                    return true;
            }
            else
                if (pStruct->QBkey == checksum)
                {
                    _printf("FloatVal %s(%f)\n", FindChecksumName(pStruct->QBkey), value);
                    pStruct->value.f = value;
                    return true;
                }
            pStruct = pStruct->NextHeader;
        }
    }

    return false;
}


bool ChangeParamToUnnamedScript(CStruct* pStruct, CScript* pScript)
{
    _printf("UnnamedScript ");
    CStructHeader* pFunc = NULL;
    if (pStruct->GetStruct(Checksums::FUNCTION, &pFunc))
    {
        //MessageBox(0, "Got Function", pFunc->pStr, 0);
        CStructHeader* pParam = NULL;
        if (pStruct->GetStruct(Checksums::param, &pParam))
        {
            if (pScript->params->GetStruct(pParam->Data, &pParam))
            {
                //MessageBox(0, "Got Param", pParam->pStr, 0);
                /*CStructHeader header;
                header.Type = QBKeyHeader::STRING;
                header.pStr = pParam->pStr;*/
                CStruct* pStructParam = new CStruct();
                pStructParam->AddParam(0, QBKeyHeader::STRING, pParam->pStr);
                /*pStruct.head = &header;
                pStruct.tail = &header;*/
                _printf(pFunc->pStr);
                _printf("\n");
                memcpy(funcName, pFunc->pStr, strlen(pFunc->pStr) + 1);
                funcParam = pStructParam;
                CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ExecuteQBThread, 0, 0, 0);

                return true;
            }
        }
    }
    _printf("couldn't find function\n");
    return false;
}

bool CallWithNoNameScript(CStruct* pStruct, CScript* pScript)
{
    MessageBox(0, "trying script", "", 0);
    CStructHeader* pFunc = NULL;
    if (pStruct->GetStruct(Checksums::FUNCTION, &pFunc))
    {
        if (pStruct->ContainsFlag("String"))
        {
            CStruct pStruct;
            QBKeyHeader* header = GetQBKeyHeader(Checksum("UnnamedString"));
            pStruct.AddParam(0, QBKeyHeader::STRING, header->pStr);
            ExecuteQBScript(pFunc->pStr, &pStruct);
            return true;
        }
        else if (pStruct->ContainsFlag("Int"))
        {
            CStruct pStruct;
            QBKeyHeader* header = GetQBKeyHeader(Checksum("UnnamedInt"));
            pStruct.AddParam(0, QBKeyHeader::INT, header->pStr);
            return true;
        }

        _printf("Invalid call to %s\nNeed param String|Int\n", __FUNCTION__);
        return false;
    }
    else
        MessageBox(0, "couldn't find function", "", 0);
    return false;
}

bool KillMovingObjectScript(CStruct* pStruct, CScript* pScript)
{
    for (DWORD i = 0; i < movingObjects.size(); i++)
    {
        if (movingObjects[i].pScript->GetNodeName() == pScript->GetNodeName())
        {
            movingObjects[i].Kill();
        }
    }
    return true;
}

static bool printedDebug = false;

bool MoveObjectScript(CStruct* pStruct, CScript* pScript)
{
    /*DEBUGSTART()
    {*/
    int name = 0;
    _printf("Maybe Move pScript %p CalledFrom %p\n", pScript, pScript->address);
    if (pStruct->GetScript("Name", &name))
    {
        SuperSector* sector = SuperSector::GetSuperSector(name);
        if (sector)
        {
            _printf("Got Name %X\n", name);
            CStructHeader* param = NULL;

            if (pStruct->GetStruct(Checksums::Type, &param))
            {
                _printf("Got Type: ");
                switch (param->value.i)
                {
                case Checksums::ANGULAR_VELOCITY:
                    _printf("ANGULAR_VELOCITY\n");
                    if (pStruct->GetStruct(Checksums::ORIENT, &param))
                    {
                        for (DWORD i = 0; i < movingObjects.size(); i++)
                        {
                            if (movingObjects[i].sector == sector)
                            {
                                _printf("ORIENT\n");
                                D3DXVECTOR3 angle = *param->pVec * D3DX_PI / 2048.0f;
                                //angle.y += D3DX_PI;
                                movingObjects[i].goalAngle = angle;
                                return true;
                            }
                        }
                        movingObjects.push_back(MovingObject(sector, *param->pVec, pScript, name));
                        MovingObject& obj = movingObjects.back();
                        obj.vertices = new D3DXVECTOR3[obj.sector->numVertices];
                        //std::copy(obj.sector->vertices, obj.sector->vertices + obj.sector->numVertices, obj.vertices);
                        D3DXVECTOR3 position = (sector->bboxMax + sector->bboxMin) / 2.0f;
                        for (DWORD i = 0; i < sector->numVertices; i++)
                        {
                            obj.vertices[i] = sector->vertices[i] - position;
                        }
                        return true;
                    }
                    else
                        if (pStruct->GetStruct(Checksums::ANGULAR_VELOCITY, &param))
                        {
                            for (DWORD i = 0; i < movingObjects.size(); i++)
                            {
                                if (movingObjects[i].sector == sector)
                                {
                                    _printf("updating sector\n");
                                    D3DXVECTOR3 angle = *param->pVec * D3DX_PI / 2048.0f;
                                    //angle += D3DX_PI;
                                    movingObjects[i].acceleration = angle;
                                    return true;
                                }
                            }

                            movingObjects.push_back(MovingObject(sector, *param->pVec, pScript, name));
                            MovingObject& obj = movingObjects.back();
                            obj.vertices = new D3DXVECTOR3[obj.sector->numVertices];
                            //std::copy(obj.sector->vertices, obj.sector->vertices + obj.sector->numVertices, obj.vertices);
                            D3DXVECTOR3 position = (sector->bboxMax + sector->bboxMin) / 2.0f;
                            for (DWORD i = 0; i < obj.sector->numVertices; i++)
                            {
                                obj.vertices[i] = obj.sector->vertices[i] - position;
                            }
                            return true;
                        }
                    _printf("Need to pass param 'ANGULAR_VELOCITY'\n");
                    return false;
                case Checksums::MOVE_TO_POS:
                    _printf("MOVE_TO_POS\n");
                    if (pStruct->GetStruct(Checksums::Relpos, &param))
                    {
                        _printf("Going to mov Object -> Relpos %f %f %f pScript %p\n", param->pVec->x, param->pVec->y, param->pVec->z, pScript);


                        for (DWORD i = 0; i < movingObjects.size(); i++)
                        {
                            if (movingObjects[i].sector == sector)
                                return false;
                        }

                        D3DXVECTOR3 position = (sector->bboxMax + sector->bboxMin) / 2.0f;
                        position += *param->pVec;
                        movingObjects.push_back(MovingObject(sector, position, pScript));
                        /*MovingObject& obj = movingObjects.back();
                        obj.vertices = new D3DXVECTOR3[obj.sector->numVertices];
                        //std::copy(obj.sector->vertices, obj.sector->vertices + obj.sector->numVertices, obj.vertices);
                        for (DWORD i = 0; i < obj.sector->numVertices; i++)
                        {
                            obj.vertices[i] = obj.sector->vertices[i] - position;
                        }*/
                        return true;
                    }
                    _printf("Need to pass param 'Relpos'\n");
                    return false;
                default:
                    _printf("Defaulting to FOLLOW_PATH_LINKED\n");
                    for (DWORD i = 0; i < movingObjects.size(); i++)
                    {
                        if (movingObjects[i].sector == sector)
                            return false;
                    }
                    _printf("Going to mov Object -> FollowPathLinked pScript %p\n", pScript);

                    CArray* links = pScript->node->GetNodeStruct()->GetArray(Checksums::Links);
                    if (links)
                    {
                        _printf("Links %p", links);
                        D3DXVECTOR3 position = (sector->bboxMax + sector->bboxMin) / 2.0f;
                        CStructHeader* pos = NULL;
                        CStructHeader* link = NULL;
                        if (links->GetNumItems() == 1)
                            link = Node::GetNodeStructByIndex((*links)[0]);
                        else
                            link = Node::GetNodeStructByIndex((*links)[Rnd(link->GetNumItems())]);//Pick a random path
                        if (link)
                        {
                            if (link->GetStruct(Checksums::Position, &pos))
                            {
                                //position += *pos->pVec;
                                movingObjects.push_back(MovingObject(sector, position, *pos->pVec, pScript, link));
                                MovingObject& obj = movingObjects.back();
                                obj.vertices = new D3DXVECTOR3[obj.sector->numVertices];
                                //std::copy(obj.sector->vertices, obj.sector->vertices + obj.sector->numVertices, obj.vertices);
                                for (DWORD i = 0; i < obj.sector->numVertices; i++)
                                {
                                    obj.vertices[i] = obj.sector->vertices[i] - position;
                                }
                                return true;
                            }
                            _printf("couldn't find node->position\n");
                            return false;
                        }
                        _printf("couldn't find link in NodeArray\n");
                        return false;
                    }
                    _printf("Couldn't find node->links\n");
                    return false;
                }
            }
            else
            {

                /*if (!printedDebug)
                {
                    printedDebug = true;
                    FILE* f = fopen("QBKeys.tbl", "w+b");
                    for (auto i = 0; i<qbKeys.size(); i++)
                    {
                        auto it = qbTable->qbTable.find(qbKeys[i]);
                        fwrite(&it->first, 4, 1, f);
                        fwrite(it->second, strlen(it->second) + 1, 1, f);
                    }
                    fclose(f);

                 }*/
                _printf("No Type, defaulting to FOLLOW_PATH_LINKED\n");
                for (DWORD i = 0; i < movingObjects.size(); i++)
                {
                    _printf("What's going on?[%d][%d] sector %p\n", i, movingObjects.size(), movingObjects[i].sector);
                    if (movingObjects[i].sector == sector)
                    {
                        _printf("returning false..\n");
                        return false;
                    }
                }
                _printf("Going to mov Object %X -> FollowPathLinked Node %X \n", name, pScript->node->name);

                CArray* links = pScript->node->GetNodeStruct()->GetArray(Checksums::Links);
                if (links)
                {
                    _printf("Links %p\n", links);
                    D3DXVECTOR3 position = (sector->bboxMax + sector->bboxMin) / 2.0f;
                    CStructHeader* pos = NULL;
                    CStructHeader* link = Node::GetNodeStructByIndex((*links)[0]);
                    if (link->GetStruct(Checksums::Position, &pos))
                    {
                        _printf("position %f %f %f pos %f %f %f\n", position.x, position.y, position.z, pos->pVec->x, pos->pVec->y, pos->pVec->z);
                        //position += *pos->pVec;
                        movingObjects.push_back(MovingObject(sector, position, *pos->pVec, pScript, link));
                        MovingObject& obj = movingObjects.back();
                        obj.vertices = new D3DXVECTOR3[obj.sector->numVertices];
                        //std::copy(obj.sector->vertices, obj.sector->vertices + obj.sector->numVertices, obj.vertices);
                        for (DWORD i = 0; i < obj.sector->numVertices; i++)
                        {
                            obj.vertices[i] = obj.sector->vertices[i] - position;
                        }
                        return true;
                    }
                    _printf("couldn't find node->position\n");
                    return false;
                }
                _printf("Couldn't find node->links\n");
            }
        }
        else
            _printf("Couldn't find SuperSector(0x%X)", name);
    }
    /*}
    DEBUGEND()*/
    return false;
}
bool Initialize(CStruct*, CScript*);
const CompiledScript scripts[] =
{
    /*{"TestFunction",         TestFunc},
    {"Math",         Math},
    {"String",       String},
    {"GenChecksum",       GenChecksum},
    {"CallFuncByName",    CallScriptByName},
    {"CopyString", ScriptCopyString},
    {"ClearObserverState", ClearObserverState},*/
    {"ChangeString", ChangeString},
    {"strstr", strstr},
    { "GetZAngle", GetZAngle },
    { "GetSpeed", GetSpeedScript },
    { "TestForAcid", TestForAcid },
    {"WallplantTimeGreaterThan", WallplantTimeGreaterThan},
    { "EnterObserveMode2", EnterObserveMode },
    { "LeaveObserveMode2", LeaveObserveMode },
    { "ObserveNext", ObserveNext },
    { "GetSkaterPos", GetSkaterPos },
    { "StoreSkaterPos", StoreSkaterPos },
    { "GotoStoredPos", SetSkaterPos },
    { "GetSkaterLook", GetSkaterLook },
    { "Not", NotScript },
    { "IsNot", NotScript },
    { "NotTrue", NotScript},
    { "FileExists", FileExistsScript },
    { "AddParam", AddParamScript },
    { "SubToGlobal", SubToGlobal },
    { "AddToGlobal", AddToGlobal },
    { "FreezeCamera", FreezeCamera },
    { "UnfreezeCamera", UnfreezeCamera },
    //{ "GrafStarted", GrafStarted },
    { "ChangeValues", ChangeValues },
    { "CreatePair", CreatePair },
    { "GetSliderValue", GetSliderValue },
    { "ToggleWindowed", ToggleWindowedScript },
    { "RestoreGoBack", RestoreGoBackScript },
    { "FixGoback", FixGoBackScript },
    { "GetMaximumIndex", GetMaximumIndexScript },
    { "GetOptionValue", GetOptionValue},
    { "LaunchGFXCommand", LaunchGFXCommand },
    { "SetSliderText", SetSliderArrayText},
    { "InitLevelMod", Initialize },
    { "MoveObject", MoveObjectScript   },
{ "KillMovingObject", KillMovingObjectScript},
{"ChangeParamToUnnamed", ChangeParamToUnnamedScript},
{"IsOptionOn", IsOptionOn},
{"IsOptionOverriden", IsOptionOverriden},
{"IsOptionOff", IsOptionOff},
{"AddOption", AddOption},
{"ToggleOption", ToggleOption},
{"ToggleHostOption", ToggleHostOption},
    {"LM_GotParam", LM_GotParamScript },
    {"GetParam", GetParamScript},
    {"SetOption", SetOption},
    {"GetOptionText", GetOptionText},
    {"LM_PrintInfo", GetInfoScript },
    {"TestReloadQB", TestReloadQB},
    {"OnPostLevelLoad", OnPostLevelLoad},
    {"Change_Local", ChangeLocalScript},
{ "NewShatter", NewShatterScript},
    {"AddLights", AddLights},
    {"RemoveLights", RemoveLights},
/*{"SetMemoryPoolSize", SetMemoryPoolSize},
{"GetMemoryPoolSize", GetMemoryPoolSize},
{"GetMemoryPoolSizeText", GetMemoryPoolSizeText},*/
//{"GetMotd", GetMotd},
};



std::vector<std::string> uninstalled;


void ShowMessage(char* msg, DWORD numFrames)
{
    sprintf(download_message, msg);
    showmessage = numFrames;
}

void PrintDownloadString(char* string)
{
    //return;
    /*QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"download_str"));

    if (header->pStr)
    freex(header->pStr);
    const DWORD lens = strlen(string) + 1;
    header->pStr = (char*)mallocx(lens);
    for (DWORD i = 0; i < lens; i++)
    {
    header->pStr[i] = string[i];
    }*/

    /*CStruct params;
    CStructHeader header;*/
    strncpy(download_message, string, strlen(string) + 1);
    //SetElementText(Checksum("helper_menu_1"), download_message);/**/



    /*header.Type = QBKeyHeader::STRING;
    header.QBkey = crc32f((unsigned char*)"text");
    header.pStr = string;
    params.head = &header;
    params.tail = &header;

    CStructHeader* param = params.AddParam("text", QBKeyHeader::STRING);
    param->pStr = string;

    ExecuteQBScript("SetHelperText", &params);
    /*params.DellocateCStruct();*/
}

unsigned long NumReadable(SOCKET sock) {
    unsigned long n = -1;
    if (ioctlsocket(sock, FIONREAD, &n) < 0) {
        /* look in WSAGetLastError() for the error code */
        return 0;
    }
    return n;
}


bool DownloadAndInstall(std::string& path, float build, SOCKADDR* service)
{
    //FILE* f = fopen(path.c_str(), "wb+");
    sprintf(download_message, "Dling: %s 0%% Version: %.1f", path.c_str(), build);

    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == INVALID_SOCKET)
    {
        PrintDownloadString((char*)"Invalid Socket");
        return false;
    }

    u_long iMode = 0;//Set blocking mode
    ioctlsocket(fd, FIONBIO, &iMode);

    int retry = 0;

    while (connect(fd, service, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
    {
        if (retry == 5)
        {
            PrintDownloadString((char*)"can't connect");
            closesocket(fd);
            return false;
        }

        else
        {
            retry++;
        }
    }
    string request = "GET /";
    request.append(path);
    request += " HTTP/1.1\r\n"
        "Referer: http://levelmod.freetzi.com/\r\n"
        "Accept: */*\r\n"
        "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.1; WOW64; Trident/7.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; .NET4.0C; .NET4.0E; Zoom 3.6.0)\r\n"
        "Host: levelmod.freetzi.com\r\n"
        "Cache-Control: no-cache\r\n"
        "\r\n";

    if (send(fd, request.c_str(), request.size(), 0) == SOCKET_ERROR)
    {
        PrintDownloadString((char*)"request timeout?");
        request.clear();
        closesocket(fd);
        return false;
    }

    request.clear();
    std::string response;

    float percent = 0.0f;
    DWORD size = MAXRECV * 1024;

    response.reserve(MAXRECV * 1024);
    char buffer[MAXRECV];
    int resp_leng = recv(fd, (char*)&buffer, MAXRECV, 0);
    if (resp_leng > 0)
    {
        response.append(buffer, resp_leng);
    }
    else
    {
        sprintf(download_message, "ERROR: %d", WSAGetLastError());
    }

    int length = response.find("Content-Length");// +16;

    if (length == string::npos)
    {
        PrintDownloadString((char*)"can't find Content-Length");
        request.clear();
        closesocket(fd);
        return false;
    }
    length += 16;

    sscanf(&response.c_str()[length], "%d\r\n", &size);

    int pos = response.find("\r\n\r\n");

    if (pos == string::npos)
    {
        PrintDownloadString((char*)"Can't find End of header");
        request.clear();
        closesocket(fd);
        return false;
    }

    pos += 4;
    std::string head = response;
    response.erase(0, pos);
    if (size > response.size())
        response.reserve(size);
    //fwrite(&response.begin(), response.size(), 1, f);

    DWORD readBytes = response.size();
    percent = (float)readBytes / (float)size * 100.0f;

    sprintf(&download_message[7], "%s %.2f%% Version: %.1f", path.c_str(), percent, build);

    while (resp_leng > 0)
    {
        resp_leng = recv(fd, (char*)&buffer, MAXRECV, 0);
        if (resp_leng > 0)
        {
            response.append(buffer, resp_leng);
            readBytes += resp_leng;
            percent = (float)readBytes / (float)size * (float)100.0f;
            sprintf(&download_message[7], "%s %.2f%% Version: %.1f", path.c_str(), percent, build);
        }
        else
        {
            sprintf(download_message, "ERROR: %d", WSAGetLastError());
        }
    }
    PrintDownloadString((char*)"Finished Downloading");
    //MessageBox(0, "Really finished?", "", 0);
    //fclose(f);

    while (readBytes < size)
    {
        PrintDownloadString((char*)"not ok");
        sprintf(download_message, "%d/%d bytes downloaded going to retry", readBytes, size);

        char requestString[256] = "";
        sprintf(requestString, "HTTP / 1.1\r\n"
            "Host: levelmod.freetzi.com\r\n"
            "Range: bytes=%d-%d\r\n"
            "Accept-Language: en-US\r\n"
            "Accept-Encoding: gzip, deflate\r\n"
            "User-Agent: Mozilla FireFox/4.0\r\n"
            //"Connection: Keep-Alive"
            "\r\n", readBytes, size);

        request = "GET /";
        request.append(path);
        request += requestString;

        if (send(fd, &request[0], request.size(), 0) == SOCKET_ERROR)
        {
            closesocket(fd);
            fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (fd == INVALID_SOCKET)
            {
                PrintDownloadString((char*)"Invalid Socket");
                return false;
            }

            u_long iMode = 0;//Set blocking mode
            ioctlsocket(fd, FIONBIO, &iMode);

            retry = 0;

            while (connect(fd, service, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
            {
                if (retry == 5)
                {
                    PrintDownloadString((char*)"can't connect");
                    closesocket(fd);
                    return false;
                }

                else
                {
                    retry++;
                }
            }
        }
        request.clear();
        DWORD oldSize = response.size();

        resp_leng = recv(fd, (char*)&buffer, 1024, 0);
        if (resp_leng > 0)
            response.append(buffer, buffer + resp_leng);

        pos = response.find("\r\n\r\n");
        while (pos == string::npos && resp_leng > 0)
        {
            resp_leng = recv(fd, (char*)&buffer, 1024, 0);
            if (resp_leng > 0)
                response.append(buffer, buffer + resp_leng);
            pos = response.find("\r\n\r\n");
        }
        if (pos == string::npos)
        {
            PrintDownloadString((char*)"not found2?");
            request.clear();
            closesocket(fd);
            return false;
        }
        pos += 4;
        readBytes += response.size() - oldSize - pos;
        response.erase(response.begin() + oldSize, response.end());

        //addedBytes = 0;
        percent = (float)readBytes / (float)size * 100.0f;
        sprintf(msg, "Dling: %s %.2f%%", path.c_str(), percent);
        PrintDownloadString(msg);

        while (resp_leng > 0)
        {
            resp_leng = recv(fd, (char*)&buffer, MAXRECV, 0);
            if (resp_leng > 0)
                response.append(buffer, buffer + resp_leng);
            readBytes += resp_leng;
            percent = (float)(readBytes) / (float)size * 100.0f;
            sprintf(msg, "Dling: %s %.2f%%", path.c_str(), percent);
            PrintDownloadString(msg);
        }
    }

    closesocket(fd);

    if (head.find("404 Not Found") == string::npos)
    {
        FILE* f = fopen(path.c_str(), "w+b");
        fwrite(&response[0], response.size(), 1, f);
        fclose(f);
        char installPath[MAX_PATH + 1];
        GetCurrentDirectory(MAX_PATH, installPath);
        char final[MAX_PATH * 2 + 100];
        sprintf(final, "%s/LevelMod-Installer.exe", installPath);
        _printf("%s", final);
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        // set the size of the structures
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        char cmdArgs[256] = "";
        sprintf(cmdArgs, "LevelMod-Installer.exe %s", path.c_str());


        // start the program up
        CreateProcess(final,   // the path
            cmdArgs,        // Command line
            NULL,           // Process handle not inheritable
            NULL,           // Thread handle not inheritable
            FALSE,          // Set handle inheritance to FALSE
            0,              // No creation flags
            NULL,           // Use parent's environment block
            NULL,           // Use parent's starting directory 
            &si,            // Pointer to STARTUPINFO structure
            &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
        );
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        CStruct params;
        ExecuteQBScript("MenuQuitGame", &params);
    }
    else
    {
        PrintDownloadString((char*)"404");
        uninstalled.clear();
        response.clear();
        return false;
    }
    //response.clear();
    uninstalled.clear();
    return uninstalled.size() == 0;
}



void DownloadAndInstall(char* address, char* filename)
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return;
    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == INVALID_SOCKET)
    {
        WSACleanup();
        return;
    }
    SOCKADDR_IN service;
    service.sin_family = AF_INET;
    service.sin_port = htons(80);
    LPHOSTENT host = gethostbyname(address);
    if (!host)
    {

        PrintDownloadString((char*)"no host");
        closesocket(fd);
        WSACleanup();
        return;
    }
    service.sin_addr = *((LPIN_ADDR)*host->h_addr_list);
    //DownloadAndInstall(filename, (SOCKADDR*)&service);
}

void GetLevelModVersion()
{
    bDownloading = true;
    CStruct params;
    ExecuteQBScript("ShowDownloadInfo", &params);
    ExecuteQBScript("SetHelperPos", &params);
    /*ChilkatUnlock();
    ChilkatCheckVersion();*/


    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return;
    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == INVALID_SOCKET)
    {
        WSACleanup();
        return;
    }
    u_long iMode = 0;
    ioctlsocket(fd, FIONBIO, &iMode);
    SOCKADDR_IN service;
    service.sin_family = AF_INET;
    service.sin_port = htons(80);
    LPHOSTENT host = gethostbyname("levelmod.freetzi.com");
    if (!host)
    {

        PrintDownloadString((char*)"no host");
        closesocket(fd);
        WSACleanup();
        return;
    }
    service.sin_addr = *((LPIN_ADDR)*host->h_addr_list);
    if (connect(fd, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {

        PrintDownloadString((char*)"not connected");
        closesocket(fd);
        WSACleanup();
        return;
    }
    PrintDownloadString((char*)"Checking version...");
    static const char* GetStr = "GET /Version.txt HTTP/1.1\r\n"
        "Host: levelmod.freetzi.com\r\n"
        "User-Agent: Mozilla FireFox/4.0\r\n" //"User-Agent: Mozilla / 5.0 (Windows NT 6.1; WOW64; rv:36.0) Gecko / 20100101 Firefox / 36.0\r\n"
        "Accept: text/html, application/xhtml+xml, *//**\r\n"
        "Accept-Language: en-gb,en;q=0.5\r\n"
        "Connection: Keep-Alive\r\n"
        "Accept-Encoding: gzip, deflate\r\n\r\n"
        /*"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*//*;q=0.8\r\n"
        "Accept-Language: en-gb,en;q=0.5\r\n"
        "Accept-Encoding: gzip,deflate\r\n"
        "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n\r\n"*/;

    if (send(fd, GetStr, strlen(GetStr) + 1, 0) == SOCKET_ERROR)
    {

        closesocket(fd);
        WSACleanup();
        PrintDownloadString((char*)"error");
        return;
    }
    std::string response;// = "";
    int resp_leng = 1024;
    char buffer[1024];
    while (resp_leng > 0)
    {
        resp_leng = recv(fd, (char*)&buffer, 1024, 0);
        if (resp_leng > 0)
            response += std::string(buffer).substr(0, resp_leng);
    }

    if (strstr(response.c_str(), "404 Not Found"))
    {

        response.clear();

        WSACleanup();
        PrintDownloadString((char*)"404");
        return;
    }
    else
    {
        int pos = response.find("\r\n\r\n");
        pos += 4;
        float latest = 0.0f;
        float build = 0.0f;

        char buildpath[128] = "";
        char buildname[128] = "";

        std::istringstream f(&response.c_str()[pos]);
        std::string line;// , line2;
        std::getline(f, line);

        sscanf(line.c_str(), "[%f]", &build);
        std::getline(f, line);
        bool asked = false;
        //MessageBox(0, "dling", "", 0);
        bool sucess = false;
        if (build > version)
        {
            PrintDownloadString((char*)"Found new Version");

            //std::getline(f, line2);
            /*CStruct params;
            ExecuteQBScript("AskDownload", &params);
            /*QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"Answear"));
            while ((DWORD)header->value.i == crc32f((unsigned char*)"None"))
            {
            header = GetQBKeyHeader(crc32f((unsigned char*)"Answear"));
            Sleep(200);
            }*/

            /*if ((DWORD)header->value.i == crc32f((unsigned char*)"No"))
            {
            response.clear();
            line.clear();
            f.clear();
            closesocket(fd);
            WSACleanup();
            downloading = false;
            return;
            }*/
            /*asked = true;
            latest = build;
            //sscanf(line.c_str(), "%s %s", &buildpath, &buildname);
            //DownloadAndInstall(buildpath, buildname);
            sucess = DownloadAndInstall((char*)line.c_str(), (SOCKADDR*)&service);
            /*using namespace System::Net;
            WebClient^ client = gcnew WebClient;
            Uri ^uri = gcnew Uri(address);

            // Specify that the DownloadFileCallback method gets called
            // when the download completes.
            client->DownloadFileCompleted += gcnew AsyncCompletedEventHandler(DownloadFileCallback2);

            // Specify a progress notification handler.
            client->DownloadProgressChanged += gcnew DownloadProgressChangedEventHandler(DownloadProgressCallback);
            client->DownloadDataAsync()
            response.clear();
            closesocket(fd);
            WSACleanup();
            downloading = false;
            return;*/
            closesocket(fd);
            sucess = DownloadAndInstall(line, build, (SOCKADDR*)&service);
            /*std::getline(f, line);
            while (std::getline(f, line))
            {
                if (line[0] == ';')
                    break;
                else if (line[0] == '[')
                    sscanf(line.c_str(), "[%f]", &latest);
                else
                {
                    sucess = DownloadAndInstall((char*)line.c_str(), (SOCKADDR*)&service);
                }
            }*/
        }
        else if (build == version)
            PrintDownloadString((char*)"You have latest version :)");

        //if (sscanf(&response.c_str()[pos], "%f"/* %s %f %s"*///, &latest/*, patchname, &build, buildname*/))
        if (latest > version && sucess)
        {
            /*FILE* fp = fopen("settings", "r+b");
            fseek(fp, 7, SEEK_SET);
            fwrite(&latest, 4, 1, fp);
            fclose(fp);*/

            /*if(build>version && build != latest)
            {
            DownloadAndInstall(buildname, (SOCKADDR *)&service);
            }
            DownloadAndInstall(patchname, (SOCKADDR *)&service);

            if(uninstalled.size() != 0)
            {
            for(DWORD i=0; i<uninstalled.size(); i++)
            {
            MessageBox(0, uninstalled[i].c_str(), "ERROR couldn't install file", 0);
            }
            uninstalled.clear();
            //DeleteFile("temp.zip");
            }*/
            /*HWND hwnd = ::GetActiveWindow();
            SetFocus(HWND_DESKTOP);
            ShellExecute(NULL, "open", "http://www.chapter-3.net/thps3/site/distribution/viewtopic.php?f=19&t=2677",
            NULL, NULL, SW_SHOWNORMAL);
            if (MessageBox(hwnd, "There is a new version of LevelMod do you want to close thps3?", "LevelMod update checker", MB_YESNO) == IDYES)
            {
            response.clear();
            closesocket(fd);
            WSACleanup();
            WSACleanup();
            ExitProcess(0);
            return;
            }
            else
            SetFocus(hwnd);*/
        }
        line.clear();
        //line2.clear();
        f.clear();
    }

    response.clear();
    //closesocket(fd);
    WSACleanup();
    Sleep(10000);
    bDownloading = false;
}

bool shouldfix = false;

void FixClampBugs()
{
    if (shouldfix)
    {
        //MessageBox(0, clamp, "YEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEAH", 0);
        const BYTE vertfix1[116] = {
          0xD9, 0x85, 0x60, 0x87, 0x00, 0x00, 0xD8, 0x5C, 0x24, 0x14, 0xDF, 0xE0, 0x25, 0x00, 0x41, 0x00, 0x00, 0x75, 0x0A, 0x8B, 0x85, 0x60, 0x87, 0x00, 0x00, 0x89, 0x44, 0x24, 0x14, 0x8B, 0x44, 0x24, 0x10, 0x8B, 0x4C, 0x24, 0x14, 0xE9, 0x4F, 0x29, 0x0a, 0x00,
          0x31, 0xFF, 0xD9, 0x85, 0x60, 0x86, 0x00, 0x00, 0xD8, 0x25, 0x38, 0x03, 0x40, 0x00, 0x6A, 0x00, 0x6A, 0x08, 0x6A, 0x00, 0x8B, 0xCD, 0xD9, 0x9D, 0x60, 0x86, 0x00, 0x00, 0xD9, 0x85, 0x70, 0x86, 0x00, 0x00, 0xD8, 0x25, 0x38, 0x03, 0x40, 0x00, 0xD9, 0x9D, 0x70, 0x86, 0x00, 0x00, 0xE8, 0x5B, 0xF9, 0x09, 0x00, 0x84, 0xC0, 0x0F, 0x85, 0xFC, 0x28, 0x0A, 0x00, 0x47, 0x83, 0xFF, 0x0C, 0x7C, 0xC1, 0xE9, 0x5B, 0x2A, 0x0A, 0x00, 0x00, 0x00, 0x40, 0x40
        };
        DWORD old;
        VirtualProtect((void*)0x004A2C39, sizeof(vertfix1), PAGE_EXECUTE_READWRITE, &old);
        memcpy((void*)0x004A2C39, vertfix1, sizeof(vertfix1));
        //VirtualProtect((void*)0x004A2C39, sizeof(vertfix1), old, &old);
        VirtualProtect((void*)0x004A2C23, sizeof(clampfunc), PAGE_EXECUTE_READWRITE, &old);
        memcpy((void*)0x004A2C39, clampfunc, sizeof(clampfunc));
        //VirtualProtect((void*)0x004A2C23, sizeof(clampfunc), old, &old);
    }
}

void TestForAcid()
{
    Skater* skater = Skater::Instance();
    skater->Store();

    Vertex pos = *(Vertex*)skater->GetPosition();
    Vertex start = pos;
    Vertex end = start;

    Vertex velocity = *(Vertex*)skater->GetVelocity();


    for (float dist = 0.0f; dist < 600.0f; dist += 20.0f)
    {
        Vertex vel_normal = GetNormal(&velocity);

        start.x = pos.x + vel_normal.x * dist;
        start.z = pos.z + vel_normal.z * dist;

        end.x = pos.x + vel_normal.x * dist;
        end.y = pos.y - 4000.0f;
        end.z = pos.z + vel_normal.z * dist;
        velocity.y -= 0.5f;
        skater->SetRay(*(D3DXVECTOR3*)&start, *(D3DXVECTOR3*)&end);


        if (skater->CollisionCheck(Collision::Flags::Vert))
        {
            //
            float height = skater->GetPosition()->y - skater->GetHitPoint()->y;

            if (height < 20.0f)
                break;


            float absnorm = fabsf(skater->GetCollisionNormal()->y);
            if (absnorm > 0.48f && absnorm < 0.98f)
            {
                //_printf("norm %f %f vel %f %f", vel_normal.x, vel_normal.z, velocity.x, velocity.z);
                float test = ((vel_normal.x - skater->GetCollisionNormal()->x) + (vel_normal.z - skater->GetCollisionNormal()->z));
                if (fabsf(test) < 0.82f)
                {
                    _printf("above\n");
                    skater->CallMemberFunction(Checksum("DoNextTrick"));
                    skater->FlagException("AcidDrop");
                    break;
                }
            }
        }
        skater->Restore();
    }
}
char* fopen_path = NULL;
char* checksum_name = NULL;
bool game = true;

vector<CStructHeader> compressedStructs;
DWORD numCompressed = 0;

void AddCompressedNodes()
{

    FILE* f = fopen(qbPath, "r+b");
    if (f)
    {
        _printf("Opened: %s\n", qbPath);
        fseek(f, 0, SEEK_END);
        DWORD size = ftell(f);

        //ZeroMemory(&compressedStructs.front(), compressedStructs.size() * sizeof(CStructHeader));

        BYTE* pFile = new BYTE[size];

        fseek(f, 0, SEEK_SET);
        fread(pFile, size, 1, f);
        if (*pFile != 0xAA)
            return;
        QScript::compNodes.clear();
        compressedStructs.clear();
        pFile++;


        compressedStructs.reserve(size / 4);
        while (*pFile != 0x0)
        {
            switch (*pFile)
            {
            default:
                pFile++;
                break;
            case 0x16:
                pFile++;
                /*sprintf(msg, "P %X", opcode);
                MessageBox(0, msg, msg, 0);*/
                DWORD name = *(DWORD*)pFile;
                _printf("Name %X\n", name);
                pFile += 4;


                QScript::compNodes.push_back(QScript::CompressedNode());
                QScript::CompressedNode* comp = &QScript::compNodes.back();

                comp->checksum = name;

                /*CStructHeader* pMap = *(CStructHeader**)0x008E1E04;
                pMap = pMap->NextHeader;
                *(CStructHeader**)0x008E1E04 = pMap;*/
                compressedStructs.push_back(CStructHeader());
                comp->first = &compressedStructs.back();



                CStructHeader* header = comp->first;
                comp->last = header;

                _printf("chc %X comp %X\n", name, comp);
                while (*pFile != 0 && *pFile != 1)
                {
                    pFile++;

                    DWORD name = *(DWORD*)pFile;
                    pFile += 4;
                    header->Type = QBKeyHeader::QBKeyType::LOCAL;

                    if (*(pFile) == 0x7)
                    {
                        pFile += 2;
                        DWORD script = *(DWORD*)pFile;
                        pFile += 4;
                        header->QBkey = name;
                        header->Data = script;

                        if (*(pFile) != 1 && *pFile != 0)
                        {
                            /*CStructHeader* pMap = *(CStructHeader**)0x008E1E04;
                            pMap = pMap->NextHeader;
                            *(CStructHeader**)0x008E1E04 = pMap;*/
                            compressedStructs.push_back(CStructHeader());
                            header->NextHeader = &compressedStructs.back();
                            header = header->NextHeader;
                        }
                        else
                        {
                            header->NextHeader = 0;
                            comp->last = header;
                            break;
                        }
                    }
                    else
                    {
                        header->Data = name;

                        if (*(pFile) != 1 && *pFile != 0)
                        {
                            /*CStructHeader* pMap = *(CStructHeader**)0x008E1E04;
                            pMap = pMap->NextHeader;
                            *(CStructHeader**)0x008E1E04 = pMap;*/
                            compressedStructs.push_back(CStructHeader());
                            header->NextHeader = &compressedStructs.back();
                            /*pMap = pMap->NextHeader;
                            *(CStructHeader**)0x008E1E04 = pMap;
                            _printf("pMap %X\n", pMap);*/
                            header = header->NextHeader;
                        }
                        else
                        {
                            header->NextHeader = 0;
                            comp->last = header;
                            break;
                        }

                    }
                }

                header->NextHeader = 0;
                comp->last = header;

                break;
            }
        }



        delete[] pFile;
        fclose(f);

    }
}
bool bHookDone = false;

void AddFunctions();

static char ShaderFile[128] = "";

void HookedFopen(char* p)
{
    if (p && strlen(p))
    {
        if (p[strlen(p) - 1] == 'b' && p[strlen(p) - 2] == 'q' && p[strlen(p) - 3] == '.' && game)
        {
            if (!bHookDone)
            {
                AddFunctions();
                bHookDone = true;
            }
            else if (strstr(p, "Levels\\") || strstr(p, "levels\\"))
            {
                DWORD pos = std::string(p).find_last_of("\\");

                char temp_level[50];
                memcpy(temp_level, &p[pos + 1], strlen(p) - 3 - (pos + 1));
                temp_level[strlen(p) - 3 - (pos + 1)] = 0x00;
                Game::level_checksum = Checksum(temp_level);
                //MessageBox(0, temp_level, Level, 0);


                if (bDebugMode)
                {
                    _printf("Fopen: %s\n", p);
                    memcpy(qbPath, p, strlen(p) + 1);
                    qbPath[strlen(qbPath) - 1] = 0x0;

                    /*MessageBox(0, p, qbPath, 0);
                    AddCompressedNodes();*/
                    if (bDebugMode)
                        QScript::Scripts->OpenScript(p, true);
                }


                strcpy(ShaderFile, p);

                ShaderFile[strlen(p) - 2] = 's';
                ShaderFile[strlen(p) - 1] = 'h';
                /*ShaderFile[strlen(p)] = 'h';
                ShaderFile[strlen(p)+1] = 0x0;*/

                //LoadCustomShaders(ShaderFile);

            }
        }
    }
}

Skater* oldSkater;

void LoadCustomShaderThread()
{
    //We have started a new game

    //First update skater pointer
    Game::skater = Skater::UpdateSkater();
    while(!Game::skater) Skater::UpdateSkater();

    //Then check if we are host and if we are, send the HostOptions to clients
    Network::NetHandler* net_handler = Network::NetHandler::Instance();

    if (net_handler->InNetGame() && net_handler->OnServer())
    {
        for (auto it = LevelModSettings::overrideOptions.begin(); it != LevelModSettings::overrideOptions.end(); ++it)
        {
            SendHostOptionChanged(it->first, it->second.value);
        }
    }

    //Then update shaders
    Gfx::LoadCustomShaders(ShaderFile);
}

bool OnPostLevelLoad(CStruct* pStruct, CScript* pScript)
{
    _printf("OnPostLevelLoad...\n");
    oldSkater = Game::skater;

    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LoadCustomShaderThread, NULL, NULL, NULL);
    return true;
}

bool ChangeLocalScript(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* header = pStruct->head;
    if (header)
    {
        CStructHeader* param = pScript->GetParam(header->QBkey);
        if (param)
        {
            _printf("param %s value %d header %s value %d\n", FindChecksumName(param->QBkey), param->value.i, FindChecksumName(header->QBkey), header->value.i);
            _printf("param_type %s header_type %s\n", QScript::QBTypes[param->Type], QScript::QBTypes[header->Type]);
            param->Data = header->Data;
            return true;
        }
        else
            _printf(__FUNCTION__ "Couldn't find %s in script %s\n", FindChecksumName(header->QBkey), FindChecksumName(pScript->scriptChecksum));
    }

    _printf("No param passed to Local_Param? in script %s\n", FindChecksumName(pScript->scriptChecksum));
    return false;
}


struct Light
{
    D3DXVECTOR3 position;
    float clipping;

    Light(D3DXVECTOR3& pos, float& clipping_distance)
    {
        position = pos;
        clipping = clipping_distance;
    }
};
std::vector<Light> lights;

bool RemoveLights(CStruct* pStruct, CScript* pScript)
{
    lights.clear();
    return true;
}

bool AddLights(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* header = pStruct->head;

    while (header)
    {
        if (header->Type = QBKeyHeader::QBKeyType::ARRAY)
        {
            for (DWORD i = 0; i < header->pArray->GetNumItems(); i++)
            {
                D3DXVECTOR3 position = D3DXVECTOR3(1000.0f, 8000.0f, 1000.0f);
                float clipping = 0;

                CStruct *pItem = header->pArray->GetStructure(i);

                CStructHeader* data;
                if (pItem->GetStruct(Checksums::Pos, &data))
                {
                    position = *data->pVec;
                }

                if (pItem->GetStruct(Checksum("Clipping"), &data))
                {
                    clipping = data->value.f;
                }
                
                lights.push_back(Light(position, clipping));
            }
            return true;
        }
        header = header->NextHeader;
    }
    return false;
}

bool RemoveLightsScript(CStruct* pStruct, CScript* pScript)
{
    lights.clear();
    return true;
}

void AddChecksum(int key, char* name, void* retAddr)
{
    lastQB = name;
    if (name && strlen(name) > 1)
    {
        std::map<DWORD, char*>::iterator it = QScript::Scripts->qbTable.find(key);
        if (it == QScript::Scripts->qbTable.end())
        {
            /*FILE* debugFile = fopen("debug.txt", "r+t");
            fseek(debugFile, 0, SEEK_END);
            fprintf(debugFile, "AddingKey %s %X, CalledFrom %p\r\n", name, key, retAddr);
            printf("AddingKey %s %X, CalledFrom %p\r\n", name, key, retAddr);
            fclose(debugFile);*/
            //_printf("AddChecksum %s 0x%X\n", name, key);
            QScript::Scripts->qbTable.insert(std::pair<DWORD, char*>(key, String::AddString(name)));
            QScript::qbKeys.push_back(key);
        }
        /*else if (__stricmp(it->second, name))
        {
            FILE* debugFile = fopen("debug.txt", "r+t");
            fseek(debugFile, 0, SEEK_END);
            fprintf(debugFile, "found collision  %s(%X) %s(%X)\nCalled From %p\n", it->second, it->first, name, key, retAddr);
            printf("found collision  %s(%X) %s(%X)\nCalled From %p\n", it->second, it->first, name, key, retAddr);
            fclose(debugFile);
            //fclose(debugFile);
        }*/
    }
    else
    {
        std::map<DWORD, char*>::iterator it = QScript::Scripts->qbTable.find(key);
        if (it == QScript::Scripts->qbTable.end())
        {
            /*FILE* debugFile = fopen("debug.txt", "r+t");
            fseek(debugFile, 0, SEEK_END);
            fprintf(debugFile, "AddChecksum without valid string %X\n", key);
            printf("AddChecksum without valid string %X\n", key);
            fclose(debugFile);*/
        }
    }

}

__declspec(naked) void Checksum_naked()
{
    static void* retAddr;
    static void** pESP;
    static const DWORD jmpBack = 0x004110E4;
    static DWORD chc = 0;

    _asm mov ecx, [esp + 4];
    _asm mov checksum_name, ecx;
    __asm { mov pESP, esp };




    _asm call[jmpBack];
    _asm mov chc, eax
    _asm pushad;
    _asm pushfd;
    retAddr = pESP[0];
    AddChecksum(chc, checksum_name, retAddr);
    _asm popfd;
    _asm popad;
    _asm mov eax, chc
    _asm ret;
}

__declspec(naked) void Fopen_naked()
{
    static DWORD fopen = *(DWORD*)0x0058D0B0;
    static const DWORD jmpBack = fopen + 5;
    _asm mov fopen_path, esp;
    _asm pushad;
    _asm pushfd;

    fopen_path = fopen_path + 0x4;
    fopen_path = (char*)*(DWORD*)fopen_path;
    HookedFopen(fopen_path);
    _asm popfd;
    _asm popad;
    _asm mov edi, edi;
    _asm push ebp;
    _asm mov ebp, esp;
    _asm jmp[jmpBack];
}

FILE* __cdecl _fopen(const char* p, const char* b)
{
    _printf("Fopen: %s\n");
    if (p[strlen(p) - 1] == 'b' && p[strlen(p) - 2] == 'q' && p[strlen(p) - 3] == '.' && game)
    {

        memcpy(qbPath, p, strlen(p) + 1);
        qbPath[strlen(qbPath) - 1] = 0x0;
        //MessageBox(0, p, qbPath, 0);
        AddCompressedNodes();
        QScript::Scripts->OpenScript((char*)p, true);
    }

    return fopen(p, b);
}


struct BBox
{
    Vertex max;
    Vertex min;
};

struct CollisionObject;

struct UnkStruct
{
    UnkStruct* prev;
    UnkStruct* next;
    DWORD flags;//??0x34?
    CollisionObject* object;
    void* unk1;
    bool(*pFunction)(UnkStruct*, UnkStruct*);//some collision testing maybe??
    WORD numUnk2;//0x2E?
    WORD flag;//0x1
    DWORD unk2;//0x0
    DWORD numVerts;
    DWORD numTris;
};

struct CollisionObject//+0x64 collisionflags*
{
    DWORD transform;//0xFFFFFFFF
    DWORD checksum;
    WORD* indices;
    Vertex* verticse;
    DWORD zero;//could be transform?
    void* unk1;//never accessed?
    void* unk2;//never accessed?
    void* unk3;//dunno
    UnkStruct* unk;
    void* unkptr[6];//ptrs to itself?? like if on address 0050000 then it 
    //ptrs to itself and 0050000+4 ptrs to 0050000 and 0050000+8 
    //ptrs to 0050000+8 and 0050000+12 ptrs to 0050000+8 etc
    BBox bbox;
    DWORD padding;//never accessed
    WORD numTris;//??
    DWORD numTris2;//??
    void* unkptr2[2];
    WORD* collFlags;//vert, skatable, etc
};

//BYTE RenderFix[4096] = { 0 };

FILE* Dump;

void AddDump(const char* dump, ...)
{
    _printf("Dumping %s", dump);
    Dump = fopen("dump.txt", "w+t");
    fseek(Dump, 0, SEEK_END);
    va_list va_alist;
    char logbuf[512] = { 0 };

    va_start(va_alist, dump);
    _vsnprintf(logbuf + strlen(logbuf), sizeof(logbuf) - strlen(logbuf), dump, va_alist);
    va_end(va_alist);
    fprintf(Dump, logbuf);
    fclose(Dump);
}


const Vertex* const __restrict GetSkaterPos()
{
    static const DWORD ptr = 0x008E2498;
    VALIDATE_PTR((void*)ptr);
    DWORD pPos = *(DWORD*)ptr + 0x13C;
    VALIDATE_PTR((void*)pPos);
    pPos = *(DWORD*)pPos + 0x14;
    VALIDATE_PTR((void*)pPos);
    const DWORD old = pPos;
    pPos = *(DWORD*)pPos + 0x4BC;
    if (InvalidReadPtr((void*)pPos))
    {
        pPos = old;
        pPos = *(DWORD*)pPos + 0x3B4;
        VALIDATE_PTR((void*)pPos);
    }
    pPos = *(DWORD*)pPos + 0x18;
    VALIDATE_DATA((Vertex*)pPos, sizeof(Vertex));
    return (const Vertex* const __restrict)pPos;
}

float GetSkaterYAngle()
{
    /*angle on ground
    atan(maybe abs(new.z - old.z / maybe abs(new.x - old.x)
    angle up/down
    atan(maybe abs(new.y - old.y / maybe abs(new.x - old.x)
    or maube 100% not working mot-algo unless you use p4*/
    static const DWORD ptr = 0x008E1E90;
    VALIDATE_PTR((void*)ptr);
    DWORD pAngle = *(DWORD*)ptr + 0xC0;
    VALIDATE_PTR((void*)pAngle);
    pAngle = *(DWORD*)pAngle + 0x5C4;
    VALIDATE_PTR((void*)pAngle);
    pAngle = *(DWORD*)pAngle + 0x0C;//--
    VALIDATE_PTR((void*)pAngle);
    pAngle = *(DWORD*)pAngle + 0x10;
    VALIDATE_PTR((void*)pAngle);
    pAngle = *(DWORD*)pAngle + 0x40;//--
    VALIDATE_DATA((float*)pAngle, sizeof(float));
    float checker = *(float*)pAngle;
    pAngle += 8;
    VALIDATE_DATA((float*)pAngle, sizeof(float));
    float angle = *(float*)pAngle;
    if (checker >= 0.0f)
    {
        return (angle + 1.0f) * 3.14159f * 0.5f;
    }
    else
    {
        return ((angle + 1.0f) * 3.14159f * 0.5f) * -1.0f;
    }
}

void ShowPm(char* message)
{
    CStruct params;
    CStructHeader* param = params.AddParam("text", QBKeyHeader::STRING);
    param->pStr = message;
    while (*message != 0x0)
    {
        if (*message == '%')
            *message = '_';
        message++;
    }
    ExecuteQBScript("LaunchGrafCounter", &params);
    params.DellocateCStruct();
}

char* GetPlayerName()
{
    return (*(char**)0x00970158) + 0x4;
}

void GotMessage(char* message)
{
    char* playerName = GetPlayerName();
    char* c = message;
    while (*c != 0x3A)
    {
        if (*c++ != *playerName++)
        {
            ShowPm(message);
            return;
        }
    }
}

__declspec(naked) void AddMessageHook(void)
{
    static DWORD retAddr = 0;
    _asm pop retAddr;
    static char* message = NULL;
    static DWORD old = 0;
    _asm
    {
        test edx, edx;
        mov esi, [esp + 0x00000820];
        je skip;
        test esi, esi;
        jne skip;
        mov old, eax;
        lea eax, [esp + 0x0C];
        test eax, eax;
        je skip;
        mov message, eax;
        mov eax, old;
        pushad;
        pushfd;
    }
    GotMessage(++message);
    _asm
    {
        popfd;
        popad;
    skip:
        push retAddr;
        ret;
    }



    /*static DWORD AddMessageRetAddr = 0;
    char* message;
    __asm
    {

    pop AddMessageRetAddr;

    test edx, edx;
    je skip;
    test esi, esi;
    jne skip;
    pushad;
    pushfd;
    lea eax, [esp+0x2C];
    test eax, eax;
    je skip;
    mov message, eax;
    }
    GotMessage(message++);
    __asm
    {

    popfd;
    popad;
    skip:
    mov esi,[esp+0x00000820];

    push AddMessageRetAddr;

    ret;
    }*/
}

VOID WriteBytesASM(DWORD destAddress, LPVOID patch, DWORD numBytes)
{
    // Store old protection of the memory page
    DWORD oldProtect = 0;

    // Store the source address
    DWORD srcAddress = PtrToUlong(patch);

    // Make sure page is writeable
    VirtualProtect((void*)(destAddress), numBytes, PAGE_EXECUTE_READWRITE, &oldProtect);

    // Do the patch (oldschool style to avoid memcpy)
    __asm
    {
        nop						// Filler
        nop						// Filler
        nop						// Filler

        mov esi, srcAddress		// Save the address
        mov edi, destAddress	// Save the destination address
        mov ecx, numBytes		// Save the size of the patch
        Start :
        cmp ecx, 0				// Are we done yet?
            jz Exit					// If so, go to end of function

            mov al, [esi]			// Move the byte at the patch into AL
            mov[edi], al			// Move AL into the destination byte
            dec ecx					// 1 less byte to patch
            inc esi					// Next source byte
            inc edi					// Next destination byte
            jmp Start				// Repeat the process
            Exit :
        nop						// Filler
            nop						// Filler
            nop						// Filler
    }

    // Restore old page protection
    VirtualProtect((void*)(destAddress), numBytes, oldProtect, &oldProtect);
}

VOID HookFunc(DWORD destAddress, VOID(*func)(VOID), BYTE nopCount)
{
    DWORD offset = (PtrToUlong(func) - destAddress) - 5;

    BYTE nopPatch[0xFF] = { 0 };

    BYTE patch[5] = { 0xE8, 0x00, 0x00, 0x00, 0x00 };
    memcpy(patch + 1, &offset, sizeof(DWORD));
    WriteBytesASM(destAddress, patch, 5);

    if (nopCount == 0)
        return;

    memset(nopPatch, 0x90, nopCount);

    WriteBytesASM(destAddress + 5, nopPatch, nopCount);
}

static char motd[128] = "";


void GetMotd()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return;
    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == INVALID_SOCKET)
    {
        WSACleanup();
        return;
    }
    SOCKADDR_IN service;
    service.sin_family = AF_INET;
    service.sin_port = htons(80);
    LPHOSTENT host = gethostbyname("vadru.chapter-3.net");
    if (!host)
    {
        closesocket(fd);
        WSACleanup();
        return;
    }
    service.sin_addr = *((LPIN_ADDR)*host->h_addr_list);
    if (connect(fd, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {
        closesocket(fd);
        WSACleanup();
        return;
    }
    static const char* GetStr = "GET /LevelModMotd HTTP/1.1\r\n"
        "Host: vadru.chapter-3.net\r\n"
        "User-Agent: Mozilla FireFox/4.0\r\n"
        "User-Agent: Mozilla Firefox/4.0\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*//*;q=0.8\r\n"
        "Accept-Language: en-gb,en;q=0.5\r\n"
        "Accept-Encoding: gzip,deflate\r\n"
        "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n\r\n";

    if (send(fd, GetStr, strlen(GetStr) + 1, 0) == SOCKET_ERROR)
    {
        closesocket(fd);
        WSACleanup();
        return;
    }
    std::string response;// = "";
    int resp_leng = 1024;
    char buffer[1024];
    while (resp_leng > 0)
    {
        resp_leng = recv(fd, (char*)&buffer, 1024, 0);
        if (resp_leng > 0)
            response += std::string(buffer).substr(0, resp_leng);
    }
    if (strstr(response.c_str(), "404 Not Found"))
    {
        response.clear();
        closesocket(fd);
        WSACleanup();
        return;
    }
    else
    {
        int pos = response.find("\r\n\r\n");
        if (pos != string::npos)
        {
            pos += 4;
            response.erase(0, pos);
            for (DWORD i = 0, len = response.size(); i < len; i++)
            {
                motd[i] = response[i];
            }
        }
    }

    response.clear();
    closesocket(fd);
    WSACleanup();
}



void FixMessage()
{
    printf("Fixing Messages\n");



    /*QBKeyHeader* header = GetQBKeyHeader(Checksum("ver"));
    DWORD times = 0;
    while (header == 0 && times < 10000)
    {
        header = GetQBKeyHeader(Checksum("ver"));
        times++;
        Sleep(50);
    }
    if (header)
    {
        printf("Found Version header\n");
        std::ostringstream ss;
        ss << VERSION;
        if (header->pStr)
            freex(header->pStr);
        static const std::string message = "2011-2013, LevelMod " + std::string(ss.str());

        const DWORD len = message.length() + 1;
        header->pStr = (char*)mallocx(len);
        for (DWORD i = 0; i < len; i++)
        {
            header->pStr[i] = message[i];
        }
        printf("Version String: %s\n", header->pStr);

        header = GetQBKeyHeader(Checksum("motd"));

        if (header->pStr)
            freex(header->pStr);
        const DWORD lens = strlen(motd) + 1;
        header->pStr = (char*)mallocx(lens);
        for (DWORD i = 0; i < lens; i++)
        {
            header->pStr[i] = motd[i];
        }
    }*/
    printf("FixMessage DONE\n");
    init2 = true;
}

/**(DWORD*)0x0042C18D = 0x000FFFFF;
*(DWORD*)0x0042C1C7 = 0x000FFFEF;
*(DWORD*)0x0042C1D5 = 0x000FFFEF;
*(DWORD*)0x0042C1E1 = 0x000FFFF7;
*(DWORD*)0x0042C1EC = 0x000FFFFB;
*(BYTE*)0x00426088 = 0x74;
*(BYTE*)0x004260E7 = 0x74;
*(BYTE*)0x004288F7 = 0x74;
*(BYTE*)0x00428B97 = 0x74;
*(BYTE*)0x004288F7 = 0x74;*;*/

/*VirtualProtect((void*)0x004260E7, 7, PAGE_EXECUTE_READWRITE, &old);
*(DWORD*)0x004260E7 = 0x9090F775;
*(WORD*)0x004260EB = 0x9090;
*(BYTE*)0x004260ED = 0x90;*/

void InitSkater()
{
    Slerp::transfer = false;
    Slerp::landing = false;
    Slerp::slerping = false;
    D3DXMatrixIdentity(&Slerp::start);
    D3DXMatrixIdentity(&Slerp::end);
    D3DXMatrixIdentity(&Slerp::old);
    Slerp::duration = 0.0f;
    Slerp::radians = 0.0f;
    Slerp::timer = 0.0f;
    Slerp::axis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void D3D8BASE(void)
{
    Sleep(10000);
    //CreateConsole();
    /*HookDebugMessages(NULL, NULL);
    *(BYTE*)0x00478983 = 0x75;
    PresentHook = Hook(15, HookedPresent);
    pPresent = (Present_t)PresentHook.func;*/
}


DWORD oldTagCount = 0;

int LineCount = 0;
struct Point {
    float x, y, z, rhw;
    DWORD color;


}; //lineList[1000], drawList[1000];

struct Line
{
    D3DXVECTOR3 v;
    float rhw;
    D3DXCOLOR color;

    Line(D3DXVECTOR3 _v, D3DXCOLOR _c)
    {
        this->v = _v;
        rhw = 1.0f;
        this->color = _c;
    }
};


std::vector<Line> lineList;


/*
void Test()
{
    Sleep(10000);
    MessageBox(0, "", "", 0);
    DWORD old, addr, offset = 0;
    VirtualProtect((void*)0x004294BE, 5, PAGE_EXECUTE_READWRITE, &old);
    *(BYTE*)0x004294BE = 0xE9;
    addr = 0x004294BF;
    offset = (PtrToUlong(Ncomp_naked) - 0x004294BE) - 5;
    *(DWORD*)addr = offset;
}
*/


bool DumpScripts(CStruct* params, CScript* pScript)
{
    CScript::DumpScripts();
    return true;
}

bool (*pDisplayLoadingScreen)(CStruct*, CScript*);
bool (*pHideLoadingScreen)(CStruct*, CScript*);
bool show_loading_screen = false;
bool rendering_loading_screen = false;

void DrawEye();

void RenderLoadingScreen()
{
    while (show_loading_screen)
    {
        DrawEye();
        Gfx::pDevice->Present(0, 0, 0, 0);
        Sleep(16);
    }
    DrawEye();
    Gfx::pDevice->Present(0, 0, 0, 0);
    Sleep(16);

    rendering_loading_screen = false;
}


bool HideLoadingScreen(CStruct* pStruct, CScript* pScript)
{
    show_loading_screen = false;
    //while (rendering_loading_screen) Sleep(10);
    return pHideLoadingScreen(pStruct, pScript);
}

bool DisplayLoadingScreen(CStruct* pStruct, CScript* pScript)
{
    eye_state = 6;
    last_state = 0;
    wheel_timer = 0;
    actual_timer = 0;
    rotating = false;

    show_loading_screen = true;
    rendering_loading_screen = true;
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)RenderLoadingScreen, 0, 0, 0);
    return pDisplayLoadingScreen(pStruct, pScript);
}



const DWORD numFunctions = sizeof(scripts) / sizeof(CompiledScript);

void AddFunctions()
{
    //MessageBox(0, "Adding Functions", "", 0);
    QBKeyHeader* header = NULL;
    for (int i = 0; i < numFunctions; i++)
    {
        header = AddQBKeyHeader(Checksum(scripts[i].name), 1);
        if (header)
        {
            header->type = QBKeyHeader::COMPILED_FUNCTION;
            header->pFunction = scripts[i].pFunction;
        }
        else
            MessageBox(0, "couldn't add script", scripts[i].name, 0);
    }

    header = GetQBKeyHeader(Checksums::Shatter);
    if (header)
        header->pFunction = NewShatterScript;
}

bool __stdcall proxy_FixGoBack(BYTE unk1, BYTE unk2, BYTE unk3, BYTE unk4)
{
    static void* oldECX;
    _asm mov oldECX, ecx;

    //If escape or triangle is pressed we need to handle it
    if (KeyState::GetKeyboardState(VirtualKeyCode::ESC) || KeyState::GetOldKeyboardState(VirtualKeyCode::ESC) || KeyMap::IsPressed(KeyMap::Button::Triangle))
    {
        typedef bool(__thiscall* pHandleBackKey)(void* pThis, BYTE unk1, BYTE unk2, BYTE unk3, BYTE unk4);
        return pHandleBackKey(0x004CF380)(oldECX, unk1, unk2, unk3, unk4);
    }
    _asm mov ecx, oldECX;
    return true;
}

float __cdecl proxy_SnapToGroundClamp(float a1)
{
    typedef float(__cdecl* pClacos)(float angle);
    // fixes snap to ground bugs
    float angle = a1;
    if (angle > 1.0f) angle = 1.0f;
    else if (angle < -1.0f) angle = -1.0f;
    return acosf(angle);//pClacos(0x776B31E0)(angle);
}

int proxy_sprintf(char* result, char* format, DWORD value)
{
    static Element* pElement;
    _asm mov pElement, ebx;

    if (pElement && pElement->pair)
    {
        format = (char*)0x005B4428;
        value = (DWORD)pElement->pair[value].text;
    }
    return sprintf(result, format, value);
}

//004D5759

auto constexpr size_reduction = 0x44 - sizeof(RailNode);

struct OptimizedGrind
{
    DWORD addr;
    DWORD offset;

    void Optimize()
    {
        DWORD old;
        VirtualProtect((LPVOID)addr, offset, PAGE_EXECUTE_READWRITE, &old);
        addr += offset;
        old = *(BYTE*)addr;
        *(BYTE*)addr -= size_reduction;
    }
};

OptimizedGrind optimized_grind[] = { {0x00489CCB, 3}, {0x004A507C, 2}, {0x004A5316, 2}, {0x004A5384, 2}, {0x004A5395, 2},
    {0x004A53F5, 2}, {0x004A538E, 2}, {0x004A537B, 2}, {0x004A5071, 2}, {0x004A4EC1, 2}, {0x004A4ECC, 2 }, {0x004A530B, 2}, 
    {0x004A50D3, 2},  {0x004A5059, 2}, {0x004A4F23, 2}, {0x004A4CA1, 2},  {0x004A5AFF, 3}, {0x004A6566,2}, {0x004A633E, 2}, 
    {0x004A6341, 2}, {0x004A6306, 2}, {0x004A62CE, 2}, {0x004A6278, 2}, {0x004A626F, 2}, {0x004A6735, 2},  {0x004A657E, 2},
    {0x004A5985, 2},  {0x004A596D, 2}, {0x004A542C, 2},  };

DWORD optimized2[] = { 0x0040100F, 0x00401D40, 0x00402478, 0x0041117C, 0x00411589, 0x00413A31, 0x00413A3D, 0x00413AEF,
0x00413AFB, 0x004155B2, 0x004194EC, 0x0041963B, 0x0041A6CA, 0x0041AC5A, 0x004201CD, 0x004251F9, 0x00425250, 0x0042527D,
0x0042528B, 0x004263CA, 0x0042641A, 0x0042646A, 0x004264BA, 0x0042651F, 0x0042657A, 0x004265DA, 0x004273F2, 0x00428254,
0x004286D8, 0x00429776, 0x00429856, 0x00429936, 0x00429A26, 0x00429B16, 0x00429C26, 0x00429CF6, 0x00429DD6, 0x00429E52,
0x00429F16, 0x00429FCA, 0x0042B0DF, 0x0042B2E5, 0x0042BF87, 0x0042BFBF, 0x0042C6A6, 0x0042C6B2, 0x0043299E, 0x004329B1,
0x00433BF7, 0x004355F4, 0x0043763D, 0x00438F6C, 0x0043D6D3, 0x0042A05A, 0x004508ED, 0x004508F9, 0x00504F7F, 0x004BA6F2,
0x00441913, 0x00441AAE, 0x00441AB8, 0x004422FF, 0x00442375, 0x004423D9, 0x004423E5, 0x00442E2A, 0x00442EB7, 0x00443405,
0x004434A7, 0x00443545, 0x00443D9F, 0x00443E8F, 0x00443F7F, 0x004449A9, 0x004C5E9E, 0x0047FAA0, 0x0047759E, 0x00474A98};

struct OptimizedArrayCRC
{
    DWORD string;

    void Optimize()
    {
        DWORD old;
        DWORD ptr = string;
        ptr++;
        VirtualProtect((LPVOID)ptr, 4, PAGE_EXECUTE_READWRITE, &old);
        char* c = *(char**)ptr;
        _printf("Optimizing array access: %s\n", c);
        *(DWORD*)ptr = crc32f(c);
        if (string == 0x004B8306 || string == 0x004B830D)
            return;
        ptr += 4;
        retry:
        while (*(BYTE*)ptr != 0xE8) ptr++;
        ptr++;
        if ((*(DWORD*)ptr + ptr + 4) != 0x004265D0)
            goto retry;
        VirtualProtect((LPVOID)ptr, 4, PAGE_EXECUTE_READWRITE, &old);
        *(DWORD*)ptr = 0x00426590 - ptr - 4;
    }
};

//00454D6C this one needs fix
//0048FB7C this one needs fix
//004E7B91
//0050360F

OptimizedArrayCRC optimized3[] = { 0x00412035, 0x004142C2, 0x00418869, 0x00418BAC, 0x004198C5, 0x00419CF7,  0x00419D5E,
0x0041B2F1, 0x0041D8BB, 0x0041DAFD, 0x0041DD52, 0x0041E059, 0x0041FE95, 0x00420EBF, 0x00422670, 0x00422EC0, 0x004252A8, 
0x0042C724, 0x00432786, 0x0043E0AB, 0x0043F946, 0x00442CCA, 0x00444951, 0x004457B8, 0x004460C6, 0x004469E5, 0x00446AF6,
0x00446C46, 0x00446D06, 0x00446F44, 0x00446FF2, 0x004471FF, 0x004475DE, 0x0044770E, 0x004477FA, 0x0044782C, 0x00447F0F,
0x0044871F, 0x00448AAC, 0x00448B36, 0x00448E16, 0x0044F3EA, 0x00451207, 0x00454F0A, 0x0045515E, 0x0045556E, 0x00457E3B,
0x0045872E, 0x0045932E, 0x0045A025, 0x0045C340, 0x0045C74D, 0x0045C925, 0x0045D359, 0x0045D55B, 0x0045FAD5, 0x00464786,
0x00464B4B, 0x004677D9, 0x00467E99, 0x00469448, 0x0046A680, 0x0046AF8B, 0x004714BD, 0x00474B6E, 0x00477310, 0x0047A392,
0x0047A65F, 0x0047C265, 0x00488085, 0x00498402, 0x00498437, 0x0049B903, 0x0049BD13, 0x0049C032, 0x004A5AF5, 0x004A5BE1,
0x004A6011, 0x004B50E9, 0x004B54C3, 0x004B5502, 0x004B5AC7, 0x004B6A37, 0x004B73C3, 0x004B8316, 0x004B8306, 0x004B830D, 
0x004BB741, 0x004BC052,
0x004BD32C, 0x004BDB05, 0x004BDE7A, 0x004BEA69, 0x004E3145, 0x004E31B2, 0x004E4E55, 0x004F07C5, };
LARGE_INTEGER freq;
double fFreq;
LARGE_INTEGER endTime;
#define startTime (*(LARGE_INTEGER*)0x008E8E18)
LARGE_INTEGER elapsedTime;
//void HookControls();
DWORD old_trigger;
DWORD old_trigger_frame;

__declspec(naked) void TriggerScript()
{
    static DWORD pTrigger;
    static DWORD pCall = 0x0049D070;
    static DWORD oldECX;
    _asm mov oldECX, ecx;
    _asm lea eax, [ecx + 0x0000867C];
    _asm push eax;
    _asm mov pTrigger, eax;
    pTrigger += 0x48;
    if (*(bool*)pTrigger)
    {
        pTrigger += 4;
        if (*(DWORD*)pTrigger != old_trigger || (Gfx::frameCounter-old_trigger_frame)>30)
        {
            old_trigger_frame = Gfx::frameCounter;
            old_trigger = *(DWORD*)pTrigger;

            _asm push 0;
            _asm mov ecx, oldECX;
            _asm call[pCall];
            _asm ret;
        }
    }
    _asm pop eax;
    _asm mov ecx, oldECX;
    _asm ret;
}

LARGE_INTEGER timer_time;
DWORD timer_old_start;

DWORD __cdecl GetTime()
{
    //timer_old_start = timer_time.LowPart;


    QueryPerformanceCounter(&timer_time);
    double ms = double(timer_time.LowPart)* fFreq;
    ms += 0.5f;
    DWORD truncated = ms;

    if (timer_time.HighPart)
    {
        ms = double(timer_time.HighPart) * fFreq;
        ms += 0.5f;
        DWORD truncated2 = ms;
        _asm mov edx, truncated2;
    }
    else
        _asm xor edx, edx

    return truncated;
}

DWORD frameticks;

void TimerElapsed()
{
    LARGE_INTEGER targetTime;
    targetTime.QuadPart = startTime.QuadPart + frameticks;
    while (endTime.QuadPart < targetTime.QuadPart)
    {
        QueryPerformanceCounter(&endTime);
    }
    /*if (endTime.HighPart == startTime.HighPart)
    {
        //elapsedTime.LowPart = (endTime.LowPart - startTime.LowPart);
        _asm xor edx, edx
        double ms = (double((endTime.LowPart - startTime.LowPart)) * fFreq);
        ms += 0.55f;

        DWORD truncated = ms;
        //004F9462
        return truncated;// (elapsedTime.LowPart * 1000) / freq.LowPart;
    }
    else
    {
        elapsedTime.LowPart = 0xFFFFFFFF - startTime.LowPart + endTime.LowPart;
        _asm xor edx, edx
        //return (elapsedTime.LowPart * 1000) / freq.LowPart;
        double ms = (double(elapsedTime.LowPart) * fFreq);
        ms += 0.55f;

        DWORD truncated = ms;
        return truncated;
    }*/
}


double hybrid_limit = 16.0;

//50% sleep 50% loop
void TimerElapsed_Hybrid()
{
    QueryPerformanceCounter(&endTime);
    LARGE_INTEGER elapsedTime;
    elapsedTime.QuadPart = (endTime.QuadPart - startTime.QuadPart);
    double ms = ((double)(elapsedTime.LowPart) * fFreq);

    if (hybrid_limit > 3.0)
    {
        DWORD truncated = ((hybrid_limit - ms) * 0.5);
        if (truncated && truncated < 14)
            Sleep(truncated);
    }
    else
        _printf("Use exact\n");

    LARGE_INTEGER targetTime;
    targetTime.QuadPart = startTime.QuadPart + frameticks;
    while (endTime.QuadPart < targetTime.QuadPart)
    {
        QueryPerformanceCounter(&endTime);
    }
}

DWORD TimerElapsed_Sleep()
{
    QueryPerformanceCounter(&endTime);
    /*if (endTime.HighPart == startTime.HighPart)
    {
        //elapsedTime.LowPart = (endTime.LowPart - startTime.LowPart);
        _asm xor edx, edx
        double ms = (double((endTime.LowPart - startTime.LowPart)) * fFreq);
        ms += 0.55f;

        DWORD truncated = ms;
        //004F9462
        return truncated;// (elapsedTime.LowPart * 1000) / freq.LowPart;
    }
    else
    {
        elapsedTime.LowPart = 0xFFFFFFFF - startTime.LowPart + endTime.LowPart;
        _asm xor edx, edx
        //return (elapsedTime.LowPart * 1000) / freq.LowPart;
        double ms = (double(elapsedTime.LowPart) * fFreq);
        ms += 0.55f;

        DWORD truncated = ms;
        return truncated;
    }*/

    LARGE_INTEGER elapsedTime;
    elapsedTime.QuadPart = (endTime.QuadPart - startTime.QuadPart);
    double ms = ((double)(elapsedTime.LowPart) * fFreq);
    ms += 0.55f;

    DWORD truncated = ms;
    return truncated;
}

/*DWORD TimerElapsed()
{
    QueryPerformanceCounter(&endTime);

    if (endTime.HighPart == startTime.HighPart)
    {
        elapsedTime.LowPart = (endTime.LowPart - startTime.LowPart);
        _asm xor edx, edx;
        return (DWORD)((float)elapsedTime.LowPart * fFreq);
    }
    else
    {
        elapsedTime.LowPart = 0xFFFFFFFF - startTime.LowPart + endTime.LowPart;
        _asm xor edx, edx
        return (DWORD)((float)elapsedTime.LowPart * fFreq);
    }
}*/

LARGE_INTEGER old_start;
DWORD timer_lock = 0x10;

__declspec(naked) void UpdateFrameLength()
{
    *(float*)0x00850FD0 = (float)(framelength);
    *(double*)0x00850FD8 = framelength;
    _asm ret;
}

LARGE_INTEGER TimerStart_Hybrid()
{
    old_start = startTime;


    QueryPerformanceCounter(&startTime);
    old_start.QuadPart = startTime.QuadPart - old_start.QuadPart;
    double ms = (double((old_start.LowPart)) * fFreq);
    if (ms < 32.0)
    {
        framelength = ms;

        if (ms >= 16.8) // ~59.88
        {
            if(hybrid_limit)
                hybrid_limit--;
        }
    }
    return startTime;
}

LARGE_INTEGER TimerStart()
{
    old_start = startTime;


    QueryPerformanceCounter(&startTime);
    old_start.QuadPart = startTime.QuadPart - old_start.QuadPart;
    double ms = (double((old_start.LowPart)) * fFreq);
    if (ms < 32.0)
    {
        if (ms > 16.675)//59,97 fps
        {
            _printf("Dec\n");
            frameticks-=2;
        }
        else if (ms < 16.60)//60,24 fps
        {
            _printf("Inc\n");
            frameticks++;
        }
        framelength = ms;
    }
    return startTime;
}

LARGE_INTEGER TimerStart_Sleep()
{
    old_start = startTime;


    QueryPerformanceCounter(&startTime);
    old_start.QuadPart = startTime.QuadPart - old_start.QuadPart;
    double ms = (double((old_start.LowPart)) * fFreq);
    if (ms < 32.0)
    {
        framelength = ms;
        //_printf("2nd %f ", ms);

        //We need to cap FPS around 60 because else some physics and scripts will not work correctly
        //Also this is the most fair in a game heavily dependant on speed etc
        //Maybe in the future can change this so can have more FPS
        //Vsync is always on when we are using the new timer
        //So on a screen with 60hz this will not matter too much because we will get perfect 60 FPS
        //However on my screen with 144hz it's pretty hard to get consistent 60 FPS
        //Currently worst case scenario is 59.9-65 and it's usually around 63-64
        //For some reason it's more consistent in window mode than in fullscreen mode
        if (ms >= 16.39) // ~61.01 FPS
        {
            BYTE target_ms = p_target_ms;
            if (target_ms > 1)
            {
                target_ms--;
                p_target_ms = target_ms;
            }
        }
        else if (ms < 15.38) // ~65.02 FPS
        {
            BYTE target_ms = p_target_ms;
            if (target_ms < timer_lock)
            {
                target_ms++;
                p_target_ms = target_ms;
            }
        }
    }
    return startTime;
}

void OilRigGrindPatch()
{
    Vertex* __restrict pVel;
   _asm  mov pVel, edi;
   if (Game::level_checksum != Checksums::Oil)
       return;
   //If we have speed greater than 100, rotate the velocity so we don't grind backwards on steep rails
   if(((pVel->x * pVel->x) + (pVel->z * pVel->z)) > 100.0f)
       pVel->RotateToPlane(Vertex(0, 1.0f, 0));
}

__declspec (naked) void HookEmptyRailNodeData()
{
    static RailNode** first;
    _asm push ecx;
    _asm add ecx, 0x48;
    _asm mov ecx, [ecx];
    _asm mov first, ecx
    RailManager::AllocateTempRailData(first);
    _asm pop ecx;
    _asm lea eax, [ecx + 0x00000118];
    _asm ret;
}

void HookOverlappingRailRemoval()
{
    RailManager::FixRailLinks();
    RailManager::RemoveOverlapping();
    /*typedef void(__cdecl* const pOverlappingRailRemoval)(); 
    pOverlappingRailRemoval(0x00496090)();*/
}

__declspec(naked) void FixMemOpt1()
{
    static DWORD skip = 0x00496D20;
    static DWORD jmpBack = 0x004967CD;
    _asm je skip_label;
    _asm mov edi, EndOfRail
    _asm jmp [jmpBack];
skip_label:
    _asm jmp[skip];
}

__declspec(naked) void FixMemOpt2()
{
    static DWORD skip = 0x00496172;
    static DWORD jmpBack = 0x00496163;
    _asm je skip_label;
    _asm mov edi, EndOfRail
    _asm mov ecx, [esp + 18];
    _asm jmp[jmpBack];
skip_label:
    _asm jmp[skip];
}

bool force_rail_check = false;
void Skater::PointRail(const Vertex& rail_pos)
{
    // for a single node rail, we apply in a single frame all the effects of enteringand exiting the rail state;

    /////////////////////////////////////////////////////
    // Emulate entering the rail state (with horizontal dir direction)

    // check for collision in moving from m_pos to rail_pos

    startcol = *GetPosition();
    endcol = rail_pos;
    if (CollisionCheck())
    {
        // check distance from the rail to the collision point
        Vertex temp = rail_pos - *(Vertex*)GetHitPoint();
        if (temp.LengthSqr() > 6.0f * 6.0f)
        {
            return;
        }
    }

    // check first if we are not moving much in the XY and if not, then se the XZ velocity to the matrix[Z], so we always go forward
    if (fabsf(*GetVelocity()[X]) < 0.01f && fabsf(*GetVelocity()[Z]) < 0.01f)
    {
        *GetVelocity()[X] = GetMatrix().m[Z][X];
        *GetVelocity()[Z] = GetMatrix().m[Z][Z];
    }

    if (((GetVelocity()->x * GetVelocity()->x) + (GetVelocity()->z * GetVelocity()->z)) > 100.0f)
        ((Vertex*)GetVelocity())->RotateToPlane(Vertex(0, 1.0f, 0));


    // rail direction is taken to always simply be along our horizontal velocity, rotated up
    Vertex dir = *GetVelocity();
    dir[Y] = 0.0f;
    dir.Normalize();
    float angle = DegToRad(Physics::Physics_Point_Rail_Kick_Upward_Angle);
    float c = cosf(angle);
    float s = sinf(angle);
    Vertex boost_dir(c * dir[X], s, c * dir[Z]);


    DWORD trigger_script = 0;
    CStruct* pNode;

    if (mp_rail_node->GetNode() != -1)
    {
        // get the rail node name
        CArray* pNodeArray = Node::GetNodeArray();
        pNode = pNodeArray->GetStructure(mp_rail_node->GetNode());
        pNode->GetChecksum(Checksums::Name, &m_last_rail_node_name, QScript::ASSERT);

        //TrickOffObject(m_last_rail_node_name);

        // Now we want to see if the rail has a trigger, and if it does, trigger it....

        // no need to call maybe_trip_rail_trigger for a single node rail
        if (pNode->GetChecksum(Checksums::TriggerScript, &trigger_script))
        {
            TripTrigger(
                Node::TRIGGER_LAND_ON,
                trigger_script,
                mp_rail_node->GetNode(),
                pNode
            );
        }
    }

    *GetPosition() = rail_pos;
    *GetOldPosition() = rail_pos;

    // Now we'v got onto the rail, we need to:
    // 1) kill velocity perpendicular to the rail
    // 2) add a speed boost in the direction we are going.

    SetFlag(&inVert, false);
    SetFlag(&tracking, false);

    // if we are transitioning from wall to rail, then snap him upright		
    if (m_state == WALL)
    {
        D3DXVECTOR4 normal = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
        SetNormal(normal);
        ResetLerpingMatrix();
    }

    SetState(RAIL);

    set_terrain(mp_rail_node->GetTerrain());
    //mp_sound_component->PlayLandSound(GetObject()->GetVel().Length() / GetSkater()->GetScriptedStat(CRCD(0xcc5f87aa, "Skater_Max_Max_Speed_Stat")), mp_state_component->m_terrain);

    float old_y = *GetVelocity()[Y];
    (*(Vertex*)GetVelocity()).ProjectToNormal(dir);	   							// kill perp velocity
    *GetVelocity()[Y] = old_y;											// except for Y

    *GetVelocity() += dir * Physics::Point_Rail_Speed_Boost;	// add speed boost			


    // (Mick) Set m_rail_time, otherwise there is a single frame where it is invalid
    // and this allows us to immediately re-rail and hence do the "insta-bail", since the triangle button will be held down   
    m_rail_time = GetTime();
    //_asm mov m_rail_time2, edx;

    /////////////////////////////////////////////////////
    // Emulate effects of rail state (with boost_dir direction)

    Slerp::slerping = false;
    Slerp::transfer = false;
    Slerp::done = true;

    (*(Vertex*)GetVelocity()).RotateToNormal(boost_dir);

    /////////////////////////////////////////////////////
    // Emulate exiting the rail state

    // no need to call maybe_trip_rail_trigger for a single node rail
    if (trigger_script)
    {
        TripTrigger(
            Node::TRIGGER_SKATE_OFF,
            trigger_script,
            mp_rail_node->GetNode(),
            pNode
        );
    }

    SetState(AIR);
    *GetPosition()[Y] += 1.0f;

    /////////////////////////////////////////////////////
    // Do extra point rail logic

    // trigger the appropriate 
    CStruct pStruct;
    CScript pScript;

    CStructHeader header;
    header.Type = QBKeyHeader::LOCAL;
    header.value.i = Checksum("Airborne");
    pStruct.head = &header;
    pStruct.tail = &header;

    AddTrick("Kissed the Rail", 50, TrickType::Grind);
    CallMemberFunction(Checksum("MakeSkaterGoto"), &pStruct, &pScript);

    force_rail_check = true;
    return;
}

extern DWORD GetElapsedTime(DWORD currentTime, LARGE_INTEGER last_time);
bool Skater::will_take_rail()
{


    return (!force_rail_check || (GetElapsedTime(GetTime(), *(LARGE_INTEGER*)&m_rail_time) > 500))
            && (m_state != RAIL 									// not already on a rail
                && (!tracking || *GetVelocity()[Y] > 0.0f));		// must be not vert, or going up 
}

void Skater::got_rail_hook()
{
    typedef void(__thiscall* p_got_rail)(Skater* pThis);

    if (will_take_rail())
    {
        force_rail_check = false;
        p_got_rail(0x004A60F0)(this);
    }
}
Vertex v;

__declspec(naked) void CheckForPointRail_Hook()
{
    static DWORD jmpBack = 0x004A6571;
    static DWORD ret = 0x004A6E6F;
    static Skater* pSkater;
    static DWORD terrain;
    static DWORD pCall = 0x0049BA80;
    static Vertex& param = v;

    _asm mov pSkater, ecx;
    _asm call pCall;
    if (pSkater->mp_rail_node->GetNextLink())
    {
        _asm mov ecx, pSkater;
        _asm jmp[jmpBack];
    }
    else
    {
        _asm add esp, 0x18;
        _asm lea eax, [esp];
        _asm sub esp, 0x18;
        _asm mov param, eax;
        pSkater->PointRail(param);
        _asm jmp[ret];
    }
}

void CheatDetected()
{
    CStruct params;
    CStructHeader param(QBKeyHeader::STRING, Checksums::text, stat_cheat_message);
    params.AddParam(&param);
    ExecuteQBScript("LaunchGrafCounter", &params);
}

void InitLevelMod()
{
    //HookControls();


    //Initializing the new timer
    QueryPerformanceFrequency(&freq);
    fFreq = 1000.0 / (double)freq.QuadPart;
    frameticks = (DWORD)(16666.666666666 / (1000000.0 / (double)freq.QuadPart));
    //Add some headroom for hardware delay and rounding errors, probably should add this to ini for platform specific stored value
    frameticks -= 750;
    DWORD frameticks2 = (DWORD)(16.666666666 / fFreq);
    DWORD frameticks3 = 0.0166666666 / (1.0 / (double)freq.QuadPart);

    _printf("f1 %u f2 %u f3 %u\n", frameticks, frameticks2, frameticks3);

    if (!p_bWindowed)
        timer_lock = 0x0D;

    /*_printf("%d %f", freq.LowPart, fFreq);
    MessageBox(0, 0, 0, 0);*/


    DWORD old;
    //HookFunction(0x004C04F0, TimerElapsed);
    HookFunction(0x004F9463, UpdateFrameLength);

    //Fix menu crashing
    VirtualProtect((LPVOID)0x004CEDE4, 8, PAGE_EXECUTE_READWRITE, &old);
    /**(BYTE*)0x004CEDE8 = 0xE9;
    *(DWORD*)0x004CEDE9 = 0x0000008E;
    *(BYTE*)0x004CEDED = 0x90;
    *(WORD*)0x004CEDEE = 0x9090;*/

    //Change the RailNode size
    BYTE optimize_grind[] = { 0xB8, 0x2C, 0x01, 0x00, 0x00, 0x89, 0x86, 0xB8, 0x84, 0x00, 0x00, 0xEB, 0x14, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xD9 };
    InjectHook(0x004A4C18, optimize_grind, sizeof(optimize_grind));
    BYTE optimize_grind2[] = { 0xB8, 0xF4, 0x01, 0x00, 0x00, 0x53, 0x8D, 0x8E, 0x60, 0x83, 0x00, 0x00, 0x89, 0x86, 0xB8, 0x84, 0x00, 0x00, 0xEB, 0x14, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    InjectHook(0x004A4A8F, optimize_grind2, sizeof(optimize_grind2));
    //004A60E1

    HookFunction(0x004A8B1A, &Skater::got_rail_hook);
    HookFunction(0x004A656D, &CheckForPointRail_Hook, 0xE9);
    //Cheat detection
    BYTE CheatDetection[]{ 0x8B, 0x44, 0x24, 0x04, 0x56, 0x8B, 0xF1, 0x81, 0xBC, 0x86, 0xD0, 0x83, 0x00, 0x00, 0x00, 0x00, 0x80, 0xBF, 0x75, 0x11, 0x6A, 0x01, 0x68, 0x78, 0x4C, 0x5C, 0x00, 0xE8, 0xEF, 0x6F, 0xF8, 0xFF, 0x83, 0xC4, 0x08, 0xEB, 0x2A, 0xD9, 0x84, 0x86, 0xD0, 0x83, 0x00, 0x00, 0xD8, 0x15, 0xFC, 0xF3, 0x49, 0x00, 0xDF, 0xE0, 0x66, 0xA9, 0x00, 0x41, 0x75, 0x15, 0x90, 0x90, 0x90, 0x90, 0x90, 0xEB, 0x0E, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x00, 0x00, 0x20, 0x41, 0xD9, 0x5C, 0x24, 0x08, 0x8B, 0xCE, 0x6A, 0xEC, 0xE8, 0x03, 0x5F, 0x01, 0x00, 0x84, 0xC0, 0x74, 0x08, 0xD9, 0x05, 0x3C, 0xF4, 0x49, 0x00, 0xEB, 0x04, 0xD9, 0x44, 0x24, 0x08, 0x8B, 0xB6, 0x68, 0x92, 0x00, 0x00, 0x85, 0xF6, 0x74, 0x0D, 0x8A, 0x46, 0x50, 0x84, 0xC0, 0x74, 0x06, 0xD8, 0x05, 0x38, 0xF4, 0x49, 0x00, 0x5E, 0xC2, 0x04, 0x00 , 0x00 , 0x00 , 0x40 , 0x40 , 0x00, 0x00 , 0x70 , 0x41 };
    InjectHook(0x0049F3B1, CheatDetection, sizeof(CheatDetection));
    HookFunction(0x0049F3EC, CheatDetected, 0xE8);
    HookFunction(0x0049F563, 0x0049F3B1);
    HookFunction(0x0049F5B8, 0x0049F3B1);
    HookFunction(0x004AB187, 0x0049F3B1);
    HookFunction(0x004AB1AB, 0x0049F3B1);
    HookFunction(0x004AB1C9, 0x0049F3B1);
    HookFunction(0x004AB1E7, 0x0049F3B1);
    HookFunction(0x004AB205, 0x0049F3B1);
    HookFunction(0x004AB223, 0x0049F3B1);
    HookFunction(0x004AB241, 0x0049F3B1);
    HookFunction(0x004AB25F, 0x0049F3B1);
    HookFunction(0x004AB27D, 0x0049F3B1);
    HookFunction(0x004AB29B, 0x0049F3B1);
    //MessageBox(0, 0, 0, 0);

    //Remove dublicate NodeArray loading...
    BYTE nop_func[] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
    InjectHook(0x0043A6D6, nop_func, 5);
    //InjectHook(0x00419D07, nop_func, 5);

    InjectHook(0x0049D15D, nop_func, 5);
    InjectHook(0x0049D180, nop_func, 5);

    //Add RailNode to list
    BYTE AddRailFix[] = { 0x50, 0x51, 0x52, 0x56, 0x57, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x5A, 0x59, 0x58, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
    VirtualProtect((LPVOID)0x00419AB5, sizeof(AddRailFix), PAGE_EXECUTE_READWRITE, &old);
    memcpy((void*)0x00419AB5, AddRailFix, sizeof(AddRailFix));
    HookFunction(0x00419ABB, &RailManager::AddRailNode);
    
    //Empty RailNode list
    HookFunction(0x004198B8, HookEmptyRailNodeData);

    //Remove overlapping rails and finalize RailNode list
    HookFunction(0x00419BCF, HookOverlappingRailRemoval);

    //This code is not really used....
    BYTE MemOptFix1_array[] = { 0x83, 0xC6, 0x44, 0x39, 0xFE };
    BYTE MemOptFix2_array[] = { 0x83, 0xC0, 0x44, 0x39, 0xF8 };
    VirtualProtect((LPVOID)0x0049616B, sizeof(MemOptFix1_array), PAGE_EXECUTE_READWRITE, &old);
    memcpy((void*)0x0049616B, MemOptFix2_array, sizeof(MemOptFix1_array));
    VirtualProtect((LPVOID)0x00496CD8, sizeof(MemOptFix1_array), PAGE_EXECUTE_READWRITE, &old);
    memcpy((void*)0x00496CD8, MemOptFix1_array, sizeof(MemOptFix1_array));
    VirtualProtect((LPVOID)0x00496BAB, 1, PAGE_EXECUTE_READWRITE, &old);
    *(BYTE*)0x00496BAB = 0x94;
    VirtualProtect((LPVOID)0x00496BBC, 1, PAGE_EXECUTE_READWRITE, &old);
    *(BYTE*)0x00496BBC = 0xD2;
    HookFunction(0x004967C8, FixMemOpt1, 0xE9, 1);
    HookFunction(0x0049615E, FixMemOpt2, 0xE9, 1);  

    //New RailManager functions
    HookFunction(0x004A480C, RailManager::StickToRail);
    HookFunction(0x004A6246, RailManager::StickToRail);

    HookFunction(0x004189CD, RailManager::SetActive);
    HookFunction(0x00418C5F, RailManager::SetActive);

    HookFunction(0x0041D1D4, RailManager::Cleanup);
    HookFunction(0x00438344, RailManager::Cleanup);
    HookFunction(0x00495711, RailManager::Cleanup);

    HookFunction(0x004A540C, Skater::skate_off_rail);
    //HookFunction(0x004A5424, maybe_skate_off_rail, 0xE9);

    //This is needed to fix crash issue when multiple links to same RailNode
    HookFunction(0x004A4A00, &Skater::maybe_trip_rail_trigger);
    HookFunction(0x004A4C14, &Skater::maybe_trip_rail_trigger);
    HookFunction(0x004A5403, &Skater::maybe_trip_rail_trigger);
    HookFunction(0x004A5A6A, &Skater::maybe_trip_rail_trigger);
    HookFunction(0x004A5BD1, &Skater::maybe_trip_rail_trigger);
    HookFunction(0x004A6816, &Skater::maybe_trip_rail_trigger);
    HookFunction(0x004A70BB, &Skater::maybe_trip_rail_trigger);
    HookFunction(0x004B088C, &Skater::maybe_trip_rail_trigger);
    HookFunction(0x00489C7F, &RailNode::ProbablyOnSameRailAs);

    HookFunction(0x00403C13, GetTime);
    HookFunction(0x00403D61, GetTime);
    HookFunction(0x00403F29, GetTime);
    HookFunction(0x00404086, GetTime);
    HookFunction(0x0040409A, GetTime);
    HookFunction(0x00405824, GetTime);
    HookFunction(0x004058C1, GetTime);
    HookFunction(0x0040864A, GetTime);
    HookFunction(0x0040A1E7, GetTime);
    HookFunction(0x0040A209, GetTime);
    HookFunction(0x00417131, GetTime);
    HookFunction(0x004171CA, GetTime);
    HookFunction(0x00419764, GetTime);
    HookFunction(0x004279BE, GetTime);
    HookFunction(0x00428022, GetTime);
    HookFunction(0x0042F0C4, GetTime);
    HookFunction(0x0043A3DF, GetTime);
    HookFunction(0x00441004, GetTime);
    HookFunction(0x004615AF, GetTime);
    HookFunction(0x0046E635, GetTime);
    HookFunction(0x0046E788, GetTime);
    HookFunction(0x0046E7E7, GetTime);
    HookFunction(0x0046E933, GetTime);
    HookFunction(0x0046E9D1, GetTime);
    HookFunction(0x0046F4B9, GetTime);
    HookFunction(0x004712F2, GetTime);
    HookFunction(0x00471901, GetTime);
    HookFunction(0x00473124, GetTime);
    HookFunction(0x00473989, GetTime);
    HookFunction(0x004740C6, GetTime);
    HookFunction(0x0047418E, GetTime);
    HookFunction(0x00474493, GetTime);
    HookFunction(0x004745DD, GetTime);
    HookFunction(0x00474649, GetTime);
    HookFunction(0x004750B6, GetTime);
    HookFunction(0x004751B9, GetTime);
    HookFunction(0x004754F9, GetTime);
    HookFunction(0x00475744, GetTime);
    HookFunction(0x0047699C, GetTime);
    HookFunction(0x00479098, GetTime);
    HookFunction(0x0047BCF5, GetTime);
    HookFunction(0x0047D172, GetTime);
    HookFunction(0x0047D1A1, GetTime);
    HookFunction(0x0047DEBF, GetTime);
    HookFunction(0x0047E6B4, GetTime);
    HookFunction(0x0047E72E, GetTime);
    HookFunction(0x0047E757, GetTime);
    HookFunction(0x0047EB75, GetTime);
    HookFunction(0x0047F427, GetTime);
    HookFunction(0x0047F449, GetTime);
    HookFunction(0x0047FE62, GetTime);
    HookFunction(0x0048154F, GetTime);
    HookFunction(0x00481624, GetTime);
    HookFunction(0x00487978, GetTime);
    HookFunction(0x00487A4B, GetTime);
    HookFunction(0x00487A63, GetTime);
    HookFunction(0x00487C36, GetTime);
    HookFunction(0x0048858E, GetTime);
    HookFunction(0x00489E48, GetTime);
    HookFunction(0x0048A026, GetTime);
    HookFunction(0x0048E4F0, GetTime);
    HookFunction(0x00498993, GetTime);
    HookFunction(0x004989EB, GetTime);
    HookFunction(0x00499AD9, GetTime);
    HookFunction(0x00499ECA, GetTime);
    HookFunction(0x00499EE9, GetTime);
    HookFunction(0x0049AEE6, GetTime);
    HookFunction(0x0049AFCF, GetTime);
    HookFunction(0x0049B183, GetTime);
    HookFunction(0x0049B282, GetTime);
    HookFunction(0x0049B793, GetTime);
    HookFunction(0x0049B85F, GetTime);
    HookFunction(0x0049BA16, GetTime);
    HookFunction(0x0049BAB6, GetTime);
    HookFunction(0x0049BACD, GetTime);
    HookFunction(0x0049BFB4, GetTime);
    HookFunction(0x0049C222, GetTime);
    HookFunction(0x0049C613, GetTime);
    HookFunction(0x0049C63B, GetTime);
    HookFunction(0x0049C686, GetTime);
    HookFunction(0x0049C6B8, GetTime);
    HookFunction(0x0049C6F6, GetTime);
    HookFunction(0x0049C734, GetTime);
    HookFunction(0x0049C767, GetTime);
    HookFunction(0x0049C7A5, GetTime);
    HookFunction(0x0049C910, GetTime);
    HookFunction(0x0049CBB4, GetTime);
    HookFunction(0x0049CBE1, GetTime);
    HookFunction(0x0049D367, GetTime);
    HookFunction(0x0049D386, GetTime);
    HookFunction(0x0049D3A5, GetTime);
    HookFunction(0x0049D3D5, GetTime);
    HookFunction(0x0049D421, GetTime);
    HookFunction(0x0049D666, GetTime);
    HookFunction(0x0049D688, GetTime);
    HookFunction(0x0049D6AA, GetTime);
    HookFunction(0x0049D6E9, GetTime);
    HookFunction(0x0049E3FA, GetTime);
    HookFunction(0x0049EFC8, GetTime);
    HookFunction(0x0049EFF7, GetTime);
    HookFunction(0x0049F019, GetTime);
    HookFunction(0x0049F035, GetTime);
    HookFunction(0x0049F243, GetTime);
    HookFunction(0x0049F35C, GetTime);
    HookFunction(0x0049F37B, GetTime);
    HookFunction(0x0049F6D6, GetTime);
    HookFunction(0x0049F7C0, GetTime);
    HookFunction(0x0049F912, GetTime);
    HookFunction(0x0049F9BA, GetTime);
    HookFunction(0x0049FB3B, GetTime);
    HookFunction(0x004A11B1, GetTime);
    HookFunction(0x004A11DA, GetTime);
    HookFunction(0x004A19DB, GetTime);
    HookFunction(0x004A2148, GetTime);
    HookFunction(0x004A216A, GetTime);
    HookFunction(0x004A218C, GetTime);
    HookFunction(0x004A258E, GetTime);
    HookFunction(0x004A2971, GetTime);
    HookFunction(0x004A2999, GetTime);
    HookFunction(0x004A29D8, GetTime);
    HookFunction(0x004A3328, GetTime);
    HookFunction(0x004A3653, GetTime);
    HookFunction(0x004A38CA, GetTime);
    HookFunction(0x004A54D5, GetTime);
    HookFunction(0x004A61DB, GetTime);
    HookFunction(0x004A6407, GetTime);
    HookFunction(0x004A7C27, GetTime);
    HookFunction(0x004A7C67, GetTime);
    HookFunction(0x004A8B5A, GetTime);
    HookFunction(0x004A90CD, GetTime);
    HookFunction(0x004AA670, GetTime);
    HookFunction(0x004AC942, GetTime);
    HookFunction(0x004AC955, GetTime);
    HookFunction(0x004ACA65, GetTime);
    HookFunction(0x004ACAC7, GetTime);
    HookFunction(0x004AD218, GetTime);
    HookFunction(0x004ADD41, GetTime);
    HookFunction(0x004AE5B1, GetTime);
    HookFunction(0x004AE84B, GetTime);
    HookFunction(0x004AF97C, GetTime);
    HookFunction(0x004AF9A2, GetTime);
    HookFunction(0x004AFBA2, GetTime);
    HookFunction(0x004AFFB8, GetTime);
    HookFunction(0x004B0797, GetTime);
    HookFunction(0x004B0E52, GetTime);
    HookFunction(0x004B1975, GetTime);
    HookFunction(0x004B2FBD, GetTime);
    HookFunction(0x004B3138, GetTime);
    HookFunction(0x004B321B, GetTime);
    HookFunction(0x004B3363, GetTime);
    HookFunction(0x004B359F, GetTime);
    HookFunction(0x004B389C, GetTime);
    HookFunction(0x004B3B16, GetTime);
    HookFunction(0x004B3C91, GetTime);
    HookFunction(0x004B3DD2, GetTime);
    HookFunction(0x004B3F5B, GetTime);
    HookFunction(0x004B4147, GetTime);
    HookFunction(0x004B43C1, GetTime);
    HookFunction(0x004B45BD, GetTime);
    HookFunction(0x004B486C, GetTime);
    HookFunction(0x004B499E, GetTime);
    HookFunction(0x004B49F4, GetTime);
    HookFunction(0x004B4FCF, GetTime);
    HookFunction(0x004C04AC, GetTime);
    HookFunction(0x004D88E4, GetTime);
    HookFunction(0x004D8ED3, GetTime);
    HookFunction(0x004DA359, GetTime);
    HookFunction(0x004DAAAA, GetTime);
    HookFunction(0x004DD99D, GetTime);
    HookFunction(0x004DD9B4, GetTime);
    HookFunction(0x004DD9C4, GetTime);
    HookFunction(0x004DFF25, GetTime);
    HookFunction(0x004E062F, GetTime);
    HookFunction(0x004E06DB, GetTime);
    HookFunction(0x004E07D0, GetTime);
    HookFunction(0x004EA1ED, GetTime);
    HookFunction(0x004EA2C0, GetTime);
    HookFunction(0x004EE836, GetTime);
    HookFunction(0x004EF333, GetTime);
    HookFunction(0x004F24BA, GetTime);
    HookFunction(0x004F36F5, GetTime);
    HookFunction(0x004F3BA1, GetTime);
    HookFunction(0x004F3D07, GetTime);
    HookFunction(0x004F40AB, GetTime);
    HookFunction(0x004FA86D, GetTime);
    HookFunction(0x00502BA0, GetTime);
    HookFunction(0x00502BE2, GetTime);

    HookFunction(0x0049FAA1, &Skater::PlayJumpSound);
    HookFunction(0x0049FAC1, &Skater::PlayJumpSound);
    HookFunction(0x0049FB1F, &Skater::PlayJumpSound);
    HookFunction(0x004AEEBC, &Skater::PlayJumpSound);
    HookFunction(0x004AEED1, &Skater::PlayJumpSound);

    //Not really used?
    *(BYTE*)0x004960E7 = 0xEB;
    
    //Remove the old memory "optimization"
    VirtualProtect((LPVOID)0x00495750, 2, PAGE_EXECUTE_READWRITE, &old);
    *(WORD*)0x00495750 = 0x90C3;

    //Initialize the new RailManager
    RailManager::Initialize();

    //Sound pan volume fix
    /*HookFunction(0x004130C0, &SfxManager::SetVolumeFromPosHook);
    HookFunction(0x004AAD61, &SfxManager::SetVolumeFromPosHook);
    HookFunction(0x004C4777, &SfxManager::SetVolumeFromPosHook);
    HookFunction(0x004C5B36, &SfxManager::SetVolumeFromPosHook);
    HookFunction(0x004C601E, &SfxManager::SetVolumeFromPosHook);
    HookFunction(0x004C62E8, &SfxManager::SetVolumeFromPosHook);*/

    //This fixes the grinding on the crane in OilRig
    BYTE oil_grind_fix[] = { 0x8D, 0xBE, 0x34, 0x03, 0x00, 0x00, 0xD9, 0x07, 0xD8, 0x1D, 0x5C, 0xD8, 0x58, 0x00, 0xDF, 0xE0, 0xF6, 0xC4, 0x44, 0x7A, 0x1B, 0xD9, 0x47, 0x08, 0xD8, 0x1D, 0x5C, 0xD8, 0x58, 0x00, 0xDF, 0xE0, 0xF6, 0xC4, 0x44, 0x7A, 0x0B, 0x8B, 0x47, 0x0, 0x8B, 0x4F, 0x48, 0x89, 0x07, 0x89, 0x4F, 0x08, 0xE8, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    VirtualProtect((LPVOID)0x004A66D0, sizeof(oil_grind_fix), PAGE_EXECUTE_READWRITE, &old);
    memcpy((void*)0x4a66d0, oil_grind_fix, sizeof(oil_grind_fix));
    HookFunction(0x004A6701, OilRigGrindPatch);

    //backup the original fps limiter timer
    VirtualProtect((LPVOID)0x004C04E3, sizeof(LevelModSettings::original_timer), PAGE_EXECUTE_READWRITE, &old);
    memcpy(LevelModSettings::original_timer, (void*)0x004C04E3, sizeof(LevelModSettings::original_timer));
    //Fix stutter if it's enabled
    MaybeFixStutter();

    VirtualProtect((LPVOID)0x00400019, 6, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x0042FA0D, 9, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x0042FA9D, 19, PAGE_EXECUTE_READWRITE, &old);
    BYTE codeCaveRenderHook[] = { 0xC6, 0x05, 0x19, 0x00, 0x40, 0x00, 0x01, 0x90, 0x90 };
    BYTE codeCaveRenderHook2[] = { 0x5E, 0x5D, 0xC6, 0x05, 0x19, 0x00, 0x40, 0x00, 0x00, 0xEB, 0x08, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    memcpy((void*)0x0042FA0D, codeCaveRenderHook, sizeof(codeCaveRenderHook));
    memcpy((void*)0x0042FA9D, codeCaveRenderHook2, sizeof(codeCaveRenderHook2));

    //Fixing bug that produces duplicate TriggerScripts
    HookFunction(0x00499B48, TriggerScript, 0xE9);

    for (DWORD i = 0; i < sizeof(optimized_grind) / sizeof(OptimizedGrind); i++)
    {
       optimized_grind[i].Optimize();
    }

    //BouncyObject fixes
    BYTE codeCaveBouncyObject[] = { 0x84, 0xC0, 0x0F, 0x84, 0xA0, 0x3E, 0x08, 0x00, 0x8A, 0x84, 0x24, 0x00, 0x01, 0x00, 0x00, 0x24, 0x40, 0x74, 0x0D, 0x8B, 0x84, 0x24, 0x14, 0x01, 0x00, 0x00, 0x89, 0x05, 0xCB, 0x03, 0x40, 0x00, 0x8A, 0x84, 0x24, 0x00, 0x01, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90, 0x24, 0x10, 0x0F, 0x85, 0x74, 0x3E, 0x08, 0x00, 0xE9, 0x0D, 0x3A, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00 };
    DWORD BouncyObject_Addr = 0x00400392;
    VirtualProtect((LPVOID)BouncyObject_Addr, sizeof(codeCaveBouncyObject), PAGE_EXECUTE_READWRITE, &old);
    memcpy((void*)BouncyObject_Addr, codeCaveBouncyObject, sizeof(codeCaveBouncyObject));
    HookFunction(0x004003BA, BouncyObj_OnBounce_Naked, 0xE9);

    //Rendering of models hook, currently used to render custom shattered meshes
    HookFunction(0x004F9C0A, Render_Naked, 0xE9);

    VirtualProtect((LPVOID)0x00483D55, 1, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00483DD0, 1, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00483DD1, 7, PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((LPVOID)0x004092AB, 6, PAGE_EXECUTE_READWRITE, &old);

    *(DWORD*)0x005CEC78 *= 4;

    *(BYTE*)0x00483D55 = 0x0;
    *(BYTE*)0x00483DD0 = 0xE9;
    *(DWORD*)0x00483DD1 = 0xFFF7C5BD;
    *(BYTE*)0x00483DD5 = 0x90;
    *(BYTE*)0x00483DD6 = 0x90;
    *(BYTE*)0x00483DD7 = 0x90;


    //Fix to add a MessageBox when call a script that's not found instead of crashing game
    *(WORD*)0x00427A9B = 0x840F;//je
    HookFunction(0x00427A9D, NotGood_naked);

    //Hook to add new effects, blendmodes, anims to static scene meshes
    HookFunction(0x004F42AA, SetVertexShader_hook);

    //Fix the snap to ground issue
    HookFunction(0x0049F1DD, proxy_SnapToGroundClamp);

    //Currently used for alt+enter toggle windowed mode
    HookFunction(0x00403C75, proxy_GetAsyncKeyState, 0xE8, 1);

    /*//Hook dinput
    HookFunction(0x0040CA2B, proxy_Dinput_GetDeviceState);*/

    //Used for stored windowed position
    *(BYTE*)0x004092AB = 0xBD;
    *(DWORD*)0x004092AC = PtrToUlong(proxy_GetMessage);
    *(BYTE*)0x004092B0 = 0x90;

    //HookFunction(0x004D575A, proxy_sprintf, 0xE8, 1);

    //Fix SuperSector size limitations and crashing issues + improve performance of GetSuperSector function
    HookFunction(0x00412160, SuperSector::GetSuperSector);
    HookFunction(0x00412278, SuperSector::GetSuperSector);
    HookFunction(0x004122C7, SuperSector::GetSuperSector);
    HookFunction(0x00412307, SuperSector::GetSuperSector);
    HookFunction(0x0041236B, SuperSector::GetSuperSector);
    HookFunction(0x0041246B, SuperSector::GetSuperSector);

    //Hook to add new effects, blendmodes, anims to non static scene meshes
    HookFunction(0x00526D4B, Obj_SetShader_hook);

    *(DWORD*)0x00427AA3 = 0x90909090;//nop
    *(WORD*)(0x00427AA3 + 4) = 0x9090;

    //Temporarly commented out Obj_ functions until figure out why it causes issues
    //HookFunction(0x0048E036, Obj_MoveToNode_Naked, 0xE9);
    //HookFunction(0x0048DA53, Obj_FollowPathLinked_Naked, 0xE9);
    HookFunction(0x004846E6, BouncyObj_Go_Naked, 0xE8);

    //Hooks to add custom net messages
    HookFunction(0x00474D08, &Network::MessageHandler::AddClientMessages);
    HookFunction(0x004751EA, &Network::MessageHandler::AddServerMessages);

    //Old code
    //HookFunction(0x0058D0B1, Fopen_naked, 0xE9);
    /*if(!QScript::Scripts)
        QScript::Scripts = new QScript::QBScript();*/

    //If debugmode is enabled we want to hook checksum generating function
    if (bDebugMode)
        HookFunction(0x004265F1, Checksum_naked, 0xE9);
    else
    {
        //Else we want to optimize the checksum
        DWORD old;
        for (DWORD i = 0; i < sizeof(optimized2) / 4; i++)
        {
            DWORD addr = optimized2[i] + 1;
            VirtualProtect((LPVOID)addr, 4, PAGE_EXECUTE_READWRITE, &old);
            *(DWORD*)addr = 0x004110E0 - addr - 4;
        }
    }

    /*VirtualProtect((LPVOID)0x004265D9, 3 + 4 * 4, PAGE_EXECUTE_READWRITE, &old);

    *(WORD*)0x004265D9 = 0x31FF;
    *(BYTE*)0x004265DB = 0xE8;
    *(DWORD*)0x004265DC = 0xFFFFFFB1;
    *(DWORD*)0x004265E0 = 0xC308C483;
    *(DWORD*)0x004265E4 = 0x90909090;
    *(DWORD*)0x004265E8 = 0x90909090;*/

    //Optimize the static array checksum access
    for (DWORD i = 0; i < sizeof(optimized3) / sizeof(OptimizedArrayCRC); i++)
    {
        optimized3[i].Optimize();
    }

    //This uses original code that was made for thps2 and used even in thug2
    //It's very predictable and will yield almost same result everytime you relaunch game
    //With this random init atleast it will yield same result every 49 days
    //Maybe should implement more "true" randomness?
    InitialRand(GetTickCount());
    KeyMap::UpdateKeyMap();
    QBKeyHeader* header = NULL;


    //Hook DumpScripts ScriptFunction, this function is empty in thps3 PC
    //However it's there in XBOX version of thps3
    //This is a reproduced version to view similar info
    header = GetQBKeyHeader(Checksum("DumpScripts"));
    if (header)
    {
        header->pFunction = DumpScripts;
    }

    /*header = GetQBKeyHeader(Checksum("DisplayLoadingScreen"));
    {
        pDisplayLoadingScreen = header->pFunction;
        header->pFunction = DisplayLoadingScreen;
    }

    header = GetQBKeyHeader(Checksum("HideLoadingScreen"));
    {
        pHideLoadingScreen = header->pFunction;
        header->pFunction = HideLoadingScreen;
    }*/

}




EXTERN void SendChatMsg(char* text)
{
    DEBUGSTART()
    {
        int Param3;

        typedef int(__cdecl* pFixStackAddress)(BYTE);
        pFixStackAddress pFixStack = (pFixStackAddress)(0x00471C60);
        Param3 = pFixStack(0);

        typedef void(__cdecl* pSendChatMsgAddress)(char* text, int Param2, int Param3);
        pSendChatMsgAddress pSendChatMsg = (pSendChatMsgAddress)(0x004788B0);

        _asm mov Param3, eax//Random bugfix 0x0058D668
        pSendChatMsg(text, 0x0058D668, Param3);
    }
    DEBUGEND()
}

static char version_string[150] = "";

bool Initialize(CStruct* pStruct, CScript* pScript)
{
    if (init)//00425e10
    {

        XINPUT::vibrating = false;
        XINPUT::vibrationFrames;

        Slerp::transfer = false;
        Slerp::landing = false;
        Slerp::m_began_frame_in_transfer = false;
        Slerp::slerping = false;
        D3DXMatrixIdentity(&Slerp::start);
        D3DXMatrixIdentity(&Slerp::end);
        D3DXMatrixIdentity(&Slerp::old);
        Slerp::duration = 0.0f;
        Slerp::radians = 0.0f;
        Slerp::timer = 0.0f;
        Slerp::axis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        Slerp::vel = Vertex(0.0f, 0.0f, 0.0f);
        Slerp::facing = Vertex(0.0f, 0.0f, 0.0f);
        Slerp::height = 0.0f;
        Slerp::goal = Vertex(0.0f, 0.0f, 0.0f);
        //
        Slerp::last = Vertex(0.0f, 0.0f, 0.0f);
        Slerp::lerp = 0.0f;
        Slerp::inAcid = false;
        Slerp::vert = false;
        Slerp::addedvel = false;
        Slerp::landed = true;
        Slerp::OnGround = false;
        Slerp::OnGrind = false;
        Slerp::done = false;
        Slerp::trying = false;
        Slerp::value = 0.0f;
        Slerp::wallplant = false;
        Slerp::m_last_wallplant_time_stamp.QuadPart = 0;
        Slerp::realVelocity = Vertex(0.0f, 0.0f, 0.0f);
        Slerp::targetNormal = Vertex(0.0f, 0.0f, 0.0f);
        Slerp::target_normal = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

        Slerp::speed = 0.0f;

        LevelModSettings::bUseNewMenu = true;
        LevelModSettings::bAA = false;
        LevelModSettings::AllowNewTricks = LevelModSettings::ALLOW_NONE;
        LevelModSettings::bUnlimitedGraf = false;
        LevelModSettings::bFixSound = true;
        LevelModSettings::bTeleFix = true;
        LevelModSettings::bGrafCounter = true;
        LevelModSettings::MemorySize = 0xFA000;
        LevelModSettings::bHookedControls = false;


        LevelModSettings::SpineButton = 7;
        LevelModSettings::SpineButton2 = -1;

        /*LevelModSettings::NewMenu = { 0x90, 0x90, 0x90, 0x90, 0x74, 0x70, 0xEB, 0x7B, 0x90 };//000404ce
        LevelModSettings::OldMenu = { 0xC6, 0x46, 0x04, 0x01, 0x74, 0x70, 0x57, 0x6A, 0x00 };*/
        /*FILE* fp = fopen("LevelMod.ini", "ab+");
        fseek(fp, 0, SEEK_END);
        DWORD size = ftell(fp);


        if (size == 0)//file was empty
        {*/

        /*}
        else
            fclose(fp);*/
        DWORD old;
        VirtualProtect((void*)0x00425E32, 0x10, PAGE_EXECUTE_READWRITE, &old);
        VirtualProtect((void*)0x005B7501, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
        sprintf(version_string, "LevelMod Build %.2f", VERSION);
        *(DWORD*)0x005B7501 = 0x2073;
        *(BYTE*)0x00425E32 = 0x68;
        *(DWORD*)0x00425E33 = (DWORD)&version_string;
        *(BYTE*)0x00425E37 = 0x90;

        *(BYTE*)0x00425E3C = 0x90;
        *(BYTE*)0x00425E3D = 0x90;
        *(BYTE*)0x00425E3E = 0x90;

        //MessageBox(0, "INIT", "", 0);
        init = false;
        _printf("Going to init\n");
        //MessageBox(0, "Going to Init", "", 0);

        InitLevelMod();
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)FixMessage, 0, 0, 0);
        //CreateThread(0, 0, (LPTHREAD_START_ROUTINE)GetLevelModVersion, 0, 0, 0);


        while (!init2)
            Sleep(100);
        if (!debug)
        {
            DWORD old;
            bHooked = true;
            static BYTE callHooked[] = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0xC3 };

            VirtualProtect((void*)0x00401960, 6, PAGE_EXECUTE_READWRITE, &old);
            memcpy(oldCustomPrint, (void*)0x00401960, 6);

            *(DWORD*)&callHooked[1] = ((DWORD)add_log - 0x00401960 - 5);

            memcpy((void*)0x00401960, callHooked, 6);


            static const DWORD addr = (DWORD)GetProcAddress(GetModuleHandle("msvcrt.dll"), "printf");
            if (addr)
            {
                VirtualProtect((void*)addr, 6, PAGE_EXECUTE_READWRITE, &old);
                memcpy(oldCustomPrint, (void*)addr, 6);
                DWORD hookedAddrs = ((DWORD)add_log - addr - 5);
                *(DWORD*)&callHooked[1] = hookedAddrs;
                memcpy((void*)addr, callHooked, 6);
            }
            //logFile = fopen("loggers.txt", "w+t");*/
        }

        printf("Init DONE\n");
        return true;

    }
    else if (!bAddedOptions)
    {

    //Gfx::command = Gfx::Command::ToggleWindowed;
        //MessageBox(0, "GOING TO ADD HOSTOPTIONS", "", 0);
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckForScriptUpdates, NULL, 0/*CREATE_SUSPENDED*/, NULL);
        using namespace LevelModSettings;
        _printf("Already inited\n");

        bAddedOptions = true;

        QBKeyHeader* header = GetQBKeyHeader(Checksum("PlaySound"));
        if (header)
            Game::PlaySound = header->pFunction;

        UpdateScriptConstants();


        header = GetQBKeyHeader(crc32f("LM_HostOptions"));

        LevelModSettings::SpineButton3 = KeyMap::GetVKeyCode(KeyMap::MappedKey::Unused);

        /*if (!bDebugMode)
        {
            
            char* dir = QScript::GetScriptDir();
            unsigned int i = 13;
            const char* settings = "LevelMod\\Settings.qb";
            while (*settings != 0x00)
            {
                dir[i] = *settings;
                dir[i + 1] = 0;
                i++;
                settings++;
            }

            dir[i] = 0;
            if (!QScript::Scripts)
                QScript::Scripts = new QScript::QBScript(false);
            QScript::Scripts->OpenScript(dir);
        }*/

        if (header)
        {
            for (DWORD i = 1; i < header->pArray->GetNumItems(); i++)
            {
                CStructHeader* HostOption = header->pArray->GetCStruct(i, __FUNCTION__);

                if (HostOption)
                {
                    //Set option max to 1(bool)
                    LevelModSettings::pMax = NULL;

                    CStructHeader* Name;
                    if (HostOption->GetStruct(Checksums::Name, &Name))
                    {
                        char OptionName[256] = "";
                        char* pName = FindChecksumName(Name->Data, false);
                        memcpy(OptionName, pName, strlen(pName) + 1);

                        CStructHeader* Value;
                        if (HostOption->GetStruct(Checksums::Value, &Value))
                        {
                            OverrideOption::Type type;
                            CStructHeader* override;
                            if (HostOption->GetStruct(crc32f("OVERRIDE_TRUE"), &override))
                            {
                                type = OverrideOption::Type::OVERRIDE_TRUE;
                                _printf("OVERRIDE TRUE ");
                            }
                            else if (HostOption->GetStruct(crc32f("OVERRIDE_FALSE"), &override))
                            {
                                type = OverrideOption::Type::OVERRIDE_FALSE;
                                _printf("OVERRIDE FALSE ");

                            }
                            else if (HostOption->GetStruct(crc32f("OVERRIDE"), &override))
                            {
                                type = OverrideOption::Type::OVERRIDE;
                                _printf("OVERRIDE ");
                            }
                            else
                            {
                                _printf("Need param #OVERRIDE/#OVERRIDE_FALSE/#OVERRIDE_TRUE in HostOption %s\n", OptionName);
                                return true;
                            }
                            _printf("%s\n", FindChecksumName(override->Data));
                            AddOption(OptionName, Value->value.i, false, override->Data, (BYTE)type);

                        }
                        else
                        {
                            _printf("Need param #Value in HostOption %s\n", OptionName);
                        }
                    }
                }

            }

            _printf("Going to Store OverrideData\n");
            for (auto override = overrideOptions.begin(); override != overrideOptions.end(); ++override)
            {
                auto it = options.find(override->second.option);
                if (it != options.end())
                {
                    _printf("OK2\n");
                    it->second.pOverride = &override->second;
                }
                else
                    _printf("Option %s not found in HostOption %s\n", FindChecksumName(override->second.option), FindChecksumName(override->first));
            }
            _printf("Finished storing OverrideData\n");

            _printf("Truncating IniFile %s\n", IniPath);
            FILE* f = fopen(IniPath, "w");
            fclose(f);
            for (auto it = options.begin(); it != options.end(); ++it)
            {
                char* name = FindChecksumName(it->first, false);
                _printf("Adding Option to Ini %s\n", name);
                OptionWriter->WriteInt("Script_Settings", name, it->second.value);
            }
        }
        else
            _printf("Couldn't find HostOptions\n");
    }

    /*int id = -255;
    pStruct->GetScript("id", &id);
    printf("ID %d", id);
    char levltext[260] = "LeveL";
    if (id != -255)
    {
        MessageBox(0, "LEVELMOD", "", 0);
        printf("going to set text\n");
        SetElementText(id, levltext);
        printf("set text\n");
    }*/
    /*else if (init2)
    {
        init2 = false;

    }*/
    _printf("Already inited\n");
    //MessageBox(0, "Already Inited", "", 0);
    return true;
}

void SetStructValues(CStructHeader* pStruct, CStructHeader* values)
{
    pStruct->SetValues(values);
}

void SetArrayValues(CArray* pArray, CStructHeader* values)
{
    pArray->SetValues(values);
}

bool InvalidReadPtr(const void* const __restrict ptr, const DWORD size)
{
    MEMORY_BASIC_INFORMATION mbi;
    if ((VirtualQuery(ptr, &mbi, sizeof(mbi)) == 0) || (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)))
        return true;
    else if ((mbi.State & MEM_COMMIT) && (((DWORD)((BYTE*)ptr + size)) <= ((DWORD)((BYTE*)mbi.BaseAddress + mbi.RegionSize))))
        return ((mbi.Protect & PAGE_READABLE) == 0);
    else
        return true;
}


bool InvalidReadPtr(const void* const __restrict ptr)
{
    MEMORY_BASIC_INFORMATION mbi;
    if ((VirtualQuery(ptr, &mbi, sizeof(mbi)) == 0) || (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)))
        return true;
    else if (mbi.State & MEM_COMMIT)
        return ((mbi.Protect & PAGE_READABLE) == 0);
    else
        return true;
}


DWORD GetTagCount()
{
    DWORD* pTags = *(DWORD**)0x0058C300;
    if (pTags == 0 || *pTags == 0)
        return 0;
    return *(DWORD*)((BYTE*)pTags + 0x80);
}

extern QBKeyHeader triggers[];
EXTERN QBKeyHeader* GetQBKeyHeader(unsigned long QBKey)
{
    int ShortedQBKey = QBKey & 0x0000FFFF;//last 16 bit of QBKey, old code - last 12 bits of QBKey
    int* LoadAddress = (int*)(ShortedQBKey + ShortedQBKey * 4);//get index
    QBKeyHeader* header = (QBKeyHeader*)((int)LoadAddress * 4 + (DWORD)&triggers);//0x0087D8F8 = start of main qbTable

    while (header->QBKeyCheck)//loop until end of table or corect header is found
    {
        if (((header->QBKeyCheck << 0x0C) | ShortedQBKey) == QBKey)
            return header;//found header lets return it
        header++;
    }

    typedef QBKeyHeader* (__cdecl* const GetQBKeyHeaderFunc)(const unsigned long QBKey);
    return ((GetQBKeyHeaderFunc)(0x00426340))(QBKey);//didn't find header lets let game search through sub qbTables
}

__restrict LPDIRECT3DDEVICE9 Gfx::pDevice = NULL;
float Physics::Rail_Max_Snap = 40.0f;
float Physics::Rail_Corner_Leave_Angle = 50.0f;
float Physics::Physics_Point_Rail_Kick_Upward_Angle = 35.0f;
float Physics::Point_Rail_Speed_Boost = 125.0f;

void DrawLines()
{
    //pDevice->SetTransfD3DTRANSOFMRorm(0, &lineWorld);
    Gfx::pDevice->SetFVF(line_fvf);
    Gfx::pDevice->DrawPrimitiveUP(D3DPT_LINELIST,         //PrimitiveType
        numLineVertices / 2,              //PrimitiveCount
        lineVertices,            //pVertexStreamZeroData
        sizeof(line_vertex));   //VertexStreamZeroStride
}

void DrawFrame2()
{
    /*if (line && numLineVertices)
    {
        line->Begin();
        line->DrawTransform(lineVertices, numLineVertices, &lineWorld, lineColor);
        line->End();
    }*/
}

void OnRelease()
{
    if (!init)
    {
        _printf("OnRelease\n");
        if (m_font)
        {
            m_font->Release();
            m_font = NULL;
        }
        if (QScript::Scripts)
            delete QScript::Scripts;
        QScript::Scripts = NULL;
        movingObjects.clear();
        if (pObserve)
            delete pObserve;
        pObserve = NULL;
        if (OptionWriter)
            delete OptionWriter;
        OptionWriter = NULL;
        if (OptionReader)
            delete OptionReader;
        OptionReader = NULL;

        /*for (DWORD i = 0; i < 6; i++)
        {
            wheel_texture[i]->Release();
        }
        eye_sprite->Release();*/
    }
}

void OnLost()
{
    _printf("OnLost\n");
    if (m_font)
    {
        m_font->OnLostDevice();
        //eye_sprite->OnLostDevice();
    }
}

void OnReset()
{
    _printf("OnReset\n");
    if (m_font)
    {
        m_font->OnResetDevice();
        //eye_sprite->OnResetDevice();
    }
}

static char* mmap_file(size_t* len, const char* filename) {
#ifdef _WIN32
    HANDLE file =
        CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (file == INVALID_HANDLE_VALUE) { /* E.g. Model may not have materials. */
        return NULL;
    }

    HANDLE fileMapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);

    LPVOID fileMapView = MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0);
    char* fileMapViewChar = (char*)fileMapView;

    DWORD file_size = GetFileSize(file, NULL);
    (*len) = (size_t)file_size;

    return fileMapViewChar;
#else

    FILE* f;
    long file_size;
    struct stat sb;
    char* p;
    int fd;

    (*len) = 0;

    f = fopen(filename, "r");
    if (!f)
    {
        perror("open");
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    file_size = ftell(f);
    fclose(f);

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return NULL;
    }

    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        return NULL;
    }

    if (!S_ISREG(sb.st_mode)) {
        fprintf(stderr, "%s is not a file\n", "lineitem.tbl");
        return NULL;
    }

    p = (char*)mmap(0, (size_t)file_size, PROT_READ, MAP_SHARED, fd, 0);

    if (p == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }

    if (close(fd) == -1) {
        perror("close");
        return NULL;
    }

    (*len) = (size_t)file_size;

    return p;

#endif
}

/* path will be modified */
static char* get_dirname(char* path)
{
    char* last_delim = NULL;

    if (path == NULL) {
        return path;
    }

#if defined(_WIN32)
    /* TODO: Unix style path */
    last_delim = strrchr(path, '\\');
#else
    last_delim = strrchr(path, '/');
#endif

    if (last_delim == NULL) {
        /* no delimiter in the string. */
        return path;
    }

    /* remove '/' */
    last_delim[0] = '\0';

    return path;
}

static void get_file_data(const char* filename, const int is_mtl, const char* obj_filename, char** data, size_t* len) {
    if (!filename) {
        fprintf(stderr, "null filename\n");
        (*data) = NULL;
        (*len) = NULL;
    }

    const char* ext = strrchr(filename, '.');

    size_t data_len = 0;

    if (strcmp(ext, ".gz") == 0) {
        MessageBox(0, 0, 0, 0);

    }
    else {

        char* basedirname = NULL;

        char tmp[MAX_PATH] = "";
        DWORD len = strlen(filename);
        strncpy(tmp, filename, len);

        /* For .mtl, extract base directory path from .obj filename and append .mtl filename */
        if (is_mtl && obj_filename) {
            tmp[len - 3] = 'm';
            tmp[len - 2] = 't';
            tmp[len - 1] = 'l';
        }

        *data = mmap_file(&data_len, tmp);
    }

    (*len) = data_len;
}

static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) {
    float v10[3];
    float v20[3];
    float len2;

    v10[0] = v1[0] - v0[0];
    v10[1] = v1[1] - v0[1];
    v10[2] = v1[2] - v0[2];

    v20[0] = v2[0] - v0[0];
    v20[1] = v2[1] - v0[1];
    v20[2] = v2[2] - v0[2];

    N[0] = v20[1] * v10[2] - v20[2] * v10[1];
    N[1] = v20[2] * v10[0] - v20[0] * v10[2];
    N[2] = v20[0] * v10[1] - v20[1] * v10[0];

    len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
    if (len2 > 0.0f) {
        float len = (float)sqrt((double)len2);

        N[0] /= len;
        N[1] /= len;
    }
}

struct TexCoord
{
    float u, v;

    TexCoord(float u, float v)
    {
        this->u = u;
        this->v = v;
    }
};

struct Direct3DVertex
{
    float x, y, z;
    //DWORD color;
    float u, v;

    Direct3DVertex(D3DXVECTOR3& vec, TexCoord& uv)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        //color = 0xFFFFFFFF;
        u = uv.u;
        v = uv.v;
    }
};

bool ReadLine(BYTE* pFile, BYTE* eof, char** line)
{
    if (pFile >= eof)
        return false;
    while (*pFile == '\n' || *pFile == '\r')
        pFile++;

    if (pFile >= eof)
        return false;
    *line = (char*)pFile;
    return true;
}

struct SimpleMesh
{
    DWORD numVertices;
    DWORD numTriangles;
    std::vector<Direct3DVertex> vertices;
    size_t face_offset = 0;

    SimpleMesh(char* filename)
    {
        tinyobj_attrib_t attrib;
        tinyobj_shape_t* shapes = NULL;
        size_t num_shapes;
        tinyobj_material_t* materials = NULL;
        size_t num_materials;

        FILE* f = fopen(filename, "rb+");
        fseek(f, 0, SEEK_END);
        DWORD size = ftell(f);

        BYTE* pFile = new BYTE[size];
        BYTE* eof = pFile + size;
        fseek(f, 0, SEEK_SET);
        fread(pFile, size, 1, f);
        fclose(f);
        BYTE* pBuffer = pFile;

        std::vector<D3DXVECTOR3> positions;
        std::vector<TexCoord> texcoords;

        char* line;
        int success = 0;

        while (ReadLine(pFile, eof, &line))
        {
            switch (*line)
            {
            case 'f':
                line += 2;
                int vert_idx[3];
                int tex_idx[3];
                int normal_idx[3];

                success = sscanf(line, "%d/%d/%d %d/%d/%d %d/%d/%d", &vert_idx[0], &tex_idx[0], &normal_idx[0], &vert_idx[1], &tex_idx[1], &normal_idx[1], &vert_idx[2], &tex_idx[2], &normal_idx[2]);
                if (success != 9)
                    MessageBox(0, "Not Good", 0, 0);
                vertices.push_back(Direct3DVertex(positions[vert_idx[0] - 1], texcoords[tex_idx[0] - 1]));
                vertices.push_back(Direct3DVertex(positions[vert_idx[1] - 1], texcoords[tex_idx[1] - 1]));
                vertices.push_back(Direct3DVertex(positions[vert_idx[2] - 1], texcoords[tex_idx[2] - 1]));
                while (*line != '\n' && *line != '\r')
                    line++;
                pFile = (BYTE*)line;
                break;
            case 'v':
                line++;
                if (*line == 't')
                {
                    line += 2;
                    float u, v;
                    success = sscanf(line, "%f %f", &u, &v);
                    if (success != 2)
                        MessageBox(0, "Not Good", 0, 0);
                    texcoords.push_back(TexCoord(u, v));

                }
                else if (*line == ' ')
                {
                    line++;
                    float x, y, z;
                    success = sscanf(line, "%f %f %f", &x, &y, &z);
                    if (success != 3)
                        MessageBox(0, "Not Good", 0, 0);
                    positions.push_back(D3DXVECTOR3(x, y, z));
                }
                while (*line != '\n' && *line != '\r')
                    line++;
                pFile = (BYTE*)line;
                break;
            default:
                while (*line != '\n' && *line != '\r')
                    line++;
                pFile = (BYTE*)line;
                break;

            }
        }

        numVertices = vertices.size();
        numTriangles = numVertices / 3;

        char temp[256] = "";

        f = fopen("Temp.txt", "w+b");
        for (DWORD i = 0; i < vertices.size(); i++)
        {
            fprintf(f, "%f %f %f\n", vertices[i].x, vertices[i].y, vertices[i].z);
        }
        fclose(f);
        texcoords.clear();
        positions.clear();
        sprintf(temp, "numVerts %d numTri %d\n", numVertices, numTriangles);
        MessageBox(0, temp, temp, 0);

        /*float* vb;
        size_t face_offset = 0;
        size_t i;

        size_t num_triangles = attrib.num_face_num_verts;
        size_t stride = 9;

        vb = (float*)vertices;

        for (i = 0; i < attrib.num_face_num_verts; i++) {
            size_t f;
            assert(attrib.face_num_verts[i] % 3 ==
                0);
            for (f = 0; f < (size_t)attrib.face_num_verts[i] / 3; f++) {
                size_t k;
                float v[3][3];
                float n[3][3];
                float t[3][2];
                float c[3];
                float len2;

                tinyobj_vertex_index_t idx0 = attrib.faces[face_offset + 3 * f + 0];
                tinyobj_vertex_index_t idx1 = attrib.faces[face_offset + 3 * f + 1];
                tinyobj_vertex_index_t idx2 = attrib.faces[face_offset + 3 * f + 2];

                for (k = 0; k < 3; k++) {
                    int f0 = idx0.v_idx;
                    int f1 = idx1.v_idx;
                    int f2 = idx2.v_idx;
                    assert(f0 >= 0);
                    assert(f1 >= 0);
                    assert(f2 >= 0);

                    v[0][k] = attrib.vertices[3 * (size_t)f0 + k];
                    v[1][k] = attrib.vertices[3 * (size_t)f1 + k];
                    v[2][k] = attrib.vertices[3 * (size_t)f2 + k];
                }

                if (attrib.num_texcoords > 0) {
                    int f0 = idx0.vt_idx;
                    int f1 = idx1.vt_idx;
                    int f2 = idx2.vt_idx;
                    if (f0 >= 0 && f1 >= 0 && f2 >= 0) {
                        assert(f0 < (int)attrib.num_texcoords);
                        assert(f1 < (int)attrib.num_texcoords);
                        assert(f2 < (int)attrib.num_texcoords);
                        for (k = 0; k < 2; k++) {
                            t[0][k] = attrib.texcoords[2 * (size_t)f0 + k];
                            t[1][k] = attrib.texcoords[2 * (size_t)f1 + k];
                            t[2][k] = attrib.texcoords[2 * (size_t)f2 + k];
                        }
                    }
                }

                if (attrib.num_normals > 0) {
                    int f0 = idx0.vn_idx;
                    int f1 = idx1.vn_idx;
                    int f2 = idx2.vn_idx;
                    if (f0 >= 0 && f1 >= 0 && f2 >= 0) {
                        assert(f0 < (int)attrib.num_normals);
                        assert(f1 < (int)attrib.num_normals);
                        assert(f2 < (int)attrib.num_normals);
                        for (k = 0; k < 3; k++) {
                            n[0][k] = attrib.normals[3 * (size_t)f0 + k];
                            n[1][k] = attrib.normals[3 * (size_t)f1 + k];
                            n[2][k] = attrib.normals[3 * (size_t)f2 + k];
                        }
                    }
                    else {
                        CalcNormal(n[0], v[0], v[1], v[2]);
                        n[1][0] = n[0][0];
                        n[1][1] = n[0][1];
                        n[1][2] = n[0][2];
                        n[2][0] = n[0][0];
                        n[2][1] = n[0][1];
                        n[2][2] = n[0][2];
                    }
                }
                else {

                    CalcNormal(n[0], v[0], v[1], v[2]);
                    n[1][0] = n[0][0];
                    n[1][1] = n[0][1];
                    n[1][2] = n[0][2];
                    n[2][0] = n[0][0];
                    n[2][1] = n[0][1];
                    n[2][2] = n[0][2];
                }

                for (k = 0; k < 3; k++) {
                    vb[(3 * i + k) * stride + 0] = v[k][0];
                    vb[(3 * i + k) * stride + 1] = v[k][1];
                    vb[(3 * i + k) * stride + 2] = v[k][2];
                    vb[(3 * i + k) * stride + 6] = n[k][0];
                    vb[(3 * i + k) * stride + 7] = n[k][1];
                    vb[(3 * i + k) * stride + 8] = n[k][2];

                    vb[(3 * i + k) * stride + 4] = t[k][0];
                    vb[(3 * i + k) * stride + 5] = t[k][1];

                    c[0] = n[k][0];
                    c[1] = n[k][1];
                    c[2] = n[k][2];
                    len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
                    if (len2 > 0.0f) {
                        float len = (float)sqrt((double)len2);

                        c[0] /= len;
                        c[1] /= len;
                        c[2] /= len;
                    }

                    BYTE rgb[4];
                    rgb[0] = 0;// (c[0] * 255.0f);
                    rgb[1] = 0;// (c[1] * 255.0f);
                    rgb[2] = 0;// (c[2] * 255.0f);
                    rgb[3] = 255;

                    *(DWORD*)&vb[(3 * i + k) * stride + 3] = *(DWORD*)rgb;

                }
            }

            face_offset += (size_t)attrib.face_num_verts[i];
        }
        numTriangles = numVertices / 3;*/
        delete[] pBuffer;
    }

};

SimpleMesh* Eye = NULL;


LPDIRECT3DTEXTURE9 target_texture, eye_texture;
LPDIRECT3DSURFACE9 eye_target;

std::vector<ShadowMap> shadows;

void DrawEye()
{
    static const D3DXVECTOR3 origin(0, 0, 0);
    LPDIRECT3DSURFACE9 old_target;
    D3DXMATRIX old_view, old_world;

    Gfx::pDevice->GetRenderTarget(0, &old_target);

    LPDIRECT3DSURFACE9 old_surface;
    D3DVIEWPORT9 old_viewport;
    Gfx::pDevice->GetDepthStencilSurface(&old_surface);
    Gfx::pDevice->GetViewport(&old_viewport);
    Gfx::pDevice->SetDepthStencilSurface(nullptr);

    Gfx::pDevice->BeginScene();

    Gfx::pDevice->SetViewport(&Gfx::world_viewport);

    Gfx::pDevice->SetRenderTarget(0, Gfx::world_rendertarget);

    eye_sprite->Begin(D3DXSPRITE_ALPHABLEND);

    D3DXMATRIX M;
    D3DXMatrixTranslation(&M, 10.0f, 500.0f, 0.0f);
    eye_sprite->SetTransform(&M);

    if (eye_state < 6)
    {
        last_state = eye_state;
        eye_sprite->Draw(wheel_texture[eye_state], NULL, NULL, &origin, 0xFFFFFFFF);
    }

    switch (eye_state)
    {
    case 0:
        wheel_timer++;
        if (wheel_timer > 45)
        {
            wheel_timer = 0;
            eye_state = (std::rand() % 5) + 1;
            if (eye_state > 6)
                eye_state = 0;
        }
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    //case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        wheel_timer++;
        actual_timer++;
        if (actual_timer > 90)
        {
            wheel_timer = 0;
            actual_timer = 0;
            eye_state = 6;
        }
        if (wheel_timer > 30)
        {
            wheel_timer = 0;
            eye_state = 0;
        }
        break;


    case 6:
        actual_timer++;
        wheel_timer++;
        if (!rotating)
        {
            actual_timer = 0;
            rotating = true;
            last_state++;
        }
        else if(wheel_timer > 7)
        {
            wheel_timer = 0;
            last_state++;
        }
        if (last_state > 5)
            last_state = 0;
        eye_sprite->Draw(wheel_texture[last_state], NULL, NULL, &origin, 0xFFFFFFFF);
        if (actual_timer > 700)
        {
            eye_state = 0;
            wheel_timer = 0;
            actual_timer = 0;
            rotating = false;
        }
        break;

    }
    eye_sprite->End();

    Gfx::pDevice->EndScene();

    Gfx::pDevice->SetDepthStencilSurface(old_surface);
    Gfx::pDevice->SetViewport(&old_viewport);
    Gfx::pDevice->SetRenderTarget(0, old_target);
}

bool bToggleWindowed = false;

SHORT __stdcall proxy_GetAsyncKeyState(int key)
{
    if (KeyState::GetKeyboardState(VirtualKeyCode::ALT) && KeyState::GetKeyboardState(VirtualKeyCode::ENTER) && !KeyState::GetOldKeyboardState(VirtualKeyCode::ENTER))//alt + enter
    {
        //Unpress the enter KeyboardState
        KeyState::Unpress(VirtualKeyCode::ENTER);
        bToggleWindowed = true;
    }
    else
        bToggleWindowed = false;

    return GetAsyncKeyState(key);
}

typedef BOOL(__stdcall* const pGetMessage)(LPMSG lpMsg,
    HWND  hWnd,
    UINT  wMsgFilterMin,
    UINT  wMsgFilterMax);
BOOL __stdcall proxy_GetMessage(LPMSG lpMsg,
    HWND  hWnd,
    UINT  wMsgFilterMin,
    UINT  wMsgFilterMax)
{
    BOOL result = pGetMessage(*(DWORD*)0x0058D228)(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
    if (result)
    {
        switch (lpMsg->message)
        {
        case WM_WINDOWPOSCHANGING:
        case WM_WINDOWPOSCHANGED:
        case WM_MOVING:
        case WM_MOVE:
                MessageBox(0, 0, 0, 0);
                OptionWriter->WriteInt("Other_Settings", "LM_WindowedX", lpMsg->lParam);
                OptionWriter->WriteInt("Other_Settings", "LM_WindowedY", lpMsg->wParam);
                break;
        }
    }
    return result;
}

static DWORD lastTime = 0;

bool IsOptionOn(const char* option)
{
    DWORD value = 0;
    DWORD new_value = OptionReader->ReadInt("Script_Settings", option, value);
    if (new_value < 2)
        value = new_value;
    return value != 0;
}

void ToggleWindowed()
{
    D3DPRESENT_PARAMETERS8* d3dpp = (D3DPRESENT_PARAMETERS8*)0x00973be0;
    //Inverse windowed param to DirectX
    d3dpp->Windowed = !d3dpp->Windowed;
    //Toggle windowed mode flag
    d3dpp->Windowed ? *(BYTE*)0x00851094 &= ~1 : *(BYTE*)0x00851094 |= 1;

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = d3dpp->BackBufferWidth;
    rect.bottom = d3dpp->BackBufferHeight;

    if (d3dpp->Windowed)
    {

        ShowWindow(Gfx::hFocusWindow, SW_NORMAL);
        SetFocus(Gfx::hFocusWindow);

        SetWindowLongPtr(Gfx::hFocusWindow, GWL_STYLE, WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE);
        AdjustWindowRect(&rect, WS_CAPTION | WS_POPUPWINDOW, FALSE);
        SetWindowPos(Gfx::hFocusWindow, HWND_NOTOPMOST, 0, 0, d3dpp->BackBufferWidth, d3dpp->BackBufferHeight, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
        //MoveWindow(Gfx::hFocusWindow, 0, 0, rect.right - rect.left, rect.bottom - rect.top, TRUE);

    }
    else
    {
        POINT Point = { 0 };
        HMONITOR Monitor = MonitorFromPoint(Point, MONITOR_DEFAULTTONEAREST);
        MONITORINFO MonitorInfo = { sizeof(MonitorInfo) };
        if (GetMonitorInfo(Monitor, &MonitorInfo)) {

            DWORD Style = WS_POPUP | WS_VISIBLE;
            SetWindowLongPtr(Gfx::hFocusWindow, GWL_STYLE, Style);
            SetWindowPos(Gfx::hFocusWindow, HWND_TOPMOST, MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
                MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
                SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE);

            ShowWindow(Gfx::hFocusWindow, SW_NORMAL);
            SetFocus(Gfx::hFocusWindow);
        }
    }
}

bool ToggleWindowedScript(CStruct* pStruct, CScript* pScript)
{
    Gfx::command = Gfx::Command::ToggleWindowed;
    return true;
}

bool FixGoBackScript(CStruct* pStruct, CScript* pScript)
{
    /**(BYTE*)0x004CEDE8 = 0xE9;
    *(DWORD*)0x004CEDE9 = 0x0000008E;
    *(BYTE*)0x004CEDED = 0x90;
    *(WORD*)0x004CEDEE = 0x9090;*/
    HookFunction(0x004CEDE4, proxy_FixGoBack);
    return true;
}

bool RestoreGoBackScript(CStruct* pStruct, CScript* pScript)
{
    /*BYTE restored[] = { 0x84, 0xC0, 0x0F, 0x85, 0x8B, 0x00, 0x00, 0x00 };
    memcpy((LPVOID)0x004CEDE8, restored, sizeof(restored));*/
    *(DWORD*)0x004CEDE4 = 0x00000598;
    return true;
}

void MaybeFixStutter()
{
    if (Gfx::fps_fix)
    {
        BYTE timer[] = { 0xE8, 0x98, 0xF4, 0xF7, 0x79, 0xB9, 0x0E, 0x00, 0x00, 0x00, 0x39, 0xC8, 0x73, 0x27, 0x29, 0xC1, 0x51, 0xE8, 0xD7, 0x29, 0x8C, 0x75, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x85, 0xC0, 0x75, 0x16, 0xE8, 0x00, 0x00, 0x00, 0x00, 0xEB, 0x0F };

        switch (Gfx::fps_fix)
        {
        case 1:
            *(DWORD*)&timer[1] = (PtrToUlong(TimerElapsed) - 0x004C04E4) - 4;
            HookFunction(0x004C0519, TimerStart);
            break;
        case 2:
            *(DWORD*)&timer[1] = (PtrToUlong(TimerElapsed_Hybrid) - 0x004C04E4) - 4;
            HookFunction(0x004C0519, TimerStart_Hybrid);
            break;
        case 3:
            *(DWORD*)&timer[1] = (PtrToUlong(TimerElapsed_Sleep) - 0x004C04E4) - 4;
            HookFunction(0x004C0519, TimerStart_Sleep);
            break;
        }
        /*if (Gfx::fps_fix == 3)
            HookFunction(0x004C0519, TimerStart_Sleep);
        else
            HookFunction(0x004C0519, TimerStart);*/
        *(bool**)&timer[23] = &show_loading_screen;
        *(BYTE*)&timer[31] = 0x90;
        *(DWORD*)&timer[32] = 0x90909090;// (PtrToUlong(DrawEye) - (0x004C04E4 + 31)) - 4;
        if (Gfx::fps_fix != 3)
            *(WORD*)&timer[5] = 0x2EEB;

        memcpy((void*)0x004C04E3, timer, sizeof(timer));


        static const DWORD addr = (DWORD)GetProcAddress(GetModuleHandle("KERNELBASE.dll"), "Sleep");
        HookFunction(0x004C04F5, (void*)addr);
    }
    else//Set back original
        memcpy((void*)0x004C04E3, LevelModSettings::original_timer, sizeof(LevelModSettings::original_timer));
}

bool LaunchGFXCommand(CStruct* pStruct, CScript* pScript)
{
    for (auto header = pStruct->head; header != NULL; header = header->NextHeader)
    {
        if (header->Type == QBKeyHeader::LOCAL)
        {
            switch (header->Data)
            {
            case Checksums::Reset:
                Gfx::command = Gfx::Command::Reset;
                    break;
            case Checksums::ToggleWindowed:
                if(IsOptionOn("LM_GFX_bWindowed") == (*(BYTE*)0x00851094 & 1))
                    Gfx::command = Gfx::Command::ToggleWindowed;
                break;
            case Checksums::FixStutter:
                Gfx::command = Gfx::Command::FixStutter;
                break;
            }

            return true;
        }
    }
    return false;
}

bool GetMaximumIndexScript(CStruct* pStruct, CScript* pScript)
{
    const CArray* pArray;
    pStruct->GetArray("array", &pArray);

    CStructHeader* param = pScript->params->AddParam("Max", QBKeyHeader::QBKeyType::INT);
    param->Data = (pArray->GetNumItems() - 1);
    return true;
}

__declspec(noalias) HRESULT PostRender(HRESULT hres)
{

    if (hres == D3D_OK && GameState::IsActive())
    {
        if (bToggleWindowed)
        {

            //Make sure we toggle only 1 per 2 sec
            DWORD time = GetCurrentTime();
            if (time < lastTime + 2000)
                return false;
            lastTime = time;

            //Unpress enter key, should already be unpressed but better safe than sorry...
            KeyState::Unpress(KeyCode::ENTER);

            //Set focus to desktop
            SetFocus(HWND_DESKTOP);
            //minimize the window to reset engine
            ShowWindow(Gfx::hFocusWindow, SW_MINIMIZE);

            ToggleWindowed();
            return D3DERR_DEVICELOST;

        }
        else if (Gfx::command != Gfx::Command::None)
        {
            //Make sure command is only executed once
            Gfx::Command command = Gfx::command;
            Gfx::command = Gfx::Command::None;

            D3DPRESENT_PARAMETERS8* d3dpp = (D3DPRESENT_PARAMETERS8*)0x00973be0;

            //Set focus to desktop
            SetFocus(HWND_DESKTOP);
            //minimize the window to reset engine
            ShowWindow(Gfx::hFocusWindow, SW_MINIMIZE);
            LevelModSettings::Option* option = NULL;

            switch (command)
            {
            case Gfx::Command::ToggleFiltering:
                Gfx::filtering = !Gfx::filtering;
                //Maybe need to reset some flags?
                break;

            case Gfx::Command::ChangeResolution:
                d3dpp->BackBufferWidth = Gfx::width;
                d3dpp->BackBufferHeight = Gfx::height;
                break;

            case Gfx::Command::ToggleWindowed:
                ToggleWindowed();
                return D3DERR_DEVICELOST;

            case Gfx::Command::Reset:
                option = GetOption(crc32f("LM_GFX_bVSync"));
                if (option)
                    Gfx::bVSync = option->value;

                option = GetOption(crc32f("LM_GFX_eBuffering"));
                if(option)
                    Gfx::numBackBuffers = option->value;

                option = GetOption(crc32f("LM_GFX_eAntiAlias"));
                    Gfx::AntiAliasing = option->value;
                break;

            case Gfx::Command::FixStutter:
                option = GetOption(crc32f("LM_GFX_eFixStutter"));
                if (option)
                {
                    Gfx::fps_fix = option->value;
                    _printf("fps_fix %d\n", Gfx::fps_fix);
                }
                else
                    _printf("NO OPT\n");
                MaybeFixStutter();
                break;
            }


            ShowWindow(Gfx::hFocusWindow, SW_NORMAL);
            SetFocus(Gfx::hFocusWindow);
            return D3DERR_DEVICELOST;
        }

    }
    return hres;
}

__declspec(noalias) void DrawFrame()
{
    //MessageBox(0, 0, 0, 0);
    if (!m_font) [[unlikely]]
    {
        GameState::GotSuperSectors = false;
        XINPUT::Player1 = new CXBOXController();
        //MessageBox(0,"init font","",0);
        rct.left = 200;
        rct.right = 200 + 1000;
        rct.top = 10;
        rct.bottom = 10 + 80;



        D3DXMatrixIdentity(&lineWorld);
        /*D3DXCreateLine(pDevice, &line);
        line->SetWidth(100);
        line->SetPattern(0xffffffff);
        //pDevice->CreateLine
        lineColor = D3DCOLOR_RGBA(255, 255, 255, 255);*/

        D3DXCreateFontA(Gfx::pDevice, 45, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &m_font);


        //Eye = new SimpleMesh("C:\\Program Files (x86)\\Activision\\Thps3\\Data\\LevelMod\\Neversoft\\eyes3.obj\0");

        //D3DXCreateTextureFromFile(Gfx::pDevice, "blank_texture.png", &blank_texture);
        //Gfx::pDevice->CreateTexture(120, 120, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &target_texture, nullptr);
        /*HRESULT hres = D3DXCreateTextureFromFileExA(Gfx::pDevice, "C:\\Program Files (x86)\\Activision\\Thps3\\Data\\LevelMod\\Neversoft\\eyes2.png", D3DX_DEFAULT, D3DX_DEFAULT, D3DX_FROM_FILE, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &eye_texture);
        if (FAILED(hres))
            MessageBox(0, 0, "FAILED", 0);*/


        /*D3DXCreateSprite(Gfx::pDevice, &eye_sprite);
        char temp[MAX_PATH] = "";
        GetCurrentDirectory(MAX_PATH, temp);
        char imageName[MAX_PATH] = "";
        for (DWORD i = 0; i < 6; i++)
        {
            sprintf(imageName, "%s\\Data\\Images\\wheel_%02d.png", temp, i+1);
            HRESULT hres = D3DXCreateTextureFromFileExA(Gfx::pDevice, imageName, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_FROM_FILE, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &wheel_texture[i]);
            if (FAILED(hres))
            {
                MessageBox(0, "Failed to load texture", imageName, 0);
            }
        }*/

    }

        if (GameState::GotSuperSectors) [[likely]]
        {
            //Skater * skater = Skater::Instance();
            if (Game::skater) [[likely]]
            {
                Gfx::frameCounter++;
                Gfx::uv_anim_timer += Game::skater->GetFrameLength();
                if (LevelModSettings::bHookedControls && XINPUT::Player1->IsConnected())
                {
                    if (XINPUT::vibrationFrames)
                    {
                        XINPUT::Player1->Vibrate(XINPUT::vibration.wLeftMotorSpeed, XINPUT::vibration.wRightMotorSpeed);
                        XINPUT::vibrationFrames -= Game::skater->GetFrameLength();
                    }
                    else
                    {
                        XINPUT::Player1->Vibrate(0, 0);
                    }

                }

            //RenderShadows();
            //ProxyPad(skater);

            updatingObjects = true;
            for (DWORD i = 0; i < movingObjects.size(); i++) [[unlikely]]
            {
                //_printf("Killed?...");
                if (!(movingObjects[i].state & MeshState::kill)) [[likely]]
                {
                    //_printf("FALSE\n");
                    if (movingObjects[i].Update(Game::skater->GetFrameLength()))
                        movingObjects[i].sector->Update();//Send state to update vertexbuffer
                }
                else
                {
                    _printf("SuperSector->Killed(): TRUE\n");
                    movingObjects.erase(movingObjects.begin() + i);
                    i--;
                }
            }
            updatingObjects = false;
        }
        }
            /*else
            {*/
           /* if(show_loading_screen)
               DrawEye();*/

    //}
    if (pObserve && pObserve->observing) [[unlikely]]
    {
        Skater * skater = Skater::Instance();
        if (skater)
        {
            KeyState* ollie = skater->GetKeyState(KeyState::OLLIE);
            if (ollie->IsPressed() && ollie->GetPressedTime() != pObserve->timeNext)
                pObserve->Next(ollie->GetPressedTime());
            pObserve->Update();
        }
        else
        {
            pObserve->Leave();
            delete pObserve;
            pObserve = NULL;
        }
    }
        if (LevelModSettings::bGrafCounter && Modulating()) [[unlikely]]
        {
            //TestForAcid();
            DWORD tagCount = GetTagCount();
            if (tagCount != oldTagCount)
            {
                if (tagCount && tagCount < 1000)
                {
                    oldTagCount = tagCount;
                    sprintf(&tags[6], "%u %X", tagCount, *(DWORD*)(0x0040033C + ((tagCount - 1) * 4)));
                    CStruct params;
                    CStructHeader param(QBKeyHeader::STRING, Checksums::text, tags);
                    params.AddParam(&param);
                    ExecuteQBScript("LaunchGrafCounter", &params);
                }
                else
                {
                    oldTagCount = 0;
                    CStruct params;
                    ExecuteQBScript("LaunchGrafCounter", &params);
                }
            }
        }
            if (bDownloading || showmessage) [[unlikely]]
            {
                if (showmessage)
                  showmessage--;
                m_font->DrawTextA(NULL, download_message, -1, &rct, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 255));
            }
                if (false)
                {
                    Gfx::pDevice->BeginScene();

                    DWORD  D3DCMP = 0, pShared = 0, vShared = 0, Cull = 0, StencileEnable = 0, Clipping = 0, Clipplane = 0, COLOROP1 = 0, COLORARG1 = 0, COLORARG2 = 0, COLOROP2 = 0;
                    UINT oldStride = 0;

                    DWORD oldRenderState;
                    Gfx::pDevice->GetRenderState(D3DRS_ZENABLE, &oldRenderState);
                    Gfx::pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
                    Gfx::pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);
                    //pDevice->GetPixelShader(&pShared);
                    Gfx::pDevice->SetPixelShader(NULL);
                    Gfx::pDevice->SetVertexShader(NULL);
                    Gfx::pDevice->SetTexture(0, NULL);
                    Gfx::pDevice->SetRenderState(D3DRS_ZWRITEENABLE, 0);

                    /* IDirect3DVertexBuffer8* oldBuffer = NULL;
                     IDirect3DBaseTexture8* oldTexture1, * oldTexture2 = NULL;
                     pDevice->GetStreamSource(0, &oldBuffer, &oldStride);
                     pDevice->GetPixelShader(&pShared);
                     pDevice->GetVertexShader(&vShared);*/
                    Gfx::pDevice->GetRenderState(D3DRS_CULLMODE, &Cull);
                    Gfx::pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
                    Gfx::pDevice->SetRenderState(D3DRS_CLIPPING, 0);
                    //pDevice->SetStreamSource(0, 0, 0);

                    //m_pIDirect3DDevice8->SetStreamSource(0,0,0);

                    //m_pIDirect3DDevice8->SetViewport(&pViewport);
                    //m_pIDirect3DDevice8->SetTexture(0,NULL);
                    /*pDevice->SetPixelShader(NULL);


                    pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
                    //m_pIDirect3DDevice8->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
                    pDevice->GetRenderState(D3DRS_ZFUNC, &D3DCMP);
                    pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
                    //m_pIDirect3DDevice8->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
                    //m_pIDirect3DDevice8->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
                    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
                    pDevice->GetRenderState(D3DRS_STENCILENABLE, &StencileEnable);
                    pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
                    pDevice->GetRenderState(D3DRS_CLIPPING, &Clipping);
                    pDevice->SetRenderState(D3DRS_CLIPPING, FALSE);
                    pDevice->GetRenderState(D3DRS_CLIPPLANEENABLE, &Clipplane);
                    pDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);

                    pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
                    pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
                        D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

                    pDevice->GetTextureStageState(0, D3DTSS_COLOROP, &COLOROP1);
                    pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTSS_COLOROP);
                    pDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &COLORARG1);
                    pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
                    pDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &COLORARG2);
                    pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

                    pDevice->GetTextureStageState(1, D3DTSS_COLOROP, &COLOROP2);
                    pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
                    pDevice->GetTexture(0, &oldTexture1);
                    pDevice->GetTexture(1, &oldTexture2);
                    pDevice->SetTexture(0, NULL);
                    pDevice->SetTexture(1, NULL);*/


                    //pDevice->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);


                    //pDevice->DrawPrimitiveUP(D3DPT_LINELIST, lineList.size() / 2, &lineList.front(), sizeof(Line));
                    DrawLines();
                    Gfx::pDevice->EndScene();

                    Gfx::pDevice->SetRenderState(D3DRS_ZENABLE, oldRenderState);
                    //pDevice->SetPixelShader(pShared);
                    /*pDevice->SetTexture(0, oldTexture1);
                    pDevice->SetTexture(1, oldTexture2);
                    pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
                    //m_pIDirect3DDevice8->SetRenderState( D3DRS_ZENABLE, TRUE);
                    pDevice->SetRenderState(D3DRS_STENCILENABLE, StencileEnable);
                    pDevice->SetRenderState(D3DRS_CLIPPING, Clipping);
                    pDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, Clipplane);*/


                    Gfx::pDevice->SetRenderState(D3DRS_CULLMODE, Cull);
                    /*pDevice->SetPixelShader(pShared);
                    pDevice->SetVertexShader(vShared);
                    pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP);
                    pDevice->SetStreamSource(0, oldBuffer, oldStride);*/



                }
            /*else if (downloading)
            {
            m_font->DrawText(msg, -1, &rct, 0, D3DCOLOR_ARGB(255, 0, 0, 255));
            }*/
            /*else
            {
            DWORD tagCount = GetTagCount();
            if (tagCount && tagCount < 1000)
            {
            oldTagCount = tagCount;
            DWORD crc = *(DWORD*)(0x0040033C + (tagCount * 4));
            if (crc)
            {
            sprintf(&tags[6], "%X", crc);
            CStruct params;
            CStructHeader* param = params.AddParam("text", QBKeyHeader::STRING);
            param->pStr = tags;
            ExecuteQBScript("LaunchGrafCounter", &params);
            params.DellocateCStruct();
            }
            }
            else if(oldTagCount != 0)
            {
            oldTagCount = 0;
            CStruct params;
            ExecuteQBScript("LaunchGrafCounter", &params);
            }
            }*/


            /*DWORD  D3DCMP, pShared, vShared, Cull, StencileEnable, Clipping, Clipplane, COLOROP1, COLORARG1, COLORARG2, COLOROP2;
            UINT oldStride;
            IDirect3DVertexBuffer8* oldBuffer;
            IDirect3DBaseTexture8* oldTexture1, *oldTexture2;
            m_pIDirect3DDevice8->GetStreamSource(0, &oldBuffer, &oldStride);
            m_pIDirect3DDevice8->GetPixelShader(&pShared);
            m_pIDirect3DDevice8->GetVertexShader(&vShared);
            m_pIDirect3DDevice8->GetRenderState(D3DRS_CULLMODE, &Cull);

            //m_pIDirect3DDevice8->SetStreamSource(0,0,0);

            //m_pIDirect3DDevice8->SetViewport(&pViewport);
            //m_pIDirect3DDevice8->SetTexture(0,NULL);
            m_pIDirect3DDevice8->SetPixelShader(NULL);


            m_pIDirect3DDevice8->SetRenderState(D3DRS_ZENABLE, TRUE);
            //m_pIDirect3DDevice8->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
            m_pIDirect3DDevice8->GetRenderState(D3DRS_ZFUNC, &D3DCMP);
            m_pIDirect3DDevice8->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
            //m_pIDirect3DDevice8->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
            //m_pIDirect3DDevice8->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
            m_pIDirect3DDevice8->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
            m_pIDirect3DDevice8->GetRenderState(D3DRS_STENCILENABLE, &StencileEnable);
            m_pIDirect3DDevice8->SetRenderState(D3DRS_STENCILENABLE, FALSE);
            m_pIDirect3DDevice8->GetRenderState(D3DRS_CLIPPING, &Clipping);
            m_pIDirect3DDevice8->SetRenderState(D3DRS_CLIPPING, FALSE);
            m_pIDirect3DDevice8->GetRenderState(D3DRS_CLIPPLANEENABLE, &Clipplane);
            m_pIDirect3DDevice8->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);

            m_pIDirect3DDevice8->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
            m_pIDirect3DDevice8->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
            D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

            m_pIDirect3DDevice8->GetTextureStageState(0, D3DTSS_COLOROP, &COLOROP1);
            m_pIDirect3DDevice8->SetTextureStageState(0, D3DTSS_COLOROP, D3DTSS_COLOROP);
            m_pIDirect3DDevice8->GetTextureStageState(0, D3DTSS_COLORARG1, &COLORARG1);
            m_pIDirect3DDevice8->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
            m_pIDirect3DDevice8->GetTextureStageState(0, D3DTSS_COLORARG2, &COLORARG2);
            m_pIDirect3DDevice8->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

            m_pIDirect3DDevice8->GetTextureStageState(1, D3DTSS_COLOROP, &COLOROP2);
            m_pIDirect3DDevice8->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
            m_pIDirect3DDevice8->GetTexture(0, &oldTexture1);
            m_pIDirect3DDevice8->GetTexture(1, &oldTexture2);
            m_pIDirect3DDevice8->SetTexture(0, NULL);
            m_pIDirect3DDevice8->SetTexture(1, NULL);


            m_pIDirect3DDevice8->SetVertexShader(D3DFVF_XYZ | D3DFVF_DIFFUSE);
            for (DWORD i = 0; i < pointList.size(); i++)
            m_pIDirect3DDevice8->DrawPrimitiveUP(D3DPT_LINELIST, (pointList[i].numNodes - 1) / 2, &pointList[i].v[0], sizeof(pointList[i].v[0]));*/


            //}
            /*m_pIDirect3DDevice8->SetTexture(0, oldTexture1);
            m_pIDirect3DDevice8->SetTexture(1, oldTexture2);
            m_pIDirect3DDevice8->SetRenderState(D3DRS_ZENABLE, FALSE);
            //m_pIDirect3DDevice8->SetRenderState( D3DRS_ZENABLE, TRUE);
            m_pIDirect3DDevice8->SetRenderState(D3DRS_STENCILENABLE, StencileEnable);
            m_pIDirect3DDevice8->SetRenderState(D3DRS_CLIPPING, Clipping);
            m_pIDirect3DDevice8->SetRenderState(D3DRS_CLIPPLANEENABLE, Clipplane);


            m_pIDirect3DDevice8->SetRenderState(D3DRS_CULLMODE, Cull);
            m_pIDirect3DDevice8->SetPixelShader(pShared);
            m_pIDirect3DDevice8->SetVertexShader(vShared);
            m_pIDirect3DDevice8->SetRenderState(D3DRS_ZFUNC, D3DCMP);
            m_pIDirect3DDevice8->SetStreamSource(0, oldBuffer, oldStride);*/


            if (Gfx::world_rendertarget)
            {
                Gfx::world_rendertarget->Release();
                Gfx::world_rendertarget = NULL;
            }
            return;
}

