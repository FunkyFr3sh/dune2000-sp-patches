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
#include "../extended-maps/crates-func.h"
#include "../extended-maps/messages-func.h"
#include "../extended-maps/tooltips.h"

void EvAct_AddDelivery(int xpos, int ypos, int side_id, int amount, int tag, int deploy_action, int delay, eDeliveryType delivery_type, char *unit_list)
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
  side->__Deliveries[found_free_slot].__delivery_time = gGameTicks + delay;
  side->__Deliveries[found_free_slot].__deploy_action = deploy_action;
  side->__Deliveries[found_free_slot].__delivery_type = delivery_type;
  side->__Deliveries[found_free_slot].tag = tag;
  memcpy(side->__Deliveries[found_free_slot].__units, unit_list, amount);
  side->__Deliveries[found_free_slot].__units[amount] = -1;
  if (delivery_type == DELIVERYTYPE_STARPORT)
    side->__StarportDeliveryInProgress = 1;
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

void EvAct_PlaySound(int sample_id, bool force, bool point_sound, int xpos, int ypos)
{
  if (point_sound)
    PlaySoundAt(sample_id, xpos, ypos);
  else
  {
    if (force)
      ISampleManager__EndSample(_gSampleMgr, 0);
    Sound__PlaySample(sample_id, 1, 0, force?1:0);
  }
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
  // Fix powerbar after switching side
  GetSide(side_id)->__PowerPercent2 = 0xFFFF;
  // Fix broken building/unit icons after switching side
  _blitflag = 1;
  // Force radar side icon to redraw
  _TacticalData.__radarfade = 1;
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
      gGameMap.map[x + _CellNumbersWidthSpan[y]].__shroud = 1;
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

void GetTextStringToBuffer(int string_id, char *buffer, int buffer_size)
{
  // Attempt to get custom text from mission ini file
  char mapIniPath[256];
  char id[12];
  sprintf(mapIniPath, ".\\%s%s", GameType == GT_SINGLEPLAYER ? MissionsResourcePath : MapsResourcePath, PathChangeExtension(MissionMap, ".ini"));
  sprintf(id, "%d", string_id);
  IniGetString("Text", id, "", buffer, buffer_size, mapIniPath);
  if (!strlen(buffer))
  {
    // Get text from string table
    char *string_from_table = GetTextString(string_id, 1);
    strncpy(buffer, string_from_table, buffer_size);
  }
}

void dummy(void);

void GetMessageText(char *buffer, unsigned int buffer_len, ShowMessageEventData *data)
{
  // Get the main string
  GetTextStringToBuffer(data->string_index, buffer, buffer_len);
  // Substitute variables in string
  char var_text[512];
  unsigned int pos = 0;
  unsigned int len = strlen(buffer);
  for (unsigned int i = 0; i < sizeof(data->variable_index); i++)
  {
    bool found = false;
    while (buffer[pos] && !found)
    {
      if (buffer[pos] == '@')
      {
        if (data->variable_type[i])
        {
          var_text[0] = 0;
          int_or_float val;
          val.int_val = GetVariableValue(data->variable_index[i]);
          switch (data->variable_type[i])
          {
            case MSGVARIABLETYPE_NUMBER: sprintf(var_text, "%d", val.int_val); break;
            case MSGVARIABLETYPE_TIME: { int secs = val.int_val / 25; sprintf(var_text, "%02d:%02d", secs / 60, secs % 60); break;};
            case MSGVARIABLETYPE_HEXNUMBER: sprintf(var_text, "%08X", val.int_val); break;
            case MSGVARIABLETYPE_FLOAT1: sprintf(var_text, "%.1f", *(float *)&val.float_val); break;
            case MSGVARIABLETYPE_FLOAT2: sprintf(var_text, "%.2f", *(float *)&val.float_val); break;
            case MSGVARIABLETYPE_FLOAT3: sprintf(var_text, "%.3f", *(float *)&val.float_val); break;
            case MSGVARIABLETYPE_FLOAT4: sprintf(var_text, "%.4f", *(float *)&val.float_val); break;
            case MSGVARIABLETYPE_STRING_FROM_TABLE: GetTextStringToBuffer(val.int_val, var_text, sizeof(var_text)); break;
            case MSGVARIABLETYPE_UNIT_NAME:
            {
              if (val.int_val >= 0 && val.int_val < 60)
              {
                int unit_group = _templates_unitattribs[val.int_val].__UnitType;
                int string_id = _UnitGroupTextIds[unit_group];
                strcpy(var_text, (string_id != -1)?GetTextString(string_id, 0):_templates_UnitGroupNameList[unit_group]);
              }
              break;
            };
            case MSGVARIABLETYPE_BUILDING_NAME:
            {
              if (val.int_val >= 0 && val.int_val < 100)
              {
                int building_group = _templates_buildattribs[val.int_val].GroupType;
                int string_id = _BuildingGroupTextIds[building_group];
                strcpy(var_text, (string_id != -1)?GetTextString(string_id, 0):_templates_BuildingGroupNameList[building_group]);
              }
              break;
            };
            case MSGVARIABLETYPE_UNIT_TYPE: if (val.int_val >= 0 && val.int_val < 60) {strcpy(var_text, _templates_UnitNameList[val.int_val]);} break;
            case MSGVARIABLETYPE_BUILDING_TYPE: if (val.int_val >= 0 && val.int_val < 100) {strcpy(var_text, _templates_BuildingNameList[val.int_val]);} break;
            case MSGVARIABLETYPE_UNIT_GROUP: if (val.int_val >= 0 && val.int_val < 60) {strcpy(var_text, _templates_UnitGroupNameList[val.int_val]);} break;
            case MSGVARIABLETYPE_BUILDING_GROUP: if (val.int_val >= 0 && val.int_val < 100) {strcpy(var_text, _templates_BuildingGroupNameList[val.int_val]);} break;
            case MSGVARIABLETYPE_WEAPON_NAME: if (val.int_val >= 0 && val.int_val < 64) {strcpy(var_text, _templates_BulletNameList[val.int_val]);} break;
            case MSGVARIABLETYPE_EXPLOSION_NAME: if (val.int_val >= 0 && val.int_val < 64) {strcpy(var_text, _templates_ExplosionNameList[val.int_val]);} break;
            case MSGVARIABLETYPE_WARHEAD_NAME: if (val.int_val >= 0 && val.int_val < 30) {strcpy(var_text, _WarheadNames[val.int_val]);} break;
            case MSGVARIABLETYPE_ARMOUR_TYPE: if (val.int_val >= 0 && val.int_val < 12) {strcpy(var_text, _ArmourNames[val.int_val]);} break;
          }
          unsigned int var_text_len = strlen(var_text);

          if (len + var_text_len - 1 >= buffer_len)
            break;
          if (var_text_len > 0)
          {
            for (unsigned int j = len; j > pos; j--)
            {
              buffer[j + var_text_len - 1] = buffer[j];
            }
          }
          else
          {
            for (unsigned int j = pos; j < len; j++)
            {
              buffer[j] = buffer[j + 1];
              dummy();
            }
          }
          for (unsigned int j = 0; j < var_text_len; j++)
            buffer[pos + j] = var_text[j];
          pos += var_text_len - 1;
          len += var_text_len - 1;

        }
        found = true;
      }
      pos++;
    }
  }
}

void EvAct_ShowMessage(int xoff, int yoff, int ref_id, int screen_pos, int color, eMsgSoundMode sound_mode, bool type_on, int duration, ShowMessageEventData *data)
{
  // Play message sound
  switch (sound_mode)
  {
    case MSGSOUNDMODE_DEFAULT:      Sound__PlaySample(Data__GetSoundTableID("S_CHATMSG"), 1, 0, 0); break;
    case MSGSOUNDMODE_NONE:         break;
    case MSGSOUNDMODE_CUSTOM:       Sound__PlaySample(data->sample_id, 1, 0, 0); break;
    case MSGSOUNDMODE_CUSTOM_FORCE: ISampleManager__EndSample(_gSampleMgr, 0); Sound__PlaySample(data->sample_id, 1, 0, 1); break;
  }
  // Get message text
  char buffer[512];
  GetMessageText(buffer, sizeof(buffer), data);
  // Show message
  QueueMessageExt(buffer, duration, ref_id, screen_pos, xoff, yoff, color, type_on);
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
    if (unit_index == -1)
      continue;
    Unit *unit = GetUnit(side_id, unit_index);
    if (!unit)
      continue;
    unit->__Facing = facing << 2;
    unit->__FacingTurret = facing << 2;
    unit->__FacingTurretTarget = facing << 2;
    unit->Tag = tag;
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

void EvAct_AddUnit(int xpos, int ypos, int side_id, int properties, int unit_type, int movement, int facing, int tag, int target_var)
{
  GameMapTileStruct *tile = &gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]];
  // Do not add unit if tile is already occupied by unit
  if (_templates_unitattribs[unit_type].__Behavior != UnitBehavior_SANDWORM && tile->__tile_bitflags & TileFlags_8_OCC_UNIT)
  {
    SetVariableValue(target_var, -1);
    return;
  }
  // Do not add next infantry if all 5 slots are already occupied
  if (_templates_unitattribs[unit_type].__IsInfantry && ((tile->__tile_bitflags & 0x3E0) == 0x3E0))
  {
    SetVariableValue(target_var, -1);
    return;
  }
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
  if (unit_index != -1)
  {
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
  }
  // Revert back owner side attributes
  if (occ_building && (orig_owner_side != side_id))
    tile->__tile_bitflags = (tile->__tile_bitflags & ~7) | orig_owner_side;
  // Store unit index into variable
  SetVariableValue(target_var, unit_index);
}

