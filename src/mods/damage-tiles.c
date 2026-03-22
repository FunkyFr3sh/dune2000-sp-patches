#include "macros/patch.h"
#include "dune2000.h"
#include "utils.h"
#include "crates-func.h"
#include "extended-tileset.h"
#include "extended-templates.h"

// Custom implementation of function DamageTiles
DETOUR(0x0049D2E0, 0x0049DD73, _Mod__DamageTiles);

void Mod__DamageTiles(unsigned int xpos, unsigned int ypos, unsigned int zpos, unsigned char bulletType, char firer_side_id, short firer_index, char deviator)
{
  bool damage_on_ground; // zf
  int bullet_type; // ecx
  int tile_y; // ecx
  signed int object_side_id; // ebp
  char firer_side_id_; // bl
  CSide *object_side; // eax MAPDST
  Building *bld; // esi
  unsigned int ypos_diff; // edi MAPDST
  long double distance; // st7 MAPDST
  long double bullet_radius_; // st6 MAPDST
  unsigned int actual_damage; // edi
  int building_flags; // eax
  int sound_id_hark; // ST08_4 MAPDST
  int sound_id_ord; // ST04_4 MAPDST
  int sound_id_atr; // eax MAPDST
  CSide *firer_side; // eax MAPDST
  signed int new_health; // ecx
  int building_type; // eax
  int building_type_; // eax
  int building_template_flags; // ecx
  Unit *unit; // esi
  UnitFlags unit_flags; // ebx MAPDST
  bool unit_can_be_damaged; // al
  int unit_type; // ebp
  int unit_state; // ecx
  unsigned int xpos_diff; // ecx
  int bullet_type__; // edi
  signed long long actual_damage_; // rax
  signed int unit_health; // ecx
  int *units_killed_per_type_and_side_ptr; // eax
  CSide *old_side; // eax
  int *unit_lost_per_type_ptr; // eax
  signed int object_side_id__; // edi MAPDST
  int unit_flags_; // eax MAPDST
  Unit *new_unit; // eax
  int cell_index; // esi
  int tile_flags; // ebx
  unsigned int damage_dealt; // edx
  unsigned char new_damage; // cl
  bool unit_is_sandworm; // [esp+13h] [ebp-49h]
  signed int object_side_id_; // [esp+14h] [ebp-48h]
  int bullet_radius; // [esp+18h] [ebp-44h]
  char crate_index; // [esp+1Ch] [ebp-40h]
  CSide *object_side_; // [esp+20h] [ebp-3Ch]
  int bullet_type_; // [esp+24h] [ebp-38h]
  int tile_y_; // [esp+28h] [ebp-34h]
  int tile_x_; // [esp+2Ch] [ebp-30h]
  int closest_tile_x; // [esp+30h] [ebp-2Ch]
  int closest_tile_y; // [esp+34h] [ebp-28h]
  long long raw_damage; // [esp+44h] [ebp-18h] MAPDST
  bool damage_up_in_air; // [esp+68h] [ebp+Ch]

  damage_on_ground = zpos <= 0x60000;
  damage_up_in_air = zpos > 0x60000;
  if ( damage_on_ground )
  {
    // New logic start
    // Overwrite the code which takes care of blowing up spice blooms when shot
    CheckCrateAt(xpos >> 5, ypos >> 5, &crate_index);
    if (crate_index != -1)
      HitCrate(crate_index);
    // New logic end
  }
  if ( bulletType >= gBulletTypeNum )
  {
    DebugFatal("DamageTiles", "Invalid bulletType %d", bulletType);
  }
  bullet_type = bulletType;
  bullet_type_ = bullet_type * 28;
  bullet_radius = _WarheadData[(unsigned char)_templates_bulletattribs[bullet_type].Warhead].Radius;
  tile_y = ypos >> 5;
  tile_x_ = xpos >> 5;
  tile_y_ = ypos >> 5;
  if ( ((xpos >> 5) & 0x80000000) != 0
    || (signed int)(xpos >> 5) >= gGameMap.width
    || tile_y < 0
    || tile_y >= gGameMap.height )
  {
    DebugFatal("Unit.cpp", "DamageTiles - out of range (%d, %d)", xpos >> 5, ypos >> 5);
  }
  object_side_id = 0;
  object_side_id_ = 0;
  if ( _SidesToProcess )
  {
    firer_side_id_ = firer_side_id;
    do
    {
      object_side = GetSide(object_side_id);
      object_side_ = object_side;
      if ( !damage_up_in_air )
      {
        bld = object_side->__FirstBuildingPtr;
        if ( bld )
        {
          do
          {
            if ( bld->__State != BLD_STATE_17_DEAD && !(bld->Flags & (BFLAGS_4_SELLING|BFLAGS_2_BUILDUP)) )
            {
              ClosestBuildingTile(bld, tile_x_, tile_y_, &closest_tile_x, &closest_tile_y);
              ypos_diff = ypos - 32 * closest_tile_y - 16;
              if ( abs(xpos - 32 * closest_tile_x - 16) < bullet_radius && abs(ypos_diff) < bullet_radius )
              {
                distance = sqrt(
                             (double)(signed int)((xpos - 32 * closest_tile_x - 16) * (xpos - 32 * closest_tile_x - 16))
                           + (double)(signed int)(ypos_diff * ypos_diff));
                bullet_radius_ = (double)(unsigned int)bullet_radius;
                if ( distance < bullet_radius_ )
                {
                  raw_damage = *(int *)((char *)&_templates_bulletattribs[0].__Damage + bullet_type_)
                             * (unsigned int)(unsigned char)_WarheadData[*((unsigned char *)&_templates_bulletattribs[0].Warhead
                                                                           + bullet_type_)].Verses[(unsigned char)_templates_buildattribs[bld->Type].Armour]
                             / 100;
                  actual_damage = (signed long long)((double)raw_damage * ((bullet_radius_ - distance) / bullet_radius_));
                  if ( bld->__MySide != firer_side_id_ )
                  {
                    building_flags = bld->Flags;
                    if ( !(building_flags & (BFLAGS_4_SELLING|BFLAGS_2_BUILDUP)) )
                    {
                      building_flags |= BFLAGS_1000_WAS_ATTACKED;
                      bld->Flags = building_flags;
                      if ( object_side_id == (unsigned char)gSideId )
                      {
                        sound_id_hark = GetSoundTableID("H_UNDERATTACK");
                        sound_id_ord = GetSoundTableID("O_UNDERATTACK");
                        sound_id_atr = GetSoundTableID("S_UNDERATTACK");
                        PlayMentatSound(sound_id_atr, sound_id_ord, sound_id_hark, 1, 300, 0);
                      }
                    }
                  }
                  if ( actual_damage < (unsigned int)bld->Health || bld->__DeadStateTimeCounter )
                  {
                    if ( firer_side_id_ != -1
                      && bulletType != _templates_GroupIDs.SpiceExpl
                      && _gDiplomacy[object_side_id][(int)firer_side_id_] == 2 )
                    {
                      NeutralBecomeHostile((eSideType)object_side_id, (eSideType)firer_side_id_);
                    }
                    if ( bld->__AttackerIndex == -1
                      && bulletType != _templates_GroupIDs.SpiceExpl
                      && firer_side_id_ != object_side_id )
                    {
                      bld->__AttackerSide = firer_side_id_;
                      bld->__AttackerIndex = firer_index;
                    }
                    new_health = (bld->Health - actual_damage) & (((signed int)(bld->Health - actual_damage) <= 0) - 1);
                    building_type = bld->Type;
                    bld->Health = new_health;
                    building_type_ = building_type;
                    if ( new_health < (signed int)((unsigned int)_templates_buildattribs[building_type_]._____HitPoints >> 1) )
                    {
                      building_template_flags = _templates_buildattribs[building_type_]._____Flags;
                      if ( building_template_flags & BFLAGS_200_SELECTABLE_REPAIRABLE )
                      {
                        bld->__MakeSmokeTimeCounter = (GetRandomValue(
                                                         "C:\\MsDev\\Projects\\July2000\\code\\unit.cpp",
                                                         7450) & 0x7F)
                                                    + 64;
                        bld->__SmokeAnimActiveBitfield |= GetRandomValue(
                                                            "C:\\MsDev\\Projects\\July2000\\code\\unit.cpp",
                                                            7451);
                      }
                    }
                  }
                  else
                  {
                    if ( object_side_id == (unsigned char)gSideId
                      && _templates_buildattribs[bld->Type].__Behavior != BuildingBehavior_WALL )
                    {
                      sound_id_hark = GetSoundTableID("H_BUILDINGLOST");
                      sound_id_ord = GetSoundTableID("O_BUILDINGLOST");
                      sound_id_atr = GetSoundTableID("S_BUILDINGLOST");
                      PlayMentatSound(sound_id_atr, sound_id_ord, sound_id_hark, 1, 0, 0);
                    }
                    if ( bulletType != _templates_GroupIDs.SpiceExpl )
                    {
                      if ( firer_side_id_ != -1 && _gDiplomacy[object_side_id][(int)firer_side_id_] == 2 )
                      {
                        NeutralBecomeHostile((eSideType)object_side_id, (eSideType)firer_side_id_);
                      }
                      bld->__AttackerIndex = firer_index;
                      bld->__AttackerSide = firer_side_id_;
                      firer_side = GetSide(firer_side_id_);
                      ++firer_side->__BuildingsKilledPerTypeAndSide[0].__kills_per_side[object_side_id + 8 * bld->Type];
                      firer_side = GetSide(firer_side_id_);
                      ++firer_side->__BuildingsKilled;
                    }
                    ++object_side_->__BuildingsLost;
                    DestroyBuilding((eSideType)object_side_id, bld->MyIndex, 0);
                  }
                }
              }
            }
            bld = bld->Next;
          }
          while ( bld );
          object_side = object_side_;
        }
      }
      unit = object_side->__FirstUnitPtr;
      if ( unit )
      {
        do
        {
          unit_flags = unit->Flags;
          unit_can_be_damaged = 1;
          unit_type = unit->Type << 8;
          unit_is_sandworm = *(&_templates_unitattribs[0].__Behavior + unit_type) == UnitBehavior_SANDWORM;
          if ( unit_flags & UFLAGS_40_FLYING )
          {
            unit_can_be_damaged = damage_up_in_air != 0;
          }
          else if ( damage_up_in_air )
          {
            unit_can_be_damaged = 0;
          }
          if ( unit_flags & UFLAGS_8000000_INVULNERABLE || unit_flags & UFLAGS_100_CARRYING )
          {
            unit_can_be_damaged = 0;
          }
          unit_state = unit->State;
          // New logic start
          // Fix infantry rarely remaining invulnerable after released from destroyed building
          // Work-around: add additional check if infantry stays on tile occupied by building.
          // If there's not building tile, allow infantry to be damaged even if it's still in state 8 due to a bug
          if ( unit_state == UNIT_STATE_8_LEAVING_BUILDING && UnitTileOccupiedByBuilding(unit) )
          // New logic end
          {
            unit_can_be_damaged = 0;
          }
          if ( unit_state == UNIT_STATE_17_DEAD )
          {
            unit_can_be_damaged = 0;
          }
          if ( unit_state == UNIT_STATE_23_INFILTRATING )
          {
            unit_can_be_damaged = 0;
          }
          // New logic start
          // Make sleeping sandworm not taking damage - prevents turning neutral to hostile
          if ( unit_state == UNIT_STATE_29_SLEEPING )
          {
            unit_can_be_damaged = 0;
          }
          // New logic end
          if ( unit_can_be_damaged )
          {
            xpos_diff = xpos - unit->__PosX / 0x10000;
            ypos_diff = ypos - unit->__PosY / 0x10000;
            if ( abs(xpos - unit->__PosX / 0x10000) < bullet_radius && abs(ypos_diff) < bullet_radius )
            {
              distance = sqrt((double)(signed int)(xpos_diff * xpos_diff) + (double)(signed int)(ypos_diff * ypos_diff));
              bullet_radius_ = (double)(unsigned int)bullet_radius;
              if ( distance < bullet_radius_ )
              {
                bullet_type__ = bullet_type_;
                raw_damage = *(int *)((char *)&_templates_bulletattribs[0].__Damage + bullet_type_)
                           * (unsigned int)(unsigned char)_WarheadData[*((unsigned char *)&_templates_bulletattribs[0].Warhead
                                                                         + bullet_type_)].Verses[*((unsigned char *)&_templates_unitattribs[0].__Armour
                                                                                                 + unit_type)]
                           / 0x64;
                actual_damage_ = (signed long long)((double)raw_damage * ((bullet_radius_ - distance) / bullet_radius_));
                unit_health = unit->Health;
                unit->Flags = unit_flags | 1;
                if ( (signed int)actual_damage_ < unit_health )
                {
                  unit->Health = unit_health - actual_damage_;
                  if ( firer_side_id == -1 || bulletType == _templates_GroupIDs.SpiceExpl )
                  {
                    object_side_id__ = object_side_id_;
                  }
                  else
                  {
                    object_side_id__ = object_side_id_;
                    if ( _gDiplomacy[object_side_id_][(unsigned char)firer_side_id] == 2 )
                    {
                      NeutralBecomeHostile((eSideType)object_side_id_, firer_side_id);
                    }
                  }
                  unit_flags = unit->Flags;
                  if ( unit_flags & UFLAGS_2000000 )
                  {
                    unit->Flags = unit_flags & ~UFLAGS_2000000;
                  }
                  else if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_HARVESTER
                         && object_side_id__ == (unsigned char)gSideId
                         && firer_index != -1 )
                  {
                    sound_id_hark = GetSoundTableID("H_HARVATTACK");
                    sound_id_ord = GetSoundTableID("O_HARVATTACK");
                    sound_id_atr = GetSoundTableID("S_HARVATTACK");
                    PlayMentatSound(sound_id_atr, sound_id_ord, sound_id_hark, 1, 300, 0);
                  }
                  if ( !unit_is_sandworm
                    && unit->Health < (signed int)((unsigned int)_templates_unitattribs[unit->Type].__Strength >> 1) )
                  {
                    unit->__MakeSmokeTimeCounter = (GetRandomValue(
                                                      "C:\\MsDev\\Projects\\July2000\\code\\unit.cpp",
                                                      7610) & 0x7F)
                                                 + 64;
                  }
                  if ( unit->__AttackerIndex == -1
                    && bulletType != _templates_GroupIDs.SpiceExpl
                    && (unsigned char)firer_side_id != object_side_id__ )
                  {
                    unit->__AttackerSide = firer_side_id;
                    unit->__AttackerIndex = firer_index;
                  }
                  if ( deviator )
                  {
                    if ( !unit_is_sandworm )
                    {
                      unit->Flags &= ~UFLAGS_1_WAS_ATTACKED;
                      if ( (unsigned char)firer_side_id == object_side_id_
                        || _templates_unitattribs[unit->Type].__IsInfantry )
                      {
                        unit->LastAttacked = 1;
                      }
                      else if ( UnitTileOccupiedByBuilding(unit) )
                      {
                        unit->LastAttacked = 1;
                      }
                      else
                      {
                        unit_flags_ = unit->Flags;
                        if ( !(unit_flags_ & UFLAGS_1000_DEVIATED) )
                        {
                          unit->OldSide = object_side_id_;
                        }
                        new_unit = ChangeUnitOwner((eSideType)object_side_id_, firer_side_id, unit->MyIndex, 0);
                        if ( new_unit )
                        {
                          unit_flags_ = new_unit->Flags;
                          new_unit->__DeviatedTimeCounter = 240;
                          unit_flags_ |= UFLAGS_1000_DEVIATED;
                          new_unit->Flags = unit_flags_;
                        }
                      }
                    }
                  }
                }
                else
                {
                  if ( bulletType == _templates_GroupIDs.SpiceExpl )
                  {
                    object_side_id__ = object_side_id_;
                  }
                  else
                  {
                    if ( firer_side_id == -1 )
                    {
                      object_side_id__ = object_side_id_;
                    }
                    else
                    {
                      object_side_id__ = object_side_id_;
                      if ( _gDiplomacy[object_side_id_][(unsigned char)firer_side_id] == 2 )
                      {
                        NeutralBecomeHostile((eSideType)object_side_id_, firer_side_id);
                      }
                    }
                    unit->__AttackerIndex = firer_index;
                    unit->__AttackerSide = firer_side_id;
                    units_killed_per_type_and_side_ptr = (int *)((char *)GetSide(firer_side_id)->__UnitsKilledPerTypeAndSide
                                                               + 4 * (object_side_id__ + 8 * unit->Type));
                    ++*units_killed_per_type_and_side_ptr;
                    firer_side = GetSide(firer_side_id);
                    ++firer_side->__UnitsKilled;
                  }
                  if ( _templates_unitattribs[unit->Type].__IsInfantry )
                  {
                    SetUnitAnimationType(
                      unit,
                      (unsigned char)(_WarheadData[*((unsigned char *)&_templates_bulletattribs[0].Warhead
                                                     + bullet_type__)].InfDeath
                                      + 9));
                  }
                  if ( unit_is_sandworm )
                  {
                    unit->Health = 1;
                  }
                  else
                  {
                    if ( object_side_id__ == (unsigned char)gSideId )
                    {
                      sound_id_hark = GetSoundTableID("H_UNITLOST");
                      sound_id_ord = GetSoundTableID("O_UNITLOST");
                      sound_id_atr = GetSoundTableID("S_UNITLOST");
                      PlayMentatSound(sound_id_atr, sound_id_ord, sound_id_hark, 1, 0, 0);
                    }
                    unit_flags = unit->Flags;
                    if ( unit_flags &UFLAGS_1000_DEVIATED )
                    {
                      old_side = GetSide(unit->OldSide);
                      ++old_side->__UnitsLost;
                      unit_lost_per_type_ptr = &GetSide(unit->OldSide)->__UnitsLostPerType[unit->Type];
                    }
                    else
                    {
                      object_side = object_side_;
                      ++object_side_->__UnitsLost;
                      unit_lost_per_type_ptr = &object_side->__UnitsLostPerType[unit->Type];
                    }
                    ++*unit_lost_per_type_ptr;
                    DestroyUnit((eSideType)object_side_id__, unit->MyIndex);
                  }
                }
              }
            }
          }
          unit = unit->Next;
        }
        while ( unit );
        firer_side_id_ = firer_side_id;
        object_side_id = object_side_id_;
      }
      object_side_id_ = ++object_side_id;
    }
    while ( object_side_id < (unsigned char)_SidesToProcess );
  }
  cell_index = tile_x_ + _CellNumbersWidthSpan[tile_y_];
  if ( !damage_up_in_air )
  {
    tile_flags = gGameMap.map[cell_index].__tile_bitflags;
    // New logic start
    // Make all tiles count tile damage
    // Remove check for Walk-on attribute so that tile damage is counted for any tile
    if ( true )
    // New logic end
    {
      if ( !(tile_flags & (TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE))
        && !(tile_flags & TileFlags_10_OCC_BUILDING) )
      {
        // New logic start
        // Add tile armour types
        damage_dealt = *(int *)((char *)&_templates_bulletattribs[0].__Damage + bullet_type_)
                     * (unsigned char)_WarheadData[*((unsigned char *)&_templates_bulletattribs[0].Warhead
                                                     + bullet_type_)].Verses[tile_armour_types[gGameMap.map[cell_index].__tile_index]]
                     / 100
                     / 40u;
        // New logic end
        if ( gGameMap.map[cell_index].__damage + damage_dealt >= 255 )
        {
          gGameMap.map[cell_index].__damage = 255;
        }
        else
        {
          new_damage = damage_dealt + gGameMap.map[cell_index].__damage;
          gGameMap.map[cell_index].__damage = new_damage;
          if ( new_damage > 16u )
          {
            if ( tile_flags & TileFlags_800_HAS_CONCRETE )
            {
              if ( (tile_flags & TileFlags_8000_BUILD_ON) != 0 )
              {
                // New logic start
                // When tile a is damaged, concrete flag is removed.
                // Do not remove concrete flag if the tile underneath has pre-set concrete flag (indestructible concrete)
                tile_flags &= ~TileFlags_800_HAS_CONCRETE;
                gGameMap.map[cell_index].__tile_bitflags = tile_flags | (_TileBitflags[gGameMap.map[cell_index].back_up_tile_index] & TileFlags_800_HAS_CONCRETE);
                // New logic end
                // New logic start
                // Restore back-up tile index when concrete is destroyed
                gGameMap.map[cell_index].__tile_index = gGameMap.map[cell_index].back_up_tile_index;
                // New logic end
              }
            }
          }
        }
      }
    }
  }
}
