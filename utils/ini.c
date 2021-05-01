#include <windows.h>
#include <stdbool.h>
#include <stdio.h>

typedef int (__stdcall *WritePrivateProfileStringA_)(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName);
typedef int (__stdcall *GetPrivateProfileStringA_)(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName);

static HMODULE hModule;
static WritePrivateProfileStringA_ writeIni;
static GetPrivateProfileStringA_ readIni;

// ### Write ###
bool IniSetString(LPCTSTR section, LPCTSTR key, LPCTSTR value, LPCTSTR fileName)
{
    if (!hModule) hModule = LoadLibraryA("Kernel32.dll");
    if (!hModule) return false;
    if (!writeIni) writeIni = (WritePrivateProfileStringA_)GetProcAddress(hModule,"WritePrivateProfileStringA");
    if (!writeIni) return false;
    return writeIni(section, key, value, fileName);
}

bool IniSetBool(LPCTSTR section, LPCTSTR key, bool value, LPCTSTR fileName)
{
    return IniSetString(section, key, value ? "Yes" : "No", fileName);
}

bool IniSetInt(LPCTSTR section, LPCTSTR key, int value, LPCTSTR fileName)
{
    char result[12];
    sprintf(result,"%d",value);
    return IniSetString(section, key, result, fileName);
}


// ### Read ###
int IniGetString(LPCTSTR section, LPCTSTR key, LPCTSTR defaultValue, char *out, int length, LPCTSTR fileName)
{
    if (!hModule) hModule = LoadLibraryA("Kernel32.dll");
    if (!hModule)
    {
        strcpy(out, defaultValue);
        return strlen(out);
    }
    if (!readIni) readIni = (GetPrivateProfileStringA_)GetProcAddress(hModule,"GetPrivateProfileStringA");
    if (!readIni)
    {
        strcpy(out, defaultValue);
        return strlen(out);
    }
    return readIni(section, key, defaultValue, out, length, fileName);
}

char *IniGetTempString(LPCTSTR section, LPCTSTR key, LPCTSTR defaultValue, LPCTSTR fileName)
{
    static char result[1024];
    IniGetString(section, key, defaultValue, result, 1024, fileName);
    return result;
}

bool IniGetBool(LPCTSTR section, LPCTSTR key, bool defaultValue, LPCTSTR fileName)
{
    char value[5];
    IniGetString(section, key, defaultValue ? "Yes" : "No", value, 5, fileName);

    return (strcmpi(value, "yes") == 0 || strcmpi(value, "true") == 0 || strcmpi(value, "1") == 0);
}

int IniGetInt(LPCTSTR section, LPCTSTR key, int defaultValue, LPCTSTR fileName)
{
    char dvalue[12];
    sprintf(dvalue,"%d", defaultValue);
    char value[12];
    IniGetString(section, key, dvalue, value, 12, fileName);
    
    return atoi(value);
}
