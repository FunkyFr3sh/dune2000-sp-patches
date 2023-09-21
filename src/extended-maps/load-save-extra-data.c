#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "../event-system/event-core.h"

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

  // Reset last played property of sounds in sound table
  for (int i = 0; i < _sampletablecount; i++)
    gSampleTable[i]->last_played = 0;
}
