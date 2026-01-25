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
  _WriteFile(&_gVariables, sizeof(_gVariables), 1, file);
  _WriteFile(&rulesExt__InfiniteSpice, sizeof(rulesExt__InfiniteSpice), 1, file);
  _WriteFile(&rulesExt__infantryReleaseLimit, sizeof(rulesExt__infantryReleaseLimit), 1, file);
  _WriteFile(&rulesExt__infantryReleaseChance, sizeof(rulesExt__infantryReleaseChance), 1, file);
  _WriteFile(&rulesExt__buildingsAlwaysNeedPrerequisites, sizeof(rulesExt__buildingsAlwaysNeedPrerequisites), 1, file);
  _WriteFile(&rulesExt__returnCreditsToSpiceStorage, sizeof(rulesExt__returnCreditsToSpiceStorage), 1, file);
  _WriteFile(&rulesExt__intervalsAreOffByOneTick, sizeof(rulesExt__intervalsAreOffByOneTick), 1, file);
  _WriteFile(&rulesExt__guardModeRadius, sizeof(rulesExt__guardModeRadius), 1, file);
  _WriteFile(&rulesExt__alwaysShowRadar, sizeof(rulesExt__alwaysShowRadar), 1, file);
  _WriteFile(&rulesExt__costPercentageEasy, sizeof(rulesExt__costPercentageEasy), 1, file);
  _WriteFile(&rulesExt__costPercentageHard, sizeof(rulesExt__costPercentageHard), 1, file);
  _WriteFile(&rulesExt__buildSpeedPercentageEasy, sizeof(rulesExt__buildSpeedPercentageEasy), 1, file);
  _WriteFile(&rulesExt__buildSpeedPercentageHard, sizeof(rulesExt__buildSpeedPercentageHard), 1, file);
  _WriteFile(&rulesExt__uncloakRemainingStealthUnit, sizeof(rulesExt__uncloakRemainingStealthUnit), 1, file);
  _WriteFile(&rulesExt__maxChatMessages, sizeof(rulesExt__maxChatMessages), 1, file);
  _WriteFile(&rulesExt__showNeutralBecomeHostileMsg, sizeof(rulesExt__showNeutralBecomeHostileMsg), 1, file);
  _WriteFile(&rulesExt__maxSameSoundsPlaying, sizeof(rulesExt__maxSameSoundsPlaying), 1, file);
  _WriteFile(&rulesExt__buildQueuesEnabled, sizeof(rulesExt__buildQueuesEnabled), 1, file);
  _WriteFile(&rulesExt__buildQueuesMaxPerFactory, sizeof(rulesExt__buildQueuesMaxPerFactory), 1, file);
  _WriteFile(&rulesExt__buildQueuesMaxPerUnitType, sizeof(rulesExt__buildQueuesMaxPerUnitType), 1, file);
  _WriteFile(&rulesExt__buildQueuesBulkIncrement, sizeof(rulesExt__buildQueuesBulkIncrement), 1, file);
  _WriteFile(&rulesExt__buildQueuesInfinityEnabled, sizeof(rulesExt__buildQueuesInfinityEnabled), 1, file);
}

CALL(0x00441C79, _LoadGameExtraData); // LoadGame

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
  _ReadFile(&_gVariables, sizeof(_gVariables), 1, file);
  _ReadFile(&rulesExt__InfiniteSpice, sizeof(rulesExt__InfiniteSpice), 1, file);
  _ReadFile(&rulesExt__infantryReleaseLimit, sizeof(rulesExt__infantryReleaseLimit), 1, file);
  _ReadFile(&rulesExt__infantryReleaseChance, sizeof(rulesExt__infantryReleaseChance), 1, file);
  _ReadFile(&rulesExt__buildingsAlwaysNeedPrerequisites, sizeof(rulesExt__buildingsAlwaysNeedPrerequisites), 1, file);
  _ReadFile(&rulesExt__returnCreditsToSpiceStorage, sizeof(rulesExt__returnCreditsToSpiceStorage), 1, file);
  _ReadFile(&rulesExt__intervalsAreOffByOneTick, sizeof(rulesExt__intervalsAreOffByOneTick), 1, file);
  _ReadFile(&rulesExt__guardModeRadius, sizeof(rulesExt__guardModeRadius), 1, file);
  _ReadFile(&rulesExt__alwaysShowRadar, sizeof(rulesExt__alwaysShowRadar), 1, file);
  _ReadFile(&rulesExt__costPercentageEasy, sizeof(rulesExt__costPercentageEasy), 1, file);
  _ReadFile(&rulesExt__costPercentageHard, sizeof(rulesExt__costPercentageHard), 1, file);
  _ReadFile(&rulesExt__buildSpeedPercentageEasy, sizeof(rulesExt__buildSpeedPercentageEasy), 1, file);
  _ReadFile(&rulesExt__buildSpeedPercentageHard, sizeof(rulesExt__buildSpeedPercentageHard), 1, file);
  _ReadFile(&rulesExt__uncloakRemainingStealthUnit, sizeof(rulesExt__uncloakRemainingStealthUnit), 1, file);
  _ReadFile(&rulesExt__maxChatMessages, sizeof(rulesExt__maxChatMessages), 1, file);
  _ReadFile(&rulesExt__showNeutralBecomeHostileMsg, sizeof(rulesExt__showNeutralBecomeHostileMsg), 1, file);
  _ReadFile(&rulesExt__maxSameSoundsPlaying, sizeof(rulesExt__maxSameSoundsPlaying), 1, file);
  _ReadFile(&rulesExt__buildQueuesEnabled, sizeof(rulesExt__buildQueuesEnabled), 1, file);
  _ReadFile(&rulesExt__buildQueuesMaxPerFactory, sizeof(rulesExt__buildQueuesMaxPerFactory), 1, file);
  _ReadFile(&rulesExt__buildQueuesMaxPerUnitType, sizeof(rulesExt__buildQueuesMaxPerUnitType), 1, file);
  _ReadFile(&rulesExt__buildQueuesBulkIncrement, sizeof(rulesExt__buildQueuesBulkIncrement), 1, file);
  _ReadFile(&rulesExt__buildQueuesInfinityEnabled, sizeof(rulesExt__buildQueuesInfinityEnabled), 1, file);

  // Reset MapScrollLockTicks
  MapScrollLockTicks = 0;

  // Reset last played property of sounds in sound table
  for (int i = 0; i < _sampletablecount; i++)
    gSampleTable[i]->last_played = 0;
}
