#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"
#include "utils.h"
#include "event-utils.h"
#include "event-actions.h"
#include "event-core.h"

void EvAct_AddDelivery(int xpos, int ypos, int side_id, int amount, int deploy_action, eDeliveryType delivery_type, char *unit_list)
{
  CSide *side = GetSide(side_id);
  if (delivery_type == DELIVERYTYPE_STARPORT && side->__PrimaryStarport == -1)
    return;
  int found_free_slot = 0;
  for (int i = 0; i <= 10; i++)
  {
    if (i == 10)
    {
      DebugFatal("event-actions.c", "Too many deliveries");
    }
    if (!side->__Deliveries[i].__is_active)
    {
      found_free_slot = i;
      break;
    }
  }
  side->__Deliveries[found_free_slot].__is_active = 1;
  side->__Deliveries[found_free_slot].c_field_2 = 0;
  side->__Deliveries[found_free_slot].__xpos = xpos;
  side->__Deliveries[found_free_slot].__ypos = ypos;
  side->__Deliveries[found_free_slot].__delivery_time = gGameTicks;
  side->__Deliveries[found_free_slot].__deploy_action = deploy_action;
  side->__Deliveries[found_free_slot].__delivery_type = delivery_type;
  memcpy(side->__Deliveries[found_free_slot].__units, unit_list, amount);
  side->__Deliveries[found_free_slot].__units[amount] = -1;
}

void EvAct_SetDiplomacy(int source_side, int target_side, int allegiance_type, bool both_sided)
{
  _gDiplomacy[source_side][target_side] = allegiance_type;
  if ( allegiance_type == 0 || allegiance_type == 2 )
  {
    CSide__ResetEnemyForSide(GetSide(source_side), target_side);
  }
  // Double-sided
  if (both_sided)
  {
    _gDiplomacy[target_side][source_side] = allegiance_type;
    if ( allegiance_type == 0 || allegiance_type == 2 )
    {
      CSide__ResetEnemyForSide(GetSide(target_side), source_side);
    }
  }
}

void EvAct_PlaySound(int sample_id, bool point_sound, int xpos, int ypos)
{
  if (point_sound)
    PlaySoundAt(sample_id, xpos, ypos);
  else
    Sound__PlaySample(sample_id, 0, 0, 0);
}

void EvAct_SetCash(int side_id, eValueOperation operation, int value)
{
  CSide *side = GetSide(side_id);
  int actual_cash = side->SpiceReal + side->SpiceDrip + side->CashReal + side->CashDrip;
  int target_cash = ValueOperation(actual_cash, value, operation);
  int cost = actual_cash - target_cash;

  cost = HLIMIT(cost, actual_cash);
  if (cost <= 0)
  {
    side->CashDrip -= cost;
  }
  else if ( side->SpiceDrip + side->SpiceReal <= cost )
  {
    side->CashDrip -= cost - (side->SpiceReal + side->SpiceDrip);
    side->SpiceDrip = -side->SpiceReal;
  }
  else
  {
    side->SpiceDrip -= cost;
  }
}

void EvAct_SetTech(int side_id, eValueOperation operation, bool immediate_update, int value)
{
  _gMiscData.Tech[side_id] = ValueOperation(_gMiscData.Tech[side_id], value, operation);
  // Immediately update available buildings and units
  if (immediate_update)
  {
    CSide__UpdateBuildingAndUnitIconsAndBaseBoundaries(GetSide(side_id));
  }
}

void EvAct_SwitchMySide(int side_id, int ai_switch, bool reveal_base)
{
  if (ai_switch & 2)
    _gAIArray[gSideId].__IsAI = 1;
  gSideId = side_id;
  if (reveal_base)
    RevealTilesSeenByBuildingsAndUnits(gSideId);
  if (ai_switch & 1)
    _gAIArray[gSideId].__IsAI = 0;
}

void EvAct_HideMap()
{
  ClearTImage(_RadarMap1, 0, 0);
  if ( GetMapVisState() != _mapvisstate_548010 )
  {
    _cheatstates[gSideId] |= 1u;
  }
  for (int y = 0; y < gGameMap.height; y++)
  {
    for (int x = 0; x < gGameMap.width; x++)
    {
      gGameMap.map[x + _CellNumbersWidthSpan[y]].__shroud_flags = 1;
    }
  }
  _mapvisstate_548010 = GetMapVisState();
  RevealTilesSeenByBuildingsAndUnits(gSideId);
}

void EvAct_RevealMap(int xpos, int ypos, int radius)
{
  if (radius == 0)
  {
    RevealMap();
    BlitClipTImage1(_RadarMap1, 0, 0, _RadarMap2, 0, 0, 0);
  }
  else
    RevealCircle(xpos, ypos, radius);
}

void EvAct_ShowMessage(int duration, ShowMessageEventData *data)
{
  // Play message sound
  int sample_id = Data__GetSoundTableID("S_CHATMSG");
  Sound__PlaySample(sample_id, 0, 0, 0);
  // Remember the current free message slot
  int message_slot = _gMessageData.__slot;
  // Attempt to get custom text from mission ini file
  char mapIniPath[256];
  char id[12];
  char custom_text[512];
  sprintf(mapIniPath, ".\\%s%s", GameType == GT_SINGLEPLAYER ? MissionsResourcePath : MapsResourcePath, PathChangeExtension(MissionMap, ".ini"));
  sprintf(id, "%d", data->message_index);
  IniGetString("Text", id, "", custom_text, 512, mapIniPath);
  if (strlen(custom_text) > 0)
    QueueMessage(custom_text, -1);
  else
    // Get text from string table
    QueueMessage(Data__GetTextString(data->message_index, 1), -1);
  // Set customized message duration
  if (duration)
    _gMessageData.__ticks[message_slot] = gGameTicks + duration;
}

