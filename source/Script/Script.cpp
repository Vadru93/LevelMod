#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Extension\Defines.h"
#include "Script\Node.h"
#include "Memory\String.h"
#include "Render\CustomShaders.h"
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <iostream>

//#include "FastCRC.h"

#define RELOAD_LEVEL 1
#define RELOAD_SCRIPT 2

namespace FileHandler
{
    BYTE reloading = false;

    BYTE* pFile = NULL;
    const BYTE* oldFile = NULL;
    DWORD size = 0;
    bool OpenFile(const char* path, const char* mode)
    {
        pFile = NULL;
        FILE* f = fopen(path, mode);
        if (f)
        {
            fseek(f, 0, SEEK_END);
            size = ftell(f);
            fseek(f, 0, SEEK_SET);
            pFile = new BYTE[size];
            oldFile = pFile;
            fread(pFile, size, 1, f);
            fclose(f);
            return true;
        }
        return false;
    }

    void CloseFile()
    {
        delete[] oldFile;
        oldFile = NULL;
        pFile = NULL;
        size = 0;
    }

    bool CalculateCRC(DWORD& checksum, const char* path)
    {
        if (OpenFile(path, "rb"))
        {
            checksum = crc32f_file(oldFile, size);
            CloseFile();
            return true;
        }
        return false;
    }

}

const char* QScript::QBTypes[] = {
     "None",//End of array / struct and used when parsing / calling scripts
     "Int",
     "Float",
     "String",
     "Local String",//Used in struct / array	
     "Pair",
     "Vector",
     "Scripted Function",
     "Compiled Function",
     "Global Variable/Function",//Member Function in Thps4
     "Struct",
     "Local Struct",//Used in struct / array, Structure Pointer in Thps4
     "Array",
     "Local Variable/Function",//Used in struct / array, Name in Thps4
     "Unknown1",//Just a safeguard, don't think there are any more types?
     "Unknown2",
     "Unknown3",
     "Unknown4",
     "Unknown5"
};
bool(*QScript::GotParam)(CStruct*, CScript*) = NULL;
bool(*QScript::ResetClock)(CStruct*, CScript*) = NULL;
bool (*QScript::ShatterScript)(CStruct*, CScript*) = NULL;
bool (*QScript::LaunchPanelMessage)(CStruct*, CScript*) = NULL;
QScript::QBScript* QScript::Scripts=NULL;
std::vector<QScript::CompressedNode> QScript::compNodes;
std::vector<DWORD> QScript::qbKeys;
std::vector<QScript::QBFile> QScript::qbFiles;

using namespace std;
using namespace QScript;

void CheckForNodeArrayUpdates();

HANDLE dwLevelChangeHandles[2] = { NULL, NULL };
char levelPath[MAX_PATH];

QBFile levelQB;

void QScript::PanelMessage(const char* msg, ...)
{
    char panel_msg[200];
    va_list myargs;
    va_start(myargs, msg);
    vsprintf(panel_msg, msg, myargs);
    va_end(myargs);

    CScript script;
    CStruct params;
    CStructHeader header(QBKeyHeader::STRING, 0, panel_msg);
    params.Set(&header);
    LaunchPanelMessage(&params, &script);
}

void QScript::SpawnScript(DWORD checksum, CStruct* params, DWORD node, DWORD callback, CStruct* callback_params, bool AbsentInNetGames, bool NetEnabled, bool Permanent)
{
    typedef DWORD(__cdecl* const pSpawnScript)(DWORD checksum, CStruct* params, DWORD callback, CStruct* callback_params, DWORD node, bool AbsentInNetGames, bool NetEnabled, bool Permanent);
    pSpawnScript(0x00428510)(checksum, params, callback, callback_params, node, AbsentInNetGames, NetEnabled, Permanent);
}

void QScript::UpdateSpawnedScripts()
{
    *(bool*)0x008E1DF4 = true;
    DWORD numSpawnedScripts = *(DWORD*)0x008E1E1C;
    SpawnedScript* pScript = (SpawnedScript*)0x008918F8;

    for (int i = 0; i < numSpawnedScripts; i++, pScript++)
    {
        if (pScript->script && !pScript->paused)
        {
            while (true)
            {
                DWORD ret_val = pScript->script->Update();
                if (ret_val == 0)
                {
                    if (pScript->callback)
                    {
                        ExecuteQBScript(pScript->callback, pScript->params, pScript->script->node);
                    }

                    ClearScript(i);
                    break;
                }
                else if (ret_val == 2)
                    break;
            }
        }
    }
    *(bool*)0x008E1DF4 = false;

    if (*(bool*)0x008E1DF5)
    {
        *(bool*)0x008E1DF5 = false;
        for (DWORD i = 0; i < numSpawnedScripts; i++)
        {
            ClearScript(i);
        }
    }
}

