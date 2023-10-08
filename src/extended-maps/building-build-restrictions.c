#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"

// Custom implementation of function GetOwnershipStatusOfCell
LJMP(0x00428C30, _Mod__GetOwnershipStatusOfCell);

void Mod__GetOwnershipStatusOfCell(int x, int y, unsigned char side_id, _BYTE *flags)
{
  unsigned int tile_flags; // eax

  if ( x >= 0 && x < gGameMap.width && y >= 0 && y < gGameMap.height )
  {
    tile_flags = gGameMap.map[x + _CellNumbersWidthSpan[y]].__tile_bitflags;
    // New logic - only tile with Build On flag provides build radius
    bool tile_buildable = (_TileBitflags[gGameMap.map[x + _CellNumbersWidthSpan[y]].back_up_tile_index] & TileFlags_8000_BUILD_ON) == TileFlags_8000_BUILD_ON;
    if ( (tile_flags & TileFlags_10_OCC_BUILDING) && tile_buildable )
    {
      if ( (tile_flags & 7) == side_id )
      {
        *flags |= 1u;
      }
    }
    if ( tile_flags & TileFlags_800_HAS_CONCRETE )
    {
      if ( ((tile_flags >> 17) & 7) == side_id )
      {
        *flags |= 2u;
      }
    }
  }
}

bool CheckTerrainRestriction(int tile_flags, int default_flag, int restriction);

CALL(0x00428D16, _Mod__HandleBuildingPlacement); // BlitGame

