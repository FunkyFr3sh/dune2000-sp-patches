#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"
#include "utils.h"

void HandleExplosionCrate(CrateStruct *crate, Unit *unit, unsigned char side_id);
void HandleSpiceBloomCrate(CrateStruct *crate, int crate_type, Unit *unit, unsigned char side_id);

// Fix disappearing crates when AI-controlled unit is nearby
CLEAR(0x0044EA24, 0x90, 0x0044EA29); // Clear call to RecycleCrate in function GetCrateFromMap
CLEAR(0x0044EA42, 0x90, 0x0044EA49); // Clear deactivation of crate in function GetCrateFromMap 
CLEAR(0x0044EA5F, 0x90, 0x0044EA66);

// Custom implementation of function PickupCrate
CALL(0x00494480, _Mod__PickupCrate);

bool Mod__PickupCrate(Unit *unit, unsigned char side_id)
{
  int v8; // edi
  int v9; // edx
  int *v10; // esi
  int v11; // ecx
  char *v12; // eax

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
  if ( crate_type != CT_SPICE_BLOOM_SPAWNER )
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
      if ( _templates_GroupIDs.EX_CASH == -1 )
      {
        return 0;
      }
      ModelAddExplosion(side_id, _templates_GroupIDs.EX_CASH, 32 * unit->BlockToX + 16, 32 * unit->BlockToY + 16, 0, 0, 0, 0, 0);
      if ( side_id == gSideId )
      {
        PlaySoundAt(_templates_explosionattribs[(int)_templates_GroupIDs.EX_CASH].__Sound, unit->BlockToX, unit->BlockToY);
      }
      return 0;
    }
    case CT_EXPLODE:
    {
      HandleExplosionCrate(crate, unit, side_id);
      return 0;
    }
    case CT_REVEAL:
      if ( _templates_GroupIDs.EX_CrateReveal == -1 )
      {
        goto LABEL_12;
      }
      ModelAddExplosion(
        side_id,
        _templates_GroupIDs.EX_CrateReveal,
        32 * unit->BlockToX + 16,
        32 * unit->BlockToY + 16,
        0,
        0,
        0,
        0,
        0);
      if ( side_id != gSideId )
      {
        return 0;
      }
      PlaySoundAt(_templates_explosionattribs[(int)_templates_GroupIDs.EX_CrateReveal].__Sound, unit->BlockToX, unit->BlockToY);
LABEL_12:
      if ( side_id != gSideId )
      {
        return 0;
      }
      RevealMap();
      BlitClipTImage1(_RadarMap1, 0, 0, _RadarMap2, 0, 0, 0);
      return 0;
    case CT_NOMAP:
      if ( _templates_GroupIDs.EX_CrateNoMap == -1 )
      {
        goto LABEL_17;
      }
      ModelAddExplosion(
        side_id,
        _templates_GroupIDs.EX_CrateNoMap,
        32 * unit->BlockToX + 16,
        32 * unit->BlockToY + 16,
        0,
        0,
        0,
        0,
        0);
      if ( side_id != gSideId )
      {
        return 0;
      }
      PlaySoundAt(_templates_explosionattribs[(int)_templates_GroupIDs.EX_CrateNoMap].__Sound, unit->BlockToX, unit->BlockToY);
LABEL_17:
      if ( side_id != gSideId )
      {
        return 0;
      }
      ClearTImage(_RadarMap1, 0, 0);
      if ( GetMapVisState() != _mapvisstate_548010 )
      {
        _cheatstates[gSideId] |= 1u;
      }
      v8 = 0;
      if ( gGameMap.height > 0 )
      {
        v9 = gGameMap.width;
        v10 = _CellNumbersWidthSpan;
        do
        {
          v11 = 0;
          if ( v9 > 0 )
          {
            v12 = &gGameMap.map[*v10].__shroud_flags;
            do
            {
              *v12 = 1;
              v9 = gGameMap.width;
              ++v11;
              v12 += 12;
            }
            while ( v11 < gGameMap.width );
          }
          ++v8;
          ++v10;
        }
        while ( v8 < gGameMap.height );
      }
      _mapvisstate_548010 = GetMapVisState();
      side_mapvis_49F4D0(side_id);
      return 0;
    case CT_UNIT:
    {
      // Extension data: VIUUUUUU
      // V = Call MyVersionOfUnit? (1 = yes, 0 = no)
      // I = Infantry amount (0 = one infantry, 1 = five infantry)
      // U = Unit type (0-59)
      
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
    case CT_STEALTH:
      MakeUnitsStealthInRange(unit->BlockToX, unit->BlockToY, side_id);
      if ( _templates_GroupIDs.EX_CrateStealth == -1 )
      {
        return 0;
      }
      ModelAddExplosion(
        side_id,
        _templates_GroupIDs.EX_CrateStealth,
        32 * unit->BlockToX + 16,
        32 * unit->BlockToY + 16,
        0,
        0,
        0,
        0,
        0);
      if ( side_id == gSideId )
      {
        PlaySoundAt(_templates_explosionattribs[(int)_templates_GroupIDs.EX_CrateStealth].__Sound, unit->BlockToX, unit->BlockToY);
      }
      return 0;
    case CT_UNSUPPORTED6:
      DebugFatal("UNIT.CPP", "Someone picked up an unsupported crate type (%d)", crate_type);
      return 0;
    case CT_SPICE_BLOOM_SPAWNER:
      return 0;
    case CT_SPICE_BLOOM_SMALL:
    case CT_SPICE_BLOOM_MEDIUM:
    case CT_SPICE_BLOOM_LARGE:
      HandleSpiceBloomCrate(crate, crate_type, unit, side_id);
      return 0;
      break;
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
  
  ModelAddExplosion(
    side_id,
    explosion_id,
    target_x,
    target_y,
    0,
    0,
    0,
    0,
    0);
  if ( side_id == gSideId )
  {
    PlaySoundAt(_templates_explosionattribs[explosion_id].__Sound, xpos, ypos);
  }
  if ( weapon_id != -1 )
  {
    DamageTiles(target_x, target_y, 0, weapon_id, side_id, 0xFFFF, 0);
  }  
}

