#pragma once
#ifndef _KeyState_H_
#define _KeyState_H_
#undef NO_DEFINES
#include "Defines.h"
//#include "CXBOXController.h"

//0x005D0300 How many keys were pressed`?
//0x005d030c keys[256]

enum class KeyCode : BYTE
{
    UNDEFINED = 0x0,
    ENTER = 0x19,
};

enum class VirtualKeyCode : BYTE
{
    L_MOUSE = 0x1,
    R_MOUSE,
    CANCEL,
    M_MOUSE,
    U_MOUSE,
    D_MOUSE,
    UNDEFINED,
    BACK,
    TAB,
    RESERVED,
    RESERVED2,
    CLEAR,
    ENTER,
    UNDEFINED2,
    UNDEFINED3,
    SHIFT,
    CNTRL,
    ALT,
    PAUSE,
    CAPS,
    KANA,
    HANGUEL,
    HANGAL,
    IME_ON,
    KUNJA,
    FINAL,
    HANJA,
    KANJI,
    IME_OFF,
    ESC,
    CONVERT,
    NONCONVERT,
    ACCEPT,
    MODECHANGE,
    SPACE,
    PAGE_UP,
    PAGE_DOWN,
    END,
    HOME,
    LEFT,
    UP,
    RIGHT,
    DOWN,
    SELECT,
    PRINT,
    EXECUTE,
    PRNTSCRN,
    INSERT,
    DEL,
    HELP,
};

#define p_num_keys *(DWORD*)0x005D0300
#define p_key_state(idx) *(KeyCode*)(0x005d030c+idx)
struct Skater;

//EXTERN char* FindChecksumName(DWORD checksum, bool only_debug = true);
struct KeyState
{
private:
    DWORD pressed;
    DWORD timepress;
    DWORD timerelease;
    DWORD holding;//0xFF
    DWORD unk1[2];//1
    DWORD checksum;


    static KeyCode GetKeyPress(DWORD idx)
    {
        return p_key_state(idx);
    }

    static void Unpress(DWORD idx)
    {
        p_key_state(idx) = KeyCode::UNDEFINED;
    }


public:
    enum
    {
        NONE = -1, UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, SPINLEFT = 4, NOLLIE = 5, SPINRIGHT = 6, REVERT = 7, GRAB = 8, FLIP = 9, GRIND = 10, OLLIE = 11
    };

    static DWORD GetNumKeyPress()
    {
        return p_num_keys;
    }

    static bool IsPressed(KeyCode key)
    {
        DWORD numKeys = GetNumKeyPress();

        for (auto press = 0; press < numKeys; press++)
        {
            if (GetKeyPress(press) == key)
                return true;
        }
        return false;
    }

    static bool GetKeyboardState(VirtualKeyCode code)
    {
        return (*(BYTE**)(0x005D0308))[(BYTE)code] & 1;
    }

    static void Unpress(VirtualKeyCode code)
    {
        (*(BYTE**)(0x005D0308))[(BYTE)code] = 0;
    }

    static void Unpress(KeyCode key)
    {
        DWORD numKeys = GetNumKeyPress();

        for (auto press = 0; press < numKeys; press++)
        {
            if (GetKeyPress(press) == key)
                Unpress(press);
        }
    }

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