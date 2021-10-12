#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"
#include "utils.h"
#include "../event-system/event-core.h"
#include "../event-system/event-actions.h"

void HandleExplosionCrate(CrateStruct *crate, Unit *unit, unsigned char side_id);
void HandleSpiceBloomCrate(CrateStruct *crate, int crate_type, Unit *unit, unsigned char side_id);

// Custom implementation of function GetCrateFromMap
// This fixes bug with disappearing crates when AI-controlled unit is nearby
CALL(0x0041FB65, _Mod__GetCrateFromMap);

unsigned char Mod__GetCrateFromMap(int xpos, int ypos)
{
  for (int i = 0; i < 30; i++)
  {
    if (gCrates[i].__x == xpos && gCrates[i].__y == ypos && gCrates[i].__is_active)
    {
      unsigned char crate_type = gCrates[i].__type;
      // AI won't attempt to collect explosive crates with non zero extension data - these are considered as statically placed mines
      if (crate_type == CT_EXPLODE && gCrates[i].ext_data_field)
        return 11;
      // AI won't attempt to collect crates with one-white-pixel sprite - these are considered as "invisible"
      if (gCrates[i].__image == 4)
        return 11;
      return crate_type;
    }
  }
  return 11;
}

// Custom implementation of function PickupCrate
CALL(0x00494480, _Mod__PickupCrate);

