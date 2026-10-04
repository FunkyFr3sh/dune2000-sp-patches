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

void InitExtraRules(void)
{
  rulesExt__InfiniteSpice                    = false;
  rulesExt__infantryReleaseLimit             = 4;
  rulesExt__infantryReleaseChance            = 5;
  rulesExt__buildingsAlwaysNeedPrerequisites = false;
  rulesExt__returnCreditsToSpiceStorage      = false;
  rulesExt__intervalsAreOffByOneTick         = true;
  rulesExt__guardModeRadius                  = 192;
  rulesExt__alwaysShowRadar                  = false;
  rulesExt__costPercentageEasy               = 75;
  rulesExt__costPercentageHard               = 125;
  rulesExt__buildSpeedPercentageEasy         = 125;
  rulesExt__buildSpeedPercentageHard         = 75;
  rulesExt__uncloakRemainingStealthUnit      = true;
  rulesExt__maxChatMessages                  = 5;
  rulesExt__showNeutralBecomeHostileMsg      = true;
  rulesExt__maxSameSoundsPlaying             = 3;
  rulesExt__buildQueuesEnabled               = false;
  rulesExt__buildQueuesMaxPerFactory         = 100;
  rulesExt__buildQueuesMaxPerUnitType        = 10;
  rulesExt__buildQueuesBulkIncrement         = 5;
  rulesExt__buildQueuesInfinityEnabled       = false;
  rulesExt__showEnemyStructureNames          = false;
  rulesExt__showNeutralStructureNames        = false;
  rulesExt__deliverEveryOtherEnemyUnitOnEasy = true;
  rulesExt__harvsUnloadOnlyIfEnoughStorage   = false;
  rulesExt__separateBuildingVoiceLines       = false;
}

static void LoadVars(LPCTSTR fileName);
static void LoadMultiPlayerSettings(LPCTSTR fileName);

void LoadRulesFromMap()
{
    ReadVariables();
    InitExtraRules();
    LoadVars(SpawnerActive ? rulesSpawnIni : rulesIni);
    char mapIniPath[256];
    sprintf(mapIniPath, ".\\%s%s", gGameType == GAME_CAMPAIGN ? gMISSIONS_RES_PATH : gMAPS_RES_PATH, PathChangeExtension(MissionMap, ".ini"));
    LoadVars(mapIniPath);
    LoadMultiPlayerSettings(mapIniPath);
}

static void LoadMultiPlayerSettings(LPCTSTR fileName)
{
    gNetCrates = IniGetBool("MultiPlayer", "Crates", gNetCrates, fileName);
    gNetWorms = IniGetInt("MultiPlayer", "Worms", gNetWorms, fileName);
    gNetStartingCredits = IniGetInt("MultiPlayer", "Credits", gNetStartingCredits, fileName);
    gNetTechLevel = IniGetInt("MultiPlayer", "TechLevel", gNetTechLevel, fileName);
    gNetUnitCount = IniGetInt("MultiPlayer", "UnitCount", gNetUnitCount, fileName);
    StartWithMCV = IniGetBool("MultiPlayer", "StartWithMCV", true, fileName);
    UseDefaultWinLoseEvents = IniGetBool("MultiPlayer", "UseDefaultWinLoseEvents", false, fileName);
    
    int maxIcons = (GameHeight - 212) / SideBarIconHeight;
    int iconCount = IniGetInt("MultiPlayer", "SidebarIconCount", maxIcons, fileName);
    SideBarIconCount = iconCount > maxIcons ? maxIcons : iconCount < 1 ? 1 : iconCount;
}

#define LOAD_RULE(rule, type)                    rule = IniGet##type("Vars", #rule,             rule, fileName);
#define LOAD_CUSTOM_RULE(rule, type) rulesExt__##rule = IniGet##type("Vars", #rule, rulesExt__##rule, fileName);

