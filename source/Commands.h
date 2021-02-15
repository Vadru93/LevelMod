#include "pch.h"
#include "Script\Node.h"
#include "Memory\String.h"


DWORD showmessage = 0;
char msg[256] = "";
static char download_message[1024] = "";

//seems to be broken in Openspy...
void SendPm(char* who, const char* message);

char* ChatText = NULL;

const Vertex* const __restrict GetSkaterPos();

float GetSkaterYAngle();

#undef CreateConsole
BOOL CreateConsole();

void AddDump(const char* dump, ...);

void CommandKick(const char* message)
{

}
void CommandBan(const char* message)
{

}
void CommandShowCommands(const char* message);
void CommandGetInfo(const char* message)
{

    extern QBKeyHeader triggers[MAX_TRIGGERS];
    DWORD numQBKeyHeaders = 0, numInts = 0, numUndef = 0, undefSize = 0, intSize = 0,
        totalSize = 0, floatSize = 0, numFloats = 0, numArrays = 0, arraySize = 0,
        numPairs = 0, pairSize = 0, numStructs = 0, structSize = 0, numLocals = 0, localSize = 0,
        numStrings = 0, stringSize = 0, numCompiled = 0, compiledSize = 0, numScripts = 0,
        scriptSize = 0, numVec = 0, vectorSize = 0;
    for (DWORD i = 0; i < MAX_TRIGGERS; i++)
    {
        if (triggers[i].type || triggers[i].QBKeyCheck)
        {
            numQBKeyHeaders++;
            switch (triggers[i].type)
            {
            case QBKeyHeader::INT:
                numInts++;
                intSize += sizeof(QBKeyHeader);
                totalSize += intSize;
                break;
            case QBKeyHeader::FLOAT:
                floatSize += sizeof(QBKeyHeader);
                totalSize += floatSize;
                numFloats++;
                break;
            case QBKeyHeader::ARRAY:
                numArrays++;
                arraySize += triggers[i].pArray->GetSize() + sizeof(QBKeyHeader);
                totalSize += arraySize;
                break;

            case QBKeyHeader::PAIR:
                numPairs++;
                pairSize += 8 + sizeof(QBKeyHeader);
                totalSize += pairSize;
                break;

            case QBKeyHeader::LOCAL_STRUCT:
            case QBKeyHeader::STRUCT:
                numStructs++;
                structSize += (*(CStructHeader**)triggers[i].pStruct)->GetSize() + sizeof(QBKeyHeader);
                totalSize += structSize;
                break;

            case QBKeyHeader::LOCAL:
                numLocals++;
                localSize += sizeof(QBKeyHeader);
                totalSize += localSize;
                break;

            case QBKeyHeader::STRING:
            case QBKeyHeader::LOCAL_STRING:
                if (triggers[i].pStr && !InvalidReadPtr(triggers[i].pStr))
                {
                    numStrings++;
                    stringSize += sizeof(QBKeyHeader) + strlen(triggers[i].pStr) + 1;
                    totalSize += stringSize;
                }
                break;

            case QBKeyHeader::COMPILED_FUNCTION:
                numCompiled++;
                compiledSize += sizeof(QBKeyHeader);
                totalSize += compiledSize;
                break;


            case QBKeyHeader::SCRIPTED_FUNCTION:
                numScripts++;
                scriptSize += sizeof(QBKeyHeader);
                totalSize += scriptSize;
                break;

            case QBKeyHeader::VECTOR:
                numVec++;
                vectorSize += sizeof(QBKeyHeader) + 12;
                totalSize += vectorSize;
                break;

            default:
                numUndef++;
                undefSize += sizeof(QBKeyHeader);
                totalSize += sizeof(QBKeyHeader);
            }
        }

    }

    debug_print("Number of headers %d(MAX %d) with total size %X\n Header only size %X(max %X)\n", numQBKeyHeaders, MAX_TRIGGERS, totalSize, numQBKeyHeaders * sizeof(QBKeyHeader), OTHER_SIZE);
    debug_print("Number of ints %d with total size %X\n", numInts, intSize);
    debug_print("Number of floats %d with total size %X\n", numFloats, floatSize);
    debug_print("Number of arrays %d with total size %X\n", numArrays, arraySize);
    debug_print("Number of pairs %d with total size %X\n", numPairs, pairSize);
    debug_print("Number of structs %d with total size %X\n", numStructs, structSize);
    debug_print("Number of locals %d with total size %X\n", numLocals, localSize);
    debug_print("Number of strings %d with total size %X\n", numStrings, stringSize);
    debug_print("Number of CFuncs %d with total size %X\n", numCompiled, compiledSize);
    debug_print("Number of Scripts %d with total size %X\n", numScripts, scriptSize);
    debug_print("Number of Vectors %d with total size %X\n", numVec, vectorSize);
    //numQBKeyHeaders -= (numInts + numFloats + numArrays + numPairs + numStructs + numLocals + numStrings + numCompiled + numScripts + numVec);
    if (numUndef)
    {
        //t//otalSize -= undefSize
        debug_print("Number of Undefineed %d with total size %X\n", numUndef, undefSize);
    }
    Node::PrintNodeArrayInfo();
    extern BYTE hashTable[HASH_SIZE];
    debug_print("NodeHashTable %X\n\n", hashTable);
    debug_print("Total number of permanent strings %d(MAX %d)\n", String::GetNumStringsTotal(), String::GetNumMaxStringsTotal());
    debug_print("Original PermanentHeap %d %X(MAX %X)\n", String::GetNumStrings(String::HEAP::ORIGINAL), String::GetHeapSize(String::HEAP::ORIGINAL), String::GeHeapMaxSize(String::HEAP::ORIGINAL));
    debug_print("New PermanentHeap(no extra memory) %d %X(MAX %X)\n", String::GetNumStrings(String::HEAP::NEW_NOEXTRA), String::GetHeapSize(String::HEAP::NEW_NOEXTRA), String::GeHeapMaxSize(String::HEAP::NEW_NOEXTRA));
    debug_print("New PermanentHeap(extra memory) %d %X(MAX %X)\n\n", String::GetNumStrings(String::HEAP::NEW_EXTRA), String::GetHeapSize(String::HEAP::NEW_EXTRA), String::GeHeapMaxSize(String::HEAP::NEW_EXTRA));
    debug_print("Level specific strings: \n");
    debug_print("LevelPermanentHeap %d %X(MAX %X)\n\n", String::GetNumStrings(String::HEAP::LEVEL), String::GetHeapSize(String::HEAP::LEVEL), String::GeHeapMaxSize(String::HEAP::LEVEL));
    //MessageBox(0, 0, 0, 0);

    using namespace Network;
    char test[128] = "Test Msg";
    SendMessageToClients(MSG_ID_LM_TEST, strlen(test) + 1, &test);


}

