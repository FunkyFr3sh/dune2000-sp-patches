#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"

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
}

CALL(0x00441C79, _LoadGameExtraData); // LoadGame

void LoadGameExtraData(void *buffer, size_t size, size_t count, FILE *file)
{
  (void)buffer;
  (void)size;
  (void)count;
  // Call which got replaced
  _ReadFile(&gNetWorms, 1, 1, file);
  // Write font palettes
  for (int i = 0; i < 16; i++)
    _ReadFile(_FontPals[i], gBitsPerPixel * 2, 1, file);
}