void EvAct_UnitSpawn(int xpos, int ypos, int side_id, int amount, int facing, int tag, char *unit_list)
{
  for (int i = 0; i < amount; i++)
  {
    unsigned char x = xpos;
    unsigned char y = ypos;
    if (_templates_unitattribs[(int)unit_list[i]].__Behavior != UnitBehavior_SANDWORM)
      GetNearestFreeTileForUnit(&x, &y, 12u);
    int unit_index = ModelAddUnit(side_id, unit_list[i], x, y, x, y, 0, 0);
    Unit *unit = GetUnit(side_id, unit_index);
    if (unit)
    {
      unit->__Facing = facing << 2;
      unit->__FacingTurret = facing << 2;
      unit->__FacingTurretTarget = facing << 2;
      unit->Tag = tag;
    }
  }
}

void EvAct_UnBlockEvent(int operation, int event_index)
{
  if (operation)
    _gEventArray[event_index].event_flags |= EVENTFLAG_BLOCKED;
  else
    _gEventArray[event_index].event_flags &= ~EVENTFLAG_BLOCKED;
}

void EvAct_PlayMusic(char *name)
{
  //abusing the units array to pass the file name
  if (strcmp(name, "*") == 0) PlayRandomMusic();
  else if (strlen(name) > 0) Sound__PlayMusic(name);
}

void random_spread(int *x, int *y, int spread_x, int spread_y, bool circle_spread)
{
  if (circle_spread)
  {
    int angle = rand() % 32;
    if (spread_x)
      *x += _sinValues[(rand() % spread_x) + angle * 512] / 2048;
    if (spread_y)
      *y += _cosValues[(rand() % spread_y) + angle * 512] / 2048;
  }
  else
  {
    if (spread_x)
      *x += rand() % (spread_x * 2 + 1) - spread_x;
    if (spread_y)
      *y += rand() % (spread_y * 2 + 1) - spread_y;
  }
}

void EvAct_DamageTiles(int xpos, int ypos, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int weapon_type, bool circle_spread, bool hit_explosion)
{
  int pixel_pos_x = xpos * 32 + pixel_x;
  int pixel_pos_y = ypos * 32 + pixel_y;
  random_spread(&pixel_pos_x, &pixel_pos_y, spread_x, spread_y, circle_spread);

  // Do not do damage outside of map
  if (pixel_pos_x < 0 || pixel_pos_x >= gGameMapWidth * 32 || pixel_pos_y < 0 || pixel_pos_y >= gGameMapHeight * 32)
    return;
  // Do actual damage
  DamageTiles(pixel_pos_x, pixel_pos_y, 0, weapon_type, side_id, 0xFFFF, 0);
  if (hit_explosion && _templates_bulletattribs[weapon_type].__HitExplosion != -1)
  {
    ModelAddExplosion(side_id, _templates_bulletattribs[weapon_type].__HitExplosion, pixel_pos_x, pixel_pos_y, 0, 0, 0, 0, 0);
    PlaySoundAt(_templates_explosionattribs[(uint8_t)_templates_bulletattribs[weapon_type].__HitExplosion].__Sound, xpos, ypos);
  }
}

int EvAct_AddUnit(int xpos, int ypos, int side_id, int properties, int unit_type, int movement, int facing, int tag)
{
  GameMapTileStruct *tile = &gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]];
  // Do not add unit if tile is already occupied by unit
  if (_templates_unitattribs[unit_type].__Behavior != UnitBehavior_SANDWORM && tile->__tile_bitflags & TileFlags_8_OCC_UNIT)
    return -1;
  // Do not add next infantry if all 5 slots are already occupied
  if (_templates_unitattribs[unit_type].__IsInfantry && ((tile->__tile_bitflags & 0x3E0) == 0x3E0))
    return -1;
  // If tile is occupied by building belonging to different side, change owner side flags
  bool occ_building = (tile->__tile_bitflags & TileFlags_10_OCC_BUILDING) != 0;
  int orig_owner_side = tile->__tile_bitflags & 7;
  if (occ_building && (orig_owner_side != side_id))
    tile->__tile_bitflags = (tile->__tile_bitflags & ~7) | side_id;
  // Determine target spot to move unit onto
  unsigned char target_x = xpos;
  unsigned char target_y = ypos;
  switch (movement)
  {
    case 1: GetRandomAdjacentTile(&target_x, &target_y); break;
    case 2: target_x--; target_y--; break;
    case 3: target_y--; break;
    case 4: target_x++; target_y--; break;
    case 5: target_x++; break;
    case 6: target_x++; target_y++; break;
    case 7: target_y++; break;
    case 8: target_x--; target_y++; break;
    case 9: target_x--; break;
  }
  // Add unit
  int unit_index = ModelAddUnit(side_id, unit_type, xpos, ypos, target_x, target_y, 0, 0);
  // Set unit properties
  Unit *unit = GetUnit(side_id, unit_index);
  if (unit)
  {
    if (!movement)
    {
      unit->__Facing = facing << 2;
      unit->__FacingTurret = facing << 2;
      unit->__FacingTurretTarget = facing << 2;
    }
    if (properties & 1)
      unit->Flags |= UFLAGS_10_STEALTH;
    unit->Tag = tag;
  }
  // Revert back owner side attributes
  if (occ_building && (orig_owner_side != side_id))
    tile->__tile_bitflags = (tile->__tile_bitflags & ~7) | orig_owner_side;
  return unit_index;
}

