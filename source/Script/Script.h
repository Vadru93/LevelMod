#pragma once
#ifndef SCRIPT_H
#define SCRIPT_H
#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Extension\Defines.h"
#include "QBKey.h"



struct CStruct;
struct CStructHeader;
struct CArray;
struct CScript;
//struct QBKeyHeader;


void EXTERN SetStructValues(CStructHeader* pStruct, CStructHeader* values);
void EXTERN SetArrayValues(CArray* pArray, CStructHeader* values);

void CheckForScriptUpdates();

bool TestReloadQB(CStruct* pStruct, CScript* pScript);

EXTERN char* FindChecksumName(DWORD checksum, bool only_debug = true);

void UpdateScriptConstants();


//inline unsigned long Checksum(const char* string);//THPS3 Function for CRC32
struct Node;

    //calls a QB scripted function, remember to pass a CStruct.
typedef void(__cdecl* const ExecuteScript)(const char* __restrict name, const CStruct* __restrict pParams, const Node* __restrict pObject, bool assert);
__declspec(noalias) void inline ExecuteQBScript(const char* __restrict pScriptName, const CStruct* __restrict pParams = NULL, const Node* __restrict pObject = NULL, bool assert = false)
{
    return ExecuteScript(0x00428240)(pScriptName, pParams, pObject, assert);
}
typedef void(__cdecl* const ExecuteScript_Stub)(DWORD checksum, const CStruct* __restrict pParams, const Node* __restrict pObject, bool assert);
__declspec(noalias) void inline ExecuteQBScript(DWORD checksum, const CStruct* __restrict pParams = NULL, const Node* __restrict pObject = NULL, bool assert = false)
{
    return ExecuteScript_Stub(0x00428180)(checksum, pParams, pObject, assert);
}


//--------Script Stuff--------
namespace QScript
{
    extern bool(*GotParam)(CStruct*, CScript*);
    extern bool(*ResetClock)(CStruct*, CScript*);
    extern bool (*ShatterScript)(CStruct*, CScript*);
    extern bool (*LaunchPanelMessage)(CStruct*, CScript*);
    extern bool reload_qb;
    auto constexpr ASSERT = 1;
    enum ScriptToken
    {
        EndOfFile = 0x00,
        NewLine = 0x1,
        NewLineNumber = 0x2,
        Struct = 0x3,
        EndStruct = 0x4,
        Array = 0x5,
        EndArray = 0x6,
        Equals = 0x7,
        Property = 0x8,
        Comma = 0x9,
        Parenthesis = 0x0E,
        EndParenthesis = 0x0F,
        QBKey = 0x16,
        Int = 0x17,
        Float = 0x1A,
        String = 0x1B,
        LocalString = 0x1C,
        Vector = 0x1E,
        Pair = 0x1F,
        Begin = 0x20,
        Repeat = 0x21,
        Break = 0x22,
        Script = 0x23,
        EndScript = 0x24,
        If = 0x25,
        Else = 0x26,
        ElseIf = 0x27,
        EndIf = 0x28,
        Table = 0x2B,
        Global = 0x2D,
        Jump = 0x2E,
        Random = 0x2F,
        RandomRange = 0x30,
        RandomPermute = 0x37,
        Not = 0x39,
        RandomNoRepeat = 0x40,
        If2 = 0x47,
        Else2 = 0x48,
        EndSwitch2 = 0x49,

    };

    struct SpawnedScript
    {
        CScript* script;
        DWORD checksum;//not sure?
        DWORD callback;
        CStruct* params;
        bool  paused;
    };

    float GetPhysicsFloat(DWORD name);

    void ClearScript(DWORD index);

    char* GetScriptDir(bool second = false);

    struct CompressedNode
    {
        DWORD checksum;
        CStructHeader* first;
        CStructHeader* last;
        bool added;


        CompressedNode()
        {
            checksum = 0;
            first = NULL;
            last = NULL;
            added = false;
        }
    };

    char* FindReference(DWORD checksum);
    char* FindReference(char* file_name, DWORD checksumm, bool levelmod);
    //I'm not sure about the AbsentInNetGames, but the function calling this function did AbsentInNetGames = !NetEnabled, could be something else...
    void SpawnScript(DWORD checksum, CStruct* params = NULL, DWORD node = 0xFFFFFFFF, DWORD callback = 0, CStruct* callback_params = NULL, bool AbsentInNetGames = false, bool NetEnabled = true, bool Permanent = true);

    void PanelMessage(const char* msg, ...);

    void SetMenuElementText(DWORD id, const char* text);

    struct QBFile
    {
        DWORD checksum;
        DWORD size;
        char fileName[80];

        QBFile()
        {
            ZeroMemory(this, sizeof(QBFile));
        }

        QBFile(DWORD chc, char* file, DWORD fileSize, bool level = false)
        {
            checksum = chc;
            size = fileSize;
            if (!level)
            {
                if (*file == '.')
                    strcpy(fileName, &file[7]);
                else
                    strcpy(fileName, &file[5]);
            }
            else
                strcpy(fileName, file);

            debug_print("[%d]QBFile: %s crc %X size %X\n", level, fileName, checksum, size);
        }

        bool ContentChanged(bool level = false);
    };

    extern std::vector<CompressedNode> compNodes;
    extern std::vector<DWORD> qbKeys;
    extern std::vector<QBFile> qbFiles;

    bool FileExists(char* file);

    struct QBScript
    {
        char* fileName;
        std::map<DWORD, char*> qbTable;
        std::map<DWORD, char*> levelTable;
#ifdef _DEBUG
        std::vector<DWORD> scripts;
#endif

        void CreateQBTable(BYTE* table, bool level = false);