void CScript::SetScript(DWORD checksum, CStruct* _params, Node* object)
{
    //Clear current script
    ClearScript();

    //We need to get the information to decide if we jump to script, call CFunction or print debug info
    auto header = GetQBKeyHeader(checksum);
    if (header)
    {
        //It's a script, let's jump to it
        if (header->type == QBKeyHeader::SCRIPTED_FUNCTION)
        {
            //Don't think this extra allocation is needed, can save few cpu cycles and memory usuage by each script call/jump
            /*CStruct* temp_params;
            if (_params)
            {
                temp_params = new CStruct();
                for (auto param = _params->head; param; param = param->NextHeader)
                {
                    temp_params->AddComponent(param);
                }
            }*/

            //Set the address of the script
            address = (BYTE*)header->pStr + 6;// + 6 = 0x23 0x16 0x?? 0x?? 0x?? 0x?? Skip function name
            //New CStruct that will contain the params
            this->params = new CStruct();
            //Add default params, if there are any
            address = this->params->AddComponentsUntilEndOfLine(this->address);
            //Not sure what this is used for
            extra = new CStruct();
            //Assign the object
            node = object;
            //the checksum of the new script
            scriptChecksum = checksum;

            if (_params)
            {
                //Add the params to the scripts
                for (auto param = _params->head; param; param = param->NextHeader)
                {
                    this->params->AddComponent(param);
                }

                /*CStructHeader* next;
                for (auto param = temp_params->head; param; param = next)
                {
                    next = param->NextHeader;
                    param->ClearComponent();
                    delete param;
                }

                delete temp_params;*/
            }

        }
        //It's a CFunction, let's call it
        else if (header->type == QBKeyHeader::COMPILED_FUNCTION)
        {
            //Assign the object incase the function want it
            this->node = object;
            header->pFunction(_params , this);
        }
        else
            debug_print("\nWarning: QBKey %s is not callable.\nType is %s\n", FindChecksumName(checksum), QScript::QBTypes[header->type]);
    }
    else
        debug_print("\nWarning: Script %s not found.\n", FindChecksumName(checksum));

    //Just keeping it here to find pointer to original function
    /*typedef DWORD(__thiscall* const pSetScript)(CScript* pThis, DWORD checksum, CStruct* params, Node* object);
return pSetScript(0x04274A0)(this, checksum, params, object);*/

}

void CScript::AdvanceToEnd()
{
    char opcode = *address;

    while (opcode != QScript::ScriptToken::EndScript)
    {
        address++;
        switch (opcode)
        {
        case QScript::ScriptToken::If2:
        case QScript::ScriptToken::Else2:
        case QScript::ScriptToken::EndSwitch2:
            address += 2;
            break;

        case QScript::ScriptToken::String:
        case QScript::ScriptToken::LocalString:
            address += *(DWORD*)address + 4;
            break;

        case QScript::ScriptToken::Int:
        case QScript::ScriptToken::Float:
        case QScript::ScriptToken::Jump:
        case QScript::ScriptToken::NewLineNumber:
        case QScript::ScriptToken::QBKey:
            address += 4;
            break;

            //Known key values
        case QScript::ScriptToken::NewLine:
        case QScript::ScriptToken::Struct:
        case QScript::ScriptToken::EndStruct:
        case QScript::ScriptToken::Array:
        case QScript::ScriptToken::EndArray:
        case QScript::ScriptToken::Equals:
        case QScript::ScriptToken::Property:
        case QScript::ScriptToken::Comma:
        case QScript::ScriptToken::Parenthesis:
        case QScript::ScriptToken::EndParenthesis:
        case QScript::ScriptToken::Begin:
        case QScript::ScriptToken::Repeat:
        case QScript::ScriptToken::Break:
        case QScript::ScriptToken::Script:
        case QScript::ScriptToken::If:
        case QScript::ScriptToken::Else:
        case QScript::ScriptToken::ElseIf:
        case QScript::ScriptToken::EndIf:
        case QScript::ScriptToken::Global:
        case QScript::ScriptToken::RandomRange:
        case QScript::ScriptToken::Not:
            break;

        case QScript::ScriptToken::Pair:
            address += 8;
            break;

        case QScript::ScriptToken::Vector:
            address += 12;
            break;

        case QScript::ScriptToken::Random:
        case QScript::ScriptToken::RandomPermute:
        case QScript::ScriptToken::RandomNoRepeat:
            address += *(DWORD*)address * 4 + 4;
            break;

        default:
            //Should add all the unhandled opcodes to prevent crashing and other issues
            debug_print("%X @ %p\n", opcode, address);
            MessageBox(0, "Unhandled opcode...", __FUNCTION__, 0);
            break;
        }
        opcode = *address;
    }
}

void CScript::SetScript_Hook(DWORD checksum, CStruct* _params, Node* object)
{
    QBKeyHeader* header = GetQBKeyHeader(checksum);
    if (header)
    {
        if (header->type == QBKeyHeader::SCRIPTED_FUNCTION || header->type == QBKeyHeader::GLOBAL)
        {
            SetScript(checksum, _params, object);
            return;
        }
        else if (header->type == QBKeyHeader::COMPILED_FUNCTION)
        {
            auto param = params->head;
            for (; param; param = param->NextHeader)
            {
                MessageBox(0, FindChecksumName(param->Data, false), FindChecksumName(param->QBkey, false), 0);
                if (param->Type == QBKeyHeader::LOCAL_STRUCT)
                {
                    auto param2 = ((CStruct*)param->pStruct)->head;
                    for (; param2; param2 = param2->NextHeader)
                    {
                        MessageBox(0, FindChecksumName(param2->Data, false), FindChecksumName(param2->QBkey, false), 0);
                    }
                }
            }
            for (param = _params->head; param; param = param->NextHeader)
            {
                MessageBox(0, FindChecksumName(param->Data, false), FindChecksumName(param->QBkey, false), 0);
                if (param->Type == QBKeyHeader::LOCAL_STRUCT)
                {
                    auto param2 = ((CStruct*)param->pStruct)->head;
                    for (; param2; param2 = param2->NextHeader)
                    {
                        MessageBox(0, FindChecksumName(param2->Data, false), FindChecksumName(param2->QBkey, false), 0);
                    }
                }
            }
            //header->pFunction(param ? (CStruct*)param->pStruct : NULL, this);
            AdvanceToEnd();
        }
        else
            debug_print("GoTo: Invalid type %s\n", QScript::QBTypes[header->type]);
    }
    else
        debug_print("GoTo: Couldn't find QBKey %s\n", FindChecksumName(checksum));
    return;
}

void QScript::ClearScript(DWORD index)
{
    typedef void(__cdecl* const pClearScript)(DWORD index);
    pClearScript(0x004282A0)(index);
}