int EvAct_AddBuilding(int xpos, int ypos, int side_id, int properties, int building_type, int method, int facing, int tag)
{
  bool initialsetup = false;
  bool captured = false;
  bool place_concrete = false;
  //0: Built without concrete:         0 0 0
  //1: Built with concrete:            0 0 1
  //2: Instant with concrete:          1 0 0
  //3: Built without concrete no harv: 0 1 0
  //4: Built with concrete no harv:    0 1 1
  //5: Instant with concrete no harv:  1 1 0
  switch (method)
  {
    case 1: place_concrete = true; break;
    case 2: initialsetup = true; break;
    case 3: captured = true; break;
    case 4: captured = true; place_concrete = true; break;
    case 5: initialsetup = true; captured = true; break;
  }
  if (place_concrete)
    ModelAddConcrete(side_id, CSide__MyVersionOfBuilding(GetSide(side_id), _templates_GroupIDs.Concrete1, 0), xpos, ypos, 0, _templates_buildattribs[building_type]._____TilesOccupiedAll);
  int building_index = ModelAddBuilding(side_id, building_type, xpos, ypos, initialsetup, captured, captured);
  // Set building properties
  Building *bld = GetBuilding(side_id, building_index);
  if (bld)
  {
    // Turret barrel direction
    if (_templates_buildattribs[building_type].__Behavior == BuildingBehavior_TURRET)
        bld->__Facing = facing << 2;
    // Primary building
    if (properties & 1)
      SetBuildingAsPrimary(side_id, building_index);
    // Building tag
    bld->Tag = tag;
  }
  return building_index;
}

void EvAct_AddProjectile(int src_x, int src_y, int targ_x, int targ_y, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int weapon_type, bool circle_spread, bool play_sound)
{
  int pixel_pos_x = targ_x * 32 + pixel_x;
  int pixel_pos_y = targ_y * 32 + pixel_y;
  random_spread(&pixel_pos_x, &pixel_pos_y, spread_x, spread_y, circle_spread);

  ModelAddBullet(side_id, weapon_type, 0, -1, src_x*32+16, src_y*32+16, pixel_pos_x, pixel_pos_y, -1, -1);
  if (play_sound)
    PlaySoundAt(_templates_bulletattribs[weapon_type].__FiringSound, src_x, src_y);
}

void EvAct_AddExplosion(int xpos, int ypos, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int explosion_type, bool circle_spread, bool play_sound)
{
  int pixel_pos_x = xpos * 32 + pixel_x;
  int pixel_pos_y = ypos * 32 + pixel_y;
  random_spread(&pixel_pos_x, &pixel_pos_y, spread_x, spread_y, circle_spread);

  ModelAddExplosion(side_id, explosion_type, pixel_pos_x, pixel_pos_y, 0, 0, 0, 0, 0);
  if (play_sound)
  {
    PlaySoundAt(_templates_explosionattribs[explosion_type].__Sound, xpos, ypos);
  }
}

int EvAct_AddCrate(int xpos, int ypos, int crate_type, int image, int ext_data, int respawns, int expiration)
{
  // There is already crate
  if (gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]].__tile_bitflags & TileFlags_1000_HAS_CRATE)
    return -1;
  // Add the crate
  int index = GetFreeCrateIndex();
  if ( index != -1 )
  {
    gCrates[index].__is_active = 1;
    gCrates[index].__x = xpos;
    gCrates[index].__y = ypos;
    gCrates[index].__type = crate_type;
    gCrates[index].__image = image;
    gCrates[index].__timing = (expiration?expiration:INT32_MAX);
    gCrates[index].__times_to_respawn = respawns;
    gCrates[index].ext_data_field = ext_data;
    gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]].__tile_bitflags |= TileFlags_1000_HAS_CRATE;
  }
  return index;
}

void EvAct_AddConcrete(int xpos, int ypos, int width, int height, int side_id, int tilebitmask)
{
  if (tilebitmask)
    ModelAddConcrete(side_id, CSide__MyVersionOfBuilding(GetSide(side_id), _templates_GroupIDs.Concrete1, 0), xpos, ypos, 0, tilebitmask);
  else
  {
    for (int y = 0; y < height; y++)
      for (int x = 0; x < width; x++)
      {
        GameMapTileStruct *tile = &gGameMap.map[xpos + x + _CellNumbersWidthSpan[ypos + y]];
        if ((tile->__tile_bitflags & TileFlags_8000_BUILD_ON) && !(tile->__tile_bitflags & TileFlags_800_HAS_CONCRETE))
        {
          tile->__tile_bitflags &= ~0xE0000;
          tile->__tile_bitflags |= (TileFlags_800_HAS_CONCRETE | (side_id << 17));
          tile->__tile_index = 651 + 20 * (rand() % 3);
        }
      }
  }
}

bool NotSpiceOn(int x, int y)
{
  x = LIMIT(x, 0, gGameMapWidth - 1);
  y = LIMIT(y, 0, gGameMapHeight - 1);
  return (gGameMap.map[x + _CellNumbersWidthSpan[y]].__tile_bitflags & (TileFlags_100000_SPICE | TileFlags_200000_SPICE | TileFlags_400000_SPICE)) == 0;
}

