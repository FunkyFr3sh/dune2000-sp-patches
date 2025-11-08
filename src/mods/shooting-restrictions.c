#include "macros/patch.h"
#include "dune2000.h"
#include "extended-tileset.h"

const int restrictions_check[32] =
{
  0x00110000,
  0x10010000,
  0x10010000,
  0x10010000,
  0x10010000,
  0x10010000,
  0x10010000,
  0x10010000,
  0x11000000,
  0x01000001,
  0x01000001,
  0x01000001,
  0x01000001,
  0x01000001,
  0x01000001,
  0x01000001,
  0x00000011,
  0x00000110,
  0x00000110,
  0x00000110,
  0x00000110,
  0x00000110,
  0x00000110,
  0x00000110,
  0x00001100,
  0x00101000,
  0x00101000,
  0x00101000,
  0x00101000,
  0x00101000,
  0x00101000,
  0x00101000
};

bool CheckLineOfSight(int x1, int y1, int x2, int y2, int restriction_type)
{
  int start_x = x1 / 32;
  int start_y = y1 / 32;
  int end_x = x2 / 32;
  int end_y = y2 / 32;
  if (abs(start_x - end_x) <= 1 && abs(start_y - end_y) <= 1)
    return true;
  int angle = GetFacing(x1, y1, x2, y2);
  int current_x;
  int current_y;
  int *current_x_ptr = &current_x;
  int *current_y_ptr = &current_y;
  if (x2 < x1)
  {
    int tmp_x = x2;
    x2 = x1;
    x1 = tmp_x;
    int tmp_y = y2;
    y2 = y1;
    y1 = tmp_y;
  }
  int step_y = 1;
  if (y2 < y1)
    step_y = -1;
  int dx = x2 - x1;
  int dy = (y2 - y1) * step_y;
  if (dy > dx)
  {
    int tmp_1 = x1;
    x1 = y1;
    y1 = tmp_1;
    int tmp_2 = x2;
    x2 = y2;
    y2 = tmp_2;
    current_x_ptr = &current_y;
    current_y_ptr = &current_x;
    if (x2 < x1)
    {
      int tmp_x = x2;
      x2 = x1;
      x1 = tmp_x;
      int tmp_y = y2;
      y2 = y1;
      y1 = tmp_y;
    }
    step_y = 1;
    if (y2 < y1)
      step_y = -1;
    dx = x2 - x1;
    dy = (y2 - y1) * step_y;
  }
  int dx2 = dx * 2;
  int dy2 = dy * 2;
  int predictor = dy2 - dx;
  current_y = y1;
  for (current_x = x1; current_x <= x2; current_x++)
  {
    int center_x = *current_x_ptr;
    int center_y = *current_y_ptr;
    int shifted_x = (center_x + _sinValues[4 + ((16 - angle + 8) & 31) * 512] / 2048) / 32;
    int shifted_y = (center_y + _cosValues[4 + ((16 - angle + 8) & 31) * 512] / 2048) / 32;
    if (!(shifted_x == start_x && shifted_y == start_y) && !(shifted_x == end_x && shifted_y == end_y))
    {
      int current_tile_restrictions = tile_restrictions[gGameMap.map[_CellNumbersWidthSpan[shifted_y] + shifted_x].back_up_tile_index];
      if (current_tile_restrictions & (restrictions_check[angle] << restriction_type))
        return false;
    }
    shifted_x = (center_x + _sinValues[4 + ((16 - angle - 8) & 31) * 512] / 2048) / 32;
    shifted_y = (center_y + _cosValues[4 + ((16 - angle - 8) & 31) * 512] / 2048) / 32;
    if (!(shifted_x == start_x && shifted_y == start_y) && !(shifted_x == end_x && shifted_y == end_y))
    {
      int current_tile_restrictions = tile_restrictions[gGameMap.map[_CellNumbersWidthSpan[shifted_y] + shifted_x].back_up_tile_index];
      if (current_tile_restrictions & (restrictions_check[angle] << restriction_type))
        return false;
    }
    if (predictor >= 0)
    {
      current_y += step_y;
      predictor = predictor + dy2 - dx2;
    }
    else
    {
      predictor = predictor + dy2;
    }
  }
  return true;
}

// Custom implementation of function EnemyUnitInRange
DETOUR(0x004A4900, 0x004A4FCF, _Mod__EnemyUnitInRange);

