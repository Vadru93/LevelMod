#pragma once
#ifndef _KeyState_H_
#define _KeyState_H_
#undef NO_DEFINES
#include "Defines.h"
//#include "CXBOXController.h"
struct Skater;

EXTERN char* FindChecksumName(DWORD checksum);
struct KeyState
{
private:
    DWORD pressed;
    DWORD timepress;
    DWORD timerelease;
    DWORD holding;//0xFF
    DWORD unk1[2];//1
    DWORD checksum;

public:
    enum
    {
        NONE = -1, UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, SPINLEFT = 4, NOLLIE = 5, SPINRIGHT = 6, REVERT = 7, GRAB = 8, FLIP = 9, GRIND = 10, OLLIE = 11
    };

    DWORD GetChecksum()
    {
        return checksum;
    }

    //original game function
    void Update(DWORD press)
    {
        typedef void(__thiscall* const pUpdate)(KeyState* pThis, DWORD press);
        pUpdate(0x0049BAA0)(this, press);
    }


    //the press is between 0x0-0xFF, press below or equal to 0x40 is deadzone
    void Update(DWORD time, DWORD press)
    {
        _printf("press %X\nKeyState %p %s(%X)\n", press, this, FindChecksumName(this->checksum), this->checksum);
        //the press is between 0x0-0xFF, press below or equal to 0x40 is deadzone
        if (press > 0x40)
        {
            pressed = 1;
            timepress = time;
            holding = press;
        }
        else
        {
            pressed = 0;
            timerelease = time;
            holding = press;
        }

    }


    DWORD GetReleasedTime()
    {
        return timerelease;
    }

    DWORD GetPressedTime()
    {
        return timepress;
    }

    bool IsPressed()
    {
        return pressed != 0;
    }

};

#endif