#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Settings.h"
#include "KeyState.h"
#include "IniReader.h"
#include "IniWriter.h"
#include "Node.h"
#include "String.h"

#define NO_EXTRA_INCLUDE
#include "Extension.h"
#include "Extension.h"

using namespace LevelModSettings;


extern bool init3, debugMode, hooked;

extern CIniWriter* OptionWriter;
extern CIniReader* OptionReader;

extern BYTE oldCustomPrint[6];
extern BYTE oldPrint[6];

extern void SetTagLimit(DWORD _limit);

extern void __cdecl add_log(const char* string, ...);

#undef CreateConsole
extern BOOL CreateConsole();

bool LevelModSettings::UseNewMenu = true;
bool LevelModSettings::AA = false;
WORD LevelModSettings::AllowNewTricks = LevelModSettings::ALLOW_NONE;
bool LevelModSettings::UnlimitedGraf = false;
bool LevelModSettings::FixSound = true;
bool LevelModSettings::TeleFix = true;
bool LevelModSettings::grafCounter = true;
DWORD LevelModSettings::MemorySize = 0xFA000;
bool LevelModSettings::HookedControls = false;
std::map<DWORD, Option> LevelModSettings::options;
std::map<DWORD, OverrideOption> LevelModSettings::overrideOptions;


int LevelModSettings::SpineButton = 7;
int LevelModSettings::SpineButton2 = -1;

void LevelModSettings::ToggleNewMenu()
{
    const static DWORD Addr = 0x004404CE;
    UseNewMenu = !UseNewMenu;

    if (UseNewMenu)
        memcpy((void*)Addr, &NewMenu, sizeof(NewMenu));
    else
        memcpy((void*)Addr, &OldMenu, sizeof(OldMenu));
}

void LevelModSettings::ToggleSoundFix()
{
    const static DWORD Addr = 0x004C665D;
    FixSound = !FixSound;

    if (FixSound)
        *(BYTE*)Addr = 0xEB;
    else
        *(BYTE*)Addr = 0x75;
}

void LevelModSettings::FixTele()
{
    const static DWORD Addr = 0x004AE562;
    const static DWORD Addr2 = 0x004AE581;
    DWORD old;
    if (VirtualProtect((LPVOID)Addr, 4, PAGE_EXECUTE_READWRITE, &old))
    {
        *(DWORD*)Addr = 0x90909090;//84 c0 75 26
        if (VirtualProtect((LPVOID)Addr2, 1, PAGE_EXECUTE_READWRITE, &old))
            *(BYTE*)Addr2 = 0x75;//74
        else
            MessageBox(0, "couldn't fix protection for", "TeleFix", 0);
    }
    else
        MessageBox(0, "couldn't fix protection for", "TeleFix", 0);
}

struct TrickSpeed
{
    DWORD trickName;
    float originalSpeed;
    float th4Speed;
    float fastAir;

    TrickSpeed(DWORD name, float speed, float speed2, float speed3)
    {
        trickName = name;
        originalSpeed = speed;
        th4Speed = speed2;
        fastAir = speed3;
    }
};


void SendHostOptionChanged(int option, int value)
{
    using namespace Network;
    SendMessageToClients(MSG_ID_LM_HOSTOPTION_CHANGED, 8, &option);
}