void CStruct::AddCompressedNode(DWORD checksum, QBKeyInfoContainer* container)
{

    for (unsigned int i = 0; i < compNodes.size(); i++)
    {
        if (compNodes[i].checksum == checksum)
        {
            if (!compNodes[i].added)
            {

                CStructHeader* header = compNodes[i].first;
                /*CStructHeader* pMap = *(CStructHeader**)0x008E1E04;

                compNodes[i].first = pMap->NextHeader;*/

                CStructHeader* added = this->AddCStruct(header->QBkey, header->Type, header->Data);
                compNodes[i].first = added;
                header = header->NextHeader;

                while (header)
                {

                    added = this->AddCStruct(header->QBkey, header->Type, header->Data);
                    header = header->NextHeader;
                }
                /*pMap->NextHeader = 0;*/
                compNodes[i].added = true;
                compNodes[i].last = added;
            }
            else
            {
                if (this->tail)
                    this->tail->NextHeader = compNodes[i].first;

                if (!this->head)
                    this->head = compNodes[i].first;
                this->tail = compNodes[i].last;

                if (container)
                {
                    CStructHeader* header = this->head;
                    while (header != this->tail && header)
                    {

                        debug_print("CONTAINER:.:..\n");
                        typedef DWORD(__cdecl* const pShiftQBKey)(DWORD qbKey);
                        DWORD shifted = pShiftQBKey(0x00426DB0)(header->Data);


                        typedef QBKeyInfo* (__thiscall* const pAddInfo)(QBKeyInfoContainer* pThis, DWORD qbKey);

                        QBKeyInfo* info = pAddInfo(0x00428A30)(container, shifted);

                        typedef void(__thiscall* const pAddContainer)(CStruct* pThis, QBKeyInfo* info);
                        pAddContainer(0x004292B0)(this, info);

                        header = header->NextHeader;
                    }
                }
            }
            debug_print("Added Comp Node: %X %X COMP %X\n", checksum, this, &compNodes[i]);


            break;
        }
    }

}

BYTE* CStruct::MaybeAddCompressed(DWORD qbKey, BYTE* pFile, QBKeyInfoContainer* container)
{
    typedef BYTE* (__thiscall* const pAddCStruct)(CStruct* pThis, DWORD qbKey, BYTE* pFile, QBKeyInfoContainer* other);
    if (qbKey != 0x68A9B4E1)
        return pAddCStruct(0x00428DF0)(this, qbKey, pFile, container);

    pFile++;
    DWORD checksum = *(DWORD*)pFile;
    pFile += 4;
    AddCompressedNode(checksum, container);
    return pFile;
}



EXTERN void CStruct::FreeHeader()
{
    CStructHeader* p = (CStructHeader*)this;

    while (p != NULL)
    {
        if (p->pData != NULL)
        {
            if (p->Type == 10 || p->Type == 11)
                ((CStruct*)p->pData)->FreeHeader();
            else if (p->Type == 12)
                ((CArray*)p->pData)->Free();
            free(p->pData);
        }
        p = p->NextHeader;
    }
}

EXTERN void CStruct::RemoveParam(DWORD name)
{
    CStructHeader* header = head;
    while (header != NULL)
    {
        if (header->QBkey == name)
        {
            DellocateCStruct(header);
            break;
        }
        header = header->NextHeader;
    }
}

EXTERN void CStruct::Free()
{
    if (head == NULL)
        return;

    CStructHeader* p = head;

    while (p != tail)
    {
        if (p->pData != NULL)
        {
            if (p->Type == 10 || p->Type == 11)
                ((CStruct*)p->pData)->FreeHeader();
            else if (p->Type == 12)
                ((CArray*)p->pData)->Free();
            free(p->pData);
        }
        p = p->NextHeader;
    }
}

EXTERN CStructHeader* CStruct::AddParam(const char* name, QBKeyHeader::QBKeyType type)
{
    if (head)
    {
        if (tail)
        {
            CStructHeader* param = AllocateCStruct();
            tail->NextHeader = param;
            tail = param;
            tail->Type = type;
            tail->SetName(name);
            tail->NextHeader = NULL;
        }
        else
        {
            CStructHeader* param = head->NextHeader;
            CStructHeader* prevparam = NULL;
            while (param)
            {
                prevparam = param;
                param = param->NextHeader;
            }
            if (prevparam)
            {
                prevparam->NextHeader = AllocateCStruct();
                param = prevparam->NextHeader;
            }
            tail = param;
            tail->Type = type;
            tail->SetName(name);
            tail->NextHeader = NULL;
        }
    }
    else
    {
        head = AllocateCStruct();
        tail = head;
        tail->Type = type;
        tail->SetName(name);
        tail->NextHeader = NULL;
    }
    return tail;
}

EXTERN DWORD CStructHeader::GetSize()
{
    DWORD size = 0;

    CStructHeader* header = this;
    while (header)
    {
        switch (header->Type)
        {
        case QBKeyHeader::INT:
            size += sizeof(CStructHeader);
            break;
        case QBKeyHeader::FLOAT:
            size += sizeof(CStructHeader);
            break;
        case QBKeyHeader::ARRAY:
            size += header->pArray->GetSize() + sizeof(CStructHeader);
            break;

        case QBKeyHeader::PAIR:
            size += 8 + sizeof(QBKeyHeader);
            break;

        case QBKeyHeader::LOCAL_STRUCT:
        case QBKeyHeader::STRUCT:
            size += (*(CStructHeader**)header->pStruct)->GetSize() + sizeof(QBKeyHeader);
            break;

        case QBKeyHeader::LOCAL:
            size += sizeof(CStructHeader);
            break;

        case QBKeyHeader::STRING:
        case QBKeyHeader::LOCAL_STRING:
            size += sizeof(CStructHeader) + strlen(header->pStr) + 1;
            break;

        case QBKeyHeader::COMPILED_FUNCTION:
            size += sizeof(CStructHeader);
            break;


        case QBKeyHeader::SCRIPTED_FUNCTION:
            size += sizeof(CStructHeader);
            break;

        case QBKeyHeader::VECTOR:
            size += sizeof(CStructHeader) + 12;
            break;

        default:
            size += sizeof(CStructHeader);
        }
        header = header->NextHeader;
    }
    return size;
}

