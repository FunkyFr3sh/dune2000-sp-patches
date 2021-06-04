#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"
#include "utils.h"

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
  // Remove crate from map
  int crate_type = crate->__type;
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
      // Extended data: SEWWWWWW
      // S = Use specific weapon? (1 = yes, 0 = no)
      // E = Use weapon's hit explosion? (1 = yes, 0 = no)
      // W = Weapon type (0-64)
      
      // Extended behavior: custom weapon and use of hit explosion
      int weapon_id = (crate->ext_data_field & 0x80) ? crate->ext_data_field & 0x3f : _templates_GroupIDs.CRATE;
      int explosion_id = (crate->ext_data_field & 0x40) ? _templates_bulletattribs[weapon_id].__HitExplosion : _templates_GroupIDs.EX_WHITEN1;
      
      ModelAddExplosion(
        side_id,
        explosion_id,
        32 * unit->BlockToX + 16,
        32 * unit->BlockToY + 16,
        0,
        0,
        0,
        0,
        0);
      if ( side_id == gSideId )
      {
        PlaySoundAt(_templates_explosionattribs[explosion_id].__Sound, unit->BlockToX, unit->BlockToY);
      }
      if ( weapon_id != -1 )
      {
        DamageTiles(32 * unit->BlockToX + 16, 32 * unit->BlockToY + 16, 0, weapon_id, side_id, 0xFFFF, 0);
      }
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
      // Extended data: VIUUUUUU
      // V = Call MyVersionOfUnit? (1 = yes, 0 = no)
      // I = Infantry amount (0 = one infantry, 1 = five infantry)
      // U = Unit type (0-64)
      
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
      SpiceMound(unit->BlockToX, unit->BlockToY, 4);
      return 0;
      break;
    case CT_SPICE_BLOOM_MEDIUM:
      SpiceMound(unit->BlockToX, unit->BlockToY, 5);
      return 0;
      break;
    case CT_SPICE_BLOOM_LARGE:
      SpiceMound(unit->BlockToX, unit->BlockToY, 6);
      return 0;
      break;
  }
  return 0;
}