TrickSpeed trickSpeed[] = {
    {TrickSpeed(Checksums::Trick_The900,  1.3f, 1.4f, 1.3f)},
    {TrickSpeed(Checksums::Trick_KFBackflip, 1.0f, 1.3f, 1.2f) },
    {TrickSpeed(Checksums::Trick_StaleFrontFlip, 1.1f, 1.3f, 1.2f) },
    {TrickSpeed(Checksums::Trick_StaleBackFlip, 1.1f, 1.3f, 1.2f) },
    {TrickSpeed(Checksums::Trick_MistyFlip, 1.2f, 1.45625f, 1.2f) },
    {TrickSpeed(Checksums::Trick_FS540HeelFlip, 1.2f, 1.3f, 1.45625f) },
    {TrickSpeed(Checksums::Trick_FS540, 1.0f, 1.3f, 1.425f) },
    {TrickSpeed(Checksums::Trick_McTwist, 1.0f, 1.1f, 1.2f) },
    {TrickSpeed(Checksums::Trick_Layout, 1.0f, 1.3f, 1.0f) },
    {TrickSpeed(Checksums::Trick_SackTap, 1.1f, 1.5f, 0.0f) },
    {TrickSpeed(Checksums::Trick_2KickMadonnaFlip, 1.15f, 1.3f, 1.15f) },
    {TrickSpeed(Checksums::Trick_AirCasperFlip, 1.1f, 1.3f, 1.1f) },
    {TrickSpeed(Checksums::Trick_PizzaGuy, 1.0f, 1.0f, 1.0f) },
    {TrickSpeed(Checksums::Trick_540TailWhip, 1.0f, 1.2f, 1.2f) },
    {TrickSpeed(Checksums::Trick_GhettoBird, 1.0f, 1.3f, 1.2f) },
    {TrickSpeed(Checksums::Trick_Berzerker, 1.1f, 1.3f, 1.1f) },
    {TrickSpeed(Checksums::Trick_FingerFlipAirWalk, 1.15f, 1.3f, 1.15f) },
    {TrickSpeed(Checksums::Trick_FingerFlip, 1.0f, 1.3f, 1.0f) },
    {TrickSpeed(Checksums::Trick_Jackass, 1.0f, 1.2f, 1.0f) },
    {TrickSpeed(Checksums::Trick_1234, 1.25f, 1.1f, 1.25f) },
    {TrickSpeed(Checksums::Trick_540Flip, 1.1f, 1.2f, 1.2f) },
    {TrickSpeed(Checksums::Trick_BetweenLegsSlam, 1.0f, 1.3f, 1.2f) },
    {TrickSpeed(Checksums::Trick_NollieFlipUnderflip, 1.0f, 1.3f, 1.2f) },
    {TrickSpeed(Checksums::Trick_HardFlipBackFootFlip, 1.0f, 1.3f, 1.2f) },
    {TrickSpeed(Checksums::Trick_KickFlipUnderFlip, 1.0f, 1.3f, 1.2f) },
    {TrickSpeed(Checksums::Trick_360Hardflip, 1.1f, 1.3f, 1.45625f) },
    {TrickSpeed(Checksums::Trick_QuadrupleHeelFlip, 1.0f, 1.3f, 1.2f) },
    {TrickSpeed(Checksums::Trick_OllieAirwalk, 1.0f, 1.3f, 1.0f) },
    {TrickSpeed(Checksums::CSOllieairwalkshoveit, 1.0f, 1.3f, 1.2f) },
    {TrickSpeed(Checksums::CSOllienorthbackfootflip, 1.0f, 1.3f, 1.2f) },
    {TrickSpeed(Checksums::Trick_Trick_FFImpossible, 1.0f, 1.3f, 1.2f) },
    {TrickSpeed(Checksums::CSdoublefrontfootimpossible, 1.45625f, 1.8f, 1.6f) },
    {TrickSpeed(Checksums::CSDoubleFingerFlip, 1.0f, 1.3f, 1.2f) },
    {TrickSpeed(Checksums::Trick_360Varial, 1.0f, 1.3f, 1.0f) },
    {TrickSpeed(Checksums::Trick_SalFlip, 1.0f, 1.3f, 1.0f) },
    {TrickSpeed(Checksums::Trick_180Varial, 1.0f, 1.3f, 1.0f) },
    {TrickSpeed(Checksums::Trick_OneFootJapan, 1.0f, 1.3f, 1.0f) },
    {TrickSpeed(Checksums::Trick_Cannonball, 0.75f, 0.75f, 0.75f) },
    {TrickSpeed(Checksums::CSFingerFlipCannonball, 1.1f, 1.4f, 1.3f) },
    {TrickSpeed(Checksums::Trick_Method, 1.0f, 1.2f, 1.0f)},
    {TrickSpeed(Checksums::Trick_Judo, 1.15f, 1.25f, 1.15f)},





};