bool CScript::AddCStruct(CStructHeader* pStruct, bool allocate)
{
    if (!pStruct)
        return false;

    for (auto param = params->head; param != NULL; param = param->NextHeader)
    {
        if (param->Data == pStruct->Data)
            return false;
    }

    auto param = allocate ? params->AllocateCStruct() : pStruct;
    param->Type = pStruct->Type;
    param->QBkey = pStruct->QBkey;
    param->Data = pStruct->Data;

    auto head = params->head;
    auto tail = params->tail;

    if (head)
    {
        if (tail)
        {
            tail->NextHeader = param;
            tail = param;
            tail->NextHeader = NULL;
        }
        else
        {
            auto currparam = head->NextHeader;
            CStructHeader* prevparam = NULL;
            while (currparam)
            {
                prevparam = currparam;
                currparam = param->NextHeader;
            }
            prevparam->NextHeader = param;
            param = prevparam->NextHeader;
            tail->NextHeader = param;
            tail = param;
            tail->NextHeader = NULL;
        }
    }
    else
    {
        head = param;
        tail = head;
        tail->NextHeader = NULL;
    }
    return true;
}

CStructHeader* CScript::GetParam(DWORD name)
{
    CStructHeader* header = params->head;

    while (header)
    {
        if (header->QBkey == name)
            return header;

        if (header->Type == QBKeyHeader::LOCAL_STRUCT || header->Type == QBKeyHeader::STRUCT)
        {
            CStructHeader* pStruct = *(CStructHeader**)header->pStruct;

            while (pStruct)
            {
                if (pStruct->QBkey == name)
                    return pStruct;
                else if (pStruct->Type == QBKeyHeader::LOCAL_STRUCT || pStruct->Type == QBKeyHeader::STRUCT)
                {
                    //Only 2 levels for now
                    CStructHeader* pStruct2 = *(CStructHeader**)pStruct->pStruct;
                    while (pStruct2)
                    {
                        if (pStruct2->QBkey == name)
                            return pStruct2;
                        pStruct2 = pStruct2->NextHeader;
                    }
                }
                pStruct = pStruct->NextHeader;
            }
        }
        else if (header->Type == QBKeyHeader::ARRAY)
        {
            debug_print("Parsing array...\n");
            CArray* pArray = header->pArray;

            for (int i = 0; i < pArray->GetNumItems(); i++)
            {
                if (pArray->Type == 0xA || pArray->Type == 0xB)
                {
                    //debug_print("Index %d of %d\n", i, pArray->GetNumItems());
                    CStructHeader* pStruct = pArray->GetCStruct(i);

                    while (pStruct)
                    {
                        debug_print("pStruct %p NextHeader %p\n", pStruct, pStruct->NextHeader);
                        debug_print("Name %s Data %X\n", FindChecksumName(pStruct->QBkey), pStruct->Data);
                        if (pStruct->QBkey == name)
                            return pStruct;
                        else if (pStruct->Type == QBKeyHeader::LOCAL_STRUCT || pStruct->Type == QBKeyHeader::STRUCT)
                        {
                            CStructHeader* pStruct2 = *(CStructHeader**)pStruct->pStruct;
                            while (pStruct2)
                            {
                                if (pStruct2->QBkey == name)
                                    return pStruct2;
                                pStruct2 = pStruct2->NextHeader;
                            }
                        }
                        pStruct = pStruct->NextHeader;
                    }
                }
            }
        }
        header = header->NextHeader;
    }
    return NULL;
}

bool CScript::GotParam(DWORD name)
{
    CStructHeader* header = params->head;

    while (header)
    {
        if (header->QBkey == name)
            return true;

        if (header->Type == QBKeyHeader::LOCAL_STRUCT || header->Type == QBKeyHeader::STRUCT)
        {
            CStructHeader* pStruct = *(CStructHeader**)header->pStruct;

            while (pStruct)
            {
                if (pStruct->QBkey == name)
                    return true;
                else if (pStruct->Type == QBKeyHeader::LOCAL_STRUCT || pStruct->Type == QBKeyHeader::STRUCT)
                {
                    //Only 2 levels for now
                    CStructHeader* pStruct2 = *(CStructHeader**)pStruct->pStruct;
                    while (pStruct2)
                    {
                        if (pStruct2->QBkey == name)
                            return true;
                        pStruct2 = pStruct2->NextHeader;
                    }
                }
                pStruct = pStruct->NextHeader;
            }
        }
        else if (header->Type == QBKeyHeader::ARRAY)
        {
            debug_print("Parsing array...\n");
            CArray* pArray = header->pArray;

            for (int i = 0; i < pArray->GetNumItems(); i++)
            {
                if (pArray->Type == 0xA || pArray->Type == 0xB)
                {
                    //debug_print("Index %d of %d\n", i, pArray->GetNumItems());
                    CStructHeader* pStruct = pArray->GetCStruct(i);

                    while (pStruct)
                    {
                        debug_print("pStruct %p NextHeader %p\n", pStruct, pStruct->NextHeader);
                        debug_print("Name %s Data %X\n", FindChecksumName(pStruct->QBkey), pStruct->Data);
                        if (pStruct->QBkey == name)
                            return true;
                        else if (pStruct->Type == QBKeyHeader::LOCAL_STRUCT || pStruct->Type == QBKeyHeader::STRUCT)
                        {
                            CStructHeader* pStruct2 = *(CStructHeader**)pStruct->pStruct;
                            while (pStruct2)
                            {
                                if (pStruct2->QBkey == name)
                                    return true;
                                pStruct2 = pStruct2->NextHeader;
                            }
                        }
                        pStruct = pStruct->NextHeader;
                    }
                }
            }
        }
        header = header->NextHeader;
    }
    return false;
}


