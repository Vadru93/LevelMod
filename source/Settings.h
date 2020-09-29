#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H
#undef NO_DEFINES
#include "defines.h"

namespace LevelModSettings
{
    static enum NewTrickOptions
    {
        ALLOW_NONE = 0, ALLOW_SPINE = 1, ALLOW_BANK = 2, ALLOW_ACID = 4, ALLOW_WALLPLANT = 8, ALL_SPINE = (ALLOW_SPINE | ALLOW_BANK | ALLOW_ACID)
    };
    static BYTE NewMenu[9] = { 0x90, 0x90, 0x90, 0x90, 0x74, 0x70, 0xEB, 0x7B, 0x90 };//000404ce
    static BYTE OldMenu[9] = { 0xC6, 0x46, 0x04, 0x01, 0x74, 0x70, 0x57, 0x6A, 0x00 };

    extern bool UseNewMenu;// = true;
    extern bool AA;// = false;
    extern WORD AllowNewTricks;// = ALLOW_NONE;
    extern bool UnlimitedGraf;// = false;
    extern bool FixSound;// = true;
    extern bool TeleFix;// = true;
    extern bool grafCounter;// = true;
    extern DWORD MemorySize;// = 0xFA000;
    extern bool HookedControls;// = false;


    extern int SpineButton;// = 7;
    extern int SpineButton2;// = -1;

    void ToggleNewMenu();


    void ToggleSoundFix();

    void FixTele();

    struct OverrideOption
    {
        enum class Type : unsigned char
        {
            OVERRIDE_TRUE,
            OVERRIDE_FALSE,
            OVERRIDE,
        };
        Type type;
        int value;
        DWORD option;

        OverrideOption(Type t, int v, DWORD o)
        {
            type = t;
            value = v;
            option = o;
        }

    };

    struct Option
    {   
        OverrideOption* override;
        int value;

        Option(int Value)
        {
            override = NULL;
            value = Value;
        }

        bool Overriden()
        {
            if ((override->type == OverrideOption::Type::OVERRIDE_TRUE && override->value != 0) //Only true is overriden
                || (override->type == OverrideOption::Type::OVERRIDE_FALSE && override->value == 0)) //Only false is overriden
                return false;
            else
                return true;
        }
    };

    extern std::map<DWORD, OverrideOption> overrideOptions;
    extern std::map<DWORD, Option> options;
};

void SetAirTrickSpeed(DWORD speed);

void UpdateOption(DWORD checksum, int value);//, bool HostOption = false);

int AddOption(char* name, int value, bool update = false, DWORD HostOption = 0,  BYTE type = 0);

void __stdcall SendHostOptionChanged(int option, int value);
#endif