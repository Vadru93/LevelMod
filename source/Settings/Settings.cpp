#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Settings\Settings.h"
#include "Input\KeyState.h"
#include "Settings\IniReader.h"
#include "Settings\IniWriter.h"
#include "Script\Node.h"
#include "Memory\String.h"

#define NO_EXTRA_INCLUDE
#include "Extension\Extension.h"

using namespace LevelModSettings;


extern bool bAddedOptions, bDebugMode, bHooked;

extern CIniWriter* OptionWriter;
extern CIniReader* OptionReader;

extern BYTE oldCustomPrint[6];
extern BYTE oldPrint[6];

extern void SetTagLimit(DWORD _limit);

extern void __cdecl add_log(const char* string, ...);

#undef CreateConsole
extern BOOL CreateConsole();

CStructHeader* LevelModSettings::pMax = NULL;
bool LevelModSettings::bUseNewMenu = true;
bool LevelModSettings::bAA = false;
WORD LevelModSettings::AllowNewTricks = LevelModSettings::ALLOW_NONE;
bool LevelModSettings::bUnlimitedGraf = false;
bool LevelModSettings::bFixSound = true;
bool LevelModSettings::bTeleFix = true;
bool LevelModSettings::bGrafCounter = true;
DWORD LevelModSettings::MemorySize = 0xFA000;
bool LevelModSettings::bHookedControls = false;
BYTE LevelModSettings::original_timer[38];
std::map<DWORD, Option> LevelModSettings::options;
std::map<DWORD, OverrideOption> LevelModSettings::overrideOptions;


int LevelModSettings::SpineButton = 7;
int LevelModSettings::SpineButton2 = -1;
VirtualKeyCode LevelModSettings::SpineButton3 = VirtualKeyCode::Undefined;



struct TrickSpeed
{
    DWORD TrickName;
    float fOriginalSpeed;
    float fTh4Speed;
    float fFastAir;

    TrickSpeed(DWORD name, float speed, float speed2, float speed3) :
        TrickName(name), fOriginalSpeed(speed), fTh4Speed(speed2), fFastAir(speed3)
    {}
};


void __stdcall SendHostOptionChanged(int option, int value)
{
    using namespace Network;
    HostOptionMsg msg(option, value);
    SendMessageToClients(MSG_ID_LM_HOSTOPTION_CHANGED, 8, &msg);
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

bool IsOptionOverriden(CStruct* pStruct, CScript* pScript)
{
    CStructHeader* header = pStruct->head;

    while (header)
    {
        if (header->Type == QBKeyHeader::LOCAL)
        {
            auto it = options.find(header->Data);
            if (it != options.end())
            {
                if (it->second.pOverride == NULL)
                {
                    //Option is not linked to a HostOption, so return original value   
                    return false;
                }
                else
                {
                    CStructHeader* pOption;
                    if (!pScript->params->GetStruct(Checksum("option_id"), &pOption))
                    {
                        _printf("couldn't find option_id, going to auto-generate it...\n");
                        //Add the option_id
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
                        pParam->Type = QBKeyHeader::QBKeyType::LOCAL;
                        pParam->QBkey = Checksum("option_id");
                        char name[128] = "";
                        sprintf(name, "%s_id", FindChecksumName(header->Data, false));
                        _printf("Generated: %s\n", name);
                        pParam->Data = Checksum(name);
                        pParam->NextHeader = NULL;
                    }

                    return it->second.Overriden();

                }
            }
            else
            {
                //Should not happen
                _printf(__FUNCTION__ " Couldn't find option %s\nRemember to add the option the the list\nCheck settings.q for more info\n", FindChecksumName(header->Data));
            }

        }
        header = header->NextHeader;
    }
    _printf(__FUNCTION__" No Param? %s\n", FindChecksumName(pScript->scriptChecksum));
    return false;
}

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
                if (it->second.pOverride == NULL)
                {
                    //Option is not linked to a HostOption, so return original value   
                    return it->second.value;
                }
                else
                {

                    if (!it->second.Overriden())
                    {
                        return it->second.value;
                    }
                    else
                    {
                        return it->second.pOverride->value;
                    }

                }
            }
            else
            {
                //Should not happen
                _printf(__FUNCTION__ " Couldn't find option %s\nRemember to add the option the the list\nCheck settings.q for more info\n", FindChecksumName(header->Data));
            }

        }
        header = header->NextHeader;
    }
    _printf(__FUNCTION__" No Param? %s\n", FindChecksumName(pScript->scriptChecksum));
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
        QBKeyHeader* header = GetQBKeyHeader(trickSpeed[i].TrickName);
        if (header)
        {
            switch (speed)
            {
            case 0:
                if (!header->SetFloat(Checksums::Speed, trickSpeed[i].fOriginalSpeed))
                    _printf("Couldn't find variable speed in struct %s\n", FindChecksumName(trickSpeed[i].TrickName));
                break;
            case 1:
                if (!header->SetFloat(Checksums::Speed, trickSpeed[i].fTh4Speed))
                    _printf("Couldn't find variable speed in struct %s\n", FindChecksumName(trickSpeed[i].TrickName));
                break;
            case 2:
                if (!header->SetFloat(Checksums::Speed, trickSpeed[i].fFastAir))
                    _printf("Couldn't find variable speed in struct %s\n", FindChecksumName(trickSpeed[i].TrickName));
                break;
            case 3:
            case 4:
                if (!header->SetFloat(Checksums::Speed, trickSpeed[i].fOriginalSpeed * ((speed - 2) * 0.1f + 1.0f)))
                    _printf("Couldn't find variable speed in struct %s\n", FindChecksumName(trickSpeed[i].TrickName));
                break;

            default:
                _printf("Wrong value in option LM_Control_AirTrickSpeed\nValue should be between 0-4\n");
                break;
            }
        }
        else
            _printf("Couldn't find trick %s[%d]\n", FindChecksumName(trickSpeed[i].TrickName), i);
    }
}


