#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "IniReader.h"
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace std;

CIniReader::CIniReader(char* szFileName)
{
    memset(m_szFileName, 0x00, 255);
    memcpy(m_szFileName, szFileName, strlen(szFileName));
}
int CIniReader::ReadInt(const char* szSection, const char* szKey, int iDefaultValue)
{
    int iResult = GetPrivateProfileInt(szSection, szKey, iDefaultValue, m_szFileName);
    return iResult;
}
float CIniReader::ReadFloat(const char* szSection, const char* szKey, float fltDefaultValue)
{
    char szResult[255];
    char szDefault[255];
    float fltResult;
    sprintf(szDefault, "%f", fltDefaultValue);
    GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName);
    fltResult = atof(szResult);
    return fltResult;
}
bool CIniReader::ReadBool(const char* szSection, const char* szKey, bool bolDefaultValue)
{
    char szResult[255];
    char szDefault[255];
    bool bolResult;
    sprintf(szDefault, "%s", bolDefaultValue ? "1" : "0");
    GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName);
    bolResult = (strcmp(szResult, "1") == 0) ? true : false;
    return bolResult;
}

char* CIniReader::ReadString(const char* szSection, const char* szKey, const char* szDefaultValue)
{
    char* szResult = new char[255];
    memset(szResult, 0x00, 255);
    GetPrivateProfileString(szSection, szKey, szDefaultValue, szResult, 255, m_szFileName);

    printf("INI string: %s\r\n", szResult);

    return szResult;
}