#include "pch.h"
#include "String.h"

namespace String
{
    bool useExtraMemory = false;
    bool outOfMemory = false;
    char* ExtraMemoryTop = (char*)0x0087D8FC;
    char* ExtraMemoryBottom = (char*)0x0087D8FC;
    DWORD* pNumExtraStrings = (DWORD*)0x0087D8F8;
    DWORD* pNumStrings = (DWORD*)0x008E1E14;
    DWORD numExtraStrings = 0;
    DWORD numLevelStrings = 0;
    //DWORD numStrings = 0;
    DWORD numNoExtraStrings = 0;
    DWORD numExtraOriginal = 0;
    DWORD numOriginalStrings = 0;
    DWORD numExtraNewHeap = 0;


#define MAX_NUM_LEVEL 15000


    PermanentString levelStrings[MAX_NUM_LEVEL] = { 0 };

    char LevelHeapBottom[MAX_NUM_LEVEL * 80];
    char* LevelHeapTop = LevelHeapBottom;


    //#define MAX_NUM_EXTRA 30000

        //PermanentString permanentStrings[MAX_NUM_EXTRA] = { 0 };

    char PermanentHeapBottom[MAX_PERMANENT_STRINGS * 80];
    char* PermanentHeapTop = PermanentHeapBottom;
    char* StringHeapTop = NULL;

    char* AddString(const char* str)
    {
        DWORD checksum = CaseSenseChecksum(str);

        return AddString(checksum, str);
    }

    PermanentString* GetPermanentStringList()
    {
        return *(PermanentString**)0x008E1E10;
    }

    DWORD GetNumStrings()
    {
        return *(DWORD*)0x008E1E14;
    }

    DWORD GetNumStringsTotal()
    {
        return GetNumStrings();
    }

    DWORD GetNumMaxStringsTotal()
    {
        return MAX_NUM_STRINGS;// MAX_PERMANENT_STRINGS + MAX_NUM_EXTRA;
    }

    DWORD GetNumStrings(HEAP heap)
    {
        switch (heap)
        {
        case HEAP::ORIGINAL:
            return numExtraOriginal;
        case HEAP::NEW_NOEXTRA:
            return numNoExtraStrings;
        case HEAP::NEW_EXTRA:
            return numExtraNewHeap;
        case HEAP::LEVEL:
            return numLevelStrings;
        }
    }

    DWORD GetHeapSize(HEAP heap)
    {
        switch (heap)
        {
        case HEAP::ORIGINAL:
            return *(DWORD*)0x008E1E0C - 0x008935CC;
        case HEAP::NEW_NOEXTRA:
            return ExtraMemoryTop - ExtraMemoryBottom;
        case HEAP::NEW_EXTRA:
            return PermanentHeapTop - PermanentHeapBottom;
        case HEAP::LEVEL:
            return LevelHeapTop - LevelHeapBottom;
        }
    }

    DWORD GeHeapMaxSize(HEAP heap)
    {
        switch (heap)
        {
        case HEAP::ORIGINAL:
            return 0x008E1DF0 - 0x008935CC;
        case HEAP::NEW_NOEXTRA:
            return OLD_OTHER_SIZE;

        case HEAP::NEW_EXTRA:
            return sizeof(PermanentHeapBottom);

        case HEAP::LEVEL:
            return sizeof(LevelHeapBottom);
        }
    }

    void RemoveLevelStrings()
    {
        numLevelStrings = 0;
        LevelHeapTop = LevelHeapBottom;
    }