bool Mod__HandleBuildingPlacement(eSideType side_id, int tile_bitfield, int tile_bitfield_solid, TImage *img)
{
  unsigned int v5; // esi
  int tile_y; // edx
  int tile_x; // ecx
  int check_y; // edi
  int check_min_x; // ebx
  int check_max_x; // ebp
  int check_x; // esi
  bool v12; // zf
  int draw_at_y; // ebx
  int *v14; // ebp
  int draw_at_x; // edi
  int step_x; // esi
  int tile_flags; // eax
  TImage *image_to_draw; // eax
  unsigned char flags; // [esp+4h] [ebp-3Ch]
  bool result; // [esp+5h] [ebp-3Bh]
  char tile_out_of_map; // [esp+6h] [ebp-3Ah]
  char v22; // [esp+7h] [ebp-39h]
  int tile_bit; // [esp+8h] [ebp-38h]
  int mouse_tile_y; // [esp+Ch] [ebp-34h]
  int check_max_y; // [esp+10h] [ebp-30h]
  int v26; // [esp+14h] [ebp-2Ch]
  int v27; // [esp+18h] [ebp-28h]
  int v28; // [esp+1Ch] [ebp-24h]
  int v29; // [esp+20h] [ebp-20h]
  int v30; // [esp+24h] [ebp-1Ch]
  int mouse_tile_x; // [esp+28h] [ebp-18h]
  int draw_at_x_base; // [esp+2Ch] [ebp-14h]
  RECT rect; // [esp+30h] [ebp-10h]
  int tile_y_; // [esp+44h] [ebp+4h]

  (void)tile_bitfield_solid;
  result = 1;
  tile_bit = 1;
  v22 = 0;
  rect.left = 0;
  rect.top = _OptionsBarHeight;
  rect.right = _ViewportWidth;
  rect.bottom = _OptionsBarHeight + _ViewportHeight;
  if ( _gMousePos.y < _OptionsBarHeight )
  {
    return 0;
  }

  // New logic start
  // Get building type and its build restrictions
  int building_type = GetSide(side_id)->__BuildingBuildQueue.__type;
  BuildingAtrbStruct *building_template = &_templates_buildattribs[building_type];
  int build_restriction = building_template->BuildRestriction;
  int max_build_distance = building_template->BuildMaxDistance?building_template->BuildMaxDistance:2;
  // New logic end

  v5 = _gMousePos.y - _OptionsBarHeight + _ViewportYPos;
  mouse_tile_x = (_ViewportXPos + _gMousePos.x) / 32;
  mouse_tile_y = v5 >> 5;
  draw_at_x_base = _gMousePos.x
                 - (((((_ViewportXPos + _gMousePos.x) >> 31) ^ abs(_ViewportXPos + LOBYTE(_gMousePos.x))) & 0x1F)
                  - ((_ViewportXPos + _gMousePos.x) >> 31));
  v30 = _gMousePos.y - (v5 & 0x1F) - _OptionsBarHeight;

  tile_y = mouse_tile_y;
  tile_out_of_map = 0;
  flags = 0;
  v28 = mouse_tile_y;
  v29 = 4;
  do
  {
    tile_x = mouse_tile_x;
    v27 = 4;
    v26 = mouse_tile_x;
    do
    {
      if ( tile_bit & tile_bitfield )
      {
        if ( tile_x >= gGameMap.width || tile_y >= gGameMap.height )
        {
          tile_out_of_map = 1;
        }
        else
        {
          // New logic start
          // Take max build distance into account
          check_y = (tile_y - max_build_distance) & ((tile_y - max_build_distance <= 0) - 1);
          check_min_x = (tile_x - max_build_distance) & ((tile_x - max_build_distance <= 0) - 1);
          check_max_y = tile_y + max_build_distance + 1;
          if ( tile_y + max_build_distance + 1 >= gGameMap.height )
          {
            check_max_y = gGameMap.height;
          }
          check_max_x = tile_x + max_build_distance + 1;
          if ( tile_x + max_build_distance + 1 >= gGameMap.width )
          {
            check_max_x = gGameMap.width;
          }
          // New logic end
          for ( ; check_y < check_max_y; ++check_y )
          {
            check_x = check_min_x;
            if ( check_min_x < check_max_x )
            {
              do
              {
                // New logic start
                // Call modded version of GetOwnershipStatusOfCell
                Mod__GetOwnershipStatusOfCell(check_x++, check_y, side_id, &flags);
                // New logic end
              }
              while ( check_x < check_max_x );
              tile_y = v28;
              tile_x = v26;
            }
          }
        }
      }
      ++tile_x;
      tile_bit *= 2;
      v12 = v27 == 1;
      v26 = tile_x;
      --v27;
    }
    while ( !v12 );
    ++tile_y;
    v12 = v29 == 1;
    v28 = tile_y;
    --v29;
  }
  while ( !v12 );
  if ( flags )
  {
    if ( !tile_out_of_map )
    {
      v22 = 1;
    }
  }
  draw_at_y = v30;
  tile_bit = 1;
  tile_y_ = mouse_tile_y;
  v14 = &_CellNumbersWidthSpan[mouse_tile_y];
  v29 = 4;
  do
  {
    draw_at_x = draw_at_x_base;
    step_x = 0;
    v30 = 4;
    do
    {
      if ( tile_bit & tile_bitfield )
      {
        tile_flags = gGameMap.map[*v14 + step_x + mouse_tile_x].__tile_bitflags;

        if ( mouse_tile_x + step_x < gGameMap.width
          && tile_y_ < gGameMap.height
          // Mod - Don't allow building on tiles with spice
          && (!(tile_flags & (TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE|TileFlags_1000_HAS_CRATE|TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_10_OCC_BUILDING|TileFlags_8_OCC_UNIT)))
          // New logic - Perform check for tile flags according to build restriction
          && CheckTerrainRestriction(tile_flags, TileFlags_8000_BUILD_ON, build_restriction)
          && v22
          && flags & 3 )
        {
          if ( tile_flags & TileFlags_800_HAS_CONCRETE )
          {
            image_to_draw = _image_placement_marker_buildable_concrete;
          }
          else
          {
            image_to_draw = _image_placement_marker_buildable;
          }
        }
        else
        {
          image_to_draw = _image_placement_marker_nonbuildable;
          result = 0;
        }
        BlitClipTImage2(img, &rect, draw_at_x, draw_at_y + _OptionsBarHeight, image_to_draw, 1, 1);
      }
      ++step_x;
      draw_at_x += 32;
      v12 = v30 == 1;
      tile_bit *= 2;
      --v30;
    }
    while ( !v12 );
    ++v14;
    draw_at_y += 32;
    v12 = v29 == 1;
    ++tile_y_;
    --v29;
  }
  while ( !v12 );
  return result;
}

