#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "../event-system/event-core.h"

bool CheckTerrainRestriction(int tile_flags, int default_flag, int restriction)
{
  if (restriction == 0)
    return (tile_flags & default_flag) != 0;
  else if (restriction < 8)
    return ((tile_flags >> 29) & 7) == restriction;
  else if (restriction < 15)
    return (((tile_flags >> 29) & 7) & (restriction & 7)) == (restriction & 7);
  else if (restriction == 15)
    return (tile_flags & TileFlags_10000_SANDY) != 0;
  else
    return false;
};

// Custom implementation of function CanUnitUseSquare
BOOL Mod__CanUnitUseSquare(dwXYStruct x, Unit *unit, eSideType side_id, char route_mode)
{
  Unit *unit_; // ecx
  int v5; // edx
  int tile_flags; // ebx
  unsigned int tile_owner; // edi
  char is_infantry; // dl
  eSideType side_id_; // si
  int v11; // eax
  Building *v12; // eax
  bool v13; // al
  Building *v14; // eax
  Unit *v15; // eax
  Unit *v16; // eax
  Unit *v17; // eax
  char can_crush; // [esp+13h] [ebp-5h]
  unsigned int v20; // [esp+14h] [ebp-4h]

  int movement_restriction = _templates_unitattribs[unit->Type].MovementRestriction;

  unit_ = unit;
  v5 = unit->Type << 8;
  can_crush = *(&_templates_unitattribs[0].__CanCrush + v5);
  is_infantry = *(&_templates_unitattribs[0].__IsInfantry + v5);
  if ( x.X < 0 || x.Y < 0 || x.X >= gGameMap.width || x.Y >= gGameMap.height )
  {
    return 0;
  }
  tile_flags = gGameMap.map[x.X + _CellNumbersWidthSpan[x.Y]].__tile_bitflags;
  tile_owner = gGameMap.map[x.X + _CellNumbersWidthSpan[x.Y]].__tile_bitflags & 7;
  v20 = gGameMap.map[x.X + _CellNumbersWidthSpan[x.Y]].__tile_bitflags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_10_OCC_BUILDING|TileFlags_8_OCC_UNIT);
  if ( *(&_templates_unitattribs[0].__Behavior + v5) == UnitBehavior_SANDWORM )
  {
    if ( tile_flags & TileFlags_10000_SANDY && !(tile_flags & TileFlags_1000000_OCC_SANDWORM) )
    {
      return 1;
    }
    return 0;
  }
  side_id_ = side_id;
  if ( !is_infantry
    && !(tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_8_OCC_UNIT))
    && tile_flags & TileFlags_10_OCC_BUILDING
    && tile_flags & TileFlags_10000000_BUILDING_EXIT
    && (unsigned char)side_id == tile_owner )
  {
    return 1;
  }
  v11 = unit_->State;
  if ( v11 != UNIT_STATE_24_MOVING_TO_REPAIR_PAD )
  {
    if ( v11 == UNIT_STATE_5_ATTACKING_BUILDING )
    {
      if ( !(tile_flags & TileFlags_10_OCC_BUILDING) )
      {
        goto LABEL_34;
      }
      v14 = GetBuilding((eSideType)unit_->EnemySide, unit_->EnemyIndex);
      if ( !v14 )
      {
        goto LABEL_33;
      }
      v13 = BuildingOccupiesTile(v14, x.X, x.Y);
    }
    else
    {
      if ( v11 != UNIT_STATE_4_ATTACKING_UNIT )
      {
        goto LABEL_34;
      }
      if ( abs(unit_->BlockFromX - x.X) <= 1 && abs(unit_->BlockFromY - x.Y) <= 1 )
      {
        goto LABEL_33;
      }
      if ( !(tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_8_OCC_UNIT)) )
      {
        goto LABEL_33;
      }
      if (unit_->EnemyIndex == -1)
      {
        goto LABEL_33;
      }
      v15 = GetUnit((eSideType)unit_->EnemySide, unit_->EnemyIndex);
      if ( !v15 )
      {
        goto LABEL_33;
      }
      v13 = UnitOccupiesTile(v15, x.X, x.Y);
    }
    goto LABEL_31;
  }
  if ( !(tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_8_OCC_UNIT))
    && tile_flags & TileFlags_10_OCC_BUILDING
    && (unsigned char)side_id == tile_owner )
  {
    v12 = GetBuilding(side_id, unit_->RepairPadIndex);
    if ( !v12 )
    {
LABEL_33:
      goto LABEL_34;
    }
    v13 = BuildingOccupiesTile(v12, x.X, x.Y);
LABEL_31:
    if ( v13 )
    {
      return 1;
    }
    goto LABEL_33;
  }