bool Mod__PickupCrate(Unit *unit, unsigned char side_id)
{
  int xpos = unit->BlockToX;
  int ypos = unit->BlockToY;
  // Sandworm cannot pickup crates
  if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_SANDWORM )
  {
    return 0;
  }
  // There is no crate on the target tile
  if ( !(gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]].__tile_bitflags & TileFlags_1000) )
  {
    return 0;
  }
  // Find crate on a tile
  CrateStruct *crate = NULL;
  int crate_index = -1;
  for (int i = 0; i < 30; i++)
  {
    if (gCrates[i].__x == xpos && gCrates[i].__y == ypos && gCrates[i].__is_active)
    {
      crate = &gCrates[i];
      crate_index = i;
      break;
    }
  }
  if (!crate)
  {
    return 0;
  }
  int crate_type = crate->__type;
  // Guard against "Tile already occupied with unit" error
  if (crate_type == CT_UNIT && gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]].__tile_bitflags & TileFlags_8_OCC_UNIT)
  {
    return 0;
  }
  // Remove crate from map
  // Spice bloom spawner and stealth crate removal is handled elsewhere
  if ( crate_type != CT_SPICE_BLOOM_SPAWNER && crate_type != CT_POWERUP )
  {
    if (crate_type > CT_SPICE_BLOOM_SPAWNER && crate_type <= CT_SPICE_BLOOM_LARGE )
    {
      RecycleCrate(crate_index);
    }
    else
    {
      crate->__is_active = 0;
      gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]].__tile_bitflags &= ~TileFlags_1000;
    }
  }
  // Do crate action
  CSide *side = GetSide(side_id);
  switch ( crate_type )
  {
    case CT_CASH:
    {
      // Extended behavior: amount of cash = ext_data_field * 100;
      int crate_cash = crate->ext_data_field ? crate->ext_data_field * 100 : _gVariables.CrateCash;
      CSide_add_cash_drip(side, crate_cash);
      if ( _templates_GroupIDs.EX_CASH != -1 )
      {
        ModelAddExplosion(side_id, _templates_GroupIDs.EX_CASH, 32 * unit->BlockToX + 16, 32 * unit->BlockToY + 16, 0, 0, 0, 0, 0);
        if ( side_id == gSideId )
        {
          PlaySoundAt(_templates_explosionattribs[(int)_templates_GroupIDs.EX_CASH].__Sound, unit->BlockToX, unit->BlockToY);
        }
      }
      return 0;
    }
    case CT_EXPLODE:
    {
      HandleExplosionCrate(crate, unit, side_id);
      return 0;
    }
    case CT_REVEAL:
    {
      // Extension data: XXXYYYRR
      // X = Relative X-offset (3-bit value, possible offsets: 0, 4, 8, 12, -16, -12, -8, -4)
      // Y = Relative Y-offset (3-bit value, possible offsets: 0, 4, 8, 12, -16, -12, -8, -4)
      // R = Range (2-bit value, possible ranges: 4, 5, 6, 7)
      // Reveal all map (default behavior) if extension data is zero
      
      // Extended behavior
      if (crate->ext_data_field)
      {
        if ( side_id != gSideId )
        {
          return 0;
        }
        int xx = (crate->ext_data_field >> 5) & 7;
        if (xx & 4)
          xx |= 0xfffffffc;
        xx = LIMIT(xpos + (xx * 4), 0, gGameMapWidth - 1);
        int yy = (crate->ext_data_field >> 2) & 7;
        if (yy & 4)
          yy |= 0xfffffffc;
        yy = LIMIT(ypos + (yy * 4), 0, gGameMapHeight - 1);
        int range = (crate->ext_data_field & 3) + 4;
        RevealCircle(xx, yy, range);
      }
      // Default behavior
      else
      {
        if ( _templates_GroupIDs.EX_CrateReveal != -1 )
        {
          ModelAddExplosion(side_id, _templates_GroupIDs.EX_CrateReveal, 32 * unit->BlockToX + 16, 32 * unit->BlockToY + 16, 0, 0, 0, 0, 0);
          if (side_id == gSideId)
          {
            PlaySoundAt(_templates_explosionattribs[(int)_templates_GroupIDs.EX_CrateReveal].__Sound, xpos, ypos);
          }
        }
        if ( side_id == gSideId )
        {
          RevealMap();
          BlitClipTImage1(_RadarMap1, 0, 0, _RadarMap2, 0, 0, 0);
        }
      }
      return 0;
    }
    case CT_NOMAP:
    {
      if ( _templates_GroupIDs.EX_CrateNoMap != -1 )
      {
        ModelAddExplosion(side_id, _templates_GroupIDs.EX_CrateNoMap, 32 * unit->BlockToX + 16, 32 * unit->BlockToY + 16, 0, 0, 0, 0, 0);
        if ( side_id == gSideId )
        {
          PlaySoundAt(_templates_explosionattribs[(int)_templates_GroupIDs.EX_CrateNoMap].__Sound, unit->BlockToX, unit->BlockToY);
        }
      }
      if ( side_id == gSideId )
      {
        EvAct_HideMap();
      }
      return 0;
    }
    case CT_UNIT:
    {
      // Extension data: VIUUUUUU
      // V = Call MyVersionOfUnit? (1 = yes, 0 = no)
      // I = Infantry amount (0 = one infantry, 1 = five infantry)
      // U = Unit type (0-59)
      // Give random unit (default behavior) if extension data is zero
      
      // Specific unit = extended behavior
      if (crate->ext_data_field)
      {
        int unit_type = crate->ext_data_field & 0x3f;
        int unit_version = (crate->ext_data_field & 0x80) ? CSide__MyVersionOfUnit(side, unit_type, 0) : unit_type;
        ModelAddUnit(side_id, unit_version, xpos, ypos, xpos, ypos, 0, 0);
        if (_templates_unitattribs[unit_version].__IsInfantry && (crate->ext_data_field & 0x40))
        {
          for (int i = 1; i < 5; i++)
            ModelAddUnit(side_id, unit_version, xpos, ypos, xpos, ypos, 0, 0);
        }
        return 1;
      }
      // Random unit = default behavior
      int unit_type;
      int unit_version;
      int behavior;
      do
      {
        unit_type = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 8928) % _templates_UnitTypeCount;
        unit_version = CSide__MyVersionOfUnit(side, unit_type, 0);
        behavior = _templates_unitattribs[unit_version].__Behavior;
      }
      while ( behavior == UnitBehavior_SANDWORM
           || behavior == UnitBehavior_FRIGATE
           || behavior == UnitBehavior_ORNITHOPTER
           || behavior == UnitBehavior_CARRYALL
           || behavior == UnitBehavior_DEATH_HAND );
      ModelAddUnit(side_id, unit_version, xpos, ypos, xpos, ypos, 0, 0);
      if ( _templates_unitattribs[unit_version].__IsInfantry )
      {
        for (int i = 1; i < 5; i++)
        {
          do
          {
            unit_type = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 8939) % _templates_UnitTypeCount;
            unit_version = CSide__MyVersionOfUnit(side, unit_type, 0);
          }
          while ( !_templates_unitattribs[unit_version].__IsInfantry );  
          ModelAddUnit(side_id, unit_version, xpos, ypos, xpos, ypos, 0, 0);
        }
      }
      return 1;
    }
    case CT_POWERUP:
    {
      // Originally stealth-type crate
      // Extension data: SP-MMMEE
      // S = Play pickup animation and sound? (1 = yes, 0 = no)
      // P = Always pickup? (0 = Pickup the crate only if it has any actual effect, 1 = Pick up crate always)
      // - = Unused (reserved)
      // M = Crate mode:
      //     0 = Make unit stealth
      //     1 = Restore full health
      //     2 = Restore 50% health
      //     3 = Restore 25% health
      //     4 = Change unit type: 
      //         The target type for specific unit type is specified in unit definition in templates.bin:
      //         byte 0xAB: UnitUpgradeAllowed (1 = yes, 0 = no)
      //         byte 0xAC: UnitUpgradeTargetType
      //     5, 6, 7 = Unused (reserved)      
      // E = Range of effect (0 = only the unit picking up crate, 1/2/3 = units max. 1/2/3 tiles far from the crate)
      // Make units stealth in range of 2, always pickup (default behavior) if extension data is zero
      
      // Values for the default behavior
      bool play_animation = true;
      bool always_pickup = true;
      int mode = 0;
      int range = 2;
      int explosion_id = _templates_GroupIDs.EX_CrateStealth;
      
      // Extened behavior
      if (crate->ext_data_field)
      {
        play_animation = (crate->ext_data_field & 128) != 0;
        always_pickup = (crate->ext_data_field & 64) != 0;
        mode = (crate->ext_data_field >> 2) & 7;
        range = crate->ext_data_field & 3;
        explosion_id = _templates_GroupIDs.CrateAnimations[mode];
        if (crate->ext_data_field & 32)
          DebugFatal("crates-func.c", "Unused bit in ext_data_field is set to 1", mode);
      }
      
      bool crate_used = always_pickup;
      
      // Do the action
      for (Unit *u = GetSide(side_id)->_Units_8; u; u = u->Next)
      {
        if ((range == 0 && u == unit) || ((range > 0) && (abs(u->BlockFromX - xpos) <= range) && (abs(u->BlockFromY - ypos) <= range)))
        {
          switch (mode)
          {
            case 0:
              // Make unit stealth
              if ( _templates_unitattribs[u->Type].__Behavior == UnitBehavior_SABOTEUR )
              {
                u->__SpecialVal_S = -96;
                crate_used = true;
              }
              else if ( !Unit_49F5F0(u) )
              {
                u->Flags |= UFLAGS_10_STEALTH;
                u->S_c_field_31 = 0;
                crate_used = true;
              }
              break;
            case 1:
              if (u->Health == _templates_unitattribs[u->Type].__Strength)
                continue;
              u->Health = _templates_unitattribs[u->Type].__Strength;
              crate_used = true;
              break;
            case 2:
              if (u->Health == _templates_unitattribs[u->Type].__Strength)
                continue;
              u->Health = HLIMIT(u->Health + _templates_unitattribs[u->Type].__Strength / 2, _templates_unitattribs[u->Type].__Strength);
              crate_used = true;
              break;
            case 3:
              if (u->Health == _templates_unitattribs[u->Type].__Strength)
                continue;
              u->Health = HLIMIT(u->Health + _templates_unitattribs[u->Type].__Strength / 4, _templates_unitattribs[u->Type].__Strength);
              crate_used = true;
              break;
            case 4:
              // Change unit type
              if (!_templates_unitattribs[u->Type].UnitUpgradeAllowed)
                continue;
              int target_type = _templates_unitattribs[u->Type].UnitUpgradeTargetType;
              u->Health = LLIMIT(u->Health + _templates_unitattribs[target_type].__Strength - _templates_unitattribs[u->Type].__Strength, 1);
              u->Flags = (u->Flags & (~_templates_unitattribs[u->Type].Flags)) | _templates_unitattribs[target_type].Flags;
              u->Speed = _templates_unitattribs[target_type].__Speed;
              u->Type = target_type;
              crate_used = true;
              break;
            default:
              DebugFatal("crates-func.c", "Unit powerup crate mode %d not implemented", mode);
          }
        }
      }
      
      // Play pickup animation and sound
      if (play_animation && crate_used && explosion_id != -1)
      {
        ModelAddExplosion(side_id, explosion_id, 32 * unit->BlockToX + 16, 32 * unit->BlockToY + 16, 0, 0, 0, 0, 0);
        if ( side_id == gSideId )
        {
          PlaySoundAt(_templates_explosionattribs[explosion_id].__Sound, unit->BlockToX, unit->BlockToY);
        }
      }
      
      // Remove the crate here
      if (crate_used)
      {
        crate->__is_active = 0;
        gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]].__tile_bitflags &= ~TileFlags_1000;        
      }
      
      return 0;
    }
    case CT_EXECUTE_EVENT:
    {
      // New crate type: execute event
      ProcessEvent(crate->ext_data_field);  
      return 0;
    }
    case CT_SPICE_BLOOM_SPAWNER:
      return 0;
    case CT_SPICE_BLOOM_SMALL:
    case CT_SPICE_BLOOM_MEDIUM:
    case CT_SPICE_BLOOM_LARGE:
      HandleSpiceBloomCrate(crate, crate_type, unit, side_id);
      return 0;
      break;
    default:
      DebugFatal("UNIT.CPP", "Someone picked up an unsupported crate type (%d)", crate_type);
  }
  return 0;
}