void EvAct_SpiceBloom(int xpos, int ypos, int range, eSpiceBloomMode mode, bool randomizer)
{
  // Extended behavior: instant spice blooms
  if (mode != SPICEBLOOM_CLASSIC)
  {
    // Get the circle
    char *circle = NULL;
    if (mode == SPICEBLOOM_CIRCLE || mode == SPICEBLOOM_DUNE2)
    {
      range = HLIMIT(range, 7);
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
        if ((tile->__tile_bitflags & (TileFlags_10000_SANDY | TileFlags_10_OCC_BUILDING | TileFlags_800_HAS_CONCRETE)) != TileFlags_10000_SANDY)
          continue;
        // Use randomizer
        if (randomizer && !(xx == xpos && yy == ypos) && (rand() % 100) < 40)
          continue;
        // Update pixels on radar
        if ( gBitsPerPixel == 16 )
          SetPixelOnRadar16(xx, yy, _radarcolor_word_517898_spicecolor);
        else
          SetPixelOnRadar8(xx, yy, _radarcolor_byte_517780_spicecolor);
        // Add two pieces of spice
        int spice_amount = (tile->__tile_bitflags >> 20) & 7;
        int spice_limit = (mode == SPICEBLOOM_DUNE2)?4:2;
        spice_amount = (spice_amount < spice_limit)?HLIMIT(spice_amount + 2, spice_limit):spice_amount;
        tile->__tile_bitflags &= ~(TileFlags_100000_SPICE | TileFlags_200000_SPICE | TileFlags_400000_SPICE);
        tile->__tile_bitflags |= (spice_amount << 20);
      }
    }
    // Dune 2 style spice bloom
    if (mode == SPICEBLOOM_DUNE2)
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
          if ((tile->__tile_bitflags & (TileFlags_10000_SANDY | TileFlags_10_OCC_BUILDING | TileFlags_800_HAS_CONCRETE)) != TileFlags_10000_SANDY)
            continue;
          // Check spice amount and surrounding tiles
          int spice_amount = (tile->__tile_bitflags >> 20) & 7;
          if (spice_amount > 2 &&
            (  NotSpiceOn(xx - 1, yy - 1)
            || NotSpiceOn(xx, yy - 1)
            || NotSpiceOn(xx + 1, yy - 1)
            || NotSpiceOn(xx - 1, yy)
            || NotSpiceOn(xx + 1, yy)
            || NotSpiceOn(xx - 1, yy + 1)
            || NotSpiceOn(xx, yy + 1)
            || NotSpiceOn(xx + 1, yy + 1)
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
    r.left = LLIMIT(xpos - range - 1, 0);
    r.top = LLIMIT(ypos - range - 1, 0);
    r.right = HLIMIT(xpos + range + 2, gGameMapWidth);
    r.bottom = HLIMIT(ypos + range + 2, gGameMapHeight);
    UpdateSpiceInRegion(&r);
  }
  // Standard behavior: original spice bloom
  else
  {
    SpiceMound(xpos, ypos, range);
  }
}

void EvAct_CenterViewport(int xpos, int ypos)
{
  _ViewportXPos = LIMIT(xpos * 32 - (_ViewportWidth / 2) + 16, 0, gGameMapWidth * 32 - _ViewportWidth);
  _ViewportYPos = LIMIT(ypos * 32 - (_ViewportHeight / 2) + 16, 0, gGameMapHeight * 32 - _ViewportHeight);
}

void ChangeMapTile(int xpos, int ypos, int new_tile_index)
{
  if (new_tile_index == 65535)
    return;
  GameMapTileStruct *tile = &gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]];
  uint16_t old_tile_index = tile->back_up_tile_index;
  // Determine properties of new tile
  bool no_drive_on =  (_TileBitflags[new_tile_index] & TileFlags_2000_DRIVE_ON) == 0;
  bool no_walk_on =   (_TileBitflags[new_tile_index] & TileFlags_4000_WALK_ON) == 0;
  bool no_build_on =  (_TileBitflags[new_tile_index] & TileFlags_8000_BUILD_ON) == 0;
  bool occ_unit =     (_TileBitflags[new_tile_index] & TileFlags_8_OCC_UNIT) != 0;
  bool occ_building = (_TileBitflags[new_tile_index] & TileFlags_10_OCC_BUILDING) != 0;
  bool occ_infantry = (_TileBitflags[new_tile_index] & 0x3E0) != 0;
  // Determine actions
  bool destroy_buildings = no_build_on || occ_building || occ_unit || occ_infantry;
  bool kill_infantry =     no_walk_on  || occ_building || occ_unit || occ_infantry;
  bool kill_light_veh =    no_drive_on || occ_building || occ_unit || occ_infantry;
  bool kill_heavy_veh =    no_drive_on || occ_building || occ_unit;
  // Destroy buildings on the tile
  eSideType side_id = 0;
  _WORD index;
  Building *bld;
  bool real_building_on_tile = false;
  if ((tile->__tile_bitflags & TileFlags_10_OCC_BUILDING) && GetBuildingOnTile_0(xpos, ypos, &bld, &side_id, &index))
  {
    if (destroy_buildings && !(_templates_buildattribs[bld->Type]._____Flags & BFLAGS_400000_NO_CONCRETE))
      DestroyBuilding(side_id, index, 0);
    else
      real_building_on_tile = true;
  }
  // Destroy units on tile
  Unit *unit;
  bool real_unit_on_tile = false;
  if ((tile->__tile_bitflags & 0x3E8) && (GetUnitOnTile(32 * xpos, 32 * ypos, &side_id, &index, false)))
  {
    index = -1;
    while ((unit = GetNextUnitOnTile(xpos, ypos, side_id, &index)))
    {
      bool is_infantry = _templates_unitattribs[unit->Type].__IsInfantry;
      bool is_heavy_veh = _templates_unitattribs[unit->Type].__CanCrush;
      if ((is_infantry && kill_infantry) || (!is_infantry && !is_heavy_veh && kill_light_veh) || (!is_infantry && is_heavy_veh && kill_heavy_veh))
        DestroyUnit(side_id, index);
      else
        real_unit_on_tile = true;
    }
  }
  // Remove concrete if new tile is not buildable
  if (no_build_on && (tile->__tile_bitflags & TileFlags_800_HAS_CONCRETE))
  {
    tile->__tile_index = tile->back_up_tile_index;
    tile->__tile_bitflags &= ~(TileFlags_800_HAS_CONCRETE);
  }
  // Remove spice if the new tile is not sandy
  if (!(_TileBitflags[new_tile_index] & TileFlags_10000_SANDY) && (tile->__tile_bitflags & 0x700000))
  {
    tile->__tile_index = tile->back_up_tile_index;
    tile->__tile_bitflags &= ~0x700000;
  }
  // Change tile index
  if (tile->__tile_index == tile->back_up_tile_index)
  {
    tile->__tile_index = new_tile_index;
  }
  tile->back_up_tile_index = new_tile_index;
  // Change attributes
  tile->__tile_bitflags &= ~_TileBitflags[old_tile_index];
  tile->__tile_bitflags |= _TileBitflags[new_tile_index];
  // Restore building or unit owner attributes
  if (real_building_on_tile || real_unit_on_tile)
    tile->__tile_bitflags = (tile->__tile_bitflags & ~7) | side_id;
  // Reset tile damage
  tile->__damage = 0;
};

