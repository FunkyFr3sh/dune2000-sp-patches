#include "macros/patch.h"
#include "dune2000.h"
#include "extended-tileset.h"
#include "radar.h"

// Extended array of tile images
TImage *        _images_tiles[4000];
int             num_tileset_tiles;

// Tileset data
TilesetHeader   tileset_header;
uint32_t        _TileBitflags[4000];
uint32_t        extra_tileflags[4000];
int             _TileTooltips[4000];
uint32_t        restrictions[4000];
char            extra_attribute_names[8][32];
int             radar_color_rules_used;
RadarColorRule  radar_color_rules[32];

// LoadMapData - change address where tile attributes are loaded
SETDWORD(0x0044CCD4, __TileBitflags);
SETDWORD(0x0044CCE9, __TileTooltips);

// Extension wrapper for function LoadMapData
CALL(0x00441D7B, _Ext__LoadMapData); // LoadGame
CALL(0x004488B9, _Ext__LoadMapData); // GameLoop

void Ext__LoadMapData(const CHAR *ArgList, char a2)
{
  LoadMapData(ArgList, a2);
  if (!gRestartGame)
  {
    char path[256];
    sprintf(path, "Tilesets\\%s.TLS", _BloxFileName);
    FILE *file = _OpenFile(path, "rb", NULL);
    if (!file)
    {
      InitDefaultRadarColorRules();
      return;
    }
    _ReadFile(&tileset_header,          sizeof(tileset_header),         1, file);
    _ReadFile(_TileBitflags,            sizeof(_TileBitflags),          1, file);
    _ReadFile(extra_tileflags,          sizeof(extra_tileflags),        1, file);
    _ReadFile(_TileTooltips,            sizeof(_TileTooltips),          1, file);
    _ReadFile(restrictions,             sizeof(restrictions),           1, file);
    _ReadFile(extra_attribute_names,    sizeof(extra_attribute_names),  1, file);
    _ReadFile(&radar_color_rules_used,  sizeof(radar_color_rules_used), 1, file);
    _ReadFile(radar_color_rules,        sizeof(radar_color_rules),      1, file);
    CloseFile(file);
    if (!tileset_header.custom_minimap_colors_allowed)
    {
      InitDefaultRadarColorRules();
      return;
    }
    for (int i = 0; i < radar_color_rules_used; i++)
    {
      uint32_t color = radar_color_rules[i].color;
      if (gBitsPerPixel == 16)
        radar_color_rules[i].color_16bit = GetColor16bit(_colormask1, color);
      if (gBitsPerPixel == 8)
        radar_color_rules[i].color_8bit = GetColor8bit(color >> 18 & 63, color >> 10 & 63, color >> 2 & 63, _PalettePtr, 0, 0, 1);
    }
  }
}

// Custom implementation of function LoadTilesetResourceFile
DETOUR(0x00462C80, 0x00462D0B, _Mod__LoadTilesetResourceFile);

void Mod__LoadTilesetResourceFile(char *filename)
{
  FILE *file; // esi
  int fileno; // eax
  int filesize; // edi
  TImage **image_ptr; // esi
  ResourceInfo resource_info;

  file = _OpenFile(filename, "rb", 0);
  if ( !file )
  {
    ReportFileError(filename, 0);
  }
  fileno = __fileno(file);
  filesize = _filelength(fileno);
  _tileset_resource_buffer = Alloc(filesize, 0);
  _ReadFile(_tileset_resource_buffer, 1u, filesize, file);
  CloseFile(file);
  resource_info.address = (int)_tileset_resource_buffer;
  image_ptr = _images_tiles;
  num_tileset_tiles = 0;
  do
  {
    *image_ptr = SetTImageToResource(&resource_info);
    ++image_ptr;
    ++num_tileset_tiles;
  }
  while ( resource_info.address < (int)_tileset_resource_buffer + filesize );
}

// Custom implementation of function BlitTiles
DETOUR(0x00423D20, 0x00423FA8, _Mod__BlitTiles);