bool GetInfoScript(CStruct* pStruct, CScript* pScript)
{
    CommandGetInfo(NULL);
    return true;
}

void CommandConsole(const char* message);
void CommandTell(const char* message);
void CommandAdd(const char* message);
void CommandToggleLogging(const char* message);

struct Commands
{
    DWORD checksum;
    void(*function)(const char* message);
    char name[12];
};
Commands commands[] = {
    { crc32f("commands"), &CommandShowCommands, "commands" },
    { crc32f("tell"), &CommandTell, "tell" },
    { crc32f("kick"), &CommandKick, "kick" },
    { crc32f("ban"), &CommandBan, "ban" },
    { crc32f("getinfo"), &CommandGetInfo, "getinfo" },
    { crc32f("console"), &CommandConsole, "console" },
#ifdef    _DEBUG
    { crc32f("log"), &CommandToggleLogging, "log" },
#endif
    { crc32f("add"), &CommandAdd, "add"} };



inline void ReadChat()
{
    if (ChatText)
    {
        DWORD chatLen = strlen(ChatText);
        if (chatLen > 150)
            ChatText[149] = 0x0;
        for (DWORD i = 0; i < chatLen; i++)
        {
            if (ChatText[i] == '%')
                ChatText[i] = '_';
        }
        static DWORD CrownCount = 0, RestartCount = 0, ZoneCount = 0, numSpawns = 0;

        register const char* ptr = &ChatText[0];
        /*if(*ptr++ == '/')
        {*/
        if (*ptr == '/')
        {
            const char* message = ptr + 1;

            char command[12];
            DWORD j = 0;
            while (message[j] != 0x20 && message[j] != 0x0)
            {
                command[j] = message[j];
                j++;
                if (j > 12)
                {
                    j = 0;
                    break;
                }
            }
            if (j)
            {
                command[j] = 0x00;

                DWORD cmd = crc32f(command);
                for (DWORD i = 0; i < sizeof(commands) / sizeof(Commands); i++)
                {
                    if (cmd == commands[i].checksum)
                    {
                        commands[i].function(message);
                        break;
                    }
                }
            }
            /*char* commands[] = {
              "tell", "kick", "ban", "getinfo", "console", "debug", NULL
              };

              DWORD i = 0;
              char* command = commands[i];
              while (command)
              {
              DWORD j = 0;
              while (*command != 0)
              {
              if (*command++ != message[j])
              goto nextCommand;
              j++;
              }
              switch (i)
              {
              case 4:
              CreateConsole();
              debug_print("Welcome to Console!!!!\n\n");
              *(BYTE*)0x00478983 = 0x75;
              break;
              case 5:
              HookDebugMessages(NULL, NULL);
              *(BYTE*)0x00478983 = 0x75;
              break;
              case 0:
              message += 5;
              char name[128];
              DWORD j = 0;

              while (*message != 0x20 && *message != 0)
              {
              name[j] = *message++;
              j++;
              }
              name[j] = 0;
              message++;

              SendPm(name, message);
              *(BYTE*)0x00478983 = 0x75;
              break;
              }
              break;
              nextCommand:
              i++;
              command = commands[i];
              }*/
        }
        if (*ptr++ == 'a' && *ptr++ == 'd' && *ptr++ == 'd')
        {
            static const char* commands[] = { "flag", "zone", "key", "crown", "spawn" };
            DWORD i = 0;
        NextCommand:

            const char* pCommand = commands[i];
            //char* ptr = &ChatText[4];
            while (*pCommand)
            {
                if (*pCommand != *ptr)
                {
                    if (i < sizeof(commands) / sizeof(char*))
                    {
                        i++;
                        ptr = &ChatText[3];
                        goto NextCommand;
                    }
                    else
                        return;
                }
                ptr++;
                pCommand++;
            }
            if (i == 0)
            {
                Vertex vertex = *GetSkaterPos();
                if (*ptr == '\0')
                {
                    AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team1\nClass = GenericNode\nType = CTF_1\nCreatedAtStart }\n \r\n", vertex.x, vertex.y, vertex.z * -1);
                    AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team1\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = \"Team: CTF\"\nrestart_types = ARRAY(\n\nCTF_1\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n", vertex.x, vertex.y, vertex.z * -1);
                }
                else if (strstr(ptr, "blue"))
                {
                    AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team1\nClass = GenericNode\nType = CTF_1\nCreatedAtStart }\n \r\n", vertex.x, vertex.y, vertex.z * -1);
                    AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team1\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = \"Team: CTF\"\nrestart_types = ARRAY(\n\nCTF_1\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n", vertex.x, vertex.y, vertex.z * -1);
                }
                else if (strstr(ptr, "red"))
                {
                    AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team02\nClass = GenericNode\nType = CTF_2\nCreatedAtStart }\n \r\n", vertex.x, vertex.y, vertex.z * -1);
                    AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team2\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = \"Team: CTF\"\nrestart_types = ARRAY(\n\nCTF_2\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n", vertex.x, vertex.y, vertex.z * -1);
                }
                else
                {
                    AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team1\nClass = GenericNode\nType = CTF_1\nCreatedAtStart }\n \r\n", vertex.x, vertex.y, vertex.z * -1);
                    AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team1\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = \"Team: CTF\"\nrestart_types = ARRAY(\n\nCTF_1\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n", vertex.x, vertex.y, vertex.z * -1);
                }
                *(BYTE*)0x00478983 = 0x75;
            }
            else if (i == 1)
            {
                Vertex vertex = *GetSkaterPos();
                unsigned int zone_multiplier = 1;
                sscanf(ChatText, "addzone %d", &zone_multiplier);
                AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CONTROL_ZONE%d\nClass = GenericNode\nType = ZONE\nCreatedAtStart zone_multiplier = %d\n}\n \r\n", vertex.x, vertex.y + 50, vertex.z * -1, ZoneCount, zone_multiplier);
                ZoneCount++;
                CrownCount++;
                AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_NewCrown%d\nClass = GenericNode\nType = Crown\nCreatedAtStart }\n \r\n", vertex.x, vertex.y, vertex.z * -1, CrownCount);
                *(BYTE*)0x00478983 = 0x75;
            }
            else if (i == 2)
            {
                Vertex vertex = *GetSkaterPos();
                AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_ZONE_KEY\nClass = GenericNode\nType = ZONE_KEY\nCreatedAtStart zone_multiplier = 1\n}\n \r\n", vertex.x, vertex.y, vertex.z * -1);
                *(BYTE*)0x00478983 = 0x75;
            }
            else if (i == 3)
            {
                Vertex vertex = *GetSkaterPos();
                CrownCount++;
                AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_NewCrown%d\nClass = GenericNode\nType = Crown\nCreatedAtStart }\n \r\n", vertex.x, vertex.y, vertex.z * -1, CrownCount);
                *(BYTE*)0x00478983 = 0x75;
            }
            else if (i == 4)
            {
                numSpawns++;
                Vertex vertex = *GetSkaterPos();
                float angle = GetSkaterYAngle();
                AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; %f; 0.0]\nName = TRG_Restart%u\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = \"Restart %u\"\nrestart_types = ARRAY(\nMultiPlayer\n)\nTriggerScript = TRG_SpawnSkater\n}\n\r\n", vertex.x, vertex.y, vertex.z * -1, angle, numSpawns, numSpawns);
                *(BYTE*)0x00478983 = 0x75;
            }
        }
        // }
        /*if(strstr(ChatText,"addflag"))
        {

        Vertex vertex = *GetSkaterPos();

        if(strstr(ChatText,"blue"))
        {
        AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team1\nClass = GenericNode\nType = CTF_1\nCreatedAtStart }\n \r\n",vertex.x,vertex.y,vertex.z*-1);
        AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team1\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = Team: CTF\nrestart_types = ARRAY(\n\nCTF_1\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n",vertex.x,vertex.y,vertex.z*-1);
        }
        else if(strstr(ChatText,"red"))
        {
        AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team02\nClass = GenericNode\nType = CTF_2\nCreatedAtStart }\n \r\n",vertex.x,vertex.y,vertex.z*-1);
        AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team2\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = Team: CTF\nrestart_types = ARRAY(\n\nCTF_2\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n",vertex.x,vertex.y,vertex.z*-1);
        }
        else
        {
        AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team1\nClass = GenericNode\nType = CTF_1\nCreatedAtStart }\n \r\n",vertex.x,vertex.y,vertex.z*-1);
        AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team1\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = Team: CTF\nrestart_types = ARRAY(\n\nCTF_1\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n",vertex.x,vertex.y,vertex.z*-1);
        }

        }
        else if(!strcmp(ChatText,"addzone"))
        {
        Vertex vertex = *GetSkaterPos();
        AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CONTROL_ZONE%d\nClass = GenericNode\nType = ZONE\nCreatedAtStart zone_multiplier = 1\n}\n \r\n",vertex.x,vertex.y+50,vertex.z*-1,ZoneCount);
        ZoneCount++;
        }

        else if(strstr(ChatText,"addzone "))
        {
        unsigned int zone_multiplier = 1;
        Vertex vertex = *GetSkaterPos();
        sscanf(ChatText,"addzone %d",&zone_multiplier);
        AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CONTROL_ZONE%d\nClass = GenericNode\nType = ZONE\nCreatedAtStart zone_multiplier = %d\n}\n \r\n",vertex.x,vertex.y+50,vertex.z*-1,ZoneCount,zone_multiplier);
        ZoneCount++;
        }

        else if(!strcmp(ChatText,"addkey"))
        {
        Vertex vertex = *GetSkaterPos();
        AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_ZONE_KEY\nClass = GenericNode\nType = ZONE_KEY\nCreatedAtStart zone_multiplier = 1\n}\n \r\n",vertex.x, vertex.y, vertex.z*-1);
        }

        else if(!strcmp(ChatText,"addcrown"))
        {
        Vertex vertex = *GetSkaterPos();
        CrownCount++;
        AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_NewCrown%d\nClass = GenericNode\nType = Crown\nCreatedAtStart }",vertex.x,vertex.y,vertex.z*-1,CrownCount);
        }

        /*else if(strstr(ChatText,"addspawn "))
        {
        Vertex vertex = *GetSkaterPos();
        RestartCount++;
        char Chat[MAX_PATH];
        sprintf(Chat,"STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_NewRestart_%d\nClass = Restart\nType = MultiPlayer\nCreatedAtStart RestartName = \x22NewRestart%d\x22\nrestart_types = ARRAY(\nMultiPlayer ",vertex.x, vertex.y, vertex.z*-1,RestartCount,RestartCount);
        if(strstr(ChatText,"horse"))
        strcat(Chat,"Horse ");
        if(strstr(ChatText,"blue"))
        strcat(Chat,"CTF_1");
        if(strstr(ChatText,"red"))
        strcat(Chat,"CTF_2");
        AddDump(Chat);
        AddDump("\n)\nTriggerScript = TRG_Spawn\n}\n \r\n");

        if(!SpawnFunc)
        {
        SpawnFunc=true;
        AddDump("\n#15801  FUNCTION SpawnFunc\n#15808 MakeSkaterGoto StartSkating1\n#15809 END FUNCTION\n \r\n");
        }
        }*/
    }
}

