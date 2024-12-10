#include "dune2000.h"
#include "utils.h"
#include "radar.h"
#include "macros/patch.h"

RadarColorRule radar_color_rules[32];
int radar_color_rules_used;
uint32_t extra_tileflags[800];
RadarMarker gRadarMarkers[MAX_RADAR_MARKERS];

// Extension wrapper for function LoadMapData
CALL(0x00441D7B, _Ext__LoadMapData); // LoadGame
CALL(0x004488B9, _Ext__LoadMapData); // GameLoop

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
  FILE *file = _OpenFile(filename, "rb", NULL);
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
DETOUR(0x00462010, 0x004620A1, _Mod__SetPixelOnRadar8);

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
DETOUR(0x004620B0, 0x00462138, _Mod__SetPixelOnRadar16);

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
DETOUR(0x00462140, 0x00462281, _Mod__SetupRadarMap);

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

// Custom implementation of function BlitRadar
DETOUR(0x00426000, 0x004263AB, _Mod__BlitRadar);

void Mod__BlitRadar(TImage *img)
{
  int img_height; // ecx
  int v3; // kr04_4
  int draw_offset_x; // ebp
  int img_buffer_pos; // esi
  __int16 *img_buffer_ptr; // edi
  __int16 sidecolor16; // bp
  Unit *unit; // eax MAPDST
  __int16 *target_pixel_ptr; // edx
  char unit_behavior; // cl MAPDST
  char *img_buffer_ptr_; // edi
  char sidecolor_8; // bl
  char *target_pixel_ptr_; // eax
  unsigned __int8 side_id; // [esp+10h] [ebp-24h] MAPDST
  unsigned __int8 unit_x; // [esp+14h] [ebp-20h] MAPDST
  unsigned __int8 unit_y; // [esp+18h] [ebp-1Ch] MAPDST
  int draw_offset_y; // [esp+20h] [ebp-14h]
  RECT rect; // [esp+24h] [ebp-10h]

  img_height = img->height;
  rect.right = img->width;
  rect.left = 0;
  rect.top = 0;
  rect.bottom = img_height;
  if ( _TacticalData.__radarfade )
  {
    BlitTImage(img, _RadarLocationX, _RadarLocationY, _BlankRadarImage, 0, 0, 0);
    _TacticalData.__radarfade = 0;
  }
  v3 = _RadarHouseImages[0]->width - gGameMap.width;
  draw_offset_y = (_RadarHouseImages[0]->height - gGameMap.height) / 2;
  draw_offset_x = v3 / 2;
  img_buffer_pos = _RadarLocationX + v3 / 2 + (_RadarLocationY + draw_offset_y) * img->width;
  BlitTImage(img, v3 / 2 + _RadarLocationX, _RadarLocationY + draw_offset_y, _RadarMap1, 0, 0, 0);
  if ( img->bitdepth == 16 )
  {
    img_buffer_ptr = (__int16 *)&img->buffer[2 * img_buffer_pos];
    side_id = 0;
    if ( _SidesToProcess )
    {
      do
      {
        sidecolor16 = _radarcolor16_sidecolor[side_id];
        for ( unit = GetSide(side_id)->__FirstUnitPtr; unit; unit = unit->Next )
        {
          unit_x = unit->BlockToX;
          unit_y = unit->BlockToY;
          if ( unit_x < gGameMap.width
            && unit_y < gGameMap.height
            && !gGameMap.map[unit_x + _CellNumbersWidthSpan[unit_y]].__shroud
            && (side_id == gSideId || !(unit->Flags & UFLAGS_4_CLOAKED)) )
          {
            target_pixel_ptr = &img_buffer_ptr[unit_x + unit_y * img->width];
            unit_behavior = _templates_unitattribs[unit->Type].__Behavior;
            if ( unit_behavior == UnitBehavior_SANDWORM )
            {
              *target_pixel_ptr = _GreyColor16;
            }
            else if ( !(unit->Flags & UFLAGS_40_FLYING) || unit_behavior == UnitBehavior_ORNITHOPTER )
            {
              *target_pixel_ptr = sidecolor16;
            }
          }
        }
        ++side_id;
      }
      while ( side_id < (unsigned __int8)_SidesToProcess );
      draw_offset_x = v3 / 2;
    }
    DrawRectClipTImage(
      img,
      &rect,
      _RadarLocationX + draw_offset_x + _ViewportXPos / 32,
      _RadarLocationY + draw_offset_y + _ViewportYPos / 32,
      _ViewportWidth / 32 + _RadarLocationX + draw_offset_x + _ViewportXPos / 32 - 1,
      _RadarLocationY + draw_offset_y + _ViewportYPos / 32 + _ViewportHeight / 32,
      _GreenColor16);
    // New logic start
    // Draw radar markers
    for (int i = 0; i < MAX_RADAR_MARKERS; i++)
    {
      RadarMarker *rm = &gRadarMarkers[i];
      if ((int)gGameTicks > rm->max_game_ticks)
        continue;
      int max_radius = ((rm->counter++) >> 2) & 7;
      for (int radius = LLIMIT(max_radius - rm->thickness, 0); radius <= max_radius; radius++)
        for (int angle = 0; angle < 32; angle++)
        {
          int pos_x = rm->x + _sinValues[radius + ((16 - angle) & 31) * 512] / 2048;
          int pos_y = rm->y + _cosValues[radius + ((16 - angle) & 31) * 512] / 2048;
          if (pos_x >= 0 && pos_x < gGameMapWidth && pos_y >= 0 && pos_y < gGameMapHeight)
            img_buffer_ptr[pos_x + pos_y * img->width] = rm->color_16bit;
        }
    }
    // New logic end
  }
  else
  {
    img_buffer_ptr_ = &img->buffer[img_buffer_pos];
    side_id = 0;
    if ( _SidesToProcess )
    {
      do
      {
        sidecolor_8 = _radarcolor8_sidecolor[side_id];
        for ( unit = GetSide(side_id)->__FirstUnitPtr; unit; unit = unit->Next )
        {
          unit_x = unit->BlockToX;
          unit_y = unit->BlockToY;
          if ( unit_x < gGameMap.width
            && unit_y < gGameMap.height
            && !gGameMap.map[unit_x + _CellNumbersWidthSpan[unit_y]].__shroud
            && (side_id == gSideId || !(unit->Flags & UFLAGS_4_CLOAKED)) )
          {
            target_pixel_ptr_ = &img_buffer_ptr_[unit_y * img->width + unit_x];
            unit_behavior = _templates_unitattribs[unit->Type].__Behavior;
            if ( unit_behavior == UnitBehavior_SANDWORM )
            {
              *target_pixel_ptr_ = _GreyColor8;
            }
            else if ( !(unit->Flags & UFLAGS_40_FLYING) || unit_behavior == UnitBehavior_ORNITHOPTER )
            {
              *target_pixel_ptr_ = sidecolor_8;
            }
          }
        }
        ++side_id;
      }
      while ( side_id < (unsigned __int8)_SidesToProcess );
      draw_offset_x = v3 / 2;
    }
    DrawRectClipTImage(
      img,
      &rect,
      _RadarLocationX + draw_offset_x + _ViewportXPos / 32,
      _RadarLocationY + draw_offset_y + _ViewportYPos / 32,
      _ViewportWidth / 32 + _RadarLocationX + draw_offset_x + _ViewportXPos / 32 - 1,
      _RadarLocationY + draw_offset_y + _ViewportYPos / 32 + _ViewportHeight / 32,
      (unsigned __int8)_GreenColor8);
    // New logic start
    // Draw radar markers
    for (int i = 0; i < MAX_RADAR_MARKERS; i++)
    {
      RadarMarker *rm = &gRadarMarkers[i];
      if ((int)gGameTicks > rm->max_game_ticks)
        continue;
      int max_radius = ((rm->counter++) >> 2) & 7;
      for (int radius = LLIMIT(max_radius - rm->thickness, 0); radius <= max_radius; radius++)
        for (int angle = 0; angle < 32; angle++)
        {
          int pos_x = rm->x + _sinValues[radius + ((16 - angle) & 31) * 512] / 2048;
          int pos_y = rm->y + _cosValues[radius + ((16 - angle) & 31) * 512] / 2048;
          if (pos_x >= 0 && pos_x < gGameMapWidth && pos_y >= 0 && pos_y < gGameMapHeight)
            img_buffer_ptr_[pos_x + pos_y * img->width] = rm->color_8bit;
        }
    }
    // New logic end
  }
}
