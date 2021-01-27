#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Script\Script.h"
#include "Math\_Math.h"
#include "Game\Skater.h"
#include "Script\Checksum.h"
#include "dinput.h"

bool XINPUT::vibrating = false;
XINPUT_VIBRATION XINPUT::vibration;
DWORD XINPUT::vibrationFrames = 0;
CXBOXController* __restrict XINPUT::Player1 = NULL;
KeyMap* __restrict keyMap = NULL;
BYTE* __restrict keyState = NULL;


void  KeyMap::UpdateKeyMap()
{
    DWORD pKeyboardControl = *(DWORD*)0x008510B8;
    DWORD pKeyMap = *(DWORD*)(pKeyboardControl + 0x10);
    pKeyMap = *(DWORD*)(pKeyMap + 4);
    keyState = (BYTE*)(pKeyMap + 0x24);
    pKeyMap = *(DWORD*)(pKeyMap + 0xE8);

    keyMap = (KeyMap*)(pKeyMap + 0x80);
}

bool KeyMap::IsPressed(Button button)
{
    return keyState[(BYTE)button] > 0x40;
}

VirtualKeyCode KeyMap::GetVKeyCode(MappedKey key)
{
    return (VirtualKeyCode)MapVirtualKeyA((UINT)keyMap[(BYTE)key].DIK_KeyCode, MAPVK_VSC_TO_VK);
}


