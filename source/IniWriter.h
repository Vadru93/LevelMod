#pragma once
#ifndef INIWRITER_H
#define INIWRITER_H
class CIniWriter
{
public:
    CIniWriter(char* szFileName);
    void WriteInt(const char* szSection, const char* szKey, int iValue);
    void WriteFloat(const char* szSection, const char* szKey, float fltValue);
    void WriteBool(const char* szSection, const char* szKey, bool bolValue);
    void WriteString(const char* szSection, const char* szKey, char* szValue);

    bool find(const char* szSection, char* szKey);
private:
    char m_szFileName[255];
};
#endif //INIWRITER_H