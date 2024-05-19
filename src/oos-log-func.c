#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "hotkeys.h"
#include "ini.h"

CALL(0x0045AAE9, _WriteOOSLog);

uint32_t RandomClassLog[4096];
uint32_t RandomGameTick[4096];

int WriteOOSLog()
{
    FILE *fp = fopen("OOS.txt", "wt");
    if (fp)
    {
        /*
        int dura = GetTickCount() - GameStartTickCount;
        
        if (dura > 1000 && GameTicks > 0)
        {
            fprintf(fp, "AFPS=%u\n", GameTicks / (dura / 1000));
        }
        */
        
        fprintf(fp, "GameTicks=%u\n", GameTicks);
        fprintf(fp, "RandSeed=%d\n", RandSeed);
        fprintf(fp, "GameSpeed=%d\n", GameSpeed);
        fprintf(fp, "NetUnitCount=%d\n", NetUnitCount);
        fprintf(fp, "NetTechLevel=%d\n", NetTechLevel);
        fprintf(fp, "NetStartingCredits=%d\n", NetStartingCredits);
        fprintf(fp, "NetAIPlayers=%d\n", NetAIPlayers);
        fprintf(fp, "NetCrates=%s\n", NetCrates ? "Yes" : "No");
        fprintf(fp, "NetWorms=%d\n", NetWorms);
        fprintf(fp, "DisableEngineer=%s\n", DisableEngineer ? "Yes" : "No");
        fprintf(fp, "DisableTurrets=%s\n", DisableTurrets ? "Yes" : "No");
        fprintf(fp, "NoCarryall=%s\n", NoCarryall ? "Yes" : "No");
        fprintf(fp, "ShortGame=%s\n", ShortGame ? "Yes" : "No");
        fprintf(fp, "MapName=%s\n", MapName);
        
        fprintf(fp, "%s\n", "");
        
        for (int i = 0; i < 6; i++)
        {
            if (strlen(NetPlayersExt[i].name) == 0) 
                continue;
            
            fprintf(
                fp, 
                "Player%d=%s %s\n", 
                i,
                NetPlayersExt[i].name,
                NetPlayersExt[i].isSpectator ? "(Spec)" : "");
        }
        
        fprintf(fp, "%s\n", "");

        for (uint32_t i = 0; i < sizeof(RandomClassLog)/sizeof(RandomClassLog[0]); i++)
            fprintf(fp, "rand[%d]=%x, Tick=%u\n", i, RandomClassLog[i], RandomGameTick[i]);
        
        fclose(fp);
    }
    
    return GameEndState;
}