void HitCrate(int crate_index)
{
  CrateStruct *crate = &gCrates[crate_index];
  int crate_type = crate->__type;
  // Spice bloom
  if (crate_type >= CT_SPICE_BLOOM_SMALL && crate_type <= CT_SPICE_BLOOM_LARGE && !(crate->ext_data_field & 128))
  {
    RecycleCrate(crate_index);
    HandleSpiceBloomCrate(crate, crate_type, NULL, gSideId);
    return;
  }
  // Explosion crate
  if (crate->__type == CT_EXPLODE && crate->ext_data_field & 128)
  {
    // Remove crate
    crate->__is_active = 0;
    gGameMap.map[crate->__x + _CellNumbersWidthSpan[crate->__y]].__tile_bitflags &= ~TileFlags_1000;
    // Do damage
    HandleExplosionCrate(crate, NULL, gSideId);
    return;
  }
}

void HandleExplosionCrate(CrateStruct *crate, Unit *unit, unsigned char side_id)
{
  // Extension data: DAWWWWWW
  // D = Can be destroyed when shot? (1 = yes, 0 = no)
  // A = Area of effect (0 = center of tile, 1 = edge of tile nearest to the unit picking up the crate)
  // W = Weapon type (0-63)
  // Use default crate weapon (default behavior) if extension data is zero

  int xpos = crate->__x;
  int ypos = crate->__y;
  
  int weapon_id = _templates_GroupIDs.CRATE;
  int explosion_id = _templates_GroupIDs.EX_WHITEN1;
  
  // Extended behavior: custom weapon and use of hit explosion
  if (crate->ext_data_field)
  {
    weapon_id = crate->ext_data_field & 0x3f;
    if (_templates_bulletattribs[weapon_id].__HitExplosion != -1)
    {
      explosion_id = _templates_bulletattribs[weapon_id].__HitExplosion;
    }
  }
  
  int target_x = 32 * xpos + 16;
  int target_y = 32 * ypos + 16;

  // Extended behavior: make the damage closer to the unit picking up the crate 
  if (crate->ext_data_field & 64 && unit)
  {
    if (unit->BlockFromX < xpos)
      target_x -= 16;
    else if (unit->BlockFromX > xpos)
      target_x += 15;
    if (unit->BlockFromY < ypos)
      target_y -= 16;
    else if (unit->BlockFromY > ypos)
      target_y += 15;
  }
  
  ModelAddExplosion(side_id, explosion_id, target_x, target_y, 0, 0, 0, 0, 0);
  if ( side_id == gSideId )
  {
    PlaySoundAt(_templates_explosionattribs[explosion_id].__Sound, xpos, ypos);
  }
  if ( weapon_id != -1 )
  {
    DamageTiles(target_x, target_y, 0, weapon_id, side_id, 0xFFFF, 0);
  }  
}