__declspec(naked) void ReadChatHook(void)
{
    static DWORD ReadChatRetAddr = 0;
    __asm
    {

        pop ReadChatRetAddr

        mov ChatText, edx

        pushad
        pushfd
    }
    *(BYTE*)0x00478983 = 0x74;
    ReadChat();
    __asm
    {
        popfd
        popad

        sub esp, 0x000001E8

        push ReadChatRetAddr

        /*_asm mov eax, 0x004788CA
        _asm mov [esp], eax*/
        ret
    }
}

void CommandConsole(const char* message)
{
    CreateConsole();
    debug_print("Welcome to Console!!!!\n\n");
    *(BYTE*)0x00478983 = 0x75;
}

void CommandTell(const char* message)
{
    message += 5;
    char name[128];
    DWORD j = 0;

    while (*message != 0x20 && *message != 0)
    {
        name[j] = *message++;
        j++;
    }
    if (message != 0)
    {
        name[j] = 0;
        message++;

        SendPm(name, message);
        *(BYTE*)0x00478983 = 0x75;
    }
}

char* ChatText2 = NULL;


bool SpawnFunc = false;
DWORD CrownCount = 0, ZoneCount = 0, RestartCount = 0;

extern FILE* logFile;
void CommandToggleLogging(const char* message)
{
#ifdef _DEBUG
    LevelModSettings::bLogging = !LevelModSettings::bLogging;
    if (LevelModSettings::bLogging)
    {
        auto header = GetQBKeyHeader(Checksum("LaunchPanelMessage"));
        CStruct pStruct;
        CScript pScript;
        CStructHeader param(QBKeyHeader::STRING, 0, (void*)"Logging enabled");
        pStruct.AddParam(&param);
        header->pFunction(&pStruct, &pScript);
        logFile = fopen("LM_Log.txt", "w");
    }
    else if (logFile)
    {
        auto header = GetQBKeyHeader(Checksum("LaunchPanelMessage"));
        CStruct pStruct;
        CScript pScript;
        CStructHeader param(QBKeyHeader::STRING, 0, (void*)"Logging disabled");
        pStruct.AddParam(&param);
        header->pFunction(&pStruct, &pScript);
        fclose(logFile);
        logFile = NULL;
    }
#endif
}