void EvAct_AddBuilding(int xpos, int ypos, int side_id, int properties, int building_type, int method, int facing, int tag, int target_var)
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
  // Store unit index into variable
  SetVariableValue(target_var, building_index);
}

void EvAct_AddBullet(int src_x, int src_y, int targ_x, int targ_y, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int weapon_type, bool circle_spread, bool play_sound, int tag, int target_var)
{
  int pixel_pos_x = targ_x * 32 + pixel_x;
  int pixel_pos_y = targ_y * 32 + pixel_y;
  random_spread(&pixel_pos_x, &pixel_pos_y, spread_x, spread_y, circle_spread);

  int bullet_index = ModelAddBullet(side_id, weapon_type, 0, -1, src_x*32+16, src_y*32+16, pixel_pos_x, pixel_pos_y, -1, -1);
  SetVariableValue(target_var, bullet_index);
  if (bullet_index != -1)
  {
    Bullet *bullet = (Bullet *)&GetSide(side_id)->__ObjectArray[bullet_index];
    bullet->Tag = tag;
  }
  if (play_sound)
    PlaySoundAt(_templates_bulletattribs[weapon_type].__FiringSound, src_x, src_y);
}

void EvAct_AddExplosion(int xpos, int ypos, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int explosion_type, bool circle_spread, bool play_sound, int tag, int target_var)
{
  int pixel_pos_x = xpos * 32 + pixel_x;
  int pixel_pos_y = ypos * 32 + pixel_y;
  random_spread(&pixel_pos_x, &pixel_pos_y, spread_x, spread_y, circle_spread);

  int explosion_index = ModelAddExplosion(side_id, explosion_type, pixel_pos_x, pixel_pos_y, 0, 0, 0, 0, 0);
  SetVariableValue(target_var, explosion_index);
  if (explosion_index != -1)
  {
    Explosion *explosion = (Explosion *)&GetSide(side_id)->__ObjectArray[explosion_index];
    explosion->Tag = tag;
  }
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

void EvAct_AddConcrete(int min_x, int min_y, int max_x, int max_y, int side_id, int tilebitmask)
{
  if (tilebitmask)
    ModelAddConcrete(side_id, CSide__MyVersionOfBuilding(GetSide(side_id), _templates_GroupIDs.Concrete1, 0), min_x, min_y, 0, tilebitmask);
  else
  {
    for (int y = min_y; y <= max_y; y++)
      for (int x = min_x; x <= max_x; x++)
      {
        GameMapTileStruct *tile = &gGameMap.map[x + _CellNumbersWidthSpan[y]];
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

void ChangeMapTile(int xpos, int ypos, int new_tile_index, eChangeTileMode mode)
{
  if (new_tile_index == 65535)
    return;
  GameMapTileStruct *tile = &gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]];
  switch (mode)
  {
    case CHANGETILE_NORMAL:   break;
    case CHANGETILE_VISUAL:   tile->__tile_index = new_tile_index; return;
    case CHANGETILE_RESTORE:  tile->__tile_index = tile->back_up_tile_index; return;
  }
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
    if (destroy_buildings && bld->__State != 17 && !(_templates_buildattribs[bld->Type]._____Flags & BFLAGS_400000_NO_CONCRETE))
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

void EvAct_ChangeMapBlock(int xpos, int ypos, int width, int height, eChangeTileMode mode, uint16_t *tiles)
{
  int tile_count = height * width;
  if (tile_count > 12)
    DebugFatal("event-actions.c", "Max allowed tiles in Change Tiles event is 12, actual is %d", tile_count);
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      ChangeMapTile(x + xpos, y + ypos, tiles[y * width + x], mode);
  // Update spice visuals
  RECT r;
  r.left = LLIMIT(xpos - 1, 0);
  r.top = LLIMIT(ypos - 1, 0);
  r.right = HLIMIT(xpos + width + 1, gGameMapWidth);
  r.bottom = HLIMIT(ypos + height + 1, gGameMapHeight);
  UpdateSpiceInRegion(&r);
}

void EvAct_TransformTiles(int amount, eChangeTileMode mode, uint16_t *tiles)
{
  if (amount > 6)
    DebugFatal("event-actions.c", "Max allowed tile pairs in Transform Tiles event is 6, actual is %d", amount);
  for (int y = 0; y < gGameMapHeight; y++)
    for (int x = 0; x < gGameMapWidth; x++)
      for (int i = 0; i < amount; i++)
        if (gGameMap.map[x + _CellNumbersWidthSpan[y]].__tile_index == tiles[i * 2])
        {
          ChangeMapTile(x, y, tiles[i * 2 + 1], mode);
          break;
        }
}

void EvAct_AddBuildingDestruct(int xpos, int ypos, int side_id, int building_type)
{
  int old_screen_shakes = _ScreenShakes;
  int building_index = ModelAddBuilding(side_id, building_type, xpos, ypos, 0, 1, 1);
  if (building_index == -1)
    return;
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

void EvAct_RemoveMessage(eRemoveMessageMode mode, int ref_id, int amount)
{
  switch (mode)
  {
    case REMOVEMSGMODE_REFID:
    {
      for (int i = 0; i < MAX_MESSAGES; i++)
        if (gMessageData[i].ref_id >= ref_id && gMessageData[i].ref_id < (ref_id + amount))
          gMessageData[i].expire_ticks = 0;
      break;
    }
    case REMOVEMSGMODE_CHATOLDEST:
    {
      while (amount)
      {
        unsigned int min_order_added = UINT_MAX;
        int slot_to_remove = -1;
        for (int i = 0; i < MAX_MESSAGES; i++)
        {
          if (gMessageData[i].expire_ticks <= gGameTicks)
            continue;
          if (gMessageData[i].is_chat && gMessageData[i].order_added < min_order_added)
          {
            min_order_added = gMessageData[i].order_added;
            slot_to_remove = i;
          }
        }
        if (slot_to_remove != -1)
        {
          gMessageData[slot_to_remove].expire_ticks = 0;
          amount--;
        }
        else
          break;
      }
      break;
    }
    case REMOVEMSGMODE_CHATNEWEST:
    {
      while (amount)
      {
        unsigned int max_order_added = 0;
        int slot_to_remove = -1;
        for (int i = 0; i < MAX_MESSAGES; i++)
        {
          if (gMessageData[i].expire_ticks <= gGameTicks)
            continue;
          if (gMessageData[i].is_chat && gMessageData[i].order_added > max_order_added)
          {
            max_order_added = gMessageData[i].order_added;
            slot_to_remove = i;
          }
        }
        if (slot_to_remove != -1)
        {
          gMessageData[slot_to_remove].expire_ticks = 0;
          amount--;
        }
        else
          break;
      }
      break;
    }
  }
}

void EvAct_SetMessageColor(int color_index, eSetMessageColorMode mode, int transition_speed, int transition_stages, int color1, int color2)
{
  switch (mode)
  {
    case SETMSGCOLORMODE_SOLID_SHADOW: SetFontColorSolid(color_index, color1, color2); break;
    case SETMSGCOLORMODE_COLOR_GRADIENT: SetFontColorGradient(color_index, color1, color2, -1, -1); break;
    case SETMSGCOLORMODE_TIME_TRANSITION_1: SetFontColorGradient(color_index, color1, color2, (gGameTicks >> transition_speed) % transition_stages, transition_stages); break;
    case SETMSGCOLORMODE_TIME_TRANSITION_2:
    {
      int modtick = (gGameTicks >> transition_speed) % (transition_stages + transition_stages - 2);
      int stage = (modtick < transition_stages)?modtick:transition_stages - 2 - (modtick - transition_stages);
      SetFontColorGradient(color_index, color1, color2, stage, transition_stages);
      break;
    }
  }
}

void EvAct_SetTooltip(int line, eSetTooltipColorMode color_mode, int color, ShowMessageEventData *data)
{
  switch (color_mode)
  {
    case SETTOOLTIPCOLORMODE_KEEP: break;
    case SETTOOLTIPCOLORMODE_YELLOW: gTooltipExtraData[line].color = _tooltipcolor_yellow; break;
    case SETTOOLTIPCOLORMODE_GRAY: gTooltipExtraData[line].color = _tooltipcolor_gray; break;
    case SETTOOLTIPCOLORMODE_CUSTOM: gTooltipExtraData[line].color = color; break;
  }
  GetMessageText(gTooltipExtraData[line].text, 100, data);
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
  sprintf(header, "Side %d (%p) Cash %d %d Spice %d %d Storage %d PowerOutput %d PowerDrain %d", side_id, (char *)side + offset, side->CashDrip, side->CashReal, side->SpiceDrip, side->SpiceReal, side->__MaxStorage, side->__PowerOutput, side->__PowerDrained);
  ShowDataOnScreen(header, ((unsigned char *)side) + offset);
}

void EvAct_SetAIProperty(int side_id, eDataType data_type, eValueOperation operation, int offset, int value)
{
  CAI_ *ai = &_gAIArray[side_id];
  SetDataValue((char *)ai, data_type, offset, operation, value);
}

void EvAct_ShowAIData(int side_id, int offset)
{
  CAI_ *ai = &_gAIArray[side_id];
  char header[128];
  memset(header, 0, sizeof(header));
  sprintf(header, "AI %d (%p) memory dump at offset %d", side_id, (char *)ai + offset, offset);
  ShowDataOnScreen(header, ((unsigned char *)ai) + offset);
}

void EvAct_SetMemoryData(eDataType data_type, eValueOperation operation, int address, int value)
{
  if (!address)
    DebugFatal("event-actions.c", "Memory address is NULL");
  SetDataValue((char *)address, data_type, 0, operation, value);
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
  Unit *unit = GetUnit(side_id, unit_index);
  if (unit->State == UNIT_STATE_17_DEAD)
    return;
  if (silent)
  {  
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
  if (unit->State == UNIT_STATE_17_DEAD)
    return;
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

void EvAct_SetUnitProperty(int side_id, eDataType data_type, int offset, eValueOperation operation, int value, int unit_index)
{
  Unit *unit = GetUnit(side_id, unit_index);
  SetDataValue((char *)unit, data_type, offset, operation, value);
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
  int8_t queue_pos = CSide__GetQueuePos(side, unit);
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
  sprintf(buf, "Unit %d (%p) Type %d HP %d State %d Flags: ", unit_index, unit, unit->Type, unit->Health, unit->State);
  int l = strlen(buf);
  memset(&buf[l], ' ', 43);
  for (int i = 0; i < 32; i++)
    buf[l + i + (i / 8)] = ((unit->Flags >> i & 1)?'X':'o');
  ShowDataOnScreen(buf, (unsigned char *)unit);
}

void EvAct_DestroyBuilding(int side_id, bool silent, int building_index)
{
  Building *bld = GetBuilding(side_id, building_index);
  if (bld->__State == UNIT_STATE_17_DEAD)
    return;
  if (silent)
  {
    bld->__State = UNIT_STATE_17_DEAD;
    bld->__DeadStateTimeCounter = 1;
    bld->__AttackerIndex = -1;
  }
  else
    DestroyBuilding(side_id, building_index, 0);
}

void EvAct_DamageHealBuilding(int side_id, int action, int units, int value, int building_index)
{
  Building *bld = GetBuilding(side_id, building_index);
  if (bld->__State == UNIT_STATE_17_DEAD)
    return;
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

void EvAct_SetBuildingProperty(int side_id, eDataType data_type, int offset, eValueOperation operation, int value, int building_index)
{
  Building *bld = GetBuilding(side_id, building_index);
  SetDataValue((char *)bld, data_type, offset, operation, value);
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
  sprintf(buf, "Building %d (%p) Type %d HP %d State %d Flags: ", building_index, bld, bld->Type, bld->Health, bld->__State);
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

void EvAct_PickupCrate(int side_id, int crate_index)
{
  DoPickupCrate(crate_index, NULL, side_id);
}

void EvAct_SetCrateProperty(eDataType data_type, int offset, eValueOperation operation, int value, int crate_index)
{
  CrateStruct *crate = &gCrates[crate_index];
  int old_x = crate->__x;
  int old_y = crate->__y;
  SetDataValue((char *)crate, data_type, offset, operation, value);
  // Crate was moved around - fix attributes
  if (old_x != crate->__x || old_y != crate->__y)
  {
    gGameMap.map[old_x + _CellNumbersWidthSpan[old_y]].__tile_bitflags &= ~TileFlags_1000_HAS_CRATE;
    gGameMap.map[crate->__x + _CellNumbersWidthSpan[crate->__y]].__tile_bitflags |= TileFlags_1000_HAS_CRATE;
  }
}

void EvAct_ShowCrateData(int crate_index)
{
  CrateStruct *crate = &gCrates[crate_index];
  char buf[128];
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "Crate %d (%p) at %d , %d Type %d Image %d Ext data %d Respawns %d Timing: %d", crate_index, crate, crate->__x, crate->__y, crate->__type, crate->__image, crate->ext_data_field, crate->__times_to_respawn, crate->__timing);
  QueueMessageExt(buf, 1, 250, 0, 0, 0, 0, 0);
}

void EvAct_ChangeTile(eChangeTileMode mode, int tile_index, int xpos, int ypos)
{
  ChangeMapTile(xpos, ypos, tile_index, mode);
}

void EvAct_SetTileAttribute(eFlagOperation operation, int attribute, int xpos, int ypos)
{
  GameMapTileStruct *tile = &gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]];
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
    RECT r;
    r.left = LLIMIT(xpos - 1, 0);
    r.top = LLIMIT(ypos - 1, 0);
    r.right = HLIMIT(xpos + 2, gGameMapWidth);
    r.bottom = HLIMIT(ypos + 2, gGameMapHeight);
    UpdateSpiceInRegion(&r);
  }
}

void EvAct_SetTileDamage(eValueOperation operation, int value, int xpos, int ypos)
{
  GameMapTileStruct *tile = &gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]];
  int damage = ValueOperation(tile->__damage, value, operation);
  tile->__damage = LIMIT(damage, 0, 255);
}

void EvAct_RevealTile(int radius, int xpos, int ypos)
{
  RevealCircle(xpos, ypos, radius);
}

void EvAct_HideTile(int xpos, int ypos)
{
  if ( gBitsPerPixel == 16 )
  {
    int *img = _RadarMap1 + 16;
    uint16_t *buffer = (uint16_t *)*img;
    buffer[ypos * gGameMap.width + xpos] = 0;
  }
  else
  {
    int *img = _RadarMap1 + 16;
    uint8_t *buffer = (uint8_t *)*img;
    buffer[ypos * gGameMap.width + xpos] = 0;
  }
  for (int y = LLIMIT(ypos - 1, 0); y <= HLIMIT(ypos + 1, gGameMapHeight - 1); y++)
    for (int x = LLIMIT(xpos - 1, 0); x <= HLIMIT(xpos + 1, gGameMapWidth - 1); x++)
      gGameMap.map[x + _CellNumbersWidthSpan[y]].__shroud = 1;
  RECT r;
  r.left = xpos - 2;
  r.top = ypos - 2;
  r.right = xpos + 3;
  r.bottom = ypos + 3;
  UpdateShroudInRegion(&r, gGameMapWidth, gGameMapHeight);
}

void EvAct_ShowTileData(int xpos, int ypos)
{
  GameMapTileStruct *tile = &gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]];
  char buf[128];
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "Tile %d %d (%p) Idx %d Backup %d Shroud %d Damage %d Flags: ", xpos, ypos, tile, tile->__tile_index, tile->back_up_tile_index, tile->__shroud, tile->__damage);
  int l = strlen(buf);
  memset(&buf[l], ' ', 43);
  for (int i = 0; i < 32; i++)
    buf[l + i + (i / 8)] = ((tile->__tile_bitflags >> i & 1)?'X':'o');
  QueueMessageExt(buf, 1, 250, 0, 0, 0, 0, 0);
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

