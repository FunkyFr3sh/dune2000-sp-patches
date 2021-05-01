#define _CRTIMP
#include <windows.h>
#include <stdbool.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"

CALL(0x004A3BF1, _ParseCommandLine);

void ParseCommandLine(char *args)
{
    char *s = strtok(args," ");
    while (s != NULL)
    {
        for(unsigned int i = 0, l = strlen(s); i < l; i++)
            if(s[i] == '"') strcpy(s + i, s + i + 1);
        
        if (!strcmpi(s, "-g")) Force480Height = true;
        else if (!strcmpi(s, "-WOL")) JumpToWOLLogin = true;
        else if (!strcmpi(s, "-Skirmish")) JumpToSkirmish = true;
        else if (!strcmpi(s, "-LAN")) JumpToLAN = true;
        else if (!strcmpi(s, "-Spawn"))
        {
            JumpToGame = true;
            SpawnerActive = true;
        }
        
        s = strtok(NULL, " ");
    }
}
