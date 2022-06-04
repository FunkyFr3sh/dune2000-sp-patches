#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"

void GetOwnershipStatusOfCellExt(int x, int y, unsigned char side_id, _BYTE *flags, int building_type)
{
  unsigned int tile_flags; // eax
  (void)building_type;

  if ( x >= 0 && x < gGameMap.width && y >= 0 && y < gGameMap.height )
  {
    tile_flags = gGameMap.map[x + _CellNumbersWidthSpan[y]].__tile_bitflags;
    // New logic - only tile with Build On flag provides build radius
    if ( (tile_flags & (TileFlags_10_OCC_BUILDING | TileFlags_8000_BUILD_ON )) == (TileFlags_10_OCC_BUILDING | TileFlags_8000_BUILD_ON ))
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
                // Call extended version of GetOwnershipStatusOfCell
                GetOwnershipStatusOfCellExt(check_x++, check_y, side_id, &flags, building_type);
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
