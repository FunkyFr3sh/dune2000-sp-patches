#include <windows.h>
#include "dune2000.h"

// Add more debug information to investigate "unit->pos.steps > 255" error

void DebugUnitCrash(Unit *unit)
{
  DebugFatal("MoveUnitExtraDebug", "speed: %x (%d) posx: %x (%d) posx2: %x (%d) posy: %x (%d) posy2: %x (%d)", unit->Speed, unit->Speed >> 12, unit->__PosX, unit->__PosX >> 21, unit->__posx2, unit->__posx2 >> 21, unit->__PosY, unit->__PosY >> 21, unit->__posy2, unit->__posy2 >> 21);
}
