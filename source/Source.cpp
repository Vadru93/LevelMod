#undef NO_DEFINES
#undef NO_EXTRA_INCLUDE
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Commands.h"
#include "Extension.h"
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
#include "IniReader.h"
#include "IniWriter.h"
#include "Bugfixes.h"


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

void SetTagLimit(DWORD limit);


bool init = true;
bool init2 = false;
bool init3 = false;

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
char qbPath[256] = "";
char Level[256] = "";
bool downloading = false;
bool debugMode = false;
//int pSpeed;
static const char* clamp = "Clamp";
BYTE clampfunc[] = { 0x2E, 0x3E, 0x2B, 0xFF, 0xFE, 0xCE, 0xAE, 0xBE, 0x90, 0x90, 0x23 };
bool FirstTime = true;
void* __restrict mallocx(const DWORD buflen);
static const char* lastQB = NULL;
#define REV_END(x)  ((x << 24) | ((x << 8) & 0xFF0000) | (x >> 24) | ((x >> 8) & 0xFF00))
//#define CScript void
#define MAXRECV 20576
#undef SendMessage
struct CStruct;
struct CStructHeader;
struct CArray;
struct KeyState;
void SetStructValues(CStructHeader* pStruct, CStructHeader* values);
void SetArrayValues(CArray* pArray, CStructHeader* values);
#define VERSION 4.8f
static float version = VERSION;

bool InvalidReadPtr(const void* const __restrict ptr, const DWORD size);
bool InvalidReadPtr(const void* const __restrict ptr);
//FILE* _cdecl _fopen(const char* p, const char* b);
//void Ncomp_naked();
struct Matrix;

FILE* debugFile = NULL;


Vertex vertex;

extern ObserveMode* observe;




struct D3D_PARAMS
{
};

//HRESULT (*CreateDevice)(UINT Adapter,DWORD DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3D_PARAMS* pPresentationParameters,void** ppReturnedDeviceInterface);
//HRESULT (*Present)(LPDIRECT3DDEVICE8 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)=NULL;

/*HRESULT __stdcall Present(LPDIRECT3DDEVICE8 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
typedef HRESULT(__stdcall* Present_t)(LPDIRECT3DDEVICE8 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
Present_t pPresent;*/


__inline void HookFunction(DWORD addr, void* function, BYTE byteCode = 0, DWORD nopCount = 0)
{
	DWORD old;
	VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, PAGE_EXECUTE_READWRITE, &old);
	if (byteCode)
		*(DWORD*)(addr - 1) = byteCode;
	*(DWORD*)addr = (PtrToUlong(function) - addr) - 4;
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
		auto backup = { 0 };
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

Hook CreateDeviceHook;


Hook PresentHook;

char tags[256] = "Tags: 0";
LPD3DXFONT m_font = NULL;
RECT rct;




static BYTE test = 0;