LABEL_34:
  switch ( route_mode )
  {
    case 0:
      if ( is_infantry )
      {
        if ( CheckTerrainRestriction(tile_flags, TileFlags_4000_WALK_ON, movement_restriction) && !(tile_flags & (TileFlags_10_OCC_BUILDING|TileFlags_8_OCC_UNIT)) )
        {
          if ( tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID) )
          {
            if ( tile_owner == (unsigned char)side_id_ )
            {
              return FindFreeSpotForInfantry((TileFlags)tile_flags) != 0;
            }
            return 0;
          }
          return 1;
        }
      }
      else if ( CheckTerrainRestriction(tile_flags, TileFlags_2000_DRIVE_ON, movement_restriction) && !(tile_flags & (TileFlags_10_OCC_BUILDING|TileFlags_8_OCC_UNIT)) )
      {
        if ( tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID) )
        {
          if ( can_crush )
          {
            return _gDiplomacy[(unsigned char)side_id_][tile_owner] != 0;
          }
          return 0;
        }
        return 1;
      }
      return 0;
    case 1:
      if ( is_infantry )
      {
        if ( CheckTerrainRestriction(tile_flags, TileFlags_4000_WALK_ON, movement_restriction) && !(tile_flags & TileFlags_10_OCC_BUILDING) )
        {
          if ( !v20 )
          {
            return 1;
          }
          if ( tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID)
            && tile_owner == (unsigned char)side_id_
            && FindFreeSpotForInfantry((TileFlags)tile_flags) )
          {
            return 1;
          }
          if ( !_gDiplomacy[(unsigned char)side_id_][tile_owner] )
          {
            v16 = GetUnitOnTile(32 * x.X, 32 * x.Y, (eSideType *)&unit, (_WORD *)&side_id, 0);
            if ( v16 )
            {
              return Unit_49E140(v16) != 0;
            }
          }
        }
        return 0;
      }
      if ( !CheckTerrainRestriction(tile_flags, TileFlags_2000_DRIVE_ON, movement_restriction) || tile_flags & TileFlags_10_OCC_BUILDING )
      {
        return 0;
      }
      if ( !v20 )
      {
        return 1;
      }
      if ( !_gDiplomacy[(unsigned char)side_id_][tile_owner] )
      {
        if ( v20 )
        {
          v17 = GetUnitOnTile(32 * x.X, 32 * x.Y, (eSideType *)&unit, (_WORD *)&side_id, 0);
          if ( v17 )
          {
            return Unit_49E140(v17) != 0;
          }
          return 0;
        }
        return 1;
      }
      goto LABEL_77;
    case 2:
      if ( is_infantry )
      {
        if ( CheckTerrainRestriction(tile_flags, TileFlags_4000_WALK_ON, movement_restriction) && !(tile_flags & TileFlags_10_OCC_BUILDING) )
        {
          if ( v20 )
          {
            if ( !_gDiplomacy[(unsigned char)side_id_][tile_owner] )
            {
              return 1;
            }
            return 0;
          }
          return 1;
        }
        return 0;
      }
      if ( !CheckTerrainRestriction(tile_flags, TileFlags_2000_DRIVE_ON, movement_restriction) || tile_flags & TileFlags_10_OCC_BUILDING )
      {
        return 0;
      }
      if ( !v20 || !_gDiplomacy[(unsigned char)side_id_][tile_owner] )
      {
        return 1;
      }
