#include <stdint.h>

void LoadRulesFromMap();

extern LPCTSTR rulesIni;
extern LPCTSTR rulesSpawnIni;

// Original rules
extern int harvestUnloadDelay;
extern int harvestBlobValue;
extern int harvestLoadSpiceDelay;
extern int starportUpdateDelay;
extern int starportStockIncreaseDelay;
extern int starportStockIncreaseProb;
extern int starportCostVariationPercent;
extern int starportFrigateDelay;
extern int refineryExplosionOffsetX;
extern int refineryExplosionOffsetY;
extern int HarvesterDriveDistance;
extern int RepairDriveDistance;
extern int BuildingRepairValue;
extern int UnitRepairValue;
extern int SinglePlayerDelay;
extern uint8_t NumberOfFremen;
extern uint8_t SandWormAppetite;
extern uint8_t SandWormInitialSleep;
extern uint8_t SandWormFedSleep;
extern uint8_t SandWormShotSleep;
extern uint8_t NumberOfCrates;
extern bool CratesPerPlayer;
extern uint8_t DevastatorExplodeDelay;
extern int IgnoreDistance;
extern int CrateCash;
extern bool ShowWarnings;
extern uint8_t DeathHandAccuracy;

// Newly added rules
extern bool     rulesExt__InfiniteSpice;
extern uint8_t  rulesExt__infantryReleaseLimit;
extern uint8_t  rulesExt__infantryReleaseChance;
extern bool     rulesExt__buildingsAlwaysNeedPrerequisites;
extern bool     rulesExt__returnCreditsToSpiceStorage;
extern bool     rulesExt__intervalsAreOffByOneTick;
extern uint32_t rulesExt__guardModeRadius;
extern bool     rulesExt__alwaysShowRadar;