void NotGood(DWORD checksum, CScript* pScript)
{
	_printf("Called a function that doesn't exsits..\nName %s in function %s location %p\n", FindChecksumName(checksum), FindChecksumName(pScript->scriptChecksum), pScript->address);
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

bool UsingNewMenu(CStruct* pStruct, CScript* pScript)
{
	return LevelModSettings::UseNewMenu;
}
bool UsingSoundFix(CStruct* pStruct, CScript* pScript)
{
	return LevelModSettings::FixSound;
}
bool UsingTeleFix(CStruct* pStruct, CScript* pScript)
{
	return LevelModSettings::TeleFix;
}

BYTE oldLimit = 32;

bool ToggleNewMenu(CStruct* pStruct, CScript* pScript)
{
	DWORD old;
	if (VirtualProtect((LPVOID)0x004404CE, sizeof(LevelModSettings::NewMenu), PAGE_EXECUTE_READWRITE, &old))
	{
		LevelModSettings::UseNewMenu = !LevelModSettings::UseNewMenu;
		if (LevelModSettings::UseNewMenu)
			memcpy((void*)0x004404CE, &LevelModSettings::NewMenu, sizeof(LevelModSettings::NewMenu));
		else
			memcpy((void*)0x004404CE, &LevelModSettings::OldMenu, sizeof(LevelModSettings::OldMenu));
		VirtualProtect((LPVOID)0x004404CE, sizeof(LevelModSettings::NewMenu), old, &old);
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
		return true;
	}
	else
	{
		MessageBox(0, "couldn't fix protection for", "NewMenu", 0);
		return false;
	}
}

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

bool CounterIsOn(CStruct* pStruct, CScript* pScript)
{
	return LevelModSettings::grafCounter;
}


bool ToggleGrafCounter(CStruct* pStruct, CScript* pScript)
{
	/*DWORD addr = NULL;
	do {
		addr = (DWORD)GetModuleHandle("d3d8.dll");
		Sleep(20);
	} while (!addr);

	DWORD_PTR* vTable = Hook::GetD3D8VTable(addr, 0x0001A4F4);//0x128000);
	if (!vTable || !vTable[15])
		return false;

	addr = (DWORD)vTable[15];

	LevelModSettings::grafCounter = !LevelModSettings::grafCounter;
	if (LevelModSettings::grafCounter)
	{
		BYTE backup[5];
		memcpy(backup, (void*)addr, 5);
		memcpy((void*)addr, PresentHook.backup, 5);
		memcpy(PresentHook.backup, backup, 5);
	}
	else
	{
		BYTE backup[5];
		memcpy(backup, (void*)addr, 5);
		memcpy((void*)addr, PresentHook.backup, 5);
		memcpy(PresentHook.backup, backup, 5);
	}*/
	LevelModSettings::grafCounter = !LevelModSettings::grafCounter;
	return true;
}

bool ToggleTeleFix(CStruct* pStruct, CScript* pScript)
{
	const static DWORD Addr = 0x004AE562;
	const static DWORD Addr2 = 0x004AE581;

	DWORD old;
	if (VirtualProtect((LPVOID)Addr, 4, PAGE_EXECUTE_READWRITE, &old))
	{
		if (VirtualProtect((LPVOID)Addr2, 1, PAGE_EXECUTE_READWRITE, &old))
		{
			LevelModSettings::TeleFix = !LevelModSettings::TeleFix;
			if (LevelModSettings::TeleFix)
			{
				*(DWORD*)Addr = 0x90909090;//84 c0 75 26
				*(BYTE*)Addr2 = 0x75;//74
			}
			else
			{
				*(DWORD*)Addr = 0x2675C084;
				*(BYTE*)Addr2 = 0x74;
			}
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
			return true;
		}
		else
		{
			MessageBox(0, "couldn't fix protection for", "TeleFix", 0);
			return false;
		}
	}
	else
	{
		MessageBox(0, "couldn't fix protection for", "TeleFix", 0);
		return false;
	}
}

bool ToggleSoundFix(CStruct* pStruct, CScript* pScript)
{
	const static DWORD Addr = 0x004C665D;
	DWORD old;
	if (VirtualProtect((void*)Addr, 1, PAGE_EXECUTE_READWRITE, &old))
	{
		LevelModSettings::FixSound = !LevelModSettings::FixSound;

		if (LevelModSettings::FixSound)
			*(BYTE*)Addr = 0xEB;
		else
			*(BYTE*)Addr = 0x75;
		//VirtualProtect((void*)Addr, 1, old, &old);
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
		return true;
	}
	else
	{
		MessageBox(0, "couldn't fix protection for", "SoundFix", 0);
		return false;
	}
}

bool GetMotd(CStruct* pStruct, CScript* pScript)
{
	/*WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	return false;
	SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0)
	{
	WSACleanup();
	return false;
	}
	SOCKADDR_IN service;
	service.sin_family = AF_INET;
	service.sin_port = htons(80);
	LPHOSTENT host = gethostbyname("vadru.chapter-3.net");
	if (!host)
	{
	closesocket(fd);
	WSACleanup();
	return false;
	}
	service.sin_addr = *((LPIN_ADDR)*host->h_addr_list);
	if (connect(fd, (SOCKADDR *)&service, sizeof(service)) < 0)
	{
	closesocket(fd);
	WSACleanup();
	return false;
	}
	static const char* GetStr = "GET /LevelModMotd HTTP/1.1\r\n"
	"Host: vadru.chapter-3.net\r\n"
	"User-Agent: Mozilla FireFox/4.0\r\n"
	"User-Agent: Mozilla Firefox/4.0\r\n"
	"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*//*;q=0.8\r\n"
	"Accept-Language: en-gb,en;q=0.5\r\n"
	"Accept-Encoding: gzip,deflate\r\n"
	"Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n\r\n";

	send(fd, GetStr, strlen(GetStr)+1, 0);
	std::string response = "";
	int resp_leng= 1024;
	char buffer[1024];
	while (resp_leng > 0)
	{
	resp_leng= recv(fd, (char*)&buffer, 1024, 0);
	if (resp_leng>0)
	response += std::string(buffer).substr(0,resp_leng);
	}
	if(strstr(response.c_str(),"404 Not Found"))
	{
	closesocket(fd);
	WSACleanup();
	return false;
	}
	else
	{
	int pos = response.find("\r\n\r\n");
	pos += 4;
	char motd[1024] = "";
	if(sscanf(&response.c_str()[pos], "%s", motd))
	{
	int id = 0;
	pStruct->GetScript("id", &id);
	//SetElementText(18, "hey");
	/*QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"motd"));
	if(header)
	{
	if(header->str)
	{
	if(strlen(motd)>strlen(header->str))
	{
	free(header->str);
	header->str = (char*)malloc(strlen(motd)+1);
	strcpy(header->str, motd);
	}
	else
	{
	char* p = motd;
	char* d = header->str;
	while(*p != '\0')
	{
	*d = *p;
	d++;
	p++;
	}
	*d = '\0';
	}
	}
	else
	{
	header->str = (char*)malloc(strlen(motd)+1);
	strcpy(header->str, motd);
	}
	return true;
	}
	else
	{
	header = AddQBKeyHeader(crc32f((unsigned char*)"motd"), 2);
	if(header)
	{
	header->type = QBKeyHeader::STRING;
	header->str = (char*)malloc(strlen(motd)+1);
	strcpy(header->str, motd);
	return true;
	}

	}*/
	/*}
	}*/

	/* response.clear();
	closesocket(fd);
	WSACleanup();*/
	return true;
}

void DestroySuperSectors()
{
	_printf("Going to remove MovingObjects\n");
	GameState::GotSuperSectors = false;
	if (movingObjects.size())
		movingObjects.clear();
}
void CreateSuperSectors()
{
	_printf("Going to create MovingObjects\n");
	GameState::GotSuperSectors = true;
}
FILE* logFile;
void __cdecl add_log(const char* string, ...)
{
	if (string == (const char*)0x005B6120) [[unlikely]]
		DestroySuperSectors();
	else if (string == (const char*)0x005B6104) [[unlikely]]
		CreateSuperSectors();

	if (debugMode) [[unlikely]]
	{
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
		if (pScript)
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

/*void LowMemoryMode()
{
*(DWORD*)0x0042C18D = 0x000FA000;
*(DWORD*)0x0042C1C7 = 0x000F9FF0;
*(DWORD*)0x0042C1D5 = 0x000F9FF0;
*(DWORD*)0x0042C1E1 = 0x000F9FF8;
*(DWORD*)0x0042C1EC = 0x000F9FFC;
*(BYTE*)0x00426088 = 0x75;
*(BYTE*)0x004260E7 = 0x75;
*(BYTE*)0x004288F7 = 0x75;
*(BYTE*)0x00428B97 = 0x75;
*(BYTE*)0x004288F7 = 0x75;
}*/

void NormalMemoryMode()
{
	/**(DWORD*)0x0042C18D = 0x000FA000;
	*(DWORD*)0x0042C1C7 = 0x000F9FF0;
	*(DWORD*)0x0042C1D5 = 0x000F9FF0;
	*(DWORD*)0x0042C1E1 = 0x000F9FF8;
	*(DWORD*)0x0042C1EC = 0x000F9FFC;
	*(BYTE*)0x00426088 = 0x74;
	*(BYTE*)0x004260E7 = 0x74;
	*(BYTE*)0x004288F7 = 0x74;
	*(BYTE*)0x00428B97 = 0x74;
	*(BYTE*)0x004288F7 = 0x74;*/

	/*00428B97

	  0042C280*/
}


bool hooked = false;
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

bool UnhookDebugMessages(CStruct* pStruct, CScript* pScript)
{
	debugMode = false;
	/*if (hooked)
	{
		hooked = false;
		memcpy((void*)0x00401960, oldCustomPrint, 6);

		static const DWORD addr = (DWORD)GetProcAddress(GetModuleHandle("msvcrt.dll"), "printf");
		if (addr)
		{
			memcpy((void*)addr, oldCustomPrint, 6);
		}
		fclose(logFile);
	}*/
	return true;
}

bool CreateConsole(CStruct* pStruct, CScript* pScript)
{
	return CreateConsole() == TRUE;
}
bool HideConsole(CStruct* pStruct, CScript* pScript)
{
	return FreeConsole() == TRUE;
}

bool HookDebugMessages(CStruct* pStruct, CScript* pScript)
{
	if (pStruct && pScript)
		debugMode = true;
	if (!hooked)
	{
		hooked = true;
		DWORD old;
		static BYTE callHooked[] = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0xC3 };

		VirtualProtect((void*)0x00401960, 6, PAGE_EXECUTE_READWRITE, &old);
		memcpy(oldCustomPrint, (void*)0x00401960, 6);

		*(DWORD*)&callHooked[1] = ((DWORD)add_log - 0x00401960 - 5);

		memcpy((void*)0x00401960, callHooked, 6);
		//VirtualProtect((void*)0x00401960, 6, old, &old);


		static const DWORD addr = (DWORD)GetProcAddress(GetModuleHandle("msvcrt.dll"), "printf");
		if (addr)
		{
			VirtualProtect((void*)addr, 6, PAGE_EXECUTE_READWRITE, &old);
			memcpy(oldCustomPrint, (void*)addr, 6);
			DWORD hookedAddrs = ((DWORD)add_log - addr - 5);
			*(DWORD*)&callHooked[1] = hookedAddrs;
			memcpy((void*)addr, callHooked, 6);
			//VirtualProtect((void*)addr, 6, old, &old);
		}
		//logFile = fopen("loggers.txt", "w+t");
	}
	return true;
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
						Message msg(connectionSocket, (const char*)&LevelModSettings::UnlimitedGraf, 1, clientInfo.sin_addr);
						HANDLE hRequestThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendMessage, (LPVOID)&msg, 0/*CREATE_SUSPENDED*/, NULL);
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
	if (LevelModSettings::UnlimitedGraf)
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartedGraf, &structScript, 0, 0);
	//MessageBox(0,"done","done",0);
	return true;
}

