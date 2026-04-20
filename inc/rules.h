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
bool     rulesExt__InfiniteSpice;
uint8_t  rulesExt__infantryReleaseLimit;
uint8_t  rulesExt__infantryReleaseChance;
bool     rulesExt__buildingsAlwaysNeedPrerequisites;
bool     rulesExt__returnCreditsToSpiceStorage;
bool     rulesExt__intervalsAreOffByOneTick;
uint32_t rulesExt__guardModeRadius;
bool     rulesExt__alwaysShowRadar;
uint32_t rulesExt__costPercentageEasy;
uint32_t rulesExt__costPercentageHard;
uint32_t rulesExt__buildSpeedPercentageEasy;
uint32_t rulesExt__buildSpeedPercentageHard;
bool     rulesExt__uncloakRemainingStealthUnit;
uint8_t  rulesExt__maxChatMessages;
bool     rulesExt__showNeutralBecomeHostileMsg;
uint8_t  rulesExt__maxSameSoundsPlaying;
bool     rulesExt__buildQueuesEnabled;
uint8_t  rulesExt__buildQueuesMaxPerFactory;
uint8_t  rulesExt__buildQueuesMaxPerUnitType;
uint8_t  rulesExt__buildQueuesBulkIncrement;
bool     rulesExt__buildQueuesInfinityEnabled;
bool     rulesExt__showEnemyStructureNames;
bool     rulesExt__showNeutralStructureNames;