LABEL_77:
      if ( !(tile_flags & TileFlags_8_OCC_UNIT)
        && tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID)
        && can_crush )
      {
        return 1;
      }
      return 0;
    case 3:
      if ( CheckTerrainRestriction(tile_flags, is_infantry?TileFlags_4000_WALK_ON:TileFlags_2000_DRIVE_ON, movement_restriction) )
      {
        if ( tile_flags & TileFlags_400_HAS_WALL )
        {
          if ( !_gDiplomacy[(unsigned char)side_id_][tile_owner] )
          {
            return 0;
          }
          return 1;
        }
        if ( !(tile_flags & TileFlags_10_OCC_BUILDING) )
        {
          return 1;
        }
      }
      break;
    default:
      DebugFatal("CanUnitUseSquare", "Invalid routeMode %d", route_mode);
      break;
  }
  return 0;
}

LJMP(0x0045D990, _Ext__CanUnitUseSquare);

bool Ext__CanUnitUseSquare(dwXYStruct x, Unit *unit, eSideType side_id, char route_mode)
{
  return ExecuteEventHook(HOOK_CANUNITUSESQUARE, 5, Mod__CanUnitUseSquare(x, unit, side_id, route_mode), x.X, x.Y, side_id, unit->MyIndex);
}

// Redirect setting of can't move / move cursor to call of custom function SetMouseCursorForUnitMovementRestriction
// Superseded by Mod__HandleGameLoopEvents
//LJMP(0x004438B2, 0x004466D3); // HandleGameLoopEvents
//LJMP(0x00443896, 0x004466D3); // HandleGameLoopEvents
// Utilize free space between functions for additional call and jump instructions
//CALL(0x004466D3, _SetMouseCursorForUnitMovementRestriction);
//LJMP(0x004466D8, 0x00443D3B); // Jump back and skip call of SetMouseCursor function

void SetMouseCursorForUnitMovementRestriction()
{
  dwXYStruct pos;
  pos.Y = (_gMousePos.y + _ViewportYPos - _OptionsBarHeight) / 32;
  pos.X = (_gMousePos.x + _ViewportXPos) / 32;
  for (Unit *unit = GetSide(gSideId)->__FirstUnitPtr; unit; unit = unit->Next)
  {
    if (!unit->__IsSelected)
      continue;
    if (Ext__CanUnitUseSquare(pos, unit, gSideId, 0))
    {
      SetMouseCursor(CURSOR_MOVE);
      return;
    }
  }
  SetMouseCursor(CURSOR_CANT_MOVE);
}

// Prevent units with special terrain restriction from getting airlifted
CALL(0x0041C153, _Mod__CSide__GetQueuePos); // CAI__DoProtection
CALL(0x0049A78D, _Mod__CSide__GetQueuePos); // UpdateUnit

char __thiscall Mod__CSide__GetQueuePos(CSide *this, Unit *unit)
{
  char result; // al
  unsigned char v3; // bl
  unsigned char a2a; // [esp+8h] [ebp+4h]

  // New logic start
  // Prevent units with special terrain restriction from getting airlifted
  if (_templates_unitattribs[unit->Type].MovementRestriction)
  {
    return -1;
  }
  // New logic end

  if ( unit->State == UNIT_STATE_18_AWAITING_AIRLIFT )
  {
    return 0;
  }
  if ( !this->__CarryallCount )
  {
    return -1;
  }
  v3 = this->__PickupQueueCount;
  result = 0;
  a2a = 0;
  if ( !v3 )
  {
    return -1;
  }
  while ( this->__PickupQueue[a2a].__state )
  {
    a2a = ++result;
    if ( (unsigned char)result >= v3 )
    {
      return -1;
    }
  }
  return result;
}