void EvAct_ChangeMapBlock(int xpos, int ypos, int width, int height, uint16_t *tiles)
{
  int tile_count = height * width;
  if (tile_count > 12)
    DebugFatal("event-actions.c", "Max allowed tiles in Change Tiles event is 12, actual is %d", tile_count);
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      ChangeMapTile(x + xpos, y + ypos, tiles[y * width + x]);
  // Update spice visuals
  RECT r;
  r.left = LLIMIT(xpos - 1, 0);
  r.top = LLIMIT(ypos - 1, 0);
  r.right = HLIMIT(xpos + width + 1, gGameMapWidth);
  r.bottom = HLIMIT(ypos + height + 1, gGameMapHeight);
  UpdateSpiceInRegion(&r);
}

void EvAct_TransformTiles(int amount, uint16_t *tiles)
{
  if (amount > 6)
    DebugFatal("event-actions.c", "Max allowed tile pairs in Transform Tiles event is 6, actual is %d", amount);
  for (int y = 0; y < gGameMapHeight; y++)
    for (int x = 0; x < gGameMapWidth; x++)
      for (int i = 0; i < amount; i++)
        if (gGameMap.map[x + _CellNumbersWidthSpan[y]].back_up_tile_index == tiles[i * 2])
        {
          ChangeMapTile(x, y, tiles[i * 2 + 1]);
          break;
        }
}

void EvAct_AddBuildingDestruct(int xpos, int ypos, int side_id, int building_type)
{
  int old_screen_shakes = _ScreenShakes;
  int building_index = ModelAddBuilding(side_id, building_type, xpos, ypos, 0, 1, 1);
  DestroyBuilding(side_id, building_index, 0);
  CSide *side = GetSide(side_id);
  side->__BuildingsBuilt--;
  side->__BuildingsBuiltPerType[building_type]--;
  Building *bld = GetBuilding(side_id, building_index);
  bld->__DeadStateTimeCounter = 1;
  _ScreenShakes = old_screen_shakes;
}

void EvAct_ActivateTimer(int condition_index)
{
  ConditionData *condition = &_gConditionArray[condition_index];
  if (!(condition->condition_type == CT_INTERVAL || condition->condition_type == CT_TIMER || condition->condition_type == CT_RANDOMINTERVAL))
    DebugFatal("event-actions.c", "Activate Timer: The target condition (ID %d) must be Timer, Interval or Random Interval type.", condition_index);
  // Set Inactive to false
  condition->arg1 = 0;
  // Set Base time to current time
  condition->val4 = gGameTicks;
}

