#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "../event-system/event-core.h"
#include "radar.h"
#include "rules.h"
#include "extended-templates.h"
#include <patch.h>

CALL(0x00441836, _SaveGameExtraData); // SaveGame

#define SAVE_DATA(var) _WriteFile(&var, sizeof(var), 1, file);

void SaveGameExtraData(void *buffer, size_t size, size_t count, FILE *file)
{
  (void)buffer;
  (void)size;
  (void)count;
  // Call which got replaced
  _WriteFile(&gNetWorms, 1, 1, file);
  // Write font palettes
  for (int i = 0; i < 16; i++)
    _WriteFile(_FontPals[i], gBitsPerPixel * 2, 1, file);
  // Write event variables
  _WriteFile(gEventVariableArray, sizeof(gEventVariableArray), 1, file);
  // Write event extra data
  _WriteFile(gEventExtraData, sizeof(gEventExtraData), 1, file);
  // Write event hooks
  _WriteFile(event_hooks, sizeof(event_hooks), 1, file);
  // Write map scroll data
  _WriteFile(&map_scroll_data, sizeof(map_scroll_data), 1, file);
  // Write radar markers
  _WriteFile(gRadarMarkers, sizeof(gRadarMarkers), 1, file);
  // Write side extra data
  _WriteFile(gSideExtraData, sizeof(gSideExtraData), 1, file);
  // Write temaplates
  _WriteFile(_templates_unitattribs, sizeof(_templates_unitattribs), 1, file);
  _WriteFile(_templates_buildattribs, sizeof(_templates_buildattribs), 1, file);
  _WriteFile(_templates_bulletattribs, sizeof(_templates_bulletattribs), 1, file);
  _WriteFile(_templates_explosionattribs, sizeof(_templates_explosionattribs), 1, file);
  _WriteFile(_WarheadData, sizeof(_WarheadData), 1, file);
  _WriteFile(_speed_values, sizeof(_speed_values), 1, file);
  // Write rules
  SAVE_DATA(_gVariables)
  SAVE_DATA(rulesExt__InfiniteSpice)
  SAVE_DATA(rulesExt__infantryReleaseLimit)
  SAVE_DATA(rulesExt__infantryReleaseChance)
  SAVE_DATA(rulesExt__buildingsAlwaysNeedPrerequisites)
  SAVE_DATA(rulesExt__returnCreditsToSpiceStorage)
  SAVE_DATA(rulesExt__intervalsAreOffByOneTick)
  SAVE_DATA(rulesExt__guardModeRadius)
  SAVE_DATA(rulesExt__alwaysShowRadar)
  SAVE_DATA(rulesExt__costPercentageEasy)
  SAVE_DATA(rulesExt__costPercentageHard)
  SAVE_DATA(rulesExt__buildSpeedPercentageEasy)
  SAVE_DATA(rulesExt__buildSpeedPercentageHard)
  SAVE_DATA(rulesExt__uncloakRemainingStealthUnit)
  SAVE_DATA(rulesExt__maxChatMessages)
  SAVE_DATA(rulesExt__showNeutralBecomeHostileMsg)
  SAVE_DATA(rulesExt__maxSameSoundsPlaying)
  SAVE_DATA(rulesExt__buildQueuesEnabled)
  SAVE_DATA(rulesExt__buildQueuesMaxPerFactory)
  SAVE_DATA(rulesExt__buildQueuesMaxPerUnitType)
  SAVE_DATA(rulesExt__buildQueuesBulkIncrement)
  SAVE_DATA(rulesExt__buildQueuesInfinityEnabled)
  SAVE_DATA(rulesExt__showEnemyStructureNames)
  SAVE_DATA(rulesExt__showNeutralStructureNames)
  SAVE_DATA(rulesExt__deliverEveryOtherEnemyUnitOnEasy)
  SAVE_DATA(rulesExt__harvsUnloadOnlyIfEnoughStorage)
  SAVE_DATA(rulesExt__harvsCanBeOrderedToUndock)
  SAVE_DATA(rulesExt__separateBuildingVoiceLines)
  // Extra dummy bytes, to be replaced by new rules in future
  char dummy[28] = {0};
  _WriteFile(dummy, sizeof(dummy), 1, file);
}

