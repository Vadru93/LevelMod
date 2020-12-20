#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Extension.h"
#include "String.h"

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

void Element::SetSliderArrayText(const CArray* pArray)
{
    if (!pair)
    {
        pair = AllocatePairedText(pArray->GetNumItems());

        for (DWORD i = 0; i < pArray->GetNumItems(); i++)
        {
            pair[i].text = String::AddString(pArray->texts[i]);
            pair[i].value = i;
        }
    }
}



typedef Element* (__cdecl* const pAllocateElement)(DWORD unk);
static const pAllocateElement AllocateElement = (pAllocateElement)(0x004D12A0);
typedef void(__cdecl* const pFreeElement)();
static const pFreeElement FreeElement = (pFreeElement)(0x004D12F0);
typedef void* (__cdecl* const pCastPointer)(void* pointer, LONG VfDelta, DWORD SrcType, DWORD TargetType, BOOL isReference);
static const pCastPointer CastPointer = (pCastPointer)(*(DWORD*)0x0058D150);// 0x00577E58);


bool SetElementSliderText(DWORD id, const CArray* pArray)
{
    Element* container = AllocateElement(0);
    Element* element = container->GetElement(id);
    MessageBox(0, "ELEMENTE", "", 0);
    if (element)
    {
        //element = (Element*)CastPointer((void*)element, 0, 0x005B6344, 0x005B6638, FALSE);
        element->SetSliderArrayText(pArray);
        FreeElement();

        return true;
    }
    FreeElement();

    return false;
}

bool SetSliderValueScript(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* option = NULL;
    if (pStruct->GetStruct(Checksums::option, &option))
    {
        Element* container = AllocateElement(0);
        char* ok = FindChecksumName(option->Data, false);
        static char tempChar[MAX_PATH + 1] = "";
        static char id[MAX_PATH + 1] = "";
        sprintf(id, "%s_id", ok);
        memcpy(tempChar, ok, strlen(ok) + 1);
        Element* element = container->GetElement(crc32f(id));
        if (element)
        {
            //element = (Element*)CastPointer((void*)element, 0, 0x005B6344, 0x005B6638, FALSE);
            auto it = LevelModSettings::options.find(option->Data);
            if (it != LevelModSettings::options.end())
                element->value = it->second.value;
            FreeElement();

            return true;
        }
        FreeElement();

        return false;
    }
    return false;
}

DWORD GetElementSliderValue(DWORD name)
{
    Element* container = AllocateElement(0);
    Element* element = container->GetElement(name);
    if (element)
    {
        //element = (Element*)CastPointer((void*)element, 0, 0x005B6344, 0x005B6638, FALSE);
        DWORD value = element->GetValue();
        FreeElement();

        return value;
    }
    FreeElement();

    return 0xFFFFFFFF;
}


//LevelMod Example Script
//Gets the slider value of a menu element, check optionsmenu.qb
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

bool SetSliderArrayText(CStruct* pStruct, CScript* pScript)
{
    int id = 0;
    pStruct->GetScript("id", &id);
    const CArray* pText = NULL;

    pStruct->GetArray("enum", &pText);

    return SetElementSliderText(id, pText);
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