void EvAct_TransferCredits(int side_id, eTransferCreditsOperation operation, int value)
{
  CSide *side = GetSide(side_id);
  int remaining_transfer = 0;
  switch (operation)
  {
  case TRANSFERCREDITS_ALL_TO_CASH: remaining_transfer = side->SpiceDrip + side->SpiceReal; break;
  case TRANSFERCREDITS_ALL_TO_SPICE_STORAGE: remaining_transfer = side->__MaxStorage - (side->SpiceDrip + side->SpiceReal); break;
  case TRANSFERCREDITS_ALL_TO_SPICE_FORCE: remaining_transfer = side->CashDrip + side->CashReal; break;
  case TRANSFERCREDITS_VALUE_TO_CASH: remaining_transfer = value; break;
  case TRANSFERCREDITS_VALUE_TO_SPICE_STORAGE: remaining_transfer = HLIMIT(side->__MaxStorage - (side->SpiceDrip + side->SpiceReal), value); break;
  case TRANSFERCREDITS_VALUE_TO_SPICE_FORCE: remaining_transfer = value; break;
  }

  switch (operation)
  {
    case TRANSFERCREDITS_ALL_TO_CASH:
    case TRANSFERCREDITS_VALUE_TO_CASH:
    {
      int transfer_spice_drip = HLIMIT(side->SpiceDrip, remaining_transfer);
      remaining_transfer -= transfer_spice_drip;
      side->CashDrip += transfer_spice_drip;
      side->SpiceDrip -= transfer_spice_drip;
      int transfer_spice_real = HLIMIT(side->SpiceReal, remaining_transfer);
      side->CashReal += transfer_spice_real;
      side->SpiceReal -= transfer_spice_real;
      break;
    }
    case TRANSFERCREDITS_ALL_TO_SPICE_STORAGE:
    case TRANSFERCREDITS_ALL_TO_SPICE_FORCE:
    case TRANSFERCREDITS_VALUE_TO_SPICE_STORAGE:
    case TRANSFERCREDITS_VALUE_TO_SPICE_FORCE:
    {
      int transfer_cash_drip = LIMIT(side->CashDrip, 0, remaining_transfer);
      remaining_transfer -= transfer_cash_drip;
      side->SpiceDrip += transfer_cash_drip;
      side->CashDrip -= transfer_cash_drip;
      int transfer_cash_real = HLIMIT(side->CashReal, remaining_transfer);
      side->SpiceReal += transfer_cash_real;
      side->SpiceReal = HLIMIT(side->SpiceReal, side->__MaxStorage);
      side->CashReal -= transfer_cash_real;
      break;
    }
  }
}

void EvAct_SetBuildingUpgrades(int side_id, int building_group, eValueOperation operation, int value)
{
  CSide *side = GetSide(side_id);
  int old_upgrades = side->__BuildingGroupUpgradeCount[building_group];
  int new_upgrades = LLIMIT(ValueOperation(old_upgrades, value, operation), 0);
  side->__BuildingGroupUpgradeCount[building_group] = new_upgrades;
  CSide__UpdateBuildingAndUnitIconsAndBaseBoundaries(side);
  if ((old_upgrades < new_upgrades) && (side->__BuildingUpgradeQueue.__type == CSide__MyVersionOfBuilding(side, building_group, 0)))
  {
    side->__BuildingUpgradeQueue.__on_hold = 1;
    GenerateUpgradeCancelOrder(side_id, side->__BuildingUpgradeQueue.__type);
  }
}

void EvAct_SetStarportCost(int side_id, int unit_type, eValueOperation operation, bool default_cost, int value)
{
  CSide *side = GetSide(side_id);
  if (default_cost)
    side->__StarportUnitTypeCost[unit_type] = _templates_unitattribs[unit_type].__Cost;
  else
    side->__StarportUnitTypeCost[unit_type] = LLIMIT(ValueOperation(side->__StarportUnitTypeCost[unit_type], value, operation), 0);
}

void EvAct_ShowSideData(int side_id, int offset)
{
  CSide *side = GetSide(side_id);
  char header[128];
  memset(header, 0, sizeof(header));
  sprintf(header, "Side %d Cash %d %d Spice %d %d Storage %d PowerOutput %d PowerDrain %d", side_id, side->CashDrip, side->CashReal, side->SpiceDrip, side->SpiceReal, side->__MaxStorage, side->__PowerOutput, side->__PowerDrained);
  ShowDataOnScreen(header, ((unsigned char *)side) + offset);
}

void EvAct_ShowAIData(int side_id, int offset)
{
  CAI_ *ai = &_gAIArray[side_id];
  char header[128];
  memset(header, 0, sizeof(header));
  sprintf(header, "AI %d memory dump at offset %X", side_id, offset);
  ShowDataOnScreen(header, ((unsigned char *)ai) + offset);
}

void EvAct_SetMemoryData(int bytes, int value, int address)
{
  if (!address)
    DebugFatal("event-actions.c", "Memory address is NULL");
  unsigned char *memory = (unsigned char *)address;
  for (int i = 0; i < bytes; i++)
    memory[i] = value;
}

void EvAct_ShowMemoryData(int address)
{
  if (!address)
    DebugFatal("event-actions.c", "Memory address is NULL");
  char header[128];
  memset(header, 0, sizeof(header));
  sprintf(header, "Memory dump at address %08X", address);
  ShowDataOnScreen(header, (unsigned char *)address);
}

void EvAct_DestroyUnit(int side_id, bool silent, int unit_index)
{
  if (silent)
  {
    Unit *unit = GetUnit(side_id, unit_index);
    unit->State = UNIT_STATE_17_DEAD;
    unit->__DeadStateTimeCounter = 1;
    unit->__AttackerIndex = -1;
  }
  else
    DestroyUnit(side_id, unit_index);
}

void EvAct_DamageHealUnit(int side_id, int action, int units, int value, int unit_index)
{
  Unit *unit = GetUnit(side_id, unit_index);
  UnitAtribStruct *unit_template = &_templates_unitattribs[unit->Type];
  int hit_points = (units?((unit_template->__Strength * value) / 100):value) * (action?1:-1);
  unit->Health = LIMIT(unit->Health + hit_points, 0, unit_template->__Strength);
  if (!unit->Health)
    DestroyUnit(side_id, unit_index);
}

