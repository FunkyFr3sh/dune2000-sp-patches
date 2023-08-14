#include "macros/patch.h"
#include "dune2000.h"
#include "../event-system/event-core.h"
#include "messages-func.h"

bool StartWithMCV = true;

double sqrt(double n){
    __asm__(
            "fsqrt\n"
            :"+t" (n)
            );
    return n;
}

void PlaceStaticCrateExt(uint8_t x, uint8_t y, eCrateType type, eCrateImage image, uint8_t ext_data_field)
{
  int index = GetFreeCrateIndex();
  if ( index != -1 )
  {
    gCrates[index].__is_active = 1;
    gCrates[index].__x = x;
    gCrates[index].__y = y;
    gCrates[index].__type = type;
    gCrates[index].__image = image;
    gCrates[index].__timing = INT32_MAX;
    gCrates[index].__times_to_respawn = 0;
    gCrates[index].ext_data_field = ext_data_field;
    GameMapTileStruct *tile = &gGameMap.map[x + _CellNumbersWidthSpan[y]];
    if (type == CT_SPICE_BLOOM_SPAWNER)
    {
      gCrates[index].__image = 4;
      gCrates[index].__timing = rand() % 1000 + 2000;
      if (image & 1)
        gCrates[index].__times_to_respawn = 255;
      if ((image & 6) == 2)
        tile->__tile_bitflags = (tile->__tile_bitflags & ~0x700000) | TileFlags_200000_SPICE;
      else if ((image & 6) == 4)
        tile->__tile_bitflags = (tile->__tile_bitflags & ~0x700000) | TileFlags_400000_SPICE;
      else if ((image & 6) == 6)
        gCrates[index].__timing = 1;
    }
    tile->__tile_bitflags |= TileFlags_1000_HAS_CRATE;
  }
}

CALL(0x004488F8, _Mod__setupmapstuff);