EXTERN DWORD CScript::GetNodeName()
{
    return node->name;
}

void QBScript::CreateQBTable(BYTE* table, bool level)
{
    while (*table == ScriptToken::Table)
    {
        table++;
        int key = *(int*)table;
        table += 4;
        char* name = (char*)table;
        //debug_print("QbNAme %s\n", name);
        DWORD len = strlen(name) + 1;
        name = new char[len];
        if (!name)
            MessageBox(0, "nooo", "", 0);
        memcpy(name, (char*)table, len);


        //debug_print("QbAllocated %s\n", name);
        map<DWORD, char*>::iterator it = qbTable.find(key);

        if (it == qbTable.end())
        {
            /*FILE* debugFile = fopen("debug.txt", "r+t");
            fseek(debugFile, 0, SEEK_END);
            fprintf(debugFile, "AddingKey %s %X, in file %s\r\n", name, key, fileName);
            printf("AddingKey %s %X, in file %s\r\n", name, key, fileName);
            fclose(debugFile);*/
            //debug_print("AddChecksum %s 0x%X\n", name, key);
            if (!level)
            {
                qbTable.insert(pair<DWORD, char*>(key, String::AddString(name)));
                qbKeys.push_back(key);
            }
            else
            {
                it = levelTable.find(key);
                if (it == levelTable.end())
                {
                    levelTable.insert(pair<DWORD, char*>(key, String::AddLevelString(name)));
                    qbKeys.push_back(key);
                }
            }
        }
        /*else if (__stricmp(it->second, name))
        {
            FILE* debugFile = fopen("
            ", "r+t");
            fseek(debugFile, 0, SEEK_END);
            fprintf(debugFile, "found collision %s %s\r\n", it->second, name);
            fclose(debugFile);

        }*/
        //debug_print("inserted\n");
        table += len;
        //debug_print("added\n");
    }
}

char* QScript::GetScriptDir(bool second)
{
    char* qdir = (char*)0x5BBAF8;
    if (!second)
    {
        static char dir[256] = "data\\";
        DWORD len = strlen(qdir) + 1;
        memcpy(&dir[5], qdir, len);

        return dir;
    }

        static char dir2[256] = ".\\data\\";
        DWORD len = strlen(qdir) + 1;
        memcpy(&dir2[5], qdir, len);

        return dir2;
}

void QScript::CallCFunction(DWORD checksum, void* param, QBKeyHeader::QBKeyType type)
{
    auto pFunctionHeader = GetQBKeyHeader(checksum);
    if (pFunctionHeader)
    {
        CScript script;
        CStruct params;
        CStructHeader header(type, 0, param);
        params.Set(&header);
        pFunctionHeader->pFunction(&params, &script);
    }
}

void QScript::CallCFunction(DWORD checksum, CStruct* pParams)
{
    auto pFunctionHeader = GetQBKeyHeader(checksum);
    if (pFunctionHeader)
    {
        CScript script;
        pFunctionHeader->pFunction(pParams, &script);
    }
}


char* QScript::FindReference(char* file_name, DWORD checksum, bool levelmod)
{
    static char reference[528];
    FILE* f = fopen(file_name, "r+b");
    file_name[strlen(file_name) - 1] = 0x0;
    if (f)
    {
        fseek(f, 0, SEEK_END);
        DWORD size = ftell(f);

        fseek(f, 0, SEEK_SET);
        BYTE* pFile = new BYTE[size + 1];
        fread(pFile, size, 1, f);
        fclose(f);
        BYTE* oldData = pFile;


        DWORD pos = std::string(file_name).find_last_of('\\');
        DWORD line = 1;
        while (true)
        {
            switch (*pFile)
            {
            case ScriptToken::NewLine:
                pFile++;
                line++;
                break;
            case ScriptToken::NewLineNumber:
                pFile += 5;
                line++;
                break;
            case ScriptToken::Int:
            case ScriptToken::Float:
            case 0x2E:
                pFile += 5;
                break;

            case ScriptToken::QBKey:
                pFile++;
                if (*(DWORD*)pFile == checksum)
                {
                    if (levelmod)
                        sprintf(reference, "[%s](https://github.com/Vadru93/LevelMod/blob/master/LevelMod/Data/Scripts/LevelMod/%s#L%u) ", &file_name[pos + 1], &file_name[pos +1], line);
                    else
                        sprintf(reference, "%sb Line %u ", &file_name[pos + 1], line);
                    delete[] oldData;
                    return reference;
                }
                pFile += 4;
                break;

            case ScriptToken::String:
            case ScriptToken::LocalString:
                pFile++;
                pFile += *(int*)pFile + 4;
                break;

            case ScriptToken::Vector:
                pFile += 13;
                break;

            case ScriptToken::Pair:
                pFile += 9;
                break;

            case ScriptToken::EndOfFile:
            case ScriptToken::Table:
                delete[] oldData;
                return NULL;

            case 0x47:
            case 0x48:
            case 0x49:
                pFile += 3;
                break;

            default:
                //debug_print("default: %X\n", *pFile);
                /*char def[2];
                sprintf(def, "%X", *pFile);
                MessageBox(0,"default", def, 0);*/
                pFile++;
                break;

            case 0x2F:
            case 0x37:
            case 0x40:
            case 0x41:
                pFile++;
                int count = *(int*)pFile;
                pFile += 4;
                /*if (*(int*)pFile == 65537 || (count == 1 && *(WORD*)pFile == 1))
                {
                    pFile += 2 * count;
                }*/
                pFile += count * 4;
                break;
            }
        }
        delete[] oldData;
    }
    return NULL;
}
char* QScript::FindReference(DWORD checksum)
{
    static char references[MAX_PATH*500] = "";
    sprintf(references, "");

    char* dir = GetScriptDir();
    FILE* f = fopen(dir, "rb+");


    fseek(f, 0, SEEK_END);
    DWORD size = ftell(f);

    fseek(f, 0, SEEK_SET);
    BYTE* pFile = new BYTE[size + 1];
    fread(pFile, size, 1, f);
    fclose(f);
    BYTE* oldData = pFile;

    //MessageBox(0, dir, "going to parse", 0);
    char* file;
    while (pFile < (oldData + size - 5))
    {
        file = NULL;
        unsigned int i = 13;
        while (pFile < (oldData + size) && *pFile != 0x0A && *pFile != 0x0D)
        {
            if (!file)
                file = (char*)pFile;
            dir[i] = *pFile;
            dir[i + 1] = 0;
            i++;
            pFile++;
        }

        dir[i] = 0;

        if (file)
        {
            char* ref = FindReference(dir, checksum, strstr(file, "LevelMod"));
            if (ref)
                strcat(references, ref);
        }
        pFile++;
        while (pFile < (oldData + size) && (*pFile == 0x0D || *pFile == 0x0A))
            pFile++;
    }
    delete[] oldData;
    if (strlen(references))
        return references;
    else
    {
        sprintf(references, "None");
        return references;
    }
}