        QBScript(bool nothing)
        {
            fileName = NULL;
        }

        void ClearLevelTable()
        {
            levelTable.clear();
        }

        void AddScripts();

        void ClearMap();

        QBScript()
        {
            MessageBox(0, "Going to add", "", 0);
            AddScripts();
        }
        char unkString[256] = "";

        char* GetQBKeyName(int checksum);

        void OpenScript(char* path, bool level = false);

        ~QBScript()
        {
            ClearMap();
        }

    };

    extern void CallCFunction(DWORD checksum, void* param, QBKeyHeader::QBKeyType type = QBKeyHeader::QBKeyType::STRING);
    extern void CallCFunction(DWORD checksum, CStruct* pParams = NULL);

    extern QBScript* Scripts;
    extern const char* QBTypes[19];

    void UpdateSpawnedScripts();

};
//--------Script Stuff--------

//Add a QBkeyHeader to game array, this can be a value/function that you can acces/call in qb script.
typedef QBKeyHeader* (__cdecl* const RegisterQBKeyHeaderFunc)(const int QBKey, const int HeaderID);
const RegisterQBKeyHeaderFunc AddQBKeyHeader = (RegisterQBKeyHeaderFunc)(0x0042B900);

//structure to hold a function that you can call from QB
//
//To add your own functions, make an array like this: 
//CompiledScript scripts[] = { {"ScriptName",         CFunctionName}, etc... };
//
//Then make a function like this:
//
//for (int i = 0; i < sizeof(scripts) / sizeof(CompiledScript); i++)
//{
//    QBKeyHeader* header = AddQBKeyHeader(Checksum(scripts[i].name), 1);
//    if (header)
//    {
//        header->type = QBKeyHeader::COMPILED_FUNCTION;
//        header->pFunction = scripts[i].pFunction;
//    }
//    else
//        MessageBox(0, "couldn't add script", scripts[i].name, 0);
//}
struct CompiledScript
{
    const char* name;
    bool(*pFunction)(CStruct*, CScript*);
};
struct Node;

struct CScript;



//pack(1) to make sure alignment is correct when try to acess game data
//#pragma pack(1)
struct EXTERN CScript
{
    BYTE* address;//address of where script - parser/executer is in script
    CStruct* params;//params sent when called the script
    void* extra;//extra stuff
    BYTE unk[0x35C];//not looked into
    unsigned long long startTime;
    unsigned long long waitPeriod;
    BYTE unk2[0x4];//not looked into
    //linked list
    //37c
    CScript* next;
    //380
    CScript* prev;
    //384
    int nodeIndex;//-1 if no node
    //388
    bool spawned;
    BYTE padding[3];
    //38C
    Node* node;
    //390
    DWORD scriptChecksum;//The name of the script
    //394
    DWORD paused;

    CScript()
    {
        ZeroMemory(this, sizeof(CScript));
    }


    

    bool GotParam(DWORD name);
    CStructHeader* GetParam(DWORD name);

    //Used to not make stuff crash when calling a CFunction
    void SetScript_Hook(DWORD checksum, CStruct* params, Node* object);

    void SetScript(DWORD checksum, CStruct* params, Node* object);

    void ClearScript()
    {
        typedef void(__thiscall* const pClearScript)(CScript* pThis);
        pClearScript(0x00427650)(this);
    }

    void SkipToken()
    {
        char opcode = *address;

        switch (opcode)
        {
        case QScript::ScriptToken::If2:
        case QScript::ScriptToken::Else2:
        case QScript::ScriptToken::EndSwitch2:
            address += 3;
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
            address += 5;
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
            address++;
            break;

        case QScript::ScriptToken::Pair:
            address += 9;
            break;

        case QScript::ScriptToken::Vector:
            address += 13;
            break;

        case QScript::ScriptToken::Random:
        case QScript::ScriptToken::RandomPermute:
        case QScript::ScriptToken::RandomNoRepeat:
            address += *(DWORD*)address * 4 + 5;
            break;

        default:
            //Should add all the unhandled opcodes to prevent crashing and other issues
            debug_print("%X @ %p\n", opcode, address);
            MessageBox(0, "Unhandled opcode...", __FUNCTION__, 0);
            break;
        }
    }

    void AdvanceToEnd();

    static CScript* GetNextScript(CScript* pScript = NULL)
    {
        if (pScript)
            return pScript->next;
        else
            return (CScript*)*(DWORD*)0x008E1DFC;
    }

    static DWORD GetLongestWaitPeriod()
    {
        DWORD longest = 0;
        CScript* p_scr = GetNextScript();

        while (p_scr)
        {
            if (p_scr->paused == 2 && p_scr->waitPeriod > longest)
                longest = p_scr->waitPeriod;
            p_scr = GetNextScript(p_scr);
        }
        return longest;
    }

    DWORD Update()
    {
        typedef DWORD(__thiscall* const pUpdate)(CScript* pThis);
        return pUpdate(0x00427940)(this);
    }

    static void ResetScriptTimers(DWORD time_now, DWORD old_time)
    {
        CScript* p_scr = GetNextScript();

        while (p_scr)
        {
            if (p_scr->paused == 2)
            {
                debug_print("Reseting timer on Script %s\n", FindChecksumName(p_scr->scriptChecksum));
                DWORD elapsed = old_time - *(DWORD*)&p_scr->startTime;
                p_scr->startTime = (time_now - elapsed);
            }
            p_scr = GetNextScript(p_scr);
        }
    }

    static void ResetScriptTimers()
    {
        CScript* p_scr = GetNextScript();

        while (p_scr)
        {
            if (p_scr->paused == 2)
            {
                debug_print("Reseting timer on Script %s\n", FindChecksumName(p_scr->scriptChecksum));
                p_scr->startTime = 0;
            }
            p_scr = GetNextScript(p_scr);
        }
    }

