#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Script\Script.h"
#include "Math\_Math.h"
#include "Game\Skater.h"
#include "Script\Checksum.h"
#include "dinput.h"
#include "Settings\Settings.h"
#include "Settings\IniReader.h"
#include "Settings\IniWriter.h"

bool XINPUT::vibrating = false;
XINPUT_VIBRATION XINPUT::vibration;
DWORD XINPUT::vibrationFrames = 0;
CXBOXController* __restrict XINPUT::Player1 = NULL;
KeyMap* __restrict keyMap = NULL;
BYTE* __restrict keyState = NULL;



const char* vk_names[] =
{
    "Unassigned",
    "Left Mouse",//0x1
    "Right Mouse",//0x2
    "Cancel"//0x3
    "Middle Mouse",//0x4
    "Scroll Up",//0x5
    "Scroll Down",//0x6
    "UNDEFINED",//0x7
    "Backspace",//0x8
    "Unknown",
    "Tab",//0x9
    "Unassigned",//0xA
    "Unassigned",//0xB
    "Clear",//0xC
    "Enter",//0xD
    "Undefined",//0xE
    "Undefined",//0xF
    "Shift",//0x10
    "Control",//0x11
    "Alt",//0x12
    "Pause",//0x13
    "CapsLock",//0x14
    "IME Kana",//0x15
    "IME On",//0x16
    "IME Junja",//0x17
    "IME Final",//0x18
    "IME Hanja"//0x19
    "IME Off",//0x1A
    "Unknown",
    "Escape",//0x1B
    "IME Convert",//0x1C
    "IME Nonconvert",//0x1D
    "IME Accept",//0x1E
    "IME ChangeMode",//0x1F
    "Space",//0x20
    "PageUp",//0x21
    "PageDown",//0x22
    "End",//0x23
    "Home",//0x24
        "Left",//0x25
        "Up",//0x26
        "Right",//0x27
        "Down",//0x28
        "Select",//0x29
        "Print",//0x2A
        "Execute",//0x2B
        "PrintScreen",//0x2C
        "Insert",//0x2D
        "Delete",//0x2E
        "Help",//0x2F
        "0",//0x30
        "1",//0x31
        "2",//0x32
        "3",//0x33
        "4",//0x34
        "5",//0x35
        "6",//0x36
        "7",//0x37
        "8",//0x38
        "9",//0x39
        "Undefined",//0x3A
        "Undefined",//0x3B
    "Undefined",//0x3C
    "Undefined",//0x3D
    "Undefined",//0x3E
    "Undefined",//0x3F
    "Undefined",//0x40
        "A",//0x41
        "B",//0x42
        "C",//0x43
        "D",//0x44
        "E",//0x45
        "F",//0x46
        "G",//0x47
        "H",//0x48
        "I",//0x49
        "J",//0x4A
        "K",//0x4B
        "L",
        "M",
        "N",
        "O",
        "P",
        "Q",
        "R",
        "S",
        "T",
        "U",
        "V",
        "W",
        "X",
        "Y",
        "Z",//0x5A
        "LeftWindowsKey",//0x5B
        "RightWindowsKey",
        "ApplicationsKey",
        "Reserved"
        "Sleep",
        "Unknown",
        "NumPad0",
        "NumPad1",
        "NumPad2",
        "NumPad3",
        "NumPad4",
        "NumPad5",
        "NumPad6",
        "NumPad7",
        "NumPad8",
        "NumPad9",
        "Multiply",
        "Add",
        "Seperator",
        "Subtract",
        "Decimal",
        "Divide",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5",
        "F6",
        "F7",
        "F8",
        "F9",
        "F10",
        "F11",
        "F12",
        "F13",
        "F14",
        "F15",
        "F16",
        "F17",
        "F18",
        "F19",
        "F20",
        "F21",
        "F22",
        "F23",
        "F24",
        "Unassigned",
        "Unassigned",
            "Unassigned",
            "Unassigned",
            "Unassigned",
            "Unassigned",
            "Unassigned",
            "Unassigned",
        "Numlock",
        "ScrollLock",
        "Unknown",
            "Unknown",
            "Unknown",
            "Unknown",
            "Unknown",
            "Unassigned",
            "Unassigned",
            "Unassigned",
            "Unassigned",
            "Unassigned",
            "Unassigned",
            "Unassigned",
            "Unassigned",
        "LeftShift",
        "RightShift",
        "LeftControl",
        "RightContol",
        "LeftMenu",
        "RightMenu",
        "BrowserBack",
        "BrowserForward",
        "BrowserRefresh",
        "BrowserStop",
        "BrowserSearch",
        "BrowserFavorites",
        "BrowserHome",
        "VolumeMute",
        "VolumeDown",
        "VolumeUp",
        "NextTrack",
        "PreviousTrack",
        "StopMedia",
        "PlayPause",
        "LaunchMail",
        "SelectMedia",
        "LaunchApp1",
        "LaunchApp2",
        "OEM1",
        "OEMPlus",
        "OEMComma",
        "OEMMinus",
        "OEMPeriod",
        "OEM2",
        "OEM3",
        "OEM4",
        "OEM5",
        "OEM6",
        "OEM7",
        "OEM8",
        "OEM102",
        "Process",
        "Packet",
        "Attn",
        "CrSel",
        "ExSel",
        "EraseEOF",
        "Play",
        "Zoom",
        "PA1",
        "OEMClear",
            "Unknown",
};


