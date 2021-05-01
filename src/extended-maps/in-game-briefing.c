#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"
#include "utils.h"

CALL(0x00480E20, _FakeData__GetTextString_Briefing);

char *FakeData__GetTextString_Briefing(int stringId, bool showError)
{
    static char result[4096];
    if (MissionNumber == 0 && SpawnerActive && GameType == GT_SINGLEPLAYER)
    {
        char mapIniPath[256];
        sprintf(mapIniPath, ".\\%s%s", MissionsResourcePath, PathChangeExtension(MissionMap, ".ini"));
    
        IniGetString("Basic", "Briefing", "", result, 4096, mapIniPath);
        
        for (uint32_t i = 0; i < strlen(result); i++)
        {
            char *c = (char *)result + i;
            if (*c == '_') 
                *c = 0xAC;
        }
        return result;
    }
    else 
        return Data__GetTextString(stringId, showError);
}