    static DWORD GetNumCScripts()
    {
        return *(DWORD*)0x008E1E18;//NumCStruct *(DWORD*)0x008E1E34;
    }

    static void DumpScripts()
    {
        debug_print("###########################################################\n\n");
        debug_print("All the CScripts that currently exist ...\n\n");

        CScript* p_scr = GetNextScript();

        while (p_scr)
        {
            debug_print("CScript %s %p\n", FindChecksumName(p_scr->scriptChecksum), p_scr);

            p_scr = GetNextScript(p_scr);

        }
        debug_print("\n");
    }

    bool AddCStruct(CStructHeader* pStruct, bool allocate = true);

    DWORD GetNodeName();

};
//compressed map
struct QBKeyInfo
{
    const enum QBKeyType
    {
        UNDEFINED = 0,
        INT = 1,
        FLOAT = 2,
        STRING = 3,
        LOCAL_STRING = 4,
        PAIR = 5,
        VECTOR = 6,
        SCRIPTED_FUNCTION = 7,
        COMPILED_FUNCTION = 8,
        GLOBAL = 9,
        STRUCT = 10,
        LOCAL_STRUCT = 11,
        ARRAY = 12,
        LOCAL = 13,
    };

    QBKeyType type;
    DWORD shiftedKey;
    QBKeyInfo* next;
    union
    {
        //This is the Value/Address of the QBKey ex:
        //if Type is int this is the value of the int
        //if Type is function this is the address of the function
        //values:
        union
        {
            float f;
            int i;
        } value;
        //pointers:
        char* pStr;
        CScript* pScript;
        D3DXVECTOR3* pVec;
        bool(*pFunction)(CStruct*, CScript*);
        CStructHeader** pStruct;
    };
};


struct QBKeyInfoContainer
{
    QBKeyInfo* first;
    QBKeyInfo* last;
};



//Get information about a QBKey, first it will try with dll function and then ingame function. If no information is found it will return NULL
EXTERN QBKeyHeader* GetQBKeyHeader(unsigned long QBKey);

//Information about a QBKeys is stored in a few arrays inside the exe
struct CStructHeader
{
    QBKeyHeader::QBKeyType Type;
    int QBkey;
    const union
    {
        union
        {
            float f;
            int i;
        } value;
        char* pStr;
        CScript* pScript;
        bool(*pFunction)(CStruct*, CScript*);
        //int Data;
        void* pData;
        D3DXVECTOR3* pVec;
        int Data;
        CStructHeader** pStruct;
        CArray* pArray;
        //This is the Value/Address of the QBKey ex:
        //if Type is int this is the value of the int
        //if Type is function this is the address of the function
    };

    
    //Unnamed int
    CStructHeader(QBKeyHeader::QBKeyType Type, int value, CStructHeader* next = NULL)
    {
        this->Type = Type;
        this->QBkey = 0;
        this->value.i = value;
        NextHeader = next;
    }

    void operator delete(void* ptr)
    {
        ((CStructHeader*)ptr)->Type = QBKeyHeader::UNDEFINED;
        ((CStructHeader*)ptr)->QBkey = 0;
        ((CStructHeader*)ptr)->NextHeader = *(CStructHeader**)0x008E1E04;
        *(void**)0x008E1E04 = ptr;
    }

    void ClearComponent()
    {
        typedef void(__cdecl* const pClearComponent)(CStructHeader* pThis);
        pClearComponent(0x00428920)(this);
    }

    CStructHeader(QBKeyHeader::QBKeyType Type, DWORD checksum, DWORD data)
    {
        this->Type = Type;
        this->QBkey = checksum;
        this->Data = data;
        NextHeader = NULL;
    }


    CStructHeader(QBKeyHeader::QBKeyType Type, DWORD checksum, void* data)
    {
        this->Type = Type;
        this->QBkey = checksum;
        this->pData = data;
        NextHeader = NULL;
    }

    CStructHeader(QBKeyHeader::QBKeyType Type, const char* text, CStructHeader* next = NULL)
    {
        this->Type = Type;
        this->QBkey = 0;
        this->pData = (void*)text;
        NextHeader = next;
    }

    inline float GetFloat()
    {
        return value.f;
    }
    inline int GetInt()
    {
        return value.i;
    }
    inline char* GetString()
    {
        return pStr;
    }
    inline CScript* GetScript()
    {
        return pScript;
    }

    EXTERN DWORD GetSize();

    CArray* GetArray(DWORD checksum)
    {
        if (this->QBkey == checksum)
        {
            debug_print("Array? %p %p\n", this, this->pArray);
            return this->pArray;
        }
        CStructHeader* header = this->NextHeader;
        while (header)
        {
            if (header->QBkey == checksum)
            {
                debug_print("Array? %p %p\n", header, header->pArray);
                return header->pArray;
            }
            header = header->NextHeader;
        }
        return NULL;
    }

    D3DXVECTOR3* GetVector(DWORD checksum)
    {
        if (QBkey == checksum)
        {
            return pVec;
        }

        CStructHeader* pThis = NextHeader;
        while (pThis)
        {
            if (pThis->QBkey == checksum)
            {
                return pThis->pVec;
            }
            pThis = pThis->NextHeader;
        }
        return NULL;
    }

