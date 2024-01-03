#include "macros/patch.h"
#include "dune2000.h"
#include "../event-system/event-core.h"

// Set mouse cursor hook

// Custom implementation of function SetMouseCursor
LJMP(0x0044C420, _Mod__SetMouseCursor);

void Mod__SetMouseCursor(int cursor)
{
  if ( gUIMgr->dw_field_110_index )
  {
    _MouseCursorID = 0;
    _StoredMouseCursorIndex = cursor;
  }
  else
  {
    cursor = ExecuteEventHook(HOOK_SETMOUSECURSOR, 1, cursor, 0, 0);
    _StoredMouseCursorIndex = cursor;
    _MouseCursorID = cursor;
  }
}

// Update unit hooks

CALL(0x00458F15, _Ext_UpdateUnit); // ModelUpdates

char Ext_UpdateUnit(Unit *unit, eSideType side_id, __int16 myIndex)
{
  side_id &= 255;
  if (ExecuteEventHook(HOOK_PREUPDATEUNIT, 3, 0, side_id, myIndex))
    return 1;
  int result = UpdateUnit(unit, side_id, myIndex);
  return ExecuteEventHook(HOOK_POSTUPDATEUNIT, 3, result, side_id, myIndex);
}

// Update building hooks

CALL(0x00458FEC, _Ext_UpdateBuilding); // ModelUpdates

bool Ext_UpdateBuilding(Building *bld, int side_id, __int16 building_index)
{
  side_id &= 255;
  if (ExecuteEventHook(HOOK_PREUPDATEBUILDING, 3, 0, side_id, building_index))
    return 1;
  int result = UpdateBuilding(bld, side_id, building_index);
  return ExecuteEventHook(HOOK_POSTUPDATEBUILDING, 3, result, side_id, building_index);
}

// Update bullet hooks

CALL(0x004590A9, _Ext_UpdateBullet); // ModelUpdates

char Ext_UpdateBullet(Bullet *bul, int side_id)
{
  side_id &= 255;
  if (ExecuteEventHook(HOOK_PREUPDATEBULLET, 3, 0, side_id, bul->MyIndex))
    return 1;
  int result = UpdateBullet(bul, side_id);
  return ExecuteEventHook(HOOK_POSTUPDATEBULLET, 3, result, side_id, bul->MyIndex);
}

// Update explosion hooks

CALL(0x004590EC, _Ext_UpdateExplosion); // ModelUpdates

bool Ext_UpdateExplosion(Explosion *exp, int side_id)
{
  side_id &= 255;
  if (ExecuteEventHook(HOOK_PREUPDATEEXPLOSION, 3, 0, side_id, exp->MyIndex))
    return 1;
  int result = UpdateExplosion(exp, side_id);
  return ExecuteEventHook(HOOK_POSTUPDATEEXPLOSION, 3, result, side_id, exp->MyIndex);
}

// Build speed hooks

CALL(0x004469DE, _Ext_GetUnitBuildSpeedPercentage); // DoIconBuild

unsigned int Ext_GetUnitBuildSpeedPercentage(unsigned char unit_type, unsigned char side_id)
{
  return ExecuteEventHook(HOOK_GETUNITBUILDSPEEDPERCENTAGE, 3, GetUnitBuildSpeedPercentage(unit_type, side_id), side_id, unit_type);
}

CALL(0x0044683D, _Ext_GetBuildingBuildSpeedPercentage); // DoIconBuild

unsigned int Ext_GetBuildingBuildSpeedPercentage(unsigned char side_id)
{
  return ExecuteEventHook(HOOK_GETBUILDINGBUILDSPEEDPERCENTAGE, 2, GetBuildingBuildSpeedPercentage(side_id), side_id, 0);
}