const char* MappedKeyNames[] =
{
    "Pause",
    "CameraToggle",
    "SpineTransfer",
    "CameraLock",
    "Grind",
    "Grab",
    "Ollie",
    "Flip",
    "SpinLeft",
    "SpinRight",
    "Nollie",
    "Revert",
    "Right",
    "Left",
    "Up",
    "Down",
    "Undefined"
};

KeyMap::MappedKey KeyMap::GetKeyType()
{
    for (BYTE i = 0; i < (BYTE)MappedKey::Undefined; i++)
    {
        if (this == &keyMap[i])
            return (MappedKey)i;
    }
    return MappedKey::Undefined;
}

const char* KeyMap::GetName(KeyMap::MappedKey key)
{
    return MappedKeyNames[(BYTE)key];
}

void  KeyMap::UpdateKeyMap()
{
    DWORD pKeyboardControl = *(DWORD*)0x008510B8;
    if (!pKeyboardControl)
        return;
    DWORD pKeyMap = *(DWORD*)(pKeyboardControl + 0x10);
    if (!pKeyMap || pKeyMap == 0x10)
        return;
    pKeyMap = *(DWORD*)(pKeyMap + 4);
    if (!pKeyMap || pKeyMap == 4)
        return;
    keyState = (BYTE*)(pKeyMap + 0x24);
    if (!pKeyMap || pKeyMap == 0x24)
        return;
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

void KeyMap::Set(VirtualKeyCode code, bool mapped)
{
    //Enable mapping for this key?
    this->mapped = mapped;
    //Map the VirtualKeyCode to DirectInput KeyCode
    if (code != VirtualKeyCode::Undefined)
        this->DIK_KeyCode = (WORD)MapVirtualKeyA((UINT)code, MAPVK_VK_TO_VSC);
    else//Undefined keycode, set to unassigned
        this->DIK_KeyCode = (WORD)code;

    UpdateText(code);
}

void KeyMap::UpdateText(VirtualKeyCode code)
{
    CStruct params;
    CStructHeader param(QBKeyHeader::STRING, 0, code == VirtualKeyCode::MAX ? (char*)KeyState::GetVKName((VirtualKeyCode)MapVirtualKeyA((UINT)this->DIK_KeyCode, MAPVK_VSC_TO_VK)) : (char*)KeyState::GetVKName(code));
    CStructHeader param2;
    param.NextHeader = &param2;

    KeyMap::MappedKey key = this->GetKeyType();
    switch ((DWORD)key)
    {
    case (DWORD)KeyMap::MappedKey::Up:
        //param = CStructHeader(QBKeyHeader::LOCAL, Checksum("KeyMap"), Checksums::Up);
        param2 = CStructHeader(QBKeyHeader::LOCAL, Checksums::id, Checksum("up_text_id"));
        break;
    case (BYTE)KeyMap::MappedKey::Down:
        //param = CStructHeader(QBKeyHeader::LOCAL, Checksum("KeyMap"), Checksums::Down);
        param2 = CStructHeader(QBKeyHeader::LOCAL, Checksums::id, Checksum("down_text_id"));
        break;
    case (BYTE)KeyMap::MappedKey::Left:
        //param = CStructHeader(QBKeyHeader::LOCAL, Checksum("KeyMap"), Checksums::Left);
        param2 = CStructHeader(QBKeyHeader::LOCAL, Checksums::id, Checksum("left_text_id"));
        break;
    case (BYTE)KeyMap::MappedKey::Right:
        //param = CStructHeader(QBKeyHeader::LOCAL, Checksum("KeyMap"), Checksums::Right);
        param2 = CStructHeader(QBKeyHeader::LOCAL, Checksums::id, Checksum("right_text_id"));
        break;
    case (BYTE)KeyMap::MappedKey::Flip:
        //param = CStructHeader(QBKeyHeader::LOCAL, Checksum("KeyMap"), Checksums::Square);
        param2 = CStructHeader(QBKeyHeader::LOCAL, Checksums::id, Checksum("flip_text_id"));
        break;
    case (BYTE)KeyMap::MappedKey::Grab:
        //param = CStructHeader(QBKeyHeader::LOCAL, Checksum("KeyMap"), Checksums::Circle);
        param2 = CStructHeader(QBKeyHeader::LOCAL, Checksums::id, Checksum("grab_text_id"));
        break;
    case (BYTE)KeyMap::MappedKey::Grind:
        //param = CStructHeader(QBKeyHeader::LOCAL, Checksum("KeyMap"), Checksums::Triangle);
        param2 = CStructHeader(QBKeyHeader::LOCAL, Checksums::id, Checksum("grind_text_id"));
        break;
    case (BYTE)KeyMap::MappedKey::SpinLeft:
        //param = CStructHeader(QBKeyHeader::LOCAL, Checksum("KeyMap"), Checksums::L1);
        param2 = CStructHeader(QBKeyHeader::LOCAL, Checksums::id, Checksum("spinleft_text_id"));
        break;
    case (BYTE)KeyMap::MappedKey::SpinRight:
        //param = CStructHeader(QBKeyHeader::LOCAL, Checksum("KeyMap"), Checksums::R1);
        param2 = CStructHeader(QBKeyHeader::LOCAL, Checksums::id, Checksum("spinright_text_id"));
        break;
    case (BYTE)KeyMap::MappedKey::Nollie:
        //param = CStructHeader(QBKeyHeader::LOCAL, Checksum("KeyMap"), Checksums::L2);
        param2 = CStructHeader(QBKeyHeader::LOCAL, Checksums::id, Checksum("nollie_text_id"));
        break;
    case (BYTE)KeyMap::MappedKey::Revert:
        //param = CStructHeader(QBKeyHeader::LOCAL, Checksum("KeyMap"), Checksums::R2);
        param2 = CStructHeader(QBKeyHeader::LOCAL, Checksums::id, Checksum("revert_text_id"));
        break;
    case (BYTE)KeyMap::MappedKey::Unused:
        //param = CStructHeader(QBKeyHeader::LOCAL, Checksum("KeyMap"), Checksums::SpineTransfer);
        param2 = CStructHeader(QBKeyHeader::LOCAL, Checksums::id, Checksum("spine_text_id"));
        //New SpineButton
        LevelModSettings::SpineButton3 = code;
        break;

    }

    params.Set(&param);
    //Set KeyMap text
    QScript::CallCFunction(Checksum("SetMenuElementText"), &params);
    //Reset error text
    SetErrorText("");
}

bool held_pause = false;
bool held_select = false;

//Game uses a hybrid between dinput and GetAsyncKeyState
//here we intercept the call to dinput GetDeveiceState and change both dinput and GetAsyncKeyState result according to our controller state
//there is also a buffer of pressed keys since last frame that seems to only be needed for a very few events, like menu navigations
//currently only 1 map is added to this buffer(start -> esc) and it seems to work
__declspec(noalias) bool __stdcall proxy_Dinput_GetDeviceState(DWORD size, LPBYTE data)
{
    typedef bool(__stdcall* pDinput_GetDeviceState)(DWORD size, LPBYTE data);
    bool suceeded = pDinput_GetDeviceState(0x0040C770)(size, data);

    //MessageBox(0, 0, 0, 0);

    BYTE* buffer = data;

    //KeyMap key = keyMap[(BYTE)KeyMap::MappedKey::Grind];
    //buffer[key.DIK_KeyCode] = buffer[DIK_Y];

    if (LevelModSettings::bHookedControls && XINPUT::Player1 && XINPUT::Player1->IsConnected())
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
        if (!buffer[key->DIK_KeyCode] && (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) && !held_pause)
        {
            buffer[key->DIK_KeyCode] = KeyState::Down;
            KeyState::Press(VirtualKeyCode::ESC);
            KeyState::SetKeyboardState(VirtualKeyCode::ESC, KeyState::Down);
            held_pause = true;
        }
        else if (!(state.Gamepad.wButtons & XINPUT_GAMEPAD_START))
            held_pause = false;

        key = &map[(BYTE)KeyMap::MappedKey::CameraToggle];
        if (!buffer[key->DIK_KeyCode] && (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) && !held_select)
        {
            buffer[key->DIK_KeyCode] = KeyState::Down;
            KeyState::SetKeyboardState(KeyMap::GetVKeyCode(KeyMap::MappedKey::CameraToggle), KeyState::Down);
            held_select = true;
        }
        else if (!(state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK))
            held_select = false;

        //Is this really R3?
        key = &map[(BYTE)KeyMap::MappedKey::CameraLock];
        if (!buffer[key->DIK_KeyCode] && state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
        {
            buffer[key->DIK_KeyCode] = KeyState::Down;
            KeyState::SetKeyboardState(KeyMap::GetVKeyCode(KeyMap::MappedKey::CameraLock), KeyState::Down);
        }

        key = &map[(BYTE)KeyMap::MappedKey::Grind];
        if (!buffer[key->DIK_KeyCode] && (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y))
        {
            buffer[key->DIK_KeyCode] = KeyState::Down;
            KeyState::SetKeyboardState(KeyMap::GetVKeyCode(KeyMap::MappedKey::Grind), KeyState::Down);
        }

        key = &map[(BYTE)KeyMap::MappedKey::Grab];
        if (!buffer[key->DIK_KeyCode] && (state.Gamepad.wButtons & XINPUT_GAMEPAD_B))
        {
            buffer[key->DIK_KeyCode] = KeyState::Down;
            KeyState::SetKeyboardState(KeyMap::GetVKeyCode(KeyMap::MappedKey::Grab), KeyState::Down);
        }

        key = &map[(BYTE)KeyMap::MappedKey::Ollie];
        if (!buffer[key->DIK_KeyCode] && (state.Gamepad.wButtons & XINPUT_GAMEPAD_A))
        {
            buffer[key->DIK_KeyCode] = KeyState::Down;
            KeyState::SetKeyboardState(KeyMap::GetVKeyCode(KeyMap::MappedKey::Ollie), KeyState::Down);
        }

        key = &map[(BYTE)KeyMap::MappedKey::Flip];
        if (!buffer[key->DIK_KeyCode] && (state.Gamepad.wButtons & XINPUT_GAMEPAD_X))
        {
            buffer[key->DIK_KeyCode] = KeyState::Down;
            KeyState::SetKeyboardState(KeyMap::GetVKeyCode(KeyMap::MappedKey::Flip), KeyState::Down);
        }

        key = &map[(BYTE)KeyMap::MappedKey::SpinLeft];
        if (!buffer[key->DIK_KeyCode] && (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER))
        {
            buffer[key->DIK_KeyCode] = KeyState::Down;
            KeyState::SetKeyboardState(KeyMap::GetVKeyCode(KeyMap::MappedKey::SpinLeft), KeyState::Down);
        }

        key = &map[(BYTE)KeyMap::MappedKey::SpinRight];
        if (!buffer[key->DIK_KeyCode] && (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER))
        {
            buffer[key->DIK_KeyCode] = KeyState::Down;
            KeyState::SetKeyboardState(KeyMap::GetVKeyCode(KeyMap::MappedKey::SpinRight), KeyState::Down);
        }

        key = &map[(BYTE)KeyMap::MappedKey::Nollie];
        if (!buffer[key->DIK_KeyCode] && ((state.Gamepad.bLeftTrigger) > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
        {
            buffer[key->DIK_KeyCode] = KeyState::Down;
            KeyState::SetKeyboardState(KeyMap::GetVKeyCode(KeyMap::MappedKey::Nollie), KeyState::Down);
        }

        key = &map[(BYTE)KeyMap::MappedKey::Revert];
        if (!buffer[key->DIK_KeyCode] && ((state.Gamepad.bRightTrigger) > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
        {
            buffer[key->DIK_KeyCode] = KeyState::Down;
            KeyState::SetKeyboardState(KeyMap::GetVKeyCode(KeyMap::MappedKey::Revert), KeyState::Down);
        }

        key = &map[(BYTE)KeyMap::MappedKey::Right];
        if (!buffer[key->DIK_KeyCode])
        {
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
                buffer[key->DIK_KeyCode] = KeyState::Down;
            else
            {
                if (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
                {
                    buffer[key->DIK_KeyCode] = KeyState::Down;// ClampValue(state.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 32767, 0x40, 0xFF);
                }

            }
            if(buffer[key->DIK_KeyCode])
                KeyState::SetKeyboardState(KeyMap::GetVKeyCode(KeyMap::MappedKey::Right), KeyState::Down);
        }

        key = &map[(BYTE)KeyMap::MappedKey::Left];
        if (!buffer[key->DIK_KeyCode])
        {
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
                buffer[key->DIK_KeyCode] = KeyState::Down;
            else
            {
                if (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
                {
                    buffer[key->DIK_KeyCode] = KeyState::Down;// ClampValue(state.Gamepad.sThumbLX, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32767, 0x40, 0xFF);
                }
            }
            if (buffer[key->DIK_KeyCode])
                KeyState::SetKeyboardState(KeyMap::GetVKeyCode(KeyMap::MappedKey::Left), KeyState::Down);
        }

        key = &map[(BYTE)KeyMap::MappedKey::Up];
        if (!buffer[key->DIK_KeyCode])
        {
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
                buffer[key->DIK_KeyCode] = KeyState::Down;
            else
            {
                if (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
                {
                    buffer[key->DIK_KeyCode] = KeyState::Down;// ClampValue(state.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 32767, 0x40, 0xFF);
                }
            }
            if (buffer[key->DIK_KeyCode])
                KeyState::SetKeyboardState(KeyMap::GetVKeyCode(KeyMap::MappedKey::Up), KeyState::Down);
        }

        key = &map[(BYTE)KeyMap::MappedKey::Down];
        if (!buffer[key->DIK_KeyCode])
        {
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
                buffer[key->DIK_KeyCode] = KeyState::Down;
            else
            {
                if (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
                {
                    buffer[key->DIK_KeyCode] = KeyState::Down;// ClampValue(state.Gamepad.sThumbLY, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 32767, 0x40, 0xFF);
                }

            }
            if (buffer[key->DIK_KeyCode])
                KeyState::SetKeyboardState(KeyMap::GetVKeyCode(KeyMap::MappedKey::Down), KeyState::Down);
        }

    }

    if (LevelModSettings::pEditKeyMap)
    {
        /*if (KeyState::GetNumKeyPress())
            MessageBox(0, "NUM", 0, 0);*/

        ZeroMemory(data, size);
    }

    return suceeded;
}

bool KeyState::FindMappedKey(VirtualKeyCode code, KeyMap::MappedKey* already_mapped)
{
    DWORD din_code = MapVirtualKeyA((DWORD)code, MAPVK_VK_TO_VSC);

    for (BYTE i = 0; i < (BYTE)KeyMap::MappedKey::Undefined; i++)
    {
        if (keyMap[i].mapped && keyMap[i].DIK_KeyCode == din_code)
        {
            *already_mapped = (KeyMap::MappedKey)i;
            return true;
        }
    }
    return false;
}

const char* KeyState::GetVKName(VirtualKeyCode code)
{
    return vk_names[(BYTE)code];
}

WORD KeyState::XINPUT_UpdateCamera_Hook(BYTE gamestate, BYTE* game_config)
{
    if (LevelModSettings::bHookedControls && XINPUT::Player1 && XINPUT::Player1->IsConnected())
    {
        BYTE* gamepad = *(BYTE**)(game_config + 0x18);

        if (gamepad)
        {
            XINPUT_STATE state = XINPUT::Player1->GetState();

            //Maybe this is actually interpolated between 0x1-0x7F and 0xFF-0x81?
            //For now let's leave it at the standard values the keyboard use
            //For all buttons:
            //0x80 = no button pressed
            //For the sticks:
            //0x1 = positive button pressed
            //0xFF = negative button pressed
            //For Normal buttons(ie Flip, Grab etc):
            //0x7F
            if (state.Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
                gamepad[CameraMovement::RightLeft] = LevelModSettings::bInvertedX ? CameraMovement::Left : CameraMovement::Right;
            else if (state.Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
                gamepad[CameraMovement::RightLeft] = LevelModSettings::bInvertedX ? CameraMovement::Right : CameraMovement::Left;
            if (state.Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
                gamepad[CameraMovement::UpDown] = LevelModSettings::bInvertedY ? CameraMovement::Down : CameraMovement::Up;
            else if (state.Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
                gamepad[CameraMovement::UpDown] = LevelModSettings::bInvertedY ? CameraMovement::Up : CameraMovement::Down;
        }
    }
    else
    {
        if (LevelModSettings::bInvertedX || LevelModSettings::bInvertedY)
        {
            BYTE* gamepad = *(BYTE**)(game_config + 0x18);

            if (gamepad)
            {
                if (LevelModSettings::bInvertedX)
                {
                    if (gamepad[CameraMovement::RightLeft] == CameraMovement::Left)
                        gamepad[CameraMovement::RightLeft] = CameraMovement::Right;
                    else if(gamepad[CameraMovement::RightLeft] == CameraMovement::Right)
                        gamepad[CameraMovement::RightLeft] = CameraMovement::Left;
                }
                if (LevelModSettings::bInvertedY)
                {
                    if (gamepad[CameraMovement::UpDown] == CameraMovement::Up)
                        gamepad[CameraMovement::UpDown] = CameraMovement::Down;
                    else if (gamepad[CameraMovement::UpDown] == CameraMovement::Down)
                        gamepad[CameraMovement::UpDown] = CameraMovement::Up;
                }
            }
        }
    }

    typedef WORD(__thiscall* const pUpdate)(KeyState* pThis, BYTE state, BYTE* key_data);
    return pUpdate(0x00498800)(this, gamestate, game_config);
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
    /*case Checksums::X:
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
        */
    case Checksums::Right:
        //state->Update(time, press);
        if(!(XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))
        {
            if (XINPUT::Player1->GetState().Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
            {
                press = ClampValue(XINPUT::Player1->GetState().Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 32767, 0x40, 0xFF);
            }
        }
        break;
    case (DWORD)Checksums::Left:
        if(!(XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))
        {
            if (XINPUT::Player1->GetState().Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
            {
                press = ClampValue(XINPUT::Player1->GetState().Gamepad.sThumbLX, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32767, 0x40, 0xFF);
            }
        }
        break;

    case (DWORD)Checksums::Up:
        if (!(XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP))
        {
            if (XINPUT::Player1->GetState().Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
            {
                press = ClampValue(XINPUT::Player1->GetState().Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 32767, 0x40, 0xFF);
            }
        }
        break;
    case (DWORD)Checksums::Down:
        
        if (!(XINPUT::Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN))
        {
            if (XINPUT::Player1->GetState().Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
            {
                press = ClampValue(XINPUT::Player1->GetState().Gamepad.sThumbLY, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 32767, 0x40, 0xFF);
            }
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

void KeyState::ClearAllKeys()
{
    debug_print("ClearAllKeys\n");
    //Store current at old
    memcpy(*(void**)0x005D0304, *(void**)0x005D0308, 254);
    //ZeroMemory Keyboard State
    ZeroMemory(*(void**)0x005D0308, 254);
    /*if (Game::skater)
        ZeroMemory((void*)Game::skater->GetKeyState(0), sizeof(KeyState) * 0x0B);*/
    debug_print("ClearKeys DOne\n");
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

__inline void HookFunction(DWORD addr, WORD(KeyState::* function)(BYTE gamestate, BYTE* key_data), BYTE byteCode = 0, DWORD nopCount = 0)
{
    DWORD old;
    VirtualProtect((void*)addr, (byteCode ? 5 : 4) + nopCount, PAGE_EXECUTE_READWRITE, &old);
    if (byteCode)
        *(DWORD*)(addr - 1) = byteCode;
    *(DWORD*)addr = (PtrToUlong((void*&)function) - addr) - 4;
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

bool ResetKeyStateScript(CStruct* pStruct, CScript* pScript)
{
    KeyState::ResetTimers();
    return true;
}

const char tst[] = "N";

bool GetTextFromKeyMapScript(CStruct* pStruct, CScript* pScript)
{
    auto header = pStruct->GetHeader();
    if (header)
    {
        const char* key_name = NULL;
        switch (header->Data)
        {
        case Checksums::Up:
            key_name = vk_names[(BYTE)KeyMap::GetVKeyCode(KeyMap::MappedKey::Up)];
            break;
        case Checksums::Down:
            key_name = vk_names[(BYTE)KeyMap::GetVKeyCode(KeyMap::MappedKey::Down)];
            break;
        case Checksums::Left:
            key_name = vk_names[(BYTE)KeyMap::GetVKeyCode(KeyMap::MappedKey::Left)];
            break;
        case Checksums::Right:
            key_name = vk_names[(BYTE)KeyMap::GetVKeyCode(KeyMap::MappedKey::Right)];
            break;
        case Checksums::Triangle:
            key_name = vk_names[(BYTE)KeyMap::GetVKeyCode(KeyMap::MappedKey::Grind)];
            break;
        case Checksums::Square:
            key_name = vk_names[(BYTE)KeyMap::GetVKeyCode(KeyMap::MappedKey::Flip)];
            break;
        case Checksums::Circle:
            key_name = vk_names[(BYTE)KeyMap::GetVKeyCode(KeyMap::MappedKey::Grab)];
            break;
        case Checksums::X:
            key_name = vk_names[(BYTE)KeyMap::GetVKeyCode(KeyMap::MappedKey::Ollie)];
            break;
        case Checksums::R2:
            key_name = vk_names[(BYTE)KeyMap::GetVKeyCode(KeyMap::MappedKey::Revert)];
            break;
        case Checksums::L2:
            key_name = vk_names[(BYTE)KeyMap::GetVKeyCode(KeyMap::MappedKey::Nollie)];
            break;
        case Checksums::L1:
            key_name = vk_names[(BYTE)KeyMap::GetVKeyCode(KeyMap::MappedKey::SpinLeft)];
            break;
        case Checksums::R1:
            key_name = vk_names[(BYTE)KeyMap::GetVKeyCode(KeyMap::MappedKey::SpinRight)];
            break;
        case Checksums::SpineTransfer:
            key_name = vk_names[(BYTE)KeyMap::GetVKeyCode(KeyMap::MappedKey::Unused)];
            break;
            
        }

        if (key_name)
        {
            CStructHeader* pParam = pScript->GetParam(crc32f("text"));
            if (!pParam)
            {
                pParam = pScript->params->AllocateCStruct();
                if (!pParam)
                {
                    debug_print(__FUNCTION__ "couldn't Allocate CStruct...\n");
                    return false;
                }
                pParam->Type = QBKeyHeader::STRING;
                pParam->QBkey = crc32f("text");
                pParam->pStr = (char*)mallocx(strlen("Undefined Key") + 1);
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

            //pParam->pStr = (char*)mallocx(strlen(key_name) + 1);
            memcpy(pParam->pStr, key_name, strlen(key_name) + 1);
            pParam->NextHeader = NULL;
            return true;
        }
    }
    return false;
}

bool EditKeyMapScript(CStruct* pStruct, CScript* pScript)
{
    auto header = pStruct->GetHeader();
    if (header)
    {
        debug_print("EditKeyMap Key: %s\n", FindChecksumName(header->Data));
        switch (header->Data)
        {
        case Checksums::Up:
            LevelModSettings::pEditKeyMap = &keyMap[(BYTE)KeyMap::MappedKey::Up];
            break;
        case Checksums::Down:
            LevelModSettings::pEditKeyMap = &keyMap[(BYTE)KeyMap::MappedKey::Down];
            break;
        case Checksums::Left:
            LevelModSettings::pEditKeyMap = &keyMap[(BYTE)KeyMap::MappedKey::Left];
            break;
        case Checksums::Right:
            LevelModSettings::pEditKeyMap = &keyMap[(BYTE)KeyMap::MappedKey::Right];
            break;
        case Checksums::Triangle:
            LevelModSettings::pEditKeyMap = &keyMap[(BYTE)KeyMap::MappedKey::Grind];
            break;
        case Checksums::Square:
            LevelModSettings::pEditKeyMap = &keyMap[(BYTE)KeyMap::MappedKey::Flip];
            break;
        case Checksums::Circle:
            LevelModSettings::pEditKeyMap = &keyMap[(BYTE)KeyMap::MappedKey::Grab];
            break;
        case Checksums::X:
            LevelModSettings::pEditKeyMap = &keyMap[(BYTE)KeyMap::MappedKey::Ollie];
            break;
        case Checksums::R2:
            LevelModSettings::pEditKeyMap = &keyMap[(BYTE)KeyMap::MappedKey::Revert];
            break;
        case Checksums::L2:
            LevelModSettings::pEditKeyMap = &keyMap[(BYTE)KeyMap::MappedKey::Nollie];
            break;
        case Checksums::L1:
            LevelModSettings::pEditKeyMap = &keyMap[(BYTE)KeyMap::MappedKey::SpinLeft];
            break;
        case Checksums::R1:
            LevelModSettings::pEditKeyMap = &keyMap[(BYTE)KeyMap::MappedKey::SpinRight];
            break;
        case Checksums::SpineTransfer:
            LevelModSettings::pEditKeyMap = &keyMap[(BYTE)KeyMap::MappedKey::Unused];
            break;
        }
        return true;
    }
    return false;
}

extern CIniWriter* ControlWriter;
extern CIniReader* ControlReader;

bool LoadKeyMap()
{
    FILE* f = fopen("Data\\input.map", "r+b");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        DWORD size = ftell(f);
        BYTE* pFile = new BYTE[size];
        fseek(f, 0, SEEK_SET);
        fread(pFile, size, 1, f);
        fclose(f);
        BYTE* pData = pFile;

        pFile += 0x7C;
        KeyMap* new_map = (KeyMap*)pFile;
        for (DWORD i = 0; i != (DWORD)KeyMap::MappedKey::Undefined; i++)
        {
            keyMap[i].Set(new_map[i].GetVKeyCode(), new_map[i].mapped);
        }
        delete pData;
        return true;
    }
    return false;
}

bool SaveKeyMap()
{
    FILE* f = fopen("Data\\input.map", "r+b");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        DWORD size = ftell(f);
        BYTE* pFile = new BYTE[size];
        fseek(f, 0, SEEK_SET);
        fread(pFile, size, 1, f);
        fseek(f, 0, SEEK_SET);
        BYTE* pData = pFile;

        pFile += 0x7C;
        KeyMap* new_map = (KeyMap*)pFile;
        for (DWORD i = 0; i != (DWORD)KeyMap::MappedKey::Undefined; i++)
        {
            new_map[i] = keyMap[i];
        }
        fwrite(pFile, size, 1, f);
        delete pData;
        return true;
    }
    return false;
}

bool KeyMapScript(CStruct* pStruct, CScript* pScript)
{
    KeyMap::UpdateKeyMap();
    if (keyMap)
    {
        BYTE control_map[(DWORD)KeyMap::MappedKey::Undefined * 2 + 3];
        BYTE* p_control_map;
        auto header = pStruct->GetHeader();
        if (header)
        {
            switch (header->Data)
            {
            case Checksums::SaveGame:
                return SaveKeyMap();
                break;
            case Checksums::LoadGame:
                return LoadKeyMap();
                break;
            case Checksums::SaveSettings:
                control_map[0] = '\"';
                for (DWORD i = 0; i != (DWORD)KeyMap::MappedKey::Undefined; i++)
                {
                    control_map[i * 2 + 1] = (BYTE)keyMap[i].mapped + 0x23;
                    control_map[i * 2 + 2] = keyMap[i].DIK_KeyCode + 0x20;
                }
                control_map[33] = '\"';
                control_map[34] = 0x0;
                ControlWriter->WriteString("Controls", "KeyMap", (char*)control_map);
                return true;
                break;
            case Checksums::LoadSettings:
                p_control_map = (BYTE*)ControlReader->ReadString("Controls", "KeyMap", "");
                if (strlen((char*)p_control_map) >= 31)
                {
                    for (DWORD i = 0; i != (DWORD)KeyMap::MappedKey::Undefined; i++)
                    {
                        keyMap[i].mapped = p_control_map[i * 2] - 0x23;
                        keyMap[i].DIK_KeyCode = p_control_map[i * 2 + 1] - 0x20;
                        keyMap[i].UpdateText();
                    }
                    return true;
                }
                break;
            case Checksums::LoadDefault:
                break;

            }
        }
    }
    return false;
}