__declspec(noalias) bool __stdcall proxy_Dinput_GetDeviceState(DWORD size, LPBYTE data)
{
    typedef bool(__stdcall* pDinput_GetDeviceState)(DWORD size, LPBYTE data);
    bool suceeded = pDinput_GetDeviceState(0x0040C770)(size, data);

    //MessageBox(0, 0, 0, 0);

    BYTE* buffer = data;

    //KeyMap key = keyMap[(BYTE)KeyMap::MappedKey::Grind];
    //buffer[key.DIK_KeyCode] = buffer[DIK_Y];

    if (XINPUT::Player1 && XINPUT::Player1->IsConnected())
    {
        //Move the global pointer to stack to make sure compiler optimize well
        KeyMap* __restrict const map = keyMap;
        //Move the controller1 state to stack for cache optimization
        XINPUT_STATE state = XINPUT::Player1->GetState();

        //This happens for every mapped key
        //First get pointer to the appropiet KeyMap
        //Then check if the the key is already pressed on keyboard
        //If it's not pressed check if it's pressed on controller
        KeyMap* __restrict key = &map[(BYTE)KeyMap::MappedKey::Pause];
        if(!buffer[key->DIK_KeyCode])
            buffer[key->DIK_KeyCode] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) * 0x80;
        //Set stack boolean to know if Pause key is pressed
        bool pause = (buffer[key->DIK_KeyCode] == 0x80);

        key = &map[(BYTE)KeyMap::MappedKey::CameraToggle];
        if (!buffer[key->DIK_KeyCode])
            buffer[key->DIK_KeyCode] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) * 0x80;
        //Set stack boolean to know if CameraToggle key is pressed
        bool start = (buffer[key->DIK_KeyCode] == 0x80);

        key = &map[(BYTE)KeyMap::MappedKey::CameraLock];
        if (!buffer[key->DIK_KeyCode])
            buffer[key->DIK_KeyCode] = (pause && start);

        key = &map[(BYTE)KeyMap::MappedKey::Grind];
        if (!buffer[key->DIK_KeyCode])
            buffer[key->DIK_KeyCode] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) * 0x80;

        key = &map[(BYTE)KeyMap::MappedKey::Grab];
        if (!buffer[key->DIK_KeyCode])
            buffer[key->DIK_KeyCode] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) * 0x80;

        key = &map[(BYTE)KeyMap::MappedKey::Ollie];
        if (!buffer[key->DIK_KeyCode])
            buffer[key->DIK_KeyCode] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) * 0x80;

        key = &map[(BYTE)KeyMap::MappedKey::Flip];
        if (!buffer[key->DIK_KeyCode])
            buffer[key->DIK_KeyCode] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) * 0x80;

        key = &map[(BYTE)KeyMap::MappedKey::SpinLeft];
        if (!buffer[key->DIK_KeyCode])
            buffer[key->DIK_KeyCode] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) * 0x80;

        key = &map[(BYTE)KeyMap::MappedKey::SpinRight];
        if (!buffer[key->DIK_KeyCode])
            buffer[key->DIK_KeyCode] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) * 0x80;

        key = &map[(BYTE)KeyMap::MappedKey::Nollie];
        if (!buffer[key->DIK_KeyCode])
            buffer[key->DIK_KeyCode] = ((state.Gamepad.bLeftTrigger) > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? 0x80 : 0;

        key = &map[(BYTE)KeyMap::MappedKey::Revert];
        if (!buffer[key->DIK_KeyCode])
            buffer[key->DIK_KeyCode] = ((state.Gamepad.bRightTrigger) > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? 0x80 : 0;

        key = &map[(BYTE)KeyMap::MappedKey::Right];
        if (!buffer[key->DIK_KeyCode])
        {
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
                buffer[key->DIK_KeyCode] = 0x80;
            else
            {
                if (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
                {
                    buffer[key->DIK_KeyCode] = 0x80;// ClampValue(state.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 32767, 0x40, 0xFF);
                }
                else
                    buffer[key->DIK_KeyCode] = 0;

            }
        }

        key = &map[(BYTE)KeyMap::MappedKey::Left];
        if (!buffer[key->DIK_KeyCode])
        {
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
                buffer[key->DIK_KeyCode] = 0x80;
            else
            {
                if (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
                {
                    buffer[key->DIK_KeyCode] = 0x80;// ClampValue(state.Gamepad.sThumbLX, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32767, 0x40, 0xFF);
                }
                else
                    buffer[key->DIK_KeyCode] = 0;
            }
        }

        key = &map[(BYTE)KeyMap::MappedKey::Up];
        if (!buffer[key->DIK_KeyCode])
        {
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
                buffer[key->DIK_KeyCode] = 0x80;
            else
            {
                if (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
                {
                    buffer[key->DIK_KeyCode] = 0x80;// ClampValue(state.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 32767, 0x40, 0xFF);
                }
                else
                    buffer[key->DIK_KeyCode] = 0;
            }
        }

        key = &map[(BYTE)KeyMap::MappedKey::Down];
        if (!buffer[key->DIK_KeyCode])
        {
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
                buffer[key->DIK_KeyCode] = 0x80;
            else
            {
                if (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
                {
                    buffer[key->DIK_KeyCode] = 0x80;// ClampValue(state.Gamepad.sThumbLY, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 32767, 0x40, 0xFF);
                }
                else
                    buffer[key->DIK_KeyCode] = 0;
            }
        }

    }

    return suceeded;
}


//Adds a vibration, this function is hooked into the games
//Skater->CallMemberFunction()
void AddVibration(CStruct* pStruct, CScript* pScript)
{
    if (pStruct)
    {
        if (pScript)
            debug_print("Adding vibration from script %s location %p\n", FindChecksumName(pScript->scriptChecksum), pScript->address);
        else
            debug_print("Adding vibration from unknown script??\n");

        CStructHeader* pActuator, * pPercent, * pDuration = NULL;

        pStruct->GetStruct(Checksums::Actuator, &pActuator);

        //Check if got actuator, if got it return the value to bool rightMotor, else default to leftMotor
        bool rightMotor = pActuator ? pActuator->value.i : false;
        //Get pointer to first motor value
        //And add the bool to the motor pointer
        WORD* motor = &XINPUT::vibration.wLeftMotorSpeed;
        motor += rightMotor;

        //Flag to turn off vibration on the motor
        if (pStruct->ContainsFlag("Off"))
        {
            *motor = 0;
            return;
        }

        //The percent is value between 0-100
        if (pStruct->GetStruct(Checksums::Percent, &pPercent))
        {
            debug_print("GotParam Percent %d\n", pPercent->value.i);
            double vibrationLevel = 65535.0;
            vibrationLevel *= (((double)pPercent->value.i / 100.0));

            *motor = (WORD)(vibrationLevel + 0.5);
            debug_print("Setting vibration to %f(%d) on motor %d\n", vibrationLevel, *motor, rightMotor);
        }
        else
        {
            debug_print("Setting vibration to MAX on motor %d\n", rightMotor);
            *motor = 65535;
        }

        //The duration is a value between 0-1
        //The game multiply that value by 1000 to get the number of frames
        //If duration is not found it should vibrate until off is called
        XINPUT::vibrationFrames = 0xFFFFFFFF;

        if (pStruct->GetStruct(Checksums::Duration, &pDuration))
        {
            XINPUT::vibrationFrames = (DWORD)(pDuration->value.f * 1000.0f);
            debug_print("Vibration for %d frames\n", XINPUT::vibrationFrames);

        }
        else
            debug_print("Vibration until Off is called\n");

    }
    else if (pScript)
        debug_print("no params for vibration?\nIn script %s location %p\n", FindChecksumName(pScript->scriptChecksum), pScript->address);
    else
        debug_print("Wrong pointers in " __FUNCTION__ "\n");

}




//insert hook 004B1885
//jmp to adress 004B1FB9
//don't need pushad or pushfd


__declspec(naked) void __cdecl HookVibrate_naked()
{
    static DWORD jmpBack = 0x004B1FB9;
    static CStruct* pStruct = NULL;
    static CScript* pScript = NULL;
    static DWORD pOldESP, pOldEBP;
    _asm mov pOldESP, esp;
    _asm mov pOldEBP, ebp;
    _asm mov eax, [esp + 0x000001C8];
    _asm mov pStruct, eax;
    _asm mov eax, [esp + 0x000001CC];
    _asm mov pScript, eax;
    AddVibration(pStruct, pScript);
    _asm mov esp, pOldESP;
    _asm mov ebp, pOldEBP;
    _asm jmp[jmpBack];
}


void HookVibrate()
{
    DWORD old = 0;
    VirtualProtect((LPVOID)0x004B1885, 6, PAGE_EXECUTE_READWRITE, &old);

    *(DWORD*)0x004B1885 = 0xE9;
    *(DWORD*)0x004B1886 = (PtrToUlong(HookVibrate_naked) - 0x004B1885) - 5;
    *(DWORD*)0x004B188A = 0x90;
}

DWORD UpdateKeyState(KeyState* state, DWORD press)
{
    //debug_print("Updaing state %p\n", state);
    if (!XINPUT::Player1->IsConnected())
        return press;


    //DWORD time = _GetCurrentTime();
    switch (state->GetChecksum())
    {
    case Checksums::X:
        press = (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) * 0xFF;
        break;
    case Checksums::Circle:
        press = (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) * 0xFF;
        break;
    case Checksums::Square:
        press = (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X) * 0xFF;
        break;
    case Checksums::Triangle:
        press = (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y) * 0xFF;
        break;
    case Checksums::L1://LEFTSPIN
        press = (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) * 0xFF;
        break;
    case (DWORD)Checksums::R1://RIGHTSPIN
        press = (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) * 0xFF;
        break;
    case (DWORD)Checksums::L2://NOLLIE
        if ((XINPUT::Player1->GetState().Gamepad.bLeftTrigger) > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
            press = 0xFF;
        else
            press = 0;
        break;
    case Checksums::R2://REVERT
        if ((XINPUT::Player1->GetState().Gamepad.bRightTrigger) > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
            press = 0xFF;
        else
            press = 0;
        break;

    case Checksums::Right:
        //state->Update(time, press);
        if (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
            press = 0xFF;
        else
        {
            if (XINPUT::Player1->GetState().Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
            {
                press = ClampValue(XINPUT::Player1->GetState().Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 32767, 0x40, 0xFF);
            }
            else
                press = 0;
        }
        break;
    case (DWORD)Checksums::Left:
        if (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
            press = 0xFF;
        else
        {
            if (XINPUT::Player1->GetState().Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
            {
                press = ClampValue(XINPUT::Player1->GetState().Gamepad.sThumbLX, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32767, 0x40, 0xFF);
            }
            else
                press = 0;
        }
        break;

    case (DWORD)Checksums::Up:
        if (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
            press = 0xFF;
        else
        {
            if (XINPUT::Player1->GetState().Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
            {
                press = ClampValue(XINPUT::Player1->GetState().Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 32767, 0x40, 0xFF);
            }
            else
                press = 0;
        }
        break;
    case (DWORD)Checksums::Down:
        if (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
            press = 0xFF;
        else
        {
            if (XINPUT::Player1->GetState().Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
            {
                press = ClampValue(XINPUT::Player1->GetState().Gamepad.sThumbLY, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 32767, 0x40, 0xFF);
            }
            else
                press = 0;
        }
        break;

    default:
        debug_print("Unknown button press %s(%X)", FindChecksumName(state->GetChecksum()));
        break;
    }
    //MessageBox(0, "THIS IS NOT GOOD", "", 0);
    return press;
}



BYTE UpdateKeyState2(DWORD button, BYTE pressed)
{
    enum
    {
        up = 1,
        down = 2,
        left = 3,
        right = 4,
        upleft = 5,
        upright = 6,
        downleft = 7,
        downright = 8,
        grab = 9,
        flip = 10,
        jump = 11,
        grind = 12,
        spinleft = 13,
        nollie = 14,
        spinright = 15,
        revert = 16
    };
    if (!GameState::GotSuperSectors || !XINPUT::Player1->IsConnected() || button > 16)
    {
        if (pressed)
            debug_print("Unhandled keypress %d pressed %d\n", button, pressed);
        return pressed;
    }
    Skater* skater = Skater::Instance();

    switch (button)
    {
    case up:
    case down:
    case left:
    case right:
        return skater->GetKeyState(button - 1)->IsPressed();
        break;

    case upleft:
        return skater->GetKeyState(KeyState::UP)->IsPressed() && skater->GetKeyState(KeyState::LEFT)->IsPressed();
        break;

    case upright:
        return skater->GetKeyState(KeyState::UP)->IsPressed() && skater->GetKeyState(KeyState::RIGHT)->IsPressed();
        break;

    case downleft:
        return skater->GetKeyState(KeyState::DOWN)->IsPressed() && skater->GetKeyState(KeyState::LEFT)->IsPressed();
        break;

    case downright:
        return skater->GetKeyState(KeyState::DOWN)->IsPressed() && skater->GetKeyState(KeyState::RIGHT)->IsPressed();
        break;

    case grab:
        return skater->GetKeyState(KeyState::GRAB)->IsPressed();
        break;

    case flip:
        return skater->GetKeyState(KeyState::FLIP)->IsPressed();
        break;

    case jump:
        return skater->GetKeyState(KeyState::OLLIE)->IsPressed();
        break;

    case grind:
        return skater->GetKeyState(KeyState::GRIND)->IsPressed();
        break;

    case spinleft:
        return skater->GetKeyState(KeyState::SPINLEFT)->IsPressed();
        break;

    case nollie:
        return skater->GetKeyState(KeyState::NOLLIE)->IsPressed();
        break;

    case spinright:
        return skater->GetKeyState(KeyState::SPINRIGHT)->IsPressed();
        break;

    case revert:
        return skater->GetKeyState(KeyState::REVERT)->IsPressed();
        break;
    }
    //should never happen
    return pressed;
}

void __cdecl UpdateKeyState_naked(DWORD press, void* pUnk)
{
    static bool bUpdate;
    KeyState* pState;
    static DWORD pCall = 0x0049BAA0;
    _asm mov pState, ecx;
    press = UpdateKeyState(pState, press);
    typedef void(__thiscall* const pUpdateKeyState)(KeyState* state, DWORD press, void* pUnk);
    pUpdateKeyState(0x0049BAA0)(pState, press, pUnk);
    _asm mov esp, ebp
    _asm pop ebp;
    _asm ret 8;
}
typedef void(__thiscall* const pUpdateKeyBuffer)(void* pThis, DWORD key, DWORD value);


__declspec(naked) void __cdecl UpdateKeyState2_naked()
{
    static DWORD pCall = 0x004B49D0;
    static DWORD key = 0;
    static BYTE value = 0;
    static DWORD pOldESP;
    static DWORD pOldEBP;
    static void* pOld;
    _asm mov pOldESP, esp;
    _asm mov pOldEBP, ebp;
    _asm mov pOld, ecx;
    _asm mov eax, [esp + 4];
    _asm mov key, eax;
    _asm mov al, [esp + 8];
    _asm mov value, al;
    value = UpdateKeyState2(key, value);
    _asm mov al, value
    _asm mov esp, pOldESP;
    _asm mov ebp, pOldEBP;
    _asm mov[esp + 8], al;
    _asm mov ecx, pOld;
    _asm jmp[pCall];
    _asm ret;

    /*}
    _asm mov ecx, pSkater
    _asm mov esp, pOldESP;
    _asm mov ebp, pOldEBP;
    _asm ret 8;*/
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

void HookControls()
{
    HookVibrate();
    //Hook dinput
    HookFunction(0x0040CA2B, proxy_Dinput_GetDeviceState);

    return;
    DWORD old;
    VirtualProtect((LPVOID)0x00498D5D, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498D6B, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498D79, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498D87, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498D95, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498DA3, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498DB1, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498DBF, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498DDB, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498DE9, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498DF7, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498F6D, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498F87, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498FA3, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498FBF, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498FDC, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00498FFB, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x0049901A, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00499037, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00499056, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00499075, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00499092, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x004990B1, 4, PAGE_EXECUTE_READWRITE, &old);

    VirtualProtect((LPVOID)0x00499167, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00499176, 5, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00499186, 5, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00499196, 5, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x004991A6, 5, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x004991B6, 5, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x004991C6, 5, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x004991D6, 5, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x004991E4, 5, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x004991F2, 5, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00499200, 5, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x0049920E, 5, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x0049921C, 5, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x0049922A, 5, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00499238, 5, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((LPVOID)0x00499246, 5, PAGE_EXECUTE_READWRITE, &old);



    /**(DWORD*)0x00499167 = (PtrToUlong(UpdateKeyState2_naked) - 0x00499167) - 4;
    *(DWORD*)0x00499177 = (PtrToUlong(UpdateKeyState2_naked) - 0x00499177) - 4;
    *(DWORD*)0x00499187 = (PtrToUlong(UpdateKeyState2_naked) - 0x00499187) - 4;
    *(DWORD*)0x00499197 = (PtrToUlong(UpdateKeyState2_naked) - 0x00499197) - 4;
    *(DWORD*)0x004991A7 = (PtrToUlong(UpdateKeyState2_naked) - 0x004991A7) - 4;
    *(DWORD*)0x004991B7 = (PtrToUlong(UpdateKeyState2_naked) - 0x004991B7) - 4;
    *(DWORD*)0x004991C7 = (PtrToUlong(UpdateKeyState2_naked) - 0x004991C7) - 4;
    *(DWORD*)0x004991D7 = (PtrToUlong(UpdateKeyState2_naked) - 0x004991D7) - 4;
    *(DWORD*)0x004991E5 = (PtrToUlong(UpdateKeyState2_naked) - 0x004991E5) - 4;

    *(DWORD*)0x004991F3 = (PtrToUlong(UpdateKeyState2_naked) - 0x004991F3) - 4;

    *(DWORD*)0x00499201 = (PtrToUlong(UpdateKeyState2_naked) - 0x00499201) - 4;

    *(DWORD*)0x0049920F = (PtrToUlong(UpdateKeyState2_naked) - 0x0049920F) - 4;

    *(DWORD*)0x0049921D = (PtrToUlong(UpdateKeyState2_naked) - 0x0049921D) - 4;

    *(DWORD*)0x0049922B = (PtrToUlong(UpdateKeyState2_naked) - 0x0049922B) - 4;

    *(DWORD*)0x00499239 = (PtrToUlong(UpdateKeyState2_naked) - 0x00499239) - 4;

    *(DWORD*)0x00499247 = (PtrToUlong(UpdateKeyState2_naked) - 0x00499247) - 4;*/

    /**(BYTE*)0x00499176 = 0x90;
    *(DWORD*)0x00499177 = 0x90909090;
    *(BYTE*)0x00499186 = 0x90;
    *(DWORD*)0x00499187 = 0x90909090;
    *(BYTE*)0x00499196 = 0x90;
    *(DWORD*)0x00499197 = 0x90909090;
    *(BYTE*)0x004991A6 = 0x90;
    *(DWORD*)0x004991A7 = 0x90909090;
    *(BYTE*)0x004991B6 = 0x90;
    *(DWORD*)0x004991B7 = 0x90909090;
    *(BYTE*)0x004991C6 = 0x90;
    *(DWORD*)0x004991C7 = 0x90909090;
    *(BYTE*)0x004991D6 = 0x90;
    *(DWORD*)0x004991D7 = 0x90909090;
    *(BYTE*)0x004991E4 = 0x90;
    *(DWORD*)0x004991E5 = 0x90909090;
    *(BYTE*)0x004991F2 = 0x90;
    *(DWORD*)0x004991F3 = 0x90909090;
    *(BYTE*)0x00499200 = 0x90;
    *(DWORD*)0x00499201 = 0x90909090;*/


    /**(BYTE*)0x0049920E = 0x90;
    *(DWORD*)0x0049920F = 0x90909090;
    *(BYTE*)0x0049921C = 0x90;
    *(DWORD*)0x0049921D = 0x90909090;
    *(BYTE*)0x0049922A = 0x90;
    *(DWORD*)0x0049922B = 0x90909090;
    *(BYTE*)0x00499238 = 0x90;
    *(DWORD*)0x00499239 = 0x90909090;
    *(BYTE*)0x00499246 = 0x90;
    *(DWORD*)0x00499247 = 0x90909090;*/

    //Set to zero
    //*(DWORD*)0x00498D5D = (PtrToUlong(UpdateKeyState) - 0x00498D5D) - 4;
    //*(DWORD*)0x00498D6B = (PtrToUlong(UpdateKeyState) - 0x00498D6B) - 4;
    //*(DWORD*)0x00498D79 = (PtrToUlong(UpdateKeyState) - 0x00498D79) - 4;
    //*(DWORD*)0x00498D87 = (PtrToUlong(UpdateKeyState) - 0x00498D87) - 4;
    //*(DWORD*)0x00498D95 = (PtrToUlong(UpdateKeyState) - 0x00498D95) - 4;
    //*(DWORD*)0x00498DA3 = (PtrToUlong(UpdateKeyState) - 0x00498DA3) - 4;
    //*(DWORD*)0x00498DB1 = (PtrToUlong(UpdateKeyState) - 0x00498DB1) - 4;
    //*(DWORD*)0x00498DBF = (PtrToUlong(UpdateKeyState) - 0x00498DBF) - 4;
    //*(DWORD*)0x00498DCD = (PtrToUlong(UpdateKeyState) - 0x00498DCD) - 4;
    //*(DWORD*)0x00498DDB = (PtrToUlong(UpdateKeyState) - 0x00498DDB) - 4;
    //*(DWORD*)0x00498DE9 = (PtrToUlong(UpdateKeyState) - 0x00498DE9) - 4;
    //*(DWORD*)0x00498DF7 = (PtrToUlong(UpdateKeyState) - 0x00498DF7) - 4;


    * (DWORD*)0x00498F6D = (PtrToUlong(UpdateKeyState_naked) - 0x00498F6D) - 4;
    *(DWORD*)0x00498F87 = (PtrToUlong(UpdateKeyState_naked) - 0x00498F87) - 4;
    *(DWORD*)0x00498FA3 = (PtrToUlong(UpdateKeyState_naked) - 0x00498FA3) - 4;
    *(DWORD*)0x00498FBF = (PtrToUlong(UpdateKeyState_naked) - 0x00498FBF) - 4;
    *(DWORD*)0x00498FDC = (PtrToUlong(UpdateKeyState_naked) - 0x00498FDC) - 4;
    *(DWORD*)0x00498FFB = (PtrToUlong(UpdateKeyState_naked) - 0x00498FFB) - 4;
    *(DWORD*)0x0049901A = (PtrToUlong(UpdateKeyState_naked) - 0x0049901A) - 4;
    *(DWORD*)0x00499037 = (PtrToUlong(UpdateKeyState_naked) - 0x00499037) - 4;
    *(DWORD*)0x00499056 = (PtrToUlong(UpdateKeyState_naked) - 0x00499056) - 4;
    *(DWORD*)0x00499075 = (PtrToUlong(UpdateKeyState_naked) - 0x00499075) - 4;
    *(DWORD*)0x00499092 = (PtrToUlong(UpdateKeyState_naked) - 0x00499092) - 4;
    *(DWORD*)0x004990B1 = (PtrToUlong(UpdateKeyState_naked) - 0x004990B1) - 4;
}

EXTERN void ProxyPad(Skater* skater)
{
    if (XINPUT::Player1->IsConnected()) [[unlikely]]
    {
        //debug_print("Player1 Is Connected.\n going to update...\n");
        skater->UpdateKeyState(KeyState::OLLIE, (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) * 0xFF);
        skater->UpdateKeyState(KeyState::GRAB, (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) * 0xFF);
        skater->UpdateKeyState(KeyState::FLIP, (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X) * 0xFF);
        skater->UpdateKeyState(KeyState::GRIND, (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y) * 0xFF);
        skater->UpdateKeyState(KeyState::SPINLEFT, (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) * 0xFF);
        skater->UpdateKeyState(KeyState::SPINRIGHT, (XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) * 0xFF);

        //IsButton above trigger threshold?
        if ((XINPUT::Player1->GetState().Gamepad.bLeftTrigger) > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
            skater->UpdateKeyState(KeyState::NOLLIE, 0xFF);
        else
            skater->UpdateKeyState(KeyState::NOLLIE, 0);

        //Is button above trigger threshold?
        if ((XINPUT::Player1->GetState().Gamepad.bRightTrigger) > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
          skater->UpdateKeyState(KeyState::REVERT, 0xFF);
        else
            skater->UpdateKeyState(KeyState::REVERT, 0);

        //Is thumb in deadzone?
        if (XINPUT::Player1->GetState().Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
            //ClampValue between 0x40 and 0xFF(0-0x40=deadzone)
            skater->UpdateKeyState(KeyState::RIGHT, ClampValue(XINPUT::Player1->GetState().Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 32767, 0x40, 0xFF));
            skater->UpdateKeyState(KeyState::LEFT, 0);
        }
        else if (XINPUT::Player1->GetState().Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
            //ClampValue between 0x40 and 0xFF(0-0x40=deadzone)
            skater->UpdateKeyState(KeyState::LEFT, ClampValue(XINPUT::Player1->GetState().Gamepad.sThumbLX, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32767, 0x40, 0xFF));
            skater->UpdateKeyState(KeyState::RIGHT, 0);
        }
        else
        {
            skater->UpdateKeyState(KeyState::LEFT, 0);
            skater->UpdateKeyState(KeyState::RIGHT, 0);
        }

    }
}
