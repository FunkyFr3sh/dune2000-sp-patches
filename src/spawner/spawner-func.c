#include <winsock2.h>
#include <stdio.h>
#include "dune2000.h"
#include "patch.h"
#include "macros/patch.h"
#include "ini.h"
#include "utils.h"


bool SpawnerActive = false;
LPCTSTR spawnIni = ".\\spawn.ini";


#define GetInt(a,b,c) SpawnIniGetInt(a,b,c)
#define GetBool(a,b,c) SpawnIniGetBool(a,b,c)
#define GetString(a,b,c,d,e) SpawnIniGetString(a,b,c,d,e)

void InitSpawner()
{
    SpawnerActive = true;
    RandSeed = SpawnIniGetInt("Settings", "Seed", 1);
    
    GameSpeed = SpawnIniGetInt("Settings", "GameSpeed", GameSpeed);
    SpawnIniGetString("Settings", "Scenario", "2play1", NetMap, 50);
    strcpy(MissionMap, NetMap);
    SpawnIniGetString("Settings", "ScenarioName", NetMap, MapName, 60);
    MissionNumber = SpawnIniGetInt("Settings", "MissionNumber", 0);
    MySideID = SpawnIniGetInt("Settings", "MySideID", -1);
    gDifficultyLevel = SpawnIniGetInt("Settings", "DifficultyLevel", 1);
    //stats hack
    NetPlayerCount = 1;
    gNetAIPlayers = 7;

    GameState = GS_MAINLOOP;
    CUIManager__LoadMission(gUIMgr, NetMap);
}

// ### Write ###
bool SpawnIniSetString(LPCTSTR section, LPCTSTR key, LPCTSTR value)
{
    return IniSetString(section, key, value, spawnIni);
}

bool SpawnIniSetBool(LPCTSTR section, LPCTSTR key, bool value)
{
    return IniSetBool(section, key, value, spawnIni);
}

bool SpawnIniSetInt(LPCTSTR section, LPCTSTR key, int value)
{
    return IniSetInt(section, key, value, spawnIni);
}


// ### Read ###
int SpawnIniGetString(LPCTSTR section, LPCTSTR key, LPCTSTR defaultValue, char *out, int length)
{
    return IniGetString(section, key, defaultValue, out, length, spawnIni);
}

bool SpawnIniGetBool(LPCTSTR section, LPCTSTR key, bool defaultValue)
{
    return IniGetBool(section, key, defaultValue, spawnIni);
}

int SpawnIniGetInt(LPCTSTR section, LPCTSTR key, int defaultValue)
{
    return IniGetInt(section, key, defaultValue, spawnIni);
}