bool IsOptionOn(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* header = pStruct->head;

    while (header)
    {
        if (header->Type == QBKeyHeader::LOCAL)
        {
            auto it = options.find(header->Data);
            if (it != options.end())
            {
                if (it->second.override == 0)
                {
                    //Option is not overriden, so return original value   
                    return it->second.value;
                }
                else
                {
                    auto override = it->second.override;

                    if ((override->type == OverrideOption::Type::OVERRIDE_TRUE && override->value == 0) //Only true is overriden
                        || (override->type == OverrideOption::Type::OVERRIDE_FALSE && override->value != 0)) //Only false is overriden
                    {
                        return it->second.value;
                    }
                    else
                    {
                        return override->value;
                    }

                }
            }
            else
            {
                //Should not happen
                _printf(__FUNCTION__ " Couldn't find option %s\nRemember to add the option the the list\nCheck settings.q for more info\n", header->Data);
            }

        }
        header = header->NextHeader;
    }
    _printf(__FUNCTION__" No Param?\n");
    return false;
}

bool IsOptionOff(CStruct* pStruct, CScript* pScript)
{
    return !IsOptionOn(pStruct, pScript);
}

void SetAirTrickSpeed(DWORD speed)
{
    _printf("Setting AirTrickSpeed to %d\n", speed);
    for (int i = 0; i < sizeof(trickSpeed) / sizeof(TrickSpeed); i++)
    {
        QBKeyHeader* header = GetQBKeyHeader(trickSpeed[i].trickName);
        if (header)
        {
            switch (speed)
            {
            case 0:
                if (!header->SetFloat(Checksums::Speed, trickSpeed[i].originalSpeed))
                    _printf("Couldn't find variable speed in struct %s\n", FindChecksumName(trickSpeed[i].trickName));
                break;
            case 1:
                if (!header->SetFloat(Checksums::Speed, trickSpeed[i].th4Speed))
                    _printf("Couldn't find variable speed in struct %s\n", FindChecksumName(trickSpeed[i].trickName));
                break;
            case 2:
                if (!header->SetFloat(Checksums::Speed, trickSpeed[i].fastAir))
                    _printf("Couldn't find variable speed in struct %s\n", FindChecksumName(trickSpeed[i].trickName));
                break;
            case 3:
            case 4:
                if (!header->SetFloat(Checksums::Speed, trickSpeed[i].originalSpeed * ((speed - 2) * 0.1f + 1.0f)))
                    _printf("Couldn't find variable speed in struct %s\n", FindChecksumName(trickSpeed[i].trickName));
                break;

            default:
                _printf("Wrong value in option LM_Control_AirTrickSpeed\nValue should be between 0-4\n");
                break;
            }
        }
        else
            _printf("Couldn't find trick %s[%d]\n", FindChecksumName(trickSpeed[i].trickName), i);
    }
}