void EvAct_SetVariable(int target_var, bool use_offset, int offset_var, eValueOperation operation, int value)
{
  if (use_offset)
    target_var += GetVariableValue(offset_var);
  SetVariableValue(target_var, ValueOperation(GetVariableValue(target_var), value, operation));
}

void EvAct_GetVariable(int target_var, int src_var_base, int src_var_offset)
{
  SetVariableValue(target_var, GetVariableValue(src_var_base + GetVariableValue(src_var_offset)));
}

void EvAct_SetFloatVariable(int target_var, bool use_offset, int offset_var, eValueOperation operation, int value)
{
  if (use_offset)
    target_var += GetVariableValue(offset_var);
  int_or_float operand1;
  int_or_float operand2;
  int_or_float result;
  operand1.int_val = GetVariableValue(target_var);
  operand2.int_val = value;
  result.float_val = ValueOperationFloat(operand1.float_val, operand2.float_val, operation);
  SetVariableValue(target_var, result.int_val);
}

void EvAct_ConvertVariable(int first_var, int number_of_vars, int operation)
{
  for (int i = 0; i < number_of_vars; i++)
  {
    if (operation == 0)
    {
      int operand = GetVariableValue(first_var + i);
      int_or_float result;
      result.float_val = (float)operand;
      SetVariableValue(first_var + i, result.int_val);
    }
    else
    {
      int_or_float operand;
      operand.int_val = GetVariableValue(first_var + i);
      int result = (int)operand.float_val;
      SetVariableValue(first_var + i, result);
    }
  }
}