void QBScript::AddScripts()
{
#ifdef _DEBUG
    FILE* fd = fopen("Script functions.txt", "wb");
#endif
    char* dir = GetScriptDir();
    FILE* f = fopen(dir, "rb+");


    fseek(f, 0, SEEK_END);
    DWORD size = ftell(f);

    fseek(f, 0, SEEK_SET);
    BYTE* pFile = new BYTE[size + 1];
    fread(pFile, size, 1, f);
    fclose(f);
    BYTE* oldData = pFile;

    //MessageBox(0, dir, "going to parse", 0);
    while (pFile < (oldData + size - 5))
    {
        unsigned int i = 13;
        while (pFile < (oldData + size) && *pFile != 0x0A && *pFile != 0x0D)
        {
            dir[i] = *pFile;
            dir[i + 1] = 0;
            i++;
            pFile++;
        }

        dir[i] = 0;


        OpenScript(dir);
        pFile++;
        while (pFile < (oldData + size) && (*pFile == 0x0D || *pFile == 0x0A))
            pFile++;

#ifdef _DEBUG
        if (fd)
        {
            for (DWORD i = 0; i < scripts.size(); i++)
            {
                fprintf(fd, "Script %X File %s\n", scripts[i], dir);
            }
        }
#endif
    }
    delete[] oldData;

#ifdef _DEBUG
    fclose(fd);
#endif
}

char* QBScript::GetQBKeyName(int checksum)
{
    if (qbTable.size())
    {
        std::map<DWORD, char*>::iterator it;

        it = qbTable.find(checksum);

        if (it != qbTable.end())
            return it->second;

        it = levelTable.find(checksum);

        if (it != levelTable.end())
            return it->second;
        /*else
            printf("couldn't find QBKey %X\n", checksum);*/
    }
    /*else
        printf("couldn't find QBKey %X\n", checksum);*/
    return NULL;
}

bool CheckForMatch_stub(char* file1, char*& file2)
{
    while (*file2 != 0x0D && *file2 != 0x0A && *file2 != 0xFF)
    {
        if (*file1 != *file2)
        {
            while (*file2 != 0x0A && *file2 != 0x0D)
            {
                file2++;
            }

            return false;
        }
        file1++;
        file2++;
    }

    return true;
}

char* CheckForMatch(char* fileName, char* qbFiles)
{

    while (*qbFiles != 0xFF)
    {
        if (CheckForMatch_stub(fileName, qbFiles))
        {
            //debug_print("FileExists: %s\n", fileName);
            return qbFiles;
        }

        while (*qbFiles == 0x0A || *qbFiles == 0x0D)
        {
            qbFiles++;
        }
    }

    debug_print("New File: %s\n", fileName);
    return NULL;
}

bool QBFile::ContentChanged(bool level)
{
    char dir[256];
    if (!level)
    {
        strcpy(dir, GetScriptDir());

        unsigned int i = 5;

        /*char* path = new char[strlen(dir) + strlen(fileName) + 1];
        strcpy(path, dir);*/
        strcpy(&dir[i], fileName);
    }
    else
        strcpy(dir, fileName);

    FILE* f = fopen(dir, "rb+");

    fseek(f, 0, SEEK_END);
    DWORD newSize = ftell(f);
    fclose(f);

    unsigned long crc;
    if (newSize != size)
    {

        //checksum = FastCRC::CFastCRC32::Calculate(&crc, dir);
        if (FileHandler::CalculateCRC(crc, dir) == false)
            MessageBox(0, "Error calculating checksum for file", fileName, 0);
        checksum = crc;
        return true;

    }
    //debug_print("%s %s\n", path, fileName);

    /*if (FastCRC::CFastCRC32::Calculate(&crc, dir) != 0)
        MessageBox(0, "Error calculating checksum for file", dir, 0);*/
    if (FileHandler::CalculateCRC(crc, dir) == false)
        MessageBox(0, "Error calculating checksum for file", fileName, 0);

    //delete[] path;

    bool changed = crc != checksum;
    if (changed)
    {
        checksum = crc;
        debug_print("Contents changed in file: %s\n", fileName);
    }

    return changed;
}


