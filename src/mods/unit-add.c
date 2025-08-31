#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "stats.h"

// Custom implementation of function ModelAddUnit
DETOUR(0x00455870, 0x00455FBD, _Mod__ModelAddUnit);

short Mod__ModelAddUnit(eSideType side_id, unsigned char unit_type, unsigned char add_at_x, unsigned char add_at_y, unsigned char move_to_x, unsigned char move_to_y, int pixel_offset_x, int pixel_offset_y)
{
  int unit_type_offset; // ebp
  char behavior; // al MAPDST
  int add_at_y_; // esi
  TileFlags tile_flags; // eax MAPDST
  CSide *side; // eax MAPDST
  Unit *unit; // esi
  int pixel_offset_x_; // ecx
  int pixel_offset_y_; // edx
  unsigned char facing; // al
  int *v20; // eax
  int v21; // edx
  unsigned int v23; // ecx
  char pickup_queue_count; // al
  char v25; // dl
  char facing_; // al
  char x; // al
  short y; // cx
  char endloop; // dl
  TileFlags check_flag; // ecx
  int y_; // edi
  int x_; // eax
  int cell_index; // eax
  char cellspot; // al
  int spot_offset_index; // eax
  int v41; // eax
  UnitFlags unit_flags; // eax MAPDST
  int string_id; // eax
  char *string; // eax

  unit_type_offset = unit_type << 8;
  behavior = *(&_templates_unitattribs[0].__Behavior + unit_type_offset);
  if ( behavior != UnitBehavior_CARRYALL
    && behavior != UnitBehavior_FRIGATE
    && behavior != UnitBehavior_ORNITHOPTER
    && behavior != UnitBehavior_DEATH_HAND
    && behavior != UnitBehavior_SANDWORM )
  {
    if ( add_at_x >= gGameMap.width || (add_at_y_ = add_at_y, add_at_y >= gGameMap.height) )
    {
      add_at_y_ = add_at_y;
      DebugFatal("ModelAddUnit", "Unit added at %d %d, outside map area", add_at_x, add_at_y);
    }
    tile_flags = gGameMap.map[add_at_x + _CellNumbersWidthSpan[add_at_y_]].__tile_bitflags;
    if ( tile_flags & TileFlags_10_OCC_BUILDING )
    {
      if ( (tile_flags & (TileFlags_4_OWNER|TileFlags_2_OWNER|TileFlags_1_OWNER)) != side_id )
      {
        return -1;
      }
    }
  }
  side = GetSide(side_id);
  if ( !side->__ParticipatesInGame )
  {
    return -1;
  }
  unit = CSide__GetNextFreeObject(side);
  if ( !unit )
  {
    string_id = GetTextID("ReachedUnitLimit");
    string = GetTextString(string_id, 1);
    QueueMessage(string, -1);
    return -1;
  }
  // New logic start
  // Spawner - save unit owned stats
  if (SpawnerActive)
  {
    if (side_id < 8 && unit_type < 30)
    {
      int *ptr = (int *)&PlayersUnitsOwned[side_id];
      ptr[unit_type]++;
    }
  }
  // New logic end
  ++side->__UnitsBuilt;
  ++side->__UnitsBuiltPerType[unit_type];
  unit->Health = _templates_unitattribs[unit_type].__Strength;
  unit->Flags = _templates_unitattribs[unit_type].Flags;
  unit->Type = unit_type;
  unit->State = UNIT_STATE_2_IDLE;
  unit->OldState = UNIT_STATE_1;
  unit->__IsSelected = 0;
  unit->__UnitTurnDelayCounter = _templates_unitattribs[unit_type].__TurningSpeed;
  unit->__TurretTurnDelayCounter = _templates_unitattribs[unit_type].__BarrelTurnSpeed;
  unit->__ReloadDelayCounter = _templates_unitattribs[unit_type].PrimaryWeaponLongDelay;
  unit->__StealthUnCloakDelayCounter = 0;
  unit->__CurrentAnimFrame = 0;
  unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = 0;
  unit->__CurrentAnimType = 0;
  unit->__Lying = 0;
  if ( _templates_unitattribs[unit_type].__IsInfantry )
  {
    unit->__DeadStateTimeCounter = 50;
  }
  else
  {
    unit->__DeadStateTimeCounter = 10;
  }
  unit->__MakeSmokeTimeCounter = 0;
  unit->__FlickerFrames = 0;
  unit->__GroupID = -1;
  unit->EnemyIndex = -1;
  unit->EnemySide = side_id;
  unit->LastAttacked = -1;
  unit->LastFired = -1;
  unit->LastMoved = -1;
  unit->__SpecialPurpose = 0;
  unit->Speed = _templates_unitattribs[unit_type].__Speed;
  unit->__AttackerIndex = -1;
  pixel_offset_x_ = (char)pixel_offset_x;
  unit->RefineryIndex = -1;
  unit->RepairPadIndex = -1;
  unit->__AttackerSide = -1;
  unit->__RouteSteps = 0;
  unit->__RouteTotalSteps = 0;
  unit->RouteMode = 0;
  unit->__PosX = (32 * (char)add_at_x + pixel_offset_x_ + 16) << 16;
  unit->RB_c_field_7A = 0;
  pixel_offset_y_ = (char)pixel_offset_y;
  unit->__RouteRetryDelayCounter = 0;
  unit->__PosZHeight = 0;
  unit->__PosY = (32 * (char)add_at_y + pixel_offset_y_ + 16) << 16;
  if ( add_at_x != move_to_x || add_at_y != move_to_y )
  {
    facing = GetFacing_and_252(add_at_x, add_at_y, move_to_x, move_to_y);
    unit->__Facing = facing;
  }
  else
  {
    unit->__Facing = 0;
  }
  unit->BlockFromY = add_at_y;
  unit->BlockToY = add_at_y;
  unit->__RememberPosY = add_at_y;
  unit->TargetX = move_to_x;
  unit->BlockFromX = add_at_x;
  unit->BlockToX = add_at_x;
  unit->__RememberPosX = add_at_x;
  unit->__dw_field_88_ptr = &side->__LastUnitPtr;
  unit->__FacingTurret = 0;
  unit->__FacingTurretTarget = 0;
  unit->TargetY = move_to_y;
  unit->__posx2 = 0;
  unit->__posy2 = 0;
  unit->pos_steps = 0;
  unit->__CellspotTo = 0;
  unit->__CellspotFrom = 0;
  unit->ObjectType = 1;
  unit->__dw_field_84_ptr = (int)&side->__FirstUnitPtr;
  side->__LastAddedUnit = unit;
  CSide__SetupObject(side, unit, OBJECT_UNIT);
  v20 = &side->__UnitsExistPerType[unit->Type];
  v21 = *v20 + 1;
  *v20 = v21;
  behavior = _templates_unitattribs[unit_type].__Behavior;
  switch ( behavior )
  {
    case UnitBehavior_DEATH_HAND:
      unit->Flags |= UFLAGS_20000_DEATHHAND|UFLAGS_40_FLYING;
      unit->__PosZHeight = 0x3C0000;
      unit->__Facing = 0;
      unit->__SpecialPurpose = 8;
      UnitAdjustState(unit, UNIT_STATE_7_MOVING);
      unit->TargetX = gGameMap.width / 2;
      unit->TargetY = gGameMap.height / 2;
      break;
    case UnitBehavior_HARVESTER:
      ++side->__HarvesterCount;
      unit->RefineryIndex = GetRefineryIndex(side_id);
      break;
    case UnitBehavior_CARRYALL:
      ++side->__CarryallCount;
      v23 = unit->Flags | UFLAGS_40_FLYING;
      unit->__PosZHeight = 0x3C0000;
      unit->Flags = v23;
      pickup_queue_count = side->__PickupQueueCount + 1;
      v25 = side->__CarryallCount2 + 1;
      side->__PickupQueueCount = pickup_queue_count;
      side->__CarryallCount2 = v25;
      if ( (unsigned char)pickup_queue_count >= 32u )
      {
        side->__PickupQueueCount = 32;
      }
      v23 = move_to_x;
      v21 = move_to_y;
      pixel_offset_x = v23;
      pixel_offset_y = v21;
      facing_ = GetFlyingUnitSpawnPositionAndFacing((_WORD *)&pixel_offset_x, (_WORD *)&pixel_offset_y, 0);
      unit->__FacingTurret = facing_;
      unit->__Facing = facing_;
      x = pixel_offset_x;
      y = pixel_offset_y;
      unit->__PosX = (signed short)pixel_offset_x << 21;
      unit->BlockFromX = x;
      unit->BlockFromY = y;
      unit->BlockToX = x;
      unit->BlockToY = y;
      unit->__RememberPosX = x;
      unit->__RememberPosY = y;
      unit->__PosY = y << 21;
      unit->TargetX = add_at_x;
      unit->TargetY = add_at_y;
      UnitAdjustState(unit, UNIT_STATE_7_MOVING);
      break;
    case UnitBehavior_FRIGATE:
    case UnitBehavior_ORNITHOPTER:
    case UnitBehavior_BIRD:
      unit_flags = unit->Flags;
      unit->__PosZHeight = 0x3C0000;
      unit_flags = unit_flags | UFLAGS_40_FLYING;
      unit->Flags = unit_flags;
      break;
  }
  if ( (add_at_x == move_to_x && add_at_y == move_to_y)
    || (behavior = _templates_unitattribs[unit->Type].__Behavior, behavior == UnitBehavior_DEATH_HAND)
    || _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_CARRYALL )
  {
    if ( _templates_unitattribs[unit_type].__Behavior == UnitBehavior_HARVESTER )
    {
      UnitAdjustState(unit, UNIT_STATE_12_HARVESTING);
    }
    else if ( side_id != gSideId && !_IsMultiplayer )
    {
      behavior = _templates_unitattribs[unit->Type].__Behavior;
      if ( behavior != UnitBehavior_CARRYALL
        && _templates_unitattribs[unit->Type].__Behavior != UnitBehavior_FRIGATE
        && _templates_unitattribs[unit->Type].__Behavior != UnitBehavior_DEATH_HAND )
      {
        UnitAdjustState(unit, UNIT_STATE_3_GUARDING);
      }
    }
  }
  else
  {
    UnitAdjustState(unit, UNIT_STATE_8_LEAVING_BUILDING);
  }
  if ( _templates_unitattribs[unit_type].__Behavior == UnitBehavior_SANDWORM )
  {
    unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = _gVariables.SandWormInitialSleep;
    UnitAdjustState(unit, UNIT_STATE_29_SLEEPING);
    unit->Flags |= UFLAGS_200000_NOT_DRAWN;
  }
  if ( unit->State == UNIT_STATE_8_LEAVING_BUILDING && (unit->TargetX >= 128u || unit->TargetY >= 128u) )
  {
    DebugFatal("Model.cpp", "ModelAddUnit: targetX = %d, targetY = %d", unit->TargetX, unit->TargetY);
  }
  if ( unit->State == UNIT_STATE_17_DEAD )
  {
    DebugFatal("ModelAddUnit", "Error - exploding");
  }
  behavior = _templates_unitattribs[unit_type].__Behavior;
  if ( behavior != UnitBehavior_CARRYALL
    && behavior != UnitBehavior_FRIGATE
    && behavior != UnitBehavior_ORNITHOPTER
    && behavior != UnitBehavior_DEATH_HAND
    && behavior != UnitBehavior_SANDWORM )
  {
    if ( gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags & TileFlags_8_OCC_UNIT )
    {
      DebugFatal("ModelAddUnit", "Tile already occupied by unit");
    }
    if ( _templates_unitattribs[unit_type].__IsInfantry )
    {
      endloop = 0;
      check_flag = TileFlags_200_CSPOT_TL;
      y_ = unit->BlockFromY;
      x_ = unit->BlockFromX;
      pixel_offset_x = 4;
      cell_index = x_ + _CellNumbersWidthSpan[y_];
      tile_flags = gGameMap.map[cell_index].__tile_bitflags;
      do
      {
        if ( tile_flags & check_flag )
        {
          check_flag = (unsigned int)check_flag >> 1;
          pixel_offset_x = pixel_offset_x - 1;
        }
        else
        {
          endloop = 1;
        }
        if ( check_flag < TileFlags_20_CSPOT_MID )
        {
          endloop = 1;
          check_flag = 0;
        }
      }
      while ( !endloop );
      if ( check_flag )
      {
        gGameMap.map[cell_index].__tile_bitflags = tile_flags & ~(TileFlags_4_OWNER|TileFlags_2_OWNER|TileFlags_1_OWNER);
        gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags |= (unsigned char)side_id | (unsigned int)check_flag;
        gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags &= ~(TileFlags_8000000_OWNER2|TileFlags_4000000_OWNER2|TileFlags_2000000_OWNER2);
        cellspot = pixel_offset_x;
        gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags |= (unsigned char)side_id << 25;
        // New logic start
        // Implement uncrushable infantry
        if ( _templates_unitattribs[unit->Type].__CanCrush )
          gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].num_uncrushable_infantry++;
        // New logic end
        unit->__CellspotTo = cellspot;
        unit->__CellspotFrom = cellspot;
        spot_offset_index = 2 * (unsigned char)pixel_offset_x;
        unit->__PosX += (_InfantryCellspotPosOffsets2[spot_offset_index] - 16) << 16;
        unit->__PosY += (_InfantryCellspotPosOffsets2[spot_offset_index + 1] - 16) << 16;
      }
      else
      {
        DebugFatal("Model.cpp", "Couldn't find free slot for bloke");
      }
    }
    else
    {
      gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags &= ~(TileFlags_4_OWNER|TileFlags_2_OWNER|TileFlags_1_OWNER);
      v41 = unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY];
      gGameMap.map[v41].__tile_bitflags |= (unsigned char)side_id | (unsigned int)TileFlags_8_OCC_UNIT;
    }
  }
  if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_FREMEN )
  {
    unit_flags = unit->Flags;
    unit_flags = unit_flags | UFLAGS_10_STEALTH;
    unit->Flags = unit_flags;
  }
  if ( !strcmp(_templates_UnitNameList[unit->Type], "STEALTH RAIDER") )
  {
    unit_flags = unit->Flags;
    unit_flags = unit_flags | UFLAGS_10_STEALTH;
    unit->Flags = unit_flags;
  }
  return unit->MyIndex;
}