void UpdateOption(DWORD checksum, int value, bool HostOption)
{
    DWORD old = 0;//used for VirtualProtect
    _printf("Updating Option %s %d\n", FindChecksumName(checksum), value);

    if (!HostOption)
    {
        auto it = options.find(checksum);
        if (it->second.override)
        {
            OverrideOption* override = it->second.override;
 
            if (override->type == OverrideOption::Type::OVERRIDE //Override both true and false
                || (value && override->type == OverrideOption::Type::OVERRIDE_TRUE) //Override true
                || (!value && override->type == OverrideOption::Type::OVERRIDE_FALSE)) //Override false
            {
                if (!value == override->value)
                    return;
                value = override->value;
            }
        }
    }

    switch (checksum)
    {
    case Checksums::LM_GameOption_b251Patch:
        if (value)
            *(BYTE*)0x004355AD = 0xEB;
        else
            *(BYTE*)0x004355AD = 0x7E;
        break;
    case Checksums::LM_Control_AirTrickSpeed:
        SetAirTrickSpeed(value);
        break;
    case Checksums::LM_DebugOption_bDebugMode:
        if (init3)//this means we have already added all options, so we have to alert about restart
        {
            int result = MessageBox(0, "Do you want to exit now?", "This option requires restart", MB_YESNO);
            if (result == IDYES)
                ExitProcess(0);
            else
                return;
        }
        if (debugMode || !value)//If we are already debugMode or value is false just return
            return;

        CreateConsole();
        _printf("Welcome to DebugMode\n");

        //Check if qbTable exist, if it doesn't exists
        //new Script() will generate qbTable from all .qb files found in qdir
        //If it does already exists it means it's another function has generated a table
        //Then just append the qbTables found .qb to the already existing qbTable
        if (!QScript::Scripts)
            QScript::Scripts = new QScript::QBScript();
        else
            QScript::Scripts->AddScripts();

        //We don't want to hook this twice...
        //Probably don't need to check since we should not be here twice
        //But added just in case
        if (!hooked)
        {
            hooked = true;
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
            //logFile = fopen("loggers.txt", "w+t");
        }

        //Finally tell whole engine we are in debugMode
        debugMode = true;
        break;

    case Checksums::LM_GameOption_bLimitTags:
        if (value)
            SetTagLimit(200);
        else
            SetTagLimit(32);
        LevelModSettings::UnlimitedGraf = !value;
        break;


    case Checksums::LM_GUI_bShowGrafCounter:
        LevelModSettings::grafCounter = value;
        break;

    case Checksums::LM_GUI_bNewMenu:
        VirtualProtect((LPVOID)0x004404CE, sizeof(LevelModSettings::NewMenu), PAGE_EXECUTE_READWRITE, &old);
        if (value)
            memcpy((void*)0x004404CE, &LevelModSettings::NewMenu, sizeof(LevelModSettings::NewMenu));
        else
            memcpy((void*)0x004404CE, &LevelModSettings::OldMenu, sizeof(LevelModSettings::OldMenu));
        break;

    case Checksums::LM_BugFix_bSoundFix:
        LevelModSettings::FixSound = value;

        VirtualProtect((LPVOID)0x004C665D, 1, PAGE_EXECUTE_READWRITE, &old);
        if (LevelModSettings::FixSound)
            *(BYTE*)0x004C665D = 0xEB;
        else
            *(BYTE*)0x004C665D = 0x75;
        break;

    case Checksums::LM_BugFix_bTeleFix:

        LevelModSettings::TeleFix = value;

        VirtualProtect((LPVOID)0x004AE562, 4, PAGE_EXECUTE_READWRITE, &old);
        VirtualProtect((LPVOID)0x004AE581, 1, PAGE_EXECUTE_READWRITE, &old);

        if (LevelModSettings::TeleFix)
        {
            *(DWORD*)0x004AE562 = 0x90909090;//84 c0 75 26
            *(BYTE*)0x004AE581 = 0x75;//74
        }
        else
        {
            *(DWORD*)0x004AE562 = 0x2675C084;
            *(BYTE*)0x004AE581 = 0x74;
        }
        break;
    case Checksums::LM_Control_bWallplant:
        if (value)
            LevelModSettings::AllowNewTricks |= LevelModSettings::ALLOW_WALLPLANT;
        else
            LevelModSettings::AllowNewTricks &= ~LevelModSettings::ALLOW_WALLPLANT;
        break;
    case Checksums::LM_Control_bSpine:
        if (value)
            LevelModSettings::AllowNewTricks |= LevelModSettings::ALLOW_SPINE;
        else
            LevelModSettings::AllowNewTricks &= ~LevelModSettings::ALLOW_SPINE;
        break;
    case Checksums::LM_Control_bAcid:
        if (value)
            LevelModSettings::AllowNewTricks |= LevelModSettings::ALLOW_ACID;
        else
            LevelModSettings::AllowNewTricks &= ~LevelModSettings::ALLOW_ACID;
        break;
    case Checksums::LM_Control_bBank:
        if (value)
            LevelModSettings::AllowNewTricks |= LevelModSettings::ALLOW_BANK;
        else
            LevelModSettings::AllowNewTricks &= ~LevelModSettings::ALLOW_BANK;
        break;
        /*case Checksums::LM_Control_bNewTricks:
            LevelModSettings::AllowNewTricks = value;
            break;*/

    case Checksums::LM_Control_bXinput:
        if (init3)//this means we have already added all options, so we have to alert about restart
        {
            int result = MessageBox(0, "Do you want to exit now?", "This option requires restart", MB_YESNO);
            if (result == IDYES)
                ExitProcess(0);
            else
                return;
        }
        if (LevelModSettings::HookedControls || !value)
            return;
        HookControls();
        LevelModSettings::HookedControls = true;
        break;


    case Checksums::LM_Control_SpineButton:
        switch (value)
        {
        case 0:
            LevelModSettings::SpineButton = KeyState::REVERT;
            LevelModSettings::SpineButton2 = KeyState::NONE;
            break;
        case 1:
            LevelModSettings::SpineButton = KeyState::NOLLIE;
            LevelModSettings::SpineButton2 = KeyState::NONE;
            break;
        case 2:
            LevelModSettings::SpineButton = KeyState::SPINLEFT;
            LevelModSettings::SpineButton2 = KeyState::NONE;
            break;
        case 3:
            LevelModSettings::SpineButton = KeyState::SPINRIGHT;
            LevelModSettings::SpineButton2 = KeyState::NONE;
            break;
        case 4:
            LevelModSettings::SpineButton = KeyState::REVERT;
            LevelModSettings::SpineButton2 = KeyState::NOLLIE;
            break;
        case 5:
            LevelModSettings::SpineButton = KeyState::SPINLEFT;
            LevelModSettings::SpineButton2 = KeyState::SPINRIGHT;
            break;
        default:
            LevelModSettings::SpineButton = KeyState::REVERT;
            LevelModSettings::SpineButton2 = KeyState::NONE;
            _printf("Invalid SpineButton %d defaulting to revert\nPlease check LevelMod.ini\nValue should be between 0-5\n", value);
            break;
        }
        break;
    }
}