char Mod__EnemyUnitInRange(int xpos, int ypos, eSideType side_id, int radius, int *target_xpos_ptr, int *target_ypos_ptr, _BYTE *target_side_id_ptr, short *target_index_ptr, char primary_weapon_type, char secondary_weapon_type, char a11, char a12, char a13)
{
  int v13; // edi
  char v14; // cl
  int v15; // eax
  unsigned char v16[4]; // ebx
  int v17; // esi
  char v18; // dl
  TileFlags tile_flags; // eax
  int v20; // esi
  char secondary_weapon_type_; // dl
  int primary_weapon_type_; // eax
  char primary_weapon_antiaircraft; // cl
  int secondary_weapon_type___; // ecx
  unsigned int primary_weapon_range; // eax
  unsigned int secondary_weapon_range; // ecx
  unsigned short v29; // di
  unsigned char v30; // cl
  int v31; // eax
  int v32; // edx
  bool v33; // zf
  Unit *unit; // esi MAPDST
  unsigned int range_; // edi
  int secondary_weapon_type__; // edx
  unsigned char v39; // bl
  char v40; // cl
  int v41; // ebp
  short v43; // dx
  unsigned char v44; // [esp+10h] [ebp-24h]
  unsigned char v45; // [esp+10h] [ebp-24h]
  char v46; // [esp+10h] [ebp-24h]
  eSideType side_id_ptr; // [esp+11h] [ebp-23h]
  short index_ptr; // [esp+12h] [ebp-22h]
  unsigned char v49; // [esp+14h] [ebp-20h]
  unsigned char v50; // [esp+18h] [ebp-1Ch]
  unsigned char v51; // [esp+1Ch] [ebp-18h]
  int range; // [esp+20h] [ebp-14h]
  unsigned char v53; // [esp+24h] [ebp-10h]
  int v54; // [esp+28h] [ebp-Ch]
  int v55; // [esp+2Ch] [ebp-8h]
  int v56; // [esp+30h] [ebp-4h]
  unsigned int enemy_side_id; // [esp+60h] [ebp+2Ch] MAPDST

  int restriction_type;
  *target_index_ptr = -1;
  if ( primary_weapon_type == -1 )
  {
    DebugFatal("EnemyUnitInRange", "Primary bullet invalid");
  }
  v13 = radius;
  if ( radius )
  {
    range = radius;
    restriction_type = 3;
  }
  else if ( secondary_weapon_type == -1 )
  {
    range = _templates_bulletattribs[(int)primary_weapon_type].__Range;
    restriction_type = (_templates_bulletattribs[(int)primary_weapon_type].Flags & BULFLAGS_1000_BLOCKED_BY_WALL)?1:2;
  }
  else if ( (unsigned int)_templates_bulletattribs[(int)primary_weapon_type].__Range <= (unsigned int)_templates_bulletattribs[(int)secondary_weapon_type].__Range )
  {
    range = _templates_bulletattribs[(int)secondary_weapon_type].__Range;
    restriction_type = (_templates_bulletattribs[(int)secondary_weapon_type].Flags & BULFLAGS_1000_BLOCKED_BY_WALL)?1:2;
  }
  else
  {
    range = _templates_bulletattribs[(int)primary_weapon_type].__Range;
    restriction_type = (_templates_bulletattribs[(int)primary_weapon_type].Flags & BULFLAGS_1000_BLOCKED_BY_WALL)?1:2;
  }
  v51 = 1;
  v44 = 1;
  v50 = 0;
  v56 = ypos / 32;
  v14 = ypos / 32;
  v15 = 2 * range / 32;
  v16[0] = xpos / 32;
  v17 = v15 + 2;
  v55 = xpos / 32;
  v54 = v15 + 2;
  if ( v15 + 2 > 1 )
  {
    while ( 1 )
    {
      v16[0] += _offsets_4EB2D8[0][v50];
      v18 = _offsets_4EB2D8[1][v50];
      v53 = v16[0];
      v49 = v18 + v14;
      if ( !--v44 )
      {
        v51 += _offsets_4EB2D8[2][v50];
        v44 = v51;
        v50 = (v50 + 1) & 3;
      }
      if ( v53 < gGameMap.width && v49 < gGameMap.height )
      {
        tile_flags = gGameMap.map[v53 + _CellNumbersWidthSpan[v49]].__tile_bitflags;
        if ( tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_8_OCC_UNIT) )
        {
          if ( _gDiplomacy[side_id][gGameMap.map[v53 + _CellNumbersWidthSpan[v49]].__tile_bitflags & 7] == 1 )
          {
            v13 = v49;
            v17 = v16[0];
            v20 = 32 * v17 + 16;
            v13 = 32 * v13 + 16;
            unit = GetUnitOnTile((unsigned short)v20, (unsigned short)v13, &side_id_ptr, &index_ptr, a11 == 0);
            if ( unit && unit->State != 17 && CheckDistance(xpos, ypos, v20, v13, range) && CheckLineOfSight(xpos, ypos, v20, v13, restriction_type) )
            {
              *target_xpos_ptr = unit->__PosX / 0x10000;
              v43 = index_ptr;
              *target_ypos_ptr = unit->__PosY / 0x10000;
              *target_index_ptr = v43;
              *target_side_id_ptr = side_id_ptr;
              return 1;
            }
            v17 = v54;
          }
        }
      }
      if ( v51 >= v17 )
      {
        v16[0] = v55;
        v13 = radius;
        break;
      }
      v14 = v49;
    }
  }
  if ( v13 )
  {
    return 0;
  }
  if ( !a12 )
  {
    goto LABEL_62;
  }
  secondary_weapon_type_ = secondary_weapon_type;
  primary_weapon_type_ = primary_weapon_type;
  primary_weapon_antiaircraft = _templates_bulletattribs[primary_weapon_type_].AntiAircraft;
  if ( !primary_weapon_antiaircraft
    && (secondary_weapon_type == -1 || !_templates_bulletattribs[(int)secondary_weapon_type].AntiAircraft) )
  {
LABEL_63:
    if ( a13 )
    {
      if ( secondary_weapon_type_ == -1 )
      {
        range_ = _templates_bulletattribs[(int)primary_weapon_type].__Range;
        restriction_type = (_templates_bulletattribs[(int)primary_weapon_type].Flags & BULFLAGS_1000_BLOCKED_BY_WALL)?1:2;
      }
      else
      {
        secondary_weapon_type__ = secondary_weapon_type_;
        range_ = _templates_bulletattribs[(int)primary_weapon_type].__Range;
        restriction_type = (_templates_bulletattribs[(int)primary_weapon_type].Flags & BULFLAGS_1000_BLOCKED_BY_WALL)?1:2;
        if ( range_ <= (unsigned int)_templates_bulletattribs[secondary_weapon_type__].__Range )
        {
          range_ = _templates_bulletattribs[secondary_weapon_type__].__Range;
          restriction_type = (_templates_bulletattribs[(int)secondary_weapon_type__].Flags & BULFLAGS_1000_BLOCKED_BY_WALL)?1:2;
        }
      }
      v39 = v55;
      v49 = v56;
      v40 = 1;
      v51 = 1;
      v46 = 1;
      v50 = 0;
      v41 = (signed int)(2 * range_) / 32 + 2;
      if ( v41 > 1 )
      {
        while ( 1 )
        {
          v39 += _offsets_4EB2D8[0][v50];
          v49 += _offsets_4EB2D8[1][v50];
          v33 = v46 == 1;
          v53 = v39;
          --v46;
          if ( v33 )
          {
            v40 += _offsets_4EB2D8[2][v50];
            v51 = v40;
            v46 = v40;
            v50 = (v50 + 1) & 3;
          }
          if ( v53 < gGameMap.width
            && v49 < gGameMap.height
            && gGameMap.map[v53 + _CellNumbersWidthSpan[v49]].__tile_bitflags & TileFlags_1000000_OCC_SANDWORM )
          {
            break;
          }
LABEL_84:
          if ( v51 >= v41 )
          {
            return 0;
          }
        }
        enemy_side_id = 0;
        while ( 1 )
        {
          unit = GetSide(enemy_side_id)->__FirstUnitPtr;
          if ( unit )
          {
            break;
          }
LABEL_82:
          if ( ++enemy_side_id >= 8 )
          {
            v40 = v51;
            goto LABEL_84;
          }
        }
        while ( unit->State != UNIT_STATE_27_WANDERING
             || unit->Flags & UFLAGS_4_CLOAKED
             || unit->BlockFromX != v39
             || unit->BlockFromY != v49
             || !(CheckDistance(xpos, ypos, 32 * v39 + 16, 32 * v49 + 16, range_) && CheckLineOfSight(xpos, ypos, 32 * v39 + 16, 32 * v49 + 16, restriction_type)) )
        {
          unit = unit->Next;
          if ( !unit )
          {
            goto LABEL_82;
          }
        }
        *target_xpos_ptr = unit->__PosX / 0x10000;
        *target_ypos_ptr = unit->__PosY / 0x10000;
        *target_index_ptr = unit->MyIndex;
        *target_side_id_ptr = enemy_side_id;
        return 1;
      }
    }
    return 0;
  }
  if ( secondary_weapon_type != -1
    && primary_weapon_antiaircraft
    && (secondary_weapon_type___ = secondary_weapon_type, _templates_bulletattribs[secondary_weapon_type___].AntiAircraft) )
  {
    primary_weapon_range = _templates_bulletattribs[primary_weapon_type_].__Range;
    secondary_weapon_range = _templates_bulletattribs[secondary_weapon_type___].__Range;
    if ( primary_weapon_range <= secondary_weapon_range )
    {
      range = secondary_weapon_range;
      goto LABEL_38;
    }
  }
  else
  {
    if ( !_templates_bulletattribs[primary_weapon_type_].AntiAircraft )
    {
      goto LABEL_38;
    }
    primary_weapon_range = _templates_bulletattribs[primary_weapon_type_].__Range;
  }
  range = primary_weapon_range;
