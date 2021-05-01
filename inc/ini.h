#include <windows.h>
#include <stdbool.h>

// ### Write ###
bool IniSetString(LPCTSTR section, LPCTSTR key, LPCTSTR value, LPCTSTR fileName);
bool IniSetBool(LPCTSTR section, LPCTSTR key, bool value, LPCTSTR fileName);
bool IniSetInt(LPCTSTR section, LPCTSTR key, int value, LPCTSTR fileName);

//dune2000.ini
bool D2kIniSetString(LPCTSTR section, LPCTSTR key, LPCTSTR value);
bool D2kIniSetBool(LPCTSTR section, LPCTSTR key, bool value);
bool D2kIniSetInt(LPCTSTR section, LPCTSTR key, int value);

//spawn.ini
bool SpawnIniSetString(LPCTSTR section, LPCTSTR key, LPCTSTR value);
bool SpawnIniSetBool(LPCTSTR section, LPCTSTR key, bool value);
bool SpawnIniSetInt(LPCTSTR section, LPCTSTR key, int value);


// ### Read ###
int IniGetString(LPCTSTR section, LPCTSTR key, LPCTSTR defaultValue, char *out, int length, LPCTSTR fileName);
char *IniGetTempString(LPCTSTR section, LPCTSTR key, LPCTSTR defaultValue, LPCTSTR fileName);
bool IniGetBool(LPCTSTR section, LPCTSTR key, bool defaultValue, LPCTSTR fileName);
int IniGetInt(LPCTSTR section, LPCTSTR key, int defaultValue, LPCTSTR fileName);

//dune2000.ini
int D2kIniGetString(LPCTSTR section, LPCTSTR key, LPCTSTR defaultValue, char *out, int length);
bool D2kIniGetBool(LPCTSTR section, LPCTSTR key, bool defaultValue);
int D2kIniGetInt(LPCTSTR section, LPCTSTR key, int defaultValue);

//spawn.ini
int SpawnIniGetString(LPCTSTR section, LPCTSTR key, LPCTSTR defaultValue, char *out, int length);
bool SpawnIniGetBool(LPCTSTR section, LPCTSTR key, bool defaultValue);
int SpawnIniGetInt(LPCTSTR section, LPCTSTR key, int defaultValue);