void EvAct_GetRandomValue(int target_var, int min_value, int max_value)
{
  if (min_value > max_value)
  {
    int tmp = max_value;
    max_value = min_value;
    min_value = tmp;
  }
  int range = max_value - min_value + 1;
  SetVariableValue(target_var, (rand() % range) + min_value);
}

void EvAct_GetRandomCoords(int min_x, int min_y, int max_x, int max_y, int first_var)
{
  SetVariableValue(first_var,     (rand() % (max_x - min_x + 1)) + min_x);
  SetVariableValue(first_var + 1, (rand() % (max_y - min_y + 1)) + min_y);
}

void EvAct_GetValueFromList(int event_index, int amount, int target_var, int mode, int index_var, uint8_t *value_list)
{
  int index;
  if (!amount)
    DebugFatal("event-actions.c", "List has zero values! (event %d)", event_index);
  if (mode)
  {
    index = rand() % amount;
  }
  else
  {
    index = GetVariableValue(index_var);
    if ((index >= amount) || (index < 0))
      DebugFatal("event-actions.c", "Indexing list of values out of range! Index: %d Amount: %d (event %d)", index, amount, event_index);
  }
  SetVariableValue(target_var, value_list[index]);
}

void EvAct_GetCoordsFromList(int event_index, int amount, int first_var, int mode, int index_var, uint8_t *value_list)
{
  int index;
  if (!amount)
    DebugFatal("event-actions.c", "List has zero coordinates! (event %d)", event_index);
  if (mode)
  {
    index = rand() % amount;
  }
  else
  {
    index = GetVariableValue(index_var);
    if ((index >= amount) || (index < 0))
      DebugFatal("event-actions.c", "Indexing list of coordinates out of range! Index: %d Amount: %d (event %d)", index, amount, event_index);
  }
  SetVariableValue(first_var,     value_list[index * 2 + 1]);
  SetVariableValue(first_var + 1, value_list[index * 2 + 2]);
}

