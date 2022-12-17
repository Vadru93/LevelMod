#define NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"

CXBOXController::CXBOXController(int playerNumber)
{
    // Set the Controller Number
    _controllerNum = playerNumber - 1;
}

XINPUT_STATE CXBOXController::GetState()
{
    /*// Zeroise the state
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    // Get the state
    XInputGetState(_controllerNum, &_controllerState);
    */
    return _controllerState;
}

bool CXBOXController::IsConnected()
{
    // Zeroise the state
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    // Get the state
    DWORD Result = XInputGetState(_controllerNum, &_controllerState);

    if (Result == ERROR_SUCCESS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CXBOXController::Vibrate(int leftVal, int rightVal)
{
    // Create a Vibraton State
    XINPUT_VIBRATION Vibration;

    // Zeroise the Vibration
    ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

    // Set the Vibration Values
    Vibration.wLeftMotorSpeed = leftVal;
    Vibration.wRightMotorSpeed = rightVal;

    // Vibrate the controller
    XInputSetState(_controllerNum, &Vibration);
}