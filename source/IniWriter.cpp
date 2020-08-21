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

bool CIniWriter::find(char* szSection, char* szKey)
{
    return GetPrivateProfileInt(szSection, szKey, 0xFF, m_szFileName) != 0xFF;
}

void CIniWriter::WriteInt(char* szSection, char* szKey, int iValue)
{
 char szValue[255];
 sprintf(szValue, "%d", iValue);
 WritePrivateProfileString(szSection,  szKey, szValue, m_szFileName); 
}
void CIniWriter::WriteFloat(char* szSection, char* szKey, float fltValue)
{
 char szValue[255];
 sprintf(szValue, "%f", fltValue);
 WritePrivateProfileString(szSection,  szKey, szValue, m_szFileName); 
}
void CIniWriter::WriteBool(char* szSection, char* szKey, bool bolValue)
{
 char szValue[255];
 sprintf(szValue, "%s", bolValue ? "True" : "False");
 WritePrivateProfileString(szSection,  szKey, szValue, m_szFileName); 
}
void CIniWriter::WriteString(char* szSection, char* szKey, char* szValue)
{
 WritePrivateProfileString(szSection,  szKey, szValue, m_szFileName);
}