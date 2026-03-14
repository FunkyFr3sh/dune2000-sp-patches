#include "macros/patch.h"
#include "dune2000.h"
#include "utils.h"

// Custom implementation of function MoveUnit
DETOUR(0x00494260, 0x00494CC8, _Mod__MoveUnit);

char Mod__MoveUnit(Unit *unit, eSideType side_id, short index)
{
  Unit *unit_; // esi
  int target_x; // ecx
  unsigned char block_from_x; // al
  unsigned char block_to_x; // cl MAPDST
  unsigned long long v10; // rax
  int v11; // eax
  unsigned char cellspot; // bl MAPDST
  TileFlags check_flag; // ebp
  unsigned int target_tile_flags; // edi
  int spot_offset_index; // eax
  unsigned char block_to_y; // dl MAPDST
  int y_offset; // eax
  int unit_type_offset; // eax MAPDST
  int speed; // rax
  int actual_health; // ecx
  int max_health; // eax
  int posx2; // ebx
  int diff_x; // ecx
  unsigned int diff_y; // eax
  unsigned int pos_steps; // ebx
  int cell_index; // ecx MAPDST
  unsigned char v29; // al MAPDST
  char v30; // bl
  unsigned long long v31; // rax
  int v32; // eax
  int posx; // edi
  int speed_; // ebx
  unsigned int half_speed; // ecx
  int xpos; // eax
  int posy; // edi
  int ypos; // eax
  unsigned char pos_stepsmax; // cl
  unsigned char new_pos_steps; // al
  int block_from_y_; // edx
  int block_to_x_; // ecx
  TileFlags tile_flags; // eax MAPDST
  char *unit_name; // eax
  char unusable_square_in_route; // bl
  int route_step_index; // edi
  UnitFlags unit_flags; // eax MAPDST
  unsigned char block_from_y; // dl
  bool is_sandworm; // [esp+13h] [ebp-11h]
  char is_infantry; // [esp+28h] [ebp+4h]
  eSideType sidea; // [esp+2Ch] [ebp+8h]
  char cellspot_found; // [esp+30h] [ebp+Ch]
  dwXYStruct xy;

  unit_ = unit;
  if ( !unit->pos_steps && unit->BlockFromX == unit->TargetX && unit->BlockFromY == unit->TargetY )
  {
    return 0;
  }
  if ( abs(unit->__PosX / 0x10000 / 32 - unit->BlockFromX) > 1 && abs(unit->__PosX / 0x10000 / 32 - unit->BlockToX) > 1 )
  {
    DebugFatal("MoveUnit", "Invalid pos.x");
  }
  if ( abs(unit->__PosY / 0x10000 / 32 - unit->BlockFromY) > 1 && abs(unit->__PosY / 0x10000 / 32 - unit->BlockToY) > 1 )
  {
    DebugFatal("MoveUnit", "Invalid pos.y");
  }
  if ( (unsigned char)side_id >= 8u )
  {
    DebugFatal("MoveUnit", "Invalid Side");
  }
  target_x = unit->TargetX;
  if ( target_x > gGameMap.width || unit->TargetY > gGameMap.height )
  {
    DebugFatal(
      "MoveUnit",
      "targetX = %d, targetY = %d (unit %s)",
      target_x,
      unit->TargetY,
      _templates_UnitNameList[unit->Type]);
  }
  if ( unit->Type >= gUnitTypeNum )
  {
    DebugFatal("MoveUnit", "Invalid unit type %d", unit->Type);
  }
  unit_type_offset = unit->Type << 8;
  is_infantry = *(&_templates_unitattribs[0].__IsInfantry + unit_type_offset);
  is_sandworm = *(&_templates_unitattribs[0].__Behavior + unit_type_offset) == UnitBehavior_SANDWORM;
  if ( unit_->pos_steps )
  {
    goto LABEL_70;
  }
  if ( unit_->__RouteRetryDelayCounter )
  {
    if ( ((index & 7) == (gGameTicks & 7) || unit_->EnemyIndex != -1)
      && !UnitIsCloaked(unit_)
      && unit_->State != UNIT_STATE_10_ATTACKIG_TILE_ON_ROUTE
      && !is_sandworm )
    {
      UnitAttack(unit_, side_id, index, (eSideType *)&unit_->EnemySide, (unsigned short *)&unit_->EnemyIndex);
    }
  }
  block_from_x = unit_->BlockFromX;
  block_to_x = unit_->BlockToX;
  if ( block_from_x == block_to_x && unit_->BlockFromY == unit_->BlockToY )
  {
    GetNextSquare(unit_, side_id, index);
    if ( PickupCrate(unit_, side_id) )
    {
      unit_->__RouteSteps = 0;
      GetNextSquare(unit_, side_id, index);
    }
    else if ( unit_->State == UNIT_STATE_17_DEAD )
    {
      return 1;
    }
    block_from_x = unit_->BlockFromX;
    block_to_x = unit_->BlockToX;
    if ( block_from_x == block_to_x && unit_->BlockFromY == unit_->BlockToY )
    {
      return 1;
    }
    unit_->Flags &= ~UFLAGS_BLOCKTOMARKED;
  }
  v10 = unit_->__Facing
      - (unsigned char)GetFacing_and_252(block_from_x, unit_->BlockFromY, block_to_x, unit_->BlockToY)
      + 32;
  v11 = ((v10 >> 32) ^ (((((v10 >> 32) & 0xFF) ^ (unsigned char)v10) - ((v10 >> 32) & 0xFF)) & 0x1F)) - (v10 >> 32);
  if ( !(_BYTE)v11 )
  {
    unit_->Flags &= ~UFLAGS_20;
  }
  if ( unit_->Flags & UFLAGS_20 || ((unsigned char)v11 >= 7u && (unsigned char)v11 <= 24u) )
  {
LABEL_70:
    if ( unit_->BlockFromX != unit_->BlockToX || unit_->BlockFromY != unit_->BlockToY )
    {
      v29 = GetFacing_and_252(unit_->BlockFromX, unit_->BlockFromY, unit_->BlockToX, unit_->BlockToY);
    }
    else
    {
      v29 = unit_->__Facing;
    }
    v30 = 0;
    v31 = unit_->__Facing - v29 + 32;
    v32 = ((v31 >> 32) ^ (((((v31 >> 32) & 0xFF) ^ (unsigned char)v31) - ((v31 >> 32) & 0xFF)) & 0x1F)) - (v31 >> 32);
    if ( !(_BYTE)v32 )
    {
      unit_->Flags &= ~UFLAGS_20;
    }
    unit_flags = unit_->Flags;
    if ( unit_flags & UFLAGS_20 || ((unsigned char)v32 >= 7u && (unsigned char)v32 <= 0x18u) )
    {
      if ( (unit_flags & UFLAGS_80000000) != 0 )
      {
        unit_->Flags = (unit_flags & ~(UFLAGS_80000000|UFLAGS_40000000)) | UFLAGS_40000000;
      }
      unit_flags = unit_->Flags;
      unit_flags = unit_flags | UFLAGS_20;
      unit_->Flags = unit_flags;
    }
    else
    {
      v30 = 1;
    }
    TurnUnitInDirection(unit_, v29);
    if ( _templates_unitattribs[unit_->Type].__BarrelArt != -1 )
    {
      if ( unit_->BlockFromX != unit_->TargetX || unit_->BlockFromY != unit_->TargetY )
      {
        sidea = GetFacing_and_252(unit_->BlockFromX, unit_->BlockFromY, unit_->TargetX, unit_->TargetY);
        TurnUnitBarrelInDirection(unit_, sidea);
      }
    }
    if ( v30 )
    {
      unit_flags = unit_->Flags & ~(UFLAGS_80000000|UFLAGS_40000000);
      unit_->Flags = unit_flags;
      if ( !(unit_flags & UFLAGS_BLOCKTOMARKED) )
      {
        if ( unit_->__pos_stepsmax )
        {
          DebugFatal("MoveUnit", "Move unit but UFLAGS_BLOCKTOMARKED not set");
        }
      }
      if ( _templates_unitattribs[unit_->Type].__IsInfantry )
      {
        SetUnitAnimationType(unit_, 1);
      }
      posx = unit_->__PosX;
      speed_ = unit_->Speed;
      half_speed = (unsigned int)unit_->Speed >> 1;
      xpos = unit_->__PosX / 0x10000;
      if ( xpos >= (signed int)((unit_->__posx2 - half_speed) >> 16) )
      {
        if ( xpos > (signed int)((half_speed + unit_->__posx2) >> 16) )
        {
          unit_->__PosX = posx - speed_;
        }
      }
      else
      {
        unit_->__PosX = speed_ + posx;
      }
      posy = unit_->__PosY;
      ypos = unit_->__PosY / 0x10000;
      if ( ypos >= (signed int)((unit_->__posy2 - half_speed) >> 16) )
      {
        if ( ypos > (signed int)((unit_->__posy2 + half_speed) >> 16) )
        {
          unit_->__PosY = posy - speed_;
        }
      }
      else
      {
        unit_->__PosY = posy + speed_;
      }
      pos_stepsmax = unit_->__pos_stepsmax;
      new_pos_steps = unit_->pos_steps - 1;
      unit_->pos_steps = new_pos_steps;
      if ( new_pos_steps < pos_stepsmax )
      {
        unit_->__pos_stepsmax = 0;
        if ( is_infantry )
        {
          gGameMap.map[unit_->BlockFromX + _CellNumbersWidthSpan[unit_->BlockFromY]].__tile_bitflags &= ~(32 << unit_->__CellspotFrom);
          unit_->__CellspotFrom = unit_->__CellspotTo;
          // New logic start
          // Implement uncrushable infantry
          if ( _templates_unitattribs[unit->Type].__CanCrush )
            gGameMap.map[unit_->BlockFromX + _CellNumbersWidthSpan[unit_->BlockFromY]].num_uncrushable_infantry--;
          // New logic end
        }
        else if ( is_sandworm )
        {
          cell_index = unit_->BlockFromX + _CellNumbersWidthSpan[unit_->BlockFromY];
          gGameMap.map[cell_index].__tile_bitflags &= ~UFLAGS_1000000_DRAWN_BEHIND_OBJECTS;
        }
        else
        {
          gGameMap.map[unit_->BlockFromX + _CellNumbersWidthSpan[unit_->BlockFromY]].__tile_bitflags &= ~TileFlags_8_OCC_UNIT;
          block_from_y_ = unit_->BlockFromY;
          tile_flags = gGameMap.map[unit_->BlockFromX + _CellNumbersWidthSpan[block_from_y_]].__tile_bitflags;
          if ( tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID) )
          {
            tile_flags = tile_flags & ~7;
            gGameMap.map[unit_->BlockFromX + _CellNumbersWidthSpan[block_from_y_]].__tile_bitflags = tile_flags;
            gGameMap.map[unit_->BlockFromX + _CellNumbersWidthSpan[unit_->BlockFromY]].__tile_bitflags |= (gGameMap.map[unit_->BlockFromX + _CellNumbersWidthSpan[unit_->BlockFromY]].__tile_bitflags >> 25) & 7;
          }
        }
        block_to_y = unit_->BlockToY;
        unit_->BlockFromX = unit_->BlockToX;
        unit_flags = unit_->Flags & ~UFLAGS_BLOCKTOMARKED;
        unit_->BlockFromY = block_to_y;
        unit_->Flags = unit_flags;
      }
    }
    return 1;
  }
  xy.X = unit_->BlockToX;
  xy.Y = unit_->BlockToY;
  if ( CanUnitUseSquare(xy, unit_, side_id, 0) )
  {
    unit_flags = unit_->Flags;
    if ( !(unit_flags & UFLAGS_40000000) )
    {
      unit_->Flags = unit_flags | UFLAGS_BLOCKTOMARKED;
      if ( is_infantry )
      {
        cellspot = 4;
        check_flag = TileFlags_200_CSPOT_TL;
        cellspot_found = 0;
        target_tile_flags = gGameMap.map[unit_->BlockToX + _CellNumbersWidthSpan[unit_->BlockToY]].__tile_bitflags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_10_OCC_BUILDING|TileFlags_8_OCC_UNIT);
        if ( (32 << unit_->__CellspotFrom) & target_tile_flags )
        {
          do
          {
            if ( target_tile_flags & check_flag )
            {
              check_flag = (unsigned int)check_flag >> 1;
              --cellspot;
            }
            else
            {
              cellspot_found = 1;
            }
            if ( check_flag < TileFlags_20_CSPOT_MID )
            {
              DebugFatal("unit.cpp", "Can't find a free slot for a soldier");
            }
          }
          while ( !cellspot_found );
        }
        else
        {
          cellspot = unit_->__CellspotFrom;
        }
        block_to_x = unit_->BlockToX;
        spot_offset_index = 2 * cellspot;
        block_to_y = unit_->BlockToY;
        unit_->__posx2 = (32 * block_to_x + _InfantryCellspotPosOffsets[spot_offset_index]) << 16;
        y_offset = _InfantryCellspotPosOffsets[spot_offset_index + 1];
        unit_->__CellspotTo = cellspot;
        unit_->__posy2 = (32 * block_to_y + y_offset) << 16;
      }
      else
      {
        block_to_x = unit_->BlockToX;
        unit_->__posx2 = (unit_->BlockToX << 21) + 0x100000;
        block_to_y = unit_->BlockToY;
        unit_->__posy2 = (unit_->BlockToY << 21) + 0x100000;
      }
      if ( unit_->BlockFromX == block_to_x && unit_->BlockFromY == block_to_y )
      {
        DebugFatal("MoveUnit", "blockFrom == blockTo");
      }
      unit_type_offset = unit_->Type << 8;
      if ( *(&_templates_unitattribs[0].__IsInfantry + unit_type_offset) )
      {
        if ( unit_->__CurrentAnimType == 5 )
        {
          speed = (unsigned int)GetSpeed(unit_) >> 1;
LABEL_64:
          posx2 = unit_->__posx2;
          unit_->Speed = speed;
          diff_x = abs(unit_->__PosX - posx2);
          diff_y = abs(unit_->__PosY - unit_->__posy2);
          if ( diff_x > (signed int)diff_y )
          {
            diff_y = diff_x;
          }
          pos_steps = diff_y / unit_->Speed;
          if ( pos_steps > 255 )
          {
            // New logic start
            // Add more debug information to investigate "unit->pos.steps > 255" error
            DebugFatal("MoveUnitExtraDebug",
                       "type: %s health: %d / %d\npos_steps: %d speed: %x (%d)\nfrom: %d , %d (%d , %d)\nto: %d , %d (%d , %d)\ndist: %d",
                       _templates_UnitNameList[unit->Type], unit->Health, _templates_unitattribs[unit->Type].__Strength,
                       pos_steps,
                       speed, speed >> 12,
                       unit->__PosX >> 16, unit->__PosY >> 16, unit->__PosX >> 21, unit->__PosY >> 21,
                       unit->__posx2 >> 16, unit->__posy2 >> 16, unit->__posx2 >> 21, unit->__posy2 >> 21,
                       (int)sqrt(((unit->__PosX >> 16) - (unit->__posx2 >> 16)) * ((unit->__PosX >> 16) - (unit->__posx2 >> 16)) + ((unit->__PosY >> 16) - (unit->__posy2 >> 16)) * ((unit->__PosY >> 16) - (unit->__posy2 >> 16)))
                      );
            // New logic end
            DebugFatal("MoveUnit", "unit->pos.steps > 255");
          }
          unit_->pos_steps = pos_steps;
          unit_->__pos_stepsmax = (unsigned char)pos_steps >> 1;
          if ( is_infantry )
          {
            gGameMap.map[unit_->BlockToX + _CellNumbersWidthSpan[unit_->BlockToY]].__tile_bitflags &= ~7u;
            gGameMap.map[unit_->BlockToX + _CellNumbersWidthSpan[unit_->BlockToY]].__tile_bitflags |= (unsigned char)side_id | (32 << cellspot);
            gGameMap.map[unit_->BlockToX + _CellNumbersWidthSpan[unit_->BlockToY]].__tile_bitflags &= ~(TileFlags_8000000_OWNER2|TileFlags_4000000_OWNER2|TileFlags_2000000_OWNER2);
            cell_index = unit_->BlockToX + _CellNumbersWidthSpan[unit_->BlockToY];
            gGameMap.map[cell_index].__tile_bitflags |= (unsigned char)side_id << 25;
            // New logic start
            // Implement uncrushable infantry
            if ( _templates_unitattribs[unit->Type].__CanCrush )
              gGameMap.map[cell_index].num_uncrushable_infantry++;
            // New logic end
          }
          else if ( is_sandworm )
          {
            cell_index = unit_->BlockToX + _CellNumbersWidthSpan[unit_->BlockToY];
            gGameMap.map[cell_index].__tile_bitflags |= TileFlags_1000000_OCC_SANDWORM;
          }
          else
          {
            block_to_x_ = unit_->BlockToX;
            tile_flags = gGameMap.map[block_to_x_ + _CellNumbersWidthSpan[unit_->BlockToY]].__tile_bitflags;
            if ( tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID) )
            {
              if ( (tile_flags & 7) == side_id )
              {
                DebugFatal("MoveUnit", "Invalid blockTo, since already occupied by infantry of same side");
              }
            }
            else if ( tile_flags & TileFlags_8_OCC_UNIT )
            {
              tile_flags = gGameMap.map[block_to_x_ + _CellNumbersWidthSpan[unit_->BlockToY]].__tile_bitflags;
              unit_name = GetTextString(
                            _UnitGroupTextIds[(unsigned char)_templates_unitattribs[unit_->Type].__UnitType],
                            0);
              DebugFatal("MoveUnit", "Invalid blockTo by %s, since tile already marked (%X) ", unit_name, tile_flags);
            }
            gGameMap.map[unit_->BlockToX + _CellNumbersWidthSpan[unit_->BlockToY]].__tile_bitflags &= ~(TileFlags_4_OWNER|TileFlags_2_OWNER|TileFlags_1_OWNER);
            cell_index = unit_->BlockToX + _CellNumbersWidthSpan[unit_->BlockToY];
            gGameMap.map[cell_index].__tile_bitflags |= (unsigned char)side_id | (unsigned int)TileFlags_8_OCC_UNIT;
          }
          goto LABEL_70;
        }
      }
      else
      {
        actual_health = unit_->Health;
        max_health = _templates_unitattribs[unit_->Type].__Strength;
        if ( 3 * actual_health < max_health )
        {
          // New logic start
          // Fix units with less than one third health moving faster than units with less than half health
          speed = ((double)(unsigned int)GetSpeed(unit_) * 0.66000003);
          // New logic end
          goto LABEL_64;
        }
        if ( 2 * actual_health < max_health )
        {
          // New logic start
          // Fix units with less than one third health moving faster than units with less than half health
          speed = ((double)(unsigned int)GetSpeed(unit_) * 0.85000002);
          // New logic end
          goto LABEL_64;
        }
      }
      speed = GetSpeed(unit_);
      goto LABEL_64;
    }
  }
  unit_flags = unit_->Flags;
  if ( !(unit_flags & UFLAGS_40000000) )
  {
    unit_->pos_steps = 0;
    unit_->RouteMode = 1;
    goto LABEL_128;
  }
  unusable_square_in_route = 0;
  unit_->Flags = unit_flags & ~UFLAGS_40000000;
  route_step_index = 0;
  if ( unit_->__RouteSteps )
  {
    while ( !unusable_square_in_route )
    {
      xy.X = unit_->__RouteX[route_step_index];
      xy.Y = unit_->__RouteY[route_step_index];
      if ( !CanUnitUseSquare(
              xy,
              unit_,
              side_id,
              0) )
      {
        unusable_square_in_route = 1;
      }
      if ( ++route_step_index >= unit_->__RouteSteps )
      {
        goto LABEL_124;
      }
    }
  }
  else
  {
LABEL_124:
    if ( !unusable_square_in_route )
    {
      unit_->RouteMode = 0;
      unit_->pos_steps = 0;
      goto LABEL_128;
    }
  }
  unit_->RouteMode = 1;
  unit_->pos_steps = 0;
LABEL_128:
  unit_flags = unit_->Flags;
  unit_->__RouteSteps = 0;
  if ( unit_flags & UFLAGS_BLOCKTOMARKED )
  {
    DebugFatal("MoveUnit", "Error with routefind logic");
  }
  block_from_y = unit_->BlockFromY;
  unit_->BlockToX = unit_->BlockFromX;
  unit_->BlockToY = block_from_y;
  return 1;
}