    bool GetStruct(DWORD checksum, CStructHeader** header)
    {
        if (QBkey == checksum)
        {
            *header = this;
            return true;
        }

        CStructHeader* pThis = NextHeader;
        while (pThis)
        {
            if (pThis->QBkey == checksum)
            {
                *header = pThis;
                return true;
            }
            pThis = pThis->NextHeader;
        }

        return false;
    }
    inline bool CallScript(CStruct* pStruct, CScript* pScript)
    {
        return pFunction(pStruct, pScript);
    }
    inline void SetChecksum(const char* string)
    {
        value.i = Checksum(string);
    }
    inline void SetName(const char* name)
    {
        QBkey = Checksum(name);
    }
    CStructHeader* NextHeader;

    CStructHeader()
    {
        Type = QBKeyHeader::QBKeyType::UNDEFINED;
        QBkey = 0;
        Data = 0;
        NextHeader = NULL;
    }

    CStructHeader(QBKeyHeader::QBKeyType type, int _QBKey, float _value, CStructHeader* next)
    {
        Type = type;
        this->QBkey = _QBKey;
        this->value.f = _value;
        NextHeader = next;
    }

    CStructHeader(QBKeyHeader::QBKeyType type, int _QBKey, char* _pStr, CStructHeader* next)
    {
        Type = type;
        this->QBkey = _QBKey;
        this->pStr = _pStr;
        NextHeader = next;
    }

    inline int GetNumItems()
    {
        int numItems = 1;
        if (NextHeader)
            numItems++;
        else return numItems;
        CStructHeader* next = NextHeader;
        while (next->NextHeader != NULL)
        {
            next = next->NextHeader;
            numItems++;
        }
        return numItems;
    }


    bool SetFloat(DWORD checksum, float value)
    {
        debug_print("SetFloat %p\n", this);
        CStructHeader* header = this;
        while (header)
        {
            if (header->QBkey == checksum)
            {
                header->value.f = value;
                return true;
            }
            else if (header->Type == QBKeyHeader::STRUCT || header->Type == QBKeyHeader::LOCAL_STRUCT)
            {
                if ((*(CStructHeader**)header->pStruct)->SetFloat(checksum, value))
                    return true;
            }
            header = header->NextHeader;
        }
        return false;
    }

    //use this function to set multiply values in 1 function call
    void SetValues(CStructHeader* values)
    {
        CStructHeader* value = this;
        while (value)
        {
            CStructHeader* newValue = values;
            while (newValue)
            {
                if (value->QBkey == newValue->QBkey)
                {
                    switch (value->Type)
                    {
                    case QBKeyHeader::PAIR:
                        memcpy(value->pData, newValue->pData, 8);
                        break;
                    case QBKeyHeader::LOCAL_STRING:
                    case QBKeyHeader::STRING:
                        strcpy(value->pStr, newValue->pStr);
                        break;
                    case QBKeyHeader::VECTOR:
                        memcpy(value->pData, newValue->pData, 12);
                        break;
                    }
                }
                else
                    newValue = newValue->NextHeader;
            }
            switch (value->Type)
            {
            case QBKeyHeader::QBKeyType::LOCAL_STRUCT:
            case QBKeyHeader::QBKeyType::STRUCT:
                value->pStruct[0]->SetValues(values);
                break;
            case QBKeyHeader::QBKeyType::ARRAY:
                SetArrayValues(value->pArray, values);
                break;
            }
            value = value->NextHeader;
        }
    }
};

/*struct NodeContainer
{
    int index;
    D3DXVECTOR3 pos;
};*/


//CStructHeader Container.
struct EXTERN CStruct
{

    CStructHeader* head;//the first added header aka parent = SOF CStruct
    CStructHeader* tail;//the last added header = EOF CStruct

    CStruct()
    {
        head = NULL;
        tail = NULL;
    }

    
    void* operator new(size_t size)
    {
        CStruct* topHeap = *(CStruct**)0x008E1E30;
        *(CStruct**)0x008E1E30 = (CStruct*)topHeap->tail;
        *(DWORD*)0x008E1E34 = *(DWORD*)0x008E1E34 + 1;
        /*DWORD topHeap = *(DWORD*)(*(DWORD*)0x008E1E30 + 4);
        *(DWORD*)0x008E1E30 = topHeap;
        *(DWORD*)0x008E1E34 = *(DWORD*)0x008E1E34 + 1;*/
        return (void*)topHeap;
    }

    CStruct(CStructHeader* param)
    {
        Set(param);
    }

    CStruct(CStructHeader param, CStructHeader & param2)
    {
        param.NextHeader = &param2;
        Set(&param);
    }

    void operator delete(void* ptr)
    {
        /*DWORD topHeap = *(DWORD*)(*(DWORD*)0x008E1E30 + 4);
        *(DWORD*)(*(DWORD*)ptr + 4) = topHeap;
        *(void**)0x008E1E30 = ptr;*/

        CStruct* topHeap = *(CStruct**)0x008E1E30;
        ((CStruct*)ptr)->tail = topHeap->tail;
        *(void**)0x008E1E30 = ptr;
        *(DWORD*)0x008E1E34 = *(DWORD*)0x008E1E34 - 1;
    }

    void AddComponent(CStructHeader* param)
    {
        typedef void(__thiscall* const pAddComponent)(CStruct* pThis, CStructHeader* param);
        pAddComponent(0x004292B0)(this, param);
    }

    BYTE* AddComponentsUntilEndOfLine(BYTE* address, void* params = NULL)
    {
        typedef BYTE* (__thiscall* const pAddConentsUntilEndOfLine)(CStruct* pThis, BYTE* address, void* params);
        return pAddConentsUntilEndOfLine(0x00429500)(this, address, params);
    }

    CStruct(QBKeyHeader::QBKeyType type, int value)
    {
        static CStructHeader header;
        header = CStructHeader(type, value);
        head = &header;
        tail = head;
    }

