#include "macros/patch.h"
#include "dune2000.h"
#include "hotkeys.h"

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
  if ( !_KeyboardKeyState[Hotkey_SHIFT] )
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
        && (!_KeyboardKeyState[Hotkey_ALT] || _templates_unitattribs[unit->Type].__PrimaryWeapon != -1)
        // New logic end
        )
      {
        unit->__IsSelected = 1;
      }
    }
  }
  return result;
}

// Custom implementation of function CSide__SelectUnitsByGroupId
DETOUR(0x0046CB40, 0x0046CBCA, _Mod__CSide__SelectUnitsByGroupId);

char __thiscall Mod__CSide__SelectUnitsByGroupId(CSide *this, char group_id, char deselect_others)
{
  Unit *unit; // edx
  Unit *unit_; // esi
  signed int v5; // edi
  Building *bld; // ecx

  unit = this->__FirstUnitPtr;
  unit_ = this->__FirstUnitPtr;
  v5 = 0;
  if ( unit_ )
  {
    while ( !v5 )
    {
      // New logic start
      // Exclude dead unit from group selection
      if ( unit_->__GroupID == group_id && unit_->State != UNIT_STATE_8_LEAVING_BUILDING && unit_->State != UNIT_STATE_17_DEAD )
      // New logic end
      {
        v5 = 1;
      }
      unit_ = unit_->Next;
      if ( !unit_ )
      {
        goto LABEL_7;
      }
    }
  }
  else
  {
LABEL_7:
    if ( !v5 )
    {
      return 0;
    }
  }
  for ( ; unit; unit = unit->Next )
  {
    // New logic start
    // Exclude dead unit from group selection
    if ( unit->__GroupID != group_id || unit->State == UNIT_STATE_8_LEAVING_BUILDING || unit->State == UNIT_STATE_17_DEAD )
    // New logic end
    {
      if ( deselect_others )
      {
        unit->__IsSelected = 0;
      }
    }
    else if ( !(unit->Flags & (UFLAGS_100_CARRYING|UFLAGS_40_FLYING)) )
    {
      unit->__IsSelected = 1;
    }
  }
  for ( bld = this->__FirstBuildingPtr; bld; bld = bld->Next )
  {
    bld->__IsSelected = 0;
  }
  return 1;
}
