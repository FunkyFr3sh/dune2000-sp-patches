#include "macros/patch.h"
#include "dune2000.h"
#include "utils.h"

// Add support for different type of infantry released from destroyed/sold building for different side
// (This is driven by Owner side setting for unit types - the first infantry owned by a side matches) 

// Custom implementation of function ReleaseInfantryFromBuilding
CALL(0x0049B9E8, _Mod__ReleaseInfantryFromBuilding);
CALL(0x0049BD1A, _Mod__ReleaseInfantryFromBuilding);

void Mod__ReleaseInfantryFromBuilding(Building *building, unsigned char side_id)
{
  char random; // al
  unsigned __int8 random16; // bl
  char randomxoff; // al
  bool v13; // al
  unsigned int yy; // [esp+14h] [ebp-18h]
  unsigned int xx; // [esp+18h] [ebp-14h]
  unsigned int y; // [esp+1Ch] [ebp-10h]
  unsigned int x; // [esp+20h] [ebp-Ch]

  if (! (_templates_buildattribs[building->Type]._____Flags & BFLAGS_200_SELECTABLE_REPAIRABLE))
    return;
  if ( building->Flags & BuildingFlags_2000 )
    return;

  building->Flags |= BuildingFlags_80000;

  // Find unit type
  int unit_type = -1;
  // First pass - find any infantry unit type
  for (int i = 0; i < gUnitTypeNum; i++)
  {
    if (_templates_unitattribs[i].__IsInfantry)
    {
      unit_type = i;
      break;
    }
  }
  // Second pass - find infantry owned by specific side
  for (int i = 0; i < gUnitTypeNum; i++)
  {
    if (_templates_unitattribs[i].__IsInfantry && (_templates_unitattribs[i].__OwnerSide & (1 << _IRValues[side_id])))
    {
      unit_type = i;
      break;
    }
  }
  if ( unit_type == -1 )
    return;

  int xpos = building->dw_field_0_x / 0x10000 / 32;
  int ypos = (building->dw_field_4_y / 0x10000 - _templates_buildattribs[building->Type]._____ArtHeight) / 32;
  int tiles_occupied_solid_bitmask = _templates_buildattribs[building->Type]._____TilesOccupiedSolid;
  int building_cost = GetBuildingCost(building->Type, 0, side_id);
  int unit_cost = w__GetUnitCost(unit_type, side_id);
  int infantry_limit = 4; // TODO rule
  int spawn_chance = 5; // TODO rule
  int num_infantry_to_release = building_cost / 3 / (unit_cost?unit_cost:1);
  num_infantry_to_release = HLIMIT(num_infantry_to_release, infantry_limit);
  int max_retries = infantry_limit * spawn_chance;
 
  int retries = 0;
  int infantry_released = 0;
  if ( num_infantry_to_release )
  {
    do
    {
      do
      {
        random = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 3461);
        random16 = random & 0xF;
        randomxoff = xpos + (random & 3);
        x = randomxoff;
        xx = randomxoff;
        y = ypos + (random16 >> 2);
        yy = ypos + (random16 >> 2);
        v13 = GetRandomAdjacentTile((unsigned __int8 *)&xx, (unsigned __int8 *)&yy);
        ++retries;
      }
      while ( (!((1 << random16) & tiles_occupied_solid_bitmask)
            || !v13
            || gGameMap.map[(unsigned __int8)x + _CellNumbersWidthSpan[(unsigned __int8)y]].__tile_bitflags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_8_OCC_UNIT))
           && retries < max_retries );
      if ( retries < max_retries )
      {
        ModelAddUnit(side_id, unit_type, x, y, xx, yy, 0, 0);
      }
      ++infantry_released;
    }
    while ( infantry_released < num_infantry_to_release );
  }
}