    CStruct(QBKeyHeader::QBKeyType type, void* value)
    {
        CStructHeader param(type, 0, value);
        Set(&param);
    }

    CStruct(const char* text)
    {
        CStructHeader param(QBKeyHeader::QBKeyType::STRING, text);
        Set(&param);
    }

    DWORD GetName()
    {
        DWORD name = 0;
        GetChecksum(Checksums::Name, &name);
        return name;
    }

    void RemoveParam(DWORD name);


    //These are injected into game source and should not be called seperatly
    //The compressed nodes are generated from the .q file upon loading of the [levelname].qb
    //if the .q file exists and starts with 0xAA, it will try to generate the compressed nodes
    BYTE* MaybeAddCompressed(DWORD qbKey, BYTE* pFile, QBKeyInfoContainer* other);
    void AddCompressedNode(DWORD checksum, QBKeyInfoContainer* container);

    //Gets the first param
#ifdef _DEBUG
    __inline __declspec(noalias)CStructHeader const* __restrict GetHeader_debug(const char* func, const char* file, QBKeyHeader::QBKeyType type = QBKeyHeader::LOCAL) const
    {
        if (!head)
        {
            debug_print("%s No Param? in script %s\n", func, file);
            return NULL;
        }

        for (auto header = head; header != NULL; header = header->NextHeader)
        {
            if (header->Type == type)
                return header;
        }

        debug_print("%s couldn't find param type %s in script %s\n", func, QScript::QBTypes[type], file);
        return NULL;
}
#define CREATE_1(x) GetHeader_debug(__FUNCTION__, pScript ? FindChecksumName(pScript->scriptChecksum) : "No Script", x)
#define CREATE_0() GetHeader_debug(__FUNCTION__, pScript ? FindChecksumName(pScript->scriptChecksum) : "No Script")
#define FUNC_CHOOSER(_f1, _f2, _f3, ...) _f3
#define FUNC_RECOMPOSER(argsWithParentheses) FUNC_CHOOSER argsWithParentheses
#define CHOOSE_FROM_ARG_COUNT(...) FUNC_RECOMPOSER((__VA_ARGS__, CREATE_2, CREATE_1, ))
#define NO_ARG_EXPANDER() ,,CREATE_0
#define MACRO_CHOOSER(...) CHOOSE_FROM_ARG_COUNT(NO_ARG_EXPANDER __VA_ARGS__ ())
#define GetHeader(...) MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)
#else
    __inline __declspec(noalias) CStructHeader const* __restrict GetHeader(QBKeyHeader::QBKeyType type = QBKeyHeader::LOCAL) const
    {
        for (auto header = head; header != NULL; header = header->NextHeader)
        {
            if (header->Type == type)
                return header;
        }
        return NULL;
    }
#endif
    __inline __declspec(noalias)CStructHeader const* __restrict GetNamedHeader(DWORD name) const
    {
        for (auto header = head; header != NULL; header = header->NextHeader)
        {
            if (header->QBkey == name)
                return header;
        }
        return NULL;
    }



    CStructHeader* AddParam(const char* name, QBKeyHeader::QBKeyType type);

    typedef CStructHeader* (__cdecl* const mallocx)();
    static CStructHeader* AllocateCStruct(bool Allocate = false)
    {
        if (Allocate)
        {
            bool oldValue = *(bool*)0x008E1DF8;
            *(bool*)0x008E1DF8 = true;
            CStructHeader* temp = mallocx(0x00428B90)();
            *(bool*)0x008E1DF8 = oldValue;
            return temp;
        }
        return mallocx(0x00428B90)();
    }
    typedef CStructHeader* (__cdecl* const freex)(CStructHeader* param);
    static inline CStructHeader* DellocateCStruct(CStructHeader* param)
    {
        return freex(0x004260E0)(param);
    }
    typedef CStructHeader* (__cdecl* const freextail)(CStructHeader* param);
    inline CStructHeader* DellocateCStruct()
    {
        return freextail(0x004260E0)(tail);
    }


    //Game Functions in order

    typedef bool(__thiscall* addString)(CStruct* pThis, int name, QBKeyHeader::QBKeyType type, const void* data);
    bool inline AddParam(int name, QBKeyHeader::QBKeyType type, const void* data)
    {
        return addString(0x00428BF0)(this, name, type, data);
    }

    CStructHeader* AddCStruct(DWORD name, QBKeyHeader::QBKeyType type, DWORD data)
    {
        typedef CStructHeader* (__thiscall* pAddCStruct)(CStruct* pThis, int name, QBKeyHeader::QBKeyType type, DWORD data);
        return pAddCStruct(0x00428C80)(this, name, type, data);
    }

    inline CStructHeader* AddChecksum(int ParamName, int Data, int qbType)
    {
        //MessageBox(0,"Adding Checksum","Adding Checksum",MB_OK);
        int rett;
        static const DWORD pAddParam = 0x00428C80;

        _asm push Data;
        _asm push qbType;
        _asm push ParamName;
        _asm mov ecx, this;
        _asm call pAddParam;
        _asm mov rett, eax;
        return (CStructHeader*)rett;
    }

    /*typedef  bool (__stdcall *CStructMemberFunction)(void* Param, void* dest, int type );
    CStructMemberFunction pGetString;*/
    inline char* GetString()//use ONLY if your sure you got the param
    {
        static const DWORD pGetString = 0x00429690;
        char* pstr = NULL;
        _asm lea eax, pstr;
        _asm push 0;
        _asm push eax;
        _asm push 0;
        _asm mov ecx, this;
        _asm call pGetString;
        return pstr;
    }