void UpdateOption(DWORD checksum, int value)//, bool HostOption)
{
    DWORD old = 0;//used for VirtualProtect
    _printf("Updating Option %s %d\n", FindChecksumName(checksum), value);

    /*if (!HostOption)
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
    }*/

    switch (checksum)
    {
    case Checksums::LM_GameOption_bFixBWManual:
        if (value)
        {
            *(BYTE*)0x0049D7BE = 0x90;
            *(DWORD*)0x0049D7BF = 0x90909090;
            *(DWORD*)0x0049D7C3 = 0x90909090;
            *(DWORD*)0x0049D7C7 = 0x90909090;
            *(WORD*)0x0049D7CB = 0x9090;

            /**(DWORD*)0x0049D7C5 = 0x90909090;
            *(DWORD*)0x0049D7C9 = 0x90909090;*/
        }
        else
        {
            *(WORD*)0x0049D7BE = 0xCE8B;
            *(BYTE*)0x0049D7C0 = 0xE8;
            *(DWORD*)0x0049D7C1 = 0x0000016B;

            *(DWORD*)0x0049D7C5 = 0x850FC084;
            *(DWORD*)0x0049D7C9 = 0x00000156;
        }
        break;

    case Checksums::LM_GameOption_b251Patch:
        if (value)
            *(BYTE*)0x004355AD = 0xEB;
        else
            *(BYTE*)0x004355AD = 0x7E;
        break;
    case Checksums::LM_Control_AirTrickSpeed:
        SetAirTrickSpeed(value);
        break;
    case (DWORD)Checksums::LM_DebugOption_bDebugMode:
        //casting to DWORD to prevent compiler warning, code actually works without this cast
        //This could be prevented all together if use DWORD constexpr instead of enum for the compile-time checksums
        //However I think enum is still better since a compiler error will come if have unwanted collision in the list

        if (bAddedOptions)//this means we have already added all options, so we have to alert about restart
        {
            int result = MessageBox(0, "Do you want to exit now?", "This option requires restart", MB_YESNO);
            if (result == IDYES)
                ExitProcess(0);
            else
                return;
        }

        if (bDebugMode || !value)//If we are already debugMode or value is false just return
        {
            //If we are not in debugmode we need to add the checksums from settings.qb to be able to toggle options correctly
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
            return;
        }

        /*CreateConsole();
        _printf("Welcome to DebugMode\n");*/

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
        if (!bHooked)
        {
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
            //logFile = fopen("loggers.txt", "w+t");
        }

        //Finally tell whole engine we are in debugMode
        bDebugMode = true;
        break;

    case (DWORD)Checksums::LM_GameOption_bUnLimitTags://casting to DWORD to prevent compiler warning, code actually works without this cast
        if (value)
            SetTagLimit(200);
        else
            SetTagLimit(32);
        //LevelModSettings::bUnlimitedGraf = value;
        break;


    case Checksums::LM_GUI_bShowGrafCounter:
        LevelModSettings::bGrafCounter = value;
        break;

    case Checksums::LM_GUI_bNewMenu:
        VirtualProtect((LPVOID)0x004404CE, sizeof(LevelModSettings::NewMenu), PAGE_EXECUTE_READWRITE, &old);
        if (value)
            memcpy((void*)0x004404CE, &LevelModSettings::NewMenu, sizeof(LevelModSettings::NewMenu));
        else
            memcpy((void*)0x004404CE, &LevelModSettings::OldMenu, sizeof(LevelModSettings::OldMenu));
        break;

    case Checksums::LM_BugFix_bSoundFix:
        LevelModSettings::bFixSound = value;

        VirtualProtect((LPVOID)0x004C665D, 1, PAGE_EXECUTE_READWRITE, &old);
        if (LevelModSettings::bFixSound)
            *(BYTE*)0x004C665D = 0xEB;
        else
            *(BYTE*)0x004C665D = 0x75;
        break;

    case Checksums::LM_BugFix_bTeleFix:

        LevelModSettings::bTeleFix = value;

        VirtualProtect((LPVOID)0x004AE562, 4, PAGE_EXECUTE_READWRITE, &old);
        VirtualProtect((LPVOID)0x004AE581, 1, PAGE_EXECUTE_READWRITE, &old);

        if (LevelModSettings::bTeleFix)
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
    case (DWORD)Checksums::LM_Control_bWallplant:
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
    case (DWORD)Checksums::LM_Control_bAcid:
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
        if (bAddedOptions)//this means we have already added all options, so we have to alert about restart
        {
            int result = MessageBox(0, "Do you want to exit now?", "This option requires restart", MB_YESNO);
            if (result == IDYES)
                ExitProcess(0);
            else
                return;
        }
        if (LevelModSettings::bHookedControls || !value)
            return;
        HookControls();
        LevelModSettings::bHookedControls = true;
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

void MaybeUpdateOption(DWORD overriden_option, DWORD HostOption, DWORD value)
{
    auto option = GetOption(overriden_option);
    if (option)
    {
        //If the link between option and override option is not found we can now link it instead of crash app...
        if (!option->pOverride)
        {
            auto Override = overrideOptions.find(HostOption);
            if (Override != overrideOptions.end())
            {
                option->pOverride = &Override->second;
            }
            else
                MessageBox(0, "This is not good aswell", "HostOption badly linked...", 0);
        }

        _printf("Now we updating..\n");

        //If the override value equals the actual option value, or option is overriden we need to update the option(aka apply the option)
        if (value == option->value || option->Overriden())
            UpdateOption(overriden_option, value);
    }
    else
        MessageBox(0, "This is not good", "HostOption badly linked...", 0);
}


int AddOption(char* name, int value, bool update, DWORD overriden_option, BYTE type)
{
    DWORD checksum = crc32f(name);
    if (update)
    {
        _printf("Updating ini file %s value %d\n", name, value);
        OptionWriter->WriteInt("Script_Settings", name, value);
    }
    else
    {
        int maximum = pMax ? pMax->value.i : 2;

        if (!OptionWriter->find("Script_Settings", name))
            OptionWriter->WriteInt("Script_Settings", name, value);
        else
        {

            _printf("Reading from ini file %s, default %d ", name, value);
            int new_value = OptionReader->ReadInt("Script_Settings", name, value);

            if (new_value < maximum)
                value = new_value;
                
            else
                _printf("Too high value in ini file option %s\n Setting to default %d\n", name, value);

            _printf("value %d\n", value);
        }
        if (options.find(checksum) == options.end())
            options.insert(std::pair<DWORD, Option>(checksum, Option(value)));
        else
            MessageBox(0, "Added an option that already exists...", "Check settings.qb you dummy!!", 0);
        if (overriden_option)
        {
            if(overrideOptions.find(checksum) == overrideOptions.end())
                overrideOptions.insert(std::pair<DWORD, OverrideOption>(checksum, OverrideOption((OverrideOption::Type)type, value, overriden_option)));
            else
                MessageBox(0, "Added an HostOption that already exists...", "Check settings.qb you dummy!!", 0);
        }

        if (!QScript::Scripts)
        {
            QScript::Scripts = new QScript::QBScript(false);
        }

        //_printf("Adding to QBTable %s\n");
        /*char* tempName = new char[strlen(name) + 1];
        memcpy(tempName, name, strlen(name) + 1);*/

        if (QScript::Scripts->qbTable.find(checksum) == QScript::Scripts->qbTable.end())
            QScript::Scripts->qbTable.insert(std::pair<DWORD, char*>(checksum, String::AddString(name)));
        else
            _printf("This string already exists..\n");
        //MessageBox(0, FindChecksumName(checksum), "", 0);

    }
    if (overriden_option == 0)
    {
        UpdateOption(checksum, value);
    }
    else
    {
        if (GameState::GotSuperSectors)
        {
            _printf("Going to update HostOption %d\n", value);
            SendHostOptionChanged(checksum, value);
            _printf("Sent message\n");

            MaybeUpdateOption(overriden_option, checksum, value);
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
            _printf("Searching for: %s)\n", FindChecksumName(header->Data));
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

LevelModSettings::Option* GetOption(DWORD option)
{
    auto it = LevelModSettings::options.find(option);
    if (it != LevelModSettings::options.end())
        return &it->second;
    return NULL;
}

bool GetOptionValue(CStruct* pStruct, CScript* pScript)
{
    for(auto header = pStruct->head; header != NULL; header = header->NextHeader)
    {
        if (header->Type == QBKeyHeader::LOCAL)
        {
            auto it = options.find(header->Data);
            if (it != options.end())
            {
                auto param = pScript->params->AddParam("OptionValue", QBKeyHeader::QBKeyType::INT);
                param->Data = it->second.value;
                return true;
            }
        }
    }
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
                    char* ok = FindChecksumName(header->Data, false);
                    if (!it->second.pOverride || !it->second.Overriden())
                    {
                        it->second.value = value->value.i;
                        _printf("Setting option %s to %d\n", ok, it->second.value);
                        AddOption(ok, it->second.value, true);
                    }
                    else
                        _printf("Option %s is overriden\n", ok);
                    return true;
                }
                else
                {
                    char* ok = FindChecksumName(header->Data, false);
                    static char tempChar[MAX_PATH + 1] = "";
                    static char id[MAX_PATH + 1] = "";
                    sprintf(id, "%s_id", ok);
                    memcpy(tempChar, ok, strlen(ok) + 1);
                    DWORD value = GetElementSliderValue(crc32f(id));
                    if (value != 0xFFFFFFFF)
                    {
                        if (!it->second.pOverride || !it->second.Overriden())
                        {
                            it->second.value = value;//header->NextHeader->value.i;
                            _printf("Setting option %s to %d\n", ok, it->second);

                            AddOption(ok, it->second.value, true);
                        }
                        else
                            _printf("Option %s is overriden\n", ok);
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
                Option* option = GetOption(header->Data);
                if(option)
                    option->value = !option->value;
                else
                    _printf("couldn't find option %s\nMake Sure to add the option first\n", FindChecksumName(header->Data));
                override->second.value = !override->second.value;
                char* ok = FindChecksumName(header->Data, false);

                static char tempChar[MAX_PATH + 1] = "";
                memcpy(tempChar, ok, strlen(ok) + 1);
                _printf("Toggling HostOption %s %d\n", tempChar, override->second.value);

                AddOption(tempChar, override->second.value, true, override->second.option);

                return true;
            }
            else
                _printf("couldn't find HostOption %s\nMake Sure to add the option first\n", FindChecksumName(header->Data));
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
                char* ok = FindChecksumName(header->Data, false);
                if (!it->second.pOverride || !it->second.Overriden())
                {
                    static char tempChar[MAX_PATH + 1] = "";
                    memcpy(tempChar, ok, strlen(ok) + 1);
                    _printf("Toggling option %s\n", tempChar);
                    AddOption(ok, it->second.value, true);
                }
                else
                    _printf("Option %s is overriden\n", ok);
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
            CArray* pArray = NULL;
            if (text->Type == QBKeyHeader::QBKeyType::ARRAY)
                pArray = text->pArray;
            else
                pArray = GetQBKeyHeader(text->Data)->pArray;

            if (pArray)
            {
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
            }
            else
                _printf("Couldn't find option %s\nRemember to add the option first, check settings.qb\n", FindChecksumName(option->Data));
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
    if (bAddedOptions)
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
                    pStruct->GetStruct(crc32f("max"), &pMax);
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