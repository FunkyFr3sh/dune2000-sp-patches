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

void LoadSavedGame(const char *saveGamePath)
{
    LoadRulesFromMap();
    StatsClear();
}

void WriteSaveGame(const char *saveGamePath, const char *saveGameName)
{

}

void GamePlayRestart() // Skirmish/SinglePlayer restart game function
{
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
    if (UseImprovedFPSLimiter) timeEndPeriod(4);
}
