#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Defines.h"

static char Not_Inited[] = "NOT_INIT";
static char Unnamed_Param[] = "UnnamedParam";
char unkString[128] = "";

EXTERN char* FindChecksumName(DWORD checksum)
{
	if (QScript::Scripts)
	{
		if (checksum)
		{
			char* name = QScript::Scripts->GetQBKeyName(checksum);
			if (name)
				return name;
			else
			{
				sprintf(unkString, "UNKNOWN(0X%X)", checksum);
				return unkString;

			}

		}
		else
			return Unnamed_Param;
	}
	else
		return Not_Inited;
}



unsigned long crc32f(const char* buf)
{
	unsigned long crc = 0xFFFFFFFF;
	unsigned char c = *buf++;
	while (c != 0x0)
	{
		if (c >= 'A' && c <= 'Z') c += 32;
		if (c == '/') c = '\\';

		crc = checksumTable[(unsigned char)((crc ^ c) & 0xFF)] ^ (crc >> 8);
		c = *buf++;
	}
	return crc;
}

unsigned long Checksum(const char* string)//THPS3 Function for CRC32
{
	unsigned long checksum;

	typedef unsigned long(__cdecl* pCRC32Address)(const char* string);
	pCRC32Address pCRC32 = (pCRC32Address)(0x004265F0);
	checksum = pCRC32(string);

	return checksum;
}