CALL(0x00441C79, _LoadGameExtraData); // LoadGame

#define LOAD_DATA(var) _ReadFile(&var, sizeof(var), 1, file);

void LoadGameExtraData(void *buffer, size_t size, size_t count, FILE *file)
{
  (void)buffer;
  (void)size;
  (void)count;
  // Call which got replaced
  _ReadFile(&gNetWorms, 1, 1, file);
  // Read font palettes
  for (int i = 0; i < 16; i++)
    _ReadFile(_FontPals[i], gBitsPerPixel * 2, 1, file);
  // Read event variables
  _ReadFile(gEventVariableArray, sizeof(gEventVariableArray), 1, file);
  // Read event extra data
  _ReadFile(gEventExtraData, sizeof(gEventExtraData), 1, file);
  // Read event hooks
  _ReadFile(event_hooks, sizeof(event_hooks), 1, file);
  // Read map scroll data
  _ReadFile(&map_scroll_data, sizeof(map_scroll_data), 1, file);
  // Read radar markers
  _ReadFile(gRadarMarkers, sizeof(gRadarMarkers), 1, file);
  // Read side extra data
  _ReadFile(gSideExtraData, sizeof(gSideExtraData), 1, file);
  // Read temaplates
  _ReadFile(_templates_unitattribs, sizeof(_templates_unitattribs), 1, file);
  _ReadFile(_templates_buildattribs, sizeof(_templates_buildattribs), 1, file);
  _ReadFile(_templates_bulletattribs, sizeof(_templates_bulletattribs), 1, file);
  _ReadFile(_templates_explosionattribs, sizeof(_templates_explosionattribs), 1, file);
  _ReadFile(_WarheadData, sizeof(_WarheadData), 1, file);
  _ReadFile(_speed_values, sizeof(_speed_values), 1, file);
  // Read rules
  LOAD_DATA(_gVariables)
  LOAD_DATA(rulesExt__InfiniteSpice)
  LOAD_DATA(rulesExt__infantryReleaseLimit)
  LOAD_DATA(rulesExt__infantryReleaseChance)
  LOAD_DATA(rulesExt__buildingsAlwaysNeedPrerequisites)
  LOAD_DATA(rulesExt__returnCreditsToSpiceStorage)
  LOAD_DATA(rulesExt__intervalsAreOffByOneTick)
  LOAD_DATA(rulesExt__guardModeRadius)
  LOAD_DATA(rulesExt__alwaysShowRadar)
  LOAD_DATA(rulesExt__costPercentageEasy)
  LOAD_DATA(rulesExt__costPercentageHard)
  LOAD_DATA(rulesExt__buildSpeedPercentageEasy)
  LOAD_DATA(rulesExt__buildSpeedPercentageHard)
  LOAD_DATA(rulesExt__uncloakRemainingStealthUnit)
  LOAD_DATA(rulesExt__maxChatMessages)
  LOAD_DATA(rulesExt__showNeutralBecomeHostileMsg)
  LOAD_DATA(rulesExt__maxSameSoundsPlaying)
  LOAD_DATA(rulesExt__buildQueuesEnabled)
  LOAD_DATA(rulesExt__buildQueuesMaxPerFactory)
  LOAD_DATA(rulesExt__buildQueuesMaxPerUnitType)
  LOAD_DATA(rulesExt__buildQueuesBulkIncrement)
  LOAD_DATA(rulesExt__buildQueuesInfinityEnabled)
  LOAD_DATA(rulesExt__showEnemyStructureNames)
  LOAD_DATA(rulesExt__showNeutralStructureNames)
  LOAD_DATA(rulesExt__deliverEveryOtherEnemyUnitOnEasy)
  LOAD_DATA(rulesExt__harvsUnloadOnlyIfEnoughStorage)
  LOAD_DATA(rulesExt__harvsCanBeOrderedToUndock)
  LOAD_DATA(rulesExt__separateBuildingVoiceLines)
  // Extra dummy bytes, to be replaced by new rules in future
  char dummy[28];
  _ReadFile(dummy, sizeof(dummy), 1, file);

  // Reset last played property of sounds in sound table
  for (int i = 0; i < _sampletablecount; i++)
    gSampleTable[i]->last_played = 0;
}