void QBScript::OpenScript(char* path, bool level)
{
    if (FileHandler::reloading)
        level = FileHandler::reloading == RELOAD_LEVEL;
    fileName = path;
    debug_print("OpenScript: %s\n", path);
    FILE* f = fopen(path, "rb+");
    if (!f)
    {
        MessageBox(0, "error opening script...", path, 0);
        return;
    }
    fseek(f, 0, SEEK_END);
    DWORD size = ftell(f);

    fseek(f, 0, SEEK_SET);
    BYTE* pFile = new BYTE[size + 1];
    fread(pFile, size, 1, f);
    fclose(f);

    //data = pFile;
    BYTE* oldData = pFile;

    /*char* match = CheckForMatch(fileName, *(char**)0x008A8B48);
    if (match)
    {*/
    //MessageBox(0, 0, 0, 0);
    if (!level)
    {
        unsigned long checksum;
        /*if (FastCRC::CFastCRC32::Calculate(&checksum, path) != 0)
            MessageBox(0, "Error calculating checksum for file", fileName, 0);*/
        if (FileHandler::CalculateCRC(checksum, path) == false)
            MessageBox(0, "Error calculating checksum for file", fileName, 0);

        QBFile qbFile = QBFile(checksum, fileName, size);
        bool found = false;
        for (DWORD i = 0; i < qbFiles.size(); i++)
        {
            if (_stricmp(qbFiles[i].fileName, qbFile.fileName))
            {
                qbFiles[i].checksum = checksum;
                qbFiles[i].size = size;
                found = true;
                break;
            }
        }
        if(!found)
            qbFiles.push_back(qbFile);
    }
    else
    {
        strcpy(levelPath, path);
        DWORD checksum;
        if (FileHandler::CalculateCRC(checksum, path) == false)
            MessageBox(0, "Error calculating checksum for file", fileName, 0);
        levelQB = QBFile(checksum, fileName, size, true);

        if (!FileHandler::reloading)
        {
            if (dwLevelChangeHandles[1] != NULL)
            {
                SetEvent(dwLevelChangeHandles[1]);
                while (dwLevelChangeHandles[1] != NULL)
                {
                    Sleep(10);
                }
            }
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckForNodeArrayUpdates, 0, 0, 0);
        }
    }
    /*}
    else
        MessageBox(0, "couldn't find file in list", fileName, 0);*/


    while (true)
    {
        switch (*pFile)
        {
        case ScriptToken::NewLineNumber:
        case ScriptToken::QBKey:
        case ScriptToken::Int:
        case ScriptToken::Float:
        case 0x2E:
            pFile += 5;
            break;

        case ScriptToken::String:
        case ScriptToken::LocalString:
            pFile++;
            pFile += *(int*)pFile + 4;
            break;

        case ScriptToken::Vector:
            pFile += 13;
            break;

        case ScriptToken::Pair:
            pFile += 9;
            break;

        case ScriptToken::EndOfFile:
        case ScriptToken::Table:
            debug_print("parsing qbTable %p\n", pFile);
            goto done;

        case 0x47:
        case 0x48:
        case 0x49:
            pFile += 3;
            break;

        default:
            //debug_print("default: %X\n", *pFile);
            /*char def[2];
            sprintf(def, "%X", *pFile);
            MessageBox(0,"default", def, 0);*/
            pFile++;
            break;
#ifdef _DEBUG
        case 0x23:
            pFile += 2;
            scripts.push_back(*(DWORD*)pFile);
            pFile += 4;
            break;
#endif

        case 0x2F:
        case 0x37:
        case 0x40:
        case 0x41:
            pFile++;
            int count = *(int*)pFile;
            pFile += 4;
            /*if (*(int*)pFile == 65537 || (count == 1 && *(WORD*)pFile == 1))
            {
                pFile += 2 * count;
            }*/
            pFile += count * 4;
            break;
        }
    }
done:
    CreateQBTable(pFile, level);
    debug_print("END OpenScript\n");
    delete[] oldData;
    if (FileHandler::reloading == RELOAD_LEVEL)
    {
        SpawnScript(Checksum("UnPauseSkaters_Script"));
    }
    FileHandler::reloading = false;
}

void QBScript::ClearMap()
{
    /*map<int, char*>::iterator end = qbTable.end();
    for (map<int, char*>::iterator it = qbTable.begin(); it != end; it++)
    {
        delete[] it->second;
    }*/
    qbTable.clear();
}

bool QScript::FileExists(char* file)
{
    for (DWORD i = 0; i < qbFiles.size(); i++)
    {
        if (!_stricmp(qbFiles[i].fileName, file))
            return true;
    }
    debug_print("New File %s\n", file);
    return false;
}

void CheckForNodeArrayUpdates()
{
    TCHAR installPath[MAX_PATH + 1];
    GetCurrentDirectory(MAX_PATH, installPath);

    DWORD pos = std::string(levelPath).find_last_of("\\");
    DWORD j = strlen(installPath);

    for (DWORD i = 0; i < pos; i++)
    {
        installPath[j] = levelPath[i];
        j++;
    }
    installPath[j] = 0x0;

    debug_print("New Level Path %s qb %s\n", installPath, levelPath);


    DWORD dwWaitStatus;
    
    TCHAR lpDrive[4];
    TCHAR lpFile[_MAX_FNAME];
    TCHAR lpExt[_MAX_EXT];

    _tsplitpath_s(installPath, lpDrive, 4, NULL, 0, lpFile, _MAX_FNAME, lpExt, _MAX_EXT);

    dwLevelChangeHandles[0] = FindFirstChangeNotification(
        installPath,                         // directory to watch 
        FALSE,                         // don't watch subtrees
        FILE_NOTIFY_CHANGE_LAST_WRITE); // watch file write changes 

    dwLevelChangeHandles[1] = CreateEvent(
        NULL,               // default security attributes
        TRUE,               // manual-reset event
        FALSE,              // initial state is nonsignaled
        TEXT("ChangeLevel")  // object name
    );

    if (dwLevelChangeHandles[0] == NULL || dwLevelChangeHandles[1] == NULL)
    {
        MessageBox(0, "Error trying checking for level script changes", "Error trying checking for level script changes", 0);
        return;
    }


    while (TRUE)
    {
        dwWaitStatus = WaitForMultipleObjects(2, dwLevelChangeHandles,
            FALSE, INFINITE);

        switch (dwWaitStatus)
        {
        case WAIT_OBJECT_0:
            if (levelQB.ContentChanged(true))
            {
                typedef void(__cdecl* const pReloadNodeArray)(CStruct*, CScript*);
                debug_print("Reloading node array %s..", levelQB.fileName);
                //pReloadNodeArray(0x00419D50)(NULL, NULL);
                FileHandler::reloading = RELOAD_LEVEL;
                SpawnScript(Checksum("ReloadNodeArray_Script"));
            }
            if (FindNextChangeNotification(dwLevelChangeHandles[0]) == FALSE)
            {
                MessageBox(0, "Error trying checking for level script changes", "Error trying checking for level script changes", 0);
                return;
            }
            

            break;

        case WAIT_OBJECT_0+1:
            debug_print("ChangeLevel event\n");
            CloseHandle(dwLevelChangeHandles[0]);
            CloseHandle(dwLevelChangeHandles[1]);
            dwLevelChangeHandles[1] = NULL;
            return;
        case WAIT_TIMEOUT:
            break;
        case WAIT_FAILED:
            break;
        }
    }
}