void CommandAdd(const char* message)
{
    debug_print("CommandAdd - %s\n", message);
    message += 4;


    char command[12];
    DWORD j = 0;
    while (message[j] != 0x20 && message[j] != 0x0)
    {
        command[j] = message[j];
        j++;
        if (j > 12)
        {
            j = 0;
            break;
        }
    }
    if (j)
    {
        command[j] = 0x00;
        debug_print("Param %s\n", command);
        Vertex vertex = *GetSkaterPos();

        DWORD chc = crc32f(command);
        switch (chc)
        {
        case Checksums::Key:
            AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_ZONE_KEY\nClass = GenericNode\nType = ZONE_KEY\nCreatedAtStart zone_multiplier = 1\n}\n \r\n", vertex.x, vertex.y, vertex.z * -1);
            break;

        case Checksums::Flag:
            if (strstr(message, "blue"))
            {
                AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team1\nClass = GenericNode\nType = CTF_1\nCreatedAtStart }\n \r\n", vertex.x, vertex.y, vertex.z * -1);
                AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team1\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = Team: CTF\nrestart_types = ARRAY(\n\nCTF_1\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n", vertex.x, vertex.y, vertex.z * -1);
            }
            else if (strstr(message, "red"))
            {
                AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team02\nClass = GenericNode\nType = CTF_2\nCreatedAtStart }\n \r\n", vertex.x, vertex.y, vertex.z * -1);
                AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team2\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = Team: CTF\nrestart_types = ARRAY(\n\nCTF_2\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n", vertex.x, vertex.y, vertex.z * -1);
            }
            else
            {
                AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team1\nClass = GenericNode\nType = CTF_1\nCreatedAtStart }\n \r\n", vertex.x, vertex.y, vertex.z * -1);
                AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team1\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = Team: CTF\nrestart_types = ARRAY(\n\nCTF_1\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n", vertex.x, vertex.y, vertex.z * -1);
            }

            break;
        case Checksums::Zone:
            AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CONTROL_ZONE%d\nClass = GenericNode\nType = ZONE\nCreatedAtStart zone_multiplier = 1\n}\n \r\n", vertex.x, vertex.y + 50, vertex.z * -1, ZoneCount);
            ZoneCount++;
            break;
        case Checksums::Crown:
            CrownCount++;
            AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_NewCrown%d\nClass = GenericNode\nType = Crown\nCreatedAtStart }", vertex.x, vertex.y, vertex.z * -1, CrownCount);
            break;

        case Checksums::Spawn:
            RestartCount++;
            char Chat[MAX_PATH];
            sprintf(Chat, "STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_NewRestart_%d\nClass = Restart\nType = MultiPlayer\nCreatedAtStart RestartName = \x22NewRestart%d\x22\nrestart_types = ARRAY(\nMultiPlayer ", vertex.x, vertex.y, vertex.z * -1, RestartCount, RestartCount);
            if (strstr(message, "horse"))
                strcat(Chat, "Horse ");
            if (strstr(message, "blue"))
                strcat(Chat, "CTF_1");
            if (strstr(message, "red"))
                strcat(Chat, "CTF_2");
            AddDump(Chat);
            AddDump("\n)\nTriggerScript = TRG_Spawn\n}\n \r\n");

            if (!SpawnFunc)
            {
                SpawnFunc = true;
                AddDump("\n#15801  FUNCTION SpawnFunc\n#15808 MakeSkaterGoto StartSkating1\n#15809 END FUNCTION\n \r\n");
            }
            break;
        default:
            printf("Default...\n");
            break;
        }
    }

}

void CommandShowCommands(const char* message)
{
    DWORD k = 0;
    for (DWORD i = 1; i < sizeof(commands) / sizeof(Commands); i++)
    {
        DWORD j = 0;
        while (commands[i].name[j] != 0)
        {
            msg[k] = commands[i].name[j];
            j++;
            k++;
        }
        msg[k] = 0x20;
        k++;
    }
    msg[k] = '\n';
    msg[k] = 0;
    debug_print(msg);

    sprintf(download_message, msg);
    showmessage = 1000;

    /*CStruct param2;
    CStructHeader param(QBKeyHeader::STRING, Checksums::text, msg);
    param2.AddParam(&param);
    ExecuteQBScript("LaunchGrafCounter", &param2);*/
}

