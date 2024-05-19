#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "rules.h"
#include "utils.h"
#include "ini.h"

CALL(0x00448614, _GamePlayRestart);
CALL(0x00470BA0, _GameExit);
CALL(0x004B28C1, _GameStart);

void LoadSavedGame(const char *saveGamePath) // Skirmish/SinglePlayer load saved game function
{
    memset(&MCVDeployed, 0, sizeof(MCVDeployed));
    LoadRulesFromMap();
    InitMissionScript();
    StatsClear();

    char saveGameIniPath[256];
    sprintf(saveGameIniPath, ".\\%s.ini", saveGamePath);
    DisableEngineer = IniGetBool("SaveGame", "DisableEngineer", DisableEngineer, saveGameIniPath);
    DisableTurrets = IniGetBool("SaveGame", "DisableTurrets", DisableTurrets, saveGameIniPath);
    NoCarryall = IniGetBool("SaveGame", "NoCarryall", NoCarryall, saveGameIniPath);
    ShortGame = IniGetBool("SaveGame", "ShortGame", ShortGame, saveGameIniPath);
}

void WriteSaveGame(const char *saveGamePath, const char *saveGameName) // Skirmish/SinglePlayer save game function
{
    char saveGameIniPath[256];
    sprintf(saveGameIniPath, ".\\%s.ini", saveGamePath);
    IniSetString("SaveGame", "Name", saveGameName, saveGameIniPath);
    IniSetString("SaveGame", "Map", MissionMap, saveGameIniPath);
    IniSetBool("SaveGame", "DisableEngineer", DisableEngineer, saveGameIniPath);
    IniSetBool("SaveGame", "DisableTurrets", DisableTurrets, saveGameIniPath);
    IniSetBool("SaveGame", "NoCarryall", NoCarryall, saveGameIniPath);
    IniSetBool("SaveGame", "ShortGame", ShortGame, saveGameIniPath);
}

void GamePlayRestart() // Skirmish/SinglePlayer restart game function
{
    memset(&MCVDeployed, 0, sizeof(MCVDeployed));
    StatsClear();
    Model__ResetVars();
}

void GamePlayStart() // Triggers when the game-play starts
{
    LoadRulesFromMap();
}

void GameExit() // Clean game exit - GameState = GS_QUIT
{
    SaveDune2000Ini();
    if (UseImprovedFPSLimiter)
        timeEndPeriod(4);
}

void GameStart()
{
    cinit();
    InitImports();
    InitHighRes();
}