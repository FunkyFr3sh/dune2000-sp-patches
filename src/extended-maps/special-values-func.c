#include "dune2000.h"

void PlaceStaticCrateExt(uint8_t x, uint8_t y, eCrateType type, eCrateImage image, uint8_t ext_data_field)
{
  int index = GetFreeCrateIndex();
  if ( index != -1 )
  {
    gCrates[index].__is_active = 1;
    gCrates[index].__x = x;
    gCrates[index].__y = y;
    gCrates[index].__timing = INT32_MAX;
    gCrates[index].__type = type;
    gCrates[index].__image = image;
    gCrates[index].__times_to_respawn = 0;
    gCrates[index].ext_data_field = ext_data_field;
    gGameMap.map[x + _CellNumbersWidthSpan[y]].__tile_bitflags |= TileFlags_1000;
  }
}

bool HandleSpecialValue(int special_value, int x, int y)
{
    // Generic crate:    1TTTTIIIEEEEEEEE
    // T = crate type  (0-15)
    // I = crate image (0-7)
    // E = extended data (8 bits)
    if (special_value & 0x8000)
    {
      PlaceStaticCrateExt(x, y, (special_value >> 11) & 15, (special_value >> 8) & 7, special_value & 0x00FF);
      return true;
    }
    // Generic unit:     01-----SSSUUUUUU
    // S = side (0-7)
    // U = unit type (0-63)
    if (special_value & 0x4000)
    {
      // TODO
      return true;
    }
    // Generic building: 001---SSSBBBBBBB
    // S = side (0-7)
    // B = building (0-127)
    if (special_value & 0x2000)
    {
      // TODO
      return true;
    }
    // Standard special values    
    switch (special_value)
    {
        case 6:
        {
            PlaceStaticCrateExt(x, y, CT_CASH, CI_BLUE_CRATE, 0);
            break;
        }
        case 7:
        {
            PlaceStaticCrateExt(x, y, CT_EXPLODE, CI_BLUE_CRATE, 0);
            break;
        }
        case 9:
        {
            PlaceStaticCrateExt(x, y, CT_REVEAL, CI_RED_CRATE, 0);
            break;
        }
        case 10:
        {
            PlaceStaticCrateExt(x, y, CT_NOMAP, CI_RED_CRATE, 0);
            break;
        }
        case 12:
        {
            PlaceStaticCrateExt(x, y, CT_UNIT, CI_GREEN_CRATE, 0);
            break;
        }
        case 13:
        {
            PlaceStaticCrateExt(x, y, CT_STEALTH, CI_PURPLE_CRATE, 0);
            break;
        }
        case 15:
        {
            PlaceStaticCrateExt(x, y, CT_UNSUPPORTED6, CI_PURPLE_CRATE, 0);
            break;
        }
        case 16:
        {
            // Invisible mine
            PlaceStaticCrateExt(x, y, CT_EXPLODE, CI_INVISIBLE, 0);
            break;
        }        
        case 46:
        {
            // Static one-time spice bloom
            PlaceStaticCrateExt(x, y, CT_SPICE_BLOOM_LARGE, CI_LARGE_BLOOM, 0);
            break;
        }
        default:
        {
            // Fall back to game's default behavior for given special value
            return false;
        }
    }
    return true;
}