void EvAct_GetAreaFromList(int event_index, int amount, int first_var, int mode, int index_var, uint8_t *value_list)
{
  int index;
  if (!amount)
    DebugFatal("event-actions.c", "List has zero areas! (event %d)", event_index);
  if (mode)
  {
    index = rand() % amount;
  }
  else
  {
    index = GetVariableValue(index_var);
    if ((index >= amount) || (index < 0))
      DebugFatal("event-actions.c", "Indexing list of areas out of range! Index: %d Amount: %d (event %d)", index, amount, event_index);
  }
  SetVariableValue(first_var,     value_list[index * 4 + 1]);
  SetVariableValue(first_var + 1, value_list[index * 4 + 2]);
  SetVariableValue(first_var + 2, value_list[index * 4 + 3]);
  SetVariableValue(first_var + 3, value_list[index * 4 + 4]);
}

void EvAct_GetCount(int target_var)
{
  SetVariableValue(target_var, GetVariableValue(target_var) + 1);
}

void EvAct_GetSpiceCount(int target_var, int xpos, int ypos)
{
  SetVariableValue(target_var, GetVariableValue(target_var) + ((gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]].__tile_bitflags >> 20) & 7));
}

void EvAct_GetDamageCount(int target_var, int xpos, int ypos)
{
  SetVariableValue(target_var, GetVariableValue(target_var) + gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]].__damage);
}

void EvAct_GetObjectProperty(int side_id, eDataType data_type, int offset, int index_var, int target_var)
{
  Unit *obj = &GetSide(side_id)->__ObjectArray[GetVariableValue(index_var)];
  SetVariableValue(target_var, GetDataValue((char *)obj, data_type, offset));
}

void EvAct_GetCrateProperty(eDataType data_type, int offset, int index_var, int target_var)
{
  CrateStruct *crate = &gCrates[GetVariableValue(index_var)];
  SetVariableValue(target_var, GetDataValue((char *)crate, data_type, offset));
}

void EvAct_GetTileProperty(eDataType data_type, int offset, int first_var, int target_var)
{
  int xpos = GetVariableValue(first_var);
  int ypos = GetVariableValue(first_var + 1);
  GameMapTileStruct *tile = &gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]];
  SetVariableValue(target_var, GetDataValue((char *)tile, data_type, offset));
}

void EvAct_GetSideProperty(int side_id, eDataType data_type, int target_var, int offset)
{
  CSide *side = GetSide(side_id);
  SetVariableValue(target_var, GetDataValue((char *)side, data_type, offset));
}

void EvAct_GetAIProperty(int side_id, eDataType data_type, int target_var, int offset)
{
  CAI_ *ai = &_gAIArray[side_id];
  SetVariableValue(target_var, GetDataValue((char *)ai, data_type, offset));
}

void EvAct_GetMemoryData(eDataType data_type, int target_var, int address)
{
  if (!address)
    DebugFatal("event-actions.c", "Memory address is NULL");
  SetVariableValue(target_var, GetDataValue((char *)address, data_type, 0));
}

void EvAct_GetUnitTemplateProperty(eDataType data_type, int offset, int unit_type, int target_var)
{
  UnitAtribStruct *unit_template = &_templates_unitattribs[unit_type];
  SetVariableValue(target_var, GetDataValue((char *)unit_template, data_type, offset));
}

void EvAct_GetBuildingTemplateProperty(eDataType data_type, int offset, int building_type, int target_var)
{
  BuildingAtrbStruct *building_template = &_templates_buildattribs[building_type];
  SetVariableValue(target_var, GetDataValue((char *)building_template, data_type, offset));
}

void EvAct_GetWeaponTemplateProperty(eDataType data_type, int offset, int weapon_type, int target_var)
{
  BullAtrbStruct *weapon_template = &_templates_bulletattribs[weapon_type];
  SetVariableValue(target_var, GetDataValue((char *)weapon_template, data_type, offset));
}

void EvAct_GetExplosionTemplateProperty(eDataType data_type, int offset, int explosion_type, int target_var)
{
  ExploisonAtrbStruct *explosion_template = &_templates_explosionattribs[explosion_type];
  SetVariableValue(target_var, GetDataValue((char *)explosion_template, data_type, offset));
}

void EvAct_GetUnitType(int target_var, bool random, ObjectFilterStruct *filter)
{
  int result = -1;
  if (random)
  {
    int found = 0;
    int found_array[60];
    for (int i = 0; i < gUnitTypeNum; i++)
    {
      if (CheckIfUnitTypeMatchesFilter(filter, i))
        found_array[found++] = i;
    }
    if (found)
      result = found_array[rand() % found];
  }
  else
  {
    for (int i = 0; i < gUnitTypeNum; i++)
    {
      if (CheckIfUnitTypeMatchesFilter(filter, i))
      {
        result = i;
        break;
      }
    }
  }
  SetVariableValue(target_var, result);
}

void EvAct_GetBuildingType(int target_var, bool random, ObjectFilterStruct *filter)
{
  int result = -1;
  if (random)
  {
    int found = 0;
    int found_array[100];
    for (int i = 0; i < gBuildingTypeNum; i++)
    {
      if (CheckIfBuildingTypeMatchesFilter(filter, i))
        found_array[found++] = i;
    }
    if (found)
      result = found_array[rand() % found];
  }
  else
  {
    for (int i = 0; i < gBuildingTypeNum; i++)
    {
      if (CheckIfBuildingTypeMatchesFilter(filter, i))
      {
        result = i;
        break;
      }
    }
  }
  SetVariableValue(target_var, result);
}

void EvAct_GetGameTicks(int target_var)
{
  SetVariableValue(target_var, gGameTicks);
}

void EvAct_GetMySideId(int target_var)
{
  SetVariableValue(target_var, gSideId);
}

void EvAct_GetDifficulty(int target_var)
{
  SetVariableValue(target_var, gDifficultyLevel);
}

void EvAct_GetDiplomacy(int source, int target, int target_var)
{
  SetVariableValue(target_var, _gDiplomacy[source][target]);
}

void EvAct_GetTech(int side_id, int target_var)
{
  SetVariableValue(target_var, _gMiscData.Tech[side_id]);
}

void EvAct_GetHouseId(int side_id, int target_var)
{
  SetVariableValue(target_var, _IRValues[side_id]);
}

void EvAct_GetCredits(int side_id, eGetCreditsType what, int target_var)
{
  CSide *side = GetSide(side_id);
  switch (what)
  {
    case GETCREDITSTYPE_TOTAL:        SetVariableValue(target_var, side->CashReal + side->SpiceReal); break;
    case GETCREDITSTYPE_SPICE:        SetVariableValue(target_var, side->SpiceReal); break;
    case GETCREDITSTYPE_CASH:         SetVariableValue(target_var, side->CashReal); break;
    case GETCREDITSTYPE_MAX_STORAGE:  SetVariableValue(target_var, side->__MaxStorage); break;
  }
}

void EvAct_GetPower(int side_id, eGetPowerType what, int target_var)
{
  CSide *side = GetSide(side_id);
  switch (what)
  {
    case GETPOWERTYPE_PERCENT:      SetVariableValue(target_var, side->__PowerPercent1); break;
    case GETPOWERTYPE_TOTAL_OUTPUT: SetVariableValue(target_var, side->__PowerOutput); break;
    case GETPOWERTYPE_TOTAL_DRAIN:  SetVariableValue(target_var, side->__PowerDrained); break;
    case GETPOWERTYPE_EXTRA_OUTPUT: SetVariableValue(target_var, side->__PowerOutput - side->__PowerDrained); break;
    case GETPOWERTYPE_EXTRA_DRAIN:  SetVariableValue(target_var, side->__PowerDrained - side->__PowerOutput); break;
  }
}