CALL(0x00428CF9, _Mod__HandleConcretePlacement); // BlitGame

bool Mod__HandleConcretePlacement(int tile_bitfield1, int tile_bitfield2, TImage *img, char bool1)
{
  bool result; // al
  unsigned int v5; // esi
  int v6; // edx
  int v7; // ecx
  int check_y; // edi
  int v9; // ebx
  int check_max_x; // ebp
  int check_x; // esi
  bool v12; // zf
  int draw_at_y; // edi
  TImage *img_; // ebp
  int *v15; // ecx
  int draw_at_x; // esi
  int step_x; // ebx
  TileFlags tile_flags; // eax
  bool v19; // [esp+13h] [ebp-3Dh]
  unsigned char flags; // [esp+14h] [ebp-3Ch]
  char v21; // [esp+15h] [ebp-3Bh]
  char v22; // [esp+16h] [ebp-3Ah]
  bool v23; // [esp+17h] [ebp-39h]
  int v24; // [esp+18h] [ebp-38h]
  int pos_y; // [esp+1Ch] [ebp-34h]
  int check_max_y; // [esp+20h] [ebp-30h]
  int v27; // [esp+24h] [ebp-2Ch]
  int v28; // [esp+28h] [ebp-28h]
  int v29; // [esp+2Ch] [ebp-24h]
  int v30; // [esp+30h] [ebp-20h]
  int v31; // [esp+34h] [ebp-1Ch]
  int pos_x; // [esp+38h] [ebp-18h]
  int a2; // [esp+3Ch] [ebp-14h]
  RECT rect; // [esp+40h] [ebp-10h]
  TImage *image1; // [esp+5Ch] [ebp+Ch]

  v19 = 1;
  v23 = 0;
  v24 = 1;
  v22 = 0;
  rect.left = 0;
  rect.top = _OptionsBarHeight;
  rect.right = _ViewportWidth;
  rect.bottom = _OptionsBarHeight + _ViewportHeight;
  if ( _gMousePos.y < _OptionsBarHeight )
  {
    return 0;
  }

  // New logic start
  // Get building type and its build restrictions
  int building_type = GetSide(gSideId)->__BuildingBuildQueue.__type;
  BuildingAtrbStruct *building_template = &_templates_buildattribs[building_type];
  int build_restriction = building_template->BuildRestriction;
  int max_build_distance = building_template->BuildMaxDistance?building_template->BuildMaxDistance:2;
  // New logic end

  v5 = _gMousePos.y - _OptionsBarHeight + _ViewportYPos;
  pos_x = (_ViewportXPos + _gMousePos.x) / 32;
  pos_y = v5 >> 5;
  a2 = _gMousePos.x
     - (((((_ViewportXPos + _gMousePos.x) >> 31) ^ abs(_ViewportXPos + LOBYTE(_gMousePos.x))) & 0x1F)
      - ((_ViewportXPos + _gMousePos.x) >> 31));
  v31 = _gMousePos.y - (v5 & 0x1F) - _OptionsBarHeight;
  v6 = pos_y;
  v21 = 0;
  flags = 0;
  v29 = pos_y;
  v30 = 4;
  do
  {
    v7 = pos_x;
    v28 = 4;
    v27 = pos_x;
    do
    {
      if ( v24 & tile_bitfield1 )
      {
        if ( v7 >= gGameMap.width || v6 >= gGameMap.height )
        {
          v21 = 1;
        }
        else
        {
          // New logic start
          // Take max build distance into account
          check_y = (v6 - max_build_distance) & ((v6 - max_build_distance <= 0) - 1);
          v9 = (v7 - max_build_distance) & ((v7 - max_build_distance <= 0) - 1);
          check_max_y = v6 + max_build_distance + 1;
          if ( v6 + max_build_distance + 1 >= gGameMap.height )
          {
            check_max_y = gGameMap.height;
          }
          check_max_x = v7 + max_build_distance + 1;
          if ( v7 + max_build_distance + 1 >= gGameMap.width )
          {
            check_max_x = gGameMap.width;
          }
          // New logic end
          for ( ; check_y < check_max_y; ++check_y )
          {
            check_x = v9;
            if ( v9 < check_max_x )
            {
              do
              {
                // New logic start
                // Call modded version of GetOwnershipStatusOfCell
                Mod__GetOwnershipStatusOfCell(check_x++, check_y, gSideId, &flags);
                // New logic end
              }
              while ( check_x < check_max_x );
              v6 = v29;
              v7 = v27;
            }
          }
        }
      }
      ++v7;
      v24 *= 2;
      v12 = v28 == 1;
      v27 = v7;
      --v28;
    }
    while ( !v12 );
    ++v6;
    v12 = v30 == 1;
    v29 = v6;
    --v30;
  }
  while ( !v12 );
  if ( flags )
  {
    if ( !v21 )
    {
      v22 = 1;
    }
  }
  draw_at_y = v31;
  img_ = img;
  v24 = 1;
  v15 = &_CellNumbersWidthSpan[pos_y];
  check_max_y = pos_y;
  pos_y = (int)&_CellNumbersWidthSpan[pos_y];
  v31 = 4;
  do
  {
    draw_at_x = a2;
    step_x = 0;
    image1 = (TImage *)4;
    while ( 1 )
    {
      if ( !(v24 & tile_bitfield1) )
      {
        goto LABEL_48;
      }
      if ( pos_x + step_x >= gGameMap.width || check_max_y >= gGameMap.height )
      {
        goto LABEL_47;
      }
      tile_flags = gGameMap.map[*v15 + step_x + pos_x].__tile_bitflags;
      // Mod - Don't allow concrete on tiles with spice
      if ( tile_flags & (TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE|TileFlags_1000_HAS_CRATE|TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_10_OCC_BUILDING|TileFlags_8_OCC_UNIT)
        || (bool1 && tile_flags & TileFlags_800_HAS_CONCRETE)
        // New logic - Perform check for tile flags according to build restriction
        || !CheckTerrainRestriction(tile_flags, TileFlags_8000_BUILD_ON, build_restriction)
        || !v22 )
      {
        BlitClipTImage2(
          img_,
          &rect,
          draw_at_x,
          draw_at_y + _OptionsBarHeight,
          _image_placement_marker_nonbuildable,
          1,
          1);
LABEL_47:
        v19 = 0;
        goto LABEL_48;
      }
      if ( tile_flags & TileFlags_800_HAS_CONCRETE || !(v24 & tile_bitfield2) )
      {
        BlitClipTImage2(
          img_,
          &rect,
          draw_at_x,
          draw_at_y + _OptionsBarHeight,
          _image_placement_marker_buildable_concrete,
          1,
          1);
      }
      else
      {
        BlitClipTImage2(img_, &rect, draw_at_x, draw_at_y + _OptionsBarHeight, _image_placement_marker_buildable, 1, 1);
      }
      v23 = 1;
LABEL_48:
      ++step_x;
      draw_at_x += 32;
      v24 *= 2;
      image1 = (TImage *)((char *)image1 - 1);
      if ( !image1 )
      {
        break;
      }
      v15 = (int *)pos_y;
    }
    v15 = (int *)(pos_y + 4);
    draw_at_y += 32;
    v12 = v31 == 1;
    ++check_max_y;
    pos_y += 4;
    --v31;
  }
  while ( !v12 );
  result = v23;
  if ( !bool1 )
  {
    result = v19;
  }
  return result;
}