void Mod__setupmapstuff()
{
  int v8; // eax
  int object_type; // ecx
  unsigned int i; // esi
  int *ptr_1; // ebp
  int v25; // eax
  bool v26; // zf
  int v27; // eax
  int v28; // eax
  unsigned __int8 v29; // ST14_1
  unsigned __int8 v30; // ST10_1
  int v31; // ST08_1
  CSide *v32; // eax
  char v33; // al
  char v49; // ST1C_1
  CSide *v50; // eax
  char v51; // al
  unsigned __int8 v52; // cl
  unsigned __int8 v53; // di
  unsigned __int8 v54; // dl
  char *v55; // eax
  signed int v56; // ecx
  char v57; // al
  int v58; // ebx
  CAI_ *v59; // edi
  unsigned __int8 total_players; // al
  char v61; // bl
  eSideType v62; // di
  int v63; // esi
  int v64; // eax
  signed int mob_const_vehicle; // ebp
  signed int v66; // ecx
  char *v67; // esi
  signed int v68; // edi
  unsigned __int8 player_count; // cl
  unsigned __int8 total_players_2; // bl
  char initial_spawn_fetch; // al
  LONG *v72; // ebp
  unsigned int v73; // edi
  unsigned __int8 v74; // bl
  LONG *v75; // esi
  int v76; // eax
  signed __int64 v77; // rax
  bool v78; // cf
  int v79; // ecx
  eSideType v80; // si
  unsigned __int8 v81; // ST18_1
  unsigned __int8 v82; // ST14_1
  unsigned __int8 v83; // ST10_1
  unsigned __int8 v84; // ST0C_1
  char v85; // ST04_1
  CSide *v86; // eax
  unsigned __int8 v87; // al
  int v88; // eax
  char v89; // dl
  int v90; // ebx
  int v91; // ecx
  CSide *v92; // eax
  unsigned int v93; // esi
  int *v94; // ebx
  eSideType v95; // di
  char v96; // ST1C_1
  CSide *v97; // eax
  unsigned __int8 v98; // cl
  char v99; // [esp-8h] [ebp-17E0h]
  char *yposa; // [esp+10h] [ebp-17C8h]
  int yposb; // [esp+10h] [ebp-17C8h]
  unsigned __int8 obja; // [esp+14h] [ebp-17C4h]
  unsigned __int8 objb; // [esp+14h] [ebp-17C4h]
  unsigned int objects_on_map; // [esp+18h] [ebp-17C0h]
  char *v107; // [esp+1Ch] [ebp-17BCh]
  unsigned __int8 v108; // [esp+1Ch] [ebp-17BCh]
  unsigned __int8 x; // [esp+20h] [ebp-17B8h]
  unsigned __int8 side; // [esp+24h] [ebp-17B4h]
  unsigned __int8 y; // [esp+28h] [ebp-17B0h]
  unsigned __int8 total_players_22; // [esp+2Fh] [ebp-17A9h]
  unsigned int worm_count; // [esp+30h] [ebp-17A8h]
  int a2; // [esp+34h] [ebp-17A4h]
  eSideType a1; // [esp+38h] [ebp-17A0h]
  char type; // [esp+3Ch] [ebp-179Ch]
  unsigned __int8 v117; // [esp+40h] [ebp-1798h]
  unsigned __int8 spawn_location; // [esp+44h] [ebp-1794h]
  int v119; // [esp+48h] [ebp-1790h]
  unsigned int v120; // [esp+4Ch] [ebp-178Ch]
  unsigned __int8 v121; // [esp+50h] [ebp-1788h]
  char v122; // [esp+54h] [ebp-1784h]
  RECT v123; // [esp+58h] [ebp-1780h]
  // Increase limit of total pre-placed structures on map from 1000 to 4000
  char object_array_xpos[4000]; // [esp+68h] [ebp-1770h]   // Array size changed from 1000 to 4000 items (mod)
  char object_array_ypos[4000]; // [esp+450h] [ebp-1388h]  // Array size changed from 1000 to 4000 items (mod)
  int object_array[4000]; // [esp+838h] [ebp-FA0h]         // Array size changed from 1000 to 4000 items (mod)

  spawn_location = -1;
  objects_on_map = 0;
  worm_count = 0;
  type = -1;
  _SpawnLocationCount = 0;

  // Initialize font colors to default values upon restart
  if (gRestartGame)
    InitFontColors();

  // Store the original side ID upon starting a map
  gOldSideId = gSideId;

  // Reset last played property of sounds in sound table
  for (int i = 0; i < _sampletablecount; i++)
    gSampleTable[i]->last_played = 0;

  // Clean garbage data for conditions which use some fields for internal data storage
  for (int i = 0; i < _gConditionCount; i++)
  {
    // Clean val4 (base time) for Timer and Interval conditions
    if (_gConditionArray[i].condition_type == CT_INTERVAL || _gConditionArray[i].condition_type == CT_TIMER)
      _gConditionArray[i].val4 = 0;
    // Clean val3 (flag value) for Flag conditions
    if (_gConditionArray[i].condition_type == CT_FLAG)
      _gConditionArray[i].val3 = 0;
  }

  // Reset all event variables
  for (int i = 0; i < MAX_EVENT_VARIABLES; i++)
  {
    gEventVariableArray[i].value = 0;
    gEventVariableArray[i].old_value = 0;
    gEventVariableArray[i].ticks = 0;
  }

  // Evaluate and cache next event index of events
  for (int i = 0; i < MAX_EVENTS; i++)
  {
    gEventExtraData[i].next_event_index = i + 1;
    if (IsStartBlockEvent(i))
    {
      gEventExtraData[i].next_event_index = FindEndMarkerForBlockEvent(i) + 1;
    }
  }

  // First pass - back up tile and special value, set up tile flags and preplaced spice/concrete
  for (int ypos = 0; ypos < gGameMap.height; ypos++)
    for (int xpos = 0; xpos < gGameMap.width; xpos++)
    {
      GameMapTileStruct *tile = &gGameMap.map[_CellNumbersWidthSpan[ypos] + xpos];
      // Back up tile and special value
      tile->back_up_tile_index = tile->__tile_index & 0x0FFF;
      tile->back_up_special_value = tile->__tile_bitflags;
      // Set up tile flags
      tile->__tile_bitflags = _TileBitflags[tile->__tile_index & 0x0FFF];
      // Setup preplaced concrete
      if (tile->__tile_index & 0x1000)
      {
        tile->__tile_bitflags |= TileFlags_800_HAS_CONCRETE | ((tile->__tile_index >> 13) & 7) << 17;
        tile->__tile_index = 651 + 20 * (rand() % 3);
      }
      // Setup preplaced spice
      else
      {
        tile->__tile_bitflags |= ((tile->__tile_index >> 13) & 7) << 20;
        tile->__tile_index &= 0x0FFF;
      }
    }

  // Second pass - process special values
  for (int ypos = 0; ypos < gGameMap.height; ypos++)
    for (int xpos = 0; xpos < gGameMap.width; xpos++)
    {
      GameMapTileStruct *tile = &gGameMap.map[_CellNumbersWidthSpan[ypos] + xpos];
      // Process special value
      int special_value = tile->back_up_special_value;
      switch ( special_value )
      {
        case 0: // MAPCMD_0:
          break;
        case 1: // MAPCMD_SPARSE_SPICE:
          // Do not hard-change tile attributes to constant value for special values 1 and 2, but only set respective spice flag
          tile->__tile_bitflags = (tile->__tile_bitflags & ~0x700000) | TileFlags_200000_SPICE;
          break;
        case 2: // MAPCMD_DENSE_SPICE:
          tile->__tile_bitflags = (tile->__tile_bitflags & ~0x700000) | TileFlags_400000_SPICE;
          break;
        case 3: // MAPCMD_SUPER_DENSE_SPICE:
          DebugFatal("Setup.cpp", "SuperDense spice found on map %d,%d", xpos, ypos);
          break;
        case 6: // Cash crate
          PlaceStaticCrateExt(xpos, ypos, CT_CASH, CI_BLUE_CRATE, 0);
          break;
        case 7: // Explosion crate
          PlaceStaticCrateExt(xpos, ypos, CT_EXPLODE, CI_BLUE_CRATE, 0);
          break;
        case 9: // Reveal map crate
          PlaceStaticCrateExt(xpos, ypos, CT_REVEAL, CI_RED_CRATE, 0);
          break;
        case 10: // Hide map crate
          PlaceStaticCrateExt(xpos, ypos, CT_NOMAP, CI_RED_CRATE, 0);
          break;
        case 12: // Random unit crate
          PlaceStaticCrateExt(xpos, ypos, CT_UNIT, CI_GREEN_CRATE, 0);
          break;
        case 13: // Stealth crate
          PlaceStaticCrateExt(xpos, ypos, CT_POWERUP, CI_PURPLE_CRATE, 0);
          break;
        case 16: // Invisible mine
          PlaceStaticCrateExt(xpos, ypos, CT_EXPLODE, CI_INVISIBLE, 0);
          break;
        case 23: // MAPCMD_PLAYER_FLAG:
          if ( (unsigned __int8)_SpawnLocationCount >= 8u )
          {
            DebugFatal("Setup.CPP", "Too many flags on map");
          }
          v8 = (unsigned __int8)_SpawnLocationCount;
          _SpawnLocations[v8].x = xpos;
          _SpawnLocations[v8].y = ypos;
          _FreeSpawnLocations[v8] = 0;
          ++_SpawnLocationCount;
          break;
        case 41: // MAPCMD_SPICE_MOUND_X5:
          PlaceCrate(xpos, ypos, rand() % 1000 + 2000, CT_SPICE_BLOOM_SPAWNER, CI_INVISIBLE, 5u);
          tile->__tile_bitflags = (tile->__tile_bitflags & ~0x700000) | TileFlags_200000_SPICE;
          break;
        case 42: // MAPCMD_SPICE_MOUND_X10:
          PlaceCrate(xpos, ypos, rand() % 1000 + 2000, CT_SPICE_BLOOM_SPAWNER, CI_INVISIBLE, 10u);
          tile->__tile_bitflags = (tile->__tile_bitflags & ~0x700000) | TileFlags_200000_SPICE;
          break;
        case 43: // MAPCMD_SPICE_MOUND_X25:
          PlaceCrate(xpos, ypos, rand() % 1000 + 2000, CT_SPICE_BLOOM_SPAWNER, CI_INVISIBLE, 25u);
          tile->__tile_bitflags = (tile->__tile_bitflags & ~0x700000) | TileFlags_200000_SPICE;
          break;
        case 44: // MAPCMD_SPICE_MOUND_X50:
          PlaceCrate(xpos, ypos, rand() % 1000 + 2000, CT_SPICE_BLOOM_SPAWNER, CI_INVISIBLE, 50u);
          tile->__tile_bitflags = (tile->__tile_bitflags & ~0x700000) | TileFlags_200000_SPICE;
          break;
        case 45: // MAPCMD_SPICE_MOUND_X255:
          PlaceCrate(xpos, ypos, rand() % 1000 + 2000, CT_SPICE_BLOOM_SPAWNER, CI_INVISIBLE, 255u);
          tile->__tile_bitflags = (tile->__tile_bitflags & ~0x700000) | TileFlags_200000_SPICE;
          break;
        case 46: // Static one-time spice bloom
          PlaceStaticCrateExt(xpos, ypos, CT_SPICE_BLOOM_LARGE, CI_LARGE_BLOOM, 0);
          break;
        default:
          // Generic crate:    1TTTTIIIEEEEEEEE
          // T = crate type  (0-15)
          // I = crate image (0-7)
          // E = extended data (8 bits)
          if (special_value & 0x8000)
          {
            int crate_type = (special_value >> 11) & 15;
            int crate_image = (special_value >> 8) & 7;
            int ext_data = special_value & 255;
            PlaceStaticCrateExt(xpos, ypos, crate_type, crate_image, ext_data);
            break;
          }
          // Generic unit:     01TCDDDSSSUUUUUU
          // T = tagged
          // C = stealth
          // D = direction (0-7)
          // S = side (0-7)
          // U = unit type (0-59)
          if (special_value & 0x4000)
          {
            int unit_type = special_value & 63;
            int side_id = (special_value >> 6) & 7;
            int direction = ((special_value >> 9) & 7) << 2;
            if (unit_type >= gUnitTypeNum)
              DebugFatal("setupmapstuff.c", "Invalid unit type %d at %d,%d (maximum is %d)", unit_type, xpos, ypos, gUnitTypeNum - 1);
            int unit_index = ModelAddUnit(side_id, unit_type, xpos, ypos, xpos, ypos, 0, 0);
            Unit *unit = GetUnit(side_id, unit_index);
            if (unit)
            {
              unit->__Facing = direction;
              unit->__FacingTurret = direction;
              unit->__FacingTurretTarget = direction;
              if (special_value & 0x1000)
                unit->Flags |= UFLAGS_10_STEALTH;
              if (special_value & 0x2000)
                unit->Tag = 1;
            }
            break;
          }
          // Generic building: 001TPHSSSBBBBBBB (non-turret)
          //                   001TDDSSSBBBBBBB (turret)
          // T = tagged
          // P = primary
          // H = no new harv.
          // D = direction (0-3)
          // S = side (0-7)
          // B = building (0-99)
          if (special_value & 0x2000)
          {
            int building_type = special_value & 127;
            int side_id = (special_value >> 7) & 7;
            bool no_new_harv = (special_value & 1024) != 0;
            bool primary = (special_value & 2048) != 0;
            int direction = ((special_value >> 10) & 3) << 3;
            bool tagged = (special_value & 4096) != 0;
            if (building_type >= gBuildingTypeNum)
              DebugFatal("setupmapstuff.c", "Invalid building type %d at %d,%d (maximum is %d)", building_type, xpos, ypos, gBuildingTypeNum - 1);
            int building_index = ModelAddBuilding(side_id, building_type, xpos, ypos, true, no_new_harv, false);
            Building *bld = GetBuilding(side_id, building_index);
            // Turret barrel direction
            if (_templates_buildattribs[building_type].__Behavior == BuildingBehavior_TURRET)
            {
              if (bld)
                bld->__Facing = direction;
            }
            // Primary building
            else if (primary)
              SetBuildingAsPrimary(side_id, building_index);
            if (tagged && bld)
              bld->Tag = 1;
            break;
          }
          // Default behavior: Tiledata entry
          object_type = _tiledata[special_value];
          if ( object_type != -1 )
          {
            object_array_xpos[objects_on_map] = xpos;
            object_array_ypos[objects_on_map] = ypos;
            object_array[objects_on_map] = object_type;
            ++objects_on_map;
          }
          break;
      }
    }

  radarmap();
  if ( _SpawnLocationCount != 8 && (gGameType == GAME_SKIRMISH || _IsMultiplayer) )
  {
    DebugFatal("Setup.cpp", "Map needs %d flags (found %d)", 8, (unsigned __int8)_SpawnLocationCount);
  }
  i = 0;
  v123.left = 0;
  v123.top = 0;
  v123.right = gGameMap.width;
  v123.bottom = gGameMap.height;
  UpdateSpiceInRegion(&v123);
  if ( _IsMultiplayer )
  {
    if ( gGameType != 1 )
    {
      v54 = gTotalPlayers;
      goto LABEL_52;
    }
  }
  else if ( gGameType != GAME_SKIRMISH )
  {
    if ( objects_on_map > 0 )
    {
      ptr_1 = object_array;
      do
      {
        v25 = *ptr_1;
        v26 = *ptr_1 >= 0;
        a1 = (unsigned __int8)((unsigned int)*ptr_1 >> 16);
        if ( v26 )
        {
          v49 = *(_BYTE *)ptr_1;
          v50 = GetSide(a1);
          v51 = CSide__MyVersionOfUnit(v50, v49, 0);
          v52 = object_array_xpos[i];
          a2 = v51;
          v53 = v51;
          ModelAddUnit(a1, v51, v52, object_array_ypos[i], v52, object_array_ypos[i], 0, 0);
          if ( _templates_unitattribs[v53].__Behavior == UnitBehavior_SANDWORM )
          {
            ++worm_count;
          }
        }
        else
        {
          v27 = (unsigned __int16)v25 - 200;
          if ( v27 )
          {
            v28 = v27 - 1;
            if ( v28 )
            {
              if ( v28 != 1 )
              {
                v29 = object_array_ypos[i];
                v30 = object_array_xpos[i];
                v31 = *(_BYTE *)ptr_1;
                v32 = GetSide(a1);
                v33 = CSide__MyVersionOfBuilding(v32, v31, 0);
                ModelAddBuilding(a1, v33, v30, v29, 1, 0, 0);
              }
            }
          }
        }
        ++i;
        ++ptr_1;
      }
      while ( i < objects_on_map );
    }
    // New logic start
    // Remove the check for mandatory worms presence on map
    /* if ( !worm_count )
    {
     DebugFatal("Setup.cpp", "No worms found on this map (%s)", _MapName);
    } */
    // New logic end
    return;
  }
  v54 = 1;
  gTotalPlayers = 1;
LABEL_52:
  v55 = &_gAIArray[0].__IsAI;
  v56 = 8;
  do
  {
    *v55 = 0;
    v55 += 7608;
    --v56;
  }
  while ( v56 );
  if ( v54 <= 1u )
  {
    v54 = 1;
  }
  obja = v54;
  v57 = 1;
  v58 = (int)&_gAIArray[1];
  do
  {
    v59 = (CAI_ *)v58;
    v58 += 7608;
    memcpy(v59, _gAIArray, sizeof(CAI_));
    *(_BYTE *)(v58 - 7608) = v57++;
  }
  while ( (unsigned __int8)v57 < 8u );
  total_players = gNetAIPlayers;
  v61 = v54;
  side = v54;
  if ( v54 < v54 + (unsigned __int8)gNetAIPlayers )
  {
    v62 = side;
    v63 = v54;
    do
    {
      _IRValues[v63] = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\Setup.cpp", 5640) % 3u;
      GetSide(v62)->fHouseID = _IRValues[v63];
      v64 = v63;
      _gAIArray[v64].AISide = v61++;
      side = v61;
      _gAIArray[v64].__IsAI = 1;
      v62 = side;
      total_players = gNetAIPlayers;
      v63 = side;
    }
    while ( side < obja + (unsigned __int8)gNetAIPlayers );
  }
  mob_const_vehicle = -1;
  v66 = 0;
  *(_DWORD *)&a2 = -1;
  v67 = &_templates_unitattribs[0].__Behavior;
  v68 = 60;
  do
  {
    if ( *v67 == UnitBehavior_MCV )
    {
      mob_const_vehicle = v66;
    }
    ++v66;
    v67 += 256;
    --v68;
  }
  while ( v68 );
  *(_DWORD *)&a2 = mob_const_vehicle;
  if ( gTotalPlayers <= 1u )
  {
    player_count = 1;
  }
  else
  {
    player_count = gTotalPlayers;
  }
  total_players_2 = player_count + total_players;
  total_players_22 = player_count + total_players;
  if ( mob_const_vehicle == -1 )
  {
    DebugFatal("Setup.cpp", "There is no MCV!");
    return;
  }
  if ( total_players_2 > 8u )
  {
    DebugFatal("SETUP.CPP", "totalPlayers has silly value (%d, %d [AI])", gTotalPlayers, total_players);
  }
  initial_spawn_fetch = 0;
  side = 0;
  if ( total_players_2 )
  {
    while ( 1 )
    {
      if ( initial_spawn_fetch )
      {
        v120 = 0;
        objb = 0;
        if ( _SpawnLocationCount )
        {
          v107 = _FreeSpawnLocations;
          v72 = &_SpawnLocations[0].y;
          do
          {
            if ( !*v107 )
            {
              v73 = 0xFFFFF;
              v74 = 0;
              yposa = _FreeSpawnLocations;
              v75 = &_SpawnLocations[0].y;
              do
              {
                if ( objb != v74 && *yposa == 1 )
                {
                  v76 = *(v72 - 1) - *(v75 - 1);
                  v119 = (*v72 - *v75) * (*v72 - *v75) + v76 * v76;
                  v77 = (signed __int64)sqrt((double)v119);
                  if ( (unsigned int)v77 < v73 )
                  {
                    v73 = v77;
                  }
                }
                ++v74;
                v75 += 2;
                ++yposa;
              }
              while ( v74 < (unsigned __int8)_SpawnLocationCount );
              if ( v73 > v120 )
              {
                v120 = v73;
                spawn_location = objb;
              }
            }
            v72 += 2;
            v78 = (unsigned __int8)(objb++ + 1) < (unsigned __int8)_SpawnLocationCount;
            ++v107;
          }
          while ( v78 );
          mob_const_vehicle = *(_DWORD *)&a2;
        }
      }
      else
      {
        spawn_location = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\Setup.cpp", 5672)
                       % (unsigned int)(unsigned __int8)_SpawnLocationCount;
      }
      v79 = LOBYTE(_SpawnLocations[spawn_location].x);
      yposb = LOBYTE(_SpawnLocations[spawn_location].y);
      y = _SpawnLocations[spawn_location].y;
      _FreeSpawnLocations[spawn_location] = 1;
      v119 = v79;
      x = v79;
      GetNearestFreeTileForUnit(&x, &y, 6u);
      v80 = side;
      v81 = y;
      v82 = x;
      v83 = y;
      v84 = x;
      v85 = _templates_unitattribs[mob_const_vehicle].__UnitType;
      v86 = GetSide((eSideType)side);
      v87 = CSide__MyVersionOfUnit(v86, v85, 0);
      if (StartWithMCV) // mod
        ModelAddUnit(v80, v87, v84, v83, v82, v81, 0, 0);
      v117 = 0;
      if ( gNetUnitCount )
      {
        break;
      }
LABEL_107:
      v93 = 0;
      if ( objects_on_map )
      {
        v94 = object_array;
        do
        {
          v26 = *v94 >= 0;
          a1 = (unsigned __int8)((unsigned int)*v94 >> 16);
          if ( v26 )
          {
            v95 = a1;
            v96 = *(_BYTE *)v94;
            v97 = GetSide(a1);
            v121 = CSide__MyVersionOfUnit(v97, v96, 0);
            if ( _templates_unitattribs[v121].__Behavior == UnitBehavior_SANDWORM )
            {
              ++worm_count;
              if ( (!_IsMultiplayer && gGameType != 1) || worm_count <= gNetWorms )
              {
                v98 = object_array_xpos[v93];
                y = object_array_ypos[v93];
                x = v98;
                ModelAddUnit(v95, v121, x, y, x, y, 0, 0);
              }
            }
          }
          ++v93;
          ++v94;
        }
        while ( v93 < objects_on_map );
      }
      initial_spawn_fetch = side++ + 1;
      if ( side >= total_players_22 )
      {
        return;
      }
    }
    while ( 1 )
    {
      v88 = v117 + 10 * gNetTechLevel;
      v89 = _TechPosdata[0][v88].UnitType_Atreides;
      if ( v89 != -1 )
      {
        break;
      }
LABEL_106:
      if ( ++v117 >= (unsigned __int8)gNetUnitCount )
      {
        goto LABEL_107;
      }
    }
    v90 = v119 + _TechPosdata[0][v88].PosX;
    v91 = yposb + _TechPosdata[0][v88].PosY;
    v108 = yposb + _TechPosdata[0][v88].PosY;
    if ( v90 < 0 )
    {
      v90 = 0;
    }
    if ( v91 < 0 )
    {
      v91 = 0;
      v108 = 0;
    }
    if ( v90 > gGameMap.width - 1 )
    {
      v90 = LOBYTE(gGameMap.width) - 1;
    }
    if ( v91 > gGameMap.height - 1 )
    {
      v108 = LOBYTE(gGameMap.height) - 1;
    }
    v122 = _IRValues[(unsigned __int8)v80];
    if ( v122 )
    {
      if ( v122 == 1 )
      {
        v99 = _TechPosdata[0][v88].UnitType_Harkonnen;
LABEL_104:
        v92 = GetSide(v80);
        type = CSide__MyVersionOfUnit(v92, v99, 0);
        goto LABEL_105;
      }
      if ( v122 != 2 )
      {
        DebugFatal("SETUP.CPP", "Illegal side!");
LABEL_105:
        x = v90;
        y = v108;
        GetNearestFreeTileForUnit(&x, &y, 6u);
        ModelAddUnit(v80, type, x, y, x, y, 0, 0);
        goto LABEL_106;
      }
      v89 = _TechPosdata[0][v88].UnitType_Ordos;
    }
    v99 = v89;
    goto LABEL_104;
  }
}