    void IncreaseTopHeap(DWORD len, bool level = false)
    {
        //StringHeapTop += len;
        /*if (level)
        {
            if (LevelHeapTop >= (LevelHeapBottom + (MAX_NUM_LEVEL * 80)))
                MessageBox(0, "Level String Heap too small...", "CRITICAL ERROR", 0);
            LevelHeapTop += len;
        }*/
        if (!outOfMemory)
        {
            if (!useExtraMemory)
            {
                if ((*(DWORD*)0x008E1E0C + len) >= 0x008E1D00)//Actually F0, but we keep some memory just in case we will reload QB files etc
                {
                    char  msg[256] = "";
                    _printf(("ExtraMemory reached @ %d strings"), GetNumStrings() + numExtraStrings);
                    //MessageBox(0, msg, msg, 0);
                    useExtraMemory = true;
                    StringHeapTop = (char*)0x0087D8FC;
                    ExtraMemoryTop = StringHeapTop;
                    ExtraMemoryTop += len;
                    ExtraMemoryBottom = StringHeapTop;
                    //*(DWORD*)0x008E1E14 = ((*(DWORD*)0x008E1E14) + numExtraOriginal);
                    //MessageBox(0, "Out of memory in permanent heap...", "CRITICAL ERROR", 0);
                }
                else
                {
                    StringHeapTop = *(char**)0x008E1E0C;
                    *(DWORD*)0x008E1E0C += len;
                    numExtraOriginal++;
                    //*(DWORD*)0x008E1E14 = *(DWORD*)0x008E1E14 + 1;
                }
                return;
            }



            if ((ExtraMemoryTop + len) >= ExtraMemoryBottom + OLD_OTHER_SIZE)
            {
                _printf("Out of memory in permanent heap...\nGoing to use new heap\n");
                PermanentHeapTop += len;
                StringHeapTop = PermanentHeapBottom;
                outOfMemory = true;
            }
            else
            {
                numNoExtraStrings++;
                StringHeapTop = ExtraMemoryTop;
                ExtraMemoryTop += len;
            }
            return;
        }
        else
        {
            numExtraNewHeap++;
            StringHeapTop = PermanentHeapTop;
            PermanentHeapTop += len;
            if (PermanentHeapTop >= (PermanentHeapBottom + (MAX_PERMANENT_STRINGS * 80)))
                MessageBox(0, "Permanent String Heap too small...", "CRITICAL ERROR", 0);
        }



    }


    /*void GetTopHeap(bool level = false)
    {
        if (outOfMemory)
        {
            StringHeapTop = PermanentHeapTop;
        }
        else
        {
            StringHeapTop = useExtraMemory ? ExtraMemoryTop : *(char**)0x008E1E0C;
        }
        DWORD len = strlen(StringHeapTop);
        if (len)
        {
            MessageBox(0, "Top heap error...", StringHeapTop, 0);
            StringHeapTop += len;
            IncreaseTopHeap(len);
        }
    }*/

    char* GetString(DWORD& checksum, const char* str)
    {
        DWORD checksum_non_case = crc32f(str);
        PermanentString* strings = GetPermanentStringList();
        DWORD numStrings = GetNumStrings();

        for (DWORD i = 0; i < numStrings; i++)
        {
            if (i < numOriginalStrings)
            {
                if (strings[i].checksum == checksum)
                {
                    _printf("Returning old String %s\n", strings[i].pStr);
                    if (strcmp(strings[i].pStr, str))
                        _printf("String missmatch? %s %s\n", strings[i].pStr, str);
                    return strings[i].pStr;
                }
            }
            if (crc32f(strings[i].pStr) == checksum_non_case)
            {
                if (stricmp(strings[i].pStr, str))
                    _printf("String missmatch? %s %s\n", strings[i].pStr, str);
                return strings[i].pStr;
            }
        }
        checksum = checksum_non_case;

        /*for (DWORD i = 0; i < numExtraStrings; i++)
        {
            if (permanentStrings[i].checksum == checksum)
            {
                if (stricmp(strings[i].pStr, str))
                    MessageBox(0, strings[i].pStr, str, 0);
                _printf("Returning optimized string %s\n", strings[i].pStr);
                return strings[i].pStr;
            }
        }*/

        return NULL;
    }

