#include "dune2000.h"
#include "macros/patch.h"

uint8_t GetRadarColorForTile8bit(int xpos, int ypos)
{
  TileFlags tile_flags = gGameMap.map[_CellNumbersWidthSpan[ypos] + xpos].__tile_bitflags;
  uint8_t result;
  if ( tile_flags & TileFlags_10_OCC_BUILDING )
  {
    result = _radarcolor8_sidecolor[tile_flags & 7];
  }
  else if ( tile_flags & (TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE) )
  {
    result = _radarcolor8_spice;
  }
  else if ( tile_flags & TileFlags_10000_SANDY )
  {
    result = _radarcolor8_sand;
  }
  else
  {
    result = _radarcolor8_drive_on;
    if ( !(tile_flags & TileFlags_2000_DRIVE_ON) )
    {
      result = _radarcolor8_impassable;
    }
  }
  return result;
}

uint16_t GetRadarColorForTile16bit(int xpos, int ypos)
{
  TileFlags tile_flags = gGameMap.map[_CellNumbersWidthSpan[ypos] + xpos].__tile_bitflags;
  uint16_t result;
  if ( tile_flags & TileFlags_10_OCC_BUILDING )
  {
    result = _radarcolor16_sidecolor[tile_flags & 7];
  }
  else if ( tile_flags & (TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE) )
  {
    result = _radarcolor16_spice;
  }
  else if ( tile_flags & TileFlags_10000_SANDY )
  {
    result = _radarcolor16_sand;
  }
  else
  {
    result = _radarcolor16_driveon;
    if ( !(tile_flags & TileFlags_2000_DRIVE_ON) )
    {
      result = _radarcolor16_impassable;
    }
  }
  return result;
}

// Custom implementation of function SetPixelOnRadar8
LJMP(0x00462010, _Mod__SetPixelOnRadar8);

void Mod__SetPixelOnRadar8(unsigned char x, unsigned char y)
{
  if ( x >= _RadarMap2->width || y >= _RadarMap2->height )
  {
    DebugFatal("SetPixelOnRadar8", "Invalid coord");
  }
  uint8_t color = GetRadarColorForTile8bit(x, y);
  _RadarMap2->buffer[y * gGameMap.width + x] = color;
  if ( !gGameMap.map[x + _CellNumbersWidthSpan[y]].__shroud )
  {
    _RadarMap1->buffer[y * gGameMap.width + x] = color;
  }
}

// Custom implementation of function SetPixelOnRadar16
LJMP(0x004620B0, _Mod__SetPixelOnRadar16);

void Mod__SetPixelOnRadar16(unsigned char x, unsigned char y)
{
  char *v4; // eax

  if ( x >= _RadarMap2->width || y >= _RadarMap2->height )
  {
    DebugFatal("SetPixelOnRadar16", "Invalid coord");
  }
  uint16_t color = GetRadarColorForTile16bit(x, y);
  *(_WORD *)&_RadarMap2->buffer[2 * (x + y * gGameMap.width)] = color;
  v4 = &_RadarMap1->buffer[2 * (x + y * gGameMap.width)];
  if ( *(_WORD *)v4 != _colormask3 )
  {
    *(_WORD *)v4 = color;
  }
}

void SetPixelOnRadar(unsigned char x, unsigned char y)
{
  if ( gBitsPerPixel == 16 )
    Mod__SetPixelOnRadar16(x, y);
  else
    Mod__SetPixelOnRadar8(x, y);
}

// Custom implementation of function SetupRadarMap
void Mod__SetupRadarMap()
{
  ClearTImage(_RadarMap1, 0, 0);
  char *imagebuffer = _RadarMap2->buffer;
  if ( gBitsPerPixel == 16 )
  {
    for (int ypos = 0; ypos < gGameMap.height; ypos++)
    {
      for (int xpos = 0; xpos < gGameMap.width; xpos++)
      {
        *(_WORD *)imagebuffer = GetRadarColorForTile16bit(xpos, ypos);
        imagebuffer += 2;
      }
    }
  }
  else
  {
    for (int ypos = 0; ypos < gGameMap.height; ypos++)
    {
      for (int xpos = 0; xpos < gGameMap.width; xpos++)
      {
        *imagebuffer = GetRadarColorForTile8bit(xpos, ypos);
        ++imagebuffer;
      }
    }
  }
}