void EvAct_GetBuildingUpgrades(int side_id, int building_group, int target_var)
{
  SetVariableValue(target_var, GetSide(side_id)->__BuildingGroupUpgradeCount[building_group]);
}

void EvAct_GetStarportStock(int side_id, int unit_type, int target_var)
{
  SetVariableValue(target_var, GetSide(side_id)->__StarportUnitTypeStock[unit_type]);
}

void EvAct_GetStarportCost(int side_id, int unit_type, int target_var)
{
  SetVariableValue(target_var, GetSide(side_id)->__StarportUnitTypeCost[unit_type]);
}

void EvAct_GetStarportPick(int side_id, int unit_type, int target_var)
{
  SetVariableValue(target_var, GetSide(side_id)->__StarportUnitTypePicked[unit_type]);
}

void EvAct_GetBuildingQueueState(int side_id, eDataType data_type, int offset, int target_var)
{
  CSide *side = GetSide(side_id);
  SetVariableValue(target_var, GetDataValue((char *)&side->__BuildingBuildQueue, data_type, offset));
}

void EvAct_GetUnitQueueState(int side_id, eDataType data_type, int offset, int queue, int queue_num, int target_var)
{
  CSide *side = GetSide(side_id);
  if (queue)
  {
    queue_num = -1;
    for (int i = 0; i < 10; i++)
    {
      int unit_type_built = side->__UnitBuildQueue[i].__type;
      if (unit_type_built == -1)
        continue;
      int prereq1_group = _templates_unitattribs[unit_type_built].__PreReq1;
      char *queue_groups = (char *)&_templates_GroupIDs;
      if (prereq1_group == queue_groups[(queue <= 6)?queue:queue+3])
      {
        queue_num = i;
        break;
      }
    }
  }
  if (queue_num >= 0)
    SetVariableValue(target_var, GetDataValue((char *)&side->__UnitBuildQueue[queue_num], data_type, offset));
  else
    SetVariableValue(target_var, -1);
}

void EvAct_GetUpgradeQueueState(int side_id, eDataType data_type, int offset, int target_var)
{
  CSide *side = GetSide(side_id);
  SetVariableValue(target_var, GetDataValue((char *)&side->__BuildingUpgradeQueue, data_type, offset));
}

void EvAct_GetSpiceHarvested(int side_id, int target_var)
{
  SetVariableValue(target_var, GetSide(side_id)->__SpiceHarvested);
}

void EvAct_GetUnitsBuilt(int side_id, int unit_type, bool total, int target_var)
{
  CSide *side = GetSide(side_id);
  if (total)
    SetVariableValue(target_var, side->__UnitsBuilt);
  else
    SetVariableValue(target_var, side->__UnitsBuiltPerType[unit_type]);
}

void EvAct_GetBuildingsBuilt(int side_id, int building_type, bool total, int target_var)
{
  CSide *side = GetSide(side_id);
  if (total)
    SetVariableValue(target_var, side->__BuildingsBuilt);
  else
    SetVariableValue(target_var, side->__BuildingsBuiltPerType[building_type]);
}

void EvAct_GetUnitsLost(int side_id, int unit_type, bool total, int target_var)
{
  CSide *side = GetSide(side_id);
  if (total)
    SetVariableValue(target_var, side->__UnitsLost);
  else
    SetVariableValue(target_var, side->__UnitsLostPerType[unit_type]);
}

void EvAct_GetBuildingsLost(int side_id, int target_var)
{
  CSide *side = GetSide(side_id);
  return SetVariableValue(target_var, side->__BuildingsLost);
}

void EvAct_GetUnitsKilled(int side_id, int enemy, int unit_type, bool total, int target_var)
{
  CSide *side = GetSide(side_id);
  if (total)
    SetVariableValue(target_var, side->__UnitsKilled);
  else
    SetVariableValue(target_var, side->__UnitsKilledPerTypeAndSide[unit_type].__kills_per_side[enemy]);
}

void EvAct_GetBuildingsKilled(int side_id, int enemy, int building_type, bool total, int target_var)
{
  CSide *side = GetSide(side_id);
  if (total)
    SetVariableValue(target_var, side->__BuildingsKilled);
  else
    SetVariableValue(target_var, side->__BuildingsKilledPerTypeAndSide[building_type].__kills_per_side[enemy]);
}

void EvAct_GetMousePosition(eGetMousePositionType what, int first_var)
{
  SetVariableValue(first_var, -1);
  SetVariableValue(first_var + 1, -1);
  switch (what)
  {
    case GETMOUSEPOSITIONTYPE_ABSOLUTE:
    {
      SetVariableValue(first_var, _gMousePos.x);
      SetVariableValue(first_var + 1, _gMousePos.y);
      break;
    }
    case GETMOUSEPOSITIONTYPE_MAPPIXEL:
    {
      if (_gMousePos.x < _ViewportWidth && _gMousePos.y > _OptionsBarHeight)
      {
        SetVariableValue(first_var, _gMousePos.x + _ViewportXPos);
        SetVariableValue(first_var + 1, _gMousePos.y + _ViewportYPos - _OptionsBarHeight);
      }
      break;
    }
    case GETMOUSEPOSITIONTYPE_MAPTILE:
    {
      if (_gMousePos.x < _ViewportWidth && _gMousePos.y > _OptionsBarHeight)
      {
        SetVariableValue(first_var, (_gMousePos.x + _ViewportXPos) / 32);
        SetVariableValue(first_var + 1, (_gMousePos.y + _ViewportYPos - _OptionsBarHeight) / 32);
      }
      break;
    }
    case GETMOUSEPOSITIONTYPE_RADARPOSITION:
    {
      int radar_image_width = 128;
      int radar_image_height = 128;
      int radar_image_left = (radar_image_width - gGameMap.width) / 2;
      int radar_image_top = (radar_image_height - gGameMap.height) / 2;
      if ( _gMousePos.x >= radar_image_left + _RadarLocationX
        && _gMousePos.x < radar_image_width + _RadarLocationX - radar_image_left
        && _gMousePos.y >= radar_image_top + _RadarLocationY
        && _gMousePos.y < radar_image_height + _RadarLocationY - radar_image_top
        && !_TacticalData.__RadarState
        && _TacticalData.__RadarOnline)
      {
        SetVariableValue(first_var, _gMousePos.x - radar_image_left - _RadarLocationX);
        SetVariableValue(first_var + 1, _gMousePos.y - radar_image_top - _RadarLocationY);
      }
      break;
    }
    case GETMOUSEPOSITIONTYPE_BUILDINGICON:
    {
      if (_TacticalData.__SidebarMode == 1 && _gMousePos.x > SideBarPanelLeftUIPosX && _gMousePos.x < SideBarPanelLeftUIPosX + _SidebarIconWidth && _gMousePos.y > SideBarPanelsPosY && _gMousePos.y < SideBarPanelsPosY + SideBarIconHeight * SideBarIconCount)
      {
        unsigned int icon_number = (_gMousePos.y - SideBarPanelsPosY) / SideBarIconHeight + _TacticalData.__Strip1ScrollPos1;
        CSide *side = GetSide(gSideId);
        if (icon_number < side->__BuildingIconCount)
          SetVariableValue(first_var, side->__BuildingIcons[icon_number]);
      }
      break;
    }
    case GETMOUSEPOSITIONTYPE_UNITICON:
    {
      if (_TacticalData.__SidebarMode == 1 && _gMousePos.x > SideBarPanelRightUIPosX && _gMousePos.x < SideBarPanelRightUIPosX + _SidebarIconWidth && _gMousePos.y > SideBarPanelsPosY && _gMousePos.y < SideBarPanelsPosY + SideBarIconHeight * SideBarIconCount)
      {
        unsigned int icon_number = (_gMousePos.y - SideBarPanelsPosY) / SideBarIconHeight + _TacticalData.__Strip2ScrollPos1;
        CSide *side = GetSide(gSideId);
        if (icon_number < side->__UnitIconCount)
          SetVariableValue(first_var, side->__UnitIcons[icon_number]);
      }
      break;
    }
    case GETMOUSEPOSITIONTYPE_STARPORTICON:
    {
      if (_TacticalData.__SidebarMode == 2 && _gMousePos.x > SideBarPanelLeftUIPosX && _gMousePos.x < SideBarPanelRightUIPosX + _SidebarIconWidth && _gMousePos.y > SideBarPanelsPosY && _gMousePos.y < SideBarPanelsPosY + SideBarIconHeight * 4)
      {
        unsigned int icon_number = 2 * ((_gMousePos.y - SideBarPanelsPosY) / SideBarIconHeight);
        if (_gMousePos.x > SideBarPanelLeftUIPosX + _SidebarIconWidth)
          icon_number++;
        CSide *side = GetSide(gSideId);
        SetVariableValue(first_var, side->__StarportIcons[icon_number]);
      }
      break;
    }
    case GETMOUSEPOSITIONTYPE_UPGRADEICON:
    {
      if (_TacticalData.__SidebarMode == 3 && _gMousePos.x > SideBarPanelLeftUIPosX && _gMousePos.x < SideBarPanelRightUIPosX + _SidebarIconWidth && _gMousePos.y > SideBarPanelsPosY && _gMousePos.y < SideBarPanelsPosY + SideBarIconHeight * 4)
      {
        unsigned int icon_number = 2 * ((_gMousePos.y - SideBarPanelsPosY) / SideBarIconHeight);
        if (_gMousePos.x > SideBarPanelLeftUIPosX + _SidebarIconWidth)
          icon_number++;
        CSide *side = GetSide(gSideId);
        int building_type = side->__UpgradeIcons[icon_number];
        if (building_type >= 0 && CanSideUpgradeBuildingGroup(gSideId, _templates_buildattribs[building_type].GroupType))
          SetVariableValue(first_var, building_type);
      }
      break;
    }
  }
}