    inline char* GetString(const char* Param)//use ONLY if your sure you got the param
    {
        static const DWORD pGetString = 0x00429760;
        char* pstr = NULL;
        _asm lea eax, pstr;
        _asm push 0;
        _asm push eax;
        _asm push Param;
        _asm mov ecx, this;
        _asm call pGetString;
        return pstr;
    }

    inline bool GetString(const char** dest)
    {
        bool rett;
        static const DWORD pGetString = 0x00429690;
        //char* pstr=NULL;
        //_asm lea eax, dest;
        _asm push 0;
        _asm push dest;
        _asm push 0;
        _asm mov ecx, this;
        _asm call pGetString;
        _asm mov rett, al
        return rett;
    }

    inline bool GetString(const char* Param, const char** dest)
    {
        bool rett;
        static const DWORD pGetString = 0x00429760;
        _asm push 0;
        _asm push dest;
        _asm push Param;
        _asm mov ecx, this;
        _asm call pGetString;
        _asm mov rett, al
        return rett;
    }

    inline bool GetChecksum(DWORD checksum, DWORD* out_checksum, bool assert = false)
    {
        typedef bool(__thiscall* const pGetChecksum)(CStruct* pThis, DWORD checksum, DWORD* out_checksum, bool assert);
        return pGetChecksum(0x00429D30)(this, checksum, out_checksum, assert);
    }

    inline bool ContainsFlag(DWORD checksum, bool assert = false)
    {
        typedef bool(__thiscall* const pContainsFlag)(CStruct* pThis, DWORD checksum, bool assert);
        return pContainsFlag(0x00429F50)(this, checksum, assert);
    }


    inline int GetChecksum(const char* Param, bool assert = false)//use ONLY if your sure you got the param
    {
        static const DWORD pGetChecksum = 0x00429840;
        int pint = 0;
        _asm lea eax, pint
        _asm push assert;
        _asm push eax;
        _asm push Param;
        _asm mov ecx, this;
        _asm call pGetChecksum;
        return pint;
    }

    inline bool GetChecksum(const char* Param, int* dest)
    {
        bool rett;
        static const DWORD pGetChecksum = 0x00429840;
        _asm push 0;
        _asm push dest;
        _asm push Param;
        _asm mov ecx, this
        _asm call pGetChecksum;
        _asm mov rett, al;
        return rett;
    }

    inline int GetInt()//use ONLY if your sure you got the param
    {
        static const DWORD pGetInt = 0x00429890;
        int pint = 0;
        _asm lea eax, pint;
        _asm push 0;
        _asm push eax;
        _asm push 0;
        _asm mov ecx, this
        _asm call pGetInt;
        return pint;
    }

    inline int GetInt(const char* Param)//use ONLY if your sure you got the param
    {
        static const DWORD pGetInt = 0x00429920;
        int pint = 0;
        _asm lea eax, pint;
        _asm push 0;
        _asm push eax;
        _asm push Param;
        _asm mov ecx, this
        _asm call pGetInt;
        return pint;
    }

    typedef bool(__thiscall* getInt)(CStruct* pThis, DWORD unk1, BYTE* dest, bool assert);
    inline bool GetEnum(DWORD checksum, BYTE* out_num, bool assert = false)
    {
        return getInt(0x00429890)(this,checksum, out_num, assert);
    }

    inline bool GetInt(int* dest)
    {
        return getInt(0x00429890)(this, 0, (BYTE*)dest, 0);
    }

    inline bool GetInt(const char* Param, int* dest)
    {
        bool rett;
        static const DWORD pGetInt = 0x00429920;
        _asm push 0;
        _asm push dest;
        _asm push Param;
        _asm mov ecx, this
        _asm call pGetInt;
        _asm mov rett, al
        return rett;
    }


    bool GetVector(int checksum, D3DXVECTOR3** dest)
    {
        bool rett;
        static const DWORD pGetVector = 0x00429970;
        _asm push 0;
        _asm push dest;
        _asm push checksum;
        _asm mov ecx, this
        _asm call pGetVector
        _asm mov rett, al;
        return rett;
    }

    inline bool GetFloat(const char* Param, float* dest, bool assert = false)
    {
        bool rett;
        static const DWORD pGetFloat = 0x00429a10;
        _asm push assert;
        _asm push dest;
        _asm push Param;
        _asm mov ecx, this
        _asm call pGetFloat;
        _asm mov rett, al;
        return rett;
    }

    inline bool GetPair(const char* Param, void* dest, bool assert = false)
    {
        bool rett;
        static const DWORD pGetPair = 0x00429b00;// <- points to 00429a60
        _asm push assert;
        _asm push dest;
        _asm push Param;
        _asm mov ecx, this
        _asm call pGetPair;
        _asm mov rett, al;
        return rett;
    }

    //GetInt/Float 00429b50---references to struct?
    //00429c10 points to ^


    inline bool GetStruct(int checksum, CStruct** dest, bool assert = false)
    {
        bool rett;
        static const DWORD pGetStruct = 0x00429C60;
        _asm push assert;
        _asm push dest;
        _asm push checksum;
        _asm mov ecx, this;
        _asm call pGetStruct;
        _asm mov rett, al;
        return rett;
    }

    inline bool GetStruct(const char* Param, void** dest, bool assert = false)
    {
        bool rett;
        static const DWORD pGetStruct = 0x00429CE0;
        _asm push assert
        _asm push dest
        _asm push Param
        _asm mov ecx, this
        _asm call pGetStruct
        _asm mov rett, al
        return rett;
    }

    inline bool GetScript(int* dest, bool assert = false)
    {
        bool rett;
        static const DWORD pGetScript = 0x00429D30;
        _asm push assert;
        _asm push dest
        _asm push 0
        _asm mov ecx, this;
        _asm call pGetScript
        _asm mov rett, al;
        return rett;
    }