bool Kill3DGrass(CStruct* params, CScript* pScript)
{
	CStructHeader* param = params->AddParam("Name", QBKeyHeader::LOCAL);

	char name[256] = "";

	for (DWORD i = 0; i < 1000; i++)
	{
		sprintf(name, "3DGrassMesh%u", i);
		param->SetChecksum(name);
		ExecuteQBScript("KillIfAlive", params);
	}
	params->DellocateCStruct();
	return true;
}

bool UpdateLevelModSettings(CStruct* params, CScript* pScript)
{
	FILE* f = fopen("settings", "w+b");
	if (f)
	{
		_printf("going to save settings\n");
		fwrite(&LevelModSettings::UseNewMenu, 1, 1, f);
		fwrite(&LevelModSettings::FixSound, 1, 1, f);
		fwrite(&LevelModSettings::TeleFix, 1, 1, f);
		fwrite(&oldLimit, 1, 1, f);
		bool disableGrass = false;
		QBKeyHeader* header = GetQBKeyHeader(Checksum("GrassLayersDisabled"));
		if (header)
			disableGrass = header->value.i != 0;
		fwrite(&disableGrass, 1, 1, f);
		bool slapOn = true;
		header = GetQBKeyHeader(Checksum("SlapIsOn"));
		if (header)
			slapOn = header->value.i != 0;
		fwrite(&slapOn, 1, 1, f);
		bool hudOn = true;
		header = GetQBKeyHeader(Checksum("HudIsOn"));
		if (header)
			hudOn = header->value.i != 0;
		fwrite(&hudOn, 1, 1, f);
		fwrite(&version, 4, 1, f);

		/*bool b = false;
		header = GetQBKeyHeader(Checksum("bShowConsole"));
		if (header)
			b = header->value.i != 0;
		fwrite(&b, 1, 1, f);
		header = GetQBKeyHeader(Checksum("bPrintDebug"));
		if (header)
			b = header->value.i != 0;
		fwrite(&b, 1, 1, f);*/
		fwrite(&debugMode, 1, 1, f);
		header = GetQBKeyHeader(Checksum("spine_button_text"));
		if (header)
		{
			if (strstr(header->pStr, "Revert"))
			{
				LevelModSettings::SpineButton = KeyState::REVERT;
			}
			else if (strstr(header->pStr, "Nollie"))
			{
				LevelModSettings::SpineButton = KeyState::NOLLIE;
			}
			else if (strstr(header->pStr, "Left"))
			{
				LevelModSettings::SpineButton = KeyState::SPINLEFT;
			}
			else if (strstr(header->pStr, "Right"))
			{
				LevelModSettings::SpineButton = KeyState::SPINRIGHT;
			}
			fwrite(&LevelModSettings::SpineButton, 1, 1, f);
		}
		fclose(f);
	}
	ExecuteQBScript("LoadLevelModSettings", params);
	return true;
}

bool ToggleHookDebugMessages(CStruct* pStruct, CScript* pScript)
{
	static const DWORD addr = (DWORD)GetProcAddress(GetModuleHandle("msvcrt.dll"), "printf");
	hooked = !hooked;
	if (hooked)
	{
		logFile = fopen("loggers.txt", "w+t");
		DWORD old;
		static BYTE callHooked[] = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0xC3 };

		VirtualProtect((void*)0x00401960, 6, PAGE_EXECUTE_READWRITE, &old);
		memcpy(oldCustomPrint, (void*)0x00401960, 6);

		*(DWORD*)&callHooked[1] = ((DWORD)add_log - 0x00401960 - 5);

		memcpy((void*)0x00401960, callHooked, 6);
		//VirtualProtect((void*)0x00401960, 6, old, &old);

		if (addr)
		{
			VirtualProtect((void*)addr, 6, PAGE_EXECUTE_READWRITE, &old);
			memcpy(oldCustomPrint, (void*)addr, 6);
			DWORD hookedAddrs = ((DWORD)add_log - addr - 5);
			*(DWORD*)&callHooked[1] = hookedAddrs;
			memcpy((void*)addr, callHooked, 6);
			//VirtualProtect((void*)addr, 6, old, &old);
		}
	}
	else
	{
		fclose(logFile);
		memcpy((void*)0x00401960, oldCustomPrint, 6);

		if (addr)
		{
			memcpy((void*)addr, oldCustomPrint, 6);
		}
	}
	return true;
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
		LevelModSettings::UnlimitedGraf = (limit != 32);//Set the new settings




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

char PoolSize[128] = "";
char PoolSizeText[128] = "";