void EvAct_GetKeyboardMouseState(int target_var, int key)
{
  SetVariableValue(target_var, _KeyboardKeyState[key]);
}

void EvAct_GetUnitUnderCursor(int side_var, int index_var, bool ignore_shroud, bool ignore_stealth)
{
  SetVariableValue(side_var, SIDE_NONE);
  SetVariableValue(index_var, -1);
  if (_gMousePos.y <= _OptionsBarHeight)
    return;
  if (_gMousePos.x >= _ViewportWidth)
    return;
  int tile_x = (_ViewportXPos + _gMousePos.x) / 32;
  int tile_y = (_ViewportYPos + _gMousePos.y - _OptionsBarHeight) / 32;
  int tile_flags = gGameMap.map[_CellNumbersWidthSpan[tile_y] + tile_x].__tile_bitflags;
  int shroud = gGameMap.map[_CellNumbersWidthSpan[tile_y] + tile_x].__shroud;
  if (!ignore_shroud && (shroud & 0xF) == 1)
    return;
  eSideType side_id = tile_flags & 7;
  _WORD index;
  GetUnitOnTile(_ViewportXPos + _gMousePos.x, _ViewportYPos + _gMousePos.y - _OptionsBarHeight, &side_id, &index, !ignore_stealth && side_id != gSideId);
  SetVariableValue(side_var, side_id);
  SetVariableValue(index_var, index);
}

void EvAct_GetBuildingUnderCursor(int side_var, int index_var, bool ignore_shroud)
{
  SetVariableValue(side_var, SIDE_NONE);
  SetVariableValue(index_var, -1);
  if (_gMousePos.y <= _OptionsBarHeight)
    return;
  if (_gMousePos.x >= _ViewportWidth)
    return;
  int tile_x = (_ViewportXPos + _gMousePos.x) / 32;
  int tile_y = (_ViewportYPos + _gMousePos.y - _OptionsBarHeight) / 32;
  int tile_flags = gGameMap.map[_CellNumbersWidthSpan[tile_y] + tile_x].__tile_bitflags;
  int shroud = gGameMap.map[_CellNumbersWidthSpan[tile_y] + tile_x].__shroud;
  if (!ignore_shroud && (shroud & 0xF) == 1)
    return;
  eSideType side_id = tile_flags & 7;
  _WORD index;
  Building *bld;
  if (GetBuildingOnTile_0(tile_x, tile_y, &bld, &side_id, &index))
  {
    SetVariableValue(side_var, side_id);
    SetVariableValue(index_var, index);
  }
}

void EvAct_GetSidebarButtonUnderCursor(int button, int target_var, bool click_on_it)
{
  SetVariableValue(target_var, HandleSidebarButton(button, click_on_it));
}

void EvAct_GetGameInterfaceData(eDataType data_type, int offset, int target_var)
{
  SetVariableValue(target_var, GetDataValue((char *)&_TacticalData, data_type, offset));
}

bool EvaluateConditionalExpression(CondExprData *cond_expr)
{
  // Evaluate results of individual operations
  bool operation_result[8];
  for (int i = 0; i < 8; i++)
  {
    if (i < cond_expr->num_operations)
    {
      int first_val = GetVariableValue(cond_expr->variable[i]);
      int second_val = GetVariableValueOrConst(cond_expr->value_var_flags, i, cond_expr->value[i]);
      int operation = (cond_expr->operators >> (i * 4)) & 15;
      switch (operation)
      {
        case 0: operation_result[i] = first_val == second_val; break;
        case 1: operation_result[i] = first_val != second_val; break;
        case 2: operation_result[i] = first_val >= second_val; break;
        case 3: operation_result[i] = first_val >  second_val; break;
        case 4: operation_result[i] = first_val <= second_val; break;
        case 5: operation_result[i] = first_val <  second_val; break;
        case 6: operation_result[i] = (first_val & (1 << second_val)) != 0; break;
        case 7: operation_result[i] = (first_val & (1 << second_val)) == 0; break;
      }
    }
    else
      operation_result[i] = true;
  }
  // Evaluate and/or expression
  char and_or[7];
  for (int i = 0; i < 7; i++)
    and_or[i] = (cond_expr->and_or >> (i * 2)) & 3;
  int totalshifts = 0;
  for (int and_or_level = 3; and_or_level >= 0; and_or_level--)
  {
    int shifts = 0;
    for (int i = 0; i < cond_expr->num_operations-1 - totalshifts; i++)
    {
      if (and_or[i - shifts] == and_or_level)
      {
        if (and_or_level & 1)
          operation_result[i - shifts] = operation_result[i - shifts] || operation_result[i - shifts + 1];
        else
          operation_result[i - shifts] = operation_result[i - shifts] && operation_result[i - shifts + 1];
        for (int j = i - shifts + 1; j < cond_expr->num_operations-1 - totalshifts; j++)
          operation_result[j] = operation_result[j+1];
        for (int j = i - shifts; j < cond_expr->num_operations-1 - totalshifts; j++)
          and_or[j] = and_or[j+1];
        shifts++;
      }
    }
    totalshifts += shifts;
  }
  return operation_result[0];
}