    inline bool GetScript(const char** dest)
    {
        bool rett;
        static const DWORD pGetScript = 0x00429DC0;
        _asm push 0
        _asm push dest
        _asm push 0
        _asm mov ecx, this;
        _asm call pGetScript
        _asm mov rett, al;
        return rett;
    }

    /*inline bool GetScript(char* name, const char** dest)
    {
        bool rett;
        static const DWORD pGetScript = 0x00429DC0;
        _asm push 1
        _asm push dest
        _asm push name
        _asm mov ecx, this;
        _asm call pGetScript
        _asm mov rett, al;
        return rett;
    }*/

    inline bool GetScript(const char* script, const char** dest, bool assert = false)
    {
        bool rett;
        static const DWORD pGetScript = 0x00429DC0;
        _asm push assert
        _asm push dest
        _asm push script
        _asm mov ecx, this;
        _asm call pGetScript
        _asm mov rett, al; 0049D0B3
        return rett;
    }

    inline bool GetScript(const char* script, const int* dest)
    {
        bool rett;
        static const DWORD pGetScript = 0x00429DC0;
        _asm push 1
        _asm push dest
        _asm push script
        _asm mov ecx, this;
        _asm call pGetScript
        _asm mov rett, al;
        return rett;
    }

    inline bool GetArray(const char* name, const CArray** dest, bool assert = false)
    {
        typedef bool(__thiscall* pGetArray)(CStruct* pThis, const char* name, const CArray** dest, bool assert);
        return pGetArray(0x00429F00)(this, name, dest, assert);
    }


    inline bool ContainsFlag(const char* Param)
    {
        bool rett;
        static const DWORD pContainsFlag = 0x00429FC0;
        _asm push Param;
        _asm mov ecx, this
        _asm call pContainsFlag;
        _asm mov rett, al;
        return rett;
    }
    //EOF Game Functions



    //My Functions

    void Set(CStructHeader* pParam)
    {
        head = pParam;
        auto temp = pParam->NextHeader;
        if (temp)
        {
            while (temp->NextHeader)
            {
                temp = temp->NextHeader;
            }
            tail = temp;
        }
        else
            tail = pParam;
    }

    void FreeHeader();

    void Free();

    bool GetFloat(DWORD checksum, float* out_float, bool assert = false)
    {
        CStructHeader* pThis = head;
        QBKeyHeader* header;

        while (pThis && pThis != tail)
        {
            if (pThis->QBkey == checksum)
            {
                switch (pThis->Type)
                {
                case QBKeyHeader::QBKeyType::GLOBAL:
                    header = GetQBKeyHeader(pThis->Data);
                    if (header)
                    {
                        switch (header->type)
                        {
                        case QBKeyHeader::QBKeyType::LOCAL:
                        case QBKeyHeader::QBKeyType::INT:
                            *out_float = (float)header->value.i;
                            return true;
                        case QBKeyHeader::QBKeyType::FLOAT:
                            *out_float = header->value.f;
                            return true;
                        }
                    }
                    return false;
                case QBKeyHeader::QBKeyType::LOCAL:
                case QBKeyHeader::QBKeyType::INT:
                    *out_float = (float)pThis->value.i;
                    break;
                case QBKeyHeader::QBKeyType::FLOAT:
                    *out_float = pThis->value.f;
                    break;
                default:
                    if (assert)
                        MessageBox(0, "Need param...", FindChecksumName(checksum), 0);
                    return false;
                }
                return true;
            }
            if (pThis->NextHeader)
                pThis = pThis->NextHeader;
            else
                pThis++;

        }
        if (pThis && pThis->QBkey == checksum)
        {
            switch (pThis->Type)
            {
            case QBKeyHeader::QBKeyType::INT:
                *out_float = (float)pThis->value.i;
                break;
            case QBKeyHeader::QBKeyType::FLOAT:
                *out_float = pThis->value.f;
                break;
            default:
                if (assert)
                    MessageBox(0, "Need param...", FindChecksumName(checksum), 0);
                return false;
            }
            return true;
        }
        if (assert)
            MessageBox(0, "Need param...", FindChecksumName(checksum), 0);
        return false;
    }

    bool ContainsFlagDeep(DWORD checksum)
    {
        CStructHeader* pThis = head;
        while(pThis && pThis != tail)
        {
            if (pThis->QBkey == checksum)
            {
                return true;
            }
            else if (pThis->Type == QBKeyHeader::LOCAL_STRUCT || pThis->Type == QBKeyHeader::STRUCT)
            {
                CStructHeader* header = pThis;
                CStructHeader* pStruct = *(CStructHeader**)header->pStruct;

                while (pStruct)
                {
                    if (pStruct->QBkey == checksum)
                        return true;

                    else if (pStruct->Type == QBKeyHeader::LOCAL_STRUCT || pStruct->Type == QBKeyHeader::STRUCT)
                    {
                        //Only 2 levels for now
                        CStructHeader* pStruct2 = *(CStructHeader**)pStruct->pStruct;
                        while (pStruct2)
                        {
                            if (pStruct2->QBkey == checksum)
                                return true;
                            pStruct2 = pStruct2->NextHeader;
                        }
                    }
                    pStruct = pStruct->NextHeader;
                }
            }
            if (pThis->NextHeader)
                pThis = pThis->NextHeader;
            else
                pThis++;
        }
        if (pThis && pThis->QBkey == checksum)
        {
            return true;
        }
        return false;
    }

