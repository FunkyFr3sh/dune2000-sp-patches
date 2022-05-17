#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"
#include "rules.h"
#include "utils.h"

LPCTSTR rulesIni = ".\\rules.ini";
LPCTSTR rulesSpawnIni = ".\\rules-spawn.ini";

// Newly added rules
bool      rulesExt__InfiniteSpice               = false;
uint8_t   rulesExt__infantryReleaseLimit        = 4;
uint8_t   rulesExt__infantryReleaseChance       = 5;
bool      rulesExt__buildingsAlwaysNeedPrerequisites = false;
bool      rulesExt__returnCreditsToSpiceStorage = false;
bool      rulesExt__intervalsAreOffByOneTick    = true;
uint32_t  rulesExt__guardModeRadius             = 192;
bool      rulesExt__alwaysShowRadar             = false;
uint32_t  rulesExt__costPercentageEasy          = 75;
uint32_t  rulesExt__costPercentageHard          = 125;
uint32_t  rulesExt__buildSpeedPercentageEasy    = 125;
uint32_t  rulesExt__buildSpeedPercentageHard    = 75;
bool      rulesExt__uncloakRemainingStealthUnit = true;

static void LoadVars(LPCTSTR fileName);

void LoadRulesFromMap()
{
    Mission__LoadVarsFile();
    LoadVars(SpawnerActive ? rulesSpawnIni : rulesIni);
    char mapIniPath[256];
    sprintf(mapIniPath, ".\\%s%s", GameType == GT_SINGLEPLAYER ? MissionsResourcePath : MapsResourcePath, PathChangeExtension(MissionMap, ".ini"));
    LoadVars(mapIniPath);
}

static void LoadVars(LPCTSTR fileName)
{
    harvestUnloadDelay = IniGetInt("Vars", "harvestUnloadDelay", harvestUnloadDelay, fileName);
    harvestBlobValue = IniGetInt("Vars", "harvestBlobValue", harvestBlobValue, fileName);
    harvestLoadSpiceDelay = IniGetInt("Vars", "harvestLoadSpiceDelay", harvestLoadSpiceDelay, fileName);
    starportUpdateDelay = IniGetInt("Vars", "starportUpdateDelay", starportUpdateDelay, fileName);
    starportStockIncreaseDelay = IniGetInt("Vars", "starportStockIncreaseDelay", starportStockIncreaseDelay, fileName);
    starportStockIncreaseProb = IniGetInt("Vars", "starportStockIncreaseProb", starportStockIncreaseProb, fileName);
    starportCostVariationPercent = IniGetInt("Vars", "starportCostVariationPercent", starportCostVariationPercent, fileName);
    starportFrigateDelay = IniGetInt("Vars", "starportFrigateDelay", starportFrigateDelay, fileName);
    refineryExplosionOffsetX = IniGetInt("Vars", "refineryExplosionOffsetX", refineryExplosionOffsetX, fileName);
    refineryExplosionOffsetY = IniGetInt("Vars", "refineryExplosionOffsetY", refineryExplosionOffsetY, fileName);
    HarvesterDriveDistance = IniGetInt("Vars", "HarvesterDriveDistance", HarvesterDriveDistance, fileName);
    RepairDriveDistance = IniGetInt("Vars", "RepairDriveDistance", RepairDriveDistance, fileName);
    BuildingRepairValue = IniGetInt("Vars", "BuildingRepairValue", BuildingRepairValue, fileName);
    UnitRepairValue = IniGetInt("Vars", "UnitRepairValue", UnitRepairValue, fileName);
    SinglePlayerDelay = IniGetInt("Vars", "SinglePlayerDelay", SinglePlayerDelay, fileName);
    NumberOfFremen = IniGetInt("Vars", "NumberOfFremen", NumberOfFremen, fileName);
    SandWormAppetite = IniGetInt("Vars", "SandWormAppetite", SandWormAppetite, fileName);
    SandWormInitialSleep = IniGetInt("Vars", "SandWormInitialSleep", SandWormInitialSleep, fileName);
    SandWormFedSleep = IniGetInt("Vars", "SandWormFedSleep", SandWormFedSleep, fileName);
    SandWormShotSleep = IniGetInt("Vars", "SandWormShotSleep", SandWormShotSleep, fileName);
    NumberOfCrates = IniGetInt("Vars", "NumberOfCrates", NumberOfCrates, fileName);
    CratesPerPlayer = IniGetBool("Vars", "CratesPerPlayer", CratesPerPlayer, fileName);
    DevastatorExplodeDelay = IniGetInt("Vars", "DevastatorExplodeDelay", DevastatorExplodeDelay, fileName);
    IgnoreDistance = IniGetInt("Vars", "IgnoreDistance", IgnoreDistance, fileName);
    CrateCash = IniGetInt("Vars", "CrateCash", CrateCash, fileName);
    ShowWarnings = IniGetBool("Vars", "ShowWarnings", ShowWarnings, fileName);
    DeathHandAccuracy = IniGetInt("Vars", "DeathHandAccuracy", DeathHandAccuracy, fileName);
    
    rulesExt__InfiniteSpice = IniGetBool("Vars", "InfiniteSpice", fileName == rulesSpawnIni || fileName == rulesIni ? false : rulesExt__InfiniteSpice, fileName);
    rulesExt__infantryReleaseLimit = IniGetInt("Vars", "infantryReleaseLimit", rulesExt__infantryReleaseLimit, fileName);
    rulesExt__infantryReleaseChance = IniGetInt("Vars", "infantryReleaseChance", rulesExt__infantryReleaseChance, fileName);
    rulesExt__buildingsAlwaysNeedPrerequisites = IniGetBool("Vars", "buildingsAlwaysNeedPrerequisites", rulesExt__buildingsAlwaysNeedPrerequisites, fileName);
    rulesExt__returnCreditsToSpiceStorage = IniGetBool("Vars", "returnCreditsToSpiceStorage", rulesExt__returnCreditsToSpiceStorage, fileName);
    rulesExt__intervalsAreOffByOneTick = IniGetBool("Vars", "intervalsAreOffByOneTick", rulesExt__intervalsAreOffByOneTick, fileName);
    rulesExt__guardModeRadius = IniGetInt("Vars", "guardModeRadius", rulesExt__guardModeRadius, fileName);
    rulesExt__alwaysShowRadar = IniGetBool("Vars", "alwaysShowRadar", rulesExt__alwaysShowRadar, fileName);
    rulesExt__costPercentageEasy = IniGetInt("Vars", "costPercentageEasy", rulesExt__costPercentageEasy, fileName);
    rulesExt__costPercentageHard = IniGetInt("Vars", "costPercentageHard", rulesExt__costPercentageHard, fileName);
    rulesExt__buildSpeedPercentageEasy = IniGetInt("Vars", "buildSpeedPercentageEasy", rulesExt__buildSpeedPercentageEasy, fileName);
    rulesExt__buildSpeedPercentageHard = IniGetInt("Vars", "buildSpeedPercentageHard", rulesExt__buildSpeedPercentageHard, fileName);
    rulesExt__uncloakRemainingStealthUnit = IniGetBool("Vars", "uncloakRemainingStealthUnit", rulesExt__uncloakRemainingStealthUnit, fileName);
}