void HandleSpiceBloomCrate(CrateStruct *crate, int crate_type, Unit *unit, unsigned char side_id)
{
  // Extension data: DUMMRRRR
  // D = Can NOT be destroyed when shot? (1 = yes, 0 = no)
  // U = Always destroy unit which moved on spice bloom? (1 = yes, 0 = no)
  // M = Spice bloom mode: 
  //     0 = Classic spice bloom
  //     1 = Instant spice bloom: square, do not create thick spice, no sound
  //     2 = Instant spice bloom: circle, do not create thick spice, no sound
  //     3 = Instant spice bloom (Dune 2 style): circle, create thick spice when overlap with existing spice, screen shakes, play sound
  // R = Custom range 
  //     For classic spice bloom it can be 0-15 (4 bits)
  //     For instant spice bloom it can be 0-7  (3 bits)
  //     For instant spice bloom the 4th bit functions as randomizer (spice will be randomly scattered rather than filling whole area)
  //     If crate type is Medium spice bloom then range is increased by 1
  //     If crate type is Large spice bloom then range is increased by 2
  //     For instant circle spice blooms the maximum range is 7 (limitation of CIRCLES.BIN)
  //     For classic spice bloom the maximum range that can be specified is 17 (15 + 2 if Large spice bloom)
  // Classic spice bloom (default behavior) if extension data is zero

  
  // Extended behavior: spice bloom mode
  int mode = (crate->ext_data_field >> 4) & 3;

  // Default range for vanilla small spice bloom
  int range = 4;
  // Extended behavior: custom range
  if (crate->ext_data_field)
    range = (mode == 0)?crate->ext_data_field & 15:crate->ext_data_field & 7;
  // Range is bigger for medium and large blooms 
  if (crate_type == CT_SPICE_BLOOM_MEDIUM)
    range += 1;
  if (crate_type == CT_SPICE_BLOOM_LARGE)
    range += 2;
  
  // Extended behavior: destroy unit
  if (crate->ext_data_field & 64 && unit)
  {
    DestroyUnit(side_id, unit->MyIndex);
  }
  
  int xpos = crate->__x;
  int ypos = crate->__y;
  
  EvAct_SpiceBloom(xpos, ypos, range, mode, (crate->ext_data_field & 8) == 8);
}

