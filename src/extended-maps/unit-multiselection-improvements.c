#include "macros/patch.h"
#include "dune2000.h"

// Custom implementation of function SelectAllUnitsInArea
DETOUR(0x004A5AE0, 0x004A5BF2, _Mod__SelectAllUnitsInArea);
bool Mod__SelectAllUnitsInArea(int min_x, int min_y, int max_x, int max_y)
{
  int min_x_actual; // ebp
  int min_y_actual; // eax
  CSide *side; // edi
  bool result; // bl
  Unit *unit; // esi MAPDST
  int unit_state; // eax MAPDST
  int unit_pos_x; // ecx MAPDST
  int unit_pos_y; // eax MAPDST

  min_x_actual = min_x;
  if ( min_x > max_x )
  {
    min_x_actual = max_x;
    max_x = min_x;
  }
  min_y_actual = min_y;
  if ( min_y > max_y )
  {
    min_y = max_y;
    max_y = min_y_actual;
  }
  side = GetSide(gSideId);
  result = 0;
  unit = side->__FirstUnitPtr;
  if ( unit )
  {
    while ( !result )
    {
      unit_state = unit->State;
      if ( (unit_state != UNIT_STATE_8_LEAVING_BUILDING || unit->pos_steps) && unit_state != UNIT_STATE_17_DEAD )
      {
        unit_pos_x = unit->__PosX / 0x10000;
        unit_pos_y = unit->__PosY / 0x10000;
        if ( unit_pos_x > min_x_actual && unit_pos_x < max_x && unit_pos_y > min_y && unit_pos_y < max_y )
        {
          result = 1;
        }
      }
      unit = unit->Next;
      if ( !unit )
      {
        goto LABEL_16;
      }
    }
  }
  else
  {
LABEL_16:
    if ( !result )
    {
      return result;
    }
  }
  // New logic start
  // Additive multiselection of units when Shift key is held
  if ( !_KeyboardKeyState[VK_SHIFT] )
    DeselectAllForAllSides();
  // New logic end
  for ( unit = side->__FirstUnitPtr; unit; unit = unit->Next )
  {
    unit_state = unit->State;
    if ( (unit_state != UNIT_STATE_8_LEAVING_BUILDING || unit->pos_steps) && unit_state != UNIT_STATE_17_DEAD )
    {
      unit_pos_x = unit->__PosX / 0x10000;
      unit_pos_y = unit->__PosY / 0x10000;
      if ( unit_pos_x > min_x_actual
        && unit_pos_x < max_x
        && unit_pos_y > min_y
        && unit_pos_y < max_y
        && !(unit->Flags & (UFLAGS_100_CARRYING|UFLAGS_40_FLYING))
        // New logic start
        // Exclude non-armed units from multiselection when Alt key is held
        && (!_KeyboardKeyState[VK_MENU] || _templates_unitattribs[unit->Type].__PrimaryWeapon != -1)
        // New logic end
        )
      {
        unit->__IsSelected = 1;
      }
    }
  }
  return result;
}