void CheckForScriptUpdates()
{
    TCHAR installPath[MAX_PATH + 1];
    GetCurrentDirectory(MAX_PATH, installPath);

    strcat(installPath, "\\Data\\Scripts");
    DWORD dwWaitStatus;
    HANDLE dwChangeHandle;
    TCHAR lpDrive[4];
    TCHAR lpFile[_MAX_FNAME];
    TCHAR lpExt[_MAX_EXT];

    _tsplitpath_s(installPath, lpDrive, 4, NULL, 0, lpFile, _MAX_FNAME, lpExt, _MAX_EXT);

    dwChangeHandle = FindFirstChangeNotification(
        installPath,                         // directory to watch 
        TRUE,                         // watch subtrees
        FILE_NOTIFY_CHANGE_LAST_WRITE); // watch file write changes 

    if (dwChangeHandle == NULL)
    {
        MessageBox(0, "Error trying checking for script changes", "Error trying checking for script changes", 0);
        return;
    }

    while (TRUE)
    {
        dwWaitStatus = WaitForSingleObject(dwChangeHandle, INFINITE);

        switch (dwWaitStatus)
        {
        case WAIT_OBJECT_0:
            TestReloadQB(NULL, NULL);
            if (FindNextChangeNotification(dwChangeHandle) == FALSE)
            {
                MessageBox(0, "Error trying checking for script changes", "Error trying checking for script changes", 0);
                return;
            }
            break;
        case WAIT_TIMEOUT:
            break;
        case WAIT_FAILED:
            break;
        }
    }
}

void UpdateScriptFloat(float& value, const char* name)
{
    QBKeyHeader* header = GetQBKeyHeader(Checksum(name));
    if (header)
    {
        if (header->type == QBKeyHeader::FLOAT)
            value = header->GetFloat();
        else if (header->type == QBKeyHeader::INT)
            value = header->GetInt();
    }
    else
        MessageBox(0, name, "Couldn't find", 0);
}

void UpdateScriptConstants()
{

    UpdateScriptFloat(Gfx::uv_anim_threshold, "uv_anim_threshold");
    UpdateScriptFloat(Gfx::uv_tiling_threshold, "uv_tiling_threshold");
    UpdateScriptFloat(Gfx::shatter_speed, "shatter_speed");
    UpdateScriptFloat(Gfx::shatter_life_factor, "shatter_life_factor");
    UpdateScriptFloat(Gfx::shatter_gravity, "shatter_gravity");
    UpdateScriptFloat(Physics::Rail_Max_Snap, "Rail_Max_Snap");
    UpdateScriptFloat(Physics::Rail_Corner_Leave_Angle, "Rail_Corner_Leave_Angle");
}

bool TestReloadQB(CStruct* pStruct, CScript* pScript)
{

    typedef void(__cdecl* const pLoadQB)(char* fileName, bool unknown);

    for (DWORD i = 0; i < qbFiles.size(); i++)
    {
        if (qbFiles[i].ContentChanged())
        {
            FileHandler::reloading = RELOAD_SCRIPT;
            pLoadQB(0x0042B300)(qbFiles[i].fileName, false);
        }
    }
    char* qbFiles = *(char**)0x008A8B48;


    char* dir = GetScriptDir();
    FILE* f = fopen(dir, "rb+");


    fseek(f, 0, SEEK_END);
    DWORD size = ftell(f);

    fseek(f, 0, SEEK_SET);
    BYTE* pFile = new BYTE[size + 1];
    fread(pFile, size, 1, f);
    fclose(f);
    BYTE* oldData = pFile;

    //MessageBox(0, dir, "going to parse", 0);
    while (pFile < (oldData + size - 5))
    {
        unsigned int i = 13;
        while (pFile < (oldData + size) && *pFile != 0x0A && *pFile != 0x0D)
        {
            dir[i] = *pFile;
            dir[i + 1] = 0;
            i++;
            pFile++;
        }

        dir[i] = 0;
        //debug_print("CheckingFile: %s\n", &dir[5]);

        if (!FileExists(&dir[5]))
        {
            unsigned long checksum;
            /*if (FastCRC::CFastCRC32::Calculate(&checksum, dir) != 0)
                MessageBox(0, "Error calculating checksum for file", &dir[13], 0);
            */
            if (FileHandler::CalculateCRC(checksum, dir) == false)
                MessageBox(0, "Error calculating checksum for file", &dir[13], 0);

            QScript::qbFiles.push_back(QBFile(checksum, dir, size));

            pLoadQB(0x0042B300)(&dir[5], false);
        }

        pFile++;
        while (pFile < (oldData + size) && (*pFile == 0x0D || *pFile == 0x0A))
            pFile++;
    }
    delete[] oldData;
    UpdateScriptConstants();

    return true;
}