void EvAct_ChangeUnitType(int side_id, int target_type, bool defined_type, int unit_index)
{
  Unit *unit = GetUnit(side_id, unit_index);
  if (defined_type)
  {
    if (_templates_unitattribs[unit->Type].UnitUpgradeAllowed)
      target_type = _templates_unitattribs[unit->Type].UnitUpgradeTargetType;
    else
      return;
  }
  unit->Health = LLIMIT(unit->Health + _templates_unitattribs[target_type].__Strength - _templates_unitattribs[unit->Type].__Strength, 1);
  unit->Flags = (unit->Flags & (~_templates_unitattribs[unit->Type].Flags)) | _templates_unitattribs[target_type].Flags;
  unit->Speed = _templates_unitattribs[target_type].__Speed;
  unit->Type = target_type;
}

void EvAct_SetUnitFlag(int side_id, eFlagOperation operation, int flag, int unit_index)
{
  Unit *unit = GetUnit(side_id, unit_index);
  unit->Flags = FlagOperation(unit->Flags, flag, operation);
}

void EvAct_SetUnitProperty(int side_id, eDataSize data_size, int offset, eValueOperation operation, int value, int unit_index)
{
  Unit *unit = GetUnit(side_id, unit_index);
  SetDataValue((char *)unit, data_size, offset, operation, value);
}

void EvAct_SelectUnit(int side_id, bool exclude_from_restore, int unit_index)
{
  Unit *unit = GetUnit(side_id, unit_index);
  unit->__IsSelected = 1;
  if (exclude_from_restore)
    unit->PrevWasSelected = 0;
}

void EvAct_AirliftUnit(int side_id, int target_x, int target_y, bool units_target, int unit_index)
{
  CSide *side = GetSide(side_id);
  Unit *unit = GetUnit(side_id, unit_index);
  if (unit->State == UNIT_STATE_18_AWAITING_AIRLIFT)
    return;
  if (units_target && unit->TargetX == unit->BlockFromX && unit->TargetY == unit->BlockFromY)
    return;
  if (!units_target && target_x == unit->BlockFromX && target_y == unit->BlockFromY)
    return;
  int8_t queue_pos = CSide_46CCA0_get_queue_pos(side, unit);
  if ( queue_pos != -1 )
  {
    if (! units_target)
    {
      unit->TargetX = target_x;
      unit->TargetY = target_y;
    }
    if (unit->Flags & UFLAGS_BLOCKTOMARKED)
    {
      GameMapTileStruct *tile = &gGameMap.map[unit->BlockToX + _CellNumbersWidthSpan[unit->BlockToY]];
      tile->__tile_bitflags &= ~TileFlags_8_OCC_UNIT;
      if ( tile->__tile_bitflags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID) )
      {
        tile->__tile_bitflags &= ~(TileFlags_4_OWNER|TileFlags_2_OWNER|TileFlags_1_OWNER);
        tile->__tile_bitflags |= ((tile->__tile_bitflags >> 25) & 7);
      }
      unit->Flags &= ~UFLAGS_BLOCKTOMARKED;
      unit->BlockToX = unit->BlockFromX;
      unit->BlockToY = unit->BlockFromY;
      unit->__PosX = (unit->BlockFromX << 21) + (1 << 20);
      unit->__PosY = (unit->BlockFromY << 21) + (1 << 20);
      unit->__pos_stepsmax = 0;
      unit->pos_steps = 0;
    }
    if ( CSide__AddToQueue(side, unit, unit_index, queue_pos, 100, unit->State) )
    {
      UnitAdjustState(unit, UNIT_STATE_18_AWAITING_AIRLIFT);
    }
  }
}

void EvAct_ShowUnitData(int side_id, int unit_index)
{
  Unit *unit = GetUnit(side_id, unit_index);
  char buf[128];
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "Unit %d Type %d HP %d State %d Flags: ", unit_index, unit->Type, unit->Health, unit->State);
  int l = strlen(buf);
  memset(&buf[l], ' ', 43);
  for (int i = 0; i < 32; i++)
    buf[l + i + (i / 8)] = ((unit->Flags >> i & 1)?'X':'o');
  ShowDataOnScreen(buf, (unsigned char *)unit);
}

void EvAct_DamageHealBuilding(int side_id, int action, int units, int value, int building_index)
{
  Building *bld = GetBuilding(side_id, building_index);
  BuildingAtrbStruct *bld_template = &_templates_buildattribs[bld->Type];
  int hit_points = (units?((bld_template->_____HitPoints * value) / 100):value) * (action?1:-1);
  bld->Health = LIMIT((int)bld->Health + hit_points, 0, bld_template->_____HitPoints);
  if (!bld->Health)
    DestroyBuilding(side_id, building_index, 0);
}

void EvAct_ChangeBuildingOwner(int side_id, int target_side, int building_index)
{
  Building *bld = GetBuilding(side_id, building_index);
  bld->Flags |= BFLAGS_1000000_INFILTRATED;
  CaptureBuilding(side_id, target_side, building_index);
}

void EvAct_ChangeBuildingType(int side_id, int target_type, int building_index)
{
  Building *bld = GetBuilding(side_id, building_index);
  bld->Health = LLIMIT(bld->Health + _templates_buildattribs[target_type]._____HitPoints - _templates_buildattribs[bld->Type]._____HitPoints, 1);
  bld->Flags = (bld->Flags & (~_templates_buildattribs[bld->Type]._____Flags)) | _templates_buildattribs[target_type]._____Flags;
  bld->Type = target_type;
}

void EvAct_SetBuildingFlag(int side_id, eFlagOperation operation, int flag, int building_index)
{
  Building *bld = GetBuilding(side_id, building_index);
  bld->Flags = FlagOperation(bld->Flags, flag, operation);
}