LABEL_38:
  v29 = range;
  v30 = v56;
  v51 = 1;
  v45 = 1;
  v50 = 0;
  v31 = 2 * range / 32;
  v32 = v31 + 2;
  v54 = v31 + 2;
  if ( v31 + 2 <= 1 )
  {
LABEL_62:
    secondary_weapon_type_ = secondary_weapon_type;
    goto LABEL_63;
  }
  while ( 1 )
  {
    v16[0] += _offsets_4EB2D8[0][v50];
    v49 = _offsets_4EB2D8[1][v50] + v30;
    v33 = v45 == 1;
    v53 = v16[0];
    --v45;
    if ( v33 )
    {
      v51 += _offsets_4EB2D8[2][v50];
      v45 = v51;
      v50 = (v50 + 1) & 3;
    }
    if ( v53 < gGameMap.width
      && v49 < gGameMap.height
      && (unsigned int)&gSideArray[2].__ObjectArray[726].__dw_field_88_ptr & gGameMap.map[v53
                                                                                        + _CellNumbersWidthSpan[v49]].__tile_bitflags )
    {
      break;
    }
LABEL_59:
    if ( v51 >= v32 )
    {
      goto LABEL_62;
    }
    v30 = v49;
  }
  enemy_side_id = 0;
  enemy_side_id = 0;
  while ( 1 )
  {
    if ( _gDiplomacy[side_id][enemy_side_id] == 1 )
    {
      unit = GetSide(enemy_side_id)->__FirstUnitPtr;
      if ( unit )
      {
        break;
      }
    }
LABEL_57:
    if ( ++enemy_side_id >= 8 )
    {
      v32 = v54;
      goto LABEL_59;
    }
  }
  while ( !(unit->Flags & UFLAGS_40_FLYING)
       || unit->Type == _templates_GroupIDs.Frigate
       || (unit->BlockToX != v16[0] && unit->BlockFromX != v16[0])
       || (unit->BlockFromY != v49 && unit->BlockToY != v49)
       || !CheckDistance(xpos, ypos, 32 * v16[0] + 16, 32 * v49 + 16, v29) )
  {
    unit = unit->Next;
    if ( !unit )
    {
      goto LABEL_57;
    }
  }
  *target_xpos_ptr = unit->__PosX / 0x10000;
  *target_ypos_ptr = unit->__PosY / 0x10000;
  *target_index_ptr = unit->MyIndex;
  *target_side_id_ptr = enemy_side_id;
  return 1;
}

