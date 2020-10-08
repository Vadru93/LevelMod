#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Defines.h"
#include "Node.h"
#include "String.h"
#include "CustomShaders.h"
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
//#include "FastCRC.h"

namespace FileHandler
{
    bool reloading = false;

    BYTE* pFile = NULL;
    const BYTE* oldFile = NULL;
    DWORD size = 0;
    bool OpenFile(char* path, char* mode)
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

    bool CalculateCRC(DWORD& checksum, char* path)
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

char* QScript::QBTypes[] = {
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
     "Unknown",
     "Unknown",
     "Unknown",
     "Unknown",
     "Unknown"
};
QScript::QBScript* QScript::Scripts;
std::vector<QScript::CompressedNode> QScript::compNodes;
std::vector<DWORD> QScript::qbKeys;
std::vector<QScript::QBFile> QScript::qbFiles;

using namespace std;
using namespace QScript;

void CheckForNodeArrayUpdates();

HANDLE dwLevelChangeHandles[2] = { NULL, NULL };
char levelPath[MAX_PATH];

QBFile levelQB;


void QScript::SpawnScript(DWORD checksum, CStruct* params, DWORD node, DWORD callback, CStruct* callback_params, bool AbsentInNetGames, bool NetEnabled, bool Permanent)
{
    typedef DWORD(__cdecl* const pSpawnScript)(DWORD checksum, CStruct* params, DWORD callback, CStruct* callback_params, DWORD node, bool AbsentInNetGames, bool NetEnabled, bool Permanent);
    pSpawnScript(0x00428510)(checksum, params, callback, callback_params, node, AbsentInNetGames, NetEnabled, Permanent);
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

                        _printf("CONTAINER:.:..\n");
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
            _printf("Added Comp Node: %X %X COMP %X\n", checksum, this, &compNodes[i]);


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
            prevparam->NextHeader = AllocateCStruct();
            param = prevparam->NextHeader;
            tail->NextHeader = param;
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
            _printf("Parsing array...\n");
            CArray* pArray = header->pArray;

            for (int i = 0; i < pArray->GetNumItems(); i++)
            {
                if (pArray->Type == 0xA || pArray->Type == 0xB)
                {
                    //_printf("Index %d of %d\n", i, pArray->GetNumItems());
                    CStructHeader* pStruct = pArray->GetCStruct(i);

                    while (pStruct)
                    {
                        _printf("pStruct %p NextHeader %p\n", pStruct, pStruct->NextHeader);
                        _printf("Name %s Data %X\n", FindChecksumName(pStruct->QBkey), pStruct->Data);
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
            _printf("Parsing array...\n");
            CArray* pArray = header->pArray;

            for (int i = 0; i < pArray->GetNumItems(); i++)
            {
                if (pArray->Type == 0xA || pArray->Type == 0xB)
                {
                    //_printf("Index %d of %d\n", i, pArray->GetNumItems());
                    CStructHeader* pStruct = pArray->GetCStruct(i);

                    while (pStruct)
                    {
                        _printf("pStruct %p NextHeader %p\n", pStruct, pStruct->NextHeader);
                        _printf("Name %s Data %X\n", FindChecksumName(pStruct->QBkey), pStruct->Data);
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
        //_printf("QbNAme %s\n", name);
        DWORD len = strlen(name) + 1;
        name = new char[len];
        if (!name)
            MessageBox(0, "nooo", "", 0);
        memcpy(name, (char*)table, len);

        //_printf("QbAllocated %s\n", name);
        map<DWORD, char*>::iterator it = qbTable.find(key);

        if (it == qbTable.end())
        {
            /*FILE* debugFile = fopen("debug.txt", "r+t");
            fseek(debugFile, 0, SEEK_END);
            fprintf(debugFile, "AddingKey %s %X, in file %s\r\n", name, key, fileName);
            printf("AddingKey %s %X, in file %s\r\n", name, key, fileName);
            fclose(debugFile);*/
            //_printf("AddChecksum %s 0x%X\n", name, key);
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
        /*else if (_stricmp(it->second, name))
        {
            FILE* debugFile = fopen("debug.txt", "r+t");
            fseek(debugFile, 0, SEEK_END);
            fprintf(debugFile, "found collision %s %s\r\n", it->second, name);
            fclose(debugFile);

        }*/
        //_printf("inserted\n");
        table += len;
        //_printf("added\n");
    }
}

char* QScript::GetScriptDir()
{
    char* qdir = (char*)0x5BBAF8;
    static char dir[256] = "data\\";
    DWORD len = strlen(qdir) + 1;
    memcpy(&dir[5], qdir, len);

    return dir;
}

void QBScript::AddScripts()
{
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
    }
    delete[] oldData;
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

#include <fstream>
#include <iostream>

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
            //_printf("FileExists: %s\n", fileName);
            return qbFiles;
        }

        while (*qbFiles == 0x0A || *qbFiles == 0x0D)
        {
            qbFiles++;
        }
    }

    _printf("New File: %s\n", fileName);
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
    //_printf("%s %s\n", path, fileName);

    /*if (FastCRC::CFastCRC32::Calculate(&crc, dir) != 0)
        MessageBox(0, "Error calculating checksum for file", dir, 0);*/
    if (FileHandler::CalculateCRC(crc, dir) == false)
        MessageBox(0, "Error calculating checksum for file", fileName, 0);

    //delete[] path;

    bool changed = crc != checksum;
    if (changed)
    {
        checksum = crc;
        _printf("Contents changed in file: %s\n", fileName);
    }

    return changed;
}


void QBScript::OpenScript(char* path, bool level)
{
    if (FileHandler::reloading)
        level = true;
    fileName = path;
    _printf("OpenScript: %s\n", path);//MessageBox(0, "OpenScript", path, 0);
    FILE* f = fopen(path, "rb+");

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

        qbFiles.push_back(QBFile(checksum, fileName, size));
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
            _printf("parsing qbTable %X\n", pFile);
            goto done;

        case 0x47:
        case 0x48:
        case 0x49:
            pFile += 3;
            break;

        default:
            //_printf("default: %X\n", *pFile);
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
done:
    CreateQBTable(pFile, level);
    _printf("END OpenScript\n");
    delete[] oldData;
    if (FileHandler::reloading)
    {
        SpawnScript(Checksum("UnPauseSkaters_Script"));
        FileHandler::reloading = false;
    }
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
        if (!stricmp(qbFiles[i].fileName, file))
            return true;
    }
    _printf("New File %s\n", file);
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

    _printf("New Level Path %s qb %s\n", installPath, levelPath);


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
                _printf("Reloading node array %s..", levelQB.fileName);
                //pReloadNodeArray(0x00419D50)(NULL, NULL);
                FileHandler::reloading = true;
                SpawnScript(Checksum("ReloadNodeArray_Script"));
            }
            if (FindNextChangeNotification(dwLevelChangeHandles[0]) == FALSE)
            {
                MessageBox(0, "Error trying checking for level script changes", "Error trying checking for level script changes", 0);
                return;
            }
            

            break;

        case WAIT_OBJECT_0+1:
            _printf("ChangeLevel event\n");
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

bool TestReloadQB(CStruct* pStruct, CScript* pScript)
{

    typedef void(__cdecl* const pLoadQB)(char* fileName, bool unknown);

    for (DWORD i = 0; i < qbFiles.size(); i++)
    {
        if (qbFiles[i].ContentChanged())
            pLoadQB(0x0042B300)(qbFiles[i].fileName, false);
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
        //_printf("CheckingFile: %s\n", &dir[5]);

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

    QBKeyHeader* header = GetQBKeyHeader(Checksum("uv_anim_threshold"));
    if (header)
    {
        Gfx::uv_anim_threshold = header->value.f;
    }
    else
        MessageBox(0, "couldn't find uv_anim_threshold", "", 0);

    header = GetQBKeyHeader(Checksum("uv_tiling_threshold"));
    if (header)
    {
        Gfx::uv_tiling_threshold = header->value.f;
    }
    else
        MessageBox(0, "couldn't find uv_tiling_threshold", "", 0);

    return true;
}