int AddOption(char* name, int value, bool update, DWORD HostOption)
{
    DWORD checksum = crc32f(name);
    if (update)
    {
        _printf("Updating ini file %s value %d\n", name, value);
        OptionWriter->WriteInt("Script_Settings", name, value);
    }
    else
    {
        if (!OptionWriter->find("Script_Settings", name))
            OptionWriter->WriteInt("Script_Settings", name, value);
        else
        {
            _printf("Reading from ini file %s, default %d ", name, value);
            value = OptionReader->ReadInt("Script_Settings", name, value);
            _printf("value %d\n", value);
        }
        if (options.find(checksum) == options.end())
            options.insert(std::pair<DWORD, Option>(checksum, Option(value)));
        else
            MessageBox(0, "Added an option that already exists...", "Check settings.qb you dummy!!", 0);

        if (!QScript::Scripts)
            QScript::Scripts = new QScript::QBScript(false);

        //_printf("Adding to QBTable %s\n");
        /*char* tempName = new char[strlen(name) + 1];
        memcpy(tempName, name, strlen(name) + 1);*/

        QScript::Scripts->qbTable.insert(std::pair<int, char*>(checksum, String::AddString(name)));
        //MessageBox(0, FindChecksumName(checksum), "", 0);

    }
    if(!HostOption)
        UpdateOption(checksum, value);
    else
    {
        if (GameState::GotSuperSectors)
        {
            if (Network::OnServer())
                SendHostOptionChanged(checksum, value);

            auto it = options.find(HostOption);

            if (it->second.value != value)
            {
                auto override = it->second.override;

                if (override->type == OverrideOption::Type::OVERRIDE
                    || override->type == OverrideOption::Type::OVERRIDE_TRUE && override->value != 0
                    || override->type == OverrideOption::Type::OVERRIDE_FALSE && override->value == 0)
                {
                    UpdateOption(HostOption, value, true);
                }
            }
        }
    }
    return value;
}


