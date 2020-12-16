#pragma once
#ifndef NODE_H
#define NODE_H
#include "Script.h"

//CArray, used for all scripted arrays including NodeArray
struct EXTERN CArray
{
    union
    {
        CStructHeader** Items;
        CStruct** structures;
        int* indices;
        char** texts;
    };
    WORD Type;
    WORD NumItems;

    int& operator[] (int x) {
        if (Type != 1)
        {
            _printf("Unsupported ArrayType\n");
            return x;
        }
        if (x >= NumItems)
        {
            _printf("###TOOO HIGH INDEX IN CARRAY(%d)!!!###\n", x);
            x = (NumItems - 1);
        }

        if (NumItems > 1)
        {
            _printf("NODE %p NumItms %d RANDOM(%d)\n\n", this, NumItems, x);
            //MessageBox(0, "", "", 0);
        }
        return  *(indices + x);
    }

    DWORD GetNumItems(bool trueCount = true) const;
    void SetValues(CStructHeader* values);
    inline float GetFloat(int Item)
    {
        if (Type == 0x0A || Type == 0x0B)
        {
            return *(float*)&((CStructHeader***)Items)[Item][0]->Data;
        }
        return ((float*)Items)[Item];
    }

    inline float GetFloat(int Item, int Itm)
    {
        if (Type == 0x0A || Type == 0x0B)
        {
            return *(float*)((CStructHeader***)Items)[Item][Itm]->Data;
        }
        else return 0.0;
    }

    inline int GetInt(int Item)
    {
        if (Type == 0x0A || Type == 0x0B)
            return ((CStructHeader***)Items)[Item][0]->Data;
        return ((int*)Items)[Item];
    }

    inline int GetInt(int Item, int Itm)
    {
        if (Type == 0x0A || Type == 0x0B)
            return ((CStructHeader***)Items)[Item][Itm]->Data;
        else return 0;
    }

    DWORD GetSize();

    char* GetString(int index)
    {
        //_printf("ArrayType %d %p\n", Type, Items[index]);
        return (char*)Items[index];
    }

    /*inline char* GetString(int Item)
    {
        if (Type == 0x0A || Type == 0x0B)
            return (char*)((CStructHeader***)Items)[Item][0]->pData;
        return ((char**)Items)[Item];
    }*/

    inline char* GetString(int Item, int Itm)
    {
        if (Type == 0x0A || Type == 0x0B)
            return (char*)((CStructHeader***)Items)[Item][Itm]->pData;
        return 0;
    }

    inline D3DXVECTOR3* GetVector(int Item)
    {
        if (Type == 0x0A || Type == 0x0B)
            return (D3DXVECTOR3*)((CStructHeader***)Items)[Item][0]->pData;
        return ((D3DXVECTOR3**)Items)[Item];
    }

    inline D3DXVECTOR3* GetVector(int Item, int Itm)
    {
        if (Type == 0x0A || Type == 0x0B)
            return (D3DXVECTOR3*)((CStructHeader***)Items)[Item][Itm]->pData;
        return 0;
    }

    inline void* GetPair(int Item)
    {
        if (Type == 0x0A || Type == 0x0B)
            return (void*)((CStructHeader***)Items)[Item][0]->pData;
        return ((void**)Items)[Item];
    }

    inline void* GetPair(int Item, int Itm)
    {
        if (Type == 0x0A || Type == 0x0B)
            return (void*)((CStructHeader***)Items)[Item][Itm]->pData;
        return 0;
    }

    inline CStructHeader** GetCStructs(int Item)
    {
        return ((CStructHeader***)Items)[Item];
    }

    CStructHeader* GetCStruct(int Name, int Item, int Struct);

    inline CStructHeader* GetCStruct(char* Name, int Item, int Struct)
    {
        return GetCStruct(Checksum(Name), Item, Struct);
    }

    CStructHeader* GetCStruct(int Name, int Item, int* outItem);

    inline CStructHeader* GetCStruct(char* Name, int Item, int* outItem)
    {
        return GetCStruct(Checksum(Name), Item, outItem);
    }

    CStruct* GetStructure(DWORD index)
    {
        return structures[index];
    }

    CStructHeader* GetCStruct(DWORD index, const char* functionName = NULL)
    {
        if (Type != 0xA && Type != 0xB)
        {
            _printf("WARNING " __FUNCTION__ " Wrong array Type(%d)\n", Type);
            _printf("Index %d(%d) %p in CArray %p\n CalledFrom %s\n", index, GetNumItems(), Items[index], this, functionName ? functionName : __FUNCTION__);
            //index = 0;
            //return (CStructHeader*)Items[index];
        }
        if (index >= 0xFFFF)
        {
            _printf("###WARNING INDEX TOO HIGH###\n");
            _printf("Index %d(%d) %p in CArray %p\n CalledFrom %s\n", index, GetNumItems(), Items[index], this, functionName ? functionName : __FUNCTION__);
            index = (NumItems - 1);
        }
        //
        return *(CStructHeader**)Items[index];
    }

    bool AddString(char* String);

    bool AddCStruct(CStruct* Struct);

    void Free();
};

