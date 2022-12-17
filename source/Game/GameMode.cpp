#include "pch.h"

DWORD GameMode::GetNameChecksum()
{
    typedef DWORD(__thiscall* const pGetGameNameChecksum)(GameMode* pThis);
    return pGetGameNameChecksum(0x0046FF67)(this);
}