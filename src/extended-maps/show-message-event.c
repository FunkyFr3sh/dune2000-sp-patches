#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "ini.h"
#include "utils.h"

CALL(0x00454425, _FakeData__GetTextString_Event);

char *FakeData__GetTextString_Event(int stringId, bool showError)
{
    static char result[512];
    
    char mapIniPath[256];
    sprintf(mapIniPath, ".\\%s%s", 
        GameType == GT_SINGLEPLAYER ? MissionsResourcePath : MapsResourcePath, PathChangeExtension(MissionMap, ".ini"));
    
    char id[12];
    sprintf(id, "%d", stringId);
    IniGetString("Text", id, "", result, 512, mapIniPath);
    if (strlen(result) > 0) 
        return result;
    else 
        return Data__GetTextString(stringId, showError);
}