struct Node
{
    DWORD pMemberFunctions;
    DWORD pNULL;
    DWORD numUnk1;
    DWORD flags;//Maybe?
    DWORD unknown[2];//NULL?
    D3DXVECTOR3 position;
    BYTE unknown2[52];
    DWORD name;//checksum


    //Gets pointer to the NodeArray, type is CArray(See Above)
    static CArray* GetNodeArray()
    {
        typedef CArray* (__cdecl* const pGetNodeArray)(DWORD checksum, DWORD param);
        //_printf("NodeArray %p\n", pGetNodeArray(0x00426590)(0xC472ECC5, 1));
        return pGetNodeArray(0x00426590)(0xC472ECC5, 1);
    }

    //Gets CStructHeader of this node
    CStructHeader* GetNodeStruct()
    {
        return GetNodeArray()->GetCStruct(GetNodeIndex(name));
    }

    static void PrintNodeArrayInfo()
    {
        CArray* NodeArray = GetNodeArray();


        DWORD numCStructs = 0, numInts = 0, intSize = 0,
            totalSize = 0, floatSize = 0, numFloats = 0, numArrays = 0, arraySize = 0,
            numPairs = 0, pairSize = 0, numStructs = 0, structSize = 0, numLocals = 0, localSize = 0,
            numStrings = 0, stringSize = 0, numCompiled = 0, compiledSize = 0, numScripts = 0,
            scriptSize = 0, numVec = 0, vectorSize = 0;
        for (DWORD i = 0; i < NodeArray->GetNumItems(); i++)
        {
            CStructHeader* header = NodeArray->GetCStruct(i);

            while (header)
            {
                numCStructs++;
                switch (header->Type)
                {
                case QBKeyHeader::INT:
                    numInts++;
                    intSize += sizeof(CStructHeader);
                    totalSize += intSize;
                    break;
                case QBKeyHeader::FLOAT:
                    floatSize += sizeof(CStructHeader);
                    totalSize += floatSize;
                    numFloats++;
                    break;
                case QBKeyHeader::ARRAY:
                    numArrays++;
                    arraySize = sizeof(CStructHeader);
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
                    structSize += sizeof(CStructHeader);
                    totalSize += structSize;
                    break;

                case QBKeyHeader::LOCAL:
                    numLocals++;
                    localSize += sizeof(CStructHeader);
                    totalSize += localSize;
                    break;

                case QBKeyHeader::STRING:
                case QBKeyHeader::LOCAL_STRING:
                    if (header->pStr && !InvalidReadPtr(header->pStr))
                    {
                        numStrings++;
                        stringSize += sizeof(CStructHeader) + strlen(header->pStr) + 1;
                        totalSize += stringSize;
                    }
                    break;

                case QBKeyHeader::COMPILED_FUNCTION:
                    numCompiled++;
                    compiledSize += sizeof(CStructHeader);
                    totalSize += compiledSize;
                    break;


                case QBKeyHeader::SCRIPTED_FUNCTION:
                    numScripts++;
                    scriptSize += sizeof(CStructHeader);
                    totalSize += scriptSize;
                    break;

                case QBKeyHeader::VECTOR:
                    numVec++;
                    vectorSize += sizeof(CStructHeader) + 12;
                    totalSize += vectorSize;
                    break;

                default:
                    totalSize += sizeof(CStructHeader);
                    break;
                }
                header = header->NextHeader;
            }


        }
        _printf("---NodeArray---\n");
        _printf("Numer of Nodes %d CStructs %d (MAX %d) size %X(Max %X)\n", NodeArray->GetNumItems(), numCStructs, HASH_SIZE / 12, numCStructs * 12, HASH_SIZE);
        _printf("Number of ints %d with total size %X\n", numInts, intSize);
        _printf("Number of floats %d with total size %X\n", numFloats, floatSize);
        _printf("Number of arrays %d with total size %X\n", numArrays, arraySize);
        _printf("Number of pairs %d with total size %X\n", numPairs, pairSize);
        _printf("Number of structs %d with total size %X\n", numStructs, structSize);
        _printf("Number of locals %d with total size %X\n", numLocals, localSize);
        _printf("Number of strings %d with total size %X\n", numStrings, stringSize);
        _printf("Number of CFuncs %d with total size %X\n", numCompiled, compiledSize);
        _printf("Number of Scripts %d with total size %X\n", numScripts, scriptSize);
        _printf("Number of Vectors %d with total size %X\n", numVec, vectorSize);
    }

    //Gets the CStructHeader of a node
    static CStructHeader* GetNodeStructByIndex(DWORD index)
    {
        return GetNodeArray()->GetCStruct(index);
    }

    static CStructHeader* GetNodeStructAndIndex(DWORD checksum, DWORD& out_index)
    {
        out_index = GetNodeIndex(checksum);
        return GetNodeArray()->GetCStruct(out_index);
    }

    //Gets the CStructHeader of a node
    static CStructHeader* GetNodeStruct(DWORD checksum)
    {
        return GetNodeArray()->GetCStruct(GetNodeIndex(checksum));
    }

    //Gets the index of a node in NodeArray
    static DWORD GetNodeIndex(DWORD checksum)
    {
        typedef DWORD(__cdecl* const pGetNodeIndex)(DWORD checksum);
        return pGetNodeIndex(0x0042B2B0)(checksum);
    }
};
#endif