void Mod__BlitTiles(TImage *img)
{
  int v1; // esi MAPDST
  int tile_start_y; // edi
  int tile_start_x; // eax
  int cell_index; // edx
  int tile_x_plus_y; // edi
  int draw_y; // ebx
  GameMapTileStruct *tile; // ebp
  int draw_x; // esi
  int viewport_width; // eax
  unsigned char tile_damage; // al
  TileFlags tile_flags; // eax
  int v14; // eax
  int v15; // [esp+10h] [ebp-28h]
  unsigned char crater_degree; // [esp+14h] [ebp-24h]
  unsigned char crater_style; // [esp+18h] [ebp-20h]
  int i; // [esp+20h] [ebp-18h]
  int v20; // [esp+24h] [ebp-14h]
  RECT rect; // [esp+28h] [ebp-10h]

  rect.left = 0;
  v1 = ((_ViewportXPos >> 31) ^ (abs((_BYTE)_ViewportXPos) & 0x1F)) - (_ViewportXPos >> 31);
  rect.right = _ViewportWidth;
  rect.top = _OptionsBarHeight;
  rect.bottom = _ViewportHeight + _OptionsBarHeight;
  v20 = ((_ViewportYPos >> 31) ^ (abs((_BYTE)_ViewportYPos) & 0x1F)) - (_ViewportYPos >> 31);
  tile_start_y = (_ViewportYPos - v20) / 32;
  tile_start_x = (_ViewportXPos - v1) / 32;
  cell_index = tile_start_x + _CellNumbersWidthSpan[tile_start_y];
  tile_x_plus_y = tile_start_x + tile_start_y;
  draw_y = -v20;
  tile = &gGameMap.map[cell_index];
  if ( -v20 < _ViewportHeight - v20 + 32 )
  {
    draw_x = -v1;
    for ( i = draw_x; ; draw_x = i )
    {
      viewport_width = _ViewportWidth;
      if ( draw_x < _ViewportWidth - v1 + 32 )
      {
        v15 = tile_x_plus_y + 16;
        do
        {
          if ( (tile->__shroud & 0xF) != 1 )
          {
            if (tile->__tile_index < num_tileset_tiles)
              BlitTImageCopy(
                img,
                &rect,
                draw_x,
                draw_y + _OptionsBarHeight,
                _images_tiles[(unsigned short)tile->__tile_index]);
            else
              DebugFatal("BlitTiles", "Tile has index %d, but tileset has %d tiles", tile->__tile_index, num_tileset_tiles);
            tile_damage = tile->__damage;
            if ( tile_damage > 16u )
            {
              crater_degree = tile_damage >> 4;
              crater_style = 16 * (tile_x_plus_y % 2);
              tile_flags = tile->__tile_bitflags;
              if ( !(tile_flags & TileFlags_8000_BUILD_ON) )
              {
                if ( tile_flags & TileFlags_10000_SANDY )
                {
                  if (!tileset_header.rule_do_not_draw_sand_craters)
                    BlitClipTImage2(
                      img,
                      &rect,
                      draw_x
                    + _crater_draw_offsets_x[((tile_x_plus_y >> 31) ^ (abs((_BYTE)tile_x_plus_y) & 0xF))
                                           - (tile_x_plus_y >> 31)],
                      _OptionsBarHeight
                    + draw_y
                    + _crater_draw_offsets_y[((v15 >> 31) ^ (abs((_BYTE)v15) & 0xF)) - (v15 >> 31)],
                      _images_crater[crater_style + crater_degree][1],
                      1,
                      1);
                }
              }
              else
              {
                if (!tileset_header.rule_do_not_draw_rock_craters)
                  BlitClipTImage2(
                    img,
                    &rect,
                    draw_x
                  + _crater_draw_offsets_x[((tile_x_plus_y >> 31) ^ (abs((_BYTE)tile_x_plus_y) & 0xF))
                                         - (tile_x_plus_y >> 31)],
                    _OptionsBarHeight
                  + draw_y
                  + _crater_draw_offsets_y[((v15 >> 31) ^ (abs((_BYTE)v15) & 0xF)) - (v15 >> 31)],
                    _images_crater[crater_style + crater_degree][0],
                    1,
                    1);
              }
            }
          }
          viewport_width = _ViewportWidth;
          ++tile_x_plus_y;
          ++v15;
          draw_x += 32;
          ++tile;
        }
        while ( draw_x < _ViewportWidth - v1 + 32 );
      }
      draw_y += 32;
      v14 = viewport_width / 32;
      tile_x_plus_y -= v14;
      tile += _CellNumbersWidthSpan[1] - v14 - 1;
      if ( draw_y >= _ViewportHeight - v20 + 32 )
      {
        break;
      }
    }
  }
}