    /*bool ContainsFlag(DWORD checksum)
    {
        CStructHeader* pThis = head;
        while (pThis && pThis != tail)
        {
            if (pThis->QBkey == checksum)
            {
                return true;
            }
            if (pThis->NextHeader)
                pThis = pThis->NextHeader;
            else
                pThis++;
        }
        if (pThis && pThis->QBkey == checksum)
        {
            return true;
        }
        return false;
    }*/

    DWORD GetChecksumDeep(DWORD checksum)
    {
        CStructHeader* pThis = head;
        while (pThis && pThis != tail)
        {
            if (pThis->QBkey == checksum)
                return pThis->Data;
            else if (pThis->Type == QBKeyHeader::LOCAL_STRUCT || pThis->Type == QBKeyHeader::STRUCT)
            {
                CStructHeader* header = pThis;
                CStructHeader* pStruct = *(CStructHeader**)header->pStruct;

                while (pStruct)
                {
                    if (pStruct->QBkey == checksum)
                        return pStruct->Data;

                    else if (pStruct->Type == QBKeyHeader::LOCAL_STRUCT || pStruct->Type == QBKeyHeader::STRUCT)
                    {
                        //Only 2 levels for now
                        CStructHeader* pStruct2 = *(CStructHeader**)pStruct->pStruct;
                        while (pStruct2)
                        {
                            if (pStruct2->QBkey == checksum)
                                return pStruct2->Data;
                            pStruct2 = pStruct2->NextHeader;
                        }
                    }
                    pStruct = pStruct->NextHeader;
                }
            }
            /*else if (pThis->Type == QBKeyHeader::GLOBAL)
            {
                QBKeyHeader* header = GetQBKeyHeader(pThis->QBkey);
                if (!header)
                {
                    header = GetQBKeyHeader(pThis->Data);
                }
                if (header && (header->type == QBKeyHeader::LOCAL_STRUCT || header->type == QBKeyHeader::STRUCT))
                {
                    CStructHeader* pStruct = *(CStructHeader**)header->pStruct;

                    while (pStruct)
                    {
                        if (pStruct->QBkey == checksum)
                            return pStruct->Data;

                        else if (pStruct->Type == QBKeyHeader::LOCAL_STRUCT || pStruct->Type == QBKeyHeader::STRUCT)
                        {
                            //Only 2 levels for now
                            CStructHeader* pStruct2 = *(CStructHeader**)pStruct->pStruct;
                            while (pStruct2)
                            {
                                if (pStruct2->QBkey == checksum)
                                    return pStruct2->Data;
                                pStruct2 = pStruct2->NextHeader;
                            }
                        }
                        pStruct = pStruct->NextHeader;
                    }
                }
            }*/

            if (pThis->NextHeader)
                pThis = pThis->NextHeader;
            else
                pThis++;

        }
        if (pThis && pThis->QBkey == checksum)
        {
            return pThis->Data;
        }
        return 0;
    }

    DWORD GetChecksum(DWORD checksum, bool assert = false)
    {
        DWORD crc = 0;
        CStructHeader* pThis = head;
        while (pThis && pThis != tail)
        {
            if (pThis->QBkey == checksum)
            {
                return pThis->Data;
            }
            if (pThis->NextHeader)
                pThis = pThis->NextHeader;
            else
                pThis++;

        }
        if (pThis && pThis->QBkey == checksum)
        {
            return pThis->Data;
        }
        if (assert)
            MessageBox(0, "Need param...", FindChecksumName(checksum), 0);
        return crc;
    }

    bool GetStructDeep(DWORD checksum, CStructHeader** out_header)
    {
        CStructHeader* pThis = head;
        while (pThis && pThis != tail)
        {
            if (pThis->QBkey == checksum)
            {
                *out_header = pThis;
                return true;
            }
            else if (pThis->Type == QBKeyHeader::LOCAL_STRUCT || pThis->Type == QBKeyHeader::STRUCT)
            {
                CStructHeader* header = pThis;
                CStructHeader* pStruct = *(CStructHeader**)header->pStruct;

                while (pStruct)
                {
                    if (pStruct->QBkey == checksum)
                    {
                        *out_header = pThis;
                        return true;
                    }

                    else if (pStruct->Type == QBKeyHeader::LOCAL_STRUCT || pStruct->Type == QBKeyHeader::STRUCT)
                    {
                        //Only 2 levels for now
                        CStructHeader* pStruct2 = *(CStructHeader**)pStruct->pStruct;
                        while (pStruct2)
                        {
                            if (pStruct2->QBkey == checksum)
                            {
                                *out_header = pThis;
                                return true;
                            }
                            pStruct2 = pStruct2->NextHeader;
                        }
                    }
                    pStruct = pStruct->NextHeader;
                }
            }
            if (pThis->NextHeader)
                pThis = pThis->NextHeader;
            else
                pThis++;

        }
        if (pThis && pThis->QBkey == checksum)
        {
            *out_header = pThis;
            return true;
        }
        return false;
        //return head->GetStruct(checksum, header);
    }

    bool GetStruct(DWORD checksum, CStructHeader** header)
    {
        CStructHeader* pThis = head;
        while (pThis && pThis != tail)
        {
            if (pThis->QBkey == checksum)
            {
                *header = pThis;
                return true;
            }
            if (pThis->NextHeader)
                pThis = pThis->NextHeader;
            else
                pThis++;

        }
        if (pThis && pThis->QBkey == checksum)
        {
            *header = pThis;
            return true;
        }
        return false;
        //return head->GetStruct(checksum, header);
    }

    /*CStructHeader* GotParam(DWORD name)
    {

    }*/

    D3DXVECTOR3* GetVector(const char* param)
    {
        return head->GetVector(Checksum(param));
    }
    //EOF My Functions
};
//#pragma pop(pack)
#endif