static void LoadVars(LPCTSTR fileName)
{
    LOAD_RULE(harvestUnloadDelay, Int)
    LOAD_RULE(harvestBlobValue, Int)
    LOAD_RULE(harvestLoadSpiceDelay, Int)
    LOAD_RULE(starportUpdateDelay, Int)
    LOAD_RULE(starportStockIncreaseDelay, Int);
    LOAD_RULE(starportStockIncreaseProb, Int)
    LOAD_RULE(starportCostVariationPercent, Int)
    LOAD_RULE(starportFrigateDelay, Int)
    LOAD_RULE(refineryExplosionOffsetX, Int)
    LOAD_RULE(refineryExplosionOffsetY, Int)
    LOAD_RULE(HarvesterDriveDistance, Int)
    LOAD_RULE(RepairDriveDistance, Int)
    LOAD_RULE(BuildingRepairValue, Int)
    LOAD_RULE(UnitRepairValue, Int)
    LOAD_RULE(SinglePlayerDelay, Int)
    LOAD_RULE(NumberOfFremen, Int)
    LOAD_RULE(SandWormAppetite, Int)
    LOAD_RULE(SandWormInitialSleep, Int)
    LOAD_RULE(SandWormFedSleep, Int)
    LOAD_RULE(SandWormShotSleep, Int)
    LOAD_RULE(NumberOfCrates, Int)
    LOAD_RULE(CratesPerPlayer, Bool)
    LOAD_RULE(DevastatorExplodeDelay, Int)
    LOAD_RULE(IgnoreDistance, Int)
    LOAD_RULE(CrateCash, Int)
    LOAD_RULE(ShowWarnings, Bool)
    LOAD_RULE(DeathHandAccuracy, Int)
    
    LOAD_CUSTOM_RULE(InfiniteSpice, Bool)
    LOAD_CUSTOM_RULE(infantryReleaseLimit, Int)
    LOAD_CUSTOM_RULE(infantryReleaseChance, Int)
    LOAD_CUSTOM_RULE(buildingsAlwaysNeedPrerequisites, Bool)
    LOAD_CUSTOM_RULE(returnCreditsToSpiceStorage, Bool)
    LOAD_CUSTOM_RULE(intervalsAreOffByOneTick, Bool)
    LOAD_CUSTOM_RULE(guardModeRadius, Int)
    LOAD_CUSTOM_RULE(alwaysShowRadar, Bool)
    LOAD_CUSTOM_RULE(costPercentageEasy, Int)
    LOAD_CUSTOM_RULE(costPercentageHard, Int)
    LOAD_CUSTOM_RULE(buildSpeedPercentageEasy, Int)
    LOAD_CUSTOM_RULE(buildSpeedPercentageHard, Int)
    LOAD_CUSTOM_RULE(uncloakRemainingStealthUnit, Bool)
    LOAD_CUSTOM_RULE(maxChatMessages, Int)
    LOAD_CUSTOM_RULE(showNeutralBecomeHostileMsg, Bool)
    LOAD_CUSTOM_RULE(maxSameSoundsPlaying, Int)
    LOAD_CUSTOM_RULE(buildQueuesEnabled, Bool)
    LOAD_CUSTOM_RULE(buildQueuesMaxPerFactory, Int)
    LOAD_CUSTOM_RULE(buildQueuesMaxPerUnitType, Int)
    LOAD_CUSTOM_RULE(buildQueuesBulkIncrement, Int)
    LOAD_CUSTOM_RULE(buildQueuesInfinityEnabled, Bool)
    LOAD_CUSTOM_RULE(showEnemyStructureNames, Bool)
    LOAD_CUSTOM_RULE(showNeutralStructureNames, Bool)
    LOAD_CUSTOM_RULE(deliverEveryOtherEnemyUnitOnEasy, Bool)
    LOAD_CUSTOM_RULE(harvsUnloadOnlyIfEnoughStorage, Bool)
    LOAD_CUSTOM_RULE(separateBuildingVoiceLines, Bool)
}
