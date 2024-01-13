#include "dune2000.h"
#include "radar.h"
#include "macros/patch.h"

RadarColorRule radar_color_rules[32];
int radar_color_rules_used;
uint32_t extra_tileflags[800];

CALL(0x00441D7B, _Ext__LoadMapData);
CALL(0x004488B9, _Ext__LoadMapData);

void Ext__LoadMapData(const CHAR *ArgList, char a2)
{
  LoadMapData(ArgList, a2);
  if (!gRestartGame)
  {
    char path[256];
    sprintf(path, "%s.rcl", _BloxFileName);
    LoadRadarColorRulesFromFile(path);
  }
}

void InitDefaultRadarColorRules()
{
  radar_color_rules_used = 4;
  radar_color_rules[0] = (RadarColorRule) {(TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE) * -1, 0,  0, _radarcolor8_spice, _radarcolor16_spice};
  radar_color_rules[1] = (RadarColorRule) {TileFlags_10000_SANDY, 0,                                                        0, _radarcolor8_sand, _radarcolor16_sand};
  radar_color_rules[2] = (RadarColorRule) {0, TileFlags_2000_DRIVE_ON,                                                      0, _radarcolor8_impassable, _radarcolor16_impassable};
  radar_color_rules[3] = (RadarColorRule) {0, 0,                                                                            0, _radarcolor8_drive_on, _radarcolor16_driveon};
}

void LoadRadarColorRulesFromFile(char *filename)
{
  FILE *file = w__OpenFile(filename, "rb", NULL);
  if (!file)
  {
    InitDefaultRadarColorRules();
    return;
  }
  _ReadFile(&radar_color_rules_used, 4, 1, file);
  _ReadFile(radar_color_rules, sizeof(radar_color_rules), 1, file);
  _ReadFile(extra_tileflags, sizeof(extra_tileflags), 1, file);
  CloseFile(file);
  for (int i = 0; i < radar_color_rules_used; i++)
  {
    uint32_t color = radar_color_rules[i].color;
    if (gBitsPerPixel == 16)
      radar_color_rules[i].color_16bit = GetColor16bit(_colormask1, color);
    if (gBitsPerPixel == 8)
      radar_color_rules[i].color_8bit = GetColor8bit(color >> 18 & 63, color >> 10 & 63, color >> 2 & 63, _PalettePtr, 0, 0, 1);
  }
}

int GetRadarColorRule(int xpos, int ypos)
{
  long long tile_flags = gGameMap.map[_CellNumbersWidthSpan[ypos] + xpos].__tile_bitflags;
  tile_flags |= ((long long)extra_tileflags[gGameMap.map[_CellNumbersWidthSpan[ypos] + xpos].back_up_tile_index] << 32);
  int spice_count = (tile_flags >> 20) & 7;
  if (spice_count >= 3)
    tile_flags |= 0x2000000000000;
  else if (spice_count >= 1)
    tile_flags |= 0x1000000000000;
  for (int i = 0; i < radar_color_rules_used; i++)
  {
    RadarColorRule *rule = &radar_color_rules[i];
    if ( ((tile_flags & rule->not_attr) == 0 ) && ((rule->attr < 0 && (tile_flags & (rule->attr * -1)) != 0 ) || (rule->attr >= 0 && (tile_flags & rule->attr) == rule->attr )) )
      return i;
  }
  return radar_color_rules_used - 1;
}

uint8_t GetRadarColorForTile8bit(int xpos, int ypos)
{
  TileFlags tile_flags = gGameMap.map[_CellNumbersWidthSpan[ypos] + xpos].__tile_bitflags;
  if ( tile_flags & TileFlags_10_OCC_BUILDING )
  {
    return _radarcolor8_sidecolor[tile_flags & 7];
  }
  return radar_color_rules[GetRadarColorRule(xpos, ypos)].color_8bit;
}

uint16_t GetRadarColorForTile16bit(int xpos, int ypos)
{
  TileFlags tile_flags = gGameMap.map[_CellNumbersWidthSpan[ypos] + xpos].__tile_bitflags;
  if ( tile_flags & TileFlags_10_OCC_BUILDING )
  {
    return _radarcolor16_sidecolor[tile_flags & 7];
  }
  return radar_color_rules[GetRadarColorRule(xpos, ypos)].color_16bit;
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
