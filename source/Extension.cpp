#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Extension.h"

//Set Element Text
inline void SetElementText(int id, char* text)
{
	static const DWORD pSetText = 0x004CE940;
	_asm push text
	_asm push id
	_asm call pSetText
}






//seems to be broken in Openspy...
void SendPm(char* who, const char* message)
{
	typedef void(__thiscall* const pSendPm)(void* pNetProfile, char* who, const char* message, DWORD unk);
	pSendPm(0x004E0F20)(*(void**)0x008F14F4, who, message, 0);
}



typedef Element* (__cdecl* const pAllocateElement)(DWORD unk);
static const pAllocateElement AllocateElement = (pAllocateElement)(0x004D12A0);
typedef void(__cdecl* const pFreeElement)();
static const pFreeElement FreeElement = (pFreeElement)(0x004D12F0);
typedef void* (__cdecl* const pCastPointer)(void* pointer, LONG VfDelta, DWORD SrcType, DWORD TargetType, BOOL isReference);
static const pCastPointer CastPointer = (pCastPointer)(0x00577E58);


DWORD GetElementSliderValue(DWORD name)
{
	_asm pushad;
	_asm pushfd;
	Element* container = AllocateElement(0);
	Element* element = container->GetElement(name);
	if (element)
	{
		element = (Element*)CastPointer((void*)element, 0, 0x005B6344, 0x005B6638, FALSE);
		DWORD value = element->GetValue();
		FreeElement();

		_asm popfd;
		_asm popad;
		return value;
	}
	FreeElement();

	_asm popfd;
	_asm popad;
	return 0xFFFFFFFF;
}


//LevelMod Example Script
//Get's the slider value of a menu element, check optionsmenu.qb
bool GetSliderValue(CStruct* pStruct, CScript* pScript)
{
	int name = 0;
	pStruct->GetScript("id", &name);
	int paramName = 0;

	if (pStruct->GetScript("name", &paramName))
	{
		CStructHeader* param = NULL;

		if (pScript->params->GetStruct(paramName, &param))
		{
			param->value.f = (float)GetElementSliderValue(name);
		}
		else
			return false;
	}
	else
	{
		CStructHeader* param = pScript->params->AddParam("value", QBKeyHeader::FLOAT);
		param->value.f = (float)GetElementSliderValue(name);
	}
	return true;
}

//LevelMod Example Script
//Change the values of a struct, check optionsmenu.qb
bool ChangeValues(CStruct* pStruct, CScript* pScript)
{
	int container = 0;

	if (pStruct->GetScript("Container", &container))
	{
		QBKeyHeader* pContainer = GetQBKeyHeader(container);
		if (pContainer)
		{
			CStructHeader* values = NULL;

			if (pStruct->GetStruct(crc32f("Values"), &values))
			{
				pContainer->SetValues(*values->pStruct);
				return true;
			}
		}
	}
	return false;
}