bool GetParamScript(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* header = pStruct->head;
    while (header)
    {
        if (header->Type == QBKeyHeader::LOCAL)
        {
            _printf("Searching for: %s(%X)\n", FindChecksumName(header->Data), header->Data);
            CStructHeader* param = pScript->GetParam(header->Data);
            if (param)
            {
                CStructHeader* pParam = pScript->params->AllocateCStruct();
                if (!pParam)
                {
                    _printf(__FUNCTION__ "couldn't Allocate CStruct...\n");
                    return false;
                }

                if (pScript->params->head)
                {
                    pScript->params->tail->NextHeader = pParam;
                    pScript->params->tail = pParam;
                }
                else
                {
                    pScript->params->head = pParam;
                    pScript->params->tail = pParam;
                }
                pParam->Type = param->Type;
                pParam->QBkey = param->QBkey;
                pParam->Data = param->Data;
                pParam->NextHeader = NULL;
                /*if (param->Type == QBKeyHeader::STRING)
                {
                    _printf("Removing old string...\n%s\n", param->pStr);
                    param->Data = 0;
                    param->QBkey = 0;
                    param->Type = QBKeyHeader::LOCAL;
                    pParam->NextHeader = NULL;
                }*/
                _printf("Found it!!\nAdding Param to pScript...\nName %s(%X) Data %s(%X) Type %s\n", FindChecksumName(param->QBkey), param->QBkey, param->Type == QBKeyHeader::STRING ? param->pStr : FindChecksumName(param->Data), param->Data, QScript::QBTypes[param->Type]);
                param->Data = 0;
                param->QBkey = 0;
                param->Type = QBKeyHeader::LOCAL;
                return true;
            }
            return false;
        }
        header = header->NextHeader;
    }
    return false;
}

bool LM_GotParamScript(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* header = pStruct->head;
    while (header)
    {
        if (header->Type == QBKeyHeader::LOCAL)
        {
            bool b = pScript->GotParam(header->Data);
            if (b)
            {
                _printf("LM_GotParam returning true\n");
                return true;
            }
            else
            {
                _printf("LM_GotParam returning false\n");
                return false;
            }
        }
        header = header->NextHeader;
    }
    _printf("couldn't find param, LM_GotParam returning false\n");
    return false;
}

bool SetOption(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* header = pStruct->head;
    while (header)
    {
        if (header->Type == QBKeyHeader::LOCAL)
        {
            auto it = options.find(header->Data);
            if (it != options.end())
            {
                CStructHeader* value = NULL;
                if (pStruct->GetStruct(Checksum("value"), &value))
                {
                    char* ok = FindChecksumName(header->Data);
                    it->second = value->value.i;
                    _printf("Setting option %s to %d\n", ok, header->Data, it->second);
                    AddOption(ok, it->second.value, true);
                    return true;
                }
                else
                {
                    char* ok = FindChecksumName(header->Data);
                    static char tempChar[MAX_PATH + 1] = "";
                    static char id[MAX_PATH + 1] = "";
                    sprintf(id, "%s_id", ok);
                    memcpy(tempChar, ok, strlen(ok) + 1);
                    DWORD value = GetElementSliderValue(crc32f(id));
                    if (value != 0xFFFFFFFF)
                    {
                        it->second = value;//header->NextHeader->value.i;
                        _printf("Setting option %s to %d\n", ok, header->Data, it->second);
                        AddOption(ok, it->second.value, true);
                        return true;
                    }
                    else
                    {
                        _printf("coulnd't get value for %s\n", id);
                        return false;
                    }
                }
            }
            else
                _printf("couldn't find option %s\nMake Sure to add the option first\n", FindChecksumName(header->QBkey));
        }
        header = header->NextHeader;
    }
    _printf(__FUNCTION__ " No param?\n");
    return false;
}