    char* AddLevelString(const char* str)
    {
        DWORD checksum = CaseSenseChecksum(str);
        char* new_string = GetString(checksum, str);
        if (new_string)
            return new_string;

        for (DWORD i = 0; i < numLevelStrings; i++)
        {
            if (levelStrings[i].checksum == checksum)
            {
                if (stricmp(levelStrings[i].pStr, str))
                    MessageBox(0, levelStrings[i].pStr, str, 0);
                _printf("Returning optimized level string %s\n", levelStrings[i].pStr);
                return levelStrings[i].pStr;
            }
        }

        if (numLevelStrings >= MAX_NUM_LEVEL)
        {
            MessageBox(0, "Please increase it", "Maximum number of permanent strings reached...", 0);
            return NULL;
        }

        /*GetTopHeap(true);
        DWORD len = strlen(str) + 1;


        IncreaseTopHeap(len, true);

        for (DWORD i = 0; i < len; i++)
        {
            StringHeapTop[i] = str[i];
        }*/
        DWORD len = strlen(str) + 1;

        if ((LevelHeapTop + len) >= (LevelHeapBottom + (MAX_NUM_LEVEL * 80)))
            MessageBox(0, "Level String Heap too small...", "CRITICAL ERROR", 0);

        new_string = LevelHeapTop;
        for (DWORD i = 0; i < len; i++)
        {
            *LevelHeapTop = str[i];
            LevelHeapTop++;
        }

        levelStrings[numLevelStrings].checksum = checksum;
        levelStrings[numLevelStrings].pStr = new_string;
        numLevelStrings++;
        return new_string;
    }

    char* AddString(DWORD checksum, const char* str)
    {
        PermanentString* strings = GetPermanentStringList();
        DWORD numStrings = GetNumStrings();
        if (numOriginalStrings == 0)
            numOriginalStrings = numStrings;

        /*for (DWORD i = 0; i < numStrings; i++)
        {
            if (strings[i].checksum == checksum)
            {
                if (strcmp(strings[i].pStr, str))
                    _printf("String missmatch? %s %s\n", strings[i].pStr, str);
                _printf("Returning old String %s\n", strings[i].pStr);
                return strings[i].pStr;
            }
            else if (crc32f(strings[i].pStr) == checksum_non_case)
            {
                if (stricmp(strings[i].pStr, str))
                    _printf("String missmatch? %s %s\n", strings[i].pStr, str);
                return strings[i].pStr;
            }
        }*/
        char* pStr = GetString(checksum, str);
        if (pStr != NULL)
            return pStr;

        /*for (DWORD i = 0; i < numExtraStrings; i++)
        {
            if (permanentStrings[i].checksum == checksum)
            {
                if (stricmp(permanentStrings[i].pStr, str))
                    MessageBox(0, permanentStrings[i].pStr, str, 0);
                _printf("Returning optimized string %s\n", permanentStrings[i].pStr);
                return strings[i].pStr;
            }
        }*/

        DWORD len = strlen(str) + 1;
        IncreaseTopHeap(len);

        /*if (strlen(StringHeapTop))
        {
            MessageBox(0, StringHeapTop, "Not good..", 0);
        }*/

        for (DWORD i = 0; i < len; i++)
        {

            StringHeapTop[i] = str[i];
        }

        strings[numStrings].checksum = checksum;
        strings[numStrings].pStr = StringHeapTop;

        numExtraStrings++;
        (*pNumStrings)++;

        if (numStrings >= MAX_NUM_STRINGS)//numExtraStrings >= MAX_NUM_EXTRA)
        {
            MessageBox(0, "Please increase it", "Maximum number of permanent strings reached...", 0);
            return NULL;
        }

        return StringHeapTop;
    }

    DWORD CaseSenseChecksum(const char* str)
    {
        typedef DWORD(__cdecl* pCaseSenseChecksum)(const char* string, DWORD len);

        return pCaseSenseChecksum(0x00411030)(str, strlen(str));
    }

    char* CreateString(const char* str, STRING_HEAP heap)
    {
        if (heap == STRING_HEAP::PERMANENT)
            AddString(str);

        typedef char* (__cdecl* pCreateTempString)(DWORD length);
        char* new_string = pCreateTempString(0x00428C4E)(strlen(str) + 1);
        strcpy(new_string, str);

        return new_string;
    }
};