void EvAct_SetBuildingProperty(int side_id, eDataSize data_size, int offset, eValueOperation operation, int value, int building_index)
{
  Building *bld = GetBuilding(side_id, building_index);
  SetDataValue((char *)bld, data_size, offset, operation, value);
}

void EvAct_SelectBuilding(int side_id, bool exclude_from_restore, int building_index)
{
  Building *bld = GetBuilding(side_id, building_index);
  bld->__IsSelected = 1;
  if (exclude_from_restore)
    bld->PrevWasSelected = 0;
}

void EvAct_ShowBuildingData(int side_id, int building_index)
{
  Building *bld = GetBuilding(side_id, building_index);
  char buf[128];
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "Building %d Type %d HP %d State %d Flags: ", building_index, bld->Type, bld->Health, bld->__State);
  int l = strlen(buf);
  memset(&buf[l], ' ', 43);
  for (int i = 0; i < 32; i++)
    buf[l + i + (i / 8)] = ((bld->Flags >> i & 1)?'X':'o');
  ShowDataOnScreen(buf, (unsigned char *)bld);
}

void EvAct_RemoveCrate(int crate_index)
{
  CrateStruct *crate = &gCrates[crate_index];
  crate->__is_active = 0;
  gGameMap.map[crate->__x + _CellNumbersWidthSpan[crate->__y]].__tile_bitflags &= ~TileFlags_1000_HAS_CRATE;
}

void EvAct_SetTileAttribute(eFlagOperation operation, int attribute, int cell_index)
{
  GameMapTileStruct *tile = &gGameMap.map[cell_index];
  TileFlags old_flags = tile->__tile_bitflags;
  tile->__tile_bitflags = FlagOperation(tile->__tile_bitflags, attribute, operation);
  TileFlags new_flags = tile->__tile_bitflags;
  // Check if spice was removed
  if ((old_flags & 0x700000) && !(new_flags & 0x700000))
    tile->__tile_index = tile->back_up_tile_index;
  // Check if concrete was removed
  if ((old_flags & TileFlags_800_HAS_CONCRETE) && !(new_flags & TileFlags_800_HAS_CONCRETE))
    tile->__tile_index = tile->back_up_tile_index;
  // Update spice visuals
  if ((attribute >= 20) && (attribute <= 22))
  {
    int xpos = cell_index % gGameMapWidth;
    int ypos = cell_index / gGameMapWidth;
    RECT r;
    r.left = LLIMIT(xpos - 1, 0);
    r.top = LLIMIT(ypos - 1, 0);
    r.right = HLIMIT(xpos + 2, gGameMapWidth);
    r.bottom = HLIMIT(ypos + 2, gGameMapHeight);
    UpdateSpiceInRegion(&r);
  }
}

void EvAct_SetTileDamage(eValueOperation operation, int value, int cell_index)
{
  GameMapTileStruct *tile = &gGameMap.map[cell_index];
  int damage = ValueOperation(tile->__damage, value, operation);
  tile->__damage = LIMIT(damage, 0, 255);
}

void EvAct_RevealTile(int cell_index)
{
  int xpos = cell_index % gGameMapWidth;
  int ypos = cell_index / gGameMapWidth;
  RevealCircle(xpos, ypos, 0);
}

void EvAct_OrderUnitRetreat(int side_id)
{
  CSide *side = GetSide(side_id);
  uint8_t x, y;
  if (CSide__FindBestBasePosition(side, &x, &y))
    GenerateUnitRetreatOrder(side_id, x, y);
}

void EvAct_OrderBuildBuildingCancel(int side_id, bool force)
{
  CSide *side = GetSide(side_id);
  if (force)
    side->__BuildingBuildQueue.__on_hold = 1;
  GenerateBuildBuildingCancelOrder(side_id, side->__BuildingBuildQueue.__type);
}

void EvAct_OrderBuildPlaceBuilding(int side_id, int xpos, int ypos)
{
  CSide *side = GetSide(side_id);
  GenerateBuildPlaceBuildingOrder(side_id, side->__BuildingBuildQueue.__type, xpos, ypos);
}

void EvAct_OrderBuildUnitCancel(int side_id, bool any_unit, int unit_type, int queue, bool force)
{
  CSide *side = GetSide(side_id);
  char *queue_groups = (char *)&_templates_GroupIDs;
  for (int i = 0; i < 10; i++)
  {
    int unit_type_built = side->__UnitBuildQueue[i].__type;
    int prereq1_group = _templates_unitattribs[unit_type_built].__PreReq1;
    if ((!any_unit && unit_type_built == unit_type) || (any_unit && (queue == 0 || (queue > 0 && prereq1_group == queue_groups[(queue <= 6)?queue:queue+3]))))
    {
      if (force)
        side->__UnitBuildQueue[i].__on_hold = 1;
      GenerateBuildUnitCancelOrder(side_id, unit_type_built);
    }
  }
}

void EvAct_OrderStarportPick(int side_id, int unit_type)
{
  CSide *side = GetSide(side_id);
  if (!side->__StarportDeliveryInProgress)
    GenerateStarportPickOrder(side_id, unit_type);
}

void EvAct_OrderUpgradeCancel(int side_id, bool force)
{
  CSide *side = GetSide(side_id);
  if (force)
    side->__BuildingUpgradeQueue.__on_hold = 1;
  GenerateUpgradeCancelOrder(side_id, side->__BuildingUpgradeQueue.__type);
}
