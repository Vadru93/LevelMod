#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "IniWriter.h"
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

CIniWriter::CIniWriter(char* szFileName)
{
    memset(m_szFileName, 0x00, 255);
    memcpy(m_szFileName, szFileName, strlen(szFileName));
}

bool CIniWriter::find(const char* szSection, char* szKey)
{
    return GetPrivateProfileInt(szSection, szKey, 0xFF, m_szFileName) != 0xFF;
}

void CIniWriter::WriteInt(const char* szSection, const char* szKey, int iValue)
{
    char szValue[255];
    sprintf(szValue, "%d", iValue);
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}
void CIniWriter::WriteFloat(const char* szSection, const char* szKey, float fltValue)
{
    char szValue[255];
    sprintf(szValue, "%f", fltValue);
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}
void CIniWriter::WriteBool(const char* szSection, const char* szKey, bool bolValue)
{
    char szValue[255];
    sprintf(szValue, "%s", bolValue ? "True" : "False");
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}
void CIniWriter::WriteString(const char* szSection, const char* szKey, char* szValue)
{
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}