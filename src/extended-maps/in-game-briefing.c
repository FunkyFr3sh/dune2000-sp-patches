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
    static char result[65536];
    if (SpawnerActive && GameType == GT_SINGLEPLAYER)
    {
        char mapIniPath[256];
        sprintf(mapIniPath, ".\\%s%s", MissionsResourcePath, PathChangeExtension(MissionMap, ".ini"));
    
        IniGetString("Basic", "Briefing", "", result, 65536, mapIniPath);
        
        // If no custom briefing is specified, fall back to text from TEXT.UIB
        if (!strlen(result))
          return GetTextString(stringId, showError);
        
        // Avoid double appearance of custom briefing if vanilla UIL file is used - elliminate "IG_*M*Text2" strings
        if (!strncmp(gTextTable[stringId]->id, "IG_", 3))
          return "";
        
        for (uint32_t i = 0; i < strlen(result); i++)
        {
            char *c = (char *)result + i;
            if (*c == '_') 
                *c = 0xAC;
        }
        return result;
    }
    else 
        return GetTextString(stringId, showError);
}