// Custom implementation of function EnemyBuildingInRange
DETOUR(0x004A4FD0, 0x004A51FC, _Mod__EnemyBuildingInRange);

char Mod__EnemyBuildingInRange(int xpos, int ypos, eSideType side_id, _DWORD *target_xpos_ptr, _DWORD *target_ypos_ptr, char *target_side_id_ptr, short *target_index_ptr, char primary_weapon_type, char secondary_weapon_type, bool isai)
{
  unsigned int primary_weapon_range; // ecx
  unsigned int secondary_weapon_range; // eax
  unsigned int range; // ebp
  int v13; // eax
  unsigned char v14[4]; // ebx
  int v15; // edi
  short v16[2]; // esi
  TileFlags tile_flags; // eax
  BuildingBehaviorType behavior; // al
  int v19; // edi
  int v20; // esi
  unsigned char v21; // dl
  bool v22; // zf
  short v24; // ax
  char v25; // dl
  short index; // [esp+12h] [ebp-1Ah]
  unsigned char tile_y[4]; // [esp+14h] [ebp-18h]
  unsigned char v28; // [esp+18h] [ebp-14h]
  unsigned char v29; // [esp+1Ch] [ebp-10h]
  Building *building; // [esp+20h] [ebp-Ch]
  unsigned char tile_x[4]; // [esp+24h] [ebp-8h]
  int v32; // [esp+28h] [ebp-4h]
  char bullet1a; // [esp+4Ch] [ebp+20h]

  int restriction_type;
  *target_index_ptr = -1;
  *target_side_id_ptr = 0;
  primary_weapon_range = _templates_bulletattribs[(int)primary_weapon_type].__Range;
  building = 0;
  if ( secondary_weapon_type == -1 )
  {
    secondary_weapon_range = 0;
  }
  else
  {
    secondary_weapon_range = _templates_bulletattribs[(int)secondary_weapon_type].__Range;
    restriction_type = (_templates_bulletattribs[(int)secondary_weapon_type].Flags & BULFLAGS_1000_BLOCKED_BY_WALL)?1:2;
  }
  range = secondary_weapon_range;
  if ( secondary_weapon_range <= primary_weapon_range )
  {
    range = primary_weapon_range;
    restriction_type = (_templates_bulletattribs[(int)primary_weapon_type].Flags & BULFLAGS_1000_BLOCKED_BY_WALL)?1:2;
  }
  v29 = 1;
  bullet1a = 1;
  v28 = 0;
  tile_y[0] = ypos / 32;
  v13 = (signed int)(2 * range) / 32;
  v14[0] = xpos / 32;
  v15 = v13 + 2;
  tile_x[0] = xpos / 32;
  v32 = v13 + 2;
  if ( v13 + 2 > 1 )
  {
    while ( 1 )
    {
      if ( tile_x[0] < gGameMap.width && tile_y[0] < gGameMap.height )
      {
        tile_flags = gGameMap.map[tile_x[0] + _CellNumbersWidthSpan[tile_y[0]]].__tile_bitflags;
        if ( tile_flags & TileFlags_10_OCC_BUILDING )
        {
          if ( _gDiplomacy[side_id][gGameMap.map[tile_x[0] + _CellNumbersWidthSpan[tile_y[0]]].__tile_bitflags & 7] == 1
            && GetBuildingOnTile_0(
                 tile_x[0],
                 tile_y[0],
                 &building,
                 (eSideType *)&secondary_weapon_type,
                 &index) )
          {
            behavior = _templates_buildattribs[building->Type].__Behavior;
            if ( behavior != BuildingBehavior_WALL && (!isai || behavior == BuildingBehavior_TURRET) )
            {
              v15 = tile_y[0];
              v16[0] = v14[0];
              v19 = 32 * v15 + 16;
              v20 = 32 * v16[0] + 16;
              if ( CheckDistance(xpos, ypos, v20, v19, range) && CheckLineOfSight(xpos, ypos, v20, v19, restriction_type) )
              {
                v24 = index;
                *target_xpos_ptr = (unsigned short)v20;
                *target_ypos_ptr = (unsigned short)v19;
                v25 = secondary_weapon_type;
                *target_index_ptr = v24;
                *target_side_id_ptr = v25;
                return 1;
              }
              v15 = v32;
            }
          }
        }
      }
      v14[0] += _offsets_4EB2D8[0][v28];
      v21 = _offsets_4EB2D8[1][v28] + tile_y[0];
      v22 = bullet1a == 1;
      tile_x[0] = v14[0];
      tile_y[0] = v21;
      --bullet1a;
      if ( v22 )
      {
        v29 += _offsets_4EB2D8[2][v28];
        bullet1a = v29;
        v28 = (v28 + 1) & 3;
      }
      if ( v29 >= v15 )
      {
        return 0;
      }
    }
  }
  return 0;
}