bool GetMemoryPoolSize(CStruct* pStruct, CScript* pScript)
{
	sprintf(PoolSize, "%u", LevelModSettings::MemorySize / 0x10);
	CStructHeader* header = pScript->params->head;
	while (header)
	{
		if (header->QBkey == 0x083FDB95)
		{
			header->pStr = PoolSize;
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
	pHeader->Type = QBKeyHeader::STRING;
	pHeader->QBkey = 0x083FDB95;
	pHeader->pStr = PoolSize;
	pHeader->NextHeader = NULL;
	return true;
}

bool GetMemoryPoolSizeText(CStruct* pStruct, CScript* pScript)
{
	sprintf(PoolSizeText, "MemoryPoolSize: %u", LevelModSettings::MemorySize / 0x10);
	CStructHeader* header = pScript->params->head;
	while (header)
	{
		if (header->QBkey == 0x083FDB95)
		{
			header->pStr = PoolSizeText;
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
	pHeader->Type = QBKeyHeader::STRING;
	pHeader->QBkey = 0x083FDB95;
	pHeader->pStr = PoolSizeText;
	pHeader->NextHeader = NULL;
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
	static const pCastPointer CastPointer = (pCastPointer)(0x00577E58);

	if (pStruct->GetScript("menu_id", &id))
	{
		Element* container = AllocateElement(0);
		Element* element = container->GetElement(id);
		element = (Element*)CastPointer((void*)element, 0, 0x005B6344, 0x005B666C, FALSE);

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

std::map<int, int> options;


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
	{ "SubToGlobal", SubToGlobal },
	{ "AddToGlobal", AddToGlobal },
	{ "FreezeCamera", FreezeCamera },
	{ "UnfreezeCamera", UnfreezeCamera },
	{ "GrafStarted", GrafStarted },
	{ "ChangeValues", ChangeValues },
	{ "CreatePair", CreatePair },
	{ "GetSliderValue", GetSliderValue },
	{ "InitLevelMod", Initialize },
	{ "MoveObject", MoveObjectScript   },
{ "KillMovingObject", KillMovingObjectScript},
{"ChangeParamToUnnamed", ChangeParamToUnnamedScript},
{"IsOptionOn", IsOptionOn},
{"AddOption", AddOption},
{"ToggleOption", ToggleOption},
	{"LM_GotParam", LM_GotParamScript },
	{"GetParam", GetParamScript},
	{"SetOption", SetOption},
	{"GetOptionText", GetOptionText},
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
	downloading = true;
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
	downloading = false;
}

bool shouldfix = false;

void FixClampBugs()
{
	if (shouldfix)
	{
		MessageBox(0, clamp, "YEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEAH", 0);
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
	Skater* skater = Skater::GetSkater();
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


		if (skater->CollisionCheck(0x8, false))
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
bool hookDone = true;

void AddFunctions();

void HookedFopen(char* p)
{
	if (p && strlen(p))
	{
		if (p[strlen(p) - 1] == 'b' && p[strlen(p) - 2] == 'q' && p[strlen(p) - 3] == '.' && game)
		{
			if (hookDone)
			{
				AddFunctions();
				hookDone = false;
			}
			if (debugMode)
			{
				_printf("Fopen: %s\n", p);
				memcpy(qbPath, p, strlen(p) + 1);
				qbPath[strlen(qbPath) - 1] = 0x0;

				/*MessageBox(0, p, qbPath, 0);
				AddCompressedNodes();*/
				if (debugMode)
					QScript::Scripts->OpenScript(p);
			}
		}
	}
}

/*__declspec(naked) void Fopen_naked()
{
	static const DWORD jmpBack = 0x0040314A + 4;
	_asm pushad;
	_asm pushfd;
	_asm mov path, ecx
	HookedFopen(path);
	_asm popfd;
	_asm popad;
	_asm add esp, 0x0C;
	_asm mov al, [ecx];
	_asm jmp[jmpBack];
}*/


void AddChecksum(int key, char* name, void* retAddr)
{
	lastQB = name;
	if (name && !InvalidReadPtr(name) && strlen(name) > 1)
	{
		std::map<int, char*>::iterator it = QScript::Scripts->qbTable.find(key);
		if (it == QScript::Scripts->qbTable.end())
		{
			/*FILE* debugFile = fopen("debug.txt", "r+t");
			fseek(debugFile, 0, SEEK_END);
			fprintf(debugFile, "AddingKey %s %X, CalledFrom %p\r\n", name, key, retAddr);
			printf("AddingKey %s %X, CalledFrom %p\r\n", name, key, retAddr);
			fclose(debugFile);*/
			//_printf("AddChecksum %s 0x%X\n", name, key);
			QScript::Scripts->qbTable.insert(std::pair<int, char*>(key, name));
			QScript::qbKeys.push_back(key);
		}
		/*else if (_stricmp(it->second, name))
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
		std::map<int, char*>::iterator it = QScript::Scripts->qbTable.find(key);
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

/*CStructHeader* HookAddCStruct(int ParamName, int Data, int qbType)
{
	if (ParamName == 0xD)
	{

	}
}*/

/*struct Node
{
	CStructHeader* first;
	CStructHeader* last;
};
*/





/*__declspec(naked) void Ncomp_naked()
{
	static CStruct* node = NULL;
	static DWORD checksum = 0;
	static const DWORD jmpBack = 0x004294C3;
	static const DWORD jmpBack2 = 0x0042945F;
	static const DWORD oldESP = 0;
	static BYTE* pFile = NULL;
	_asm push esp
	_asm pushad;
	_asm pushfd;
	_asm cmp ebx, 0x68A9B4E1;
	_asm je hi;
	_asm popfd;
	_asm popad;
	_asm pop esp;
	_asm add esp, 8;
	_asm mov ecx, ebp;
	_asm jmp[jmpBack];
hi:

	_asm mov node, ebp;
	_asm mov pFile, eax;
	game = false;
	pFile++;
	checksum = *(DWORD*)pFile;
	pFile += 4;

	AddCompressedNode(checksum, node);

	game = true;
	_asm popfd;
	_asm popad;
	_asm pop esp
	_asm add esp, 8
	//_asm mov byte ptr[esp - 0x15], 00;
	_asm mov eax, pFile;
	_asm jmp[jmpBack2];
}*/
//char* fopen_path = NULL;

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
		QScript::Scripts->OpenScript((char*)p);
	}

	return fopen(p, b);
}

__declspec(naked) void TestForAcid_naked()
{
	static DWORD retaddr = 0;
	_asm pop retaddr;
	_asm pushad;
	_asm pushfd;
	TestForAcid();
	_asm popfd;
	_asm popad;
	_asm sub esp, 0x000000B4
	_asm push retaddr;
	_asm ret
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

bool DefaultMemoryMode()
{
	FILE* temp = fopen("temp", "w+b");
	if (temp)
	{
		LevelModSettings::MemorySize = 0xFA000;

		CStructHeader* backup = *(CStructHeader**)0x008E1E00;
		for (DWORD i = 0; i < 0xFA00; i++)
		{
			fwrite(backup, sizeof(CStructHeader), 1, temp);
			backup++;
		}
		freex((void*)*(CStructHeader**)0x008E1E00);
		CStructHeader* head = (CStructHeader*)mallocx(0xFA000);
		CStructHeader** ptr = *(CStructHeader***)0x008E1E2C + 64000;
		CStructHeader** end = *(CStructHeader***)0x008E1E30;

		CStructHeader* old = *(CStructHeader**)0x008E1E00;
		CStructHeader* oldEnd = old + 0xFA00;

		while (ptr < end)
		{
			if (*ptr > old && *ptr < oldEnd)
			{
				CStructHeader* parser = *ptr;
				DWORD index = ((DWORD)parser - (DWORD)old) / 0x10;
				parser = head + index;
				if (parser > head && parser < (head + 0xFA00))
					*ptr = parser;
			}
			ptr++;//;= (CStructHeader**)((DWORD)ptr+1);
		}
		//CStructHeader* old = *(CStructHeader**)0x008E1E00;
		*(CStructHeader**)0x008E1E00 = head;
		CStructHeader* parser = *(CStructHeader**)0x008E1E04;
		DWORD index = ((DWORD)parser - (DWORD)old) / 0x10;
		*(CStructHeader**)0x008E1E04 = head + index;//*(CStructHeader**)0x008E1E04+(head-old);
		if (fflush(temp) == 0)
		{
			fseek(temp, 0, SEEK_SET);
			backup = head;
			for (DWORD i = 0; i < 0xFA00; i++)
			{
				fread(backup, sizeof(CStructHeader), 1, temp);
				backup++;
			}
			fclose(temp);
			CStructHeader* header = head;
			DWORD last = (DWORD)head + 0xFA000;
			while ((DWORD)header < last)
			{
				header->NextHeader = header + 1;
				header++;
			}

			header = head + 0xF9FF;
			header->Type = QBKeyHeader::QBKeyType::UNDEFINED;
			header->value.i = 0;
			header->NextHeader = 0;
			FILE* f = fopen("settings", "w+b");
			if (f)
			{
				fwrite(&LevelModSettings::UseNewMenu, 1, 1, f);
				fwrite(&LevelModSettings::FixSound, 1, 1, f);
				fwrite(&LevelModSettings::TeleFix, 1, 1, f);
				fwrite(&oldLimit, 1, 1, f);
				bool disableGrass = false;
				QBKeyHeader* header = GetQBKeyHeader(Checksum("GrassLayersDisabled"));
				if (header)
					disableGrass = header->value.i != 0;
				fwrite(&disableGrass, 1, 1, f);
				fclose(f);
				_fcloseall();
			}
		}
		else
			fclose(temp);
		DeleteFile("temp");
		return true;
	}
	return false;
}

/*VirtualProtect((void*)0x004260E7, 7, PAGE_EXECUTE_READWRITE, &old);
*(DWORD*)0x004260E7 = 0x9090F775;
*(WORD*)0x004260EB = 0x9090;
*(BYTE*)0x004260ED = 0x90;*/

void AddLock()
{
	DWORD old;

	VirtualProtect((void*)0x004288F1, 1, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00428B91, 1, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x004260E1, 1, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00426081, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x004288F1 = 0xF9;
	*(BYTE*)0x00428B91 = 0xF9;
	*(BYTE*)0x004260E1 = 0xF9;
	*(BYTE*)0x00426081 = 0xF9;

	VirtualProtect((void*)0x004260E7, 7, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x004260E7 = 0x9090F775;
	*(WORD*)0x004260EB = 0x9090;
	*(BYTE*)0x004260ED = 0x90;

	VirtualProtect((void*)0x004288F7, 7, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x004288F7 = 0x9090F775;
	*(WORD*)0x004288FB = 0x9090;
	*(BYTE*)0x004288FD = 0x90;

	VirtualProtect((void*)0x00428B97, 4, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x00428B97 = 0x3DEBF775;

	VirtualProtect((void*)0x00426088, 4, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x00426088 = 0x2DEBF675;

	/*VirtualProtect((void*)0x004260E7, 4, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x004260E7 = 0x3DEBF775;*/
}

bool HighMemoryMode(DWORD newSize)
{
	if (newSize == LevelModSettings::MemorySize)
		return true;
	if (newSize <= 0xFA000)
	{
		if (LevelModSettings::MemorySize != 0xFA000)
			return DefaultMemoryMode();
		return false;
	}
	DWORD newCount = newSize / 0x10;
	DWORD oldSize = LevelModSettings::MemorySize;
	if (newSize < oldSize)
	{
		//MessageBox(0, "new size needs to be bigger than old", "...", 0);
		//return false;
		oldSize = newSize;
	}
	FILE* temp = fopen("temp", "w+b");
	if (temp)
	{
		LevelModSettings::MemorySize = newSize;
		DWORD oldCount = oldSize / 0x10;

		CStructHeader* backup = *(CStructHeader**)0x008E1E00;
		for (DWORD i = 0; i < oldCount; i++)
		{
			fwrite(backup, sizeof(CStructHeader), 1, temp);
			backup++;
		}
		ZeroMemory((void*)*(CStructHeader**)0x008E1E00, oldSize);
		freex((void*)*(CStructHeader**)0x008E1E00);
		CStructHeader* head = (CStructHeader*)mallocx(newSize);
		CStructHeader** ptr = *(CStructHeader***)0x008E1E2C + 30000 - 3804;
		//*(ptr-5)=head+newCount;
		CStructHeader** end = *(CStructHeader***)0x008E1E30 + ((DWORD) * (CStructHeader***)0x008E1E34);//120000

		CStructHeader* old = *(CStructHeader**)0x008E1E00;
		CStructHeader* oldEnd = old + oldCount;

		while (ptr < end)
		{
			if (*ptr >= old && *ptr < oldEnd)
			{
				CStructHeader* parser = *ptr;
				DWORD index = ((DWORD)parser - (DWORD)old) / 0x10;
				parser = head + index;
				if (parser >= head && parser < (head + newCount))
					*ptr = parser;
			}
			ptr++;//;= (CStructHeader**)((DWORD)ptr+1);
		}

		if (head == NULL)
		{
			newSize = oldSize;
			newCount = oldCount;
			head = (CStructHeader*)mallocx(oldSize);
			if (head == NULL)
			{
				newSize = 0xFA000;
				newCount = 0xFA00;
				head = (CStructHeader*)mallocx(0xFA000);
				if (head == NULL)
					return false;
			}
		}
		//CStructHeader* old = *(CStructHeader**)0x008E1E00;
		*(CStructHeader**)0x008E1E00 = head;
		CStructHeader* parser = *(CStructHeader**)0x008E1E04;
		if (parser >= old && parser < oldEnd)
		{
			DWORD index = ((DWORD)parser - (DWORD)old) / 0x10;
			*(CStructHeader**)0x008E1E04 = head + index;//*(CStructHeader**)0x008E1E04+(head-old);
		}
		if (fflush(temp) == 0)
		{
			fseek(temp, 0, SEEK_SET);
			backup = head;
			for (DWORD i = 0; i < oldCount; i++)
			{
				fread(backup, sizeof(CStructHeader), 1, temp);
				CStructHeader* parser = (CStructHeader*)backup->pData;
				if (parser >= old && parser < oldEnd)
				{
					DWORD index = ((DWORD)parser - (DWORD)old) / 0x10;
					parser = head + index;
					if (parser >= head && parser < (head + newCount))
						backup->pData = (void*)parser;
				}
				if (backup->NextHeader == NULL || (backup->NextHeader >= old && backup->NextHeader < oldEnd))
					backup->NextHeader = backup + 1;
				backup++;
			}
			fclose(temp);


			CStructHeader* header = head + (oldCount - 1);//-1
			DWORD last = (DWORD)head + newSize;
			while ((DWORD)header < last)
			{
				header->Type = QBKeyHeader::QBKeyType::UNDEFINED;
				header->QBkey = 0;
				header->Data = 0;
				header->NextHeader = header + 1;
				header++;
			}
			header = head + (newCount - 1);
			header->Type = QBKeyHeader::QBKeyType::UNDEFINED;
			header->QBkey = 0;
			header->Data = 0;
			header->NextHeader = NULL;
		}
		else
			fclose(temp);
		DeleteFile("temp");
		return true;
	}
	return false;
}

bool SetMemoryPoolSize(CStruct* pStruct, CScript* pScript)
{
	void** puEBP = NULL;
	__asm { mov puEBP, ebp };
	char ok[25];
	sprintf(ok, "%p %p", pStruct, pScript->params);
	MessageBox(0, ok, ok, 0);
	/*void * pvReturn = puEBP[2];
	char ok[25];
	sprintf(ok, "%X %p", pvReturn, pvReturn);
	MessageBox(0,ok,"",0);
	puEBP = (void**)puEBP[0];
	sprintf(ok, "%X %p", puEBP, puEBP);
	MessageBox(0,ok,"",0);*/
	Sleep(1000);
	const char* string;
	if (pStruct->GetString("string", &string))
	{
		const  BYTE* it = (const BYTE*)string;
		DWORD size = 0;
		while (*it >= '0' && *it <= '9')
		{
			size = size * 10 + *it - '0';
			it++;
		}
		if (size)
		{
			ExecuteQBScript("StopScripts", NULL);
			*(bool*)0x008E1DF9 = true;
			CStructHeader* old = *(CStructHeader**)0x008E1E00;

			DWORD oldSize = LevelModSettings::MemorySize;
			DWORD oldCount = oldSize / 0x10;

			bool ret = HighMemoryMode(size * 0x10);



			CStructHeader* head = *(CStructHeader**)0x008E1E00;
			CStructHeader* oldEnd = old + oldCount;

			CStructHeader* header = pStruct->head;
			if (header)
			{
				if (header >= old && header <= oldEnd)
				{
					DWORD index = ((DWORD)pStruct->head - (DWORD)old) / 0x10;
					pStruct->head = head + index;
				}
				if (pStruct->tail >= old && pStruct->tail <= oldEnd)
				{
					DWORD index = ((DWORD)pStruct->tail - (DWORD)old) / 0x10;
					pStruct->tail = head + index;
				}
				while (header && header->NextHeader)
				{
					if (header->NextHeader >= old && header->NextHeader <= oldEnd)
					{
						DWORD index = ((DWORD)header->NextHeader - (DWORD)old) / 0x10;
						header->NextHeader = head + index;
					}
					header = header->NextHeader;
				}
			}
			if (pScript->params)
			{
				header = pScript->params->head;
				if (header)
				{
					if (header >= old && header <= oldEnd)
					{
						DWORD index = ((DWORD)pScript->params->head - (DWORD)old) / 0x10;
						pScript->params->head = head + index;
					}
					if (pScript->params->tail >= old && pScript->params->tail <= oldEnd)
					{
						DWORD index = ((DWORD)pScript->params->tail - (DWORD)old) / 0x10;
						pScript->params->tail = head + index;
					}
					while (header && header->NextHeader)
					{
						if (header->NextHeader >= old && header->NextHeader <= oldEnd)
						{
							DWORD index = ((DWORD)header->NextHeader - (DWORD)old) / 0x10;
							header->NextHeader = head + index;
						}
						header = header->NextHeader;
					}
				}
			}

			*(bool*)0x008E1DF9 = false;
			//MessageBox(0,"done",0,0);
			return ret;
		}

		return false;
	}

	if (LevelModSettings::MemorySize != 0xFA000)
	{
		*(bool*)0x008E1DF9 = true;
		bool ret = DefaultMemoryMode();
		*(bool*)0x008E1DF9 = false;
		return ret;
	}
	return false;
}

DWORD newSize;

void HighMemMode()
{

}

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
}

//void HookControls();
void InitLevelMod()
{

	extern std::map<DWORD, DWORD> movableObjects;
	movableObjects.insert(std::pair<DWORD, DWORD>(Checksum("dt_blimp_GameOb"), Checksum("dt_blimp")));
	//HookControls();
	*(WORD*)0x00427A9B = 0x840F;//je
	HookFunction(0x00427A9D, NotGood_naked);
	*(DWORD*)0x00427AA3 = 0x90909090;//nop
	*(WORD*)(0x00427AA3 + 4) = 0x9090;
    
	HookFunction(0x0048E036, Obj_MoveToNode_Naked, 0xE9);
	HookFunction(0x0048DA53, Obj_FollowPathLinked_Naked, 0xE9);

	//HookFunction(0x0058D0B1, Fopen_naked, 0xE9);

	if (debugMode)
		HookFunction(0x004265F1, Checksum_naked, 0xE9);
	char msg[128] = "";
	/*sprintf(msg, "OFFSET %X", offset);
	MessageBox(0, msg, msg, 0);*/

	InitialRand(GetTickCount());
	QBKeyHeader* header = NULL;


	header = GetQBKeyHeader(Checksum("DumpScripts"));
	if (header)
	{
		header->pFunction = DumpScripts;
	}

	//MessageBox(0, "going to fix msg", "going to fix msg", MB_OK);


	/*FILE* f;
	f = fopen("settings", "r+b");
	BYTE tagLimit;
	tagLimit = 32;
	if (movingObjects.size())
		movingObjects.clear();

	HookDebugMessages(NULL, NULL);

	if (f)
	{
		_printf("Loading settings...\n");
		fread(&LevelModSettings::UseNewMenu, 1, 1, f);
		fread(&LevelModSettings::FixSound, 1, 1, f);
		fread(&LevelModSettings::TeleFix, 1, 1, f);
		if (fread(&tagLimit, 1, 1, f) != 1)
			fwrite(&tagLimit, 1, 1, f);
		bool disableGrass = false;
		if (fread(&disableGrass, 1, 1, f) != 1)
			fwrite(&disableGrass, 1, 1, f);
		else
		{
			QBKeyHeader* header = GetQBKeyHeader(Checksum("GrassLayersDisabled"));
			if (header)
				header->value.i = disableGrass;
		}
		bool slapOn = true;
		if (fread(&slapOn, 1, 1, f) != 1)
			fwrite(&slapOn, 1, 1, f);
		else if (!slapOn)
		{
			QBKeyHeader* header = GetQBKeyHeader(Checksum("InternetClientCollRadius"));
			if (header)
				header->value.i = 0;
			header = GetQBKeyHeader(Checksum("InternetServerCollRadius"));
			if (header)
				header->value.i = 0;
			header = GetQBKeyHeader(Checksum("SlapIsOn"));
			if (header)
				header->value.i = 0;
		}
		bool hudOn = true;
		if (fread(&hudOn, 1, 1, f) != 1)
			fwrite(&hudOn, 1, 1, f);
		else if (!hudOn)
		{
			QBKeyHeader* header = GetQBKeyHeader(Checksum("HudIsOn"));
			if (header)
				header->value.i = 0;
		}
		if (fread(&version, 4, 1, f) != 1)
		{
			_printf("version not found\n");
			version = VERSION;
			fwrite(&version, 4, 1, f);
		}
		_printf("Setting Version %.2f\nLevelMod Version %.2f", version, VERSION);
		//
		if (version != VERSION)
		{
			_printf("version not same\n");
			fseek(f, -4, SEEK_CUR);
			version = VERSION;
			fwrite(&version, 4, 1, f);
			debugMode = false;
			fwrite(&debugMode, 1, 1, f);
			SpineButton = 7;
			fwrite(&SpineButton, 1, 1, f);

			header = GetQBKeyHeader(Checksum("spine_button_text"));
			if (header)
				memcpy(header->pStr, "Revert", 7);
		}
		else
		{
			if (fread(&debugMode, 1, 1, f) != 1)
				fwrite(&debugMode, 1, 1, f);
			else if (debugMode)
			{

				CreateConsole();
				qbTable = new Script();

				//debugFile = fopen("debug.txt", "r+t");
				//fseek(debugFile, 0, SEEK_END);

				CompiledScript* pScript = (CompiledScript*)0x005B83D8;
				for (DWORD i = 0; i < 292; i++)
				{
					if (pScript->name && (DWORD)pScript->name != 0xFFFFFFFF && !InvalidReadPtr(pScript->name) && strlen(pScript->name) > 1 && strlen(pScript->name) < 50)
					{
						printf("Adding ScriptedCFunction %s %X %p\n", pScript->name, crc32f((unsigned char*)pScript->name), pScript->pFunction);
						//fprintf(debugFile, "Adding ScriptedCFunction %s %X %p\n", pScript->name, crc32f((unsigned char*)pScript->name), pScript->pFunction);
					}
					pScript++;
				}

				pScript = (CompiledScript*)0x005B7510;
				for (DWORD i = 0; i < 473; i++)
				{
					if (pScript->name && (DWORD)pScript->name != 0xFFFFFFFF && !InvalidReadPtr(pScript->name) && strlen(pScript->name) > 1 && strlen(pScript->name) < 50)
					{
						printf("Adding ScriptedCFunction %s %X %p\n", pScript->name, crc32f((unsigned char*)pScript->name), pScript->pFunction);
						//fprintf(debugFile, "Adding ScriptedCFunction %s %X %p\n", pScript->name, crc32f((unsigned char*)pScript->name), pScript->pFunction);
					}
					pScript++;

				}
				//fclose(debugFile);





			}
			if (fread(&SpineButton, 1, 1, f) != 1)
			{
				SpineButton = 7;
				fwrite(&SpineButton, 1, 1, f);
			}

			header = GetQBKeyHeader(Checksum("spine_button_text"));
			if (header)
			{
				switch (SpineButton)
				{
				case KeyState::REVERT:
					memcpy(header->pStr, "Revert", 7);
					break;
				case KeyState::NOLLIE:
					SpineButton = KeyState::NOLLIE;
					memcpy(header->pStr, "Nollie", 7);
					break;
				case KeyState::SPINLEFT:
					SpineButton = KeyState::SPINLEFT;
					memcpy(header->pStr, "Left Spin Button", 17);
					break;
				case KeyState::SPINRIGHT:
					SpineButton = KeyState::SPINRIGHT;
					memcpy(header->pStr, "Right Spin Button", 18);
					break;
				default:
					SpineButton = KeyState::REVERT;
					_printf("Trashed LevelMod Settings file...\n");
					memcpy(header->pStr, "Revert", 7);
					break;
				}
			}
		}
		version = VERSION;
	}
	else
	{
		_printf("Writing settings...\n");
		f = fopen("settings", "w+b");
		if (f)
		{
			fwrite(&LevelModSettings::UseNewMenu, 1, 1, f);
			fwrite(&LevelModSettings::FixSound, 1, 1, f);
			fwrite(&LevelModSettings::TeleFix, 1, 1, f);
			fwrite(&tagLimit, 1, 1, f);
			bool disableGrass = false;
			fwrite(&disableGrass, 1, 1, f);
			bool slapOn = true;
			fwrite(&slapOn, 1, 1, f);
			bool hudOn = true;
			fwrite(&hudOn, 1, 1, f);
			version = VERSION;
			fwrite(&version, 4, 1, f);
			debugMode = false;
			fwrite(&debugMode, 1, 1, f);
			SpineButton = 7;
			fwrite(&SpineButton, 1, 1, f);
			header = GetQBKeyHeader(Checksum("spine_button_text"));
			if (header)
				memcpy(header->pStr, "Revert", 7);
			//fwrite(&newSize, 4, 1, f);
			fclose(f);
		}
	}
	_fcloseall();



	if (LevelModSettings::UseNewMenu)
	{
		if (VirtualProtect((LPVOID)0x004404CE, sizeof(LevelModSettings::NewMenu), PAGE_EXECUTE_READWRITE, &old))
			memcpy((void*)0x004404CE, &LevelModSettings::NewMenu, sizeof(LevelModSettings::NewMenu));
		else
			MessageBox(0, "couldn't fix protection for", "NewMenu", 0);
	}
	if (LevelModSettings::TeleFix)
		LevelModSettings::FixTele();
	if (LevelModSettings::FixSound)
	{
		if (VirtualProtect((LPVOID)0x004C665D, 1, PAGE_EXECUTE_READWRITE, &old))
			*(BYTE*)0x004C665D = 0xEB;
		else
			MessageBox(0, "couldn't fix protection for", "SFXFix", 0);
	}
	if (tagLimit != 32)
	{
		CStruct pStruct;//= new CStruct;
		CStructHeader head;
		pStruct.head = &head;//new CStructHeader;
		pStruct.tail = pStruct.head;
		//oldLimit = tagLimit;
		pStruct.head->Data = tagLimit;
		pStruct.head->Type = QBKeyHeader::QBKeyType::INT;
		pStruct.head->QBkey = 0;
		SetTagLimit(&pStruct, NULL);
		//delete pStruct.head;
		LevelModSettings::UnlimitedGraf = true;
	}
	*/


	/*//MessageBox(0, "going to hook", "going to hook", MB_OK);
	PresentHook = Hook(15, HookedPresent, 1);//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)D3D8BASE, NULL, NULL, NULL);
	pPresent = (Present_t)PresentHook.func;*/

	//HookFopen();
	/*if (HookFopen())
		MessageBox(0, "sucess", "", MB_OK);
	else
		MessageBox(0, "fail", "", MB_OK);*/
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Test, 0, 0, 0);
		//MessageBox(0, "DOne", "", MB_OK);
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
		Slerp::m_last_wallplant_time_stamp = 0;
		Slerp::realVelocity = Vertex(0.0f, 0.0f, 0.0f);
		Slerp::targetNormal = Vertex(0.0f, 0.0f, 0.0f);
		Slerp::target_normal = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

		Slerp::speed = 0.0f;

		LevelModSettings::UseNewMenu = true;
		LevelModSettings::AA = false;
		LevelModSettings::AllowNewTricks =LevelModSettings::ALLOW_NONE;
		LevelModSettings::UnlimitedGraf = false;
		LevelModSettings::FixSound = true;
		LevelModSettings::TeleFix = true;
		LevelModSettings::grafCounter = true;
		LevelModSettings::MemorySize = 0xFA000;
		LevelModSettings::HookedControls = false;


		LevelModSettings::SpineButton = 7;
		LevelModSettings::SpineButton2 = -1;

		/*LevelModSettings::NewMenu = { 0x90, 0x90, 0x90, 0x90, 0x74, 0x70, 0xEB, 0x7B, 0x90 };//000404ce
		LevelModSettings::OldMenu = { 0xC6, 0x46, 0x04, 0x01, 0x74, 0x70, 0x57, 0x6A, 0x00 };*/
		/*FILE* fp = fopen("LevelMod.ini", "ab+");
		fseek(fp, 0, SEEK_END);
		DWORD size = ftell(fp);


		if (size == 0)//file was empty
		{*/
		char temp[MAX_PATH] = "";
		GetCurrentDirectory(MAX_PATH, temp);
		sprintf(IniPath, "%s/LevelMod.ini", temp);
		OptionWriter = new CIniWriter(IniPath);
		OptionReader = new CIniReader(IniPath);

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
		printf("Init DONE\n");
		return true;

	}
	else
	{
		_printf("Already inited\n");

		init3 = true;
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

LPDIRECT3DDEVICE9 pDevice = NULL;;

void DrawLines()
{
	//pDevice->SetTransfD3DTRANSOFMRorm(0, &lineWorld);
	pDevice->SetFVF(line_fvf);
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST,         //PrimitiveType
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
	if (observe)
		delete observe;
	observe = NULL;
	if (OptionWriter)
		delete OptionWriter;
	OptionWriter = NULL;
	if (OptionReader)
		delete OptionReader;
	OptionReader = NULL;
}

void OnLost()
{
	_printf("OnLost\n");
	if (m_font)
		m_font->OnLostDevice();
}

void OnReset()
{
	_printf("OnReset\n");
	if (m_font)
		m_font->OnResetDevice();
}


void DrawFrame()
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

	D3DXCreateFontA(pDevice, 45, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &m_font);
	}

		if (GameState::GotSuperSectors) [[likely]]
		{
			Skater * skater = Skater::GetSkater();
			if (skater) [[likely]]
			{
				if (XINPUT::Player1->IsConnected())
				{
					if (XINPUT::vibrationFrames)
					{
						XINPUT::Player1->Vibrate(XINPUT::vibration.wLeftMotorSpeed, XINPUT::vibration.wRightMotorSpeed);
						XINPUT::vibrationFrames -= skater->GetFrameLength();
					}
					else
					{
						XINPUT::Player1->Vibrate(0, 0);
					}

				}
			//ProxyPad(skater);

			updatingObjects = true;
			for (DWORD i = 0; i < movingObjects.size(); i++) [[unlikely]]
			{
				//_printf("Killed?...");
				if (!(movingObjects[i].state & MeshState::kill)) [[likely]]
				{
					//_printf("FALSE\n");
					if (movingObjects[i].Update(skater->GetFrameLength()))
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
			if (observe && observe->observing) [[unlikely]]
			{
				Skater * skater = Skater::GetSkater();
				if (skater)
				{
					KeyState* ollie = skater->GetKeyState(KeyState::OLLIE);
					if (ollie->IsPressed() && ollie->GetPressedTime() != observe->timeNext)
						observe->Next(ollie->GetPressedTime());
					observe->Update();
				}
				else
				{
					observe->Leave();
					delete observe;
					observe = NULL;
				}
			}
				if (LevelModSettings::grafCounter && Modulating()) [[unlikely]]
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
					if (downloading || showmessage) [[unlikely]]
					{
						if (showmessage)
						  showmessage--;
						m_font->DrawText(NULL, download_message, -1, &rct, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 255));
					}
						if (false)
						{
							pDevice->BeginScene();

							DWORD  D3DCMP = 0, pShared = 0, vShared = 0, Cull = 0, StencileEnable = 0, Clipping = 0, Clipplane = 0, COLOROP1 = 0, COLORARG1 = 0, COLORARG2 = 0, COLOROP2 = 0;
							UINT oldStride = 0;

							DWORD oldRenderState;
							pDevice->GetRenderState(D3DRS_ZENABLE, &oldRenderState);
							pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
							pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);
							//pDevice->GetPixelShader(&pShared);
							pDevice->SetPixelShader(NULL);
							pDevice->SetVertexShader(NULL);
							pDevice->SetTexture(0, NULL);
							pDevice->SetRenderState(D3DRS_ZWRITEENABLE, 0);

							/* IDirect3DVertexBuffer8* oldBuffer = NULL;
							 IDirect3DBaseTexture8* oldTexture1, * oldTexture2 = NULL;
							 pDevice->GetStreamSource(0, &oldBuffer, &oldStride);
							 pDevice->GetPixelShader(&pShared);
							 pDevice->GetVertexShader(&vShared);*/
							pDevice->GetRenderState(D3DRS_CULLMODE, &Cull);
							pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
							pDevice->SetRenderState(D3DRS_CLIPPING, 0);
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
							pDevice->EndScene();

							pDevice->SetRenderState(D3DRS_ZENABLE, oldRenderState);
							//pDevice->SetPixelShader(pShared);
							/*pDevice->SetTexture(0, oldTexture1);
							pDevice->SetTexture(1, oldTexture2);
							pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
							//m_pIDirect3DDevice8->SetRenderState( D3DRS_ZENABLE, TRUE);
							pDevice->SetRenderState(D3DRS_STENCILENABLE, StencileEnable);
							pDevice->SetRenderState(D3DRS_CLIPPING, Clipping);
							pDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, Clipplane);*/


							pDevice->SetRenderState(D3DRS_CULLMODE, Cull);
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
					return;
}