void EvAct_ExecuteBlock(int event_index, int target_event_index)
{
  if (_gEventArray[target_event_index].event_type == ET_CALLABLE_BLOCK_START)
    ExecuteEventBlock(target_event_index, EBT_BLOCK);
  else
    DebugFatal("event-actions.c", "Execute block: Target event must be of type Callable Block Start (event %d)", event_index);
}

void EvAct_If(int event_index, eIfConditionType condition_type, int side_var, int object_index_var, CondExprData *cond_expr)
{
  int else_event_index = gEventExtraData[event_index].else_event_index;
  int side_id = GetVariableValue(side_var);
  int object_index = GetVariableValue(object_index_var);
  bool result = false;
  switch (condition_type)
  {
    case IFCONDTYPE_EXPRESSION:           result = EvaluateConditionalExpression(cond_expr); break;
    case IFCONDTYPE_CHECK_UNIT:           result = CheckIfUnitMatchesFilter((ObjectFilterStruct *)cond_expr, &(GetSide(side_id)->__ObjectArray[object_index]), side_id); break;
    case IFCONDTYPE_CHECK_BUILDING:       result = CheckIfBuildingMatchesFilter((ObjectFilterStruct *)cond_expr, (Building *)&(GetSide(side_id)->__ObjectArray[object_index]), side_id); break;
    case IFCONDTYPE_CHECK_BULLET:         result = CheckIfBulletMatchesFilter((ObjectFilterStruct *)cond_expr, (Bullet *)&(GetSide(side_id)->__ObjectArray[object_index])); break;
    case IFCONDTYPE_CHECK_EXPLOSION:      result = CheckIfExplosionMatchesFilter((ObjectFilterStruct *)cond_expr, (Explosion *)&(GetSide(side_id)->__ObjectArray[object_index])); break;
    case IFCONDTYPE_CHECK_CRATE:          result = CheckIfCrateMatchesFilter((ObjectFilterStruct *)cond_expr, &gCrates[object_index]); break;
    case IFCONDTYPE_CHECK_TILE:
    {
      int x = object_index;
      int y = GetVariableValue(object_index_var + 1);
      result = CheckIfTileMatchesFilter((ObjectFilterStruct *)cond_expr, &gGameMap.map[x + _CellNumbersWidthSpan[y]], x, y);
      break;
    }
    case IFCONDTYPE_CHECK_SIDE:           result = CheckIfSideMatchesFilter((ObjectFilterStruct *)cond_expr, object_index); break;
    case IFCONDTYPE_CHECK_UNIT_TYPE:      result = CheckIfUnitTypeMatchesFilter((ObjectFilterStruct *)cond_expr, object_index); break;
    case IFCONDTYPE_CHECK_BUILDING_TYPE:  result = CheckIfBuildingTypeMatchesFilter((ObjectFilterStruct *)cond_expr, object_index); break;
  }
  if (result)
    ExecuteEventsInRange(event_index + 1, (else_event_index != -1)?(else_event_index):(gEventExtraData[event_index].next_event_index - 1), EBT_CONDITION);
  else if (else_event_index != -1)
  {
    if (_gEventArray[else_event_index].event_type == ET_ELSE_IF)
    {
      EventData *e = &_gEventArray[else_event_index];
      condition_type = GetVariableValueOrConst(e->arg_var_flags, 1, e->args[0]);
      side_var = GetVariableValueOrConst(e->arg_var_flags, 2, e->args[2]);
      object_index_var = GetVariableValueOrConst(e->arg_var_flags, 3, e->args[3]);
      EvAct_If(else_event_index, condition_type, side_var, object_index_var, (CondExprData *)&e->data[1]);
    }
    else if (_gEventArray[else_event_index].event_type == ET_ELSE)
      ExecuteEventsInRange(else_event_index + 1, gEventExtraData[event_index].next_event_index - 1, EBT_CONDITION);
  }
}

void EvAct_LoopWhile(int event_index, CondExprData *cond_expr)
{
  int num_cycles = 0;
  while (EvaluateConditionalExpression(cond_expr))
  {
    if (num_cycles++ >= 65536)
      DebugFatal("event-actions.c", "Infinite loop detected (event %d)", event_index);
    ExecuteEventBlock(event_index, EBT_LOOP);
    if (break_count)
    {
      break_count--;
      break;
    }
  }
}

void EvAct_LoopValuesFromRange(int event_index, int loop_var, int min_value, int max_value)
{
  for (int i = min_value; i <= max_value; i++)
  {
    SetVariableValue(loop_var, i);
    ExecuteEventBlock(event_index, EBT_LOOP);
    if (break_count)
    {
      break_count--;
      break;
    }
  }
}

void EvAct_LoopCoordsFromArea(int event_index, int min_x, int min_y, int max_x, int max_y, int first_var)
{
  for (int y = min_y; y <= max_y; y++)
    for (int x = min_x; x <= max_x; x++)
    {
      SetVariableValue(first_var, x);
      SetVariableValue(first_var + 1, y);
      ExecuteEventBlock(event_index, EBT_LOOP);
      if (break_count)
      {
        break_count--;
        break;
      }
    }
}

void EvAct_LoopValuesFromList(int event_index, int amount, int loop_var, uint8_t *value_list)
{
  for (int i = 0; i < amount; i++)
  {
    SetVariableValue(loop_var, value_list[i]);
    ExecuteEventBlock(event_index, EBT_LOOP);
    if (break_count)
    {
      break_count--;
      break;
    }
  }
}

void EvAct_LoopCoordsFromList(int event_index, int amount, int first_var, uint8_t *value_list)
{
  for (int i = 0; i < amount; i++)
  {
    SetVariableValue(first_var,     value_list[i * 2 + 1]);
    SetVariableValue(first_var + 1, value_list[i * 2 + 2]);
    ExecuteEventBlock(event_index, EBT_LOOP);
    if (break_count)
    {
      break_count--;
      break;
    }
  }
}

void EvAct_LoopAreasFromList(int event_index, int amount, int first_var, uint8_t *value_list)
{
  for (int i = 0; i < amount; i++)
  {
    SetVariableValue(first_var,     value_list[i * 4 + 1]);
    SetVariableValue(first_var + 1, value_list[i * 4 + 2]);
    SetVariableValue(first_var + 2, value_list[i * 4 + 3]);
    SetVariableValue(first_var + 3, value_list[i * 4 + 4]);
    ExecuteEventBlock(event_index, EBT_LOOP);
    if (break_count)
    {
      break_count--;
      break;
    }
  }
}

void EvAct_LoopObject(int event_index, int side_var, int index_var, int side_id, int object_index)
{
  SetVariableValue(side_var, side_id);
  SetVariableValue(index_var, object_index);
  ExecuteEventBlock(event_index, EBT_LOOP);
}

void EvAct_LoopItem(int event_index, int index_var, int object_index)
{
  SetVariableValue(index_var, object_index);
  ExecuteEventBlock(event_index, EBT_LOOP);
}

void EvAct_LoopTiles(int event_index, int first_var, int xpos, int ypos)
{
  SetVariableValue(first_var, xpos);
  SetVariableValue(first_var + 1, ypos);
  ExecuteEventBlock(event_index, EBT_LOOP);
}
