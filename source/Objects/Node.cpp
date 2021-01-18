#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Extension\Defines.h"
#include "Objects\Node.h"


CArray* __restrict NodeArray = NULL;
EXTERN DWORD CArray::GetSize()
{
    DWORD size = sizeof(CArray);
    switch (Type)
    {
    case QBKeyHeader::INT:
        size += 4 * GetNumItems();
        break;
    case QBKeyHeader::FLOAT:
        size += 4 * GetNumItems();
        break;

        /*case QBKeyHeader::ARRAY:
            size += ->GetSize() + sizeof(QBKeyHeader);
            break;*/

    case QBKeyHeader::PAIR:
        size += 8 * GetNumItems();
        break;


        /*case QBKeyHeader::STRUCT:
            size += GetCStruct(0)->GetSize();
            break;*/
    case QBKeyHeader::STRUCT:
    case QBKeyHeader::LOCAL_STRUCT:
        for (DWORD i = 0; i < GetNumItems(); i++)
        {
            if (GetCStruct(i) && !InvalidReadPtr(GetCStruct(i)))
                size += GetCStruct(i, __FUNCTION__)->GetSize() + 4;
        }
        break;

    case QBKeyHeader::LOCAL:
        size += 4 * GetNumItems();
        break;

    case QBKeyHeader::STRING:
    case QBKeyHeader::LOCAL_STRING:
        for (DWORD i = 0; i < GetNumItems(); i++)
        {
            size += strlen(GetString(i)) + 1 + 4;
        }
        break;

    case QBKeyHeader::COMPILED_FUNCTION:
        size += 4 * GetNumItems();
        break;


    case QBKeyHeader::SCRIPTED_FUNCTION:
        size += 4 * GetNumItems();
        break;

    case QBKeyHeader::VECTOR:
        size += 12 * GetNumItems();
        break;

    default:
        size += 4 * GetNumItems();
    }
    return size;
}

EXTERN CStructHeader* CArray::GetCStruct(int Name, int Item, int* outItem)
{
    if (Type == 0x0A || Type == 0x0B)
    {
        for (int i = Item; i < NumItems; i++)
        {
            int numItems = ((CStructHeader***)Items)[i][0]->GetNumItems();
            for (int j = 0; j < numItems; j++)
            {
                if (((CStructHeader***)Items)[i][j]->QBkey == Name)
                {
                    *outItem = i;
                    return ((CStructHeader***)Items)[i][j];
                }
            }
        }
        return 0;
    }

    return 0;
}

EXTERN void CArray::SetValues(CStructHeader* values)
{
    for (DWORD i = 0; i < NumItems; i++)
    {
        CStructHeader* value = ((CStructHeader***)Items)[i][0];
        /*CStructHeader*** StructHeader = (CStructHeader***)Items;
        DWORD numItems = StructHeader[i][0]->GetNumItems();
        for(DWORD j=0; j<numItems; j++)
        {
        CStructHeader* value = ((CStructHeader***)Items)[i][j];*/
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
                    case QBKeyHeader::LOCAL_STRING:
                    case QBKeyHeader::STRING:
                        strcpy(value->pStr, newValue->pStr);
                    case QBKeyHeader::VECTOR:
                        memcpy(value->pData, newValue->pData, 12);
                    }
                    goto next;
                }
                newValue = newValue->NextHeader;
            }
            switch (value->Type)
            {
            case QBKeyHeader::QBKeyType::LOCAL_STRUCT:
            case QBKeyHeader::QBKeyType::STRUCT:
                value->pStruct[0]->SetValues(values);
            case QBKeyHeader::QBKeyType::ARRAY:
                SetArrayValues(value->pArray, values);//value->pArray->SetValues(values);
            }
        next:
            value = value->NextHeader;
        }
        //}
    }
}


EXTERN void CArray::Free()
{
    if (Type == 10 || Type == 11)
    {
        for (int i = 0; i < NumItems; ++i)
        {
            int numItems = ((CStructHeader***)Items)[i][0]->GetNumItems();
            for (int j = 0; j < numItems; ++j)
            {
                if (((CStructHeader***)Items)[i][j]->Type == 3 || ((CStructHeader***)Items)[i][j]->Type == 4 || ((CStructHeader***)Items)[i][j]->Type == 5 || ((CStructHeader***)Items)[i][j]->Type == 6 || ((CStructHeader***)Items)[i][j]->Type == 10 || ((CStructHeader***)Items)[i][j]->Type == 11 || ((CStructHeader***)Items)[i][j]->Type == 12)
                {
                    free(((CStructHeader***)Items)[i][j]->pData);
                }
                ((CStructHeader***)Items)[i][j]->pData = NULL;
            }
            free(((CStructHeader***)Items)[i]);
            ((CStructHeader***)Items)[i] = NULL;
        }
        free(Items);
        Items = NULL;
    }
    else if (Type == 6 || Type == 3 || Type == 5)
    {
        for (int i = 0; i < NumItems; ++i)
        {
            free((void*)Items[i]);
            Items[i] = NULL;
        }
        free(Items);
        Items = NULL;
    }
    else
    {
        free(Items);
        Items = NULL;
    }
}

EXTERN bool CArray::AddCStruct(CStruct* Struct)
{
    if (Type == 10 || Type == 11)
    {
        NumItems++;
        Items = (CStructHeader**)realloc(Items, (sizeof(void**) * NumItems));
        if (Items == NULL)
            return false;
        Items[NumItems] = (CStructHeader*)Struct;
        return true;
    }
    return false;
}

EXTERN CStructHeader* CArray::GetCStruct(int Name, int Item, int Struct)
{
    if (Type == 0x0A || Type == 0x0B)
    {
        CStructHeader*** StructHeader = (CStructHeader***)Items;
        int numItems = StructHeader[Item][Struct]->GetNumItems();
        for (int i = Struct; i < numItems; i++)
        {
            if (Name == StructHeader[Item][i]->QBkey)
            {
                return StructHeader[Item][i];
            }
        }

        return 0;

    }
    else return 0;
}

EXTERN DWORD CArray::GetNumItems(bool trueCount) const
{
    if (!trueCount)
    {
        int numItems = 0;
        if (Type == 0x0A || Type == 0x0B)
        {
            for (int i = 0; i < NumItems; ++i)
            {
                numItems += ((CStructHeader***)Items)[i][0]->GetNumItems();
            }
            return numItems;
        }
    }
    return NumItems;
}

EXTERN bool CArray::AddString(char* String)
{
    if (Type == 3 || Type == 4)
    {
        NumItems++;
        Items = (CStructHeader**)realloc(Items, (sizeof(void**) * NumItems));
        if (Items == NULL)
            return false;
        Items[NumItems] = (CStructHeader*)String;
        return true;
    }
    return false;
}