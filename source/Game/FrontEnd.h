#pragma once
#include "pch.h"

struct FrontEnd
{
    void SetScriptToRun_Hook(DWORD checksum, CStruct* params)
    {
        debug_print("FrontEnd: Setting ScriptToRun to %s\n", FindChecksumName(checksum));
        QBKeyHeader* header = GetQBKeyHeader(checksum);
        if (header)
        {
            if (header->type == QBKeyHeader::SCRIPTED_FUNCTION)
            {
                typedef void(__thiscall* const pSetScriptToRun)(FrontEnd* pThis, DWORD, CStruct*);
                pSetScriptToRun(0x00449D30)(this, checksum, params);
            }
            else if (header->type == QBKeyHeader::COMPILED_FUNCTION)
                header->pFunction(params, NULL);
            else
                debug_print("FrontEnd: Cannot call type %s\n", QScript::QBTypes[header->type]);
        }
        else
            debug_print("FrontEnd: Couldn't find QBKey\n");
    }

    void UpdateScript_Hook()
    {
        CScript* pScript = (CScript*)this;
        DWORD checksum = pScript->scriptChecksum;

        debug_print("FrontEnd: Update Script %s\n", FindChecksumName(checksum));
        QBKeyHeader* header = GetQBKeyHeader(checksum);
        if (header)
        {
            if (header->type == QBKeyHeader::SCRIPTED_FUNCTION)
            {
                typedef void(__thiscall* const pUpdate)(CScript* pThis);
                pUpdate(0x00427940)(pScript);
            }
            else if (header->type == QBKeyHeader::COMPILED_FUNCTION)
                header->pFunction(pScript->params, pScript);
            else
                debug_print("FrontEnd: Cannot call type %s\n", QScript::QBTypes[header->type]);
        }
        else
            debug_print("FrontEnd: Couldn't find QBKey\n");
    }
};