bool ToggleHostOption(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* header = pStruct->head;
    while (header)
    {
        if (header->Type == QBKeyHeader::LOCAL)
        {
            auto override = overrideOptions.find(header->Data);
            if (override != overrideOptions.end())
            {
                override->second.value = !override->second.value;
                char* ok = FindChecksumName(header->Data);
                static char tempChar[MAX_PATH + 1] = "";
                memcpy(tempChar, ok, strlen(ok) + 1);
                _printf("Toggling HostOption %s(%X)\n", tempChar, header->Data);
                AddOption(tempChar, override->second.value, true, override->second.option);
                return true;
            }
            else
                _printf("couldn't find option %s\nMake Sure to add the option first\n", FindChecksumName(header->Data));
        }
        header = header->NextHeader;
    }
    _printf(__FUNCTION__ " No param?\n");
    return false;
}



bool ToggleOption(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* header = pStruct->head;
    while (header)
    {
        if (header->Type == QBKeyHeader::LOCAL)
        {
            auto it = options.find(header->Data);
            if (it != options.end())
            {
                it->second.value = !it->second.value;
                char* ok = FindChecksumName(header->Data);
                static char tempChar[MAX_PATH + 1] = "";
                memcpy(tempChar, ok, strlen(ok) + 1);
                _printf("Toggling option %s(%X)\n", tempChar, header->Data);
                AddOption(tempChar, it->second.value, true);
                return true;
            }
            else
                _printf("couldn't find option %s\nMake Sure to add the option first\n", FindChecksumName(header->Data));
        }
        header = header->NextHeader;
    }
    _printf(__FUNCTION__ " No param?\n");
    return false;
}

bool GetOptionText(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* option, * text = NULL;

    if (pStruct->GetStruct(Checksums::option, &option))
    {
        if (pStruct->GetStruct(Checksums::text, &text))
        {
            CArray* pArray = GetQBKeyHeader(text->Data)->pArray;

            auto it = options.find(option->Data);
            if (it != options.end())
            {
                char* pText = pArray->GetString(it->second.value);
                _printf("Got Option Text %p ", pText);
                _printf("%s\n", pText);
                //MessageBox(0, pText, pText, 0);
                CStructHeader* pParam = pScript->params->AllocateCStruct();
                if (!pParam)
                {
                    _printf(__FUNCTION__ "couldn't Allocate CStruct...\n");
                    return false;
                }

                if (pScript->params->head)
                {
                    pScript->params->tail->NextHeader = pParam;
                    pScript->params->tail = pParam;
                }
                else
                {
                    pScript->params->head = pParam;
                    pScript->params->tail = pParam;
                }
                _printf("Going to Add String\n");
                pParam->Type = QBKeyHeader::STRING;
                pParam->QBkey = Checksums::text;
                _printf("Going to MALLOCX\n");
                pParam->pStr = (char*)mallocx(strlen(pText) + 1);
                memcpy(pParam->pStr, pText, strlen(pText) + 1);
                pParam->NextHeader = NULL;
                _printf(__FUNCTION__ " Returning true\n");
                return true;
            }
            else
                _printf("Couldn't find option %s\nRemember to add the option first, check settings.qb\n");
        }
        else
            _printf("Need param text in function " __FUNCTION__ "\n");
    }
    else
        _printf("Need param text in function " __FUNCTION__ "\n");
    return false;
}

bool AddOption(CStruct* pStruct, CScript* pScript)
{
    if (init3)
        return false;

    CStructHeader* name = NULL;

    if (pStruct->GetStruct(Checksums::Name, &name))
    {
        if (name->Type == QBKeyHeader::STRING || name->Type == QBKeyHeader::LOCAL_STRING)
        {
            if (options.find(crc32f(name->pStr)) == options.end())
            {
                CStructHeader* DEFAULT = NULL;
                if (pStruct->GetStruct(Checksums::Value, &DEFAULT))
                {
                    _printf("Adding option %s default %d\n", name->pStr, DEFAULT->value.i);
                    AddOption(name->pStr, DEFAULT->value.i);
                }
                else
                    _printf("Need Param #DEFAULT " __FUNCTION__ "\n");
            }
            else
                _printf("Option already in list..\n");
        }
        else
            _printf("Param #Name needs to be a string " __FUNCTION__ "\n");
    }
    else
        _printf("Need param #Name " __FUNCTION__ "\n");

    return false;
}