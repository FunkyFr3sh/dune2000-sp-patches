#include "macros/patch.h"
#include "dune2000.h"

// Fix MCV can be deployed on unbuildable tile if all surrounding tiles are buildable

// Custom implementation of function CheckIfMCVCanBeDeployedOn

LJMP(0x00443250, _Mod__CheckIfMCVCanBeDeployedOn);

char Mod__CheckIfMCVCanBeDeployedOn(int xpos, int ypos)
{
  int check_y; // esi
  int max_y; // eax
  int *v4; // edx
  int max_x; // edi
  int check_x; // ecx
  TileFlags tile_flags; // eax

  check_y = ypos - 1;
  max_y = ypos + 1;
  if (ypos == 0)
    return 0;
  v4 = &_CellNumbersWidthSpan[check_y];
  max_x = xpos + 1;
  while ( 1 )
  {
    check_x = xpos - 1;
    if ( xpos - 1 <= max_x )
    {
      break;
    }
LABEL_14:
    ++check_y;
    ++v4;
    if ( check_y > max_y )
    {
      return 1;
    }
  }
  while ( check_x >= 0 )
  {
    if ( check_x >= gGameMap.width )
    {
      break;
    }
    if ( check_y >= gGameMap.height )
    {
      break;
    }
    tile_flags = gGameMap.map[*v4 + check_x].__tile_bitflags;
    // New logic start
    if ( !(tile_flags & TileFlags_8000_BUILD_ON)
    // Do not let MCV deploy on tiles with spice AND crate
    // This merges the deploy-mcv-on-crate-fix and spice-on-buildable-tiles
       || (tile_flags & (TileFlags_100000_SPICE|TileFlags_200000_SPICE|TileFlags_400000_SPICE|TileFlags_1000_HAS_CRATE|TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_10_OCC_BUILDING|TileFlags_8_OCC_UNIT)
          && (check_x != xpos || check_y != ypos))
       || (tile_flags & (TileFlags_100000_SPICE|TileFlags_200000_SPICE|TileFlags_400000_SPICE|TileFlags_1000_HAS_CRATE|TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_10_OCC_BUILDING)
          && (check_x == xpos || check_y == ypos)))
    // New logic end
    {
      break;
    }
    if ( ++check_x > max_x )
    {
      max_y = ypos + 1;
      goto LABEL_14;
    }
  }
  return 0;
}
