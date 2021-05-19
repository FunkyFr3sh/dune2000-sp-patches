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
  bool result; // al
  int ypos; // ecx
  int xpos; // edx
  int crate_cash; // ST20_4
  CSide *side; // eax MAPDST
  int v8; // edi
  int v9; // edx
  int *v10; // esi
  int v11; // ecx
  char *v12; // eax
  unsigned int random; // eax MAPDST
  char v15; // ST1C_1
  CSide *v16; // eax
  char v17; // al
  unsigned int v19; // ebp
  char v21; // ST1C_1
  unsigned __int8 v23; // ST10_1 MAPDST
  unsigned __int8 v24; // ST0C_1 MAPDST
  unsigned __int8 unit_version; // al MAPDST
  unsigned __int8 v27; // ST18_1
  unsigned __int8 unit_type; // [esp+10h] [ebp+8h] MAPDST

  if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_SANDWORM )
  {
    return 0;
  }
  ypos = unit->BlockToY;
  xpos = unit->BlockToX;
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
  switch ( crate_type )
  {
    case CT_CASH:
      crate_cash = _gVariables.CrateCash;
      side = GetSide(side_id);
      CSide_add_cash_drip(side, crate_cash);
      if ( _templates_GroupIDs.EX_CASH == -1 )
      {
        return 0;
      }
      ModelAddExplosion(side_id, _templates_GroupIDs.EX_CASH, 32 * unit->BlockToX + 16, 32 * unit->BlockToY + 16, 0, 0, 0, 0, 0);
      if ( side_id != gSideId )
      {
        return 0;
      }
      PlaySoundAt(_templates_explosionattribs[(int)_templates_GroupIDs.EX_CASH].__Sound, unit->BlockToX, unit->BlockToY);
      return 0;
    case CT_EXPLODE:
      ModelAddExplosion(
        side_id,
        _templates_GroupIDs.EX_WHITEN1,
        32 * unit->BlockToX + 16,
        32 * unit->BlockToY + 16,
        0,
        0,
        0,
        0,
        0);
      if ( side_id == gSideId )
      {
        PlaySoundAt(_templates_explosionattribs[(int)_templates_GroupIDs.EX_WHITEN1].__Sound, unit->BlockToX, unit->BlockToY);
      }
      if ( _templates_GroupIDs.CRATE == -1 )
      {
        return 0;
      }
      DamageTiles(32 * unit->BlockToX + 16, 32 * unit->BlockToY + 16, 0, _templates_GroupIDs.CRATE, side_id, 0xFFFF, 0);
      return 0;
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
      do
      {
        do
        {
          random = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 8928);
          unit_type = random % _templates_UnitTypeCount;
          v15 = random % _templates_UnitTypeCount;
          v16 = GetSide(side_id);
          v17 = _templates_unitattribs[CSide__MyVersionOfUnit(v16, v15, 0)].__Behavior;
        }
        while ( v17 == UnitBehavior_SANDWORM );
      }
      while ( v17 == UnitBehavior_FRIGATE
           || v17 == UnitBehavior_ORNITHOPTER
           || v17 == UnitBehavior_CARRYALL
           || v17 == UnitBehavior_DEATH_HAND );
      side = GetSide(side_id);
      if ( _templates_unitattribs[CSide__MyVersionOfUnit(side, unit_type, 0)].__IsInfantry )
      {
        v19 = 0;
        do
        {
          do
          {
            random = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 8939);
            unit_type = random % _templates_UnitTypeCount;
            v21 = random % _templates_UnitTypeCount;
            side = GetSide(side_id);
          }
          while ( !_templates_unitattribs[CSide__MyVersionOfUnit(side, v21, 0)].__IsInfantry );
          v23 = unit->BlockToY;
          v24 = unit->BlockToX;
          side = GetSide(side_id);
          unit_version = CSide__MyVersionOfUnit(side, unit_type, 0);
          ModelAddUnit(side_id, unit_version, v24, v23, v24, v23, 0, 0);
          ++v19;
        }
        while ( v19 < 5 );
        result = 1;
      }
      else
      {
        v27 = unit->BlockToY;
        v23 = unit->BlockToY;
        v24 = unit->BlockToX;
        side = GetSide(side_id);
        unit_version = CSide__MyVersionOfUnit(side, unit_type, 0);
        ModelAddUnit(side_id, unit_version, v24, v23, v24, v27, 0, 0);
        result = 1;
      }
      return result;
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
      if ( side_id != gSideId )
      {
        return 0;
      }
      PlaySoundAt(_templates_explosionattribs[(int)_templates_GroupIDs.EX_CrateStealth].__Sound, unit->BlockToX, unit->BlockToY);
      return 0;
    case CT_UNSUPPORTED6:
      DebugFatal("UNIT.CPP", "Someone picked up an unsupported crate type (%d)", crate_type);
      return 0;
    case CT_SPICE_BLOOM_SPAWNER:
      return 0;
    case CT_SPICE_BLOOM_SMALL:
      SpiceMound(unit->BlockToX, unit->BlockToY, 4);
      result = 0;
      break;
    case CT_SPICE_BLOOM_MEDIUM:
      SpiceMound(unit->BlockToX, unit->BlockToY, 5);
      result = 0;
      break;
    case CT_SPICE_BLOOM_LARGE:
      SpiceMound(unit->BlockToX, unit->BlockToY, 6);
      result = 0;
      break;
  }
  return result;
}