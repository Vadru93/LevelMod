#pragma once
#ifndef INIREADER_H
#define INIREADER_H

using namespace std;

class CIniReader
{
public:
    CIniReader(char* szFileName);
    int ReadInt(const char* szSection, const char* szKey, int iDefaultValue);
    float ReadFloat(const char* szSection, const char* szKey, float fltDefaultValue);
    bool ReadBool(const char* szSection, const char* szKey, bool bolDefaultValue);
    char* ReadString(const char* szSection, const char* szKey, const char* szDefaultValue);
private:
    char m_szFileName[255];
};

#endif//INIREADER_H