bool notspiceon(int x, int y)
{
  x = MAX(x, 0);
  y = MAX(y, 0);
  x = MIN(x, gGameMapWidth - 1);
  y = MIN(y, gGameMapHeight - 1);
  return (gGameMap.map[x + _CellNumbersWidthSpan[y]].__tile_bitflags & (TileFlags_100000_SPICE | TileFlags_200000_SPICE | TileFlags_400000_SPICE)) == 0;
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
  
  // Extended behavior: instant spice blooms
  if (mode != 0)
  {
    // Get the circle
    char *circle = NULL;
    if (mode == 2 || mode == 3)
    {
      range = MIN(range, 7);
      char **circle_ptr = &_ptr_circle_1x1grid;
      circle = circle_ptr[range];
    }
    
    // Add spice on tiles
    for (int y = 0; y <= range * 2; y++)
    {
      int yy = ypos + y - range;
      if (yy < 0 || yy >= gGameMapHeight)
        continue;
      for (int x = 0; x <= range * 2; x++)
      {
        int xx = xpos + x - range;
        if (xx < 0 || xx >= gGameMapWidth)
          continue;
        if (circle && circle[x + y * (range * 2 + 1)])
          continue;
        GameMapTileStruct *tile = &gGameMap.map[xx + _CellNumbersWidthSpan[yy]];
        if (!(tile->__tile_bitflags & TileFlags_10000_SANDY))
          continue;
        // Use randomizer
        if (crate->ext_data_field & 8 && !(xx == xpos && yy == ypos) && (rand() % 100) < 40)
          continue;
        // Update pixels on radar
        if ( gBitsPerPixel == 16 )
          SetPixelOnRadar16(xx, yy, _radarcolor_word_517898_spicecolor);
        else
          SetPixelOnRadar8(xx, yy, _radarcolor_byte_517780_spicecolor);
        // Add two pieces of spice
        int spice_amount = (tile->__tile_bitflags >> 20) & 7;
        spice_amount = MIN(spice_amount + 2, (mode == 3)?4:2);
        tile->__tile_bitflags &= ~(TileFlags_100000_SPICE | TileFlags_200000_SPICE | TileFlags_400000_SPICE);
        tile->__tile_bitflags |= (spice_amount << 20);
      }
    }
    // Dune 2 style spice bloom
    if (mode == 3)
    {
      // Second pass: Correct tiles with more than two pieces of spice where not all surrounding tiles have spice
      for (int y = 0; y <= range * 2; y++)
      {
        int yy = ypos + y - range;
        if (yy < 0 || yy >= gGameMapHeight)
          continue;
        for (int x = 0; x <= range * 2; x++)
        {
          int xx = xpos + x - range;
          if (xx < 0 || xx >= gGameMapWidth)
            continue;
          if (circle && circle[x + y * (range * 2 + 1)])
            continue;
          GameMapTileStruct *tile = &gGameMap.map[xx + _CellNumbersWidthSpan[yy]];
          if (!(tile->__tile_bitflags & TileFlags_10000_SANDY))
            continue;
          // Check spice amount and surrounding tiles
          int spice_amount = (tile->__tile_bitflags >> 20) & 7;
          if (spice_amount > 2 &&
            (  notspiceon(xx - 1, yy - 1)
            || notspiceon(xx, yy - 1)
            || notspiceon(xx + 1, yy - 1)
            || notspiceon(xx - 1, yy)
            || notspiceon(xx + 1, yy)
            || notspiceon(xx - 1, yy + 1)
            || notspiceon(xx, yy + 1)
            || notspiceon(xx + 1, yy + 1)
            ))
          {
            // Reset spice to 2 pieces
            tile->__tile_bitflags &= ~(TileFlags_100000_SPICE | TileFlags_200000_SPICE | TileFlags_400000_SPICE);
            tile->__tile_bitflags |= TileFlags_200000_SPICE;
          }
        }
      }
      // Shake screen
      _ScreenShakes = 10;
      // Play sound
      int sample_id = Data__GetSoundTableID("S_SPICEMOUND");
      PlaySoundAt(sample_id, xpos, ypos);
    }
    // Update spice visuals
    RECT r;
    r.left = MAX(xpos - range - 1, 0);
    r.top = MAX(ypos - range - 1, 0);
    r.right = MIN(xpos + range + 2, gGameMapWidth);
    r.bottom = MIN(ypos + range + 2, gGameMapHeight);
    UpdateSpiceInRegion(&r);
  }
  // Standard behavior: original spice bloom
  else
  {
    SpiceMound(xpos, ypos, range);
  }
}

